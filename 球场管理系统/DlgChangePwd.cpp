// DlgChangePwd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgChangePwd.h"
#include "MainDlg.h"
// CDlgChangePwd �Ի���

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


// CDlgChangePwd ��Ϣ�������

void CDlgChangePwd::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMainDlg* pMain = GETMAINWND;
	UpdateData(TRUE);

	CString strPassword = pMain->m_pageMgr.ReadPassword();
	if (m_strOldPwd != strPassword)
	{
		MessageBox(_T("���벻��ȷ��"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strNewPwd1.IsEmpty() || m_strNewPwd2.IsEmpty())
	{
		MessageBox(_T("����Ϊ�գ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strNewPwd1.GetLength() > 50 || m_strNewPwd1.GetLength() < 6)
	{

		MessageBox(_T("���볤��ӦΪ6-50���ַ���"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strNewPwd1 != m_strNewPwd2)
	{
		MessageBox(_T("�������벻һ�£�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	pMain->m_pageMgr.SetPassword(m_strNewPwd1);
	OnOK();
}
