#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxdtctl.h"

// CDlgBook �Ի���

class CDlgBook : public CDialog
{
	DECLARE_DYNAMIC(CDlgBook)

public:
	CDlgBook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBook();

// �Ի�������
	enum { IDD = IDD_DLG_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nTimeIndex;//��1��ʼ
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
