#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CServerIP �Ի���

class CServerIP : public CDialogEx
{
	DECLARE_DYNAMIC(CServerIP)

public:
	CServerIP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CServerIP();
	//CNewAsyncSocket* pSocket; 

// �Ի�������
	enum { IDD = IDD_SERVERIP_DIALOG };

protected:
	virtual BOOL CServerIP::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ServerIPEdit;
	CEdit m_PortEdit;
	afx_msg void OnBnClickedOk();
	CString serverIP;
	CString serverPort;
};
