#pragma once
#include "resource.h"

// CDlgAddUser �Ի���

class CDlgAddUser : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddUser)

public:
	CDlgAddUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddUser();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	CString m_strPwd1;
	CString m_strPwd2;
	afx_msg void OnBnClickedOk();
};
