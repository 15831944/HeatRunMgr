// EditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HeatRun.h"
#include "EditDlg.h"
#include "afxdialogex.h"



// CEditDlg �Ի���

IMPLEMENT_DYNAMIC(CEditDlg, CDialogEx)

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEditDlg::IDD, pParent)
{

}

//��ʼ���Ի���
BOOL CEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();	

	//���༭��ֵ
	SetDlgItemText(IDC_HREDIT_EDIT, m_str);

	//MessageBox(sss, _T("SSSSS"), MB_OK);  //alert��ʾ
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


// CEditDlg ��Ϣ�������


