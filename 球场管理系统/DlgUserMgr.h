#pragma once
#include "afxcmn.h"

#include "resource.h"
#include "userdata.h"
// CDlgUserMgr 对话框

class CDlgUserMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserMgr)

public:
	CDlgUserMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserMgr();

// 对话框数据
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listUser;
	void ShowAllUser(CString strFilter="");
	USER_LIST m_ayUserList;
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nCurSelectIndex;
	afx_msg void OnUserAdd();
	afx_msg void OnUserPwd();
	afx_msg void OnUseDel();
	CUserData* m_pUserData;
};
