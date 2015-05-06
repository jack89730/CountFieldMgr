#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "fielddata.h"
#include "public.h"
#include "MainDlg.h"
// CDlgFieldMgr 对话框

class CDlgFieldMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgFieldMgr)

public:
	CDlgFieldMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFieldMgr();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_DLG_FIELD_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strXML;
	CFieldData* m_pFieldData;
	int m_nCurSelectIndex;
	void ShowItemField();
	CListCtrl m_listField;
	CString m_strDate;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
