#pragma once
#include "resource.h"

// CDlgPasssword �Ի���

class CDlgPasssword : public CDialog
{
	DECLARE_DYNAMIC(CDlgPasssword)

public:
	CDlgPasssword(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPasssword();

// �Ի�������
	enum { IDD = IDD_DIALOG8 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strPassword;
	afx_msg void OnBnClickedOk();
};
