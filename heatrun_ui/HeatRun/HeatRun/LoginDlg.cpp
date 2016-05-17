// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CLoginDlg::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_QUIT_BUTTON, &CLoginDlg::OnBnClickedQuitButton)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData(TRUE);

	CString strPwd;
	this->GetDlgItemText(IDC_EDIT_PWD, strPwd);
	if(strPwd == "1") //密码

		CDialogEx::OnOK();

	else

		MessageBox(_T("Please input password!"));
}


void CLoginDlg::OnBnClickedQuitButton()
{
	// TODO: 在此添加控件通知处理程序代码
	exit(0);
}


