// NewSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "NewSocket.h"
#include "HeatRunDlg.h"

// CNewSocket

CNewSocket::CNewSocket()
{
}

CNewSocket::~CNewSocket()
{
}


// CNewSocket 成员函数

void CNewSocket::GetDlg(CHeatRunDlg *dlg)
{
	m_dlg=dlg;
}

void CNewSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnConnect(nErrorCode);
}


void CNewSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_dlg->OnReceive(nErrorCode);
	CSocket::OnReceive(nErrorCode);
}


void CNewSocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类

	CSocket::OnSend(nErrorCode);
}
