// DlgCharge.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCharge.h"
#include "MainDlg.h"

// CDlgCharge 对话框

IMPLEMENT_DYNAMIC(CDlgCharge, CDialog)

CDlgCharge::CDlgCharge(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCharge::IDD, pParent)
	, m_strName(_T(""))
	, m_strID(_T(""))
	, m_nMoney(0)
	, m_nCharge(0)
	, m_nAfterCharge(0)
	, m_nTmpMoney(0)
{

}

CDlgCharge::~CDlgCharge()
{
}

void CDlgCharge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_strID);
	DDX_Text(pDX, IDC_EDIT1, m_nMoney);
	DDX_Text(pDX, IDC_EDIT2, m_nCharge);
	DDX_Text(pDX, IDC_EDIT3, m_nAfterCharge);
	DDX_Text(pDX, IDC_EDIT6, m_nTmpMoney);
}


BEGIN_MESSAGE_MAP(CDlgCharge, CDialog)
	ON_BN_CLICKED(IDOK2, &CDlgCharge::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK, &CDlgCharge::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCharge 消息处理程序

void CDlgCharge::OnBnClickedOk2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if(m_strID.IsEmpty())
	{
		MessageBox("姓名不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_nTmpMoney <= 0)
	{
		MessageBox("充值金额必须大于0！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}


	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	VipInfo* info = pMainWnd->m_pageVip.GetVipInfo(m_strID);
	if (!info)
	{
		MessageBox("会员ID不存在！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return;
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	//更新显示，计算充值后金额等
	m_strName = info->m_strName;
	m_nMoney = atoi(info->m_strMoney.GetBuffer());
	m_nAfterCharge = m_nTmpMoney + m_nMoney;
	m_nCharge = m_nTmpMoney;
	UpdateData(FALSE);

	m_bIsSuccess = TRUE;
}

BOOL CDlgCharge::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nMoney = 0;
	m_nAfterCharge = 0;
	m_strID;
	m_nCharge = 0;
	m_bIsSuccess = FALSE;
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	return TRUE;
}

void CDlgCharge::OnBnClickedOk()
{
	//要核对信息之后才能点击充值按钮
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	VipInfo* info = pMainWnd->m_pageVip.GetVipInfo(m_strID);
	if (!info)
	{
		MessageBox("会员ID不存在！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	info->m_strMoney.Format("%d", m_nAfterCharge);
	pMainWnd->m_pageVip.ChangeVipInfo(*info);
	OnOK();
}
