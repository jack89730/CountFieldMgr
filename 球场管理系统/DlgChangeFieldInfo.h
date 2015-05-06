#pragma once

#include "resource.h"
// CDlgChangeFieldInfo 对话框

class CDlgChangeFieldInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeFieldInfo)

public:
	CDlgChangeFieldInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgChangeFieldInfo();

// 对话框数据
	enum { IDD = IDD_DLG_CHANGE_FIELD_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strID;
	CString m_strName;
	afx_msg void OnBnClickedOk();
};
