#pragma once

#include "resource.h"
// CDlgAddField �Ի���

class CDlgAddField : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddField)

public:
	CDlgAddField(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddField();

// �Ի�������
	enum { IDD = IDD_DLG_ADDFIELD };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CString m_strName;
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	void CreateFieldXML();
	CString m_strXML;
	afx_msg void OnBnClickedOk();
};
