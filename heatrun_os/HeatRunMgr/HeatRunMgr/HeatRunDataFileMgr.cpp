#include <stdio.h>
#include <time.h>
#include <direct.h>
#include <io.h> 
#include "SimpleList.h"
#include "HeatRunData.h"
#include "HeatRunDataFileMgr.h"
#include "PJOperation.h"

/*
  Nothing,Color Bar,Pixel Gamma,
  Ghost R,Ghost G,Ghost B,Ghost W,
  NRSH R,NRSH G,NRSH B,NRSH W,
  VCom R,VCom G,VCom B,VCom W,
  Gray Scale W,Gray Scale G,Reverced Gray Scale W,Reverced Gray Scale G,
  R_0/255,G_0/255,B_0/255,W_0/255,
  R_32/255,G_32/255,B_32/255,W_32/255,
  R_64/255,G_64/255,B_64/255,W_64/255,
  R_96/255,G_96/255,B_96/255,W_96/255,
  R_168/255,G_168/255,B_168/255,W_168/255,
  R_192/255,G_192/255,B_192/255,W_192/255,
  R_200/255,G_200/255,B_200/255,W_200/255,
  R_208/255,G_208/255,B_208/255,W_208/255,
  R_255/255,G_255/255,B_255/255,W_255/255,
  C.Unif R(min),C.Unif R(mid_lo),C.Unif R(mid_hi),C.Unif R(max),
  C.Unif G(min),C.Unif G(mid_lo),C.Unif G(mid_hi),C.Unif G(max),
  C.Unif B(min),C.Unif B(mid_lo),C.Unif B(mid_hi),C.Unif B(max),
  C.Unif W(min),C.Unif W(mid_lo),C.Unif W(mid_hi),C.Unif W(max)
*/

//老化颜色的写文件字符串
char Pattern[][10] = {
						"Nothing__", //0
						"Color_Bar",//1
						"GrayScale",
						"Dither___",
						"50%Dither",
						"Pxl_Gamma",//5
						"Ep_Gamma_",
						"Border___",
						"Ghost_R__",//8
						"Ghost_G__",
						"Ghost_B__",
						"Ghost_W__",
						"NRSH_R___",
						"NRSH_G___",
						"NRSH_B___",
						"NRSH_W___",
						"VCom_R___",
						"VCom_G___",
						"VCom_B___",
						"VCom_W___",
						"GammaGSWN",
						"GammaGSGN",
						"GammaGSWR",
						"GammaGSGR",//23
						"PSIG_Gray",
						"PSIGBlack",
						"SubHPosi_",
						"Reserved_",
						"Vmid_R___",
						"Vmid_G___",
						"Vmid_B___",
						"Vmid_W___",
						"B_REF_R__",
						"B_REF_G__",
						"B_REF_B__",
						"B_REF_W__",
						"R_0/255__",//36
						"G_0/255__",
						"B_0/255__",
						"W_0/255__",
						"R_32/255_",
						"G_32/255_",
						"B_32/255_",
						"W_32/255_",
						"R_64/255_",
						"G_64/255_",
						"B_64/255_",
						"W_64/255_",
						"R_96/255_",
						"G_96/255_",
						"B_96/255_",
						"W_96/255_",
						"R_168/255",
						"G_168/255",
						"B_168/255",
						"W_168/255",
						"R_192/255",
						"G_192/255",
						"B_192/255",
						"W_192/255",
						"R_208/255",
						"G_208/255",
						"B_208/255",
						"W_208/255",
						"C.Unif_R0",
						"C.Unif_R1",
						"C.Unif_R2",
						"C.Unif_R3",
						"C.Unif_G0",
						"C.Unif_G1",
						"C.Unif_G2",
						"C.Unif_G3",
						"C.Unif_B0",
						"C.Unif_B1",
						"C.Unif_B2",
						"C.Unif_B3",
						"C.Unif_W0",
						"C.Unif_W1",
						"C.Unif_W2",
						"C.Unif_W3",
						"R_255/255",
						"G_255/255",
						"B_255/255",
						"W_255/255",
						"GammaGSWn",
						"GammaGSGn",
						"GammaGSWr",
						"GammaGSGr",
						"R_200/255",//88
						"G_200/255",
						"B_200/255",
						"W_200/255"
                      };

