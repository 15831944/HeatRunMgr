#include <Windows.h>
#include <iostream>
#include "ThreadProcess.h"
#include "TimerSoft.h"
#include "Global.h"
#include "Usleep.h"
#include "HeatRunDataFileMgr.h"
#include "PJOperation.h"
#include "ThreadSocketRS.h"
using namespace std;


DWORD WINAPI ThreadSystemTimerProcess(LPVOID lpParmeter)
{
	//dwStartTick代表起始时间   dwNowTick代表当前获取时间 
	//当两者 nNowTick - nStartTick = 1000，表示1秒到时
	BYTE i;
	DWORD dwStartTick,dwNowTick;
	dwNowTick = GetTickCount();
	dwStartTick = dwNowTick;
	while (true)
	{
		dwNowTick = GetTickCount();
		//系统计数1秒(1000)到时,从新点位起始时间
		if ( (dwNowTick-dwStartTick) >= NUM_SYSTEMTICK )
		{
			//心跳带动功能(重点)
			TimerSoft_Tick(g_pSmpDataMgrListHead);
			dwStartTick = dwNowTick;

			for (i=0;i<NUM_PJ;i++)//add by ly at 2015/12/16 9:23 做堵塞的处理，
			{
				if (Clinet[i].SendTimeOut>0)
				{
					Clinet[i].SendTimeOut--;
					if (Clinet[i].SendTimeOut==0)//socket 堵塞超时，则断开socket，重新连接
					{
						if (Clinet[i].SendFlag==1)
						{
							Clinet[i].SendFlag=0;
							FD_CLR(Clinet[i].SocketId,&clientfdset);
							closesocket(Clinet[i].SocketId);
							Clinet[i].ConnectStatus=Start;
							Clinet[i].SocketId=0;
#if defined(DEBUG_RS232)
							UART_Printf("Clear potr=%d",i);
#endif
						}
					}
					
				}
				if (Clinet[i].ConnectStatus==Start)
				{
					if (i>0)
					{
						pSmpNode NodeTemp;
						PJDATA_MGR *PjDataTemp;
						NodeTemp = g_pSmpDataMgrListHead;
						while(NodeTemp)
						{
							PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
							if (i == PjDataTemp->m_Option.ClinetID)
							{
								PjDataTemp->m_HrDataGet.OnlineStatus = 0x00;
								break;
							}
							NodeTemp = NodeTemp->next;
						}
					}
				}
				else if(Clinet[i].ConnectStatus==Connectted)
				{
					if (i>0)//20151211 10：20 在线状态
					{

						pSmpNode NodeTemp;
						PJDATA_MGR *PjDataTemp;
						NodeTemp = g_pSmpDataMgrListHead;
						while(NodeTemp)
						{
							PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
							if (i == PjDataTemp->m_Option.ClinetID)
							{
								PjDataTemp->m_HrDataGet.OnlineStatus = 0x01;
								break;
							}
							NodeTemp = NodeTemp->next;
						}
					}
				}
			}

		}
		usleep(1);
	}
	return 0;
}





