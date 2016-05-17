#include <stdio.h>
#include <time.h>
#include "SimpleList.h"
#include "HeatRunData.h"

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

int SaveHrDataToFile( void *ListHead)
{
	// 如果链表头指针为空就返回失败  
	if( NULL==ListHead )
	{
		return 0;
	}

	FILE *fp = NULL;
	char cFileName[13] = {0};
	char cTmpBuf[9] = {0};
	char cWDataBuf[4096] = {0};
	int iSumWBuf = 0;
	int nCount = 0;
	TOOL_GetLocalDate( cFileName );
	sprintf(cFileName+8,".txt");
	//打开文件
	fp = fopen( cFileName, "a+" );
	if( fp == NULL ) 
	{
		return 0;
	}

	//现在开始拼接需要写入文件的数据
	//记录时间
	memset(cTmpBuf,0x00,sizeof(cTmpBuf));
	TOOL_GetLocalTime(cTmpBuf);
	memcpy(cWDataBuf,cTmpBuf,8);
	iSumWBuf += 8;
	cWDataBuf[iSumWBuf] = '\n';
	iSumWBuf += 1;
	
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//获取链表节点个数，同时这是本模块全局的
	int nSumListNode = SMPLIST_nodesum( ListHead );

	for ( nCount=1; nCount<=nSumListNode ; nCount++ )
	{
		// 遍历链表的每个节点
		NodeTemp = SMPLIST_getnode( nCount, Head );
		//提取节点数据
		PjDataMgr = (PT_PJDATA_MGR)(NodeTemp->data);
		
		//MAC
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.MacAddress,2);
		iSumWBuf += 2;
		sprintf(cWDataBuf+iSumWBuf,"-");
		iSumWBuf += 1;
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.MacAddress+2,2);
		iSumWBuf += 2;
		sprintf(cWDataBuf+iSumWBuf,"-");
		iSumWBuf += 1;
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.MacAddress+4,2);
		iSumWBuf += 2;
		sprintf(cWDataBuf+iSumWBuf,"-");
		iSumWBuf += 1;
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.MacAddress+6,2);
		iSumWBuf += 2;
		sprintf(cWDataBuf+iSumWBuf,"-");
		iSumWBuf += 1;
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.MacAddress+8,2);
		iSumWBuf += 2;
		sprintf(cWDataBuf+iSumWBuf,"-");
		iSumWBuf += 1;
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.MacAddress+10,2);
		iSumWBuf += 2;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//IP
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.IP,sizeof(PjDataMgr->m_HrDataMgr.IP)-1);
		iSumWBuf += (sizeof(PjDataMgr->m_HrDataMgr.IP)-1);

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//software ver
		memcpy(cWDataBuf+iSumWBuf,PjDataMgr->m_HrDataMgr.sFirmwareTime,sizeof(PjDataMgr->m_HrDataMgr.sFirmwareTime)-1);
		iSumWBuf += (sizeof(PjDataMgr->m_HrDataMgr.sFirmwareTime)-1);

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//AutoPowerOff
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.AutoPowerOff);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//LAMP ON
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.LampOn);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//LAMP OFF
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.LampOff);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//ImagePattern
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.ImagePattern);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//省电模式
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.EcoMode);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//Standby Mode
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.StandbyMode);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//Lamp Time H
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.sLampTime.LampTimeH);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//Lamp Time L
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.sLampTime.LampTimeL);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//开机状态
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.PowerStatus);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//安装方式
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.Install);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//在线状态
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.OnlineStatus);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '\n';
		iSumWBuf += 1;
	}

	if (fp != NULL) 
	{
		fseek( fp, 0, SEEK_END );
		fwrite( cWDataBuf, iSumWBuf, 1, fp );
		fwrite( "\n", 1, 1, fp );
	}

	fclose(fp);

	return 1;
}