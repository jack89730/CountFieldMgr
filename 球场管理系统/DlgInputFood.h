#pragma once
#include "resource.h"

// CDlgInputFood 对话框

class CDlgInputFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputFood)

public:
	CDlgInputFood(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInputFood();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	int m_nCount;
	afx_msg void OnBnClickedOk();
};
