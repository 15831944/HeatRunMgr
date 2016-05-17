#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "Global.h"
#include "TimerSoft.h"
#include "SystemInit.h"
#include "ThreadProcess.h"
#include "TimerSoftProcess.h"
#include "HeatRunData.h"
#include "ThreadSocketRS.h"
#include "HeatRunDataFileMgr.h"
using namespace std;

int SystemInit()
{
	PjGetBufferInit();
	SystemParamInit();
	SocketInit();
	ThreadInit();
	return 0;
}
int PjGetBufferInit()
{
	/*
	for (int i=0; i<100; i++)
	{
		PjGet[i] = (PT_PJDATA_MGR)malloc(sizeof(PJDATA_MGR));
		memset(PjGet[i],0,sizeof(PJDATA_MGR));
	}
	*/
	PjGet = (PT_PJDATA_MGR)malloc(sizeof(PJDATA_MGR));
	memset(PjGet,0,sizeof(PJDATA_MGR));
	return 0;
}


int SystemParamInit()
{
#if	defined(DEBUG_MANUAL_INIT)
	//�ֶ��鹹�������ݣ���Ϊ���ݹ������ʹ��
	HeatRunDataInit();
#endif // DEBUG_MANUAL_INIT

	//����һ����ʷ���ݹ���ṹ�壬ϵͳ�˳�ǰ����һ���ͷ�ż
	//alian��������õķ�����UI�����ʱ������ռ䣬����������ݺ��ͷſռ䣬��������Ҫʹ�õ�ʱ������ռ�
	g_pHisData = (PT_HISTORICAL_DATA)malloc(sizeof(HISTORICAL_DATA));
	memset( g_pHisData, 0x00, sizeof(HISTORICAL_DATA) );
#if	defined(DEBUG_READ_HRFILE_DATA)
	//��������ֶ���ȡ�ϻ����ݵ��Կڣ����޶���ǰʱ����ļ���ȡ,����Ҫ����ʵ��������޸�ʱ��
	g_pHisData->bUIQueryHisData = true;
	memcpy( g_pHisData->pQueryStart,"20160107",sizeof(g_pHisData->pQueryStart) );
	memcpy( g_pHisData->pQueryEnd,  "20160107",sizeof(g_pHisData->pQueryEnd) );
	memcpy( g_pHisData->pQueryMac,"7E937F3CFE72",sizeof(g_pHisData->pQueryMac) );
#else
	g_pHisData->bUIQueryHisData = false;
#endif // DEBUG_READ_HRFILE_DATA

	return 0;
}

/*
//������Դ����ȫ�ֱ���������������
int MutexInit()
{
	g_hMutex = CreateMutex(NULL, FALSE, "Mutex");
	return 0;
}
int CriticalSectionInit()
{
	InitializeCriticalSection(&g_csCriticalSection);
	return 0;
}
*/

int ThreadInit()
{
 	g_hThreadSystemTimer = CreateThread(NULL, 0, ThreadSystemTimerProcess, NULL, 0, NULL);
 	g_hThreadReceData	 = CreateThread(NULL, 0, ThreadProSocketConnect, NULL, 0, NULL);//�������߳�
 	g_hThreadSendData	 = CreateThread(NULL, 0, ThreadProSocketSendData, NULL, 0, NULL);//�������߳�
 	g_hThreadReceData	 = CreateThread(NULL, 0, ThreadManagentSocketStatus, NULL, 0, NULL);//�������߳�
	g_hThreadRWFileData  = CreateThread(NULL, 0, ThreadRWMgrFileDataProcess, NULL, 0, NULL);
	g_hThreadRefreshData = CreateThread(NULL, 0, ThreadHrDataProcess, NULL, 0, NULL);

	return 0;
}

int SocketInit()
{
	//Clinet[0].sin_addr.S_un.S_addr = inet_addr("192.168.1.101");
//	Clinet[0].sin_addr.S_un.S_addr = inet_addr("10.97.24.158");
	Clinet[0].ClinetHtons=htons(56789);
//	Clinet[0].ClinetHtons=htons(25);
	Clinet[0].ConnectStatus=Start;
	Clinet[0].SocketId=0;
	Clinet[0].ConnectCnt=0;
	Clinet[0].ConnectTime=0;
	Clinet[0].ConnectuTime=5000;
	Clinet[0].SendCntLock=0;
	Clinet[0].SendDataCnt=0;
	memset(&ServerPC, 0, sizeof(ServerPC));
	return 0;
}

