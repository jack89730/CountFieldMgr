// DlgBookKey.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgBookKey.h"


// CDlgBookKey 对话框

IMPLEMENT_DYNAMIC(CDlgBookKey, CDialog)

CDlgBookKey::CDlgBookKey(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBookKey::IDD, pParent)
	, m_strKey(_T(""))
{

}

CDlgBookKey::~CDlgBookKey()
{
}

void CDlgBookKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strKey);
}


BEGIN_MESSAGE_MAP(CDlgBookKey, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBookKey::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBookKey 消息处理程序

void CDlgBookKey::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strKey.IsEmpty())
	{
		MessageBox("柜子号不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	OnOK();
}
