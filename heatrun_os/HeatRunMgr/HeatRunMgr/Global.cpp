#include <Windows.h>
#include "Global.h"
#include "HeatRunDataFileMgr.h"


//����ȫ���߳̾��
HANDLE g_hThreadSystemTimer = INVALID_HANDLE_VALUE;
//�����д�ϻ������ļ��߳̾��
HANDLE g_hThreadRWFileData = INVALID_HANDLE_VALUE;
//����Socktet�߳̾��
HANDLE g_hThreadReceData = INVALID_HANDLE_VALUE;
HANDLE g_hThreadSendData = INVALID_HANDLE_VALUE;
HANDLE g_hThreadConnect = INVALID_HANDLE_VALUE;

//ˢ���߳̾��
HANDLE g_hThreadRefreshData = INVALID_HANDLE_VALUE;

//������Դ����ȫ�ֱ���������������
//�����ٽ������
//CRITICAL_SECTION g_csCriticalSection;
//�����߳������
//HANDLE g_hMutex = INVALID_HANDLE_VALUE;

//����ȫ���ϻ����ݹ�������ͷ
pSmpNode g_pSmpDataMgrListHead = NULL;

//����������汾��Ϣ
char *strHrMgrVersion = "11.12.05";   //Socket : OS : FileRW

//������Ҫ���͸�UI����ʷ���ݹ���ṹ��ָ��
PT_HISTORICAL_DATA g_pHisData = NULL;

bool Refresh_Flag = NULL;

//PT_PJDATA_MGR PjGet[100];//��ʱ��һ��ȫ�ֱ������Ժ���������ȡ

PT_PJDATA_MGR PjGet;

fd_set clientfdset; //����һ��ȫ�ֵ�socket Ƕ������