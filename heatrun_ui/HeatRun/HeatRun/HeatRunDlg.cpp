
// HeatRunDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "HeatRunDlg.h"
#include "afxdialogex.h"
#include "InsertEditDlg.h"
#include <afxsock.h>
#include <afxwin.h>
#include <WinSock2.h>
#include "LoginDlg.h"
#include "ServerIPDlg.h"
#include "RecordDlg.h"
#include "NewAsyncSocket.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString strMACAddr;
CString strMACAddrChar[6];
CString strIP;
CString strIPChar[4];
CString strType;
CString strTypeChar[15];
CString strData;
CString strDataChar[7];
CString strAutoPoeweOff;
CString strLampOnTime;
CString strLampOn;
CString strLampOnMinute;
CString strLampOnChar[4];
CString strLampOffTime;
CString strLampOff;
CString strLampOffMinute;
CString strLampOffChar[4];
CString strECO;
CString strStandBy;
CString strOnline;
CString strSetup;
CString strHeatRun;
CString strHeatRunChar[4];
CString strPatternChar[10];
CString strLampTime1;
CString strLampTime1Char[4];
CString strLampTime2;
CString strLampTime2Char[4];
CString sRunStatusNum;
//CServerIP *ServerIPDlg = (CServerIP *)AfxGetApp()->GetMainWnd();
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedVersionButton();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)

END_MESSAGE_MAP()


// CHeatRunDlg 对话框




CHeatRunDlg::CHeatRunDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHeatRunDlg::IDD, pParent)
	, m_nHRData(0)
	, m_TestEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nHRData = 0 ;
}

void CHeatRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HR_LIST, m_hrList);
	//	DDX_Text(pDX, IDC_TEST_EDIT, m_TestEdit);
}

BEGIN_MESSAGE_MAP(CHeatRunDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_HR_LIST, &CHeatRunDlg::OnLvnItemchangedHrList)
	//	ON_BN_CLICKED(IDC_HR_RUNANDSTOP, &CHeatRunDlg::OnBnClickedHrRunandstop)
	ON_BN_CLICKED(IDC_HR_SETTING, &CHeatRunDlg::OnBnClickedHrSetting)
	//	ON_BN_CLICKED(IDC_HR_MANAGE, &CHeatRunDlg::OnBnClickedHrEdit)
	ON_BN_CLICKED(IDC_HR_RECORD, &CHeatRunDlg::OnBnClickedHrRecord)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CHeatRunDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CHeatRunDlg::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CHeatRunDlg::OnBnClickedButton3)
	ON_NOTIFY(NM_DBLCLK, IDC_HR_LIST, &CHeatRunDlg::OnNMDblclkHrList)
	//	ON_NOTIFY(LVN_DELETEITEM, IDC_HR_LIST, &CHeatRunDlg::OnDeleteitemHrList)
	ON_NOTIFY(NM_CLICK, IDC_HR_LIST, &CHeatRunDlg::OnClickHrList)
	ON_BN_CLICKED(IDC_STOP_BUTTON, &CHeatRunDlg::OnBnClickedStopButton)
	ON_NOTIFY(NM_CUSTOMDRAW,IDC_HR_LIST,OnCustomdrawList)
	//ON_NOTIFY(NM_KILLFOCUS, IDC_HR_LIST, &CHeatRunDlg::OnKillfocusHrList)
END_MESSAGE_MAP()


// CHeatRunDlg 消息处理程序

