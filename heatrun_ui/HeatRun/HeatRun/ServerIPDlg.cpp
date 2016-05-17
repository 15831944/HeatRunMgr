// ServerIP.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "HeatRunDlg.h"
#include "ServerIPDlg.h"
#include "afxdialogex.h"

// CServerIP 对话框

IMPLEMENT_DYNAMIC(CServerIP, CDialogEx)

CServerIP::CServerIP(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerIP::IDD, pParent)
{

}

CServerIP::~CServerIP()
{
}

BOOL CServerIP::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CHeatRunDlg *pHeatRunDlg = (CHeatRunDlg*)GetParent();

	SetDlgItemText(IDC_SERVERIP_IPADDRESS, pHeatRunDlg->strServerIP);
	//SetDlgItemText(IDC_SERVERIP_IPADDRESS, _T("10.97.24.158"));
	CString strPort;
	if (pHeatRunDlg->nPort != -858993460)
		strPort.Format(_T("%d"), pHeatRunDlg->nPort);
	//SetDlgItemText(IDC_SERVERPORT_EDIT, strPort);
	SetDlgItemText(IDC_SERVERPORT_EDIT, _T("9906"));
	GetDlgItem(IDC_SERVERPORT_EDIT)->EnableWindow(FALSE);
	return TRUE;
}
void CServerIP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ServerIPEdit);
	DDX_Control(pDX, IDC_EDIT1, m_PortEdit);
}


BEGIN_MESSAGE_MAP(CServerIP, CDialogEx)
	ON_BN_CLICKED(IDOK, &CServerIP::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerIP 消息处理程序


void CServerIP::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItemText(IDC_SERVERIP_IPADDRESS, serverIP);
	GetDlgItemText(IDC_SERVERPORT_EDIT, serverPort);

	CHeatRunDlg *pHeatRunDlg = (CHeatRunDlg*)GetParent();
	
	pHeatRunDlg->strServerIP = serverIP;
	pHeatRunDlg->nPort = _ttoi(serverPort);
	if (serverIP != "0.0.0.0")
	{
		pHeatRunDlg->GetDlgItem(IDC_HR_SETTING)->EnableWindow(TRUE);
		pHeatRunDlg->GetDlgItem(IDC_DELETE_BUTTON)->EnableWindow(TRUE);
		pHeatRunDlg->GetDlgItem(IDC_BUTTON_REFRESH)->EnableWindow(TRUE);
		pHeatRunDlg->GetDlgItem(IDC_HR_RECORD)->EnableWindow(TRUE);
		pHeatRunDlg->GetDlgItem(IDC_STOP_BUTTON)->EnableWindow(TRUE);
	}

	pHeatRunDlg->SocketConnect(serverIP,_ttoi(serverPort));

	CDialogEx::OnOK();
}
