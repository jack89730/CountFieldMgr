#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "public.h"

// CDlgVip 对话框

class CDlgVip : public CDialog
{
	DECLARE_DYNAMIC(CDlgVip)

public:
	CDlgVip(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgVip();

// 对话框数据
	enum { IDD = IDD_DLG_VIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listVip;
	afx_msg void OnBnClickedBtnAdd();
	void ShowItemVip();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnCharge();
	CArray<VipInfo> m_ayVip;
	VipInfo* GetVipInfo(CString strVipID);
	int ChangeVipInfo(VipInfo& info);
private:
	CString m_strXML;
	int m_nMaxID;
};
