#pragma once

#include "resource.h"
// CDlgChangeFieldInfo �Ի���

class CDlgChangeFieldInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgChangeFieldInfo)

public:
	CDlgChangeFieldInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgChangeFieldInfo();

// �Ի�������
	enum { IDD = IDD_DLG_CHANGE_FIELD_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strID;
	CString m_strName;
	afx_msg void OnBnClickedOk();
};
