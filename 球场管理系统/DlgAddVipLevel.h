#pragma once

#include "resource.h"
// CDlgAddVipLevel 对话框

class CDlgAddVipLevel : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddVipLevel)

public:
	CDlgAddVipLevel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddVipLevel();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_VIP_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLow;
	CString m_strHigh;
	afx_msg void OnBnClickedOk();
	CString m_strLevelID;
	CString m_strLevelName;
};
