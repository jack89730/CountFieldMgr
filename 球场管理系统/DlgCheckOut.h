#pragma once
#include "resource.h"

// CDlgCheckOut �Ի���

class CDlgCheckOut : public CDialog
{
	DECLARE_DYNAMIC(CDlgCheckOut)

public:
	CDlgCheckOut(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCheckOut();

// �Ի�������
	enum { IDD = IDD_DLG_FOOD_MONEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTotalMoney;
	CString m_strPay;
	afx_msg void OnBnClickedOk();
};
