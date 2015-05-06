#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgInit 对话框

class CDlgInit : public CDialog
{
	DECLARE_DYNAMIC(CDlgInit)

public:
	CDlgInit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgInit();

// 对话框数据
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboStart;
	CComboBox m_comboStop;
	int m_nFieldCount;
	afx_msg void OnBnClickedOk();
};
