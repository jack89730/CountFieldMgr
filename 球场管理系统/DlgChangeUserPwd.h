#pragma once

#include "resource.h"
// CDlgChangeUserPwd �Ի���

class CDlgChangeUserPwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeUserPwd)

public:
	CDlgChangeUserPwd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChangeUserPwd();

// �Ի�������
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	CString m_strOldPwd;
	CString m_strPwd1;
	CString m_strPwd2;
	afx_msg void OnBnClickedOk();
	BOOL m_bAdmin;
};
