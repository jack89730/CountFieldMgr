#pragma once
#include "resource.h"

// CDlgChangePwd �Ի���

class CDlgChangePwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangePwd)

public:
	CDlgChangePwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChangePwd();

// �Ի�������
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPwd;
	CString m_strNewPwd1;
	CString m_strNewPwd2;
	afx_msg void OnBnClickedOk();
};
