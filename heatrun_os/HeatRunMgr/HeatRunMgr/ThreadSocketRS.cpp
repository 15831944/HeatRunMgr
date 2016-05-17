/***********************************************
  �ͻ���
***********************************************/
#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include <Windows.h>
#include "Global.h"
#include "Usleep.h"
#include "ThreadSocketRS.h"
#include "TimerSoftProcess.h"
#include "ConstData.h"
//#include "UserMesInfo.h"
#include "PJOperation.h"
#include "SystemInit.h"
#include "HeatRunDataFileMgr.h"

#pragma comment(lib,"ws2_32.lib")


// #define BUS_GLOBALS
 using namespace std;

// BOOL isLogin = false;
// 
// WORD socket1;
// 
// WORD socket2;
// 
// char nummax;
// 
// WORD socket3;
// 
// DWORD socketnum[50];
int HeatRunTime=0;
int HeatRunPoint;
SOCKETADDINFO Clinet[100];
// 
SOCKETADDINFO ServerPC;

int count;
SOCKETADDINFO ClinetAddInfo;
SOCKADDR_IN addrServer;
SOCKET serverSock;
int NUM_PJ = 1;


// char GetPowerStatus[13]={0xBE,0xEF,0x03,0x06,0x00,0x19,0xD3,0x02,0x00,0x00,0x60,0x00,0x00};
// char SetPowerOn[13]={0xBE,0xEF,0x03,0x06,0x00,0xBA,0xD2,0x01,0x00,0x00,0x60,0x01,0x00};
// char SetPowerOff[13]={0xBE,0xEF,0x03,0x06,0x00,0x2A,0xD3,0x01,0x00,0x00,0x60,0x00,0x00};
// char GetLempTime[13]={0xBE,0xEF,0x03,0x06,0x00,0xC2,0xFF,0x02,0x00,0x90,0x10,0x00,0x00};
// char FactoryMode[13]={0xBE,0xEF,0x03,0x06,0x00,0xA3,0x67,0x01,0x00,0x69,0x68,0x63,0x74};
// char WhitePlane[13]={0xBE,0xEF,0x03,0x06,0x00,0x00,0xFE,0x01,0x00,0x00,0x15,0x53,0x00};
// char PatternMode[13]={0xBE,0xEF,0x03,0x06,0x00,0x7B,0xC6,0X06,0x00,0x11,0x15,0x00,0x00};

string charToString(char temp[])
{
    string tempStr = "";
    for(int i = 0; i < sizeof(temp); i++)
        tempStr += temp[i];
    return tempStr;
}

DWORD WINAPI ThreadProSocketSendData(LPVOID lpParameter)
{
	char i;

	while(1)
	{
		
		if (Clinet[1].DebugFlag2==1)
		{
			Clinet[1].DebugFlag2=0;
		}
		
		for (i=0;i<NUM_PJ;i++)//modified by ly at 18:00    ɾ�����Դ���0
		{
			if ((Clinet[i].ConnectStatus==Connectted)&&(Clinet[i].SendFlag==0)&&(Clinet[i].SendDataCnt>0))//modified ly  at 151110 10:40
			{
					if (i==0)
					{
						send(Clinet[i].SocketId, Clinet[i].SendData,Clinet[i].SendDataCnt, 0);
						Clinet[i].SendDataCnt=0;
					}
					else
					{
						send(Clinet[i].SocketId, Clinet[i].SendData,13, 0);
#if defined(DEBUG_RS232)
						UART_Printf("sendcline[i]=%d,ip=%d\n",i,Clinet[i].sin_addr.S_un.S_un_b.s_b4);
#endif
						/*
						if (Clinet[i].SendDataCnt>=13)
						{
							memmove(&Clinet[i].SendData[0],&Clinet[i].SendData[13],Clinet[i].SendDataCnt-13);
							Clinet[i].SendDataCnt=Clinet[i].SendDataCnt-13;
						}
						UART_Printf("cline[i]=%d,cnt=%d:",i,Clinet[i].SendDataCnt);
						*/
						//UART_Printf("socketid=%d",clientSock[i]);
					}					
					Clinet[i].SendFlag=1;//����	
					Clinet[i].SendTimeOut=2;//���ö���ʱ��

			}

		}



		if ((ServerPC.SendFlag==0)&&(ServerPC.ConnectStatus==Connectted))
		{
			if (0 == ServerPC.SendCntLock)
			{
				ServerPC.SendCntLock = 1;
				if (ServerPC.SendDataCnt>0)
				{
					send(ServerPC.SocketId, ServerPC.SendData,ServerPC.SendDataCnt, 0);
					


					send(Clinet[0].SocketId, ServerPC.SendData,ServerPC.SendDataCnt, 0);

					UART_Printf("ServerPC.SendDataCnt=%d",ServerPC.SendDataCnt);
#if defined(DEBUG_RS232)
					UART_Printf("Serverid=%d,Server port=%d\n",ServerPC.SocketId,ServerPC.ClinetHtons);
#endif

					//				ServerPC.SendFlag=1;//����	

					ServerPC.SendDataCnt=0;
					
				}
				ServerPC.SendCntLock = 0;
			}
			
		}
		usleep(1);
	}
	return 0;
}




