#ifndef _HEATRUNDATA_H__
#define _HEATRUNDATA_H__

#define NUM_NAME_LEN 15
#define NUM_INVALID_LEN 20

typedef unsigned short WORD,*PWORD;
typedef unsigned int DWORD,*PDWORD;
typedef unsigned char BYTE,*PBYTE;

typedef struct
{
	WORD year;
	WORD mon;
	WORD day;
	WORD hour;
	WORD min;
	WORD sec;
}SOFTVER,*PSOFTVER;

typedef struct  
{
	WORD MacAddress1;
	WORD MacAddress2;
	WORD MacAddress3;
	WORD MacAddress4;
	WORD MacAddress5;
	WORD MacAddress6;
}MAC,*PMAC;

typedef struct
{
	WORD LampTimeH;
	WORD LampTimeL;
}LAMPTIME,*PLAMPTIME;

typedef struct
{
	char Module[NUM_NAME_LEN];
	char Type[NUM_NAME_LEN];
	SOFTVER sFirmwareTime;
	//char Sn[NUM_NAME_LEN];
	MAC sMacAddress;
	WORD EcoMode;
	WORD PowerStatus;
	WORD StandbyMode;
	WORD ImagePattern;
	LAMPTIME sLampTime;
	WORD AutoPowerOff;
	WORD OnlineStatus;

	BYTE Invalid[NUM_INVALID_LEN];
	struct HEATRUN_DATA *pNext;
}HEATRUN_DATA,*PHEATRUN_DATA;



#endif // _HEATRUNDATA_H__