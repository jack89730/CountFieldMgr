#pragma once

#include "resource.h"

// CDlgModifyFood 对话框

class CDlgModifyFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyFood)

public:
	CDlgModifyFood(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgModifyFood();

// 对话框数据
	enum { IDD = IDD_DLG_MODIFY_FOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	int m_nPrice;
	CString m_strID;
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	afx_msg void OnBnClickedOk();

	CString m_strPrice;
};
