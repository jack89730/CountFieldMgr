#pragma once

#include "resource.h"
// CDlgAddField 对话框

class CDlgAddField : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddField)

public:
	CDlgAddField(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddField();

// 对话框数据
	enum { IDD = IDD_DLG_ADDFIELD };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strName;
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	void CreateFieldXML();
	CString m_strXML;
	afx_msg void OnBnClickedOk();
};
