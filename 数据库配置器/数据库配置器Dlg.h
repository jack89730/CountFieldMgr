
// ���ݿ�������Dlg.h : ͷ�ļ�
//

#pragma once

#include "dbconfig.h"

// C���ݿ�������Dlg �Ի���
class C���ݿ�������Dlg : public CDialog
{
// ����
public:
	C���ݿ�������Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

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
	DECLARE_MESSAGE_MAP()

	CString m_strHost;
	int m_nPort;
	CString m_strUserName;
	CString m_strPwd;
	CString m_strDBName;
	CDBConfig* m_pConfig;
	afx_msg void OnBnClickedBtnNewDB();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedBtnOk();

};
