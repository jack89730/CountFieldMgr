#pragma once

#include "resource.h"
#include "afxwin.h"

// CDlgBook 对话框

class CDlgBook : public CDialog
{
	DECLARE_DYNAMIC(CDlgBook)

public:
	CDlgBook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBook();

// 对话框数据
	enum { IDD = IDD_DLG_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nTimeIndex;//从1开始
	CString m_strBookTime;
	CString m_strVipID;
	CString m_strXML;
	afx_msg void OnBnClickedOk();
	CString m_strFieldID;
	BOOL m_bSuccess;
	CComboBox m_comboStart;
	CComboBox m_comboStop;
	CArray<CString> m_ayStartTimeStr;
	CArray<CString> m_ayStopTimeStr;
	CArray<int> m_ayIndex;
	int m_indexComboInit;
};
