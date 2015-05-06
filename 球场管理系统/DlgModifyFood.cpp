// DlgModifyFood.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgModifyFood.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"
#include "operatedata.h"
#include "app.h"

// CDlgModifyFood 对话框

IMPLEMENT_DYNAMIC(CDlgModifyFood, CDialog)

CDlgModifyFood::CDlgModifyFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyFood::IDD, pParent)
	, m_strName(_T(""))
	, m_strPrice(_T(""))
	,m_strRemain(_T(""))
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
	DDX_Text(pDX, IDC_EDIT3, m_strRemain);
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

	m_strOldRemain = m_strRemain;
	m_strOldPrice = m_strPrice;
	m_strOldName = m_strName;

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

	if(atoi(m_strRemain.GetBuffer()) < 0)
	{
		MessageBox("库存不能是负数!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (m_strOldRemain == m_strRemain &&
	m_strOldPrice == m_strPrice &&
	m_strOldName == m_strName)
	{
		return;
	}
	FoodInfo info;
	info.m_strID = m_strID;
	info.m_strName = m_strName;
	info.m_strPrice = m_strPrice;
	info.m_strRemain = m_strRemain;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	pMainWnd->m_pageFood.ChangeFoodInfo(info);
	pMainWnd->m_pageFood.ShowItemFood();
	CDialog::OnOK();

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CHANGE_FOOD_INFO;
	his_op_info.m_strInfo.Format("ID:%s,名称:%s,库存:%s,价格:%s 改为 名称:%s,库存:%s,价格:%s"
		, m_strID.GetBuffer()
		, m_strOldName.GetBuffer()
		, m_strOldRemain.GetBuffer()
		, m_strOldPrice.GetBuffer()
		, m_strName.GetBuffer()
		, m_strRemain.GetBuffer()
		, m_strPrice.GetBuffer());

	pOperate->Add(his_op_info);

}
