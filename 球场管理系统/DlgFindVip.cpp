// DlgFindVip.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgFindVip.h"
#include "public.h"
#include "MainDlg.h"

// CDlgFindVip 对话框

IMPLEMENT_DYNAMIC(CDlgFindVip, CDialog)

CDlgFindVip::CDlgFindVip(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFindVip::IDD, pParent)
	, m_strVip(_T(""))
{

}

CDlgFindVip::~CDlgFindVip()
{
}

void CDlgFindVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listVip);
	DDX_Text(pDX, IDC_EDIT1, m_strVip);
}


BEGIN_MESSAGE_MAP(CDlgFindVip, CDialog)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgFindVip::OnEnChangeEdit1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgFindVip::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgFindVip 消息处理程序

void CDlgFindVip::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	m_listVip.DeleteAllItems();
	UpdateData(TRUE);

	if (m_strVip.IsEmpty())
	{
		ShowAllVip();
		return;
	}

	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++)
	{
		CString& strTmpID = ayVip[i].m_strID;
		CString& strTmpName = ayVip[i].m_strName;
		//名字或者id匹配都可以
		if (strTmpID.Find(m_strVip) != -1 || strTmpName.Find(m_strVip) != -1)
		{
			//找到了
			VipInfo* p = pMainWnd->m_pageVip.GetVipInfo(strTmpID);
			int j = 0;
			m_listVip.InsertItem(i, p->m_strID);
			m_listVip.SetItemText(row, ++j, p->m_strName);
			m_listVip.SetItemText(row, ++j, p->m_strSex);
			m_listVip.SetItemText(row, ++j, p->m_strPhone);
			m_listVip.SetItemText(row, ++j, p->m_strMoney);
			row++;
		}
	}

}
BOOL CDlgFindVip::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listVip.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listVip.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("姓名"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("性别"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("电话"),	LVCFMT_CENTER, 200);
	m_listVip.InsertColumn(k++, _T("余额"),	LVCFMT_CENTER, 100);
	ShowAllVip();
	return TRUE;
}

void CDlgFindVip::ShowAllVip()
{
	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++,row++)
	{
		CString& strTmpID = ayVip[i].m_strID;
		CString& strTmpName = ayVip[i].m_strName;
		VipInfo* p = pMainWnd->m_pageVip.GetVipInfo(strTmpID);
		int j = 0;
		m_listVip.InsertItem(i, p->m_strID);
		m_listVip.SetItemText(row, ++j, p->m_strName);
		m_listVip.SetItemText(row, ++j, p->m_strSex);
		m_listVip.SetItemText(row, ++j, p->m_strPhone);
		m_listVip.SetItemText(row, ++j, p->m_strMoney);
	}
}

void CDlgFindVip::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	m_strResult = m_listVip.GetItemText(pNotifier->iItem, 0);
	CDialog::OnOK();
}
