#pragma once

#include "resource.h"
// CDlgNewPassword �Ի���

class CDlgNewPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewPassword)

public:
	CDlgNewPassword(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewPassword();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassword1;
	CString m_strPassword2;
	afx_msg void OnBnClickedOk();
};
