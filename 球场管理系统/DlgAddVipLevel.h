#pragma once

#include "resource.h"
// CDlgAddVipLevel �Ի���

class CDlgAddVipLevel : public CDialog
{
	DECLARE_DYNAMIC(CDlgAddVipLevel)

public:
	CDlgAddVipLevel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAddVipLevel();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_VIP_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLow;
	CString m_strHigh;
	afx_msg void OnBnClickedOk();
	CString m_strLevelID;
	CString m_strLevelName;
};