char EcoModeSW[][7] = {
	                    "Normal",
						"ECO-1_",
						"ECO-2_",
						"INTECO",
						"SAVER_"
                        };

char StandbyModeSW[][7] = {
	                        "Normal",
	                        "Saving"
                             };

char OnlineStatusSW[][8] = {
	                        "OutLine",
							"OnLine_"
                              };

char InstallSW[][7] = {
	                    "FrontD",
						"Rear_D",
						"Rear_C",
						"FrontC"
                        };

//获取系统当前时间
int TOOL_GetLocalDate( char v_cLocalDate[9] )
{
	int year, month, day;
	struct tm *timeinfo;			//定义时间结构体 
	time_t nowtime;  
	time( &nowtime );				//获取时间   
	timeinfo = localtime( &nowtime );	//转化为当地时间  
	year = timeinfo->tm_year + 1900;	//从1900年开始计数，所以+1900 
	month = timeinfo->tm_mon + 1;		//从0开始计数，所以+1 
	day = timeinfo->tm_mday; 
	memset(v_cLocalDate,0x00,sizeof(v_cLocalDate));
	sprintf( v_cLocalDate, "%04d%02d%02d",year,month,day);  
	return 0;
}

int TOOL_GetLocalTime( char v_cLocalTime[9] )
{
	int hour, min, sec;
	struct tm *timeinfo;			//定义时间结构体 
	time_t nowtime;  
	time( &nowtime );				//获取时间   
	timeinfo = localtime( &nowtime );	//转化为当地时间  
	hour = timeinfo->tm_hour; 
	min = timeinfo->tm_min; 
	sec = timeinfo->tm_sec; 
	memset(v_cLocalTime,0x00,sizeof(v_cLocalTime));
	sprintf( v_cLocalTime, "%02d:%02d:%02d", hour, min, sec);  
	return 0;
}

//MacAddress[13] IpAddress[16] Type[16] FirmwareTime[20] AutoPowerOff LampOnTime LampOffTime EcoMode StandbyMode Install OnlineStatus HeatCycle ImagePattern[9] Lamp1Time Lamp2Time	
//UI通讯协议和文件读写位置一致
int SaveHrDataToFile( void *ListHead)
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}
	/*
	int   access(const   char   *filename,   int   amode); 
	amode参数为0时表示检查文件的存在性，如果文件存在，返回0，不存在，返回-1。 
	这个函数还可以检查其它文件属性： 
	06     检查读写权限 
	04     检查读权限 
	02     检查写权限 
	01     检查执行权限 
	00     检查文件的存在性
	*/
	//问题点1：因为创建文件和读取文件数据都存在系统路径切换问题，暂时先搁置，待后续添加调试
