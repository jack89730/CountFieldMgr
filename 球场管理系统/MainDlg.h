
// �򳡹���ϵͳDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "DlgField.h"
#include "DlgFood.h"
#include "DlgVip.h"
#include "DlgMgr.h"

// CMainDlg �Ի���
class CMainDlg : public CDialog
{
// ����
public:
	CMainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DLG_MAIN };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	CDialog* m_pDialog[4];  //��������Ի������ָ��
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
};
