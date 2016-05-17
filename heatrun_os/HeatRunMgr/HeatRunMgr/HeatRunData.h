#include <Windows.h>
#ifndef _HEATRUNDATA_H__
#define _HEATRUNDATA_H__
#define NUM_NAME_LEN 15
#define NUM_INVALID_LEN 20
typedef void (*FUN_TIMERSOFT_CALLBACK)(int);			// 软定时器业务处理回调函数类型

typedef enum
{
	Pattern0=0,
	Pattern1,
	Pattern2,
	Pattern3,
	Pattern4,
	Pattern5,
	Pattern6,
	Pattern7,
	Pattern8,
	Pattern9,
	PatternSum
}EPATTERN;

//ImagePattern的数量
#define SUM_IMAGEPATTERN	  (PatternSum)
#define LEN_MAC	              13
#define LEN_IP	              16
#define LEN_TYPE	          16
#define LEN_VER	              18
#define NUM_NAME_LEN          15
#define NUM_INVALID_LEN       20

typedef struct
{
	WORD	wEvent;		//Event type
	DWORD	dwParam;	//Event Paramters
}EVENT_MESSAGE;

typedef struct
{
	BYTE	eType;
	WORD	wPayloadSize;
	WORD	wCRCPacket;
}PACKET_HEADER;

typedef struct
{
	WORD			wID;
	PACKET_HEADER	sHeader;
	EVENT_MESSAGE	sEvent;
}SOCKET_PACKET,*PT_SOCKET_PACKET;

typedef struct
{
	WORD year;
	WORD mon;
	WORD day;
	WORD hour;
	WORD min;
	WORD sec;
}SOFTVER,*PT_SOFTVER;

typedef struct  
{
	WORD	MacAddress1;
	WORD	MacAddress2;
	WORD	MacAddress3;
	WORD	MacAddress4;
	WORD	MacAddress5;
	WORD	MacAddress6;
}MAC,*PT_MAC;

typedef struct  
{
	WORD	IpAddress1;
	WORD	IpAddress2;
	WORD	IpAddress3;
	WORD	IpAddress4;
	WORD	IpAddress5;
	WORD	IpAddress6;
}IP,*PT_IP;

typedef struct  
{
	EVENT_MESSAGE	MacAddress1;
	EVENT_MESSAGE	MacAddress2;
	EVENT_MESSAGE	MacAddress3;
	EVENT_MESSAGE	MacAddress4;
	EVENT_MESSAGE	MacAddress5;
	EVENT_MESSAGE	MacAddress6;
}MACSET,*PT_MACSET;

typedef struct
{
	unsigned char ucInStall;					    //装载		0x01：已经注册		0x00：还未注册
	unsigned char ucStart;						    //启动		0x01：定时器启动	0x00：定时器未启动
	unsigned char ucAutoReload;					    //自动重载	0x01：到时重载		0x00：到时不重载
	int iSetTimeTs;								    //存储软定时器的设定时间
	int iNowTimeTs;								    //软定时器倒计时时间
	FUN_TIMERSOFT_CALLBACK fTimerSoftCallBack;	    //软定时器绑定的业务处理函数
}TIMERSOFT, *PTIMERSOFT;

typedef struct  
{
	int			ClinetID;
	int			PatternTime;
	bool		ImagePattern_Flag;
	bool		FactoryMode_Flag;
	bool		Used_Flag;	//0此节点空闲
	bool		Power_Flag;
	bool		Pattern_Flag;
	DWORD		LampOnOffTime;
	EPATTERN	Pattern;

}OPTION,*POPTION;

typedef struct
{
	char    MacAddress[LEN_MAC];
	char    IpAddress[LEN_IP];
	char    Type[LEN_TYPE];
	char    FirmwareTime[LEN_VER];
	BYTE    AutoPowerOff;
	BYTE    EcoMode;           //0-Normal, 1-Eco1, 2-Eco2, 16-Intelligent Eco, 32-Saver
	BYTE    StandbyMode;
	BYTE    OnlineStatus;
	BYTE    Install;
	BYTE    ImagePattern[SUM_IMAGEPATTERN];
	DWORD   LampOnTime;
	DWORD   LampOffTime;	      //0代表无穷大
	DWORD   HeatCycle;
	DWORD   Lamp1Time;
	DWORD   Lamp2Time;
	BYTE	RunStatus;
}DATA_MGR,*PT_DATA_MGR;


typedef struct
{
 	DATA_MGR		        m_HrDataGet;
 	//DATA_MGR				m_HrDataSet;
 	TIMERSOFT				m_TimerMgr;
 	OPTION					m_Option;
 	
}PJDATA_MGR,*PT_PJDATA_MGR;


#endif // _HEATRUNDATA_H__