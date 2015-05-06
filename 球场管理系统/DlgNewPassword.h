#pragma once

#include "resource.h"
// CDlgNewPassword 对话框

class CDlgNewPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgNewPassword)

public:
	CDlgNewPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNewPassword();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassword1;
	CString m_strPassword2;
	afx_msg void OnBnClickedOk();
};
