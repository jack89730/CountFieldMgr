// DlgNewPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgNewPassword.h"

#include<iomanip>
#include<fstream>
#include "public.h"
using namespace std;

// CDlgNewPassword 对话框

IMPLEMENT_DYNAMIC(CDlgNewPassword, CDialog)

CDlgNewPassword::CDlgNewPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewPassword::IDD, pParent)
	, m_strPassword1(_T(""))
	, m_strPassword2(_T(""))
{

}

CDlgNewPassword::~CDlgNewPassword()
{
}

void CDlgNewPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPassword1);
	DDX_Text(pDX, IDC_EDIT2, m_strPassword2);
}


BEGIN_MESSAGE_MAP(CDlgNewPassword, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNewPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewPassword 消息处理程序

void CDlgNewPassword::OnBnClickedOk()
{
	UpdateData(TRUE);

	// TODO: 在此添加控件通知处理程序代码
	if (m_strPassword1.IsEmpty())
	{
		MessageBox(_T("请输入密码！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (m_strPassword1 != m_strPassword2)
	{
		MessageBox(_T("两次密码不一致！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	OnOK();
}
