// NewSocket.cpp : ʵ���ļ�
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


// CNewSocket ��Ա����

void CNewSocket::GetDlg(CHeatRunDlg *dlg)
{
	m_dlg=dlg;
}

void CNewSocket::OnConnect(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnConnect(nErrorCode);
}


void CNewSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	m_dlg->OnReceive(nErrorCode);
	CSocket::OnReceive(nErrorCode);
}


void CNewSocket::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���

	CSocket::OnSend(nErrorCode);
}