// 	//如果没有HeatRunData数据管理文件夹，就创建文件夹
// 	char HeatRunDataDir[26] = "HeatRunData";
	char HeatRunDataDir[26] = {0};

	FILE *fp = NULL;
	//记录时间
	char cLocalDate[9] = {0};
	char cLocalTime[9] = {0};
	memset(cLocalDate,0x00,sizeof(cLocalDate));
	memset(cLocalTime,0x00,sizeof(cLocalTime));
	TOOL_GetLocalDate( cLocalDate );
	TOOL_GetLocalTime( cLocalTime );
	//问题点1：因为创建文件和读取文件数据都存在系统路径切换问题，暂时先搁置，待后续添加调试
	//sprintf( HeatRunDataDir+strlen(HeatRunDataDir),"\\%s",cLocalDate );
	//获取当前日期时间作为存储txt文档名
	sprintf( HeatRunDataDir,"%s",cLocalDate );
	sprintf(HeatRunDataDir+strlen(HeatRunDataDir),".txt");
	//打开文件
	fp = fopen( HeatRunDataDir, "a+" );
	if( fp == NULL ) 
	{
		return 0;
	}
	
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;

	//获取链表节点个数，同时这是本模块全局的
	int nSumListNode = SMPLIST_nodesum( ListHead );

	//申请需要写入数据的空间，写完一台PJ数据后清除 待链表数据写完后释放
	char *cWDataBuf = (char *)malloc(LENTH_ROWDATA*sizeof(char));
	memset(cWDataBuf,0x00,LENTH_ROWDATA*sizeof(char));
	int  nWDataLen = 0;

	for ( int nCount=1; nCount<=nSumListNode ; nCount++ )
	{
		// 遍历链表的每个节点
		NodeTemp = SMPLIST_getnode( nCount, Head );
		// 提取节点数据
		PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);

		//只保存已经在使用的节点数据
		if (true == PjDataMgr->m_Option.Used_Flag)
		{
			//单台PJ数据头
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'<')

		    //MAC
			memcpy( cWDataBuf+nWDataLen, PjDataMgr->m_HrDataGet.MacAddress, LENTH_MAC );
			nWDataLen += LENTH_MAC;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			//IP
			memcpy( cWDataBuf+nWDataLen, PjDataMgr->m_HrDataGet.IpAddress, LENTH_IP );
			nWDataLen += LENTH_IP;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			//Type
			memcpy( cWDataBuf+nWDataLen, PjDataMgr->m_HrDataGet.Type, LENTH_TYPE );
			nWDataLen += LENTH_TYPE;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			//software ver(待装BYTE)
			memcpy( cWDataBuf+nWDataLen, PjDataMgr->m_HrDataGet.FirmwareTime, LENTH_VER );
			nWDataLen += LENTH_VER;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// AutoPowerOff
			sprintf( cWDataBuf+nWDataLen, "%2d", PjDataMgr->m_HrDataGet.AutoPowerOff );
			nWDataLen += LENTH_AUTOPOWEROFF;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// LampOnTime
			// hour min second
			sprintf( cWDataBuf+nWDataLen, "%3d", ((PjDataMgr->m_HrDataGet.LampOnTime)/3600) );
			nWDataLen += ((LENTH_LAMPONOFF/3)-1);
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'h')
			sprintf( cWDataBuf+nWDataLen, "%3d", (((PjDataMgr->m_HrDataGet.LampOnTime)%3600)/60) );
			nWDataLen += ((LENTH_LAMPONOFF/3)-1);
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'m')
			sprintf( cWDataBuf+nWDataLen, "%3d", (((PjDataMgr->m_HrDataGet.LampOnTime)%3600)%60) );
			nWDataLen += ((LENTH_LAMPONOFF/3)-1);
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'s')
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// LampOffTime
			// hour min second
			sprintf( cWDataBuf+nWDataLen, "%3d", ((PjDataMgr->m_HrDataGet.LampOffTime)/3600) );
			nWDataLen += ((LENTH_LAMPONOFF/3)-1);
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'h')
			sprintf( cWDataBuf+nWDataLen, "%3d", (((PjDataMgr->m_HrDataGet.LampOffTime)%3600)/60) );
			nWDataLen += ((LENTH_LAMPONOFF/3)-1);
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'m')
			sprintf( cWDataBuf+nWDataLen, "%3d", (((PjDataMgr->m_HrDataGet.LampOffTime)%3600)%60) );
			nWDataLen += ((LENTH_LAMPONOFF/3)-1);
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'s')
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// EcoMode
			switch (PjDataMgr->m_HrDataGet.EcoMode)
			{
				case 0x00:
					sprintf( cWDataBuf+nWDataLen, EcoModeSW[0] );
					break;
				case 0x01:
					sprintf( cWDataBuf+nWDataLen, EcoModeSW[1] );
					break;
				case 0x02:
					sprintf( cWDataBuf+nWDataLen, EcoModeSW[2] );
					break;
				case 0x10:
					sprintf( cWDataBuf+nWDataLen, EcoModeSW[3] );
					break;
				case 0x20:
					sprintf( cWDataBuf+nWDataLen, EcoModeSW[4] );
					break;
				default:
					break;
			}
//			sprintf( cWDataBuf+nWDataLen, EcoModeSW[PjDataMgr->m_HrDataGet.EcoMode] );
			nWDataLen += LENTH_ECOMODE;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// StandbyMode
// 			switch (PjDataMgr->m_HrDataGet.StandbyMode)
// 			{
// 				case 0x00:
// 					sprintf( cWDataBuf+nWDataLen, StandbyModeSW[0] );
// 					break;
// 				case 0x01:
// 					sprintf( cWDataBuf+nWDataLen, StandbyModeSW[1] );
// 					break;
// 				default:
// 					break;
// 			}
			sprintf( cWDataBuf+nWDataLen, StandbyModeSW[PjDataMgr->m_HrDataGet.StandbyMode] );
			nWDataLen += LENTH_ECOMODE;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// OnlineStatus
