#pragma once

#include "resource.h"
// CDlgBookInfo �Ի���

class CDlgBookInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgBookInfo)

public:
	CDlgBookInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBookInfo();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTitle;
	CString m_strVipID;
	CString m_strName;
	CString m_strSex;
	CString m_strPhone;
};
