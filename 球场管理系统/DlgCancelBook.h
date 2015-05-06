#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgCancelBook 对话框

class CDlgCancelBook : public CDialog
{
	DECLARE_DYNAMIC(CDlgCancelBook)

public:
	CDlgCancelBook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCancelBook();

// 对话框数据
	enum { IDD = IDD_DLG_CANCEL_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nTimeIndex;//从1开始
	CComboBox m_comboStart;
	CComboBox m_comboStop;
	CArray<CString> m_ayStartTimeStr;
	CArray<CString> m_ayStopTimeStr;
	CArray<int> m_ayIndex;
	CString m_strVipID;
	CString m_strXML;
	CString m_strFieldID;
	CString m_strFieldName;
	BOOL m_bSuccess;
	afx_msg void OnBnClickedOk();
};
