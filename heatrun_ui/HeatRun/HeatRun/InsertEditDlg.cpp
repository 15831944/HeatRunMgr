m_pszData
// InsertEditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "InsertEditDlg.h"
#include "HeatRunDlg.h"
#include "afxdialogex.h"
#include <afxsock.h>
#include <afxwin.h>
#include <stdio.h>
#include <stdlib.h>
#include "Global.h"

// CInsertEditDlg 对话框

IMPLEMENT_DYNAMIC(CInsertEditDlg, CDialogEx)

CInsertEditDlg::CInsertEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInsertEditDlg::IDD, pParent)
{

}


//初始化对话框
BOOL CInsertEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	


	CString strEdit(_T("Edit"));
	

	m_TypeEdit.SetLimitText(15);
	m_AutoPOffEdit.SetLimitText(2);
	SetDlgItemInt(IDC_AUTOPOFF_EDIT,20);
	m_LampON.SetLimitText(3);
	SetDlgItemInt(IDC_LAMPON_EDIT,2);
	m_LampOFF.SetLimitText(3);
	SetDlgItemInt(IDC_LAMPOFF_EDIT,0);
	m_LampONMinute.SetLimitText(2);
	SetDlgItemInt(IDC_LAMPONMINUTE_EDIT,0);
	m_LampOFFMinute.SetLimitText(2);
	SetDlgItemInt(IDC_LAMPOFFMINUTE_EDIT,15);
	SetDlgItemInt(IDC_HEATRUN_EDIT,0);

	m_comboECO.InsertString(0,_T("NORMAL"));
	m_comboECO.InsertString(1,_T("ECO-1"));
	m_comboECO.InsertString(2,_T("ECO-2"));
	m_comboECO.InsertString(3,_T("INTELLIGENT ECO"));
	m_comboECO.InsertString(4,_T("SAVER"));
	//设置ECO下拉框的默认选择项
	m_comboECO.SetCurSel(0); 

	//设置Standby下拉框的选项
	m_comboStandby.InsertString(0,_T("NORMAL")); 
	m_comboStandby.InsertString(1,_T("SAVING"));
	//设置Standby下拉框的默认选择项
	m_comboStandby.SetCurSel(0); 

	m_comboSetup.InsertString(0,_T("FRONT / DESKTOP"));
	m_comboSetup.InsertString(1,_T("REAR / DESKTOP"));
	m_comboSetup.InsertString(2,_T("REAR / CEILING"));
	m_comboSetup.InsertString(3,_T("FRONT / CEILING"));
	m_comboSetup.SetCurSel(0);

	m_comboProgram.InsertString(0,_T(""));
	m_comboProgram.InsertString(1,_T("100%White"));
	m_comboProgram.InsertString(2,_T("10%White"));
	m_comboProgram.InsertString(3,_T("100%Black"));
	m_comboProgram.InsertString(4,_T("Pattern"));
	m_comboProgram.SetCurSel(4);


	//设置Pattern下拉框的选项

	for ( int i= IDC_PATTERN0_COMBO; i<=IDC_PATTERN9_COMBO; i++)
	{
		((CComboBox *)GetDlgItem(i))->InsertString(0,_T("Nothing"));
		((CComboBox *)GetDlgItem(i))->InsertString(1,_T("Color Bar"));
		((CComboBox *)GetDlgItem(i))->InsertString(2,_T("GrayScale"));
		((CComboBox *)GetDlgItem(i))->InsertString(3,_T("Dither"));
		((CComboBox *)GetDlgItem(i))->InsertString(4,_T("50%Dither"));
		((CComboBox *)GetDlgItem(i))->InsertString(5,_T("Pixel Gamma"));
		((CComboBox *)GetDlgItem(i))->InsertString(6,_T("Ep_Gamma"));
		((CComboBox *)GetDlgItem(i))->InsertString(7,_T("Border"));
		((CComboBox *)GetDlgItem(i))->InsertString(8,_T("Ghost R"));
		((CComboBox *)GetDlgItem(i))->InsertString(9,_T("Ghost G"));
		((CComboBox *)GetDlgItem(i))->InsertString(10,_T("Ghost B"));
		((CComboBox *)GetDlgItem(i))->InsertString(11,_T("Ghost W"));
		((CComboBox *)GetDlgItem(i))->InsertString(12,_T("NRSH R"));
		((CComboBox *)GetDlgItem(i))->InsertString(13,_T("NRSH G"));
		((CComboBox *)GetDlgItem(i))->InsertString(14,_T("NRSH B"));
		((CComboBox *)GetDlgItem(i))->InsertString(15,_T("NRSH W"));
		((CComboBox *)GetDlgItem(i))->InsertString(16,_T("VCom R"));
		((CComboBox *)GetDlgItem(i))->InsertString(17,_T("VCom G"));
		((CComboBox *)GetDlgItem(i))->InsertString(18,_T("VCom B"));
		((CComboBox *)GetDlgItem(i))->InsertString(19,_T("VCom W"));
		((CComboBox *)GetDlgItem(i))->InsertString(20,_T("Gray Scale W"));
		((CComboBox *)GetDlgItem(i))->InsertString(21,_T("Gray Scale G"));
		((CComboBox *)GetDlgItem(i))->InsertString(22,_T("Reverced Gray Scale W"));
		((CComboBox *)GetDlgItem(i))->InsertString(23,_T("Reverced Gray Scale G"));
		((CComboBox *)GetDlgItem(i))->InsertString(24,_T("PSIG_Gray"));
		((CComboBox *)GetDlgItem(i))->InsertString(25,_T("PSIGBlack"));
		((CComboBox *)GetDlgItem(i))->InsertString(26,_T("SubHPosi"));
		((CComboBox *)GetDlgItem(i))->InsertString(27,_T("Reserved"));
		((CComboBox *)GetDlgItem(i))->InsertString(28,_T("Vmid_R"));
		((CComboBox *)GetDlgItem(i))->InsertString(29,_T("Vmid_G"));
		((CComboBox *)GetDlgItem(i))->InsertString(30,_T("Vmid_B"));
		((CComboBox *)GetDlgItem(i))->InsertString(31,_T("Vmid_W"));
		((CComboBox *)GetDlgItem(i))->InsertString(32,_T("B_REF R"));
		((CComboBox *)GetDlgItem(i))->InsertString(33,_T("B_REF G"));
		((CComboBox *)GetDlgItem(i))->InsertString(34,_T("B_REF B"));
		((CComboBox *)GetDlgItem(i))->InsertString(35,_T("B_REF W"));
		((CComboBox *)GetDlgItem(i))->InsertString(36,_T("R_0/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(37,_T("G_0/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(38,_T("B_0/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(39,_T("W_0/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(40,_T("R_32/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(41,_T("G_32/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(42,_T("B_32/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(43,_T("W_32/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(44,_T("R_64/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(45,_T("G_64/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(46,_T("B_64/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(47,_T("W_64/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(48,_T("R_96/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(49,_T("G_96/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(50,_T("B_96/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(51,_T("W_96/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(52,_T("R_168/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(53,_T("G_168/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(54,_T("B_168/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(55,_T("W_168/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(56,_T("R_192/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(57,_T("G_192/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(58,_T("B_192/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(59,_T("W_192/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(60,_T("R_208/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(61,_T("G_208/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(62,_T("B_208/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(63,_T("W_208/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(64,_T("C.Unif R(min)"));
		((CComboBox *)GetDlgItem(i))->InsertString(65,_T("C.Unif R(min_lo)"));
		((CComboBox *)GetDlgItem(i))->InsertString(66,_T("C.Unif R(min_hi)"));
		((CComboBox *)GetDlgItem(i))->InsertString(67,_T("C.Unif R(max)"));
		((CComboBox *)GetDlgItem(i))->InsertString(68,_T("C.Unif G(min)"));
		((CComboBox *)GetDlgItem(i))->InsertString(69,_T("C.Unif G(min_lo)"));
		((CComboBox *)GetDlgItem(i))->InsertString(70,_T("C.Unif G(min_hi)"));
		((CComboBox *)GetDlgItem(i))->InsertString(71,_T("C.Unif G(max)"));
		((CComboBox *)GetDlgItem(i))->InsertString(72,_T("C.Unif B(min)"));
		((CComboBox *)GetDlgItem(i))->InsertString(73,_T("C.Unif B(min_lo)"));
		((CComboBox *)GetDlgItem(i))->InsertString(74,_T("C.Unif B(min_hi)"));
		((CComboBox *)GetDlgItem(i))->InsertString(75,_T("C.Unif B(max)"));
		((CComboBox *)GetDlgItem(i))->InsertString(76,_T("C.Unif W(min)"));
		((CComboBox *)GetDlgItem(i))->InsertString(77,_T("C.Unif W(min_lo)"));
		((CComboBox *)GetDlgItem(i))->InsertString(78,_T("C.Unif W(min_hi)"));
		((CComboBox *)GetDlgItem(i))->InsertString(79,_T("C.Unif W(max)"));
		((CComboBox *)GetDlgItem(i))->InsertString(80,_T("R_255/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(81,_T("G_255/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(82,_T("B_255/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(83,_T("W_255/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(84,_T("GammaGSWn"));
		((CComboBox *)GetDlgItem(i))->InsertString(85,_T("GammaGSGn"));
		((CComboBox *)GetDlgItem(i))->InsertString(86,_T("GammaGSWr"));
		((CComboBox *)GetDlgItem(i))->InsertString(87,_T("GammaGSGr"));
		((CComboBox *)GetDlgItem(i))->InsertString(88,_T("R_200/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(89,_T("G_200/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(90,_T("B_200/255"));
		((CComboBox *)GetDlgItem(i))->InsertString(91,_T("W_200/255"));
		//设置Pattern下拉框的默认选择项
		//((CComboBox *)GetDlgItem(i))->SetCurSel(0);
	}
	m_comboPattern0.SelectString(-1,_T("Nothing"));
	m_comboPattern1.SelectString(-1,_T("W_208/255"));
	m_comboPattern2.SelectString(-1,_T("W_96/255"));
	m_comboPattern3.SelectString(-1,_T("W_0/255"));
	m_comboPattern4.SelectString(-1,_T("R_208/255"));
	m_comboPattern5.SelectString(-1,_T("G_208/255"));
	m_comboPattern6.SelectString(-1,_T("B_208/255"));
	m_comboPattern7.SelectString(-1,_T("Gray Scale W"));
	m_comboPattern8.SelectString(-1,_T("Reverced Gray Scale W"));
	m_comboPattern9.SelectString(-1,_T("Nothing"));

	for (INT i=0; i<26; i++)
	{

		int nIndex = m_str.Find(_T("|"));

		m_StrRecvEdit = m_str.Left(nIndex);  //取得字符串第一个|符号前的字符

		m_str.Delete(0, m_str.Left(nIndex+1).GetLength());//删除字符串第一个|符号前的字符

		m_StrRecv[i] = m_StrRecvEdit;
	}
	if ( m_iORe == strEdit)
	{
		//AfxMessageBox(m_str);
		// 	给编辑框赋值
// 		m_StrRecv[0];	MAC地址  6
// 		m_StrRecv[1];	IP地址	4
// 		m_StrRecv[2];	机型    15
// 		m_StrRecv[3];	版本   8
// 		m_StrRecv[4];	Auto Power Off 2
// 		m_StrRecv[5];	Lamp On  3
// 		m_StrRecv[6];	Lamp Off  3
// 		m_StrRecv[7];	ECO Model 1
// 		m_StrRecv[8];	Standby Model 1
// 		m_StrRecv[9];	Online 1
// 		m_StrRecv[10];	安装方式 1
// 		m_StrRecv[11];	老化周期 1
//      m_StrRecv[12];  Pattern 9
// 		m_StrRecv[13];	Lamp Time 1
// 		m_StrRecv[14];	Lamp Time 2
	
		CString strIP(m_StrRecv[1]);
		SetDlgItemText(IDC_IPADDRESS_EDIT, strIP);
		m_EditIP.EnableWindow(FALSE);
		CString strType(m_StrRecv[2]);
		SetDlgItemText(IDC_TYPE_EDIT, strType);
		CString strAutoPO(m_StrRecv[4]);
		SetDlgItemText(IDC_AUTOPOFF_EDIT, strAutoPO);

		CString strLampOnTime(m_StrRecv[5]);
		CString strLampOn;
		CString strLampOnMinute;
		int nTemp = strLampOnTime.Find(_T("H"));
		strLampOn = strLampOnTime.Left(nTemp);
		strLampOnTime.Delete(0,strLampOnTime.Left(nTemp+1).GetLength());
		strLampOnTime.Remove(_T('M'));
		strLampOnMinute = strLampOnTime;
		SetDlgItemText(IDC_LAMPON_EDIT, strLampOn);
		SetDlgItemText(IDC_LAMPONMINUTE_EDIT, strLampOnMinute);
		CString strLampOffTime(m_StrRecv[6]);
		CString strLampOff;
		CString strLampOffMinute;
	    nTemp = strLampOffTime.Find(_T("H"));
		strLampOff = strLampOffTime.Left(nTemp);
		strLampOffTime.Delete(0,strLampOffTime.Left(nTemp+1).GetLength());
		strLampOffTime.Remove(_T('M'));
		strLampOffMinute = strLampOffTime;
		SetDlgItemText(IDC_LAMPOFF_EDIT, strLampOff);
		SetDlgItemText(IDC_LAMPOFFMINUTE_EDIT, strLampOffMinute);
		CString strECO(m_StrRecv[7]);
		m_comboECO.SelectString(-1, strECO);
		CString strStandBy(m_StrRecv[8]);
		m_comboStandby.SelectString(-1, strStandBy);
 		CString strSetup(m_StrRecv[10]);
		m_comboSetup.SelectString(-1, strSetup);
		SetDlgItemText(IDC_HEATRUN_EDIT,m_StrRecv[11]);
		m_comboPattern0.SelectString(-1,m_StrRecv[12]);
		m_comboPattern1.SetCurSel(_ttoi(m_StrRecv[13]));
		m_comboPattern2.SetCurSel(_ttoi(m_StrRecv[14]));
		m_comboPattern3.SetCurSel(_ttoi(m_StrRecv[15]));
		m_comboPattern4.SetCurSel(_ttoi(m_StrRecv[16]));
		m_comboPattern5.SetCurSel(_ttoi(m_StrRecv[17]));
		m_comboPattern6.SetCurSel(_ttoi(m_StrRecv[18]));
		m_comboPattern7.SetCurSel(_ttoi(m_StrRecv[19]));
		m_comboPattern8.SetCurSel(_ttoi(m_StrRecv[20]));
		m_comboPattern9.SetCurSel(_ttoi(m_StrRecv[21]));
		m_iORe = " ";
	}
	return TRUE;
}


CInsertEditDlg::~CInsertEditDlg()
{
}

void CInsertEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ECO_COMBO, m_comboECO);
	DDX_Control(pDX, IDC__STANDBY_COMBO, m_comboStandby);
	DDX_Control(pDX, IDC_TYPE_EDIT, m_TypeEdit);
	DDX_Control(pDX, IDC_AUTOPOFF_EDIT, m_AutoPOffEdit);
	DDX_Control(pDX, IDC_SETUP_COMBO, m_comboSetup);
	DDX_Control(pDX, IDC_LAMPON_EDIT, m_LampON);
	DDX_Control(pDX, IDC_LAMPOFF_EDIT, m_LampOFF);
	DDX_Control(pDX, IDC_PATTERN0_COMBO, m_comboPattern0);
	DDX_Control(pDX, IDC_PATTERN1_COMBO, m_comboPattern1);
	DDX_Control(pDX, IDC_PATTERN2_COMBO, m_comboPattern2);
	DDX_Control(pDX, IDC_PATTERN3_COMBO, m_comboPattern3);
	DDX_Control(pDX, IDC_PATTERN4_COMBO, m_comboPattern4);
	DDX_Control(pDX, IDC_PATTERN5_COMBO, m_comboPattern5);
	DDX_Control(pDX, IDC_PATTERN6_COMBO, m_comboPattern6);
	DDX_Control(pDX, IDC_PATTERN7_COMBO, m_comboPattern7);
	DDX_Control(pDX, IDC_PATTERN8_COMBO, m_comboPattern8);
	DDX_Control(pDX, IDC_PATTERN9_COMBO, m_comboPattern9);
	DDX_Control(pDX, IDC_LAMPONMINUTE_EDIT, m_LampONMinute);
	DDX_Control(pDX, IDC_LAMPOFFMINUTE_EDIT, m_LampOFFMinute);
	DDX_Control(pDX, IDC_IPADDRESS_EDIT, m_EditIP);
	DDX_Control(pDX, IDC_PROGRAM_COMBO, m_comboProgram);
}


BEGIN_MESSAGE_MAP(CInsertEditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInsertEditDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_PROGRAM_COMBO, &CInsertEditDlg::OnCbnSelchangeProgramCombo)
END_MESSAGE_MAP()


// CInsertEditDlg 消息处理程序

//确定按钮函数
void CInsertEditDlg::OnBnClickedOk()
{
	CString strMAC;
	CString strsIP ;
	CString strType;
	CString strPWVersion;
	CString strAutoPOff;
	CString strLampOn;
	CString strLampOnMinute;
	CString strLampOnTime;
	CString strLampOff;
	CString strLampOffMinute;
	CString strLampOffTime;
	CString strEco;
	CString strStandby;
	int nECO;
	int nStandby;
	CString strOnline;
	int nOnline;
	CString strSetup;
	int nSetup;
	CString strHeatRunTime;
	CString strHeatRunTimeChar[4];
	CString strPattern0;
	CString strPattern1;
	CString strPattern2;
	CString strPattern3;
	CString strPattern4;
	CString strPattern5;
	CString strPattern6;
	CString strPattern7;
	CString strPattern8;
	CString strPattern9;
	int nPattern0;
	int nPattern1;
	int nPattern2;
	int nPattern3;
	int nPattern4;
	int nPattern5;
	int nPattern6;
	int nPattern7;
	int nPattern8;
	int nPattern9;
 	CString strLampTime1;
 	CString strLampTime2;
	CString strTemp;
	CString strAutoPOffChar[3];
	CString strLampOnChar[4];
	CString strLampOffChar[4];
	CString strLampTime1Char[4];
	CString strLampTime2Char[4];
	CString strSendStatus;
	//BYTE *pByte[100];
	BYTE pTemp[100];
	BYTE pByte[100];
	memset(pTemp,0,100);
	memset(pByte,0,100);

	strMAC = m_StrRecv[0];
	GetDlgItemText(IDC_IPADDRESS_EDIT, strsIP);
	GetDlgItemText(IDC_TYPE_EDIT, strType);
	strPWVersion = m_StrRecv[3];
	GetDlgItemText(IDC_AUTOPOFF_EDIT, strAutoPOff);
	GetDlgItemText(IDC_LAMPON_EDIT, strLampOn);
	GetDlgItemText(IDC_LAMPONMINUTE_EDIT, strLampOnMinute);
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
	GetDlgItemText(IDC_LAMPOFF_EDIT, strLampOff);
	GetDlgItemText(IDC_LAMPOFFMINUTE_EDIT, strLampOffMinute);
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
	GetDlgItemText(IDC_ECO_COMBO, strEco);
	GetDlgItemText(IDC__STANDBY_COMBO, strStandby);
	nECO = m_comboECO.GetCurSel();
	nStandby = m_comboStandby.GetCurSel();
	GetDlgItemText(IDC_SETUP_COMBO, strSetup); 
	nSetup = m_comboSetup.GetCurSel();
	strOnline = m_StrRecv[9];
	GetDlgItemText(IDC_HEATRUN_EDIT, strHeatRunTime);
	GetDlgItemText(IDC_PATTERN0_COMBO, strPattern0);
	GetDlgItemText(IDC_PATTERN1_COMBO, strPattern1);
	GetDlgItemText(IDC_PATTERN2_COMBO, strPattern2);
	GetDlgItemText(IDC_PATTERN3_COMBO, strPattern3);
	GetDlgItemText(IDC_PATTERN4_COMBO, strPattern4);
	GetDlgItemText(IDC_PATTERN5_COMBO, strPattern5);
	GetDlgItemText(IDC_PATTERN6_COMBO, strPattern6);
	GetDlgItemText(IDC_PATTERN7_COMBO, strPattern7);
	GetDlgItemText(IDC_PATTERN8_COMBO, strPattern8);
	GetDlgItemText(IDC_PATTERN9_COMBO, strPattern9);
	nPattern0 = m_comboPattern0.GetCurSel();
	if (nPattern0 != -1)
	{
		if (PatternSwitch(nPattern0))
		{
			CString strMsg;
			strMsg = strPattern0 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern1 = m_comboPattern1.GetCurSel();
	if (nPattern1 != -1)
	{
		if (PatternSwitch(nPattern1))
		{
			CString strMsg;
			strMsg = strPattern1 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern2 = m_comboPattern2.GetCurSel();
	if (nPattern2 != -1)
	{
		if (PatternSwitch(nPattern2))
		{
			CString strMsg;
			strMsg = strPattern2 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern3 = m_comboPattern3.GetCurSel();
	if (nPattern3 != -1)
	{
		if (PatternSwitch(nPattern3))
		{
			CString strMsg;
			strMsg = strPattern3 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern4 = m_comboPattern4.GetCurSel();
	if (nPattern4 != -1)
	{
		if (PatternSwitch(nPattern4))
		{
			CString strMsg;
			strMsg = strPattern4 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern5 = m_comboPattern5.GetCurSel();
	if (nPattern5 != -1)
	{
		if (PatternSwitch(nPattern5))
		{
			CString strMsg;
			strMsg = strPattern5 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern6 = m_comboPattern6.GetCurSel();
	if (nPattern6 != -1)
	{
		if (PatternSwitch(nPattern6))
		{
			CString strMsg;
			strMsg = strPattern6 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern7 = m_comboPattern7.GetCurSel();
	if (nPattern7 != -1)
	{
		if (PatternSwitch(nPattern7))
		{
			CString strMsg;
			strMsg = strPattern7 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern8 = m_comboPattern8.GetCurSel();
	if (nPattern8 != -1)
	{
		if (PatternSwitch(nPattern8))
		{
			CString strMsg;
			strMsg = strPattern8 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	nPattern9 = m_comboPattern9.GetCurSel();
	if (nPattern9 != -1)
	{
		if (PatternSwitch(nPattern9))
		{
			CString strMsg;
			strMsg = strPattern9 + _T("Can't be used for the present!");
			AfxMessageBox(strMsg);
			return;
		}
	}
	
	strLampTime1 = m_StrRecv[21];
	strLampTime2 = m_StrRecv[22];

	strPattern1.Format(_T("%d"),nPattern1);
	strPattern2.Format(_T("%d"),nPattern2);
	strPattern3.Format(_T("%d"),nPattern3);
	strPattern4.Format(_T("%d"),nPattern4);
	strPattern5.Format(_T("%d"),nPattern5);
	strPattern6.Format(_T("%d"),nPattern6);
	strPattern7.Format(_T("%d"),nPattern7);
	strPattern8.Format(_T("%d"),nPattern8);
	strPattern9.Format(_T("%d"),nPattern9);

	CHeatRunDlg *pDlg = (CHeatRunDlg*)GetParent();

	int nCount = pDlg->m_hrList.GetItemCount();
	if (m_iORe == "Insert")
	{
		CString strListIPChar[100];
		for (int i=0;i<nCount;i++)
		{
			strListIPChar[i] = pDlg->m_hrList.GetItemText(i,1);
			if (strsIP == strListIPChar[i])
			{
				AfxMessageBox(_T("IP Has Been Existing!"));
				return;
			}
		}
		strSendStatus = _T("02"); //添加
		if (strType == "")
		{
			AfxMessageBox(_T("Please input Type!"));
			return;
		}
		pDlg->m_hrList.SetTextBkColor(RGB(204,0,0));
		pDlg->m_hrList.InsertItem(nCount, strMAC);   
		pDlg->m_hrList.SetItemText(nCount, 1, strsIP);  
		pDlg->m_hrList.SetItemText(nCount, 2, strType);   
		pDlg->m_hrList.SetItemText(nCount, 3, strPWVersion); 
		pDlg->m_hrList.SetItemText(nCount, 4, strAutoPOff);   
		pDlg->m_hrList.SetItemText(nCount, 5, strLampOnTime); 
		pDlg->m_hrList.SetItemText(nCount, 6, strLampOffTime);   
		pDlg->m_hrList.SetItemText(nCount, 7, strEco);   
		pDlg->m_hrList.SetItemText(nCount, 8, strStandby); 
		pDlg->m_hrList.SetItemText(nCount, 9, strOnline); 
		pDlg->m_hrList.SetItemText(nCount, 10, strSetup);   
		pDlg->m_hrList.SetItemText(nCount, 11, strHeatRunTime); 
		pDlg->m_hrList.SetItemText(nCount, 12, strPattern0); 
		pDlg->m_hrList.SetItemText(nCount, 13, strPattern1); 
		pDlg->m_hrList.SetItemText(nCount, 14, strPattern2); 
		pDlg->m_hrList.SetItemText(nCount, 15, strPattern3); 
		pDlg->m_hrList.SetItemText(nCount, 16, strPattern4); 
		pDlg->m_hrList.SetItemText(nCount, 17, strPattern5); 
		pDlg->m_hrList.SetItemText(nCount, 18, strPattern6); 
		pDlg->m_hrList.SetItemText(nCount, 19, strPattern7); 
		pDlg->m_hrList.SetItemText(nCount, 20, strPattern8);
		pDlg->m_hrList.SetItemText(nCount, 21, strPattern9);
		pDlg->m_hrList.SetItemText(nCount, 22, strLampTime1);  
		pDlg->m_hrList.SetItemText(nCount, 23, strLampTime2);
		pDlg->m_hrList.SetItemText(nCount, 24, _T("1"));
	
		m_iORe = " ";
	}
	else
	{
		strSendStatus = _T("02"); //编辑
		if (strType == "")
		{
			AfxMessageBox(_T("Please input Type!"));
			return;
		}
		pDlg->m_hrList.DeleteItem(pDlg->nListITEM);
		if (pDlg->nListITEM ==0)
		{
			pDlg->m_hrList.InsertItem(0, strMAC);   
			pDlg->m_hrList.SetItemText(0, 1, strsIP);  
			pDlg->m_hrList.SetItemText(0, 2, strType);   
			pDlg->m_hrList.SetItemText(0, 3, strPWVersion);   
			pDlg->m_hrList.SetItemText(0, 4, strAutoPOff);   
			pDlg->m_hrList.SetItemText(0, 5, strLampOnTime); 
			pDlg->m_hrList.SetItemText(0, 6, strLampOffTime);   
			pDlg->m_hrList.SetItemText(0, 7, strEco);   
			pDlg->m_hrList.SetItemText(0, 8, strStandby); 
			pDlg->m_hrList.SetItemText(0, 9, strOnline); 
			pDlg->m_hrList.SetItemText(0, 10, strSetup);   
			pDlg->m_hrList.SetItemText(0, 11, strHeatRunTime); 
			pDlg->m_hrList.SetItemText(0, 12, strPattern0); 
			pDlg->m_hrList.SetItemText(0, 13, strPattern1); 
			pDlg->m_hrList.SetItemText(0, 14, strPattern2); 
			pDlg->m_hrList.SetItemText(0, 15, strPattern3); 
			pDlg->m_hrList.SetItemText(0, 16, strPattern4); 
			pDlg->m_hrList.SetItemText(0, 17, strPattern5); 
			pDlg->m_hrList.SetItemText(0, 18, strPattern6); 
			pDlg->m_hrList.SetItemText(0, 19, strPattern7); 
			pDlg->m_hrList.SetItemText(0, 20, strPattern8); 
			pDlg->m_hrList.SetItemText(0, 21, strPattern9); 
			pDlg->m_hrList.SetItemText(0, 22, strLampTime1);  
			pDlg->m_hrList.SetItemText(0, 23, strLampTime2);
			pDlg->m_hrList.SetItemText(nCount, 24, _T("1"));
		}
		else
		{
			if (strType == "")
			{
				AfxMessageBox(_T("Please input type!"));
				return;
			}
			pDlg->m_hrList.InsertItem(pDlg->nListITEM, strMAC);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 1, strsIP);  		
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 2, strType);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 3, strPWVersion);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 4, strAutoPOff);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 5, strLampOnTime); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 6, strLampOffTime);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 7, strEco);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 8, strStandby); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 9, strOnline); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 10, strSetup);   
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 11, strHeatRunTime); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 12, strPattern0); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 13, strPattern1); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 14, strPattern2); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 15, strPattern3); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 16, strPattern4); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 17, strPattern5); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 18, strPattern6); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 19, strPattern7); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 20, strPattern8); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 21, strPattern9); 
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 22, strLampTime1);  
			pDlg->m_hrList.SetItemText(pDlg->nListITEM, 23, strLampTime2);
			pDlg->m_hrList.SetItemText(nCount, 24, _T("1"));
		}
		

		m_iORe = " ";
		pDlg->langStr = "";
	}
	
	CString m_MACAddrChar[6];
	TCHAR sepsMAC[] = _T(":");
	if (strMAC != "")
	{
	
		TCHAR *tokenMAC = _tcstok((LPTSTR)(LPCTSTR)strMAC, sepsMAC);
		for (int i=0;i<6;i++)
		{
			m_MACAddrChar[i] = tokenMAC;
			tokenMAC = _tcstok(NULL, sepsMAC);

		}
	}
	else
	{
		 	m_MACAddrChar[0].Format(_T("%02X"), 0);
			m_MACAddrChar[1].Format(_T("%02X"), 0);
			m_MACAddrChar[2].Format(_T("%02X"), 0);
			m_MACAddrChar[3].Format(_T("%02X"), 0);
			m_MACAddrChar[4].Format(_T("%02X"), 0);
			m_MACAddrChar[5].Format(_T("%02X"), 0);
	}

	
	strAutoPOff.Format(_T("%02X"), _ttoi(strAutoPOff));
	
	strLampOnTime.Format(_T("%08X"), (_ttoi(strLampOn)*3600)+(_ttoi(strLampOnMinute)*60));
	
	strLampOffTime.Format(_T("%08X"), (_ttoi(strLampOff)*3600)+(_ttoi(strLampOffMinute)*60));
	
	
	if (strOnline == "Online")
	{
		nOnline = 1;
	}
	else
	{
		nOnline = 0;
	}
	CString strEcoNumChar;
	CString strStandbyNumChar;
	CString strSetupNumChar;
	CString strOnlineChar;
	if (nECO == 3)
	{
		strEcoNumChar.Format(_T("%02X"), 16);
	}
	else if (nECO == 4)
	{
		strEcoNumChar.Format(_T("%02X"), 32);
	}
	else
	{
		strEcoNumChar.Format(_T("%02X"), nECO);
	}
	strStandbyNumChar.Format(_T("%02X"), nStandby);
	
	strSetupNumChar.Format(_T("%02X"), nSetup);
	
	strHeatRunTime.Format(_T("%08X"), _ttoi(strHeatRunTime));

	CString strPattern0Char;
	CString strPattern1Char;
	CString strPattern2Char;
	CString strPattern3Char;
	CString strPattern4Char;
	CString strPattern5Char;
	CString strPattern6Char;
	CString strPattern7Char;
	CString strPattern8Char;
	CString strPattern9Char;
	if(nPattern0 != -1)
		strPattern0Char.Format(_T("%02X"), nPattern0);
	else
		strPattern0Char.Format(_T("%02X"), 00);
	if(nPattern1 != -1)
		strPattern1Char.Format(_T("%02X"), nPattern1);
	else
		strPattern1Char.Format(_T("%02X"), 00);
	if(nPattern2 != -1)
		strPattern2Char.Format(_T("%02X"), nPattern2);
	else
		strPattern2Char.Format(_T("%02X"), 00);
	if(nPattern3 != -1)
		strPattern3Char.Format(_T("%02X"), nPattern3);
	else
		strPattern3Char.Format(_T("%02X"), 00);
	if(nPattern4 != -1)
		strPattern4Char.Format(_T("%02X"), nPattern4);
	else
		strPattern4Char.Format(_T("%02X"), 00);
	if(nPattern5 != -1)
		strPattern5Char.Format(_T("%02X"), nPattern5);
	else
		strPattern5Char.Format(_T("%02X"), 00);
	if(nPattern6 != -1)
		strPattern6Char.Format(_T("%02X"), nPattern6);
	else
		strPattern6Char.Format(_T("%02X"), 00);
	if(nPattern7 != -1)
		strPattern7Char.Format(_T("%02X"), nPattern7);
	else
		strPattern7Char.Format(_T("%02X"), 00);
	if(nPattern8 != -1)
		strPattern8Char.Format(_T("%02X"), nPattern8);
	else
		strPattern8Char.Format(_T("%02X"), 00);
	if(nPattern9 != -1)
		strPattern9Char.Format(_T("%02X"), nPattern9);
	else
		strPattern9Char.Format(_T("%02X"), 00);

	strTemp = m_MACAddrChar[0] + m_MACAddrChar[1] + m_MACAddrChar[2] + m_MACAddrChar[3] + m_MACAddrChar[4] + m_MACAddrChar[5] + strsIP + strType;

	 char strTr[100];
	 memset(strTr,0,100);
	 memcpy(strTr,strTemp,strTemp.GetLength());
	 char buff[100];
	 memset(buff,0,100);
	 for (int i=0;i<sizeof(strTr)/sizeof(strTr[0]);i++)
	 {
		 buff[i] = strTr[2*i];
	 }


	 memcpy(&pTemp,buff,42);

 	CString strLength;
	int nstrXor=0;
	CString strXor;
	strLength.Format(_T("%04X"), 68);//发送固定长度

	
	CString strPacket(_T("7E"));
	CString strPacketEnd(_T("7E"));
	
	pByte[0]=Comm_2NumCharToHex(strPacket[0],strPacket[1]);
	pByte[1]=Comm_2NumCharToHex(strLength[0],strLength[1]);
	pByte[2]=Comm_2NumCharToHex(strLength[2],strLength[3]);
	
	pByte[4]=Comm_2NumCharToHex(strSendStatus[0],strSendStatus[1]);

	memcpy(&pByte[5],pTemp,12);
	int nIPLen=strsIP.GetLength();
	int nTypeLen=strType.GetLength();

		for (int i=17;i<32;i++)
		{
			if (i-17>=nIPLen)
			{
				pByte[i]=0x00;
			}
			else
			{
				pByte[i]=strsIP[i-17];
			}	
		}
		for (int i=32;i<47;i++)
		{
			if (i-32>=nTypeLen)
			{
				pByte[i]=0x00;
			}
			else
			{
				pByte[i]=strType[i-32];
			}	
		}


	

	pByte[47] = Comm_2NumCharToHex(strAutoPOff[0],strAutoPOff[1]);
	CString strLampOnT(strLampOnTime);

	for (int i=48;i<52;i++)
	{
		CString strToken = strLampOnT.Left(2);
		strLampOnT.Delete(0,2);

		pByte[i] = Comm_2NumCharToHex(strToken[0],strToken[1]);
	}

	CString strLampOffT(strLampOffTime);

	for (int i=52;i<56;i++)
	{
		CString strToken = strLampOffT.Left(2);
		strLampOffT.Delete(0,2);

		pByte[i] = Comm_2NumCharToHex(strToken[0],strToken[1]);
	}
	pByte[56] = Comm_2NumCharToHex(strEcoNumChar[0],strEcoNumChar[1]);
	pByte[57] = Comm_2NumCharToHex(strStandbyNumChar[0],strStandbyNumChar[1]);
	pByte[58] =Comm_2NumCharToHex(strSetupNumChar[0],strSetupNumChar[1]);

	
	CString strHeatRunT(strHeatRunTime);


	for (int i=59;i<63;i++)
	{
		CString strToken = strHeatRunT.Left(2);
		strHeatRunT.Delete(0,2);

		pByte[i] = Comm_2NumCharToHex(strToken[0],strToken[1]);
	}
	pByte[63] = Comm_2NumCharToHex(strPattern0Char[0],strPattern0Char[1]);
	pByte[64] = Comm_2NumCharToHex(strPattern1Char[0],strPattern1Char[1]);
	pByte[65] = Comm_2NumCharToHex(strPattern2Char[0],strPattern2Char[1]);
	pByte[66] = Comm_2NumCharToHex(strPattern3Char[0],strPattern3Char[1]);
	pByte[67] = Comm_2NumCharToHex(strPattern4Char[0],strPattern4Char[1]);
	pByte[68] = Comm_2NumCharToHex(strPattern5Char[0],strPattern5Char[1]);
	pByte[69] = Comm_2NumCharToHex(strPattern6Char[0],strPattern6Char[1]);
	pByte[70] = Comm_2NumCharToHex(strPattern7Char[0],strPattern7Char[1]);
	pByte[71] = Comm_2NumCharToHex(strPattern8Char[0],strPattern8Char[1]);
	pByte[72] = Comm_2NumCharToHex(strPattern9Char[0],strPattern9Char[1]);
	
	for (int i=5;i<73;i++)
	{
		nstrXor ^= pByte[i];
	}
	strXor.Format(_T("%02X"), nstrXor);
	pByte[3]=Comm_2NumCharToHex(strXor[0],strXor[1]);

	pByte[73]=Comm_2NumCharToHex(strPacketEnd[0],strPacketEnd[1]);

	
 	
	//Socket
	CHeatRunDlg *pHeatRunDlg = (CHeatRunDlg*)GetParent();
	pHeatRunDlg->pSocket->Send(pByte,74);

	CDialogEx::OnOK();
}

void CInsertEditDlg::OnCbnSelchangeProgramCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	int nComboNum = m_comboProgram.GetCurSel();

	if (nComboNum ==1)
	{
		m_comboPattern0.SelectString(-1,_T("W_255/255"));
		m_comboPattern1.SelectString(-1,_T("W_255/255"));
		m_comboPattern2.SelectString(-1,_T("W_255/255"));
		m_comboPattern3.SelectString(-1,_T("W_255/255"));
		m_comboPattern4.SelectString(-1,_T("W_255/255"));
		m_comboPattern5.SelectString(-1,_T("W_255/255"));
		m_comboPattern6.SelectString(-1,_T("W_255/255"));
		m_comboPattern7.SelectString(-1,_T("W_255/255"));
		m_comboPattern8.SelectString(-1,_T("W_255/255"));
		m_comboPattern9.SelectString(-1,_T("W_255/255"));
	}
	else if (nComboNum == 2)
	{
		m_comboPattern0.SelectString(-1,_T("W_32/255"));
		m_comboPattern1.SelectString(-1,_T("W_32/255"));
		m_comboPattern2.SelectString(-1,_T("W_32/255"));
		m_comboPattern3.SelectString(-1,_T("W_32/255"));
		m_comboPattern4.SelectString(-1,_T("W_32/255"));
		m_comboPattern5.SelectString(-1,_T("W_32/255"));
		m_comboPattern6.SelectString(-1,_T("W_32/255"));
		m_comboPattern7.SelectString(-1,_T("W_32/255"));
		m_comboPattern8.SelectString(-1,_T("W_32/255"));
		m_comboPattern9.SelectString(-1,_T("W_32/255"));
	}
	else if (nComboNum == 3)
	{
		m_comboPattern0.SelectString(-1,_T("W_0/255"));
		m_comboPattern1.SelectString(-1,_T("W_0/255"));
		m_comboPattern2.SelectString(-1,_T("W_0/255"));
		m_comboPattern3.SelectString(-1,_T("W_0/255"));
		m_comboPattern4.SelectString(-1,_T("W_0/255"));
		m_comboPattern5.SelectString(-1,_T("W_0/255"));
		m_comboPattern6.SelectString(-1,_T("W_0/255"));
		m_comboPattern7.SelectString(-1,_T("W_0/255"));
		m_comboPattern8.SelectString(-1,_T("W_0/255"));
		m_comboPattern9.SelectString(-1,_T("W_0/255"));
	}
	else if (nComboNum == 4)
	{
		m_comboPattern0.SelectString(-1,_T("Nothing"));
		m_comboPattern1.SelectString(-1,_T("W_208/255"));
		m_comboPattern2.SelectString(-1,_T("W_96/255"));
		m_comboPattern3.SelectString(-1,_T("W_0/255"));
		m_comboPattern4.SelectString(-1,_T("R_208/255"));
		m_comboPattern5.SelectString(-1,_T("G_208/255"));
		m_comboPattern6.SelectString(-1,_T("B_208/255"));
		m_comboPattern7.SelectString(-1,_T("Gray Scale W"));
		m_comboPattern8.SelectString(-1,_T("Reverced Gray Scale W"));
		m_comboPattern9.SelectString(-1,_T("Nothing"));
	}
}