void HeatRunDataInit()
{
	int nCount = 0;
	PT_PJDATA_MGR Pj[10];
	//������ռ䲻�У�������ı�����ȥ��û����
	for( nCount=0; nCount<10; nCount++ )
	{
		Pj[nCount] = (PT_PJDATA_MGR)malloc(sizeof(PJDATA_MGR));
		PJHeatRunParamInit( Pj[nCount], nCount );
	}
	//��Ҫ��ʼ����ЩͶӰ���ϻ���������
	g_pSmpDataMgrListHead = SMPLIST_init(Pj[0]);
	for( nCount=1; nCount<10; nCount++ )
	{
		SMPLIST_addnode( Pj[nCount], g_pSmpDataMgrListHead );
	}
}

int PJHeatRunParamInit( PT_PJDATA_MGR pDataMgr, int nIndex )
{
	char MacAddress[][LEN_MAC] = {
		"7E937F3CFE72",
		"7E237F3CFE73",
		"7E93745CFE74",
		"7E117F3CFE75",
		"7E93743CFE76",
		"7E45BB78F176",
		"7E56BA3C7721",
		"7ED2A13C7E34",
		"7E93743CFE33",
		"7E9EE03CFA59",
	};
	char IpAddress[][LEN_IP] = {
		"192.168.1.101",
		"192.168.1.102",
		"192.168.1.103",
		"192.168.1.104",
		"192.168.1.105",
		"192.168.1.106",
		"192.168.1.107",
		"192.168.1.108",
		"192.168.1.109",
		"192.168.1.110",
	};
	char Type[][LEN_TYPE] = {
		"CP-TW2503EF",
		"HMP-TW2503EF",
		"LWU701i",
		"CP-WU9411GF",
		"HCP-Q180",
		"DVH-WX400C",
		"SNP-EW400C",
		"HMP-4000WX",
		"CP-X8170J",
		"TCP-D1070W",
	};
	char FirmwareTime[][LEN_VER] = {
		"20150912_12:36:23",
		"20151134_08:23:14",
		"20151024_17:11:36",
		"20140430_19:37:09",
		"20130328_21:11:45",
		"20140417_23:56:59",
		"20150528_00:04:33",
		"20110616_02:00:24",
		"20090704_07:34:43",
		"20120829_11:21:56",
	};
	BYTE ImagePattern[][SUM_IMAGEPATTERN] = {
		{0x00,0x1F,0x0F,0x03,0x1C,0x1D,0x1E,0x24,0x25,0x25},
		{0x00,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x25},
		{0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x25},
		{0x00,0x1F,0x0F,0x03,0x1C,0x1D,0x1E,0x24,0x25,0x25},
		{0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x25},
		{0x00,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x25},
		{0x00,0x1F,0x0F,0x03,0x1C,0x1D,0x1E,0x24,0x25,0x25},
		{0x00,0x1F,0x0F,0x03,0x1C,0x1D,0x1E,0x24,0x25,0x25},
		{0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x25},
		{0x00,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x25},
	};

	//��Ӧָ��λ��д���ʼ������
	//MAC
	memset( pDataMgr->m_HrDataGet.MacAddress, 0x00, sizeof(pDataMgr->m_HrDataGet.MacAddress) );
	memcpy( pDataMgr->m_HrDataGet.MacAddress, MacAddress[nIndex], strlen(MacAddress[nIndex]) );
	//IP
	memset( pDataMgr->m_HrDataGet.IpAddress, 0x00, sizeof(pDataMgr->m_HrDataGet.IpAddress) );
	memcpy( pDataMgr->m_HrDataGet.IpAddress, IpAddress[nIndex], strlen(IpAddress[nIndex]) );
	//Type
	memset( pDataMgr->m_HrDataGet.Type, 0x00, sizeof(pDataMgr->m_HrDataGet.Type) );
	memcpy( pDataMgr->m_HrDataGet.Type, Type[nIndex], strlen(Type[nIndex]) );
	//software ver
	memset( pDataMgr->m_HrDataGet.FirmwareTime, 0x00, sizeof(pDataMgr->m_HrDataGet.FirmwareTime) );
	memcpy( pDataMgr->m_HrDataGet.FirmwareTime, FirmwareTime[nIndex], strlen(FirmwareTime[nIndex]) );
	//ImagePattern
	memset( pDataMgr->m_HrDataGet.ImagePattern, 0x00, sizeof(pDataMgr->m_HrDataGet.ImagePattern) );
	memcpy( pDataMgr->m_HrDataGet.ImagePattern, ImagePattern[nIndex], sizeof(pDataMgr->m_HrDataGet.ImagePattern) );
	//AutoPowerOff
	pDataMgr->m_HrDataGet.AutoPowerOff = 0x63;
	//ʡ��ģʽ
	pDataMgr->m_HrDataGet.EcoMode  = 0x01;
	//Standby Mode
	pDataMgr->m_HrDataGet.StandbyMode  = 0x00;
	//����״̬
	pDataMgr->m_HrDataGet.OnlineStatus = 0x01;
	//��װ��ʽ
	pDataMgr->m_HrDataGet.Install = 0x01;
	//LAMP ON
	switch(nIndex)
	{
	case 0:
		pDataMgr->m_HrDataGet.LampOnTime = 0x1C20;
		break;
	case 1:
		pDataMgr->m_HrDataGet.LampOnTime = 0x1518;
		break;
	case 2:
		pDataMgr->m_HrDataGet.LampOnTime = 0x1644;
		break;
	case 3:
		pDataMgr->m_HrDataGet.LampOnTime = 0x2C8BA;
		break;
	case 4:
		pDataMgr->m_HrDataGet.LampOnTime = 0x88;
		break;
	case 5:
		pDataMgr->m_HrDataGet.LampOnTime = 0x1C20;
		break;
	case 6:
		pDataMgr->m_HrDataGet.LampOnTime = 0x1518;
		break;
	case 7:
		pDataMgr->m_HrDataGet.LampOnTime = 0x1644;
		break;
	case 8:
		pDataMgr->m_HrDataGet.LampOnTime = 0x2C8BA;
		break;
	case 9:
		pDataMgr->m_HrDataGet.LampOnTime = 0x88;
		break;
	default:
		break;
	}
	//LAMP OFF
	switch(nIndex)
	{
	case 0:
		pDataMgr->m_HrDataGet.LampOffTime = 0x384;
		break;
	case 1:
		pDataMgr->m_HrDataGet.LampOffTime = 0x1C20;
		break;
	case 2:
		pDataMgr->m_HrDataGet.LampOffTime = 0x1644;
		break;
	case 3:
		pDataMgr->m_HrDataGet.LampOffTime = 0x1518;
		break;
	case 4:
		pDataMgr->m_HrDataGet.LampOffTime = 0x2C8BA;
		break;
	case 5:
		pDataMgr->m_HrDataGet.LampOffTime = 0x384;
		break;
	case 6:
		pDataMgr->m_HrDataGet.LampOffTime = 0x1C20;
		break;
	case 7:
		pDataMgr->m_HrDataGet.LampOffTime = 0x1644;
		break;
	case 8:
		pDataMgr->m_HrDataGet.LampOffTime = 0x1518;
		break;
	case 9:
		pDataMgr->m_HrDataGet.LampOffTime = 0x2C8BA;
		break;
	default:
		break;
	}
	//HeatRun Cycle
	pDataMgr->m_HrDataGet.HeatCycle  = 0x3E8;
	//Lamp1 Time
	pDataMgr->m_HrDataGet.Lamp1Time  = 0x6DDD00;
	//Lamp2 Time
	pDataMgr->m_HrDataGet.Lamp2Time  = 0x00;

	//�ϻ�����Option����������״̬��Ҫ����ʼ��
	pDataMgr->m_Option.ImagePattern_Flag = true;
	pDataMgr->m_Option.FactoryMode_Flag = true;
	pDataMgr->m_Option.Used_Flag = true;
	pDataMgr->m_Option.Pattern = Pattern0;
	pDataMgr->m_Option.LampOnOffTime = 0;
	pDataMgr->m_Option.Power_Flag = false;
	pDataMgr->m_Option.Pattern_Flag = false;
	pDataMgr->m_Option.PatternTime = 2;

	return 0;
}