#pragma once
#include "afxwin.h"
#include "resource.h"

// CDlgInit �Ի���

class CDlgInit : public CDialog
{
	DECLARE_DYNAMIC(CDlgInit)

public:
	CDlgInit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInit();

// �Ի�������
	enum { IDD = IDD_DIALOG12 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboStart;
	CComboBox m_comboStop;
	int m_nFieldCount;
	afx_msg void OnBnClickedOk();
};
