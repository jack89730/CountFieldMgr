// DlgChangePwd.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgChangePwd.h"
#include "MainDlg.h"
// CDlgChangePwd 对话框

IMPLEMENT_DYNAMIC(CDlgChangePwd, CDialog)

CDlgChangePwd::CDlgChangePwd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangePwd::IDD, pParent)
	, m_strOldPwd(_T(""))
	, m_strNewPwd1(_T(""))
	, m_strNewPwd2(_T(""))
{

}

CDlgChangePwd::~CDlgChangePwd()
{
}

void CDlgChangePwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strOldPwd);
	DDX_Text(pDX, IDC_EDIT2, m_strNewPwd1);
	DDX_Text(pDX, IDC_EDIT3, m_strNewPwd2);
}


BEGIN_MESSAGE_MAP(CDlgChangePwd, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgChangePwd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChangePwd 消息处理程序

void CDlgChangePwd::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainDlg* pMain = GETMAINWND;
	UpdateData(TRUE);

	CString strPassword = pMain->m_pageMgr.ReadPassword();
	if (m_strOldPwd != strPassword)
	{
		MessageBox(_T("密码不正确！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strNewPwd1.IsEmpty() || m_strNewPwd2.IsEmpty())
	{
		MessageBox(_T("密码为空！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strNewPwd1.GetLength() > 50 || m_strNewPwd1.GetLength() < 6)
	{

		MessageBox(_T("密码长度应为6-50个字符！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strNewPwd1 != m_strNewPwd2)
	{
		MessageBox(_T("两次输入不一致！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	pMain->m_pageMgr.SetPassword(m_strNewPwd1);
	OnOK();
}
