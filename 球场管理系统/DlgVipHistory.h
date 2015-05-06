#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgVipHistory �Ի���

class CDlgVipHistory : public CDialog
{
	DECLARE_DYNAMIC(CDlgVipHistory)

public:
	CDlgVipHistory(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgVipHistory();

// �Ի�������
	enum { IDD = IDD_DLG_VIP_HISTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strVipID;
	COleDateTime m_timeStart;
	COleDateTime m_timeStop;
	afx_msg void OnBnClickedButton1();
	void FindHistory();
	CListCtrl m_list;
	void Hook(int nType, void* pData);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nTotalMoney;
	CComboBox m_comboVip;
	void ShowAllVip();
	afx_msg void OnCbnEditchangeCombo1();
	afx_msg void OnBnClickedBtnOutput();
	afx_msg void OnCbnSelchangeCombo1();
};
