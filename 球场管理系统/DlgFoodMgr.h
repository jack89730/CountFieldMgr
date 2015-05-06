#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "public.h"
#include "fooddata.h"

// CDlgFoodMgr �Ի���

class CDlgFoodMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgFoodMgr)

public:
	CDlgFoodMgr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFoodMgr();

// �Ի�������
	enum { IDD = IDD_DLG_FOOD_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nMaxID;
	CArray<FoodInfo> m_ayFood;
	CListCtrl m_list;
	void ShowItemFood();
	CFoodData* m_pFoodData;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nCurSelectIndex;
	afx_msg void On32784();
};
