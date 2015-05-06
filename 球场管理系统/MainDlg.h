
// 球场管理系统Dlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "DlgField.h"
#include "DlgFood.h"
#include "DlgVip.h"
#include "DlgMgr.h"

// CMainDlg 对话框
class CMainDlg : public CDialog
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DLG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;
	int m_CurSelTab; 
	CDlgField m_pageField; 
	CDlgFood m_pageFood; 
	CDlgVip m_pageVip; 
	CDlgMgr m_pageMgr;
	CDialog* m_pDialog[4];  //用来保存对话框对象指针
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
