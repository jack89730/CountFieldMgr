#pragma once
#include "resource.h"

// CDlgCheckOut 对话框

class CDlgCheckOut : public CDialog
{
	DECLARE_DYNAMIC(CDlgCheckOut)

public:
	CDlgCheckOut(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCheckOut();

// 对话框数据
	enum { IDD = IDD_DLG_FOOD_MONEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTotalMoney;
	CString m_strPay;
	afx_msg void OnBnClickedOk();
};
