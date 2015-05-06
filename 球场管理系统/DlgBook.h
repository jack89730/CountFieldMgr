#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxdtctl.h"

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
	CString m_strVipID;
	CString m_strXML;
	afx_msg void OnBnClickedOk();
	CString m_strFieldID;
	CString m_strFieldName;
	BOOL m_bSuccess;
	CComboBox m_comboStart;
	CComboBox m_comboStop;
	CArray<CString> m_ayStartTimeStr;
	CArray<CString> m_ayStopTimeStr;
	CArray<int> m_ayIndex;
	int m_indexComboInit;
	afx_msg void OnBnClickedButton1();
	CComboBox m_comboVip;
	void ShowAllVip();
//	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int m_nYajin;
	CString m_strTmpName;
	CString m_strTmpPhone;
	afx_msg void OnCbnSelchangeCombo1();
	COleDateTime m_periodStop;
	CString m_strPrice;
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	CComboBox m_comboPeriod;
	afx_msg void OnCbnSelchangeCombo3();
	CDateTimeCtrl m_DateTimePicker;
};
