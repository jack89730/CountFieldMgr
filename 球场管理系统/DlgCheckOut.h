#pragma once
#include "resource.h"
#include "afxwin.h"
#include "vipdata.h"

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
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTotalMoney;
	CString m_strPay;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CButton m_radiovip;
	CButton m_radiocash;
	afx_msg void OnBnClickedBtnFind();
	CString m_strVip;
	CComboBox m_comboVip;
	CArray<VipInfo> m_ayVip;
	void ShowAllVip();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelendcancelCombo1();
	afx_msg void OnCbnCloseupCombo1();
	afx_msg void OnCbnEditupdateCombo1();
//	afx_msg void OnCbnSelendokCombo1();
};
