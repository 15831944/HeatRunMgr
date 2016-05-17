// RecordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "HeatRunDlg.h"
#include "RecordDlg.h"
#include "afxdialogex.h"
#include <afxsock.h>
#include <afxwin.h>
#include "Global.h"

CString strRecordMACAddr;
CString strRecordMACAddrChar[6];
CString strRecordIP;
CString strRecordType;
CString strRecordData;
CString strRecordAutoPoeweOff;
CString strRecordLampOnTime;
CString strRecordLampOn;
CString strRecordLampOnMinute;
CString strRecordLampOnChar[4];
CString strRecordLampOffTime;
CString strRecordLampOff;
CString strRecordLampOffMinute;
CString strRecordLampOffChar[4];
CString strRecordECO;
CString strRecordStandBy;
CString strRecordOnline;
CString strRecordSetup;
CString strRecordHeatRun;
CString strRecordHeatRunChar[4];
CString strRecordPatternChar[10];
CString strRecordLampTime1;
CString strRecordLampTime1Char[4];
CString strRecordLampTime2;
CString strRecordLampTime2Char[4];
CString strRecordRData;
CString sRecordRunStatusNum;

//int nECOTempX;
// CRecordDlg 对话框

IMPLEMENT_DYNAMIC(CRecordDlg, CDialogEx)

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRecordDlg::IDD, pParent)
	, m_TestValue(_T(""))
{

}

CRecordDlg::~CRecordDlg()
{
}