//���߳����2��������1.���ñ����˿�Ϊ9906��socket��Ϊ�������ˣ����ڵȴ�С�޵�socket���ӣ�2.����N+1���ͻ������Ӽ�Cline[n]
//��n=0ʱΪ�����������ӡ���ݣ�n>=1ʱ���Ǻ�PJ�������ӡ�n=N+1.NΪ����PJ�ĸ�����
DWORD WINAPI ThreadProSocketConnect(LPVOID lpParameter)
{
	char i;
	SOCKET clientSock[10];
	SOCKADDR_IN addrClient[100];//modified by ly at 2015/12/14 9:53 ԭ����10

	int ret;

	const char yes =1;


	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
#if defined(DEBUG_RS232)
		UART_Printf("WSAStartup failed with error: %d\n", err);
#endif
		return 1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		// Tell the user that we could not find a usable //
		// WinSock DLL.   
#if defined(DEBUG_RS232)
		UART_Printf("Could not find a usable version of Winsock.dll\n");
#endif
		WSACleanup();
		return 1;
	}
	else
	{
#if defined(DEBUG_RS232)
		UART_Printf("The Winsock 2.2 dll was found okay\n");
#endif
	}//���socket�ĳ�ʼ����������������ϵͳ��socket ��dll�ļ�

	char szHost[256];
	memset(szHost,0,256);
	gethostname(szHost,256);
	HOSTENT *pHost = gethostbyname(szHost);
	szHost[0]=pHost->h_addr_list[0][0];
	szHost[1]=pHost->h_addr_list[0][1];
	szHost[2]=pHost->h_addr_list[0][2];
	szHost[3]=pHost->h_addr_list[0][3];

	Clinet[0].sin_addr.S_un.S_un_b.s_b1 = szHost[0];
	Clinet[0].sin_addr.S_un.S_un_b.s_b2 = szHost[1];
	Clinet[0].sin_addr.S_un.S_un_b.s_b3 = szHost[2];
	Clinet[0].sin_addr.S_un.S_un_b.s_b4 = szHost[3];
	//��ȡ������IP��ַ��Ϊ��֮�󴴽��ͻ�������[0]�����ӣ������ѯ���ݡ�


	//��������1���������������ӣ���С�޵����ӣ������ö˿�Ϊ9906����������IP���˿�Ϊ9906��socket���ӡ�

	if ((serverSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)  
	{  
		perror("socket");  
		return 0;  
	}  
	if (setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)  
	{  
		perror("setsockopt");  
		return 0;  
	}  
	addrServer.sin_family = AF_INET;        
	addrServer.sin_port = htons(9906);    
	addrServer.sin_addr.s_addr = INADDR_ANY;  
	memset(addrServer.sin_zero, '\0', sizeof(addrServer.sin_zero));  
	if (bind(serverSock, (struct sockaddr *)&addrServer, sizeof(addrServer)) == -1)  
	{  
		perror("bind");  
		return 0;  
	}  
	if (listen(serverSock, 5) == -1)  
	{  
		perror("listen");  
		return 0;  
	}  
	//

	while(1)
	{
		//��������2������Clinet[n]�����ӡ�
		for(i=0;i<NUM_PJ;i++)    //20160201 ɾ�����Դ���0
		{
			if((Clinet[i].ConnectStatus==Start))
			{
				if (Clinet[i].SocketId==0)
				{
					clientSock[i] = socket(AF_INET, SOCK_STREAM, 0);
					if(clientSock[i] == -1)
					{
						perror("socket��������"); 

					}
					else
					{
#if defined(DEBUG_RS232)
						UART_Printf("i=%d  ",i);
						UART_Printf("socketid=%d",clientSock[i]);
#endif
					}
					Clinet[i].SocketId=clientSock[i];
				}

				addrClient[i].sin_addr = Clinet[i].sin_addr;
				addrClient[i].sin_family = AF_INET;
				addrClient[i].sin_port = Clinet[i].ClinetHtons;
//				if (Clinet[i].ConnectTime==0)
				if (addrClient[i].sin_port==0)//modified by ly at 2015/12/14 9:53 ������࣬������IP����������󣬲�������
				{
				}
				else
				{
					ret = connect(Clinet[i].SocketId, (sockaddr*)&addrClient[i], sizeof(SOCKADDR));
					if (ret)
					{
#if defined(DEBUG_RS232)
						UART_Printf("connect fail=%d,ip=%d\r\n",i,Clinet[i].sin_addr.S_un.S_un_b.s_b4);
#endif
						Clinet[i].ConnectCnt++;
						if (Clinet[i].ConnectCnt>60)
						{
#if defined(DEBUG_RS232)
							UART_Printf("connect error=%d\n",i);
#endif
							
							if (i==0)//modified by ly at 2015/12/14 9:53 �����ӡ��Ϣ��û���򿪣���������ݣ�֮��Ͳ�������
							{
//								memset(&Clinet[0],0,sizeof(Clinet[0]));
								Clinet[0].ClinetHtons = 0;
							
							}
							
							Clinet[i].ConnectCnt=0;
						}
						
					}
					else
					{
#if defined(DEBUG_RS232)
						UART_Printf("connect ok=%d,ip=%d\n",i,Clinet[i].sin_addr.S_un.S_un_b.s_b4);
#endif
						#if defined(DEBUG_RS232)
						UART_Printf("ip=%x  \n",Clinet[i].sin_addr);
						#endif
												
						Clinet[i].ConnectCnt=0;
						if (Clinet[i].DebugFlag1==1)
						{
							Clinet[i].DebugFlag1=0;
							Clinet[i].DebugFlag2=1;
						}
						Clinet[i].ConnectStatus=Connectted;
					}
				}
			}
		}
		usleep(1);
	}

	return 0;
}
//void OnTimer(UINT nTimerid)

