#pragma once
#include "resource.h"

// CDlgModifyPrice �Ի���

class CDlgModifyPrice : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyPrice)

public:
	CDlgModifyPrice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModifyPrice();

// �Ի�������
	enum { IDD = IDD_DLG_MODIFY_PRICE };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLevelName;
	CString m_strID;
	CString m_strLowPrice;
	CString m_strHighPrice;
	afx_msg void OnBnClickedOk();
};