// 			switch (PjDataMgr->m_HrDataGet.OnlineStatus)
// 			{
// 				case 0x00:
// 					sprintf( cWDataBuf+nWDataLen, OnlineStatusSW[0] );
// 					break;
// 				case 0x01:
// 					sprintf( cWDataBuf+nWDataLen, OnlineStatusSW[1] );
// 					break;
// 				default:
// 					break;
// 			}
			sprintf( cWDataBuf+nWDataLen, OnlineStatusSW[PjDataMgr->m_HrDataGet.OnlineStatus] );
			nWDataLen += LENTH_ONLINESTATUS;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// Install
// 			switch (PjDataMgr->m_HrDataGet.Install)
// 			{
// 				case 0x00:
// 					sprintf( cWDataBuf+nWDataLen, InstallSW[0] );
// 					break;
// 				case 0x01:
// 					sprintf( cWDataBuf+nWDataLen, InstallSW[1] );
// 					break;
// 				case 0x02:
// 					sprintf( cWDataBuf+nWDataLen, InstallSW[2] );
// 					break;
// 				case 0x03:
// 					sprintf( cWDataBuf+nWDataLen, InstallSW[3] );
// 					break;
// 				default:
// 					break;
// 			}
			sprintf( cWDataBuf+nWDataLen, InstallSW[PjDataMgr->m_HrDataGet.Install] );
			nWDataLen += LENTH_INSTALL;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// HeatCycle
			sprintf( cWDataBuf+nWDataLen, "%10d", PjDataMgr->m_HrDataGet.HeatCycle );
			nWDataLen += LENTH_HEATRUNCYCLE;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// ImagePattern (SUM_IMAGEPATTERN个状态轮询)
			//Alian：这个可以考虑挂在长青的轮训状态枚举定义上
			for (int nCnt=0; nCnt<SUM_IMAGEPATTERN; nCnt++)
			{
				sprintf( cWDataBuf+nWDataLen, "%s", Pattern[(PjDataMgr->m_HrDataGet.ImagePattern[nCnt])] );
				nWDataLen += LENTH_IMAGEPATTERN;
				FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')
			}

			// Lamp1Time
			sprintf( cWDataBuf+nWDataLen, "%7d", ((PjDataMgr->m_HrDataGet.Lamp1Time)) );
			nWDataLen += LENTH_LAMP1TTIME;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			// Lamp2Time
			sprintf( cWDataBuf+nWDataLen, "%7d", ((PjDataMgr->m_HrDataGet.Lamp2Time)) );
			nWDataLen += LENTH_LAMP2TTIME;
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'|')

			//最后加上时间
			sprintf( cWDataBuf+nWDataLen, "%s_%s", cLocalDate,cLocalTime );
			nWDataLen += LENTH_RECORDTIME;

			//单台PJ数据尾
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'>')
			FLAG_INTERVAL(cWDataBuf,nWDataLen,'\n')

			if (fp != NULL) 
			{
				fseek( fp, 0, SEEK_END );
				fwrite( cWDataBuf, nWDataLen, 1, fp );
			}

			//写好一台PJ数据后 清除缓存区 复位数据计数器
			memset(cWDataBuf,0x00,LENTH_ROWDATA*sizeof(char));
			nWDataLen = 0;
		}
	}

	//写完文件，准备退出函数时要释放申请的缓存空间
	free(cWDataBuf);
	fclose(fp);

	return 1;
}


