#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgCancelBook �Ի���

class CDlgCancelBook : public CDialog
{
	DECLARE_DYNAMIC(CDlgCancelBook)

public:
	CDlgCancelBook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCancelBook();

// �Ի�������
	enum { IDD = IDD_DLG_CANCEL_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nTimeIndex;//��1��ʼ
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
