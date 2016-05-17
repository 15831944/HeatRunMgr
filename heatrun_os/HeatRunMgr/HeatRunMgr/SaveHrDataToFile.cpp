#include <stdio.h>
#include <time.h>
#include "SimpleList.h"
#include "HeatRunData.h"

//��ȡϵͳ��ǰʱ��
int TOOL_GetLocalDate( char v_cLocalDate[9] )
{
	int year, month, day;
	struct tm *timeinfo;			//����ʱ��ṹ�� 
	time_t nowtime;  
	time( &nowtime );				//��ȡʱ��   
	timeinfo = localtime( &nowtime );	//ת��Ϊ����ʱ��  
	year = timeinfo->tm_year + 1900;	//��1900�꿪ʼ����������+1900 
	month = timeinfo->tm_mon + 1;		//��0��ʼ����������+1 
	day = timeinfo->tm_mday; 
	memset(v_cLocalDate,0x00,sizeof(v_cLocalDate));
	sprintf( v_cLocalDate, "%04d%02d%02d",year,month,day);  
	return 0;
}

int TOOL_GetLocalTime( char v_cLocalTime[9] )
{
	int hour, min, sec;
	struct tm *timeinfo;			//����ʱ��ṹ�� 
	time_t nowtime;  
	time( &nowtime );				//��ȡʱ��   
	timeinfo = localtime( &nowtime );	//ת��Ϊ����ʱ��  
	hour = timeinfo->tm_hour; 
	min = timeinfo->tm_min; 
	sec = timeinfo->tm_sec; 
	memset(v_cLocalTime,0x00,sizeof(v_cLocalTime));
	sprintf( v_cLocalTime, "%02d:%02d:%02d", hour, min, sec);  
	return 0;
}

int SaveHrDataToFile( void *ListHead)
{
	// �������ͷָ��Ϊ�վͷ���ʧ��  
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
	//���ļ�
	fp = fopen( cFileName, "a+" );
	if( fp == NULL ) 
	{
		return 0;
	}

	//���ڿ�ʼƴ����Ҫд���ļ�������
	//��¼ʱ��
	memset(cTmpBuf,0x00,sizeof(cTmpBuf));
	TOOL_GetLocalTime(cTmpBuf);
	memcpy(cWDataBuf,cTmpBuf,8);
	iSumWBuf += 8;
	cWDataBuf[iSumWBuf] = '\n';
	iSumWBuf += 1;
	
	pSmpNode Head = (pSmpNode)ListHead;
	pSmpNode NodeTemp = NULL;
	PT_PJDATA_MGR PjDataMgr = NULL;
	//��ȡ����ڵ������ͬʱ���Ǳ�ģ��ȫ�ֵ�
	int nSumListNode = SMPLIST_nodesum( ListHead );

	for ( nCount=1; nCount<=nSumListNode ; nCount++ )
	{
		// ���������ÿ���ڵ�
		NodeTemp = SMPLIST_getnode( nCount, Head );
		//��ȡ�ڵ�����
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

		//ʡ��ģʽ
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

		//����״̬
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.PowerStatus);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//��װ��ʽ
		sprintf(cWDataBuf+iSumWBuf,"%04d",PjDataMgr->m_HrDataMgr.Install);
		iSumWBuf += 4;

		cWDataBuf[iSumWBuf] = '|';
		iSumWBuf += 1;

		//����״̬
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