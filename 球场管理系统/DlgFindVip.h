#pragma once
#include "afxcmn.h"
#include "resource.h"

// CDlgFindVip �Ի���

class CDlgFindVip : public CDialog
{
	DECLARE_DYNAMIC(CDlgFindVip)

public:
	CDlgFindVip(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgFindVip();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listVip;
	CString m_strVip;
	afx_msg void OnEnChangeEdit1();
	CString m_strResult;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowAllVip();
};
