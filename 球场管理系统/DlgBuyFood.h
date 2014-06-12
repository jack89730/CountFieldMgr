#pragma once
#include "afxcmn.h"
#include "resource.h"

// CDlgBuyFood 对话框

class CDlgBuyFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgBuyFood)

public:
	CDlgBuyFood(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBuyFood();

// 对话框数据
	enum { IDD = IDD_DLG_BUYFOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listMenu;
	CListCtrl m_listPointMenu;
	int m_nCount;
	afx_msg void OnBnClickedBtnBuy();
};
