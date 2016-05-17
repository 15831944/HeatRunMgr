// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatRun.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������


void CLoginDlg::OnBnClickedButtonOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	this->UpdateData(TRUE);

	CString strPwd;
	this->GetDlgItemText(IDC_EDIT_PWD, strPwd);
	if(strPwd == "1") //����

		CDialogEx::OnOK();

	else

		MessageBox(_T("Please input password!"));
}


void CLoginDlg::OnBnClickedQuitButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	exit(0);
}


