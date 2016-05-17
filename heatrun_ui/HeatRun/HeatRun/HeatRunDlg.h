
// HeatRunDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "NewAsyncSocket.h"
#include "ServerIPDlg.h"



//#define WM_SOCKET_NOTIFY (WM_USER +100) 
// CHeatRunDlg 对话框
class CHeatRunDlg : public CDialogEx
{
// 构造
public:
	CHeatRunDlg(CWnd* pParent = NULL);	// 标准构造函数
	void OnReceive(int nErrorCode=0);
	void CHeatRunDlg::ReceiveDataMessage(BYTE *pbuf,int Len);
	void CHeatRunDlg::ReceiveVersionMessage(BYTE *pbuf,int Len);
	BYTE* CHeatRunDlg::SendDataMessage(BYTE pByte[100]);
	void CHeatRunDlg::OnConnect();
	CNewAsyncSocket* pSocket; 
	/*CServerIP *pServerIPDlg;*/

// 对话框数据
	enum { IDD = IDD_HEATRUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//afx_msg void OnLvnItemchangedHrList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_hrList;
	afx_msg void OnBnClickedHrSetting();
	CString langStr;
	int nListITEM;
	int nListITEMClick;
	afx_msg void OnBnClickedHrRecord();
	int m_nHRData;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRefresh();
	CString strServerIP;
	int nPort;
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMDblclkHrList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnSocket(WPARAM wParam, LPARAM lParam);
	void setHrList(char* str);
	char szRecValue[1024];
protected:
	
public:
	CString m_TestEdit;
	afx_msg void OnClickHrList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void SocketConnect(CString serverIP,INT portNum);
	afx_msg void OnBnClickedStopButton();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	void CHeatRunDlg::DataProcessFormOS(char* charGetString);
	CString ColorNumber;
	INT nRefreshStatus;
	INT nRefreshSecond;
	INT nSecond;
	virtual void OnOK();
};
