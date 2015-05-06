
// 数据库配置器Dlg.h : 头文件
//

#pragma once

#include "dbconfig.h"

// C数据库配置器Dlg 对话框
class C数据库配置器Dlg : public CDialog
{
// 构造
public:
	C数据库配置器Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

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
