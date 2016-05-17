#pragma once
class CHeatRunDlg;
class CRecordDlg;
#include <afxsock.h>

// CNewAsyncSocket ÃüÁîÄ¿±ê

class CNewAsyncSocket : public CAsyncSocket
{
public:
	CNewAsyncSocket();
	virtual ~CNewAsyncSocket();
	void GetDlg(CHeatRunDlg *dlg = (CHeatRunDlg *)AfxGetApp()->GetMainWnd());
	void GetRDlg(CRecordDlg *rDlg = (CRecordDlg *)AfxGetApp()->GetMainWnd());
	CHeatRunDlg *m_dlg;
	CRecordDlg *m_Rdlg;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
};


