//#include <winsock2.h>
#ifndef _THREADSOCKETRS_H_
#define _THREADSOCKETRS_H_

// #ifdef BUS_GLOBALS
// #define BUS_EXT
// #else
// #define BUS_EXT  extern 
// #endif 

enum ConnectStatus
{
	Start=0,
	Connectting,
	Connectted,
};

enum HeatRunStatus{
	HeatRunNull=0,
	HeatRunStart,
	HeatRunPowerON,
	HeatRunPowerOff,
	HeatRunWhitePlane,

};

typedef struct 
{
	struct in_addr	sin_addr;
	WORD		ClinetHtons;
	WORD		SocketId;//Ƕ����
	char		ConnectCnt;//���Ӵ���
	char		ConnectStatus;//�ͻ�������״̬ start:δ���� connetten����ʾ������
	char		ConnectTime;//���������ʹ��
	WORD		ConnectuTime;//���������ʹ��
	bool		SendFlag;//���ͱ�־λ,�������豸��
	char		SendData[5000];//modified by ly at 2015112410:26
	char		SendCntLock;
	WORD		SendDataCnt;//modified by ly at 2015112410:26
	char		ReceData[1000];
	char		ReceCntLock;//CCQȡ���ݵ�ָ��
	WORD		ReceDataCnt;//���������ܳ���
	bool		ReceFlag;//���յı�־λ
	bool		DebugFlag1;//���˵���ʹ�ã�����
	bool		DebugFlag2;//���˵���ʹ�ã�����
	bool		PowerOnFlag;//1:�����������
	bool		GetStartFlag;//��ʼ��ѯPJ���ݵı�־����һ����Ч�������óɹ�������������ѯ����˱���=1
	bool		GetSearchFlag;//��ѯ��־���ڵڶ�����Ч��������ʱ���˱���=1����ʾ��ѯ�����Ѿ��յ����ݣ��ȴ�����
	bool		GetEndFlag;//��ѯ��ɱ�־��һ��pattrenֻ��һ�β�ѯ�����˱�־=1����ʾ�Ѿ����pattern��ѭ�Ѿ���ѯ�����ȵ�pattern=9�������һ�����棩ʱ��������ȴ���һ��pattern��ѭ
	BYTE		SendTimeOut;//���ӳ�ʱʱ�䣬���socket��������ʱʱ�䵽��������Ͽ����ӣ���������
	BYTE		TempBuff[200];//��ʱ�����������ڽ���PJ�����Ĳ�ѯ���ݡ�
	BYTE		TempBuffCnt;//��ѯ���ݵĻ���������������������Cnt���ֵ���� ���в�ѯ����ķ��ظ�����������92���ֽڣ�+10����ѭ����ķ���ֵ������10��+���ɸ�����������繤��ģʽ��Pattrenģʽ�����110���ֽ�
	BYTE		GetParmCnt;
	BYTE		GetParmTime;
	BYTE		ProcessParmI;//��ʱ������I��J��K��ʾ1���ֽڵı�����
	BYTE		ProcessParmJ;//��ʱ������I��J��K��ʾ1���ֽڵı�����
	BYTE		ProcessParmK;//��ʱ������I��J��K��ʾ1���ֽڵı�����
	BYTE		ProcessParmNum;//��ʱ������Num��ʾѭ���壬��ǰִ�е��ڼ�����0:MAC 1:FirmwareTime.year 2:FirmwareTime.month 3:FirmwareTime.day 4:FirmwareTime.hour min sec //5:AutoPowerOff 6:EcoMode 7:StandByMode 8:Install 9:Lamp1TimeH 10:Lamp1TimeL 11:Lamp2TimeH 12:Lamp2TimeL 13:end
							   
	BYTE		ProcessParmPoint;//��ʱ������ƫ��ָ�룬//ƫ����+3����Ϊ���ջ�����������1D XX XX 1D XX XX��ʽ��
	WORD		ProcessParmII;//��ʱ������II��KK��ʾ2���ֽڵı�����
	WORD		ProcessParmKK;//��ʱ������II��KK��ʾ2���ֽڵı�����
	WORD		ProcessParmAA;//PJ�������ݵ�ѭ���壬��AA=1ʱ����ʾ����δ������ɡ���2������˳���1.���ݴ������һ�����2.ָ��ƫ�������ڽ��յ������ݣ�
	DWORD		ProcessParmDt;//��ʱ������Dt��Dk��ʾ4���ֽڵı��������ڴ���lampontime��lampofftime
	DWORD		ProcessParmDk;//��ʱ������Dt��Dk��ʾ4���ֽڵı��������ڴ���lampontime��lampofftime
	bool		MutexSend;//false����

}SOCKETADDINFO,*PSOCKETADDINFO;

extern SOCKETADDINFO ClinetAddInfo;
extern SOCKETADDINFO Clinet[100];

extern SOCKETADDINFO ServerPC;
int SocketInit();
void HeatRunOpeartion(int iii);
DWORD WINAPI ThreadProSocketSendData(LPVOID lpParameter);
DWORD WINAPI ThreadProSocketConnect(LPVOID lpParameter);
DWORD WINAPI ThreadManagentSocketStatus(LPVOID lpParameter);

void UART_Printf(const char *format, ...);//��ӡ��Ϣ

BYTE Comm_2NumCharToHex ( BYTE uChar_H , BYTE uChar_L );
void DataProcessFormUI(void);
void DataProcessFormUI2(void);
//void PJDataRegister(PT_PJDATA_MGR PjDataMgr);
void DataProcessFormPJ(BYTE iClient);
#endif // _THREADSOCKETRS_H_