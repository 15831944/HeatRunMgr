#ifndef _GLOBAL_H_
#define _GLOBAL_H_
#include <Windows.h>
#include "TimerSoft.h"
#include "SimpleList.h"
#include "HeatRunDataFileMgr.h"

//��ӡ��DOS���ڵĵ������ݿ�
//#define DEBUG_DOS

//��ӡ�����ڵĵ������ݿ�
//#define DEBUG_RS232

//ϵͳ����ʱ��  1000����1s  2000����2s
#define NUM_SYSTEMTICK        1000 

//д�ϻ��������ݵ������ļ�������ʱ��
#define NUM_SAVEHRDATATICK    1000*60*10

//�鹹�������ݵ��Կ�
//#define DEBUG_MANUAL_INIT

//�ļ�������ȡ���Կ�
//#define DEBUG_READ_HRFILE_DATA

//�������꿪�ر�ʾֹͣ��¼�ϻ�����
//#define DEBUG_CLOSE_SAVEDATA

//�߳̾��ȫ�ֻ�
//ϵͳ��ʱ���߳̾��
extern HANDLE g_hThreadSystemTimer;
//��д�ϻ������ļ��߳̾��
extern HANDLE g_hThreadRWFileData;
//Socktet�߳̾��
extern HANDLE g_hThreadReceData;
extern HANDLE g_hThreadSendData;
extern HANDLE g_hThreadConnect;

//ˢ���߳̾��
extern HANDLE g_hThreadRefreshData;

//������Դ����ȫ�ֱ���������������
//ȫ���ٽ������
//extern CRITICAL_SECTION g_csCriticalSection;
//ȫ���߳������
//extern HANDLE g_hMutex;

//ȫ���ϻ����ݹ�������ͷ
extern pSmpNode g_pSmpDataMgrListHead;

//ȫ�ַ������汾��Ϣ
extern char *strHrMgrVersion;

//ȫ����Ҫ���͸�UI����ʷ���ݹ���ṹ��ָ��
extern PT_HISTORICAL_DATA g_pHisData;

//extern PT_PJDATA_MGR PjGet[100];
extern PT_PJDATA_MGR PjGet;

extern bool Refresh_Flag;

extern int NUM_PJ;

extern fd_set clientfdset; //����һ��ȫ�ֵ�socket Ƕ������
#endif // _GLOBAL_H_