VOID WINAPI ProcTimer(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{		
	/*
	count++; 
	Clinet[1].SendData[Clinet[1].SendDataCnt++]=count;
	Clinet[1].SendFlag=1;//Ҫ��������
	*/
// 	if (HeatRunTime)
// 	{
// 		HeatRunTime--;
// 	}
// 	printf("time=%d",HeatRunTime);
// 	HeatRunOpeartion(0);
}

DWORD WINAPI   TimeThread(PVOID   pvoid)   
{   
	MSG  msg;   
	PeekMessage(&msg,NULL,WM_USER,WM_USER,PM_NOREMOVE);   
	UINT  timerid=SetTimer(NULL,111,1000,ProcTimer);   
	BOOL  bRet;   

	while(   (bRet = GetMessage(&msg,NULL,0,0))!=   0)   
	{     
		if(bRet==-1)   
		{   
			//   handle   the   error   and   possibly   exit   
		}   
		else   
		{    
			TranslateMessage(&msg);     
			DispatchMessage(&msg);     
		}   
	}   
	KillTimer(NULL,timerid);   
	printf("thread   end   here\n");   
	return   0;   
}
/*
VOID WINAPI TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
{
	int i;
	/*
	switch(nTimerid)
	{
		case 1:
			for (i=0;i<3;i++)
			{
				if ((Clinet[i].SendFlag==0)&&(Clinet[i].ConnectStatus=Connectted))
				{
					Clinet[i].SendFlag=1;
					if (i==0)
					{
						Clinet[i].SendData[0]=0xbd;
						Clinet[i].SendData[1]=0x11;
						Clinet[i].SendData[2]=0x22;
					}
					else if (i==1)
					{
						Clinet[i].SendData[0]=0x55;
						Clinet[i].SendData[1]=0x44;
						Clinet[i].SendData[2]=0x22;
					}
				}
			}
			break;
	}
	
	
	printf("shijian\n");

}
*/
//����
char TempBuf[200];
char TempBufCnt;

//���߳�������¹�����
//1.����socket��I/O����ģʽ
//2.����Clinet[n]��״̬���������߼�⡢�������ݺ󻺳����Դ���ա��������ݵĴ���
//3.����UI��socket��״̬���������߼�⡢�������ݺ󻺳����Դ���ա��������ݵĴ���

DWORD WINAPI ThreadManagentSocketStatus(LPVOID lpParameter)
{
		BYTE i,j,k;
		char buf[200];
		char serverbuf[200];
		
		/***********/
		int server_socket=0;
		int new_socket;
//		fd_set clientfdset;  
		struct timeval tv;  
		int length;
		WORD ii;

		SOCKADDR_IN addrClient2;

		int ret;

		int conn_num=0;

		while(1)
		{
			
			k=0;
			for (i=0;i<NUM_PJ;i++)//modified by ly at 18:00//k++ ���ж�������Ч��socket�����Ӳ��к������    20160201 ɾ�����Կ�0
			{
				if (Clinet[i].ConnectStatus==Connectted)
				{
					k++;
				}
			}
			if (ServerPC.ConnectStatus == Connectted)
			{
				k++;
			}
			//k=1;//test
			if (serverSock)
			{
				k++;
			}
			if (k)
			{
				//��������1������socket��I/0ģʽ
				FD_ZERO(&clientfdset);//���������Ա
				FD_SET(serverSock,&clientfdset);
				tv.tv_sec = 5;//�޸�Ϊ0s������һֱ������socket�Ľ���
				tv.tv_usec = 0;
				for (i=0;i<NUM_PJ;i++)//modified by ly at 18:00    20160201 ɾ�����Դ���0
				{
					if (Clinet[i].SocketId>0)
					{
						FD_SET(Clinet[i].SocketId,&clientfdset);//����Ч��Clinet��ID���뵽������
					}				
				}
				if (ServerPC.SocketId>0)
				{
					FD_SET(ServerPC.SocketId,&clientfdset);
				}
				//			FD_SET(socket2, &clientfdset); 

				ret = select(0, &clientfdset, NULL, NULL,&tv);//&tv);  
				if (ret < 0)  
				{  
					//				perror("select"); 
					//				break;
#if defined(DEBUG_RS232)
					UART_Printf("select error\n");
#endif
					ii=WSAGetLastError();
#if defined(DEBUG_RS232)
					UART_Printf("select error code=%d\n",ii);
#endif
					continue;
				}  
				else if (ret == 0)  
				{  
//					UART_Printf("waitting timeout\n");
					continue;  
				}  
				// check every fd in the set  
				//��������2������Clinet��״̬
				for (i = 0; i < NUM_PJ; i++)  //modified by ly at 18:00   20160201 ɾ�����Դ���0
				{  
					if (FD_ISSET(Clinet[i].SocketId, &clientfdset))  
					{  
						ret = recv(Clinet[i].SocketId, buf, sizeof(buf), 0);  
						//2.1�������⣬������socket�����쳣����socket���и�λ�����ID������״̬
						if (ret <= 0)  
						{       
#if defined(DEBUG_RS232)
							UART_Printf("client[%d] close,ip=%d\r\n", i,Clinet[i].sin_addr.S_un.S_un_b.s_b4);  
							UART_Printf("WTF\n");
#endif
							ii=WSAGetLastError();
#if defined(DEBUG_RS232)
							UART_Printf("rece error code=%d\n",ii);
#endif						//add by ly at 20151211 9:17
							//				close(socketnum[i]);  
							FD_CLR(Clinet[i].SocketId, &clientfdset);  
							Clinet[i].SocketId = 0; 
							Clinet[i].ConnectStatus=Start;
							Clinet[i].SendFlag=0;//	����
							Clinet[i].DebugFlag1=1;
						}  
						else  
						{    
							//printf("Client[%d] msg:%s\n", i, buf); 

							for (j=0;j<ret;j++)
							{
//								printf("msg:%x\n",Clinet[i].ReceData[j]);
								Clinet[i].ReceData[Clinet[i].ReceDataCnt++]=buf[j];
							}
							/*
							for (j=0;j<Clinet[i].ReceDataCnt;j++)
							{
								UART_Printf("msg:%x\n",Clinet[i].ReceData[j]);
							}*/
							Clinet[i].ReceFlag=1;
							if (Clinet[i].PowerOnFlag==1)
							{
								Clinet[i].PowerOnFlag=0;
							}
							
							
//							if (Clinet[i].SendFlag==1)
							{
								//	����//modified by ly at 18:00 ���Զ�PJ������һ�����ݣ������ðѷ��ͻ����е��������һ��
								Clinet[i].SendFlag=0;

								//2.2����ղ��Ѿ�����������
								if (Clinet[i].SendCntLock==0)
								{
									Clinet[i].SendCntLock=1;
									if (Clinet[i].SendDataCnt>=13)
									{
										memmove(&Clinet[i].SendData[0],&Clinet[i].SendData[13],Clinet[i].SendDataCnt-13);
										Clinet[i].SendDataCnt=Clinet[i].SendDataCnt-13;
									}
									else
									{
										Clinet[i].SendDataCnt=0;
									}
									#if defined(DEBUG_RS232)
									UART_Printf("cline[i]=%d,cnt=%d,ip=%d\n",i,Clinet[i].SendDataCnt,Clinet[i].sin_addr.S_un.S_un_b.s_b4);
									#endif
									Clinet[i].SendCntLock=0;
								}
								
							}
							//2.3����Get���� PJ�����̣����Ͳ�ѯ���ݺ󣬵ȵ���һ�����浽����GetSearchFlag=1����ʱ���Խ���PJ�����ݴ���
							if (Clinet[i].GetSearchFlag==1)
							{
#if defined(DEBUG_RS232)
								UART_Printf("process PJ data=%d,ip=%d\r\n",i,Clinet[i].sin_addr.S_un.S_un_b.s_b4);
#endif
								Clinet[i].GetStartFlag=0;
								Clinet[i].GetSearchFlag=0;
								Clinet[i].GetEndFlag = true;
//								Clinet[i].GetStartFlag=0;
								//								memmove(PJData[i],Clinet[i].ReceData,Clinet[i].ReceDataCnt);
								//
								//����PJ��ѯ��������06 06 06 �����յ�һ������1A�����ݱ�ʾ����ѯ���ݵ��ˡ�ProcessParmJ������ƫ����
								//�����������⣺��������һ����ѯ���̣�ǰ����10���06��֮����ǲ�ѯ���ݡ�
								for (Clinet[i].ProcessParmJ=0;Clinet[i].ReceData[Clinet[i].ProcessParmJ]<0x1a;Clinet[i].ProcessParmJ++)
								{
									if (Clinet[i].ProcessParmJ>250)
									{
										break;
									}//ADD BY LY AT 20151210
								}
								if (Clinet[i].ReceDataCnt>Clinet[i].ProcessParmJ)//ADD BY LY AT 20151210
								{
									memmove(Clinet[i].TempBuff,&Clinet[i].ReceData[Clinet[i].ProcessParmJ],Clinet[i].ReceDataCnt-Clinet[i].ProcessParmJ);//���06��ֵ
									Clinet[i].TempBuffCnt	=Clinet[i].ReceDataCnt-Clinet[i].ProcessParmJ;
								}
						
								//ADD BY LY AT 20151210//���˵�06�����ݺ������һ���ֽڲ���0x1d �͹������ݡ�
								if (Clinet[i].TempBuff[0]==0x1d)	
								{
									DataProcessFormPJ(i);
								}
#if defined(DEBUG_RS232)
								UART_Printf("PJ data ok=%d,ip=%d\r\n",i,Clinet[i].sin_addr.S_un.S_un_b.s_b4);
#endif
								Clinet[i].ReceDataCnt=0;//������ݣ����¿�ʼһ��pattern
							}
							
							//process data //������Ϣ��������
							if (Clinet[0].ReceFlag==1)
							{
								if ((Clinet[0].ReceData[0]==0x1a)&&(Clinet[0].ReceData[1]=0x2b))
								{
									switch(Clinet[0].ReceData[2])
									{
										case 0x3c:
											for (i=0;i<13;i++)
											{
												Clinet[1].SendData[Clinet[1].SendDataCnt++]=GetPowerStatus[i];
											}
//											Clinet[1].SendFlag=1;											
											break;
										case 0x4d:
											for (i=0;i<13;i++)
											{
												Clinet[1].SendData[Clinet[1].SendDataCnt++]=GetLamp1TimeLow[i];  //ccq
											}
//											Clinet[1].SendFlag=1;											
											break;
										case 0x5e:
											HeatRunTime=1;
											HeatRunPoint=HeatRunStart;
											break;
										case 0x6f:
											break;
									}

									Clinet[0].ReceFlag=0;
									Clinet[0].ReceDataCnt=0;
								}
							}
							
							
						}  
						
//						Clinet[i].ReceDataCnt=0;
					}  
				}  
				//��������3�����������������Ǹ�socket
				length=sizeof(SOCKADDR);
				if (FD_ISSET(serverSock, &clientfdset))  
				{  
					if (ServerPC.SocketId==0)//ֻ�з������Ͽ����Ӻ�Ž����µ����� add by ly  at 20151218 8:22
					{
						new_socket = accept(serverSock, (struct sockaddr *)&addrClient2,&length);  
						if (new_socket <= 0)  
						{  
							perror("accept");  
							ii=WSAGetLastError();
#if defined(DEBUG_RS232)
							UART_Printf("new socket error code=%d\n",ii);
#endif
							continue;  
						} 
						else
						{
#if defined(DEBUG_RS232)
							//��ӡ���ӵ���ϸ��Ϣ
							UART_Printf("new client[%d] %s:%d\n", conn_num,inet_ntoa(addrClient2.sin_addr), ntohs(addrClient2.sin_port)); 
#endif
							ServerPC.ClinetHtons = ntohs(addrClient2.sin_port);
							ServerPC.sin_addr = addrClient2.sin_addr;
							ServerPC.SocketId = new_socket;
							ServerPC.ConnectStatus = Connectted;

							//						send(new_socket, "fafa", 4, 0);
						}
					}
					
				}
				if (FD_ISSET(ServerPC.SocketId, &clientfdset))
				{
					ret = recv(ServerPC.SocketId,serverbuf, sizeof(serverbuf), 0);  
					if (ret <= 0)  
					{       
						ii=WSAGetLastError();
#if defined(DEBUG_RS232)
						UART_Printf("server error code=%d\n",ii);
						UART_Printf("to server\n");
#endif
						//				close(socketnum[i]);  
						FD_CLR(ServerPC.SocketId, &clientfdset); 
						closesocket(ServerPC.SocketId);//���������socket// add by ly at 20151218 10:09
						ServerPC.SocketId = 0; 
						ServerPC.ConnectStatus=Start;
						ServerPC.ConnectTime=0;
						ServerPC.ConnectCnt++;
						if (ServerPC.ConnectCnt>3)
						{
//							UART_Printf("ServerPC[%d] error\n", i);  
						}
					}  
					else  
					{    
						ServerPC.SendFlag=0;
						ServerPC.ReceDataCnt=0;
						//�洢UI���͵�����
						for (j=0;j<ret;j++)
						{
							//								printf("msg:%x\n",Clinet[i].ReceData[j]);
							ServerPC.ReceData[ServerPC.ReceDataCnt++]=serverbuf[j];
						}
//						UART_Printf(" msg:%s\n", ServerPC.ReceData); 
//						UART_Printf("fafa \n");
						//����UI��С�ޣ�������
						DataProcessFormUI2();
						/*
						for (j=0;j<20;j++)
						{
							printf("ServerPC:%x\n",ServerPC.ReceData[j]);
						}
						*/
						
					}  
				}
				
			}
			

		usleep(1);
		//Sleep(10000);
			

    }// end while

    return 0;
}


//�˺������ڵ��Կڴ�ӡ����
void UART_Printf(const char *format, ...)
{
	static  char  buffer[60 + 1];
	va_list     vArgs;

	va_start(vArgs, format);
	vsprintf((char *)buffer, (char const *)format, vArgs);
	va_end(vArgs);
	if (Clinet[0].SendDataCnt+strlen((char *)buffer)>1400)
	{
		Clinet[0].SendDataCnt=0;
		
	}
	
	memcpy(&Clinet[0].SendData[Clinet[0].SendDataCnt],buffer,strlen((char*)buffer));
	
	
	
	Clinet[0].SendDataCnt+=strlen((char*)buffer);
	Clinet[0].SendFlag=0;
}



/********************************************************************
�����汾��V1.0
����˵����������������ַ�����ת��Ϊһ��16��������ת�����������0��
ʹ�÷�Χ��ģ���ⲿ
�㷨������
������ֵ����ת��õ�16������
		  0 ������
�β�˵����	uChar_MSB
			uChar_LSB
�ֲ�������
ȫ�ֱ�����
����ע�ͣ��� '1''D' ���ú���ת���Ϊ 0X1D
��д���ڣ�2002_01_07
��д��Ա��pass_by
********************************************************************/
BYTE Comm_2NumCharToHex ( BYTE uChar_H , BYTE uChar_L )
{
	if ( ( uChar_H >= '0' ) && ( uChar_H <= '9' ) )
		uChar_H -= 0x30 ;
	else
	{
		if ( ( uChar_H >= 'A' ) && ( uChar_H <= 'F' ) )
			uChar_H -= 0x37 ;
		else
		{
			if ( ( uChar_H >= 'a' ) && ( uChar_H <= 'f' ) )
				uChar_H -= 0x57 ;
			else
				return (0) ;
		}
	}

	if ( ( uChar_L >= '0' ) && ( uChar_L <= '9' ) )
		uChar_L -= 0x30 ;
	else
	{
		if ( ( uChar_L >= 'A' ) && ( uChar_L <= 'F' ) )
			uChar_L -= 0x37 ;
		else
		{
			if ( ( uChar_L >= 'a' ) && ( uChar_L <= 'f' ) )
				uChar_L -= 0x57 ;
			else
				return (0) ;
		}
	}

	return ( 16*uChar_H + uChar_L ) ;
}



WORD ByteTOAscii(BYTE num)
{
	WORD i,j=0;
	i=num>>4;
	if (i>9)
	{
		j=i+0x37;
	}
	else
	{
		j=i+0x30;
	}
	j=j<<8;
	i=num&0x0f;
	if (i>9)
	{
		j+=i+0x37;
	}
	else
	{
		j+=i+0x30;
	}
	return j;
}


//�˺�������UI���������ݡ�
void DataProcessFormUI2(void)//modified by ly at 18:00
{
	BYTE TempData[200];
	//BYTE TempIp[15];
	char TempIp[15];
	DWORD dr=0;
	DWORD dk=0;
	char num=0;
	BYTE k,m=0;
	BYTE j=0;
	BYTE i=0;
	BYTE CheckNum =0;
	BYTE CheckNumFormUi=0;
	WORD Length=0;
	PT_PJDATA_MGR Pjss;
	Pjss = (PT_PJDATA_MGR)malloc(sizeof(PJDATA_MGR));
	memset(Pjss, 0, sizeof(PJDATA_MGR));
	//	if (ServerPC.ReceCntLock == 0)  //��??��??��???��??��?
	//	{
	if (ServerPC.ReceDataCnt > 0)
	{
#if defined(DEBUG_DOS)
		printf("UI->OS:");
		for (int iiii=0; iiii< ServerPC.ReceDataCnt; iiii++)
		{
			printf("%02hX ", ServerPC.ReceData[iiii]);
		}
		printf("\n");
#endif


		j=ServerPC.ReceData[0];
		k=ServerPC.ReceData[(ServerPC.ReceDataCnt-1)];
		//�ж�Э��ͷβ
		if ((k==0x7e)&&(j==0x7e))
		{
			Length=MAKEWORD(ServerPC.ReceData[2],ServerPC.ReceData[1]);
			CheckNumFormUi=ServerPC.ReceData[3];
			CheckNum=0;
			//����У���
			for (i=5;i<5+Length;i++)
			{
				CheckNum^=ServerPC.ReceData[i];
			}
			int a,b;
			a = 0;
			b = 0;
			if ((Length==ServerPC.ReceDataCnt-6)&&(CheckNum==CheckNumFormUi))
			{
				switch( (BYTE)(ServerPC.ReceData[4]) )
				{
				case 0x00:
					break;
				case 0x01://ˢ��
					Refresh_Flag = true;
		
					break;
				case 0x02://��ӱ༭
					memmove(&ServerPC.ReceData[0],&ServerPC.ReceData[5],ServerPC.ReceDataCnt-6);


					for (i=0;i<12;i++)//MAC��??ַ??
					{
						TempData[j++]=ServerPC.ReceData[i];
						Pjss->m_HrDataGet.MacAddress[i]=ServerPC.ReceData[i];
					}

					for (i=12;i<27;i++)
					{
						Pjss->m_HrDataGet.IpAddress[i-12]=ServerPC.ReceData[i];
					}

					for (i=27;i<42;i++)//TYPE ��??��??
					{
						Pjss->m_HrDataGet.Type[i-27]=ServerPC.ReceData[i];
					}


					//Auto Power Off Time ----Set
					Pjss->m_HrDataGet.AutoPowerOff=ServerPC.ReceData[42];
					//18FEE8
					dr=0;
					dr=((((DWORD)ServerPC.ReceData[43])&0X000000FF)<<24);
					dk=((((DWORD)ServerPC.ReceData[44])&0X000000FF)<<16);
					dr=dr+dk;
					dk=((((DWORD)ServerPC.ReceData[45])&0X000000FF)<<8);
					dr=dr+dk;
					dk=(((DWORD)ServerPC.ReceData[46])&0X000000FF);
					dr=dr+dk;

					//18FEE8
					Pjss->m_HrDataGet.LampOnTime=dr;

					dr=0;
					dr=((((DWORD)ServerPC.ReceData[47])&0X000000FF)<<24);
					dk=((((DWORD)ServerPC.ReceData[48])&0X000000FF)<<16);
					dr=dr+dk;
					dk=((((DWORD)ServerPC.ReceData[49])&0X000000FF)<<8);
					dr=dr+dk;
					dk=(((DWORD)ServerPC.ReceData[50])&0X000000FF);
					dr=dr+dk;

					Pjss->m_HrDataGet.LampOffTime=dr;

					Pjss->m_HrDataGet.EcoMode	=	ServerPC.ReceData[51];

					Pjss->m_HrDataGet.StandbyMode=	ServerPC.ReceData[52];
					Pjss->m_HrDataGet.Install	=	ServerPC.ReceData[53];

					dr=0;
					dr=((((DWORD)ServerPC.ReceData[54])&0X000000FF)<<24);
					dk=((((DWORD)ServerPC.ReceData[55])&0X000000FF)<<16);
					dr=dr+dk;
					dk=((((DWORD)ServerPC.ReceData[56])&0X000000FF)<<8);
					dr=dr+dk;
					dk=(((DWORD)ServerPC.ReceData[57])&0X000000FF);
					dr=dr+dk;
					Pjss->m_HrDataGet.HeatCycle=dr;	

					for (i=0;i<10;i++)
					{
						Pjss->m_HrDataGet.ImagePattern[i]=ServerPC.ReceData[58+i];
					}
					//07080C09090A080909 
					dr=0;
					PJDataRegister(Pjss);
					break;
				case 0x03://��ѯ��ʷ
//					g_pHisData->pQueryStart = ;//kong
					if (g_pHisData->bUIQueryHisData == true || g_pHisData->bSendFinish == true)
					{
					}
					else
					{
						memset(g_pHisData, 0, sizeof(HISTORICAL_DATA));
						//����MAC��ַ
						for (i=0;i<12;i++)
						{
							g_pHisData->pQueryMac[i] = ServerPC.ReceData[5+i];
						}
						g_pHisData->pQueryMac[12]='\0';
						//���ÿ�ʼʱ��
						for (i=0;i<8;i++)
						{
							g_pHisData->pQueryStart[i] = ServerPC.ReceData[17+i];
						}
						g_pHisData->pQueryStart[8]='\0';
						//���ý���ʱ��
						for (i=0;i<8;i++)
						{
							g_pHisData->pQueryEnd[i] = ServerPC.ReceData[25+i];
						}
						g_pHisData->pQueryEnd[8]='\0';


						g_pHisData->bUIQueryHisData = true;
						g_pHisData->bSendFinish = true;
					}

					
					break;
				case 0x04://ɾ��

					for (i=5;i<20;i++)
					{
						TempIp[i-5]=ServerPC.ReceData[i];
					}
					PJDataDel(TempIp);

				
					break;

				case 0x05:
					for (i=5;i<20;i++)
					{
						TempIp[i-5]=ServerPC.ReceData[i];
					}
					PJPause(TempIp);
// 					if(PJPause(TempIp))
// 					{
// 						StatusToUI(TempIp, 0x05);
// 					}
					break;
				case 0x06:
					for (i=5;i<20;i++)
					{
						TempIp[i-5]=ServerPC.ReceData[i];
					}
					PJStart(TempIp);
// 					if (PJStart(TempIp))
// 					{
// 						StatusToUI(TempIp, 0x06);
// 					}
					break;

				}
			}
		}
	}
}



// void PJDataRegister(PT_PJDATA_MGR PjDataMgr)
// {
// 
// 	PjDataMgr->m_Option.Used_Flag = true;
// 	PjDataMgr->m_Option.LampOnOffTime = 0;
// 	PjDataMgr->m_Option.Power_Flag = false; //false�ػ�
// 	PjDataMgr->m_Option.Pattern_Flag = false;//false��ѭ
// 	PjDataMgr->m_Option.PatternTime = 5; //��ѭʱ��
// 	PjDataMgr->m_Option.FactoryMode_Flag = false;
// 	PjDataMgr->m_Option.ImagePattern_Flag = false;
// 	PjDataMgr->m_Option.Pattern = Pattern0;
// 	PjDataMgr->m_HrDataGet.RunStatus = 0x01;
// 	pSmpNode NodeTemp;
// 	PJDATA_MGR *PjDataTemp;
// 	int ClinetIDTemp;
// 	//int TimerSoftMgr[10] = {0};
// 	int TimerSoftMgr;
// 	if (!g_pSmpDataMgrListHead) //����ͷΪNULL����ʼ��PjDataMgrΪ��һ���ڵ�����
// 	{
// 		PjDataMgr->m_Option.ClinetID = 1;
// 		PjDataMgr->m_Option.Used_Flag = 1;
// 		g_pSmpDataMgrListHead = SMPLIST_init(PjDataMgr); 	
// 		NUM_PJ++;         //δ������ɣ�PJ����   ccq
// 		Clinet[PjDataMgr->m_Option.ClinetID].sin_addr.S_un.S_addr = inet_addr(PjDataMgr->m_HrDataGet.IpAddress);
// 		Clinet[PjDataMgr->m_Option.ClinetID].ClinetHtons=htons(23);
// 		Clinet[PjDataMgr->m_Option.ClinetID].ConnectStatus=Start;
// 		Clinet[PjDataMgr->m_Option.ClinetID].SocketId=0;
// 		Clinet[PjDataMgr->m_Option.ClinetID].ConnectCnt=0;
// 		Clinet[PjDataMgr->m_Option.ClinetID].ConnectTime=0;
// 		Clinet[PjDataMgr->m_Option.ClinetID].ConnectuTime=5000;
// 		Clinet[PjDataMgr->m_Option.ClinetID].SendCntLock=0;
// 		Clinet[PjDataMgr->m_Option.ClinetID].SendDataCnt=0;
// 		Clinet[PjDataMgr->m_Option.ClinetID].PowerOnFlag=0;
// 		SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);  //ccq 2015/11/25 10:53
// 		TimerSoftMgr = TimerSoft_Regedit( g_pSmpDataMgrListHead, PjDataMgr->m_Option.ClinetID, FLAG_OK, 1 , PJ_TSProcess );
// 		TimerSoft_Start(g_pSmpDataMgrListHead, TimerSoftMgr);
// 
// 	} 
// 	else
// 	{
// 		NodeTemp = g_pSmpDataMgrListHead;
// 		while (NodeTemp)
// 		{
// 			PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
// 			if (strcmp(PjDataTemp->m_HrDataGet.IpAddress, PjDataMgr->m_HrDataGet.IpAddress) == 0)  //���ҵ����л���   Temp�Ǵ�����ȡ����  Mgr��С������������
// 			{
// // 				if (PjDataMgr->m_HrDataGet.HeatCycle == 0)
// // 				{
// // 					PjDataMgr->m_HrDataGet.HeatCycle = PjDataTemp->m_HrDataGet.HeatCycle;
// // 				}
// 
// 				PjDataMgr->m_HrDataGet.OnlineStatus = PjDataTemp->m_HrDataGet.OnlineStatus;
// 				PjDataMgr->m_HrDataGet.RunStatus = PjDataTemp->m_HrDataGet.RunStatus;
// 				PjDataMgr->m_TimerMgr = PjDataTemp->m_TimerMgr;
// 				PjDataMgr->m_Option = PjDataTemp->m_Option;
// //				*PjDataTemp = *PjDataMgr;//modified by ly at 20160104 16:64 ����1�����ɸ����ˣ��������±༭ʱ��MAC��ַ��¼Ϊ0������汾Ҳ��ʧ
// //				*PjDataMgr = *PjDataTemp;//����2���ϻ�ʱ�䣨LEMPONTIME LEMPOFFTIME��û���µ�������ȥ ����AutoPowerOff��Ҳû���µ�����ֱ��PJ������ˢ��
// 
// 				//�޸�����1
// 				memcpy(PjDataMgr->m_HrDataGet.FirmwareTime,PjDataTemp->m_HrDataGet.FirmwareTime,18);
// 				memcpy(PjDataMgr->m_HrDataGet.MacAddress,PjDataTemp->m_HrDataGet.MacAddress,13);
// 
// 				//�޸�����2
// 				*PjDataTemp = *PjDataMgr;
// 				NodeTemp->data = PjDataTemp;
// 				//��ע����̨����ok��UI��ʾ�����������ǲ��ǰ�ʵ�ʵ�ʱ������ͣ��δ���ԡ� ��ע2����̨����δ���ԡ�
// 
// 				SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);
// 				break;
// 			} 
// 			NodeTemp = NodeTemp->next;
// 		}
// 		bool Register_Flag = false;
// 		if (NodeTemp == NULL) //û����̨����
// 		{
// 			NodeTemp = g_pSmpDataMgrListHead;
// 			while(NodeTemp)
// 			{
// 				PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
// 				if(!PjDataTemp->m_Option.Used_Flag)  //used_Flag=0����
// 				{
// 					PjDataTemp->m_Option.Used_Flag = 1;
// 					PjDataMgr->m_TimerMgr = PjDataTemp->m_TimerMgr;
// 					PjDataMgr->m_Option = PjDataTemp->m_Option;
// 					*PjDataTemp = *PjDataMgr;
// 					Clinet[PjDataMgr->m_Option.ClinetID].sin_addr.S_un.S_addr = inet_addr(PjDataMgr->m_HrDataGet.IpAddress);
// 					Clinet[PjDataMgr->m_Option.ClinetID].ClinetHtons=htons(23);
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectStatus=Start;
// 					Clinet[PjDataMgr->m_Option.ClinetID].SocketId=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectCnt=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectTime=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectuTime=5000;
// 					Clinet[PjDataMgr->m_Option.ClinetID].SendCntLock=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].SendDataCnt=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].PowerOnFlag=0;
// 					SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);  //ccq 2015/11/25 10:53
// 					TimerSoftMgr = TimerSoft_Regedit( g_pSmpDataMgrListHead, PjDataMgr->m_Option.ClinetID, FLAG_OK, 1 , PJ_TSProcess );
// 					TimerSoft_Start(g_pSmpDataMgrListHead, TimerSoftMgr);
// 					Register_Flag = true;
// 					break;
// 				}
// 				else
// 				{
// 					ClinetIDTemp =	PjDataTemp->m_Option.ClinetID;
// 					NodeTemp = NodeTemp->next;
// 				}
// 			}//end while
// 			if (Register_Flag == false)
// 			{
// 				if(PjDataTemp->m_Option.Used_Flag)  //������û�п��нڵ�
// 				{	
// 					PjDataMgr->m_Option.Used_Flag;
// 					PjDataMgr->m_Option.ClinetID = ClinetIDTemp+1;
// 					//NodeTemp->data = PjDataMgr;
// 					SMPLIST_addnode( PjDataMgr, g_pSmpDataMgrListHead ); //������ʱ����PjDataMgr������
// 					NUM_PJ++;         //δ������ɣ�PJ����   ccq
// 					Clinet[PjDataMgr->m_Option.ClinetID].sin_addr.S_un.S_addr = inet_addr(PjDataMgr->m_HrDataGet.IpAddress);
// 					Clinet[PjDataMgr->m_Option.ClinetID].ClinetHtons=htons(23);
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectStatus=Start;
// 					Clinet[PjDataMgr->m_Option.ClinetID].SocketId=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectCnt=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectTime=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].ConnectuTime=5000;
// 					Clinet[PjDataMgr->m_Option.ClinetID].SendCntLock=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].SendDataCnt=0;
// 					Clinet[PjDataMgr->m_Option.ClinetID].PowerOnFlag=0;
// 					SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);  //ccq 2015/11/25 10:53
// 					TimerSoftMgr = TimerSoft_Regedit( g_pSmpDataMgrListHead, PjDataMgr->m_Option.ClinetID, FLAG_OK, 1 , PJ_TSProcess );
// 					TimerSoft_Start(g_pSmpDataMgrListHead, TimerSoftMgr);
// 				}
// 			}
// 			
// 		} 
// 	}
// }

//�˺�������PJ���ص����ݡ�
void DataProcessFormPJ(BYTE iClient)
{
	/*
	BYTE i=0;
	BYTE j=0;
	BYTE k=0;
	BYTE num=0;
	BYTE UpSetPoint=0;
	WORD kk=0;
	DWORD Dt,Dk;
	WORD aa=0;
	WORD ii=0;

	*/
	//����Ϊ��ʱ������������Ϊ��Щ���������⣬���Է��ڽṹ���У�ÿ��Clinet���и��Եı�����
	BYTE i=0;
	Clinet[iClient].ProcessParmI=0;
	Clinet[iClient].ProcessParmPoint=0;
	Clinet[iClient].ProcessParmNum=0;
	Clinet[iClient].ProcessParmKK=0;
	Clinet[iClient].ProcessParmK=0;
	Clinet[iClient].ProcessParmDk=0;
	Clinet[iClient].ProcessParmDt=0;
	Clinet[iClient].ProcessParmII=0;
	Clinet[iClient].ProcessParmAA=1;
	Clinet[iClient].ProcessParmJ=0;

	//memset(PjGet[iClient], 0, sizeof(PjGet[iClient]));

	while(Clinet[iClient].ProcessParmAA)
	{
		switch(Clinet[iClient].ProcessParmNum)	//	numδ��ѭ��
		{
		case 0:
			//��ȡMAC��ַ  PJ MAC��ַ��Ҫ��ȡ6���ֽ�
			for (Clinet[iClient].ProcessParmI=0;Clinet[iClient].ProcessParmI<6;Clinet[iClient].ProcessParmI++)
			{
				//���жϣ����ƫ��ָ����ڽ��ջ�����Cnt������Ϊ���ݽ������˳�whileѭ��
				if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
				{
					Clinet[iClient].ProcessParmAA=0;
					break;
				}
				PjGet->m_HrDataGet.MacAddress[Clinet[iClient].ProcessParmI*2]=(BYTE)(ByteTOAscii(Clinet[iClient].TempBuff[1+Clinet[iClient].ProcessParmI*3])>>8);
				PjGet->m_HrDataGet.MacAddress[Clinet[iClient].ProcessParmI*2+1]=(BYTE)ByteTOAscii(Clinet[iClient].TempBuff[1+Clinet[iClient].ProcessParmI*3]);
				
				Clinet[iClient].ProcessParmPoint+=3;
			}

			PjGet->m_HrDataGet.MacAddress[12]='\0';
			Clinet[iClient].ProcessParmNum=1;
			break;
		case 1:
			//��ȡFirmwareTime���� ��:1D 07 DF
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			
			//07 DF =2015
			Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			PjGet->m_HrDataGet.FirmwareTime[0]=Clinet[iClient].ProcessParmKK/1000+0x30;
			PjGet->m_HrDataGet.FirmwareTime[1]=Clinet[iClient].ProcessParmKK%1000/100+0x30;
			PjGet->m_HrDataGet.FirmwareTime[2]=Clinet[iClient].ProcessParmKK%100/10+0x30;
			PjGet->m_HrDataGet.FirmwareTime[3]=Clinet[iClient].ProcessParmKK%10+0x30;
			Clinet[iClient].ProcessParmPoint+=3;//��
			Clinet[iClient].ProcessParmNum=2;
			break;
		case 2:
			//��ȡFirmwareTime���� ��:1D 00 0C
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}

			//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
			Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			PjGet->m_HrDataGet.FirmwareTime[4]=Clinet[iClient].ProcessParmKK/10+0x30;
			PjGet->m_HrDataGet.FirmwareTime[5]=Clinet[iClient].ProcessParmKK%10+0x30;
			Clinet[iClient].ProcessParmPoint+=3;//��
			Clinet[iClient].ProcessParmNum=3;
			break;
		case 3:
			//��ȡFirmwareTime���� ��:1D 00 0C
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
			Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			PjGet->m_HrDataGet.FirmwareTime[6]=Clinet[iClient].ProcessParmKK/10+0x30;
			PjGet->m_HrDataGet.FirmwareTime[7]=Clinet[iClient].ProcessParmKK%10+0x30;
			PjGet->m_HrDataGet.FirmwareTime[8]='_';
			Clinet[iClient].ProcessParmPoint+=3;//��
			Clinet[iClient].ProcessParmNum=4;
			break;
		case 4:
			//��ȡFirmwareTime��ʱ���� �洢��ʽ00��00��00
			for(Clinet[iClient].ProcessParmI=0;Clinet[iClient].ProcessParmI<3;Clinet[iClient].ProcessParmI++)
			{
				if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
				{
					Clinet[iClient].ProcessParmAA=0;
					break;
				}
				//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
				Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
				PjGet->m_HrDataGet.FirmwareTime[9+Clinet[iClient].ProcessParmI*3]=Clinet[iClient].ProcessParmKK/10+0x30;
				PjGet->m_HrDataGet.FirmwareTime[10+Clinet[iClient].ProcessParmI*3]=Clinet[iClient].ProcessParmKK%10+0x30;
				PjGet->m_HrDataGet.FirmwareTime[11+Clinet[iClient].ProcessParmI*3]=':';
				if (Clinet[iClient].ProcessParmI==2)
				{
					PjGet->m_HrDataGet.FirmwareTime[11+Clinet[iClient].ProcessParmI*3]='\0';
				}
				Clinet[iClient].ProcessParmPoint+=3;//ʱ����
			}
			Clinet[iClient].ProcessParmNum=5;
			break;
		case 5://18FEE8
			//�洢AutoPowerOff
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
			Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			PjGet->m_HrDataGet.AutoPowerOff=(BYTE)Clinet[iClient].ProcessParmKK;
			Clinet[iClient].ProcessParmPoint+=3;//auto power off
			Clinet[iClient].ProcessParmNum=6;
			break;
		case 6:
			//��ȡEcoMode
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			PjGet->m_HrDataGet.EcoMode=Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1];
			Clinet[iClient].ProcessParmPoint+=3;//
			Clinet[iClient].ProcessParmNum=7;				
			break;
		case 7:
			//��ȡStandbyMode
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			PjGet->m_HrDataGet.StandbyMode=Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1];
			Clinet[iClient].ProcessParmPoint+=3;//
			Clinet[iClient].ProcessParmNum=8;
			break;
		case 8:
			//��ȡInstall
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			PjGet->m_HrDataGet.Install=Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1];
			Clinet[iClient].ProcessParmPoint+=3;//
			Clinet[iClient].ProcessParmNum=9;
			break;
		case 9:
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
			Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			PjGet->m_HrDataGet.Lamp1Time=Clinet[iClient].ProcessParmKK;
			Clinet[iClient].ProcessParmPoint+=3;//auto power off
			Clinet[iClient].ProcessParmNum=10;
			break;
		case 10:
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
			//Dk=MAKEWORD(Clinet[iClient].ReceData[UpSetPoint+1],Clinet[iClient].ReceData[UpSetPoint+2]);
			Clinet[iClient].ProcessParmDk=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			Clinet[iClient].ProcessParmDt=(Clinet[iClient].ProcessParmDk<<16)&0xffff0000;
			PjGet->m_HrDataGet.Lamp1Time+=Clinet[iClient].ProcessParmDt;
			Clinet[iClient].ProcessParmPoint+=3;//auto power off
			Clinet[iClient].ProcessParmNum=11;
			break;
		case 11://add by ly at 2015/12/15 16:28 add lamp2time
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			//���û��Lamp2����ֱ������13
			if (Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint]!=0x1d)
			{
				PjGet->m_HrDataGet.Lamp2Time = 0x00;
				Clinet[iClient].ProcessParmNum=13;
			}
			else
			{
				//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
				Clinet[iClient].ProcessParmKK=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
				PjGet->m_HrDataGet.Lamp2Time=Clinet[iClient].ProcessParmKK;
				Clinet[iClient].ProcessParmPoint+=3;//auto power off
				Clinet[iClient].ProcessParmNum=12;
			}

			break;
		case 12:
			if (Clinet[iClient].ProcessParmPoint>Clinet[iClient].TempBuffCnt)
			{
				Clinet[iClient].ProcessParmAA=0;
				break;
			}
			//kk=Clinet[iClient].ReceData[UpSetPoint+2]<<8+Clinet[iClient].ReceData[UpSetPoint+1];
			//Dk=MAKEWORD(Clinet[iClient].ReceData[UpSetPoint+1],Clinet[iClient].ReceData[UpSetPoint+2]);
			Clinet[iClient].ProcessParmDk=MAKEWORD(Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+1],Clinet[iClient].TempBuff[Clinet[iClient].ProcessParmPoint+2]);
			Clinet[iClient].ProcessParmDt=(Clinet[iClient].ProcessParmDk<<16)&0xffff0000;
			
			PjGet->m_HrDataGet.Lamp2Time+=Clinet[iClient].ProcessParmDt;

			
			Clinet[iClient].ProcessParmPoint+=3;//auto power off
			Clinet[iClient].ProcessParmNum=13;
			break;;

		case 13:
			Clinet[iClient].ProcessParmAA=0;
			if (iClient==2)
			{
				i++;
			}
			break;
		}
	}
	SaveGetData(iClient, PjGet);
}






