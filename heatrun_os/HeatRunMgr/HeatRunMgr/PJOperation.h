#ifndef _PJOPERATION_H__
#define _PJOPERATION_H__
#define LEN_PJDATA 94

typedef enum
{
	PatternNT = 0,
	PatternR0, 
	PatternG0,
	PatternB0,
	PatternW0,
	PatternR32,
	PatternG32,
	PatternB32,
	PatternW32,
	PatternR64,
	PatternG64,
	PatternB64,
	PatternW64,
	PatternR96,
	PatternG96,
	PatternB96,
	PatternW96,
	PatternR168,
	PatternG168,
	PatternB168,
	PatternW168,
	PatternR192,
	PatternG192,
	PatternB192,
	PatternW192,
	PatternR200,
	PatternG200,
	PatternB200,
	PatternW200,
	PatternR208,
	PatternG208,
	PatternB208,
	PatternW208,
	PatternR255,
	PatternG255,
	PatternB255,
	PatternW255,
	PatternGGSWN,
	PatternGGSWR,
	SUMPattern
}IMAGEPATTERN;

typedef struct
{
	BYTE nCountPack;
	BYTE IdDataPack;
	BOOL Finish_Flag;
	char PJData[LEN_PJDATA+2];
}REFRESH_DATA;


void PackCommand(int iClinet, BYTE CommanBuffer[13]);

void ImagePatternOperation(int iClinet, PT_PJDATA_MGR PjDataMgr);
void ImageOperation(int iClinet, BYTE ImagePattern);
void HeatRunOperation(PT_PJDATA_MGR PjDataMgrTemp);
void GetPJStatus(int iClinet);
void SetPJStatus(int iClinet, PT_PJDATA_MGR PjDataMgr);
int PJPause(char* strData);
int PJStart(char* strData);
void SaveGetData(int iClinet, PT_PJDATA_MGR PjDataMgr);
char XORCalcContinuation(char *pData, int length);
void PJDataDel(char* strData);
WORD CRC16CalcContinuation(PBYTE pacData, WORD wCount, WORD wCRC);
REFRESH_DATA HrDataProcess(REFRESH_DATA SendStatus);
void StatusToUI(char IP[15], BYTE DataType);
void PackData(char *tarBuffer, char *srcData, char DataType, int srcLength);
void PJDataRegister(PT_PJDATA_MGR PjDataMgr);

#endif
