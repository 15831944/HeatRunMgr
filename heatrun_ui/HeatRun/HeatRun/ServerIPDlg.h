#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CServerIP 对话框

class CServerIP : public CDialogEx
{
	DECLARE_DYNAMIC(CServerIP)

public:
	CServerIP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServerIP();
	//CNewAsyncSocket* pSocket; 

// 对话框数据
	enum { IDD = IDD_SERVERIP_DIALOG };

protected:
	virtual BOOL CServerIP::OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_ServerIPEdit;
	CEdit m_PortEdit;
	afx_msg void OnBnClickedOk();
	CString serverIP;
	CString serverPort;
};