BOOL CRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	// 获取编程语言列表视图控件的位置和大小   
	CRect rect;
	m_RecordList.GetClientRect(&rect);   

	// 为列表视图控件添加全行选中和栅格风格   
	m_RecordList.SetExtendedStyle(m_RecordList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// 为列表视图控件添加列   
	m_RecordList.InsertColumn(0, _T("MAC"), LVCFMT_CENTER, rect.Width()/9, 0);   
	m_RecordList.InsertColumn(1, _T("IP"), LVCFMT_CENTER, rect.Width()/9, 1);   
	m_RecordList.InsertColumn(2, _T("Type"), LVCFMT_CENTER, rect.Width()/10, 2);  
	m_RecordList.InsertColumn(3, _T("Version"), LVCFMT_CENTER, rect.Width()/7, 4);   
	m_RecordList.InsertColumn(4, _T("Auto Power Off"), LVCFMT_CENTER, rect.Width()/10, 5);   
	m_RecordList.InsertColumn(5, _T("LampOn"), LVCFMT_CENTER, rect.Width()/15, 6);   
	m_RecordList.InsertColumn(6, _T("LampOff"), LVCFMT_CENTER, rect.Width()/15, 7);   
	m_RecordList.InsertColumn(7, _T("ECO Model"), LVCFMT_CENTER, rect.Width()/8, 8);  
	m_RecordList.InsertColumn(8, _T("StandBy Model"), LVCFMT_CENTER, rect.Width()/10, 9);   
	m_RecordList.InsertColumn(9, _T("Online"), LVCFMT_CENTER, rect.Width()/15, 12);   
	m_RecordList.InsertColumn(10, _T("Install"), LVCFMT_CENTER, rect.Width()/9, 13); 
	m_RecordList.InsertColumn(11, _T("HeatRun Cycle"), LVCFMT_CENTER, rect.Width()/10, 14); 
	m_RecordList.InsertColumn(12, _T("Pattern"), LVCFMT_CENTER, rect.Width()/10, 14); 
	m_RecordList.InsertColumn(13, _T("Pattern1"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(14, _T("Pattern2"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(15, _T("Pattern3"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(16, _T("Pattern4"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(17, _T("Pattern5"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(18, _T("Pattern6"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(19, _T("Pattern7"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(20, _T("Pattern8"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(21, _T("Pattern9"), LVCFMT_CENTER, 0, 14); 
	m_RecordList.InsertColumn(22, _T("LampTime1"), LVCFMT_CENTER, rect.Width()/12, 10); 
	m_RecordList.InsertColumn(23, _T("LampTime2"), LVCFMT_CENTER, rect.Width()/12, 11); 
	m_RecordList.InsertColumn(24, _T("RecordTime"), LVCFMT_CENTER, rect.Width()/7, 12);  
	m_RecordList.InsertColumn(25, _T("Number"), LVCFMT_CENTER, rect.Width()/15, 13);  
	m_RecordList.Scroll(100);


	return TRUE;
}
void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HR_LIST, m_RecordList);
	DDX_Control(pDX, IDC_RECORD_DATETIMEPICKER1, m_RecordDate1);
	DDX_Control(pDX, IDC_RECORD_DATETIMEPICKER2, m_RecordDate2);
}


BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CRecordDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRecordDlg 消息处理程序

void CRecordDlg::RecordDataProcessFormOS(char* charGetString)
{	

		CString strTempR;
		char strMACAddrTemp[12];
		char strIPAddrTemp[16];
		char strTypeTemp[16];
		char strDateTemp[18];
		int nAutoPowerOffTemp;
		char strLampOnTemp[4];
		CString sLampOnTemp;
		char strLampOffTemp[4];
		CString sLampOffTemp;
		int nECOTemp;
		int nStandByTemp;
		int nOnlineStatusTemp;
		int nSetupTemp;
		char strHeatCycleTemp[4];
		char strPatternTemp[10];
		char strLamp1TimeTemp[4];
		CString sTime1Temp;
		char strLamp2TimeTemp[4];
		CString sTime2Temp;
		char strRecordDataTemp[18];
		char sRunStatus[2];

		memset(strMACAddrTemp,0,12);
		memcpy(strMACAddrTemp,charGetString,12);
		memset(strIPAddrTemp,0,15);
		memcpy(strIPAddrTemp,charGetString+12,15);
		strIPAddrTemp[15] = '\0';
		memset(strTypeTemp,0,16);
		memcpy(strTypeTemp,charGetString+27,15);
		strTypeTemp[15] = '\0';
		memset(strDateTemp,0,18);
		memcpy(strDateTemp,charGetString+42,17);
		strDateTemp[17] = '\0';
		nAutoPowerOffTemp = charGetString[59];
		memset(strLampOnTemp,0,4);
		memcpy(strLampOnTemp,charGetString+60,4);
		memset(strLampOffTemp,0,4);
		memcpy(strLampOffTemp,charGetString+64,4);
		nECOTemp = charGetString[68];
		//nECOTempX = nECOTemp;
		nStandByTemp = charGetString[69];
		nOnlineStatusTemp = charGetString[70];
		nSetupTemp = charGetString[71];
		memset(strHeatCycleTemp,0,4);
		memcpy(strHeatCycleTemp,charGetString+72,4);
		memset(strPatternTemp,0,10);
		memcpy(strPatternTemp,charGetString+76,10);
		memset(strLamp1TimeTemp,0,4);
		memcpy(strLamp1TimeTemp,charGetString+86,4);
		memset(strLamp2TimeTemp,0,4);
		memcpy(strLamp2TimeTemp,charGetString+90,4);
		memset(strRecordDataTemp,0,17);
		memcpy(strRecordDataTemp,charGetString+94,17);
		strRecordDataTemp[17] = '\0';
		memset(sRunStatus,0,2);
		memcpy(sRunStatus,charGetString+111,2);

		int nNum=0;
		int aa = 1;
		// 			int i,k;
		// 			int n = 0;
		if (sizeof(charGetString)/sizeof(charGetString[0]) > 0)
		{
			while(aa)
			{
				switch(nNum)	//	num未做循环
				{
				case 0:   //MAC地址
					for (int i=0;i<6;i++)
					{
						strRecordMACAddrChar[i] = Two2One(strMACAddrTemp[2*i],strMACAddrTemp[2*i+1]);
					}
					strRecordMACAddr= strRecordMACAddrChar[0] + ":" + strRecordMACAddrChar[1] + ":" + strRecordMACAddrChar[2] + ":" + strRecordMACAddrChar[3] + ":" +strRecordMACAddrChar[4] + ":" + strRecordMACAddrChar[5]; 
					nNum=1;
					break;
				case 1:  //IP地址

					strRecordIP.Format(_T("%S"),strIPAddrTemp);

					nNum=2;
					break;
				case 2:  //Type类型

					strRecordType.Format(_T("%S"),strTypeTemp);

					nNum=3;
					break;
				case 3:  //版本
					strRecordData.Format(_T("%S"),strDateTemp),
					nNum=4;
					break;
				case 4:  //AutoPowerOff
					strRecordAutoPoeweOff.Format(_T("%d"),nAutoPowerOffTemp);
					nNum=5;
					break;
				case 5:  //LampON
					sLampOnTemp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLampOnTemp[0],(unsigned char)strLampOnTemp[1],(unsigned char)strLampOnTemp[2],(unsigned char)strLampOnTemp[3]);
					strRecordLampOnTime= HexToDem(sLampOnTemp);

					strRecordLampOn.Format(_T("%d"), _ttoi(strRecordLampOnTime)/3600);
					strRecordLampOnMinute.Format(_T("%d"), (_ttoi(strRecordLampOnTime)/60%60));

					if (strRecordLampOn == "" && strRecordLampOnMinute == "")
					{
						strRecordLampOnTime = "";
					}
					else if (strRecordLampOn != "" && strRecordLampOnMinute == "")
					{
						strRecordLampOnTime = strRecordLampOn + _T("H");
					}
					else if (strRecordLampOn == "" && strRecordLampOnMinute != "")
					{
						strRecordLampOnTime = strRecordLampOnMinute + _T("M");
					}
					else
						strRecordLampOnTime = strRecordLampOn + _T("H") + strRecordLampOnMinute +_T("M");
					nNum=6;
					break;
				case 6: //LampOff

					sLampOffTemp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLampOffTemp[0],(unsigned char)strLampOffTemp[1],(unsigned char)strLampOffTemp[2],(unsigned char)strLampOffTemp[3]);
					strRecordLampOffTime = HexToDem(sLampOffTemp);

					strRecordLampOff.Format(_T("%d"), _ttoi(strRecordLampOffTime)/3600);
					strRecordLampOffMinute.Format(_T("%d"), (_ttoi(strRecordLampOffTime)/60%60));

					if (strRecordLampOff == "" && strRecordLampOffMinute == "")
					{
						strRecordLampOffTime = "";
					}
					else if (strRecordLampOff != "" && strRecordLampOffMinute == "")
					{
						strRecordLampOffTime = strRecordLampOff + _T("H");
					}
					else if (strRecordLampOff == "" && strRecordLampOffMinute != "")
					{
						strRecordLampOffTime = strRecordLampOffMinute + _T("M");
					}
					else
						strRecordLampOffTime = strRecordLampOff + _T("H") + strRecordLampOffMinute + _T("M");
					nNum=7;				
					break;
				case 7: //ECO Model
					strRecordECO.Format(_T("%d"),nECOTemp);
					if (strRecordECO == "0")
					{
						strRecordECO = "NORMAL";
					}
					else if (strRecordECO == "1")
					{
						strRecordECO = "ECO-1";
					} 
					else if (strRecordECO == "2")
					{
						strRecordECO = "ECO-2";
					} 
					else if(strRecordECO == "16")
					{
						strRecordECO = "INTELLIGENT ECO";
					}
					else if(strRecordECO == "32")
					{
						strRecordECO = "SAVER";
					}
					nNum=8;
					break;
				case 8:  //StandBy Model
					strRecordStandBy.Format(_T("%d"),nStandByTemp);
					if (strRecordStandBy == "0")
					{
						strRecordStandBy = "NORMAL";
					} 
					else if (strRecordStandBy == "1")
					{
						strRecordStandBy = "SAVING";
					}
					nNum=9;
					break;
				case 9: //Online Status
					strRecordOnline.Format(_T("%d"),nOnlineStatusTemp);
					if (strRecordOnline == "0")
					{
						strRecordOnline = "Outline";
					} 
					else if (strRecordOnline == "1")
					{
						strRecordOnline = "Online";
					}
					nNum=10;
					break;
				case 10:  //Setup Model 安装方式
					strRecordSetup.Format(_T("%d"),nSetupTemp);
					if ( strRecordSetup == "0")
					{
						strRecordSetup = "FRONT / DESKTOP";
					} 
					else if( strRecordSetup == "1")
					{
						strRecordSetup = "REAR / DESKTOP";
					}
					else if ( strRecordSetup == "2")
					{
						strRecordSetup = "REAR / CEILING";
					} 
					else if ( strRecordSetup == "3")
					{
						strRecordSetup = "FRONT / CEILING";
					}
					nNum=11;
					break;
				case 11:  // HeatRun 老化周期   显示问题待解决
					strRecordHeatRun.Format(_T("%02X%02X%02X%02X"),(unsigned char)strHeatCycleTemp[0],(unsigned char)strHeatCycleTemp[1],(unsigned char)strHeatCycleTemp[2],(unsigned char)strHeatCycleTemp[3]);
					strRecordHeatRun = HexToDem(strRecordHeatRun);
					nNum=12;
					break;
				case 12:  //Image Pattern  轮循
					for (int i=0;i<10;i++)
					{
						strRecordPatternChar[i].Format(_T("%d"),strPatternTemp[i]);
					}
					if ( strRecordPatternChar[0] == "0")
					{
						strRecordPatternChar[0] = "Nothing";
					} 
					else if( strRecordPatternChar[0] == "1")
					{
						strRecordPatternChar[0] = "Color Bar";
					}
					else if( strRecordPatternChar[0] == "2")
					{
						strRecordPatternChar[0] = "GrayScale";
					}
					else if( strRecordPatternChar[0] == "3")
					{
						strRecordPatternChar[0] = "Dither";
					}
					else if( strRecordPatternChar[0] == "4")
					{
						strRecordPatternChar[0] = "50%Dither";
					}
					else if ( strRecordPatternChar[0] == "5")
					{
						strRecordPatternChar[0] = "Pixel Gamma";
					} 
					else if( strRecordPatternChar[0] == "6")
					{
						strRecordPatternChar[0] = "Ep_Gamma";
					}
					else if( strRecordPatternChar[0] == "7")
					{
						strRecordPatternChar[0] = "Border";
					}
					else if( strRecordPatternChar[0] == "8")
					{
						strRecordPatternChar[0] = "Ghost R";
					}
					else if ( strRecordPatternChar[0] == "9")
					{
						strRecordPatternChar[0] = "Ghost G";
					} 
					else if( strRecordPatternChar[0] == "10")
					{
						strRecordPatternChar[0] = "Ghost B";
					}
					else if ( strRecordPatternChar[0] == "11")
					{
						strRecordPatternChar[0] = "Ghost W";
					} 
					else if( strRecordPatternChar[0] == "12")
					{
						strRecordPatternChar[0] = "NRSH R";
					}
					else if ( strRecordPatternChar[0] == "13")
					{
						strRecordPatternChar[0] = "NRSH G";
					} 
					else if( strRecordPatternChar[0] == "14")
					{
						strRecordPatternChar[0] = "NRSH B";
					}
					else if ( strRecordPatternChar[0] == "15")
					{
						strRecordPatternChar[0] = "NRSH W";
					} 
					else if( strRecordPatternChar[0] == "16")
					{
						strRecordPatternChar[0] = "VCom R";
					}
					else if ( strRecordPatternChar[0] == "17")
					{
						strRecordPatternChar[0] = "VCom G";
					} 
					else if( strRecordPatternChar[0] == "18")
					{
						strRecordPatternChar[0] = "VCom B";
					}
					else if ( strRecordPatternChar[0] == "19")
					{
						strRecordPatternChar[0] = "VCom W";
					} 
					else if( strRecordPatternChar[0] == "20")
					{
						strRecordPatternChar[0] = "Gray Scale W";
					}
					else if ( strRecordPatternChar[0] == "21")
					{
						strRecordPatternChar[0] = "Gray Scale G";
					} 
					else if( strRecordPatternChar[0] == "22")
					{
						strRecordPatternChar[0] = "Reverced Gray Scale W";
					}
					else if ( strRecordPatternChar[0] == "23")
					{
						strRecordPatternChar[0] = "Reverced Gray Scale G";
					} 
					else if( strRecordPatternChar[0] == "24")
					{
						strRecordPatternChar[0] = "PSIG_Gray";
					}
					else if( strRecordPatternChar[0] == "25")
					{
						strRecordPatternChar[0] = "PSIGBlack";
					}
					else if( strRecordPatternChar[0] == "26")
					{
						strRecordPatternChar[0] = "SubHPosi";
					}
					else if( strRecordPatternChar[0] == "27")
					{
						strRecordPatternChar[0] = "Reserved";
					}
					else if( strRecordPatternChar[0] == "28")
					{
						strRecordPatternChar[0] = "Vmid_R";
					}
					else if( strRecordPatternChar[0] == "29")
					{
						strRecordPatternChar[0] = "Vmid_G";
					}
					else if( strRecordPatternChar[0] == "30")
					{
						strRecordPatternChar[0] = "Vmid_B";
					}
					else if( strRecordPatternChar[0] == "31")
					{
						strRecordPatternChar[0] = "Vmid_W";
					}
					else if( strRecordPatternChar[0] == "32")
					{
						strRecordPatternChar[0] = "B_REF R";
					}
					else if( strRecordPatternChar[0] == "33")
					{
						strRecordPatternChar[0] = "B_REF G";
					}
					else if( strRecordPatternChar[0] == "34")
					{
						strRecordPatternChar[0] = "B_REF B";
					}
					else if( strRecordPatternChar[0] == "35")
					{
						strRecordPatternChar[0] = "B_REF W";
					}
					else if( strRecordPatternChar[0] == "36")
					{
						strRecordPatternChar[0] = "R_0/255";
					}
					else if ( strRecordPatternChar[0] == "37")
					{
						strRecordPatternChar[0] = "G_0/255";
					} 
					else if( strRecordPatternChar[0] == "38")
					{
						strRecordPatternChar[0] = "B_0/255";
					}
					else if ( strRecordPatternChar[0] == "39")
					{
						strRecordPatternChar[0] = "W_0/255";
					}
					else if( strRecordPatternChar[0] == "40")
					{
						strRecordPatternChar[0] = "R_32/255";
					}
					else if ( strRecordPatternChar[0] == "41")
					{
						strRecordPatternChar[0] = "G_32/255";
					} 
					else if( strRecordPatternChar[0] == "42")
					{
						strRecordPatternChar[0] = "B_32/255";
					}
					else if ( strRecordPatternChar[0] == "43")
					{
						strRecordPatternChar[0] = "W_32/255";
					}
					else if( strRecordPatternChar[0] == "44")
					{
						strRecordPatternChar[0] = "R_64/255";
					}
					else if ( strRecordPatternChar[0] == "45")
					{
						strRecordPatternChar[0] = "G_64/255";
					}
					else if( strRecordPatternChar[0] == "46")
					{
						strRecordPatternChar[0] = "B_64/255";
					}
					else if ( strRecordPatternChar[0] == "47")
					{
						strRecordPatternChar[0] = "W_64/255";
					} 
					else if( strRecordPatternChar[0] == "48")
					{
						strRecordPatternChar[0] = "R_96/255";
					}
					else if ( strRecordPatternChar[0] == "49")
					{
						strRecordPatternChar[0] = "G_96/255";
					} 
					else if( strRecordPatternChar[0] == "50")
					{
						strRecordPatternChar[0] = "B_96/255";
					}
					else if ( strRecordPatternChar[0] == "51")
					{
						strRecordPatternChar[0] = "W_96/255";
					} 
					else if( strRecordPatternChar[0] == "52")
					{
						strRecordPatternChar[0] = "R_168/255";
					}
					else if ( strRecordPatternChar[0] == "53")
					{
						strRecordPatternChar[0] = "G_168/255";
					} 
					else if( strRecordPatternChar[0] == "54")
					{
						strRecordPatternChar[0] = "B_168/255";
					}
					else if ( strRecordPatternChar[0] == "55")
					{
						strRecordPatternChar[0] = "W_168/255";
					}
					else if( strRecordPatternChar[0] == "56")
					{
						strRecordPatternChar[0] = "R_192/255";
					}
					else if ( strRecordPatternChar[0] == "57")
					{
						strRecordPatternChar[0] = "G_192/255";
					}
					else if( strRecordPatternChar[0] == "58")
					{
						strRecordPatternChar[0] = "B_192/255";
					}
					else if ( strRecordPatternChar[0] == "59")
					{
						strRecordPatternChar[0] = "W_192/255";
					}
					else if( strRecordPatternChar[0] == "60")
					{
						strRecordPatternChar[0] = "R_208/255";
					}
					else if ( strRecordPatternChar[0] == "61")
					{
						strRecordPatternChar[0] = "G_208/255";
					} 
					else if( strRecordPatternChar[0] == "62")
					{
						strRecordPatternChar[0] = "B_208/255";
					}
					else if ( strRecordPatternChar[0] == "63")
					{
						strRecordPatternChar[0] = "W_208/255";
					}
					else if( strRecordPatternChar[0] == "64")
					{
						strRecordPatternChar[0] = "C.Unif R(min)";
					}
					else if ( strRecordPatternChar[0] == "65")
					{
						strRecordPatternChar[0] = "C.Unif R(min_lo)";
					} 
					else if ( strRecordPatternChar[0] == "66")
					{
						strRecordPatternChar[0] = "C.Unif R(min_hi)";
					} 
					else if ( strRecordPatternChar[0] == "67")
					{
						strRecordPatternChar[0] = "C.Unif R(max)";
					} 
					else if ( strRecordPatternChar[0] == "68")
					{
						strRecordPatternChar[0] = "C.Unif G(min)";
					} 
					else if ( strRecordPatternChar[0] == "69")
					{
						strRecordPatternChar[0] = "C.Unif G(min_lo)";
					} 
					else if ( strRecordPatternChar[0] == "70")
					{
						strRecordPatternChar[0] = "C.Unif G(min_hi)";
					} 
					else if ( strRecordPatternChar[0] == "71")
					{
						strRecordPatternChar[0] = "C.Unif G(max)";
					} 
					else if ( strRecordPatternChar[0] == "72")
					{
						strRecordPatternChar[0] = "C.Unif B(min)";
					} 
					else if ( strRecordPatternChar[0] == "73")
					{
						strRecordPatternChar[0] = "C.Unif B(min_lo)";
					} 
					else if ( strRecordPatternChar[0] == "74")
					{
						strRecordPatternChar[0] = "C.Unif B(min_hi)";
					} 
					else if ( strRecordPatternChar[0] == "75")
					{
						strRecordPatternChar[0] = "C.Unif B(max)";
					} 
					else if ( strRecordPatternChar[0] == "76")
					{
						strRecordPatternChar[0] = "C.Unif W(min)";
					} 
					else if ( strRecordPatternChar[0] == "77")
					{
						strRecordPatternChar[0] = "C.Unif W(min_lo)";
					} 
					else if ( strRecordPatternChar[0] == "78")
					{
						strRecordPatternChar[0] = "C.Unif W(min_hi)";
					} 
					else if ( strRecordPatternChar[0] == "79")
					{
						strRecordPatternChar[0] = "C.Unif W(max)";
					}
					else if( strRecordPatternChar[0] == "80")
					{
						strRecordPatternChar[0] = "R_255/255";
					}
					else if ( strRecordPatternChar[0] == "81")
					{
						strRecordPatternChar[0] = "G_255/255";
					} 
					else if( strRecordPatternChar[0] == "82")
					{
						strRecordPatternChar[0] = "B_255/255";
					}
					else if ( strRecordPatternChar[0] == "83")
					{
						strRecordPatternChar[0] = "W_255/255";
					} 
					else if( strRecordPatternChar[0] == "84")
					{
						strRecordPatternChar[0] = "GammaGSWn";
					}
					else if( strRecordPatternChar[0] == "85")
					{
						strRecordPatternChar[0] = "GammaGSGn";
					}
					else if( strRecordPatternChar[0] == "86")
					{
						strRecordPatternChar[0] = "GammaGSWr";
					}
					else if( strRecordPatternChar[0] == "87")
					{
						strRecordPatternChar[0] = "GammaGSGr";
					}
					else if( strRecordPatternChar[0] == "88")
					{
						strRecordPatternChar[0] = "R_200/255";
					}
					else if ( strRecordPatternChar[0] == "89")
					{
						strRecordPatternChar[0] = "G_200/255";
					} 
					else if( strRecordPatternChar[0] == "90")
					{
						strRecordPatternChar[0] = "B_200/255";
					}
					else if ( strRecordPatternChar[0] == "91")
					{
						strRecordPatternChar[0] = "W_200/255";
					} 
					nNum=13;
					break;
				case 13:  //LampTime1 灯泡1时间

					sTime1Temp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLamp1TimeTemp[0],(unsigned char)strLamp1TimeTemp[1],(unsigned char)strLamp1TimeTemp[2],(unsigned char)strLamp1TimeTemp[3]);

					strRecordLampTime1 = HexToDem(sTime1Temp);
					strRecordLampTime1.Format(_T("%d"), _ttoi(strRecordLampTime1));
					strRecordLampTime1 += "H";
					nNum=14;
					break;
				case 14:  //LampTime2 灯泡2时间

					sTime2Temp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLamp2TimeTemp[0],(unsigned char)strLamp2TimeTemp[1],(unsigned char)strLamp2TimeTemp[2],(unsigned char)strLamp2TimeTemp[3]);

					strRecordLampTime2 = HexToDem(sTime2Temp);
					strRecordLampTime2.Format(_T("%d"), _ttoi(strRecordLampTime2));
					strRecordLampTime2 += "H";
					nNum=15;
					break;
				case 15:
					strRecordRData.Format(_T("%S"),strRecordDataTemp),
					nNum=16;
					break;
				case 16:
					sRecordRunStatusNum.Format(_T("%02X%02X"),(unsigned char)sRunStatus[0],(unsigned char)sRunStatus[1]);
					sRecordRunStatusNum = HexToDem(sRecordRunStatusNum);
					sRecordRunStatusNum.Format(_T("%d"), _ttoi(sRecordRunStatusNum));
					nNum=17;
					break;
				case 17:
					aa=0;
					break;
				}
			}
		}
	return;
} 

void CRecordDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime m_date1;
	CTime m_date2;
	CString recordMAC;
	CString strTemp;
	CString strRecValue;
	CString strDate1;
	CString strDate2;
	CString strSendStatus;
	CString m_MACAddrChar[6];
	m_RecordDate1.GetTime(m_date1);
	m_RecordDate2.GetTime(m_date2);
	GetDlgItemText(IDC_RECORD_MACEDIT, recordMAC);
	GetDlgItemText(IDC_RECORD_DATETIMEPICKER1,strDate1);
	GetDlgItemText(IDC_RECORD_DATETIMEPICKER2,strDate2);
	CTimeSpan timeSpan = m_date2-m_date1;
	int nDay = timeSpan.GetDays();
	
	
	if (nDay>7)
	{
		AfxMessageBox(_T("Start date and end date,don't more than seven days!"));
		return;
	}
	else if (nDay<0)
	{
		AfxMessageBox(_T("End date must later than start date!"));
		return;
	}
	m_RecordList.DeleteAllItems();
	strSendStatus = _T("03"); //查询历史数据
	
	BYTE pByte[34];
	memset(pByte,0,34);
	CString strPacket(_T("7E"));
	CString strPacketEnd(_T("7E"));
	CString strLength;
	int nstrXor=0;
	CString strXor;
	strLength.Format(_T("%04X"), 28);

	pByte[0]=Comm_2NumCharToHex(strPacket[0],strPacket[1]);
	pByte[1]=Comm_2NumCharToHex(strLength[0],strLength[1]);
	pByte[2]=Comm_2NumCharToHex(strLength[2],strLength[3]);

	pByte[4]=Comm_2NumCharToHex(strSendStatus[0],strSendStatus[1]);


	
	TCHAR sepsMAC[] = _T(":");
	if (recordMAC != "")
	{

		TCHAR *tokenMAC = _tcstok((LPTSTR)(LPCTSTR)recordMAC, sepsMAC);
		for (int i=0;i<6;i++)
		{
			m_MACAddrChar[i] = tokenMAC;
			tokenMAC = _tcstok(NULL, sepsMAC);

		}
	}
	strTemp = m_MACAddrChar[0] + m_MACAddrChar[1] + m_MACAddrChar[2] + m_MACAddrChar[3] + m_MACAddrChar[4] + m_MACAddrChar[5];

	char strTr[100];
	memset(strTr,0,24);
	memcpy(strTr,strTemp,strTemp.GetLength()*2);
	char buff[12];
	memset(buff,0,12);
	for (int i=0;i<sizeof(strTr)/sizeof(strTr[0]);i++)
	{
		buff[i] = strTr[2*i];
	}

	memcpy(&pByte[5],buff,12);
	CString strDateTemp;
	int nIndex;
	for (int i=17;i<21;i++)
	{
		pByte[i]=strDate1[i-17];
	}
	strDate1.Delete(0,5);
	nIndex = strDate1.Find(_T("/"));
	strDateTemp = strDate1.Left(nIndex);
	if (strDateTemp.GetLength() == 1)
	{
		pByte[21] = '0';
		pByte[22] = strDateTemp[0];
	}
	else
	{
		pByte[21] = strDateTemp[0];
		pByte[22] = strDateTemp[1];
	}
	strDate1.Delete(0, strDate1.Left(nIndex+1).GetLength());
	strDateTemp = strDate1;
	if (strDateTemp.GetLength() == 1)
	{
		pByte[23] = '0';
		pByte[24] = strDateTemp[0];
	}
	else
	{
		pByte[23] = strDateTemp[0];
		pByte[24] = strDateTemp[1];
	}

	for (int i=25;i<29;i++)
	{
		pByte[i]=strDate2[i-25];
	}
	strDate2.Delete(0,5);
	nIndex = strDate2.Find(_T("/"));
	strDateTemp = strDate2.Left(nIndex);
	if (strDateTemp.GetLength() == 1)
	{
		pByte[29] = '0';
		pByte[30] = strDateTemp[0];
	}
	else
	{
		pByte[29] = strDateTemp[0];
		pByte[30] = strDateTemp[1];
	}
	strDate2.Delete(0, strDate2.Left(nIndex+1).GetLength());
	strDateTemp = strDate2;
	if (strDateTemp.GetLength() == 1)
	{
		pByte[31] = '0';
		pByte[32] = strDateTemp[0];
	}
	else
	{
		pByte[31] = strDateTemp[0];
		pByte[32] = strDateTemp[1];
	}


	for (int i=5;i<33;i++)
	{
		nstrXor ^= pByte[i];
	}
	strXor.Format(_T("%02X"), nstrXor);
	pByte[3]=Comm_2NumCharToHex(strXor[0],strXor[1]);

	pByte[33]=Comm_2NumCharToHex(strPacketEnd[0],strPacketEnd[1]);
	CHeatRunDlg *pHeatRunDlg = (CHeatRunDlg*)GetParent();

	pHeatRunDlg->pSocket->Send(pByte,34);


}

void CRecordDlg::ReceiveRecordDataMessage(BYTE *pbuf)
{
	char str[RecordDataLen]={0};
	memset(str,0,RecordDataLen);
 	memcpy(str,pbuf,RecordDataLen);
	setRList(str);
}

void CRecordDlg::setRList(char* str)
{
	CRecordDlg *dlg = (CRecordDlg *)GetActiveWindow();
	CString ss;
// 	ss.Format(_T("%02X"),nECOTempX);
// 	dlg->SetDlgItemText(IDC_TESTEDIT,ss);
	int nNum;
	RecordDataProcessFormOS(str);
	nNum = _ttoi(sRecordRunStatusNum);
	dlg->m_RecordList.DeleteItem(nNum);
	dlg->m_RecordList.InsertItem(nNum, strRecordMACAddr);   
	dlg->m_RecordList.SetItemText(nNum, 1,  strRecordIP);   
	dlg->m_RecordList.SetItemText(nNum, 2,  strRecordType);   
	dlg->m_RecordList.SetItemText(nNum, 3,  strRecordData); 
	dlg->m_RecordList.SetItemText(nNum, 4,  strRecordAutoPoeweOff);   
	dlg->m_RecordList.SetItemText(nNum, 5,  strRecordLampOnTime); 
	dlg->m_RecordList.SetItemText(nNum, 6,  strRecordLampOffTime);   
	dlg->m_RecordList.SetItemText(nNum, 7,  strRecordECO);   
	dlg->m_RecordList.SetItemText(nNum, 8,  strRecordStandBy); 
	dlg->m_RecordList.SetItemText(nNum, 9,  strRecordOnline); 
	dlg->m_RecordList.SetItemText(nNum, 10, strRecordSetup);   
	dlg->m_RecordList.SetItemText(nNum, 11, strRecordHeatRun); 
	dlg->m_RecordList.SetItemText(nNum, 12, strRecordPatternChar[0]); 
	dlg->m_RecordList.SetItemText(nNum, 13, strRecordPatternChar[1]); 
	dlg->m_RecordList.SetItemText(nNum, 14, strRecordPatternChar[2]); 
	dlg->m_RecordList.SetItemText(nNum, 15, strRecordPatternChar[3]); 
	dlg->m_RecordList.SetItemText(nNum, 16, strRecordPatternChar[4]); 
	dlg->m_RecordList.SetItemText(nNum, 17, strRecordPatternChar[5]); 
	dlg->m_RecordList.SetItemText(nNum, 18, strRecordPatternChar[6]); 
	dlg->m_RecordList.SetItemText(nNum, 19, strRecordPatternChar[7]); 
	dlg->m_RecordList.SetItemText(nNum, 20, strRecordPatternChar[8]); 
	dlg->m_RecordList.SetItemText(nNum, 21, strRecordPatternChar[9]); 
	dlg->m_RecordList.SetItemText(nNum, 22, strRecordLampTime1);  
	dlg->m_RecordList.SetItemText(nNum, 23, strRecordLampTime2);
	dlg->m_RecordList.SetItemText(nNum, 24, strRecordRData);
	dlg->m_RecordList.SetItemText(nNum, 25, sRecordRunStatusNum);
	return;
		
}

void CRecordDlg::ReceiveAlert()
{
	AfxMessageBox(_T("No Historical Data"));
	return;
}

void CRecordDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}