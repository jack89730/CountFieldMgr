#pragma once

#include "resource.h"

// CDlgModifyFood �Ի���

class CDlgModifyFood : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyFood)

public:
	CDlgModifyFood(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgModifyFood();

// �Ի�������
	enum { IDD = IDD_DLG_MODIFY_FOOD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldRemain;
	CString m_strOldPrice;
	CString m_strOldName;


	CString m_strRemain;
	CString m_strName;
	CString m_strID;
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	afx_msg void OnBnClickedOk();

	CString m_strPrice;
};
