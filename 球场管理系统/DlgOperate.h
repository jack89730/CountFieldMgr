#pragma once
#include "afxwin.h"
#include "resource.h"
#include "operatedata.h"
#include "userdata.h"

// CDlgOperate �Ի���

class CDlgOperate : public CDialog
{
	DECLARE_DYNAMIC(CDlgOperate)

public:
	CDlgOperate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgOperate();

// �Ի�������
	enum { IDD = IDD_DLG_OPERATE_HIS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	void ShowAllUser();
	void FindHistory();
	void Hook(HisOperateInfo* pData);
	COleDateTime m_StartTime;
	COleDateTime m_StopTime;
	CComboBox m_comboUsers;
	CListCtrl m_list;
	afx_msg void OnBnClickedButton1();
	USER_LIST m_ayUsers;
	//int m_nSelectIndex;
	CString m_strUserName;
	afx_msg void OnBnClickedBtnOutput();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnCbnKillfocusCombo1();
};
