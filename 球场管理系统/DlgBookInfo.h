#pragma once

#include "resource.h"
// CDlgBookInfo 对话框

class CDlgBookInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgBookInfo)

public:
	CDlgBookInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBookInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTitle;
	CString m_strVipID;
	CString m_strName;
	CString m_strSex;
	CString m_strPhone;
};
