#pragma once

#include "resource.h"
// CDlgAddFood 对话框

class CDlgAddFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddFood)

public:
	CDlgAddFood(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddFood();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_FOOD };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strPrice;
	afx_msg void OnBnClickedOk();
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	afx_msg void OnBnClickedCommand1();
	CString m_strID;
	CString m_strXML;
	void CreateFoodXML();

};
