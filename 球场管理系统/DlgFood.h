#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "public.h"
// CDlgFood �Ի���

class CDlgFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgFood)

public:
	CDlgFood(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFood();

// �Ի�������
	enum { IDD = IDD_DLG_FOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listFood;
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnModify();
	void ShowItemFood();
	afx_msg void OnBnClickedBtnBuy();

	CArray<FoodInfo> m_ayFood;
	FoodInfo* GetFoodInfo(CString strID);
	int ChangeFoodInfo(FoodInfo& info);
	CString m_strXML;
	int m_nMaxID;

	int m_nCount;
	CListCtrl m_listBuy;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};
