#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"

#define  RecordDataLen    113

// CRecordDlg 对话框

class CRecordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRecordDlg)

public:
	CRecordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRecordDlg();

// 对话框数据
	enum { IDD = IDD_RECORD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL CRecordDlg::OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_RecordList;
	CIPAddressCtrl m_RecordIP;
	CDateTimeCtrl m_RecordDate1;
	CDateTimeCtrl m_RecordDate2;
	void ReceiveRecordDataMessage(BYTE *pbuf);
	void setRList(char* str);
	void RecordDataProcessFormOS(char* charGetString);
	void ReceiveAlert();
	afx_msg void OnBnClickedButton1();
	CString m_TestValue;
	CEdit m_TestR;
	virtual void OnOK();
};
