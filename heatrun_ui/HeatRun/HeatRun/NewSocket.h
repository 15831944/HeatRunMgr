#pragma once
class CHeatRunDlg;
#include <afxsock.h>
// CNewSocket ÃüÁîÄ¿±ê

class CNewSocket : public CSocket
{
public:
	CNewSocket();
	virtual ~CNewSocket();
	void GetDlg(CHeatRunDlg * dlg);
	CHeatRunDlg* m_dlg;
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
};


