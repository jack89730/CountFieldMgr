#pragma once
#include "afxcmn.h"
#include "resource.h"

// CDlgBuyFood �Ի���

class CDlgBuyFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgBuyFood)

public:
	CDlgBuyFood(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBuyFood();

// �Ի�������
	enum { IDD = IDD_DLG_BUYFOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listMenu;
	CListCtrl m_listPointMenu;
	int m_nCount;
	afx_msg void OnBnClickedBtnBuy();
};
