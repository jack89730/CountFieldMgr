#pragma once

#include "resource.h"
// CDlgChangeUserPwd 对话框

class CDlgChangeUserPwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeUserPwd)

public:
	CDlgChangeUserPwd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChangeUserPwd();

// 对话框数据
	enum { IDD = IDD_DIALOG11 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
