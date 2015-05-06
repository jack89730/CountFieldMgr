#pragma once
#include "resource.h"

// CDlgChangePwd 对话框

class CDlgChangePwd : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangePwd)

public:
	CDlgChangePwd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChangePwd();

// 对话框数据
	enum { IDD = IDD_DIALOG9 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPwd;
	CString m_strNewPwd1;
	CString m_strNewPwd2;
	afx_msg void OnBnClickedOk();
};
