// DlgModifyVip.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgModifyVip.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"
// CDlgModifyVip 对话框

IMPLEMENT_DYNAMIC(CDlgModifyVip, CDialog)

CDlgModifyVip::CDlgModifyVip(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyVip::IDD, pParent)
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strPhone(_T(""))
	, m_strMoney(_T(""))
{

}

CDlgModifyVip::~CDlgModifyVip()
{
}

void CDlgModifyVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_strSex);
	DDX_Text(pDX, IDC_EDIT3, m_strPhone);
	DDX_Text(pDX, IDC_EDIT4, m_strMoney);
}


BEGIN_MESSAGE_MAP(CDlgModifyVip, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModifyVip::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModifyVip 消息处理程序
BOOL CDlgModifyVip::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	UpdateData(FALSE);
	return TRUE;
}

void CDlgModifyVip::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	if(m_strName.IsEmpty())
	{
		MessageBox("姓名不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strSex.IsEmpty())
	{
		MessageBox("性别不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strPhone.IsEmpty())
	{
		MessageBox("电话不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strMoney.GetBuffer()) < 0)
	{
		MessageBox("余额不能是负数!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	VipInfo info;
	info.m_strID = m_strID;
	info.m_strMoney = m_strMoney;
	info.m_strName = m_strName;
	info.m_strPhone = m_strPhone;
	info.m_strSex = m_strSex;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	pMainWnd->m_pageVip.ChangeVipInfo(info);
	CDialog::OnOK();
}
