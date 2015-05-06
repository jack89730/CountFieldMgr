#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "leveldata.h"
#include "perioddata.h"
#include "afxwin.h"

//和类别的combox下标一致
#define SELECT_TYPE_ALL 0
#define SELECT_TYPE_FIELD 1
#define SELECT_TYPE_FOOD 2
// CDlgMgr 对话框

class CDlgMgr : public CDialog
{
	DECLARE_DYNAMIC(CDlgMgr)

public:
	CDlgMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMgr();

// 对话框数据
	enum { IDD = IDD_DLG_MGR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
public:
	void resize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT		m_ptOld;
	void ShowPriceList();
	CListCtrl m_listVipPrice;
	int m_nCurSelectLevelIndex;
	PRICE_LIST m_ayPrice;
	CLevelData* m_pLevelData;
	CPeriodData* m_pPeriodData;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	OnePriceInfo* GetPriceInfo(CString strLevel);
	int ChangePriceInfo(OnePriceInfo& info);
	CComboBox m_comboStart;
	CComboBox m_comboStop;
	CButton m_checkWeekend;
	CButton m_checkDisableEdit;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck1();
	PeriodInfo m_periodInfo;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
	CStatic m_timemgr;
	int m_nInitFieldCount;
	afx_msg void OnBnClickedBtnInitfield();
	COleDateTime m_dateStart;
	COleDateTime m_dateStop;
	CComboBox m_comboNameByType;
	CComboBox m_comboVip;
	CListCtrl m_listReport;
	afx_msg void OnBnClickedBtnFoodMgr();
	CComboBox m_combotest;
	afx_msg void OnBnClickedButton3();
	void Hook(int nType, void* pData);
	void ShowFieldAndFoodCombo();
	void UpdateVipLevelCombo();
	afx_msg void OnBnClickedBtnFieldMgr();
	afx_msg void OnBnClickedBtnVipMgr();
	BOOL IsGoldTime(int index);
	CString m_strPasswordFile;
	BOOL CheckPassword();
	afx_msg void OnBnClickedButton5();

	CString GetPriceByLevelAndTime(CString strLevel, CString strDate, int nTimeIndex);

	afx_msg void OnBnClickedBtnUserMgr();
	void ShowGoldTime();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddVipLevel();
	afx_msg void OnDelVipLevel();
	CString GetLevelNameByID(CString strID);
	CComboBox m_ComboType;
	afx_msg void OnCbnSelchangeType();

	CArray<CString> m_aySelectID;

	int m_nSelectType;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnEditchangeNameByType();
	afx_msg void OnCbnSelchangeVip();
	afx_msg void OnBnClickedBtnAction();
};
