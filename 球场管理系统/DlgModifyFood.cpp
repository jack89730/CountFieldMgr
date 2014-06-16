// DlgModifyFood.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgModifyFood.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"

// CDlgModifyFood 对话框

IMPLEMENT_DYNAMIC(CDlgModifyFood, CDialog)

CDlgModifyFood::CDlgModifyFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyFood::IDD, pParent)
	, m_strName(_T(""))
	, m_strPrice(_T(""))
{

}

CDlgModifyFood::~CDlgModifyFood()
{
}

void CDlgModifyFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_strPrice);
}


BEGIN_MESSAGE_MAP(CDlgModifyFood, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModifyFood::OnBnClickedOk)
END_MESSAGE_MAP()

// CDlgModifyFood 消息处理程序
BOOL CDlgModifyFood::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	UpdateData(FALSE);
	return TRUE;
}

void CDlgModifyFood::OnBnClickedOk()
{
	UpdateData(TRUE);

	if(m_strName.IsEmpty())
	{
		MessageBox("姓名不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strPrice.GetBuffer()) < 0)
	{
		MessageBox("价格不能是负数!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	FoodInfo info;
	info.m_strID = m_strID;
	info.m_strName = m_strName;
	info.m_strPrice = m_strPrice;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	pMainWnd->m_pageFood.ChangeFoodInfo(info);
	pMainWnd->m_pageFood.ShowItemFood();
	CDialog::OnOK();
}
