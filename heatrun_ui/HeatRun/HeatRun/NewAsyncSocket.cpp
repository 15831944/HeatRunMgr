// NewAsyncSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatRun.h"
#include "NewAsyncSocket.h"
#include "HeatRunDlg.h"
#include "RecordDlg.h"
#include "Global.h"
// CNewAsyncSocket

CNewAsyncSocket::CNewAsyncSocket()
{
}

CNewAsyncSocket::~CNewAsyncSocket()
{
}


// CNewAsyncSocket 成员函数


void CNewAsyncSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (0 == nErrorCode)
	{
		//BYTE *pbuf=new BYTE[1024];
//		char szRecValue[10200] = {0};
		BYTE szRecValue[60000] = {0};
		BYTE strByte[113]={0};
 		int Len = this->Receive((void *)szRecValue,sizeof(szRecValue));
		if (szRecValue[4] == 0x81)
		{
			// 			memset(strHeatRunChar,0,5100);
			// 			memcpy(strHeatRunChar,szRecValue,Len);
			m_dlg->ReceiveDataMessage((BYTE *)szRecValue,Len);
		}
		else if (szRecValue[4] == 0x07)
		{
			memcpy(strByte,szRecValue,113);
			m_dlg->ReceiveVersionMessage((BYTE *)szRecValue,Len);
		}
		else if (szRecValue[4] == 0x83)
		{
			m_Rdlg->ReceiveAlert();
		}
		else if (szRecValue[4] == 0x84)
		{
			char strLenChar[2];
			char strEndLenchar[2];
			CString strLen;
			CString strEndLen;
			memset(strLenChar,0,2);
			memcpy(strLenChar,szRecValue+5,2);
			memset(strEndLenchar,0,2);
			memcpy(strEndLenchar,szRecValue+Len-3,2);

			strLen.Format(_T("%02X%02X"),(unsigned char)strLenChar[0],(unsigned char)strLenChar[1]);
			strLen = HexToDem(strLen);
			strEndLen.Format(_T("%02X%02X"),(unsigned char)strEndLenchar[0],(unsigned char)strEndLenchar[1]);
			strEndLen = HexToDem(strEndLen);
			int nLen = _ttoi(strLen);
			int nEndLen = _ttoi(strEndLen);
			//添加Sleep时间用来等待数据接收完全
			if (nLen>100)
			{
				Sleep(300);
			}
			else if (100>nLen && nLen>50)
			{
				Sleep(100);
			}
			else if (nLen<50)
			{
				Sleep(50);
			}
			//if (nEndLen+1 == nLen)
			//{
			int nLenNum = 0;
			if(nLen%10 == 0)
			{
				nLenNum = nLen/10;
			}
			else
			{
				nLenNum = nLen/10 + 1;
			}
			for (int j=0;j<nLenNum;j++)
			{
				for (int i=0;i<10;i++)
				{
					memcpy(strByte,(szRecValue+7)+i*113+j*1138,113);
					if (strByte[12] != 0x00)
					{
						m_Rdlg->ReceiveRecordDataMessage((BYTE *)strByte);
					}	
				}
			}
			//}
		}
			
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void CNewAsyncSocket::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (0 == nErrorCode)
	{
		BYTE pByte[100];
		m_dlg->SendDataMessage(pByte);
		this->Send(pByte,7);
	}
	CAsyncSocket::OnSend(nErrorCode);
}

void CNewAsyncSocket::GetDlg(CHeatRunDlg *dlg)
{
	m_dlg=dlg;
}

void CNewAsyncSocket::GetRDlg(CRecordDlg *rDlg)
{
	m_Rdlg=rDlg;
}

void CNewAsyncSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//m_dlg->OnConnect();
	CAsyncSocket::OnConnect(nErrorCode);
}