//本线程完成两个工作：1.将老化数据定时写入文件  2.将UI需要的老化历史数据读取到历史数据发送缓冲区中
//将这两个工作何在一起，好处在于：即可达到互斥，又能在机制上做到时间块不重叠
DWORD WINAPI ThreadRWMgrFileDataProcess(LPVOID lpParmeter)
{
#if defined(DEBUG_CLOSE_SAVEDATA)
	//如果关闭记录老化数据宏开关enable，则不做记录数据工作
	Sleep(10);
	return 0;
#endif // DEBUG_CLOSE_SAVEDATA
#if defined(DEBUG_DOS)
	static int iCount = 0;
#endif // DEBUG_DOS
	//dwStartTick代表起始时间   dwNowTick代表当前获取时间 
	//当两者 nNowTick - nStartTick = 1000，表示1秒到时
	DWORD dwStartTick,dwNowTick;
	dwNowTick = GetTickCount();
	dwStartTick = dwNowTick;
	while (true)
	{
		//1.将老化数据定时写入文件
		dwNowTick = GetTickCount();
		//系统计数1秒(1000)到时,从新点位起始时间
		if ( (dwNowTick-dwStartTick) >= NUM_SAVEHRDATATICK )
		{
#if defined(DEBUG_DOS)
			printf("ThreadRWMgrFileDataProcess %d enter\n",iCount++);
#endif // DEBUG_DOS
			//定时写链表管理数据内容到文件中
			SaveHrDataToFile(g_pSmpDataMgrListHead);
			dwStartTick = dwNowTick;
		}
		Sleep(10);

		//2.将UI需要的老化历史数据读取到历史数据发送缓冲区中
		if( g_pHisData->bUIQueryHisData )
		{
			GetFileNameFromDir( g_pHisData );
			GetHRDataFromFile( g_pHisData );
			//这个时候立刻清除UI获取历史数据状态，防止重新进入
			g_pHisData->bUIQueryHisData = false;
		}
		if (ServerPC.MutexSend == false)
		{
			if(g_pHisData->bSendFinish == true)
			{
				ServerPC.MutexSend = true;
				Sleep(10);//睡眠10ms保证Socket先加锁发送
				if (0 == ServerPC.SendCntLock)
				{
					 ServerPC.SendCntLock = 1;

					 if (g_pHisData->nSumHrData==0)//查找空数据时
					 {
						 PackData(&ServerPC.SendData[ServerPC.SendDataCnt], g_pHisData->pHrData[g_pHisData->nSendToUIIndex], 0x83, g_pHisData->nLenFinalData);
						 ServerPC.SendDataCnt += (g_pHisData->nLenFinalData+6);



#if defined(DEBUG_DOS)
						 printf("OS->UI:Emply");
						 printf("\n");
#endif
						 g_pHisData->bSendFinish = false;
						 g_pHisData->nSendToUIIndex=0;
					 }
					 else
					 {

						 if (g_pHisData->nSendToUIIndex == g_pHisData->nSumHrData-1)
						 {

							 PackData(&ServerPC.SendData[ServerPC.SendDataCnt], g_pHisData->pHrData[g_pHisData->nSendToUIIndex], 0x84, g_pHisData->nLenFinalData);
							 /*
							
#if defined(DEBUG_DOS)
							 printf("OS->UI:");
							 for (int i=0; i< g_pHisData->nLenFinalData+6; i++)
							 {
								 printf("%02hX ", ServerPC.SendData[i]);
							 }
							 printf("\n");
#endif
							 */
							 
							 ServerPC.SendDataCnt += (g_pHisData->nLenFinalData+6);

						 }
						 else
						 {


							 PackData(&ServerPC.SendData[ServerPC.SendDataCnt], g_pHisData->pHrData[g_pHisData->nSendToUIIndex], 0x84, g_pHisData->nLenCommonData);
							/*

#if defined(DEBUG_DOS)
							 printf("OS->UI:");
							 for (int i=0; i< g_pHisData->nLenCommonData+6; i++)
							 {
								 printf("%02hX ", ServerPC.SendData[i]);
							 }
							 printf("\n");
#endif
							 */
							 ServerPC.SendDataCnt += (g_pHisData->nLenCommonData+6);

						 }
						 g_pHisData->nSendToUIIndex++;
						 if (g_pHisData->nSendToUIIndex == g_pHisData->nSumHrData)
						 {
							 g_pHisData->bSendFinish = false;
						 }
					 }
	
				}
				ServerPC.SendCntLock = 0;
			}
			ServerPC.MutexSend = false;
		}
	}
	return 0;
}

DWORD WINAPI ThreadHrDataProcess(LPVOID lpParmeter)
{
	REFRESH_DATA RefreshData;
	memset(&RefreshData, 0, sizeof(RefreshData));
	RefreshData.Finish_Flag = false;
	RefreshData.IdDataPack = 1;
	RefreshData.nCountPack = 0;
	while (1)
	{
		if (ServerPC.MutexSend == false)
		{
			if (Refresh_Flag == true)
			{
				ServerPC.MutexSend = true;
				if (0 == ServerPC.SendCntLock)
				{
					ServerPC.SendCntLock = 1;


					RefreshData = HrDataProcess(RefreshData);
					if (RefreshData.Finish_Flag == false)
					{
						//ServerPC.SendCntLock = 1;
						PackData(&ServerPC.SendData[ServerPC.SendDataCnt], RefreshData.PJData, 0x81, sizeof(RefreshData.PJData));

#if defined(DEBUG_DOS)
						printf("OS->UI:");
						for (int i=0; i< (ServerPC.SendDataCnt+sizeof(RefreshData.PJData)+6); i++)
						{
							printf("%02hX ", ServerPC.SendData[i]);
						}
						printf("\n");
#endif
						ServerPC.SendDataCnt += (sizeof(RefreshData.PJData)+6);
						//ServerPC.SendCntLock = 0;

					}
					else
					{
						//FirmwareToUI();
						//ServerPC.SendCntLock = 1;
						PackData(&ServerPC.SendData[ServerPC.SendDataCnt], strHrMgrVersion, 0x07, strlen(strHrMgrVersion));
						ServerPC.SendDataCnt += (strlen(strHrMgrVersion)+6);
						//ServerPC.SendCntLock = 0;


						memset(&RefreshData, 0, sizeof(RefreshData));
						RefreshData.Finish_Flag = false;
						RefreshData.IdDataPack = 1;
						RefreshData.nCountPack = 0;
						Refresh_Flag = false;
					}


					ServerPC.SendCntLock = 0;


				}
				
				ServerPC.MutexSend = false;
			}
		}	
		Sleep(1);
	}
	
}

