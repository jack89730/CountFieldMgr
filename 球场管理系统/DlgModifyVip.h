#pragma once

#include "resource.h"

// CDlgModifyVip �Ի���

class CDlgModifyVip : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyVip)

public:
	CDlgModifyVip(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModifyVip();

// �Ի�������
	enum { IDD = IDD_DLG_MODIFY_VIP };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strXML;
	CString m_strID;
	CString m_strName;
	CString m_strSex;
	CString m_strPhone;
	CString m_strMoney;
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	afx_msg void OnBnClickedOk();
};
