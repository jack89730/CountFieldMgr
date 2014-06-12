#pragma once

#include "resource.h"
#include "afxwin.h"

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
