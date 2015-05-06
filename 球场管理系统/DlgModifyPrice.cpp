// DlgModifyPrice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgModifyPrice.h"
#include "MainDlg.h"


// CDlgModifyPrice �Ի���

IMPLEMENT_DYNAMIC(CDlgModifyPrice, CDialog)

CDlgModifyPrice::CDlgModifyPrice(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyPrice::IDD, pParent)
	, m_strLevelName(_T(""))
	, m_strLowPrice(_T(""))
	, m_strHighPrice(_T(""))
{

}

CDlgModifyPrice::~CDlgModifyPrice()
{
}

void CDlgModifyPrice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strLevelName);
	DDX_Text(pDX, IDC_EDIT2, m_strLowPrice);
	DDX_Text(pDX, IDC_EDIT3, m_strHighPrice);
}


BEGIN_MESSAGE_MAP(CDlgModifyPrice, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgModifyPrice::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModifyPrice ��Ϣ�������

BOOL CDlgModifyPrice::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (m_strID == "0")
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	}
	return TRUE;
}

void CDlgModifyPrice::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CMainDlg* pMainWnd = GETMAINWND;
	OnePriceInfo newInfo;
	newInfo.m_strID = m_strID;
	newInfo.m_strName = m_strLevelName;
	newInfo.m_strPriceHigh = m_strHighPrice;
	newInfo.m_strPriceLow = m_strLowPrice;
	pMainWnd->m_pageMgr.ChangePriceInfo(newInfo);
	OnOK();
}
