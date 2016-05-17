#include <Windows.h>
#include <string.h>
#include <stdio.h>
#include "Global.h"
#include "HeatRunData.h"
#include "ThreadSocketRS.h"
#include "PJOperation.h"
#include "ConstData.h"
#include "TimerSoft.h"
#include "SimpleList.h"
#include "TimerSoftProcess.h"
#include "HeatRunDataFileMgr.h"
//#pragma comment(lib, "Winmm.lib")

#define ECO_NORMAL				0x00
#define ECO_ECO1				0x01
#define ECO_ECO2				0x02
#define ECO_ECOINTELLIGENT		0x10
#define ECO_ECOMODESAVER		0x20

#define STANDBY_NORMAL			0x00
#define STANDBY_SAVING			0x01

#define INSTALL_FRONT_DESKTOP	0x00
#define INSTALL_REAR_DESKTOP	 0x01
#define INSTALL_REAR_CEILING	0x02
#define INSTALL_FRONT_CEILING	0x03

static const BYTE  m_acCRCHiArray[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static const BYTE  m_acCRCLoArray[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

void PackCommand(int iClinet, BYTE CommanBuffer[13]) //打包命令
{
	int i=0;
	if (Clinet[iClinet].SendDataCnt<=481)//modified by ly at 2015112410:26
	{
		if (Clinet[iClinet].SendCntLock==0)
		{
			Clinet[iClinet].SendCntLock=1;
			for (i=0;i<13;i++)
			{
				Clinet[iClinet].SendData[Clinet[iClinet].SendDataCnt++]=CommanBuffer[i];
			}
			Clinet[iClinet].SendCntLock=0;
		}	
	}
}


void ImageOperation(int iClinet, BYTE ImagePattern)
{
	WORD wCRC;
	BYTE ImagePatternCommand[13]= {0xBE,0xEF,0x03,0x06,0x00,0x00,0x00,0x01,0x00,0x00,0x15,0x00,0x00};

	ImagePatternCommand[11] = ImagePattern;
	wCRC = CRC16CalcContinuation(ImagePatternCommand, sizeof(ImagePatternCommand), 0xffff);
	ImagePatternCommand[5] = wCRC & 0xff;
	ImagePatternCommand[6] = (wCRC>>8) & 0xff;
	PackCommand(iClinet,ImagePatternCommand);
#if defined(DEBUG_RS232)
	UART_Printf(" %s = %d\n",Pattern[ImagePattern], iClinet);
#endif // DEBUG_RS232
	/*
	if (ImagePattern == 1 || ImagePattern ==5 || 8<= ImagePattern <=23 || 36<= ImagePattern <=83 || 88<= ImagePattern <=91)
	{
		ImagePatternCommand[11] = ImagePattern;
		wCRC = CRC16CalcContinuation(ImagePatternCommand, sizeof(ImagePatternCommand), 0xffff);
		ImagePatternCommand[5] = wCRC & 0xff;
		ImagePatternCommand[6] = (wCRC>>8) & 0xff;
		PackCommand(iClinet,ImagePatternCommand);
#if defined(DEBUG_RS232)
		UART_Printf(" %s = %d\n",PatternA[ImagePattern], iClinet);
#endif // DEBUG_RS232
	}
	else
	{
#if defined(DEBUG_RS232)
		UART_Printf(" Nothing = %d\n",iClinet);
#endif // DEBUG_RS232
	}

	*/

}


int a = 0,b=23,c=1; //a:轮循时间/2; b:GetendFlag标记时间；c:Pattern9 switch

void ImagePatternOperation(int iClinet, PT_PJDATA_MGR PjDataMgr)
{
	int i=0;
	if (PjDataMgr->m_Option.FactoryMode_Flag == false)
	{
		PackCommand(iClinet, SetFactoryMode);
#if defined(DEBUG_RS232)
		UART_Printf("SetFactoryMode = %d\n",iClinet);
#endif // DEBUG_RS232
		PjDataMgr->m_Option.FactoryMode_Flag = true;
	}
	else if (PjDataMgr->m_Option.ImagePattern_Flag == false)
	{
		PackCommand(iClinet, SetPatternModeOn);
#if defined(DEBUG_RS232)
		UART_Printf("SetPatternModeOn = %d\n",iClinet);
#endif // DEBUG_RS232
		PjDataMgr->m_Option.ImagePattern_Flag = true;
	}
	else
	{
		a=0;
		switch(PjDataMgr->m_Option.Pattern)
		{
		case Pattern0:
			if (PjDataMgr->m_HrDataGet.ImagePattern[0]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[0]);
				a=10;
				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
				}
				
				
			}
			PjDataMgr->m_Option.Pattern = Pattern1;
			break;
		case Pattern1:
			if (PjDataMgr->m_HrDataGet.ImagePattern[1]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[1]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
				
			}
			PjDataMgr->m_Option.Pattern = Pattern2;
			break;
		case Pattern2:
			if (PjDataMgr->m_HrDataGet.ImagePattern[2]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[2]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			PjDataMgr->m_Option.Pattern = Pattern3;
			break;
		case Pattern3:
			if (PjDataMgr->m_HrDataGet.ImagePattern[3]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[3]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			PjDataMgr->m_Option.Pattern = Pattern4;
			
			break;
		case Pattern4:
			if (PjDataMgr->m_HrDataGet.ImagePattern[4]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[4]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			PjDataMgr->m_Option.Pattern = Pattern5;
			break;
		case Pattern5:
			if (PjDataMgr->m_HrDataGet.ImagePattern[5]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[5]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			PjDataMgr->m_Option.Pattern = Pattern6;
			break;
		case Pattern6:
			if (PjDataMgr->m_HrDataGet.ImagePattern[6]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[6]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			PjDataMgr->m_Option.Pattern = Pattern7;
			break;
		case Pattern7:
			if (PjDataMgr->m_HrDataGet.ImagePattern[7]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[7]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			PjDataMgr->m_Option.Pattern = Pattern8;
			break;
		case Pattern8:
			if (PjDataMgr->m_HrDataGet.ImagePattern[8]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[8]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}
			
			
			PjDataMgr->m_Option.Pattern = Pattern9;
			break;
		case Pattern9:
			if (PjDataMgr->m_HrDataGet.ImagePattern[9]!= 0)//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
			{
				ImageOperation(iClinet, PjDataMgr->m_HrDataGet.ImagePattern[9]);
				a=10;

				if (Clinet[iClinet].GetEndFlag==0)
				{
					if (Clinet[iClinet].GetStartFlag == false)//判断现在是否有在查询PJ的数据了
					{
						GetPJStatus(iClinet);

						if (PjDataMgr->m_HrDataGet.HeatCycle != 0xffffffff)
						{
							PjDataMgr->m_HrDataGet.HeatCycle++;
						}
						else
						{
							PjDataMgr->m_HrDataGet.HeatCycle = 0;
						}	
					}
					else
					{
						Clinet[iClinet].GetSearchFlag = true;
					}
				}
			}

			Clinet[iClinet].GetEndFlag = false;//把查询初始化

			PjDataMgr->m_Option.Pattern = Pattern0;


			break;
			
		default:
			//其他
			break;
		}
	}
}


void HeatRunOperation(PT_PJDATA_MGR PjDataMgr)
{
	int i = 0;
	int iClinet = PjDataMgr->m_Option.ClinetID;
	PjDataMgr->m_Option.LampOnOffTime++;
	if (PjDataMgr->m_Option.LampOnOffTime == PjDataMgr->m_HrDataGet.LampOnTime && PjDataMgr->m_Option.Power_Flag == false)
	{
		PackCommand(iClinet,SetPowerOff);
#if defined(DEBUG_RS232)
		UART_Printf("Set Power off = %d\n",iClinet);
#endif // DEBUG_RS232
		PjDataMgr->m_Option.LampOnOffTime = 0x0000;
		PjDataMgr->m_Option.Power_Flag = true;
		PjDataMgr->m_Option.Pattern_Flag = true;
		PjDataMgr->m_Option.Pattern = Pattern0;
		Clinet[iClinet].GetEndFlag = 0;
		Clinet[iClinet].GetStartFlag = 0;

	}
	else if (PjDataMgr->m_Option.LampOnOffTime == PjDataMgr->m_HrDataGet.LampOffTime && PjDataMgr->m_Option.Power_Flag == true)
	{
		PackCommand(iClinet,SetPowerOn);
		Clinet[iClinet].PowerOnFlag=1;//ccq 发送开机命令后标志置1，socket接收到1就清0表示可以走轮循 20151120 15：49
#if defined(DEBUG_RS232)
		UART_Printf("Set Power on = %d\n",iClinet);
#endif // DEBUG_RS232
		PjDataMgr->m_Option.LampOnOffTime = 0x0000;
		PjDataMgr->m_Option.Power_Flag = false;
		PjDataMgr->m_Option.Pattern_Flag = false;
		PjDataMgr->m_Option.PatternTime=60;
		PjDataMgr->m_Option.ImagePattern_Flag = false;
		PjDataMgr->m_Option.FactoryMode_Flag = false;
	} 
	else
	{
		/*
		if (Clinet[iClinet].GetParmTime>0 && Clinet[iClinet].GetStartFlag == true)
		{
			Clinet[iClinet].GetParmTime--;
			if (Clinet[iClinet].GetParmTime == 0)
			{
				Clinet[iClinet].GetParmTime=11;
				Clinet[iClinet].GetEndFlag = true;
			}
		}
		*///modified by ly at 20160104 10:38 整理：nothing 时候跳过，查询放在第一次有效设置和第二次有效设置之前的10s内
		if (PjDataMgr->m_Option.PatternTime == 0 && PjDataMgr->m_Option.Pattern_Flag == false)
		{
			ImagePatternOperation(iClinet, PjDataMgr);
			//PjDataMgr->m_Option.PatternTime = 2*a;  //CCQ
			//PjDataMgr->m_Option.PatternTime = 10;  //CCQ
			PjDataMgr->m_Option.PatternTime = a;//add by ly at 20151231 11:10 判断是nothing时候就跳过这个
		}

		if (PjDataMgr->m_Option.PatternTime && Clinet[iClinet].PowerOnFlag == false)
		{
			PjDataMgr->m_Option.PatternTime--;
		}

	}

}

void GetPJStatus(int iClinet)
{
	Clinet[iClinet].GetStartFlag = true;


		PackCommand(iClinet,SetFactoryMode);
#if defined(DEBUG_RS232)
		UART_Printf("SetFactoryMode = %d\n",iClinet);
#endif

		PackCommand(iClinet,GetMacAdd1st);
#if defined(DEBUG_RS232)
		UART_Printf("GetMacAdd1st = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetMacAdd2nd);
#if defined(DEBUG_RS232)
		UART_Printf("GetMacAdd2nd = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetMacAdd3rd);
#if defined(DEBUG_RS232)
		UART_Printf("GetMacAdd3rd = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetMacAdd4th);
#if defined(DEBUG_RS232)
		UART_Printf("GetMacAdd4th = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetMacAdd5th);
#if defined(DEBUG_RS232)
		UART_Printf("GetMacAdd5th = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetMacAdd6th);
#if defined(DEBUG_RS232)
		UART_Printf("GetMacAdd6th = %d\n",iClinet);
#endif


		PackCommand(iClinet,GetPWYear);
#if defined(DEBUG_RS232)
		UART_Printf("GetPWYear = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetPWMonth);
#if defined(DEBUG_RS232)
		UART_Printf("GetPWMonth = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetPWDay);
#if defined(DEBUG_RS232)
		UART_Printf("GetPWDay = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetPWHour);
#if defined(DEBUG_RS232)
		UART_Printf("GetPWHour = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetPWMinute);
#if defined(DEBUG_RS232)
		UART_Printf("GetPWMinute = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetPWSecond);
#if defined(DEBUG_RS232)
		UART_Printf("GetPWSecond = %d\n",iClinet);
#endif

		PackCommand(iClinet,GetAutoPowerOff);
#if defined(DEBUG_RS232)
		UART_Printf("GetAutoPowerOff = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetECOMode);
#if defined(DEBUG_RS232)
		UART_Printf("GetECOMode = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetStandbyMode);
#if defined(DEBUG_RS232)
		UART_Printf("GetStandbyMode = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetInstallation);
#if defined(DEBUG_RS232)
		UART_Printf("GetInstallation = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetLamp1TimeLow);
#if defined(DEBUG_RS232)
		UART_Printf("GetLamp1TimeLow = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetLamp1TimeHigh);
#if defined(DEBUG_RS232)
		UART_Printf("GetLamp1TimeHigh = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetLamp2TimeLow);
#if defined(DEBUG_RS232)
		UART_Printf("GetLamp2TimeLow = %d\n",iClinet);
#endif
		PackCommand(iClinet,GetLamp2TimeHigh);
#if defined(DEBUG_RS232)
		UART_Printf("GetLamp2TimeHigh = %d\n",iClinet);
#endif

// 	switch (Clinet[iClinet].GetParmCnt)
// 	{
// 	case 1:
// 		PackCommand(iClinet,SetFactoryMode);
// #if defined(DEBUG_RS232)
// 		UART_Printf("SetFactoryMode = %d\n",iClinet);
// #endif
// 		Clinet[iClinet].GetParmCnt=2;
// 		break;
// 	case 2:
// 		PackCommand(iClinet,GetMacAdd1st);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetMacAdd1st = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetMacAdd2nd);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetMacAdd2nd = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetMacAdd3rd);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetMacAdd3rd = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetMacAdd4th);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetMacAdd4th = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetMacAdd5th);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetMacAdd5th = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetMacAdd6th);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetMacAdd6th = %d\n",iClinet);
// #endif
// 
// 		Clinet[iClinet].GetParmCnt=3;
// 		break;
// 	case 3:
// 		PackCommand(iClinet,GetPWYear);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetPWYear = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetPWMonth);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetPWMonth = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetPWDay);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetPWDay = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetPWHour);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetPWHour = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetPWMinute);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetPWMinute = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetPWSecond);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetPWSecond = %d\n",iClinet);
// #endif
// 		Clinet[iClinet].GetParmCnt=4;
// 		break;
// 
// 	case 4:
// 		PackCommand(iClinet,GetAutoPowerOff);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetAutoPowerOff = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetECOMode);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetECOMode = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetStandbyMode);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetStandbyMode = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetInstallation);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetInstallation = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetLamp1TimeLow);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetLamp1TimeLow = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetLamp1TimeHigh);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetLamp1TimeHigh = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetLamp2TimeLow);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetLamp2TimeLow = %d\n",iClinet);
// #endif
// 		PackCommand(iClinet,GetLamp2TimeHigh);
// #if defined(DEBUG_RS232)
// 		UART_Printf("GetLamp2TimeHigh = %d\n",iClinet);
// #endif
// 		Clinet[iClinet].GetParmCnt=5;
// 		break;
// 
// 	default:
// 		Clinet[iClinet].GetParmCnt=1;
// 		break;
// 	}
}


char XORCalcContinuation(char *pData, int length)
{
	char CheckSum=0;
	for (int i=0; i<length; i++)
	{
		CheckSum^=*pData;
		pData++;
	}
	return CheckSum;
}



WORD CRC16CalcContinuation(PBYTE pacData, WORD wCount, WORD wCRC)
{
	BYTE    cCRCHi = (BYTE)((wCRC >> 8) & 0xff); // high byte of CRC initialized
	BYTE    cCRCLo = (BYTE)( wCRC       & 0xff); // low  byte of CRC initialized
	BYTE    cIndex;                     // will index into CRC lookup table

	// step through each byte of data
	while (wCount--)
	{
		cIndex = cCRCHi ^ *pacData++;   // calculate the CRC
		cCRCHi = cCRCLo ^ m_acCRCHiArray[cIndex];
		cCRCLo =          m_acCRCLoArray[cIndex];
	}

	wCRC = (cCRCHi << 8) + cCRCLo;

	return wCRC;
}



void SetPJStatus(int iClinet, PT_PJDATA_MGR PjDataMgr)
{
	WORD wCRC;
	BYTE SetAutoPowerOff[13]={0xBE,0xEF,0x03,0x06,0x00,0x00,0x00,0x01,0x00,0x10,0x31,0x00,0x00};

	SetAutoPowerOff[11] = PjDataMgr->m_HrDataGet.AutoPowerOff;
	wCRC = CRC16CalcContinuation(SetAutoPowerOff, sizeof(SetAutoPowerOff), 0xffff);
	SetAutoPowerOff[5] = wCRC & 0xff;
	SetAutoPowerOff[6] = (wCRC>>8) & 0xff;
	PackCommand(iClinet,SetAutoPowerOff);
#if defined(DEBUG_RS232)
	UART_Printf("SetAutoPowerOff = %d\n",iClinet);
#endif
	switch(PjDataMgr->m_HrDataGet.EcoMode)
	{
	case ECO_NORMAL:
		PackCommand(iClinet,SetECOModeOff);
#if defined(DEBUG_RS232)
		UART_Printf("SetECOModeOff = %d\n",iClinet);
#endif
		break;
	case ECO_ECO1:
		PackCommand(iClinet,SetECOMode1);
#if defined(DEBUG_RS232)
		UART_Printf("SetECOMode1 = %d\n",iClinet);
#endif
		break;
	case ECO_ECO2:
		PackCommand(iClinet,SetECOMode2);
#if defined(DEBUG_RS232)
		UART_Printf("SetECOMode2 = %d\n",iClinet);
#endif
		break;
	case ECO_ECOINTELLIGENT:
		PackCommand(iClinet,SetECOIntelligent);
#if defined(DEBUG_RS232)
		UART_Printf("SetECOIntelligent = %d\n",iClinet);
#endif
		break;
	case ECO_ECOMODESAVER:
		PackCommand(iClinet,SetECOModeSaver);
#if defined(DEBUG_RS232)
		UART_Printf("SetECOModeSaver = %d\n",iClinet);
#endif
		break;
	default:
		break;
	}

	switch (PjDataMgr->m_HrDataGet.StandbyMode)
	{
	case STANDBY_NORMAL:
		PackCommand(iClinet,SetStandbyNormal);
#if defined(DEBUG_RS232)
		UART_Printf("SetStandbyNormal = %d\n",iClinet);
#endif
		break;
	case STANDBY_SAVING:
		PackCommand(iClinet,SetStandbySaving);
#if defined(DEBUG_RS232)
		UART_Printf("SetStandbySaving = %d\n",iClinet);
#endif
		break;
	default:
		break;
	}

	switch(PjDataMgr->m_HrDataGet.Install)
	{
	case INSTALL_FRONT_DESKTOP:
		PackCommand(iClinet,SetFrontDesktop);
#if defined(DEBUG_RS232)
		UART_Printf("SetFrontDesktop = %d\n",iClinet);
#endif
		break;
	case INSTALL_REAR_DESKTOP:
		PackCommand(iClinet,SetRearDesktop);
#if defined(DEBUG_RS232)
		UART_Printf("SetRearDesktop = %d\n",iClinet);
#endif
		break;
	case INSTALL_REAR_CEILING:
		PackCommand(iClinet,SetRearCeiling);
#if defined(DEBUG_RS232)
		UART_Printf("SetRearCeiling = %d\n",iClinet);
#endif
		break;
	case INSTALL_FRONT_CEILING:
		PackCommand(iClinet,SetFrontCeiling);
#if defined(DEBUG_RS232)
		UART_Printf("SetFrontCeiling = %d\n",iClinet);
#endif
		break;
	default:
		break;
	}
}
void SaveGetData(int iClinet, PT_PJDATA_MGR PjDataMgr)
{
	pSmpNode NodeTemp;
	PJDATA_MGR *PjDataTemp;
	NodeTemp = g_pSmpDataMgrListHead;
	while(NodeTemp)
	{
		PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
		if (PjDataTemp->m_Option.ClinetID == iClinet)
		{
			//NodeTemp->data = 
			memcpy(PjDataTemp->m_HrDataGet.MacAddress, PjDataMgr->m_HrDataGet.MacAddress, sizeof(PjDataTemp->m_HrDataGet.MacAddress));
			memcpy(PjDataTemp->m_HrDataGet.FirmwareTime, PjDataMgr->m_HrDataGet.FirmwareTime, sizeof(PjDataTemp->m_HrDataGet.FirmwareTime));
			PjDataTemp->m_HrDataGet.AutoPowerOff = PjDataMgr->m_HrDataGet.AutoPowerOff;
			PjDataTemp->m_HrDataGet.EcoMode = PjDataMgr->m_HrDataGet.EcoMode;
			PjDataTemp->m_HrDataGet.StandbyMode = PjDataMgr->m_HrDataGet.StandbyMode;
			PjDataTemp->m_HrDataGet.Install = PjDataMgr->m_HrDataGet.Install;
			PjDataTemp->m_HrDataGet.Lamp1Time = PjDataMgr->m_HrDataGet.Lamp1Time;
			PjDataTemp->m_HrDataGet.Lamp2Time = PjDataMgr->m_HrDataGet.Lamp2Time;
			break;
		}
		NodeTemp = NodeTemp->next;
	}
}

void PJDataDel(char* strData)
{
	pSmpNode NodeTemp;
	PJDATA_MGR *PjDataTemp;
	NodeTemp = g_pSmpDataMgrListHead;
	int iClinetTemp;
	while(NodeTemp)
	{
		PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
		if (strcmp(strData, PjDataTemp->m_HrDataGet.IpAddress) == 0)
		{
			TimerSoft_Stop(g_pSmpDataMgrListHead, PjDataTemp->m_Option.ClinetID);
			//Clinet[PjDataTemp->m_Option.ClinetID].
			iClinetTemp = PjDataTemp->m_Option.ClinetID;
			memset(NodeTemp->data, 0, sizeof(PJDATA_MGR));
			PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
			PjDataTemp->m_Option.ClinetID = iClinetTemp;
			memset(&Clinet[iClinetTemp], 0, sizeof(Clinet[iClinetTemp]));

			break;
		}
		NodeTemp = NodeTemp->next;
	}
}

REFRESH_DATA HrDataProcess(REFRESH_DATA RefreshData)
{
	int nGetDataLen = 0;
	//int nHrDataROW = 0;
	;
	pSmpNode NodeTemp;
	PT_PJDATA_MGR PjDataMgr;
	NodeTemp = g_pSmpDataMgrListHead;

	while(NodeTemp)
	{
		PjDataMgr = (PJDATA_MGR*)NodeTemp->data;
		if (PjDataMgr->m_Option.ClinetID == RefreshData.IdDataPack)
		{
			if (PjDataMgr->m_Option.Used_Flag == 1)
			{

				RefreshData.IdDataPack++;
				RefreshData.nCountPack++;

				//MAC
				memcpy( RefreshData.PJData+nGetDataLen, PjDataMgr->m_HrDataGet.MacAddress, LENTH_MAC );
				nGetDataLen += LENTH_MAC;

				//IP
				memcpy( RefreshData.PJData+nGetDataLen, PjDataMgr->m_HrDataGet.IpAddress, LENTH_IP );
				nGetDataLen += LENTH_IP;

				//Type
				memcpy( RefreshData.PJData+nGetDataLen, PjDataMgr->m_HrDataGet.Type, LENTH_TYPE );
				nGetDataLen += LENTH_TYPE;

				//software ver
				memcpy( RefreshData.PJData+nGetDataLen, PjDataMgr->m_HrDataGet.FirmwareTime, LENTH_VER );
				nGetDataLen += LENTH_VER;

				// AutoPowerOff
				*(RefreshData.PJData+nGetDataLen) = PjDataMgr->m_HrDataGet.AutoPowerOff;
				nGetDataLen++;

				// LampOnTime
				*(RefreshData.PJData+nGetDataLen) = HIBYTE(HIWORD(PjDataMgr->m_HrDataGet.LampOnTime));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(HIWORD(PjDataMgr->m_HrDataGet.LampOnTime));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = HIBYTE(LOWORD(PjDataMgr->m_HrDataGet.LampOnTime));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(LOWORD(PjDataMgr->m_HrDataGet.LampOnTime));
				nGetDataLen++;


				// LampOffTime
				*(RefreshData.PJData+nGetDataLen) = HIBYTE(HIWORD(PjDataMgr->m_HrDataGet.LampOffTime));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(HIWORD(PjDataMgr->m_HrDataGet.LampOffTime));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = HIBYTE(LOWORD(PjDataMgr->m_HrDataGet.LampOffTime));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(LOWORD(PjDataMgr->m_HrDataGet.LampOffTime));
				nGetDataLen++;

				// EcoMode
				*(RefreshData.PJData+nGetDataLen) = PjDataMgr->m_HrDataGet.EcoMode;
				nGetDataLen++;

				// StandbyMode
				*(RefreshData.PJData+nGetDataLen) = PjDataMgr->m_HrDataGet.StandbyMode;
				nGetDataLen++;

				// OnlineStatus
				*(RefreshData.PJData+nGetDataLen) = PjDataMgr->m_HrDataGet.OnlineStatus;
				nGetDataLen++;

				// Install
				*(RefreshData.PJData+nGetDataLen) = PjDataMgr->m_HrDataGet.Install;
				nGetDataLen++;

				// HeatCycle
				*(RefreshData.PJData+nGetDataLen) = HIBYTE(HIWORD(PjDataMgr->m_HrDataGet.HeatCycle));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(HIWORD(PjDataMgr->m_HrDataGet.HeatCycle));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = HIBYTE(LOWORD(PjDataMgr->m_HrDataGet.HeatCycle));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(LOWORD(PjDataMgr->m_HrDataGet.HeatCycle));
				nGetDataLen++;

				// ImagePattern 
				memcpy( RefreshData.PJData+nGetDataLen, PjDataMgr->m_HrDataGet.ImagePattern, SUM_IMAGEPATTERN );
				nGetDataLen += SUM_IMAGEPATTERN;

				// Lamp1Time
				*(RefreshData.PJData+nGetDataLen) = HIBYTE(HIWORD(PjDataMgr->m_HrDataGet.Lamp1Time));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(HIWORD(PjDataMgr->m_HrDataGet.Lamp1Time));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = HIBYTE(LOWORD(PjDataMgr->m_HrDataGet.Lamp1Time));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(LOWORD(PjDataMgr->m_HrDataGet.Lamp1Time));
				nGetDataLen++;

				// Lamp2Time

				*(RefreshData.PJData+nGetDataLen) = HIBYTE(HIWORD(PjDataMgr->m_HrDataGet.Lamp2Time));
				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(HIWORD(PjDataMgr->m_HrDataGet.Lamp2Time));
				nGetDataLen++;

				nGetDataLen++;

				*(RefreshData.PJData+nGetDataLen) = LOBYTE(LOWORD(PjDataMgr->m_HrDataGet.Lamp2Time));
				nGetDataLen++;

				//RunStatus
				*(RefreshData.PJData+nGetDataLen) = PjDataMgr->m_HrDataGet.RunStatus;
				nGetDataLen++;

				//Send ID
				*(RefreshData.PJData+nGetDataLen) = RefreshData.nCountPack;


				return RefreshData;
			}
			else
			{
				RefreshData.IdDataPack++;
			}

		}
		NodeTemp = NodeTemp->next;
	}
	RefreshData.Finish_Flag = true;
	return RefreshData;

}


int PJPause(char* strData)
{
	int iError;
	pSmpNode NodeTemp;
	PJDATA_MGR *PjDataTemp;
	NodeTemp = g_pSmpDataMgrListHead;
	while(NodeTemp)
	{
		PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
		if (strcmp(strData, PjDataTemp->m_HrDataGet.IpAddress) == 0)
		{

			iError = TimerSoft_Stop(g_pSmpDataMgrListHead, PjDataTemp->m_Option.ClinetID);
			if (iError == PjDataTemp->m_Option.ClinetID)
			{
				PjDataTemp->m_HrDataGet.RunStatus = 0x00;
				PackCommand(PjDataTemp->m_Option.ClinetID,SetPatternModeOff);
			}
			else if (iError = -1)
			{
				printf("Fail to stop Timer, TimerID = %d", PjDataTemp->m_Option.ClinetID);
			}

#if defined(DEBUG_RS232)
			UART_Printf("SetPatternModeOff = %d\n", PjDataTemp->m_Option.ClinetID);
#endif
			//	Clinet[iClinetTemp].SendFlag=1;
			//TimerSoft_Stop(g_pSmpDataMgrListHead, PjDataTemp->m_Option.ClinetID);
			return 1;
			//break;
		}
		NodeTemp = NodeTemp->next;
	}
	return 0;
}

int PJStart(char* strData)
{
	int iError;
	pSmpNode NodeTemp;
	PJDATA_MGR *PjDataTemp;
	NodeTemp = g_pSmpDataMgrListHead;
	while(NodeTemp)
	{
		PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
		if (strcmp(strData, PjDataTemp->m_HrDataGet.IpAddress) == 0)
		{
			PackCommand(PjDataTemp->m_Option.ClinetID,SetPatternModeOn);
#if defined(DEBUG_RS232)
			UART_Printf("SetPatternModeOn = %d\n", PjDataTemp->m_Option.ClinetID);
#endif	
			iError = TimerSoft_Start(g_pSmpDataMgrListHead, PjDataTemp->m_Option.ClinetID);
			if (iError == PjDataTemp->m_Option.ClinetID)
			{
				PjDataTemp->m_HrDataGet.RunStatus = 0x01;
			}
			else if (iError == -1)
			{
				printf("Fail to start Timer, TimerID = %d", PjDataTemp->m_Option.ClinetID);
			}
			return 1;
			//break;
		}
		NodeTemp = NodeTemp->next;
	}
	return 0;
}


void PackData(char *tarBuffer, char *srcData, char DataType, int srcLength)
{
	int nLength = 0;
	*(tarBuffer+nLength) = 0x7e;
	nLength++;
	*(tarBuffer+nLength) = (srcLength>>8)&0xff;
	nLength++;
	*(tarBuffer+nLength) = srcLength&0xff;
	nLength++;
	*(tarBuffer+nLength) = XORCalcContinuation(srcData, srcLength);
	nLength++;
	*(tarBuffer+nLength) = DataType;
	nLength++;
	memcpy(tarBuffer+nLength, srcData, srcLength);
	nLength += srcLength;
	*(tarBuffer+nLength) = 0x7e;
}

void PJDataRegister(PT_PJDATA_MGR PjDataMgr)
{

	PjDataMgr->m_Option.Used_Flag = true;
	PjDataMgr->m_Option.LampOnOffTime = 0;
	PjDataMgr->m_Option.Power_Flag = false; //false关机
	PjDataMgr->m_Option.Pattern_Flag = false;//false轮循
	PjDataMgr->m_Option.PatternTime = 5; //轮循时间
	PjDataMgr->m_Option.FactoryMode_Flag = false;
	PjDataMgr->m_Option.ImagePattern_Flag = false;
	PjDataMgr->m_Option.Pattern = Pattern0;
	PjDataMgr->m_HrDataGet.RunStatus = 0x01;
	pSmpNode NodeTemp;
	PJDATA_MGR *PjDataTemp;
	int ClinetIDTemp;
	//int TimerSoftMgr[10] = {0};
	int TimerSoftMgr;
	if (!g_pSmpDataMgrListHead) //链表头为NULL，初始化PjDataMgr为第一个节点数据
	{
		PjDataMgr->m_Option.ClinetID = 1;
		PjDataMgr->m_Option.Used_Flag = 1;
		g_pSmpDataMgrListHead = SMPLIST_init(PjDataMgr); 	
		NUM_PJ++;         //未调试完成，PJ数量   ccq
		Clinet[PjDataMgr->m_Option.ClinetID].sin_addr.S_un.S_addr = inet_addr(PjDataMgr->m_HrDataGet.IpAddress);
		Clinet[PjDataMgr->m_Option.ClinetID].ClinetHtons=htons(23);
		Clinet[PjDataMgr->m_Option.ClinetID].ConnectStatus=Start;
		Clinet[PjDataMgr->m_Option.ClinetID].SocketId=0;
		Clinet[PjDataMgr->m_Option.ClinetID].ConnectCnt=0;
		Clinet[PjDataMgr->m_Option.ClinetID].ConnectTime=0;
		Clinet[PjDataMgr->m_Option.ClinetID].ConnectuTime=5000;
		Clinet[PjDataMgr->m_Option.ClinetID].SendCntLock=0;
		Clinet[PjDataMgr->m_Option.ClinetID].SendDataCnt=0;
		Clinet[PjDataMgr->m_Option.ClinetID].PowerOnFlag=0;
		SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);  //ccq 2015/11/25 10:53
		TimerSoftMgr = TimerSoft_Regedit( g_pSmpDataMgrListHead, PjDataMgr->m_Option.ClinetID, FLAG_OK, 1 , PJ_TSProcess );
		TimerSoft_Start(g_pSmpDataMgrListHead, TimerSoftMgr);

	} 
	else
	{
		NodeTemp = g_pSmpDataMgrListHead;
		while (NodeTemp)
		{
			PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
			if (strcmp(PjDataTemp->m_HrDataGet.IpAddress, PjDataMgr->m_HrDataGet.IpAddress) == 0)  //查找到已有机型   Temp是从链表取到的  Mgr是小洲设置下来的
			{
				// 				if (PjDataMgr->m_HrDataGet.HeatCycle == 0)
				// 				{
				// 					PjDataMgr->m_HrDataGet.HeatCycle = PjDataTemp->m_HrDataGet.HeatCycle;
				// 				}

				PjDataMgr->m_HrDataGet.OnlineStatus = PjDataTemp->m_HrDataGet.OnlineStatus;
				PjDataMgr->m_HrDataGet.RunStatus = PjDataTemp->m_HrDataGet.RunStatus;
				PjDataMgr->m_TimerMgr = PjDataTemp->m_TimerMgr;
				PjDataMgr->m_Option = PjDataTemp->m_Option;
				//				*PjDataTemp = *PjDataMgr;//modified by ly at 20160104 16:64 问题1：怀疑给反了，导致重新编辑时候，MAC地址记录为0，软件版本也丢失
				//				;//问题2：老化时间（LEMPONTIME LEMPOFFTIME）没更新到链表里去 还有AutoPowerOff等也没更新到链表，直到PJ的数据刷新
				//

				//修复问题1
				memcpy(PjDataMgr->m_HrDataGet.FirmwareTime,PjDataTemp->m_HrDataGet.FirmwareTime,18);
				memcpy(PjDataMgr->m_HrDataGet.MacAddress,PjDataTemp->m_HrDataGet.MacAddress,13);

				//修复问题2
				*PjDataTemp = *PjDataMgr;
				NodeTemp->data = PjDataTemp;
				//备注，单台测试ok，UI显示正常，但是是不是按实际的时间来暂停，未测试。 备注2：多台的尚未测试。

				SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);
				break;
			} 
			NodeTemp = NodeTemp->next;
		}
		bool Register_Flag = false;
		if (NodeTemp == NULL) //没有这台机型
		{
			NodeTemp = g_pSmpDataMgrListHead;
			while(NodeTemp)
			{
				PjDataTemp = (PJDATA_MGR*)NodeTemp->data;
				if(!PjDataTemp->m_Option.Used_Flag)  //used_Flag=0空闲
				{
					PjDataTemp->m_Option.Used_Flag = 1;
					PjDataMgr->m_TimerMgr = PjDataTemp->m_TimerMgr;
					PjDataMgr->m_Option = PjDataTemp->m_Option;
					*PjDataTemp = *PjDataMgr;
					Clinet[PjDataMgr->m_Option.ClinetID].sin_addr.S_un.S_addr = inet_addr(PjDataMgr->m_HrDataGet.IpAddress);
					Clinet[PjDataMgr->m_Option.ClinetID].ClinetHtons=htons(23);
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectStatus=Start;
					Clinet[PjDataMgr->m_Option.ClinetID].SocketId=0;
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectCnt=0;
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectTime=0;
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectuTime=5000;
					Clinet[PjDataMgr->m_Option.ClinetID].SendCntLock=0;
					Clinet[PjDataMgr->m_Option.ClinetID].SendDataCnt=0;
					Clinet[PjDataMgr->m_Option.ClinetID].PowerOnFlag=0;
					SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);  //ccq 2015/11/25 10:53
					TimerSoftMgr = TimerSoft_Regedit( g_pSmpDataMgrListHead, PjDataMgr->m_Option.ClinetID, FLAG_OK, 1 , PJ_TSProcess );
					TimerSoft_Start(g_pSmpDataMgrListHead, TimerSoftMgr);
					Register_Flag = true;
					break;
				}
				else
				{
					ClinetIDTemp =	PjDataTemp->m_Option.ClinetID;
					NodeTemp = NodeTemp->next;
				}
			}//end while
			if (Register_Flag == false)
			{
				if(PjDataTemp->m_Option.Used_Flag)  //链表中没有空闲节点
				{	
					PjDataMgr->m_Option.Used_Flag;
					PjDataMgr->m_Option.ClinetID = ClinetIDTemp+1;
					//NodeTemp->data = PjDataMgr;
					SMPLIST_addnode( PjDataMgr, g_pSmpDataMgrListHead ); //增加临时数据PjDataMgr到链表
					NUM_PJ++;         //未调试完成，PJ数量   ccq
					Clinet[PjDataMgr->m_Option.ClinetID].sin_addr.S_un.S_addr = inet_addr(PjDataMgr->m_HrDataGet.IpAddress);
					Clinet[PjDataMgr->m_Option.ClinetID].ClinetHtons=htons(23);
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectStatus=Start;
					Clinet[PjDataMgr->m_Option.ClinetID].SocketId=0;
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectCnt=0;
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectTime=0;
					Clinet[PjDataMgr->m_Option.ClinetID].ConnectuTime=5000;
					Clinet[PjDataMgr->m_Option.ClinetID].SendCntLock=0;
					Clinet[PjDataMgr->m_Option.ClinetID].SendDataCnt=0;
					Clinet[PjDataMgr->m_Option.ClinetID].PowerOnFlag=0;
					SetPJStatus(PjDataMgr->m_Option.ClinetID, PjDataMgr);  //ccq 2015/11/25 10:53
					TimerSoftMgr = TimerSoft_Regedit( g_pSmpDataMgrListHead, PjDataMgr->m_Option.ClinetID, FLAG_OK, 1 , PJ_TSProcess );
					TimerSoft_Start(g_pSmpDataMgrListHead, TimerSoftMgr);
				}
			}

		} 
	}
}