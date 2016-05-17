#ifndef _HAETRUNRDATAFILEMGR_H_
#define _HAETRUNRDATAFILEMGR_H_
#include "ThreadProcess.h"
#include "HeatRunData.h"

//�ϻ��������ݵĳ���
#define LENTH_MAC	          (LEN_MAC-1)
#define LENTH_IP	          (LEN_IP-1)
#define LENTH_TYPE	          (LEN_TYPE-1)
#define LENTH_VER	          (LEN_VER-1)
#define LENTH_AUTOPOWEROFF	  2
#define LENTH_LAMPONOFF	      12
#define LENTH_ECOMODE 	      (sizeof(EcoModeSW[0])-1)
#define LENTH_STANDBYMODE	  (sizeof(StandbyModeSW[0])-1)
#define LENTH_ONLINESTATUS	  (sizeof(OnlineStatusSW[0])-1)
#define LENTH_INSTALL         (sizeof(InstallSW[0])-1)
#define LENTH_HEATRUNCYCLE    10
#define LENTH_IMAGEPATTERN    (sizeof(Pattern[0])-1)
#define LENTH_LAMP1TTIME      7
#define LENTH_LAMP2TTIME      7
#define LENTH_RECORDTIME      17

//���ϻ�����״̬����
#define SUM_PATTERNTYPE       (sizeof(Pattern)/sizeof(Pattern[0]))
#define SUM_ECOMODE           (sizeof(EcoModeSW)/sizeof(EcoModeSW[0]))
#define SUM_STANDBYMODE       (sizeof(StandbyModeSW)/sizeof(StandbyModeSW[0]))
#define SUM_ONLINESTATUS      (sizeof(OnlineStatusSW)/sizeof(OnlineStatusSW[0]))
#define SUM_INSTALLSTATUS     (sizeof(InstallSW)/sizeof(InstallSW[0]))

//Pattern������������

//����TXT�ļ��и������ݵĳ���
//< MAC | IP | TYPE	| VER |APOFF|LAMPONOFF|LAMPONOFF| ECOMODE |STANDBYMODE|ONLINE| INSTALL |HEATRUNCYCLE|IMAGEPATTERN|LAMP1TTIME|LAMP2TTIME|RECORDTIME>\n
//1+12 +1+15+1+ 15 +1+17 +1+ 2 +1+  12   +1+   12  +1+   6   +1+   6     +1+ 7  +1+   6   +1+   10     +1+  10*10    +1+   7    +1+   7    +1+   17   +1+1
//����TXT�ļ���һ�����ݵĳ���(һ��һ��258���ַ�)����������ռ��ǻ�Ӱ���ջ���˺궨�巽��ʹ��������
// #define LENTH_ROWDATA     LENTH_MAC+LENTH_IP+LENTH_TYPE+LENTH_VER+LENTH_AUTOPOWEROFF \
// 	                      +(2*LENTH_LAMPONOFF)+LENTH_ECOMODE+LENTH_STANDBYMODE+LENTH_ONLINESTATUS \
// 						  +LENTH_INSTALL+LENTH_HEATRUNCYCLE+(LENTH_IMAGEPATTERN*SUM_IMAGEPATTERN) \
// 						  +LENTH_LAMP1TTIME+LENTH_LAMP2TTIME+LENTH_RECORDTIME+26  //ÿһ������ǰ���зָ�� ����Ҫ���� 1'<'+11'|'+10*1'|'+3'|'+1'>'+1\n
#define LENTH_ROWDATA     268    //ÿ��һ��Pattern���Ǽ� ��Nothing__|��10λ ԭ258->258+1*10λ

#define FLAG_INTERVAL(buf,len,flag)		{ buf[len] = flag;  \
	                                      len += 1;}
#define PSOFFSET(ps,bit)	    (ps + bit)	

//��ʷ�ļ�����ά����
#define SUM_FILENAME 10            // ��UI��ͨ�ã������ȡ7������
#define LENTH_FILECHAR 25
//���͸�UI��һ������Ҫ������̨PJ����
#define SUM_PJ_PER_PACKET    10
//��Ҫ���͸�UI�Ļ���������
#define SUM_HISDATA          1024
#define LENTH_DATA_PER_PJ    113            //һ����ʷ���ݳ���
//2BYTE ��ʷ�����ܰ��� + 10��*һ����ʷ���ݳ���(113char) + Ԥ������(10char)
#define LENTH_UIFRAMEDATA    (2+SUM_PJ_PER_PACKET*LENTH_DATA_PER_PJ+10) //Ԥ��ʮλ���ݣ��������

//UI��ѯ����ʷ���ݹ���ṹ��
typedef struct
{
	char pQueryStart[LENTH_FILECHAR];                     //��ѯ�Ŀ�ʼʱ��  1y
	char pQueryEnd[LENTH_FILECHAR];                       //��ѯ�Ľ���ʱ��  ly
	char pQueryMac[LEN_MAC];                              //Ҫ��ѯPJ��MAC��ַ  ly
	char pFileName[SUM_FILENAME][LENTH_FILECHAR];         //��Ҫ���ҵ��ļ�������,��UI��ͨ�ã������ȡ7������
	char pHrData[SUM_HISDATA][LENTH_UIFRAMEDATA];         //��Ҫ���͸�UI��ʾ����ʷ�ϻ����ݻ�����
	bool bUIQueryHisData;                                 //�Ƿ���UI������ʷ�����ڼ� true����UI��ȡ��ʷ�����ڼ� false���ڿ�����  ly
	bool bSendFinish;									  //false�������/δ����,true���ڷ���
	int  nSendToUIIndex;                                  //���͸�UI�Ķ�ά�����±�
	int  nSumFileTxt;                                     //���ҵ��ļ�����
	int  nSumHrData;                                      //��Ҫ���͸�UI��ʾ����ʷ�ϻ����ݵİ���
	int  nLenCommonData;                                  //���ݳ��ȷ����� 1.�����һ�����ݵ����ݳ���
	int  nLenFinalData;                                  //���ݳ��ȷ����� 2.���һ�����ݵ����ݳ���
}HISTORICAL_DATA,*PT_HISTORICAL_DATA;

extern char Pattern[][10];


/**********************************************************
��	  ��: 
		���յ�ǰ���ڴ򿪻򴴽��ļ���ͬʱ����������д���ļ�
����:		 
		ListHead     �� ���ݹ�������ͷ
�� �� ��:				����ʱ��: 
		alian					2015.10.28
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0  ʧ�ܷ���?
***********************************************************/
int SaveHrDataToFile( void *ListHead);


/**********************************************************
��	  ��: 
		���ļ��ж�ȡ����Ҫ����ϻ���������
����:		 
		pHrHisData �� �ϻ���ʷ���ݹ���ṹ��
�� �� ��:				����ʱ��: 
		alian					2013.05.30
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ�����0
***********************************************************/
int GetHRDataFromFile( PT_HISTORICAL_DATA pHrHisData );


/**********************************************************
��	  ��: 
		���ݸ�����ʱ�䷶Χ��ȡ�ļ����ֵ�������
����:		 
		pHrHisData �� �ϻ���ʷ���ݹ���ṹ��
�� �� ��:				����ʱ��: 
		alian					2013.05.30
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 �ɹ��Żط���Ҫ���ļ�������
***********************************************************/
int GetFileNameFromDir( PT_HISTORICAL_DATA pHrHisData );


/**********************************************************
��	  ��: 
		����������ȡIPת����4λBYTE����
����:		 
		pSrcIP   �� IP�ַ�������ָ��"192.168.1.101"
		pDstIP   �� ת����char����
�� �� ��:				����ʱ��: 
		alian					2013.05.30
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 ����ת����BYTE��λ��
***********************************************************/
int GetIPFromData( char *pSrcIP, char *pDstIP );


/**********************************************************
��	  ��: 
		����������ȡVERת����12λBYTE����
����:		 
		pSrcVer   �� IP�ַ�������ָ��"20150912 12:36:23"
		pDstVer   �� ת����char����
�� �� ��:				����ʱ��: 
		alian					2013.05.30
�� �� ��:				�޸�ʱ��: 
		��					    ��
����ֵ:
		 ����ת����BYTE��λ��
***********************************************************/
int GetVerFromData( char *pSrcVer, char *pDstVer );


#endif // _HAETRUNRDATAFILEMGR_H_