BOOL CHeatRunDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (strServerIP == "")
	{
		GetDlgItem(IDC_HR_SETTING)->EnableWindow(FALSE);
		GetDlgItem(IDC_DELETE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(FALSE);
		GetDlgItem(IDC_HR_RECORD)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(FALSE);
	}
	CLoginDlg aDlg;
	if (IDCANCEL == aDlg.DoModal())
		return TRUE;
	// 获取编程语言列表视图控件的位置和大小   
	CRect rect;
	m_hrList.GetClientRect(&rect);   

	// 为列表视图控件添加全行选中和栅格风格   
	m_hrList.SetExtendedStyle(m_hrList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   

	// 为列表视图控件添加列   
	m_hrList.InsertColumn(0, _T("MAC"), LVCFMT_CENTER, rect.Width()/9, 0);   
	m_hrList.InsertColumn(1, _T("IP"), LVCFMT_CENTER, rect.Width()/9, 1);   
	m_hrList.InsertColumn(2, _T("Type"), LVCFMT_CENTER, rect.Width()/10, 2);  
	m_hrList.InsertColumn(3, _T("Version"), LVCFMT_CENTER, rect.Width()/7, 4);   
	m_hrList.InsertColumn(4, _T("Auto Power Off"), LVCFMT_CENTER, rect.Width()/10, 5);   
	m_hrList.InsertColumn(5, _T("LampOn"), LVCFMT_CENTER, rect.Width()/15, 6);   
	m_hrList.InsertColumn(6, _T("LampOff"), LVCFMT_CENTER, rect.Width()/15, 7);   
	m_hrList.InsertColumn(7, _T("ECO Mode"), LVCFMT_CENTER, rect.Width()/8, 8);  
	m_hrList.InsertColumn(8, _T("StandBy Mode"), LVCFMT_CENTER, rect.Width()/10, 9);   
	m_hrList.InsertColumn(9, _T("Online"), LVCFMT_CENTER, rect.Width()/15, 12);   
	m_hrList.InsertColumn(10, _T("Install"), LVCFMT_CENTER, rect.Width()/9, 13); 
	m_hrList.InsertColumn(11, _T("HeatRun Cycle"), LVCFMT_CENTER, rect.Width()/10, 14); 
	m_hrList.InsertColumn(12, _T("Pattern"), LVCFMT_CENTER, rect.Width()/10, 14); 
	m_hrList.InsertColumn(13, _T("Pattern1"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(14, _T("Pattern2"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(15, _T("Pattern3"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(16, _T("Pattern4"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(17, _T("Pattern5"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(18, _T("Pattern6"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(19, _T("Pattern7"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(20, _T("Pattern8"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(21, _T("Pattern9"), LVCFMT_CENTER, 0, 14); 
	m_hrList.InsertColumn(22, _T("LampTime1"), LVCFMT_CENTER, rect.Width()/12, 10); 
	m_hrList.InsertColumn(23, _T("LampTime2"), LVCFMT_CENTER, rect.Width()/12, 11); 
	m_hrList.InsertColumn(24, _T("ColorNumber"), LVCFMT_CENTER, 0, 11); 
	m_hrList.Scroll(100);

	//	SetTimer(1, 15000, NULL); //自动刷新
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHeatRunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		UpdateData(FALSE);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHeatRunDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHeatRunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//获取List控件中被选择的项
// void CHeatRunDlg::OnLvnItemchangedHrList(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
// 	// TODO: 在此添加控件通知处理程序代码
// 	*pResult = 0;
// 	CString strLangName;
// 	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   
// 
// 	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
// 	{   
// 
// 		// 获取被选择列表项第一个子项的文本   
// 		for(int i=0; i<16; i++)
// 		{
// 			strLangName = m_hrList.GetItemText(pNMListView->iItem, i);  
// 			langStr += strLangName + _T("|");
// 			//MessageBox(langStr, _T("TestAlert"), MB_OK);
// 		}
// 	
// 		nListITEM = pNMListView->iItem;  	  
// 		CInsertEditDlg inserteditDlg;
// 		inserteditDlg.m_str = langStr;
// 
// 		inserteditDlg.m_iORe = "Edit";
// 		if(IDCANCEL == inserteditDlg.DoModal())
// 			return;
// 	}
// }

void CHeatRunDlg::DataProcessFormOS(char* charGetString)
{	
	int nXor=0;
	int nStrXor=0;
	nStrXor = charGetString[3];

	int nLen;
	int nLen1;
	int nlen2;
	CString strLen;
	CString strLen1;
	CString strLen2;
	nLen1 = charGetString[1];
	nlen2 = charGetString[2];
	strLen1.Format(_T("%d"),nLen1);
	strLen2.Format(_T("%d"),nlen2);
	strLen = strLen1+strLen2;
	nLen = _ttoi(strLen);
	char strTemp[96];
	memset(strTemp,0,nLen);
	memcpy(strTemp,charGetString+5,nLen);

	for (int i=0;i<nLen;i++)
	{
		nXor ^= strTemp[i];
	}

	if (nXor == nStrXor)
	{
		CString strTempR;
		char strMACAddrTemp[12];
		char strIPAddrTemp[15];
		char strTypeTemp[17];
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
		int sRunStatus;

		memset(strMACAddrTemp,0,12);
		memcpy(strMACAddrTemp,charGetString+5,12);
		memset(strIPAddrTemp,0,15);
		memcpy(strIPAddrTemp,charGetString+17,15);
		memset(strTypeTemp,0,17);
		memcpy(strTypeTemp,charGetString+32,17);
		memset(strDateTemp,0,17);
		memcpy(strDateTemp,charGetString+47,17);
		strDateTemp[17] = '\0';
		nAutoPowerOffTemp = charGetString[64];
		memset(strLampOnTemp,0,4);
		memcpy(strLampOnTemp,charGetString+65,4);
		memset(strLampOffTemp,0,4);
		memcpy(strLampOffTemp,charGetString+69,4);
		nECOTemp = charGetString[73];
		nStandByTemp = charGetString[74];
		nOnlineStatusTemp = charGetString[75];
		nSetupTemp = charGetString[76];
		memset(strHeatCycleTemp,0,4);
		memcpy(strHeatCycleTemp,charGetString+77,4);
		memset(strPatternTemp,0,10);
		memcpy(strPatternTemp,charGetString+81,10);
		memset(strLamp1TimeTemp,0,4);
		memcpy(strLamp1TimeTemp,charGetString+91,4);
		memset(strLamp2TimeTemp,0,4);
		memcpy(strLamp2TimeTemp,charGetString+95,4);
		sRunStatus = charGetString[99];




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
						strMACAddrChar[i] = Two2One(strMACAddrTemp[2*i],strMACAddrTemp[2*i+1]);
					}
					strMACAddr= strMACAddrChar[0] + ":" + strMACAddrChar[1] + ":" + strMACAddrChar[2] + ":" + strMACAddrChar[3] + ":" +strMACAddrChar[4] + ":" + strMACAddrChar[5]; 
					nNum=1;
					break;
				case 1:  //IP地址

					strIP.Format(_T("%S"),strIPAddrTemp);

					nNum=2;
					break;
				case 2:  //Type类型

					strType.Format(_T("%S"),strTypeTemp);

					nNum=3;
					break;
				case 3:  //版本
					strData.Format(_T("%S"),strDateTemp),
						nNum=4;
					break;
				case 4:  //AutoPowerOff
					strAutoPoeweOff.Format(_T("%d"),nAutoPowerOffTemp);
					nNum=5;
					break;
				case 5:  //LampON
					sLampOnTemp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLampOnTemp[0],(unsigned char)strLampOnTemp[1],(unsigned char)strLampOnTemp[2],(unsigned char)strLampOnTemp[3]);
					strLampOnTime= HexToDem(sLampOnTemp);

					strLampOn.Format(_T("%d"), _ttoi(strLampOnTime)/3600);
					strLampOnMinute.Format(_T("%d"), (_ttoi(strLampOnTime)/60%60));

					if (strLampOn == "" && strLampOnMinute == "")
					{
						strLampOnTime = "";
					}
					else if (strLampOn != "" && strLampOnMinute == "")
					{
						strLampOnTime = strLampOn + _T("H");
					}
					else if (strLampOn == "" && strLampOnMinute != "")
					{
						strLampOnTime = strLampOnMinute + _T("M");
					}
					else
						strLampOnTime = strLampOn + _T("H") + strLampOnMinute +_T("M");
					nNum=6;
					break;
				case 6: //LampOff

					sLampOffTemp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLampOffTemp[0],(unsigned char)strLampOffTemp[1],(unsigned char)strLampOffTemp[2],(unsigned char)strLampOffTemp[3]);
					strLampOffTime = HexToDem(sLampOffTemp);

					strLampOff.Format(_T("%d"), _ttoi(strLampOffTime)/3600);
					strLampOffMinute.Format(_T("%d"), (_ttoi(strLampOffTime)/60%60));

					if (strLampOff == "" && strLampOffMinute == "")
					{
						strLampOffTime = "";
					}
					else if (strLampOff != "" && strLampOffMinute == "")
					{
						strLampOffTime = strLampOff + _T("H");
					}
					else if (strLampOff == "" && strLampOffMinute != "")
					{
						strLampOffTime = strLampOffMinute + _T("M");
					}
					else
						strLampOffTime = strLampOff + _T("H") + strLampOffMinute + _T("M");
					nNum=7;				
					break;
				case 7: //ECO Model
					strECO.Format(_T("%d"),nECOTemp);
					if (strECO == "0")
					{
						strECO = "NORMAL";
					}
					else if (strECO == "1")
					{
						strECO = "ECO-1";
					} 
					else if (strECO == "2")
					{
						strECO = "ECO-2";
					} 
					else if(strECO == "16")
					{
						strECO = "INTELLIGENT ECO";
					}
					else if(strECO == "32")
					{
						strECO = "SAVER";
					}
					nNum=8;
					break;
				case 8:  //StandBy Model
					strStandBy.Format(_T("%d"),nStandByTemp);
					if (strStandBy == "0")
					{
						strStandBy = "NORMAL";
					} 
					else if (strStandBy == "1")
					{
						strStandBy = "SAVING";
					}
					nNum=9;
					break;
				case 9: //Online Status
					strOnline.Format(_T("%d"),nOnlineStatusTemp);
					if (strOnline == "0")
					{
						strOnline = "Outline";
					} 
					else if (strOnline == "1")
					{
						strOnline = "Online";
					}
					nNum=10;
					break;
				case 10:  //Setup Model 安装方式
					strSetup.Format(_T("%d"),nSetupTemp);
					if ( strSetup == "0")
					{
						strSetup = "FRONT / DESKTOP";
					} 
					else if( strSetup == "1")
					{
						strSetup = "REAR / DESKTOP";
					}
					else if ( strSetup == "2")
					{
						strSetup = "REAR / CEILING";
					} 
					else if ( strSetup == "3")
					{
						strSetup = "FRONT / CEILING";
					}
					nNum=11;
					break;
				case 11:  // HeatRun 老化周期   显示问题待解决
					strHeatRun.Format(_T("%02X%02X%02X%02X"),(unsigned char)strHeatCycleTemp[0],(unsigned char)strHeatCycleTemp[1],(unsigned char)strHeatCycleTemp[2],(unsigned char)strHeatCycleTemp[3]);
					strHeatRun = HexToDem(strHeatRun);
					// 					for (int i=0;i<4;i++)
					// 					{
					// 						strHeatRunChar[i].Format(_T("%d"),strHeatCycleTemp[i]);
					// 					}
					// 					strHeatRun = strHeatRunChar[0]+strHeatRunChar[1]+strHeatRunChar[2]+strHeatRunChar[3];
					// 					strHeatRun.Format(_T("%d"), _ttoi(strHeatRun));
					nNum=12;
					break;
				case 12:  //Image Pattern  轮循
					for (int i=0;i<10;i++)
					{
						strPatternChar[i].Format(_T("%d"),strPatternTemp[i]);
					}
					if ( strPatternChar[0] == "0")
					{
						strPatternChar[0] = "Nothing";
					} 
					else if( strPatternChar[0] == "1")
					{
						strPatternChar[0] = "Color Bar";
					}
					else if( strPatternChar[0] == "2")
					{
						strPatternChar[0] = "GrayScale";
					}
					else if( strPatternChar[0] == "3")
					{
						strPatternChar[0] = "Dither";
					}
					else if( strPatternChar[0] == "4")
					{
						strPatternChar[0] = "50%Dither";
					}
					else if ( strPatternChar[0] == "5")
					{
						strPatternChar[0] = "Pixel Gamma";
					} 
					else if( strPatternChar[0] == "6")
					{
						strPatternChar[0] = "Ep_Gamma";
					}
					else if( strPatternChar[0] == "7")
					{
						strPatternChar[0] = "Border";
					}
					else if( strPatternChar[0] == "8")
					{
						strPatternChar[0] = "Ghost R";
					}
					else if ( strPatternChar[0] == "9")
					{
						strPatternChar[0] = "Ghost G";
					} 
					else if( strPatternChar[0] == "10")
					{
						strPatternChar[0] = "Ghost B";
					}
					else if ( strPatternChar[0] == "11")
					{
						strPatternChar[0] = "Ghost W";
					} 
					else if( strPatternChar[0] == "12")
					{
						strPatternChar[0] = "NRSH R";
					}
					else if ( strPatternChar[0] == "13")
					{
						strPatternChar[0] = "NRSH G";
					} 
					else if( strPatternChar[0] == "14")
					{
						strPatternChar[0] = "NRSH B";
					}
					else if ( strPatternChar[0] == "15")
					{
						strPatternChar[0] = "NRSH W";
					} 
					else if( strPatternChar[0] == "16")
					{
						strPatternChar[0] = "VCom R";
					}
					else if ( strPatternChar[0] == "17")
					{
						strPatternChar[0] = "VCom G";
					} 
					else if( strPatternChar[0] == "18")
					{
						strPatternChar[0] = "VCom B";
					}
					else if ( strPatternChar[0] == "19")
					{
						strPatternChar[0] = "VCom W";
					} 
					else if( strPatternChar[0] == "20")
					{
						strPatternChar[0] = "Gray Scale W";
					}
					else if ( strPatternChar[0] == "21")
					{
						strPatternChar[0] = "Gray Scale G";
					} 
					else if( strPatternChar[0] == "22")
					{
						strPatternChar[0] = "Reverced Gray Scale W";
					}
					else if ( strPatternChar[0] == "23")
					{
						strPatternChar[0] = "Reverced Gray Scale G";
					} 
					else if( strPatternChar[0] == "24")
					{
						strPatternChar[0] = "PSIG_Gray";
					}
					else if( strPatternChar[0] == "25")
					{
						strPatternChar[0] = "PSIGBlack";
					}
					else if( strPatternChar[0] == "26")
					{
						strPatternChar[0] = "SubHPosi";
					}
					else if( strPatternChar[0] == "27")
					{
						strPatternChar[0] = "Reserved";
					}
					else if( strPatternChar[0] == "28")
					{
						strPatternChar[0] = "Vmid_R";
					}
					else if( strPatternChar[0] == "29")
					{
						strPatternChar[0] = "Vmid_G";
					}
					else if( strPatternChar[0] == "30")
					{
						strPatternChar[0] = "Vmid_B";
					}
					else if( strPatternChar[0] == "31")
					{
						strPatternChar[0] = "Vmid_W";
					}
					else if( strPatternChar[0] == "32")
					{
						strPatternChar[0] = "B_REF R";
					}
					else if( strPatternChar[0] == "33")
					{
						strPatternChar[0] = "B_REF G";
					}
					else if( strPatternChar[0] == "34")
					{
						strPatternChar[0] = "B_REF B";
					}
					else if( strPatternChar[0] == "35")
					{
						strPatternChar[0] = "B_REF W";
					}
					else if( strPatternChar[0] == "36")
					{
						strPatternChar[0] = "R_0/255";
					}
					else if ( strPatternChar[0] == "37")
					{
						strPatternChar[0] = "G_0/255";
					} 
					else if( strPatternChar[0] == "38")
					{
						strPatternChar[0] = "B_0/255";
					}
					else if ( strPatternChar[0] == "39")
					{
						strPatternChar[0] = "W_0/255";
					}
					else if( strPatternChar[0] == "40")
					{
						strPatternChar[0] = "R_32/255";
					}
					else if ( strPatternChar[0] == "41")
					{
						strPatternChar[0] = "G_32/255";
					} 
					else if( strPatternChar[0] == "42")
					{
						strPatternChar[0] = "B_32/255";
					}
					else if ( strPatternChar[0] == "43")
					{
						strPatternChar[0] = "W_32/255";
					}
					else if( strPatternChar[0] == "44")
					{
						strPatternChar[0] = "R_64/255";
					}
					else if ( strPatternChar[0] == "45")
					{
						strPatternChar[0] = "G_64/255";
					}
					else if( strPatternChar[0] == "46")
					{
						strPatternChar[0] = "B_64/255";
					}
					else if ( strPatternChar[0] == "47")
					{
						strPatternChar[0] = "W_64/255";
					} 
					else if( strPatternChar[0] == "48")
					{
						strPatternChar[0] = "R_96/255";
					}
					else if ( strPatternChar[0] == "49")
					{
						strPatternChar[0] = "G_96/255";
					} 
					else if( strPatternChar[0] == "50")
					{
						strPatternChar[0] = "B_96/255";
					}
					else if ( strPatternChar[0] == "51")
					{
						strPatternChar[0] = "W_96/255";
					} 
					else if( strPatternChar[0] == "52")
					{
						strPatternChar[0] = "R_168/255";
					}
					else if ( strPatternChar[0] == "53")
					{
						strPatternChar[0] = "G_168/255";
					} 
					else if( strPatternChar[0] == "54")
					{
						strPatternChar[0] = "B_168/255";
					}
					else if ( strPatternChar[0] == "55")
					{
						strPatternChar[0] = "W_168/255";
					}
					else if( strPatternChar[0] == "56")
					{
						strPatternChar[0] = "R_192/255";
					}
					else if ( strPatternChar[0] == "57")
					{
						strPatternChar[0] = "G_192/255";
					}
					else if( strPatternChar[0] == "58")
					{
						strPatternChar[0] = "B_192/255";
					}
					else if ( strPatternChar[0] == "59")
					{
						strPatternChar[0] = "W_192/255";
					}
					else if( strPatternChar[0] == "60")
					{
						strPatternChar[0] = "R_208/255";
					}
					else if ( strPatternChar[0] == "61")
					{
						strPatternChar[0] = "G_208/255";
					} 
					else if( strPatternChar[0] == "62")
					{
						strPatternChar[0] = "B_208/255";
					}
					else if ( strPatternChar[0] == "63")
					{
						strPatternChar[0] = "W_208/255";
					}
					else if( strPatternChar[0] == "64")
					{
						strPatternChar[0] = "C.Unif R(min)";
					}
					else if ( strPatternChar[0] == "65")
					{
						strPatternChar[0] = "C.Unif R(min_lo)";
					} 
					else if ( strPatternChar[0] == "66")
					{
						strPatternChar[0] = "C.Unif R(min_hi)";
					} 
					else if ( strPatternChar[0] == "67")
					{
						strPatternChar[0] = "C.Unif R(max)";
					} 
					else if ( strPatternChar[0] == "68")
					{
						strPatternChar[0] = "C.Unif G(min)";
					} 
					else if ( strPatternChar[0] == "69")
					{
						strPatternChar[0] = "C.Unif G(min_lo)";
					} 
					else if ( strPatternChar[0] == "70")
					{
						strPatternChar[0] = "C.Unif G(min_hi)";
					} 
					else if ( strPatternChar[0] == "71")
					{
						strPatternChar[0] = "C.Unif G(max)";
					} 
					else if ( strPatternChar[0] == "72")
					{
						strPatternChar[0] = "C.Unif B(min)";
					} 
					else if ( strPatternChar[0] == "73")
					{
						strPatternChar[0] = "C.Unif B(min_lo)";
					} 
					else if ( strPatternChar[0] == "74")
					{
						strPatternChar[0] = "C.Unif B(min_hi)";
					} 
					else if ( strPatternChar[0] == "75")
					{
						strPatternChar[0] = "C.Unif B(max)";
					} 
					else if ( strPatternChar[0] == "76")
					{
						strPatternChar[0] = "C.Unif W(min)";
					} 
					else if ( strPatternChar[0] == "77")
					{
						strPatternChar[0] = "C.Unif W(min_lo)";
					} 
					else if ( strPatternChar[0] == "78")
					{
						strPatternChar[0] = "C.Unif W(min_hi)";
					} 
					else if ( strPatternChar[0] == "79")
					{
						strPatternChar[0] = "C.Unif W(max)";
					}
					else if( strPatternChar[0] == "80")
					{
						strPatternChar[0] = "R_255/255";
					}
					else if ( strPatternChar[0] == "81")
					{
						strPatternChar[0] = "G_255/255";
					} 
					else if( strPatternChar[0] == "82")
					{
						strPatternChar[0] = "B_255/255";
					}
					else if ( strPatternChar[0] == "83")
					{
						strPatternChar[0] = "W_255/255";
					} 
					else if( strPatternChar[0] == "84")
					{
						strPatternChar[0] = "GammaGSWn";
					}
					else if( strPatternChar[0] == "85")
					{
						strPatternChar[0] = "GammaGSGn";
					}
					else if( strPatternChar[0] == "86")
					{
						strPatternChar[0] = "GammaGSWr";
					}
					else if( strPatternChar[0] == "87")
					{
						strPatternChar[0] = "GammaGSGr";
					}
					else if( strPatternChar[0] == "88")
					{
						strPatternChar[0] = "R_200/255";
					}
					else if ( strPatternChar[0] == "89")
					{
						strPatternChar[0] = "G_200/255";
					} 
					else if( strPatternChar[0] == "90")
					{
						strPatternChar[0] = "B_200/255";
					}
					else if ( strPatternChar[0] == "91")
					{
						strPatternChar[0] = "W_200/255";
					} 
					nNum=13;
					break;
				case 13:  //LampTime1 灯泡1时间

					sTime1Temp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLamp1TimeTemp[0],(unsigned char)strLamp1TimeTemp[1],(unsigned char)strLamp1TimeTemp[2],(unsigned char)strLamp1TimeTemp[3]);

					strLampTime1 = HexToDem(sTime1Temp);
					strLampTime1.Format(_T("%d"), _ttoi(strLampTime1));
					strLampTime1 += "H";
					nNum=14;
					break;
				case 14:  //LampTime2 灯泡2时间

					sTime2Temp.Format(_T("%02X%02X%02X%02X"),(unsigned char)strLamp2TimeTemp[0],(unsigned char)strLamp2TimeTemp[1],(unsigned char)strLamp2TimeTemp[2],(unsigned char)strLamp2TimeTemp[3]);

					strLampTime2 = HexToDem(sTime2Temp);
					strLampTime2.Format(_T("%d"), _ttoi(strLampTime2));
					strLampTime2 += "H";
					nNum=15;
					break;
				case 15:
					sRunStatusNum.Format(_T("%d"),sRunStatus);
					nNum=16;
					break;
				case 16:
					aa=0;
					break;
				}
			}
		}
	}
	return;
} 




void CHeatRunDlg::OnBnClickedHrSetting()
{
	// TODO: 在此添加控件通知处理程序代码

	CInsertEditDlg inserteditDlg;

	inserteditDlg.m_iORe = "Insert";
	if(IDCANCEL == inserteditDlg.DoModal())
		return;
}

void CHeatRunDlg::OnBnClickedHrRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CRecordDlg recordDlg;

	if(IDCANCEL == recordDlg.DoModal())
		return;
}

void CHeatRunDlg::SocketConnect(CString serverIP,INT portNum)
{
	AfxSocketInit();

	pSocket = new CNewAsyncSocket;
	if (!pSocket->Create(0,SOCK_STREAM,FD_CONNECT|FD_WRITE|FD_READ))
	{
		CString strMsg;
		strMsg = _T("Create Faild: ") + pSocket->GetLastError();
		AfxMessageBox(strMsg);
		return;
	}
	pSocket->Connect(serverIP, portNum);
	nSecond = 58;
	SetTimer(1, 60000, NULL); //自动刷新
	SetTimer(3,1000,NULL);
}

//定时器函数
void CHeatRunDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//	int i,nIndex;
	CHeatRunDlg *dlg = (CHeatRunDlg *)AfxGetApp()->GetMainWnd();
	CString strSecond;
	CString strTemp;
	switch(nIDEvent)
	{
	case 1:
		BYTE pByte[100];
		SendDataMessage(pByte);
		pSocket->Send(pByte,7);
		pSocket->AsyncSelect(FD_READ);
		break;
	case 2:
		strTemp.Format(_T("%d"),nRefreshSecond);
		strTemp = strTemp+_T("s");
		dlg->SetDlgItemText(IDC_BUTTON_REFRESH,strTemp);
		nRefreshSecond--;
		if (nRefreshSecond == -1)
		{
			dlg->SetDlgItemText(IDC_BUTTON_REFRESH,_T("Refresh"));
			dlg->GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(TRUE);
			KillTimer(2);
		}	
		break;
	case 3:
		strSecond.Format(_T("%d"),nSecond);
		dlg->SetDlgItemText(IDC_SECOND_STATIC,strSecond);
		nSecond--;
		if (nSecond == -1)
		{
			nSecond = 58;
		}	
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

//刷新按钮函数
void CHeatRunDlg::OnBnClickedButtonRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_hrList.DeleteAllItems();
	BYTE pByte[100]; 	
	SendDataMessage(pByte);
	pSocket->Send(pByte,7);

	//pSocket->Receive((void *)szRecValue,sizeof(szRecValue));
	pSocket->AsyncSelect(FD_READ);
	//pSocket->AsyncSelect(FD_WRITE);

	CHeatRunDlg *dlg = (CHeatRunDlg *)AfxGetApp()->GetMainWnd();
	dlg->GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(FALSE);
	nRefreshSecond = 3;
	SetTimer(2, 1000, NULL); //预防刷新过频

}


void CHeatRunDlg::OnBnClickedDeleteButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSend;
	BYTE pByte[30]; 		
	CString strPacket(_T("7E"));
	CString strPacketEnd(_T("7E"));
	CString strLength;
	int nstrXor=0;
	CString strXor;
	CString strSendStatus(_T("04")); //刷新
	strSend = m_hrList.GetItemText(nListITEMClick,1); 

	pByte[0]=Comm_2NumCharToHex(strPacket[0],strPacket[1]);
	int nstrLen = 15;
	strLength.Format(_T("%04X"),nstrLen); //发送消息体长度
	pByte[1]=Comm_2NumCharToHex(strLength[0],strLength[1]);
	pByte[2]=Comm_2NumCharToHex(strLength[2],strLength[3]);

	pByte[4]=Comm_2NumCharToHex(strSendStatus[0],strSendStatus[1]);


	int nIPLen=strSend.GetLength();

	for (int i=5;i<20;i++)
	{
		if (i-5>=nIPLen)
		{
			pByte[i]=0x00;
		}
		else
		{
			pByte[i]=strSend[i-5];
		}	
	}
	//	memcpy(&pByte[5],buff,15);

	nstrXor = 0;
	for (int i=0;i<strSend.GetLength();i++)
	{
		nstrXor ^= strSend[i];
	}
	strXor.Format(_T("%02X"), nstrXor);
	pByte[3]=Comm_2NumCharToHex(strXor[0],strXor[1]);
	pByte[20]=Comm_2NumCharToHex(strPacketEnd[0],strPacketEnd[1]);
	pSocket->Send(pByte,21);
	m_hrList.DeleteItem(nListITEMClick);
	nListITEMClick=10086;//重置ListID
}


void CHeatRunDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CServerIP serverDlg;

	if (IDCANCEL == serverDlg.DoModal())
		return;
}

//双击列表进入编辑页面
void CHeatRunDlg::OnNMDblclkHrList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
	CString strLangName;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{   

		// 获取被选择列表项第一个子项的文本   
		for(int i=0; i<26; i++)
		{
			strLangName = m_hrList.GetItemText(pNMListView->iItem, i);  
			langStr += strLangName + _T("|");
			//MessageBox(langStr, _T("TestAlert"), MB_OK);
		}

		nListITEM = pNMListView->iItem;  	  
		CInsertEditDlg inserteditDlg;
		inserteditDlg.m_str = langStr;
		langStr ="";
		inserteditDlg.m_iORe = "Edit";
		if(IDCANCEL == inserteditDlg.DoModal())
			return;
	}
	//*pResult = 0;
}
//单击列表获取ListID
void CHeatRunDlg::OnClickHrList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;   

	nListITEMClick = pNMListView->iItem;  
	*pResult = 0;
}

void CHeatRunDlg::OnReceive(int nErrorCode)
{
	if(0==nErrorCode)
	{
		char szRecValue[1024] = {0};
		pSocket->Receive((void *)szRecValue, 1024);
	}
}

void CHeatRunDlg::ReceiveDataMessage(BYTE *pbuf,int Len)
{
	char str[ReceiveData]={0};
	memset(str,0,ReceiveData);
	
	for (int i=0;i<51;i++)
	{
		memcpy(str,pbuf+i*ReceiveData,ReceiveData);
		if (str[0] != 0x00)
		{
			setHrList(str);
		}
		else
		{
			return;
		}
	}
// 	for (int i=0;i<Len/ReceiveData;i++)
// 	{
// 		memcpy(str,pbuf+i*ReceiveData,ReceiveData);
// 		if (str[0] != 0x00)
// 		{
// 			setHrList(str);
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}
}

void CHeatRunDlg::ReceiveVersionMessage(BYTE *pbuf,int Len)
{
	char str[ReceiveData]={0};
	memset(str,0,ReceiveData);
	memcpy(str,pbuf,ReceiveData);
	CHeatRunDlg *dlg = (CHeatRunDlg *)AfxGetApp()->GetMainWnd();
	char versionInfoChar[8];
	CString versionInfo;
	memset(versionInfoChar,0,8);
	memcpy(versionInfoChar,str+5,8);
	versionInfo.Format(_T("%S"),versionInfoChar);
	dlg->SetDlgItemText(IDC_VERSIONINFO,versionInfo);
	return;
}
BYTE* CHeatRunDlg::SendDataMessage(BYTE pByte[100])
{
	CString strSend;
	CString strPacket(_T("7E"));
	CString strPacketEnd(_T("7E"));
	CString strLength;
	int nstrXor=0;
	CString strXor;
	CString strSendStatus(_T("01")); //刷新
	strLength.Format(_T("%04X"),1); //发送固定长度
	strSend.Format(_T("%02X"),1);
	nstrXor = strSend[0];
	for (int i=1;i<strSend.GetLength();i++)
	{
		nstrXor ^= strSend[i];
	}
	strXor.Format(_T("%02X"), nstrXor);


	pByte[0]=Comm_2NumCharToHex(strPacket[0],strPacket[1]);
	pByte[1]=Comm_2NumCharToHex(strLength[0],strLength[1]);
	pByte[2]=Comm_2NumCharToHex(strLength[2],strLength[3]);
	pByte[3]=Comm_2NumCharToHex(strXor[0],strXor[1]);
	pByte[4]=Comm_2NumCharToHex(strSendStatus[0],strSendStatus[1]);
	pByte[5]=Comm_2NumCharToHex(strSend[0],strSend[1]); 
	pByte[6]=Comm_2NumCharToHex(strPacketEnd[0],strPacketEnd[1]);

	return pByte;
}

void CHeatRunDlg::OnConnect()
{
	//pSocket = new CNewAsyncSocket;

	//pSocket->Connect(strServerIP,nPort);
}

void CHeatRunDlg::setHrList(char* str)
{

	CHeatRunDlg *dlg = (CHeatRunDlg *)AfxGetApp()->GetMainWnd();
	int nCount = dlg->m_hrList.GetItemCount();

	int nNum;
	CString sNum;
	nNum = str[100];
	// 	sNum.Format(_T("%d"),nNum);
	// 	sNum = HexToDem(sNum);
	if (str[4] == 0x07)
	{
		char versionInfoChar[8];
		CString versionInfo;
		memset(versionInfoChar,0,8);
		memcpy(versionInfoChar,str+5,8);
		versionInfo.Format(_T("%S"),versionInfoChar);
		dlg->SetDlgItemText(IDC_VERSIONINFO,versionInfo);
		return;
	}
	else
	{
		DataProcessFormOS(str);
		dlg->m_hrList.DeleteItem(nNum-1);
		dlg->m_hrList.InsertItem(nNum-1, strMACAddr);   
		dlg->m_hrList.SetItemText(nNum-1, 1,  strIP);   
		dlg->m_hrList.SetItemText(nNum-1, 2,  strType);   
		dlg->m_hrList.SetItemText(nNum-1, 3,  strData); 
		dlg->m_hrList.SetItemText(nNum-1, 4,  strAutoPoeweOff);   
		dlg->m_hrList.SetItemText(nNum-1, 5,  strLampOnTime); 
		dlg->m_hrList.SetItemText(nNum-1, 6,  strLampOffTime);   
		dlg->m_hrList.SetItemText(nNum-1, 7,  strECO);   
		dlg->m_hrList.SetItemText(nNum-1, 8,  strStandBy); 
		dlg->m_hrList.SetItemText(nNum-1, 9,  strOnline); 
		dlg->m_hrList.SetItemText(nNum-1, 10, strSetup);   
		dlg->m_hrList.SetItemText(nNum-1, 11, strHeatRun); 
		dlg->m_hrList.SetItemText(nNum-1, 12, strPatternChar[0]); 
		dlg->m_hrList.SetItemText(nNum-1, 13, strPatternChar[1]); 
		dlg->m_hrList.SetItemText(nNum-1, 14, strPatternChar[2]); 
		dlg->m_hrList.SetItemText(nNum-1, 15, strPatternChar[3]); 
		dlg->m_hrList.SetItemText(nNum-1, 16, strPatternChar[4]); 
		dlg->m_hrList.SetItemText(nNum-1, 17, strPatternChar[5]); 
		dlg->m_hrList.SetItemText(nNum-1, 18, strPatternChar[6]); 
		dlg->m_hrList.SetItemText(nNum-1, 19, strPatternChar[7]); 
		dlg->m_hrList.SetItemText(nNum-1, 20, strPatternChar[8]); 
		dlg->m_hrList.SetItemText(nNum-1, 21, strPatternChar[9]); 
		dlg->m_hrList.SetItemText(nNum-1, 22, strLampTime1);  
		dlg->m_hrList.SetItemText(nNum-1, 23, strLampTime2);
		dlg->m_hrList.SetItemText(nNum-1, 24, sRunStatusNum);
		return;
	}
}



void CHeatRunDlg::OnBnClickedStopButton()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_hrList.SetTextBkColor(RGB(255,255,0));
	CString strColorNumber = m_hrList.GetItemText(nListITEMClick,24);
	if (strColorNumber == "0")
	{
		m_hrList.SetItemText(nListITEMClick,24,_T("1"));
		CString strSend;
		BYTE pByte[30]; 		
		CString strPacket(_T("7E"));
		CString strPacketEnd(_T("7E"));
		CString strLength;
		int nstrXor=0;
		CString strXor;
		CString strSendStatus(_T("06")); //开始老化
		strSend = m_hrList.GetItemText(nListITEMClick,1); 

		pByte[0]=Comm_2NumCharToHex(strPacket[0],strPacket[1]);
		int nstrLen = 15;
		strLength.Format(_T("%04X"),nstrLen); //发送消息体长度
		pByte[1]=Comm_2NumCharToHex(strLength[0],strLength[1]);
		pByte[2]=Comm_2NumCharToHex(strLength[2],strLength[3]);

		pByte[4]=Comm_2NumCharToHex(strSendStatus[0],strSendStatus[1]);


		int nIPLen=strSend.GetLength();

		for (int i=5;i<20;i++)
		{
			if (i-5>=nIPLen)
			{
				pByte[i]=0x00;
			}
			else
			{
				pByte[i]=strSend[i-5];
			}	
		}

		nstrXor = 0;
		for (int i=0;i<strSend.GetLength();i++)
		{
			nstrXor ^= strSend[i];
		}
		strXor.Format(_T("%02X"), nstrXor);
		pByte[3]=Comm_2NumCharToHex(strXor[0],strXor[1]);
		pByte[20]=Comm_2NumCharToHex(strPacketEnd[0],strPacketEnd[1]);
		pSocket->Send(pByte,21);
	}
	else if (strColorNumber == "1")
	{
		m_hrList.SetItemText(nListITEMClick,24,_T("0"));
		CString strSend;
		BYTE pByte[30]; 		
		CString strPacket(_T("7E"));
		CString strPacketEnd(_T("7E"));
		CString strLength;
		int nstrXor=0;
		CString strXor;
		CString strSendStatus(_T("05")); //停止老化
		strSend = m_hrList.GetItemText(nListITEMClick,1); 

		pByte[0]=Comm_2NumCharToHex(strPacket[0],strPacket[1]);
		int nstrLen = 15;
		strLength.Format(_T("%04X"),nstrLen); //发送消息体长度
		pByte[1]=Comm_2NumCharToHex(strLength[0],strLength[1]);
		pByte[2]=Comm_2NumCharToHex(strLength[2],strLength[3]);

		pByte[4]=Comm_2NumCharToHex(strSendStatus[0],strSendStatus[1]);


		int nIPLen=strSend.GetLength();

		for (int i=5;i<20;i++)
		{
			if (i-5>=nIPLen)
			{
				pByte[i]=0x00;
			}
			else
			{
				pByte[i]=strSend[i-5];
			}	
		}
		//	memcpy(&pByte[5],buff,15);

		nstrXor = 0;
		for (int i=0;i<strSend.GetLength();i++)
		{
			nstrXor ^= strSend[i];
		}
		strXor.Format(_T("%02X"), nstrXor);
		pByte[3]=Comm_2NumCharToHex(strXor[0],strXor[1]);
		pByte[20]=Comm_2NumCharToHex(strPacketEnd[0],strPacketEnd[1]);
		pSocket->Send(pByte,21);
	}
}

void CHeatRunDlg::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	*pResult = CDRF_DODEFAULT;

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF crText,crBk;
		CString strMAC = m_hrList.GetItemText(pLVCD->nmcd.dwItemSpec,0);
		CString strVersion = m_hrList.GetItemText(pLVCD->nmcd.dwItemSpec,3);
		CString strLine = m_hrList.GetItemText(pLVCD->nmcd.dwItemSpec,9);
		CString strColorNumber = m_hrList.GetItemText(pLVCD->nmcd.dwItemSpec,24);
		//if (strMAC == "00:00:00:00:00:00" || strMAC == "")
		if(strVersion == "" || strLine == "Outline")
		{
			crText = RGB(0,0,0);
			crBk = RGB(204,0,0);
		}
		else
		{
			if (strColorNumber == "0")
			{
				crText = RGB(0,0,0);
				crBk = RGB(255,255,0);
			}
			else
			{
				crText = RGB(0,0,0);
				crBk = RGB(255,255,255);
			}
		}
		pLVCD->clrText=crText;
		pLVCD->clrTextBk=crBk;
	}
}

//void CHeatRunDlg::OnKillfocusHrList(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//nListITEMClick = 10086;
//	*pResult = 0;
//}



void CHeatRunDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


