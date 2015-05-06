#pragma once
#include "resource.h"
#include "afxwin.h"
#include "leveldata.h"
// CDlgAddVip 对话框

class CDlgAddVip : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddVip)

public:
	CDlgAddVip(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddVip();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_VIP };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strSex;
	CString m_strPhone;
	CString m_strMoney;
	afx_msg void OnBnClickedOk();
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	CString m_strID;
	void CreateVipXML();
	CComboBox m_comboLevel;
	CComboBox m_comboSex;
	PRICE_LIST m_ayPriceList;
};