//MacAddress[13] IpAddress[16] Type[16] FirmwareTime[20] AutoPowerOff LampOnTime LampOffTime EcoMode StandbyMode Install OnlineStatus HeatCycle ImagePattern[9] Lamp1Time Lamp2Time	
//UI通讯协议和文件读写位置一致
int GetHRDataFromFile( PT_HISTORICAL_DATA pHrHisData )
{
	FILE *fp = NULL;
	char pGetFileData[LENTH_ROWDATA] = {0};
	char cBufChToBy1[10] = {0};
	char cBufChToBy2[10] = {0};
	char cBufChToBy3[10] = {0};
	int nCntFile = 0;
	int nCnt = 0;
	int  nHrDataROW = 0;
	int  nFramePjSum = 0;
	int  nGetDataLen = 2;            //每帧协议的最前两位留给 总共数据包数量使用
	int  nDealDataPos = 0;
	WORD wIndexPacket = 0;
	DWORD dwValue = 0;


	for (nCntFile=0; nCntFile<(pHrHisData->nSumFileTxt); nCntFile++ )
	{
		//打开文件
		fp = fopen( pHrHisData->pFileName[nCntFile], "a+" );
		if( fp == NULL ) 
		{
			return 0;
		}
		while( fread( pGetFileData, LENTH_ROWDATA, 1, fp ) )
		{
			if ( pGetFileData[0]!='<' || pGetFileData[LENTH_ROWDATA-2]!='>' )
			{
				printf("Fail to GetHRDataFromFile read data");
				return -1;
			} 
			else
			{
				nDealDataPos +=1;
				//判断数据是不是对应要求MAC地址机子的数据
				if ( 0 == memcmp( PSOFFSET(pGetFileData,nDealDataPos), pHrHisData->pQueryMac, LENTH_MAC ) )
				{
					//MAC
					memcpy( (pHrHisData->pHrData[nHrDataROW])+nGetDataLen, PSOFFSET(pGetFileData,nDealDataPos), LENTH_MAC );
					nGetDataLen += LENTH_MAC;
					nDealDataPos +=LEN_MAC;
					
					//IP
					memcpy( (pHrHisData->pHrData[nHrDataROW])+nGetDataLen, PSOFFSET(pGetFileData,nDealDataPos), LENTH_IP );
					nGetDataLen += LENTH_IP;
					nDealDataPos +=LEN_IP;

					//Type
					memcpy( (pHrHisData->pHrData[nHrDataROW])+nGetDataLen, PSOFFSET(pGetFileData,nDealDataPos), LENTH_TYPE );
					nGetDataLen += LENTH_TYPE;
					nDealDataPos +=LEN_TYPE;

					//software ver
					memcpy( (pHrHisData->pHrData[nHrDataROW])+nGetDataLen, PSOFFSET(pGetFileData,nDealDataPos), LENTH_VER );
					nGetDataLen += LENTH_VER;
					nDealDataPos +=LEN_VER;

					// AutoPowerOff
					memcpy( cBufChToBy1, pGetFileData+nDealDataPos, LENTH_AUTOPOWEROFF );
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = atoi(cBufChToBy1);
					nGetDataLen++;
					nDealDataPos +=(LENTH_AUTOPOWEROFF+1);

					// LampOnTime
					// hour min second
					memcpy( cBufChToBy1, pGetFileData+nDealDataPos, (LENTH_LAMPONOFF/3-1) );
					nDealDataPos +=(LENTH_LAMPONOFF/3);
					memcpy( cBufChToBy2, pGetFileData+nDealDataPos, (LENTH_LAMPONOFF/3-1) );
					nDealDataPos +=(LENTH_LAMPONOFF/3);
					memcpy( cBufChToBy3, pGetFileData+nDealDataPos, (LENTH_LAMPONOFF/3-1) );
					nDealDataPos +=(LENTH_LAMPONOFF/3+1);
					dwValue = (3600*atoi(cBufChToBy1))+(60*atoi(cBufChToBy2))+atoi(cBufChToBy3);
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(LOWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(LOWORD(dwValue));
					nGetDataLen++;
					memset( cBufChToBy1, 0x00, sizeof(cBufChToBy1) );
					memset( cBufChToBy2, 0x00, sizeof(cBufChToBy2) );
					memset( cBufChToBy3, 0x00, sizeof(cBufChToBy3) );
					
					// LampOffTime
					// hour min second
					memcpy( cBufChToBy1, pGetFileData+nDealDataPos, (LENTH_LAMPONOFF/3-1) );
					nDealDataPos +=(LENTH_LAMPONOFF/3);
					memcpy( cBufChToBy2, pGetFileData+nDealDataPos, (LENTH_LAMPONOFF/3-1) );
					nDealDataPos +=(LENTH_LAMPONOFF/3);
					memcpy( cBufChToBy3, pGetFileData+nDealDataPos, (LENTH_LAMPONOFF/3-1) );
					nDealDataPos +=(LENTH_LAMPONOFF/3+1);
					dwValue = (3600*atoi(cBufChToBy1))+(60*atoi(cBufChToBy2))+atoi(cBufChToBy3);
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(LOWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(LOWORD(dwValue));
					nGetDataLen++;
					memset( cBufChToBy1, 0x00, sizeof(cBufChToBy1) );
					memset( cBufChToBy2, 0x00, sizeof(cBufChToBy2) );
					memset( cBufChToBy3, 0x00, sizeof(cBufChToBy3) );

					// EcoMode
					for ( nCnt=0; nCnt<SUM_ECOMODE; nCnt++)
					{
						if ( 0 == memcmp( EcoModeSW[nCnt], pGetFileData+nDealDataPos, LENTH_ECOMODE ) )
						{
							if( 3 == nCnt )
							{
								nCnt = 0x10;
							}
							else if( 4 == nCnt )
							{
								nCnt = 0x20;
							}
							break;
						}
					}
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = nCnt;
					nGetDataLen++;
					nDealDataPos +=(LENTH_ECOMODE+1);

					// StandbyMode
					for ( nCnt=0; nCnt<SUM_STANDBYMODE; nCnt++)
					{
						if ( 0 == memcmp( StandbyModeSW[nCnt], pGetFileData+nDealDataPos, LENTH_STANDBYMODE ) )
						{
							break;
						}
					}
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = nCnt;
					nGetDataLen++;
					nDealDataPos +=(LENTH_STANDBYMODE+1);

					// OnlineStatus
					for ( nCnt=0; nCnt<SUM_ONLINESTATUS; nCnt++)
					{
						if ( 0 == memcmp( OnlineStatusSW[nCnt], pGetFileData+nDealDataPos, LENTH_ONLINESTATUS ) )
						{
							break;
						}
					}
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = nCnt;
					nGetDataLen++;
					nDealDataPos +=(LENTH_ONLINESTATUS+1);

					// Install
					for ( nCnt=0; nCnt<SUM_INSTALLSTATUS; nCnt++)
					{
						if ( 0 == memcmp( InstallSW[nCnt], pGetFileData+nDealDataPos, LENTH_INSTALL ) )
						{
							break;
						}
					}
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = nCnt;
					nGetDataLen++;
					nDealDataPos +=(LENTH_INSTALL+1);

					// HeatCycle
					memcpy( cBufChToBy1, pGetFileData+nDealDataPos, LENTH_HEATRUNCYCLE );
					dwValue = atoi(cBufChToBy1);
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(LOWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(LOWORD(dwValue));
					nGetDataLen++;
					memset( cBufChToBy1, 0x00, sizeof(cBufChToBy1) );
					nDealDataPos +=(LENTH_HEATRUNCYCLE+1);
					
					// ImagePattern (SUM_IMAGEPATTERN个状态轮询)
					for ( int PatternCnt=0; PatternCnt<SUM_IMAGEPATTERN; PatternCnt++ )
					{
						for ( nCnt=0; nCnt<SUM_PATTERNTYPE; nCnt++)
						{
							if ( 0 == memcmp( Pattern[nCnt], pGetFileData+nDealDataPos, LENTH_IMAGEPATTERN ) )
							{
								nDealDataPos +=(LENTH_IMAGEPATTERN+1);
								break;
							}
						}
						*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = nCnt;
						nGetDataLen++;
					}

					// Lamp1Time
					memcpy( cBufChToBy1, pGetFileData+nDealDataPos, LENTH_LAMP1TTIME );
					dwValue = atoi(cBufChToBy1);
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(LOWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(LOWORD(dwValue));
					nGetDataLen++;
					memset( cBufChToBy1, 0x00, sizeof(cBufChToBy1) );
					nDealDataPos +=(LENTH_LAMP1TTIME+1);

					// Lamp1Time
					memcpy( cBufChToBy1, pGetFileData+nDealDataPos, LENTH_LAMP2TTIME );
					dwValue = atoi(cBufChToBy1);
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(HIWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(LOWORD(dwValue));
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(LOWORD(dwValue));
					nGetDataLen++;
					memset( cBufChToBy1, 0x00, sizeof(cBufChToBy1) );
					nDealDataPos +=(LENTH_LAMP2TTIME+1);
					
					//RecordTime(这个是数据记录时间，有别于版本信息，在日期和时间中间加"_"作为区分)
					memcpy( (pHrHisData->pHrData[nHrDataROW])+nGetDataLen, PSOFFSET(pGetFileData,nDealDataPos), LENTH_VER );
					nGetDataLen += LENTH_VER;
					nDealDataPos +=LEN_VER;

					//最后加上查询到数据包的序号标示位
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = HIBYTE(wIndexPacket);
					nGetDataLen++;
					*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = LOBYTE(wIndexPacket);
					nGetDataLen++;
					wIndexPacket++;

					nFramePjSum++;
					//一开始读取的数据长度都是存在最后一行里面
					pHrHisData->nLenFinalData = nGetDataLen;
					//如果一包读取数量到达SUM_PJ_PER_PACKET时，就要换行
					if (nFramePjSum>=SUM_PJ_PER_PACKET)
					{
						//这个'\0'是留给strlen()使用的
						*((pHrHisData->pHrData[nHrDataROW])+nGetDataLen) = '\0';

						//当要换行的时候才要记录普通的一行长度
						pHrHisData->nLenCommonData = pHrHisData->nLenFinalData;

						//同时PJ数据数量和最后一帧数据长度清0 重新开始计数
						nGetDataLen = 2;
						nFramePjSum = 0;
						nHrDataROW++;
					}
				}
				//拷贝完一串有用的数据后就清除提取数据标记 重头开始
				nDealDataPos = 0;
			}
			//不管读取到什么数据，最后都要清除 等待下次数据读取
			memset( pGetFileData, 0x00, sizeof(pGetFileData) );
		}
		//Alian:考虑一下这个在哪里关闭文件读写句柄
		fclose(fp);
	}
	//所有数据提取的工作完成后，要在每帧数据的头两位都加上
	//这次要提取多少条PJ数据
	if( 0 == (wIndexPacket%SUM_PJ_PER_PACKET) )
	{
		nHrDataROW = (wIndexPacket/SUM_PJ_PER_PACKET);		//数据刚好是SUM_PJ_PER_PACKET整数倍
	} 
	else
	{
		nHrDataROW = (wIndexPacket/SUM_PJ_PER_PACKET)+1;	    //数据不是SUM_PJ_PER_PACKET整数倍，需要加上尾数
	}
	//存数据包中的总共包数位
	for( nCnt=0; nCnt<nHrDataROW; nCnt++ )
	{
		*( pHrHisData->pHrData[nCnt])    = HIBYTE(wIndexPacket);
		*((pHrHisData->pHrData[nCnt])+1) = LOBYTE(wIndexPacket);
	}
	//记录需要发个UI的包数
	pHrHisData->nSumHrData = nHrDataROW;
	return 0;
}


int GetFileNameFromDir( PT_HISTORICAL_DATA pHrHisData )
{
	int nIndex = 0;
	bool bFlag = true;
	long file;
	struct _finddata_t stFind;
	//问题点1：因为创建文件和读取文件数据都存在系统路径切换问题，暂时先搁置，待后续添加调试
	//路径切换为对应文件夹
	//_chdir(pDirName);
	if((file=_findfirst("*.txt", &stFind))==-1L)
	{
		return 0;
	}
	//处在条件区域的文件名字记录
	if( (memcmp(pHrHisData->pQueryStart,stFind.name,8)<=0) && (memcmp(stFind.name,pHrHisData->pQueryEnd,8)<=0) )
	{
		memcpy( pHrHisData->pFileName[nIndex++], stFind.name, 12 );
	}
	//如果都比结束时间都大，就不用遍历
	if( memcmp(pHrHisData->pQueryEnd,stFind.name,8)<=0 )
	{
		bFlag = false;
	}
	while( (_findnext(file, &stFind)==0) && bFlag )
	{
		//处在条件区域的文件名字记录
		if( (memcmp(pHrHisData->pQueryStart,stFind.name,8)<=0) && (memcmp(stFind.name,pHrHisData->pQueryEnd,8)<=0) )
		{
			memcpy( pHrHisData->pFileName[nIndex++], stFind.name, 12 );
		}
		//如果都比结束时间都大，直接跳出
		if( memcmp(pHrHisData->pQueryEnd,stFind.name,8)<=0 )
		{
			break;
		}
	
	}
	pHrHisData->nSumFileTxt = nIndex;
	//问题点1：因为创建文件和读取文件数据都存在系统路径切换问题，暂时先搁置，待后续添加调试
	//路径切换为应用程序位置
	//_chdir("..//");
	_findclose(file);
	return 0;
}

//192.168.1.101
int GetIPFromData( char *pSrcIP, char *pDstIP )
{
	char pBuf[3] = {0};
	int nBufCnt = 0;
	int nSRCCnt = 0;
	int nDstCnt = 0;
	while(nSRCCnt<15)
	{
		//读取到'.'是IP间隔，读取到0x20是IP结束符号
		if( '.' == *(pSrcIP+nSRCCnt) || 0x20 == *(pSrcIP+nSRCCnt) || 0x00 == *(pSrcIP+nSRCCnt) )
		{
			pDstIP[nDstCnt] = atoi(pBuf);
			nDstCnt++;
			if ( 0x20 == *(pSrcIP+nSRCCnt) || 0x00 == *(pSrcIP+nSRCCnt) )
			{
				break;
			}
			memset(pBuf,0x00,sizeof(pBuf));
			nSRCCnt++;
			nBufCnt=0;
		}
		else
		{
			pBuf[nBufCnt++] = *(pSrcIP+nSRCCnt);
			nSRCCnt++;
		}
	}
	return nDstCnt;
}

//20151024 17:11:36
int GetVerFromData( char *pSrcVer, char *pDstVer )
{
	int nSRCCnt = 4;
	int nDstCnt = 0; 
	char pbuf[4] = {0};
	//2015 year
	memcpy( pbuf, pSrcVer, nSRCCnt );
	*(pDstVer+nDstCnt) = LOBYTE(atoi(pbuf));
	nDstCnt++;
	*(pDstVer+nDstCnt) = HIBYTE(atoi(pbuf));
	nDstCnt++;
	memset( pbuf, 0x00, sizeof(pbuf) );
	//10 month
	memcpy( pbuf, pSrcVer+nSRCCnt, 2 );
	*(pDstVer+nDstCnt) = LOBYTE(atoi(pbuf));
	nDstCnt++;
	*(pDstVer+nDstCnt) = HIBYTE(atoi(pbuf));
	nDstCnt++;
	memset( pbuf, 0x00, sizeof(pbuf) );
	nSRCCnt+=2;
	//24 day
	memcpy( pbuf, pSrcVer+nSRCCnt, 2 );
	*(pDstVer+nDstCnt) = LOBYTE(atoi(pbuf));
	nDstCnt++;
	*(pDstVer+nDstCnt) = HIBYTE(atoi(pbuf));
	nDstCnt++;
	memset( pbuf, 0x00, sizeof(pbuf) );
	nSRCCnt+=3;
	//17 hour
	memcpy( pbuf, pSrcVer+nSRCCnt, 2 );
	*(pDstVer+nDstCnt) = LOBYTE(atoi(pbuf));
	nDstCnt++;
	*(pDstVer+nDstCnt) = HIBYTE(atoi(pbuf));
	nDstCnt++;
	memset( pbuf, 0x00, sizeof(pbuf) );
	nSRCCnt+=3;
	//11 min
	memcpy( pbuf, pSrcVer+nSRCCnt, 2 );
	*(pDstVer+nDstCnt) = LOBYTE(atoi(pbuf));
	nDstCnt++;
	*(pDstVer+nDstCnt) = HIBYTE(atoi(pbuf));
	nDstCnt++;
	memset( pbuf, 0x00, sizeof(pbuf) );
	nSRCCnt+=3;
	//36 second
	memcpy( pbuf, pSrcVer+nSRCCnt, 2 );
	*(pDstVer+nDstCnt) = LOBYTE(atoi(pbuf));
	nDstCnt++;
	*(pDstVer+nDstCnt) = HIBYTE(atoi(pbuf));
	nDstCnt++;
	return nDstCnt;
}

