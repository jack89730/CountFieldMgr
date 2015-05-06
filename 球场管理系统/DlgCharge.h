#pragma once
#include "resource.h"

// CDlgCharge 对话框

class CDlgCharge : public CDialog
{
	DECLARE_DYNAMIC(CDlgCharge)

public:
	CDlgCharge(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCharge();

// 对话框数据
	enum { IDD = IDD_DLG_CHARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strID;
	int m_nMoney;//当前余额，充值前
	int m_nCharge;//下方充值金额
	int m_nAfterCharge;
	int m_nTmpMoney;//上方充值金额
	afx_msg void OnBnClickedOk2();
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
