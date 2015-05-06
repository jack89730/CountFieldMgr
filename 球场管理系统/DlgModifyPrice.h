#pragma once
#include "resource.h"

// CDlgModifyPrice 对话框

class CDlgModifyPrice : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyPrice)

public:
	CDlgModifyPrice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModifyPrice();

// 对话框数据
	enum { IDD = IDD_DLG_MODIFY_PRICE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLevelName;
	CString m_strID;
	CString m_strLowPrice;
	CString m_strHighPrice;
	afx_msg void OnBnClickedOk();
};
