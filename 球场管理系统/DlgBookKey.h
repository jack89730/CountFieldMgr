#pragma once
#include "resource.h"

// CDlgBookKey �Ի���

class CDlgBookKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgBookKey)

public:
	CDlgBookKey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBookKey();

// �Ի�������
	enum { IDD = IDD_DLG_ADD_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strKey;
	afx_msg void OnBnClickedOk();
};
