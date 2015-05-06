#pragma once
#include "resource.h"

// CDlgAddUser 对话框

class CDlgAddUser : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddUser)

public:
	CDlgAddUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddUser();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strUserName;
	CString m_strPwd1;
	CString m_strPwd2;
	afx_msg void OnBnClickedOk();
};
