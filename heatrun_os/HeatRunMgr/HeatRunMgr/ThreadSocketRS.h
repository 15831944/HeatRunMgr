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
	WORD		SocketId;//嵌套字
	char		ConnectCnt;//连接次数
	char		ConnectStatus;//客户端连接状态 start:未连接 connetten：表示已连接
	char		ConnectTime;//这个变量不使用
	WORD		ConnectuTime;//这个变量不使用
	bool		SendFlag;//发送标志位,当发送设备锁
	char		SendData[5000];//modified by ly at 2015112410:26
	char		SendCntLock;
	WORD		SendDataCnt;//modified by ly at 2015112410:26
	char		ReceData[1000];
	char		ReceCntLock;//CCQ取数据的指针
	WORD		ReceDataCnt;//接收数据总长度
	bool		ReceFlag;//接收的标志位
	bool		DebugFlag1;//个人调试使用，勿念
	bool		DebugFlag2;//个人调试使用，勿念
	bool		PowerOnFlag;//1:开机发送完成
	bool		GetStartFlag;//开始查询PJ数据的标志，第一个有效画面设置成功后，立即发生查询命令，此变量=1
	bool		GetSearchFlag;//查询标志，在第二个有效画面设置时，此变量=1，表示查询命令已经收到数据，等待处理
	bool		GetEndFlag;//查询完成标志，一个pattren只有一次查询，当此标志=1，表示已经这次pattern轮循已经查询过，等到pattern=9（即最后一个画面）时候清除。等待下一次pattern轮循
	BYTE		SendTimeOut;//连接超时时间，如果socket堵塞，超时时间到，就立马断开连接，重新连接
	BYTE		TempBuff[200];//临时缓冲区，用于接收PJ回来的查询数据。
	BYTE		TempBuffCnt;//查询数据的缓冲区计数变量。理论上Cnt最大值等于 所有查询命令的返回个数（好像是92个字节）+10个轮循画面的返回值个数（10）+若干个设置命令（例如工厂模式、Pattren模式）差不多110个字节
	BYTE		GetParmCnt;
	BYTE		GetParmTime;
	BYTE		ProcessParmI;//临时变量，I、J和K表示1个字节的变量。
	BYTE		ProcessParmJ;//临时变量，I、J和K表示1个字节的变量。
	BYTE		ProcessParmK;//临时变量，I、J和K表示1个字节的变量。
	BYTE		ProcessParmNum;//临时变量，Num表示循环体，当前执行到第几个，0:MAC 1:FirmwareTime.year 2:FirmwareTime.month 3:FirmwareTime.day 4:FirmwareTime.hour min sec //5:AutoPowerOff 6:EcoMode 7:StandByMode 8:Install 9:Lamp1TimeH 10:Lamp1TimeL 11:Lamp2TimeH 12:Lamp2TimeL 13:end
							   
	BYTE		ProcessParmPoint;//临时变量，偏移指针，//偏移量+3，因为接收回来的数据是1D XX XX 1D XX XX格式。
	WORD		ProcessParmII;//临时变量，II和KK表示2个字节的变量。
	WORD		ProcessParmKK;//临时变量，II和KK表示2个字节的变量。
	WORD		ProcessParmAA;//PJ处理数据的循环体，当AA=1时，表示数据未处理完成。有2种情况退出，1.数据处理到最后一个完成2.指针偏移量大于接收到的数据，
	DWORD		ProcessParmDt;//临时变量，Dt和Dk表示4个字节的变量。用于处理lampontime和lampofftime
	DWORD		ProcessParmDk;//临时变量，Dt和Dk表示4个字节的变量。用于处理lampontime和lampofftime
	bool		MutexSend;//false空闲

}SOCKETADDINFO,*PSOCKETADDINFO;

extern SOCKETADDINFO ClinetAddInfo;
extern SOCKETADDINFO Clinet[100];

extern SOCKETADDINFO ServerPC;
int SocketInit();
void HeatRunOpeartion(int iii);
DWORD WINAPI ThreadProSocketSendData(LPVOID lpParameter);
DWORD WINAPI ThreadProSocketConnect(LPVOID lpParameter);
DWORD WINAPI ThreadManagentSocketStatus(LPVOID lpParameter);

void UART_Printf(const char *format, ...);//打印信息

BYTE Comm_2NumCharToHex ( BYTE uChar_H , BYTE uChar_L );
void DataProcessFormUI(void);
void DataProcessFormUI2(void);
//void PJDataRegister(PT_PJDATA_MGR PjDataMgr);
void DataProcessFormPJ(BYTE iClient);
#endif // _THREADSOCKETRS_H_