// int main()
// {
//     WORD wVersionRequested;
//     WSADATA wsaData;
//     int err;
// 	int timer1 = 1;
// 	char i,j;
// 	WORD ii;
// 	//	SOCKET clientSock[10];
// 	//	SOCKADDR_IN addrClient[10];
// 
// 
// 	
// 
// 	int ret;
// 	
// 	Clinet[0].sin_addr.S_un.S_addr = inet_addr("10.97.24.150");
// 	Clinet[0].ClinetHtons=htons(23);
// 	Clinet[0].ConnectStatus=Start;
// 	Clinet[0].SocketId=0;
// 	Clinet[0].ConnectCnt=0;
// 	Clinet[0].ConnectTime=0;
// 	Clinet[0].ConnectuTime=5000;
// 	Clinet[0].SendCntPoint=0;
// 	Clinet[0].SendDataCnt=0;
// 
// 
// 	Clinet[1].sin_addr.S_un.S_addr = inet_addr("10.97.24.170");
// 	Clinet[1].ClinetHtons=htons(23);
// 	Clinet[1].ConnectStatus=Start;
// 	Clinet[1].SocketId=0;
// 	Clinet[1].ConnectCnt=0;
// 	Clinet[1].ConnectTime=0;
// 	Clinet[1].ConnectuTime=5000;
// 	Clinet[1].SendCntPoint=0;
// 	Clinet[1].SendDataCnt=0;
// 	/*
//     wVersionRequested = MAKEWORD(2, 2);
// 
//     err = WSAStartup(wVersionRequested, &wsaData);
//     if (err != 0) {
//         printf("WSAStartup failed with error: %d\n", err);
//         return 1;
//     }
// 
//     if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
//         // Tell the user that we could not find a usable //
//         // WinSock DLL.                                  //
//         printf("Could not find a usable version of Winsock.dll\n");
//         WSACleanup();
//         return 1;
//     }
//     else
//         printf("The Winsock 2.2 dll was found okay\n");
// */
// 
// 	HANDLE thHandle3 = CreateThread(NULL, 0, TimeThread, NULL, 0, NULL);//�������߳�
// 	if(NULL == thHandle3)
// 	{
// 		cout<<"Create Thread failed !"<<endl;
// 		return -1;
// 	}
// 	
// 	HANDLE thHandle = CreateThread(NULL, 0, ProConnect, NULL, 0, NULL);//�������߳�
// 	if(NULL == thHandle)
// 	{
// 		cout<<"Create Thread failed !"<<endl;
// 		return -1;
// 	}
// 
// 	HANDLE thHandle4 = CreateThread(NULL, 0, ProSendData, NULL, 0, NULL);//�������߳�
// 	if(NULL == thHandle4)
// 	{
// 		cout<<"Create Thread failed !"<<endl;
// 		return -1;
// 	}
// 
// 
// 	HANDLE thHandle2 = CreateThread(NULL, 0, ProcThread, NULL, 0, NULL);//�������߳�
// 	if(NULL == thHandle2)
// 	{
// 		cout<<"Create Thread2 failed !"<<endl;
// 		return -1;
// 	}
// 	
//     WSACleanup();
// 
//     
//     system("PAUSE");
//     return 0;
// }