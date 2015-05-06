#pragma once
#include "afxcmn.h"

#include "resource.h"
#include "userdata.h"
// CDlgUserMgr �Ի���

class CDlgUserMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserMgr)

public:
	CDlgUserMgr(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUserMgr();

// �Ի�������
	enum { IDD = IDD_DIALOG10 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
