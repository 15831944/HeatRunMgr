// EditDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "EditDlg.h"
#include "afxdialogex.h"



// CEditDlg 对话框

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
{

}

//初始化对话框
BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	

	//给编辑框赋值
	SetDlgItemText(IDC_HREDIT_EDIT, m_str);

	//MessageBox(sss, _T("SSSSS"), MB_OK);  //alert提示
	return TRUE;
}

CEditDlg::~CEditDlg()
{
}

void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEditDlg, CDialogEx)

END_MESSAGE_MAP()


// CEditDlg 消息处理程序


