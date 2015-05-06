// DlgInputFood.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgInputFood.h"


// CDlgInputFood 对话框

IMPLEMENT_DYNAMIC(CDlgInputFood, CDialog)

CDlgInputFood::CDlgInputFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputFood::IDD, pParent)
	, m_strName(_T(""))
	, m_nCount(0)
{

}

CDlgInputFood::~CDlgInputFood()
{
}

void CDlgInputFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_nCount);
}


BEGIN_MESSAGE_MAP(CDlgInputFood, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInputFood::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputFood 消息处理程序

void CDlgInputFood::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_nCount <= 0)
	{
		MessageBox("数量要大于0！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
