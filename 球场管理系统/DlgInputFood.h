#pragma once
#include "resource.h"

// CDlgInputFood �Ի���

class CDlgInputFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgInputFood)

public:
	CDlgInputFood(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInputFood();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	int m_nCount;
	afx_msg void OnBnClickedOk();
};
