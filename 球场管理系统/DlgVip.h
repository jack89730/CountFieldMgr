#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "public.h"
#include "vipdata.h"

// CDlgVip �Ի���

class CDlgVip : public CDialog
{
	DECLARE_DYNAMIC(CDlgVip)

public:
	CDlgVip(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVip();

// �Ի�������
	enum { IDD = IDD_DLG_VIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	BOOL CheckPwd();
	CListCtrl m_listVip;
	afx_msg void OnBnClickedBtnAdd();
	void ShowItemVip();
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnBnClickedBtnModify();
	afx_msg void OnBnClickedBtnCharge();
	CArray<VipInfo> m_ayVip;
	CArray<VipInfo> m_ayDelVip;
	VipInfo* GetVipInfo(CString strVipID);
	VipInfo* GetDelVipInfo(CString strVipID);
	int ChangeVipInfo(VipInfo& info);
	CVipData* m_pVipData;
	int NewID();
private:
	int m_nMaxID;
	CString m_strCurSelectVip;
	
public:
	afx_msg void OnNMDblclkListVip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListVip(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32779();
	afx_msg void On32781();
	afx_msg void On32780();
	afx_msg void On32783();
	afx_msg void On32782();
	void resize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT		m_ptOld;
	afx_msg void On32785();
	CString m_strFilter;
	afx_msg void OnEnChangeEdit2();
};
