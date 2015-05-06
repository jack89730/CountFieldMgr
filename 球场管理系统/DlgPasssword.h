#pragma once
#include "resource.h"

// CDlgPasssword 对话框

class CDlgPasssword : public CDialog
{
	DECLARE_DYNAMIC(CDlgPasssword)

public:
	CDlgPasssword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPasssword();

// 对话框数据
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassword;
	afx_msg void OnBnClickedOk();
};
