#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CInsertEditDlg �Ի���

class CInsertEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInsertEditDlg)

public:
	CInsertEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInsertEditDlg();

// �Ի�������
	enum { IDD = IDD_INSERTEDIT_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	CString m_str;
	CString m_iORe;
	CString strLangName;
	CString m_StrRecv[100];
	CString m_StrRecvEdit;

	DECLARE_MESSAGE_MAP()
	
	CEdit m_TypeEdit;
	CEdit m_AutoPOffEdit;
	CEdit m_LampON;
	CEdit m_LampOFF;
	CEdit m_LampONMinute;
	CEdit m_LampOFFMinute;
	CIPAddressCtrl m_EditIP;
	CComboBox m_comboECO;
	CComboBox m_comboStandby;
	CComboBox m_comboSetup;
	CComboBox m_comboPattern0;
	CComboBox m_comboPattern1;
	CComboBox m_comboPattern2;
	CComboBox m_comboPattern3;
	CComboBox m_comboPattern4;
	CComboBox m_comboPattern5;
	CComboBox m_comboPattern6;
	CComboBox m_comboPattern7;
	CComboBox m_comboPattern8;
	CComboBox m_comboPattern9;
	CComboBox m_comboProgram;

	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeProgramCombo();
	
};
