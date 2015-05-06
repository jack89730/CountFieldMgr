// DlgFoodMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgFoodMgr.h"
#include "MainDlg.h"
#include "fooddata.h"
#include "DlgAddFood.h"
#include "DlgModifyFood.h"
#include "DlgInputFood.h"
#include "operatedata.h"
#include "app.h"

// CDlgFoodMgr 对话框

IMPLEMENT_DYNAMIC(CDlgFoodMgr, CDialog)

CDlgFoodMgr::CDlgFoodMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFoodMgr::IDD, pParent)
{

}

CDlgFoodMgr::~CDlgFoodMgr()
{
}

void CDlgFoodMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CDlgFoodMgr, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgFoodMgr::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgFoodMgr::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CDlgFoodMgr::OnBnClickedButtonModify)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgFoodMgr::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgFoodMgr::OnNMRClickList1)
	ON_COMMAND(ID_32784, &CDlgFoodMgr::On32784)
END_MESSAGE_MAP()


// CDlgFoodMgr 消息处理程序

BOOL CDlgFoodMgr::OnInitDialog()
{
	m_nMaxID = 0;
	m_nCurSelectIndex = 0;
	m_pFoodData = GetFoodData;

	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	int k=0;
	m_list.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 120);
	m_list.InsertColumn(k++, _T("名称"),	LVCFMT_CENTER, 200);
	m_list.InsertColumn(k++, _T("价格"),	LVCFMT_CENTER, 80);
	m_list.InsertColumn(k++, _T("库存"),	LVCFMT_CENTER, 80);
	ShowItemFood();
	return TRUE;
}

void CDlgFoodMgr::ShowItemFood()
{
	//内存只保留当天数据
	m_list.DeleteAllItems();
	m_ayFood.RemoveAll();

	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageFood.ShowItemFood();

	//读出所有数据
	m_pFoodData->GetAllData(m_ayFood);

	m_nMaxID = 0;

	int nFoodCount = m_ayFood.GetCount();
	for (int i = 0; i < nFoodCount; i++)
	{
		int nID = atoi(m_ayFood[i].m_strID);
		if (nID > m_nMaxID)
		{
			m_nMaxID = nID;
		}
		int j = 0;
		m_list.InsertItem(i, m_ayFood[i].m_strID);
		m_list.SetItemText(i, ++j, m_ayFood[i].m_strName);
		m_list.SetItemText(i, ++j, m_ayFood[i].m_strPrice);
		m_list.SetItemText(i, ++j, m_ayFood[i].m_strRemain);
	}

	UpdateData(FALSE);
}

void CDlgFoodMgr::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CDlgAddFood dlg;
	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
	if (dlg.IsSuccess())
	{
		ShowItemFood();
	}
}

void CDlgFoodMgr::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	POSITION	pos;
	int			iPos;
	pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_list.GetNextSelectedItem(pos);
	CString strID		= m_list.GetItemText(iPos, 0);

	CMainDlg* pMainWnd = GETMAINWND;
	int ret = pMainWnd->m_pageFood.m_pFoodData->DeleteFood(strID);

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_DEL_FOOD;
	CString strName = m_list.GetItemText(iPos, 1);
	his_op_info.m_strInfo.Format("删除商品:%s", strName.GetBuffer());
	pOperate->Add(his_op_info);

	ShowItemFood();
}

void CDlgFoodMgr::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	CDlgModifyFood dlg;
	POSITION	pos;
	int			iPos;
	pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_list.GetNextSelectedItem(pos);
	dlg.m_strID		= m_list.GetItemText(iPos, 0);
	dlg.m_strName		= m_list.GetItemText(iPos, 1);
	dlg.m_strPrice = m_list.GetItemText(iPos, 2);
	dlg.m_strRemain = m_list.GetItemText(iPos, 3);

	dlg.DoModal();

	ShowItemFood();
	UpdateData(FALSE);
}

void CDlgFoodMgr::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	CDlgModifyFood dlg;

	dlg.m_strID		= m_list.GetItemText(pNotifier->iItem, 0);
	dlg.m_strName		= m_list.GetItemText(pNotifier->iItem, 1);
	dlg.m_strPrice = m_list.GetItemText(pNotifier->iItem, 2);
	dlg.m_strRemain = m_list.GetItemText(pNotifier->iItem, 3);

	dlg.DoModal();
	ShowItemFood();
	UpdateData(FALSE);
}

void CDlgFoodMgr::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	m_nCurSelectIndex = pNotifier->iItem;
	if (pNotifier->iItem < 0)
	{
		return;
	}

	CMenu menu ,* pSubMenu; //定义下面要用到的cmenu对象

	menu.LoadMenu(IDR_MENU_INPUT_FOOD);

	//获取第一个弹出菜单，所以第一个菜单必须有子菜单
	pSubMenu = menu.GetSubMenu(0);
	CPoint oPoint; //定义一个用于确定光标位置的位置
	GetCursorPos( &oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标

	//在指定位置显示弹出菜单
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;

}

void CDlgFoodMgr::On32784()
{
	// TODO: 在此添加命令处理程序代码
	ASSERT(m_nCurSelectIndex >= 0 && m_nCurSelectIndex < m_ayFood.GetSize());

	CDlgInputFood dlg;
	dlg.m_strName = m_ayFood[m_nCurSelectIndex].m_strName;
	dlg.m_nCount = 0;
	if (dlg.DoModal() == IDOK)
	{
		CString strCount;
		strCount.Format("%d", atoi(m_ayFood[m_nCurSelectIndex].m_strRemain) + dlg.m_nCount);
		m_ayFood[m_nCurSelectIndex].m_strRemain = strCount;
		m_pFoodData->ChangeFoodInfo(m_ayFood[m_nCurSelectIndex]);
		ShowItemFood();

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_STOCK_FOOD;
		his_op_info.m_strInfo.Format("ID:%s,名称:%s,进货数量:%d"
			, m_ayFood[m_nCurSelectIndex].m_strID.GetBuffer()
			, m_ayFood[m_nCurSelectIndex].m_strName.GetBuffer()
			, dlg.m_nCount);

		pOperate->Add(his_op_info);

	}
}
