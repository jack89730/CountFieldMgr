// DlgAddVipLevel.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAddVipLevel.h"
#include "public.h"
#include "leveldata.h"

// CDlgAddVipLevel 对话框

IMPLEMENT_DYNAMIC(CDlgAddVipLevel, CDialog)

CDlgAddVipLevel::CDlgAddVipLevel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddVipLevel::IDD, pParent)
	, m_strLow(_T(""))
	, m_strHigh(_T(""))
	, m_strLevelID(_T(""))
	, m_strLevelName(_T(""))
{

}

CDlgAddVipLevel::~CDlgAddVipLevel()
{
}

void CDlgAddVipLevel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strLow);
	DDX_Text(pDX, IDC_EDIT2, m_strHigh);
	DDX_Text(pDX, IDC_EDIT3, m_strLevelID);
	DDX_Text(pDX, IDC_EDIT4, m_strLevelName);
}


BEGIN_MESSAGE_MAP(CDlgAddVipLevel, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddVipLevel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddVipLevel 消息处理程序

void CDlgAddVipLevel::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strHigh.IsEmpty() || m_strLow.IsEmpty() || m_strLevelName.IsEmpty() || m_strLevelID.IsEmpty())
	{
		MessageBox("信息不全！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (atoi(m_strLevelID) > 100)
	{
		MessageBox("等级数值不能大于100！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	CLevelData* pPrice =  GetLevelData;
	PRICE_LIST pList;
	pPrice->GetAllData(pList);
	int n = pList.GetSize();
	for (int i = 0; i < n; i++)
	{
		if (pList[i].m_strID == m_strLevelID)
		{
			MessageBox("等级值已经存在", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		if (pList[i].m_strName == m_strLevelName)
		{
			MessageBox("等级名称已经存在", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	OnePriceInfo price;
	price.m_strID = m_strLevelID;
	price.m_strName = m_strLevelName;
	price.m_strPriceHigh = m_strHigh;
	price.m_strPriceLow = m_strLow;
	pPrice->AddPrice(price);
	OnOK();
}
