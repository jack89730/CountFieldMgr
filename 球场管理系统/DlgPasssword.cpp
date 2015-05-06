// DlgPasssword.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPasssword.h"


// CDlgPasssword 对话框

IMPLEMENT_DYNAMIC(CDlgPasssword, CDialog)

CDlgPasssword::CDlgPasssword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPasssword::IDD, pParent)
	, m_strPassword(_T(""))
{

}

CDlgPasssword::~CDlgPasssword()
{
}

void CDlgPasssword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgPasssword, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPasssword::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPasssword 消息处理程序

void CDlgPasssword::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}
