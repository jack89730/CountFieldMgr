// DlgVip.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVip.h"
#include "DlgAddVip.h"
#include "DlgModifyVip.h"
#include "DlgCharge.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"
#include "DlgVipHistory.h"
#include <iostream>
using namespace std;
#include "app.h"
#include "public.h"
#include "DlgPasssword.h"
#include "operatedata.h"

// CDlgVip 对话框

IMPLEMENT_DYNAMIC(CDlgVip, CDialog)

CDlgVip::CDlgVip(CWnd* pParent /*=NULL*/)
: CDialog(CDlgVip::IDD, pParent)
, m_strFilter(_T(""))
{

}

CDlgVip::~CDlgVip()
{
}

void CDlgVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIP, m_listVip);
	DDX_Text(pDX, IDC_EDIT2, m_strFilter);
}


BEGIN_MESSAGE_MAP(CDlgVip, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgVip::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgVip::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CDlgVip::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_CHARGE, &CDlgVip::OnBnClickedBtnCharge)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VIP, &CDlgVip::OnNMDblclkListVip)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_VIP, &CDlgVip::OnNMRClickListVip)
	ON_COMMAND(ID_32779, &CDlgVip::On32779)
	ON_COMMAND(ID_32781, &CDlgVip::On32781)
	ON_COMMAND(ID_32780, &CDlgVip::On32780)
	ON_COMMAND(ID_32783, &CDlgVip::On32783)
	ON_COMMAND(ID_32782, &CDlgVip::On32782)
	ON_WM_SIZE()
	ON_COMMAND(ID_32785, &CDlgVip::On32785)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgVip::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CDlgVip 消息处理程序

BOOL CDlgVip::OnInitDialog()
{
	CRect rect;    
	GetClientRect(&rect);     //取客户区大小  
	m_ptOld.x=rect.right-rect.left;
	m_ptOld.y=rect.bottom-rect.top;

	m_pVipData = GetVipData;

	m_nMaxID = 0;

	CDialog::OnInitDialog();

	m_listVip.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listVip.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("姓名"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("性别"),	LVCFMT_CENTER, 60);
	m_listVip.InsertColumn(k++, _T("电话"),	LVCFMT_CENTER, 150);
	if (ISADMIN)
	{
		m_listVip.InsertColumn(k++, _T("余额"),	LVCFMT_CENTER, 120);
	}
	m_listVip.InsertColumn(k++, _T("等级"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("冻结"),	LVCFMT_CENTER, 120);

	ShowItemVip();
	if (ISADMIN)
	{
		GetDlgItem(IDC_BTN_DEL)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->ShowWindow(FALSE);
	}

	return TRUE;
}

void CDlgVip::OnBnClickedBtnAdd()
{
	CDlgAddVip dlg;

	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.DoModal();

	if (dlg.IsSuccess())
	{
		ShowItemVip();
	}
	else
	{
		--m_nMaxID;
	}
}

void CDlgVip::ShowItemVip()
{
	UpdateData(TRUE);

	m_listVip.DeleteAllItems();
	m_ayVip.RemoveAll();
	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK != pMainWnd->m_pageVip.m_pVipData->GetAllData(m_ayVip, m_ayDelVip))
	{
		return;
	}

	m_nMaxID = 0;
	//int nVipCount = m_ayVip.GetSize();
	CArray<VipInfo> tmp_ay;
	tmp_ay.Copy(m_ayVip);
	for (int i = 0; i < m_ayVip.GetSize(); i++)
	{
		VipInfo& info = m_ayVip[i];
		int j = 0;
		int nID = atoi(info.m_strID.GetBuffer());
		if (nID > m_nMaxID)
		{
			m_nMaxID = nID;
		}
		//临时非会员不显示
		if (info.m_strLevelID == VIP_LEVEL_0)
		{
			m_ayVip.RemoveAt(i);
			i--;
			continue;
		}

		if (!m_strFilter.IsEmpty())
		{
			if (info.m_strID.Find(m_strFilter) == -1 && info.m_strName.Find(m_strFilter) == -1)
			{
				m_ayVip.RemoveAt(i);
				i--;
				continue;
			}
		}
		m_listVip.InsertItem(i, info.m_strID);
		m_listVip.SetItemText(i, ++j, info.m_strName);
		m_listVip.SetItemText(i, ++j, info.m_strSex);
		m_listVip.SetItemText(i, ++j, info.m_strPhone);
		if (ISADMIN)
		{
			m_listVip.SetItemText(i, ++j, info.m_strMoney);
		}
		
		
		CString strLevelName = pMainWnd->m_pageMgr.GetLevelNameByID(info.m_strLevelID);
		CString strTmp;
		strTmp.Format("%s-%s", info.m_strLevelID.GetBuffer(), strLevelName.GetBuffer());
		m_listVip.SetItemText(i, ++j, strTmp);
		if (info.m_bLock)
		{
			m_listVip.SetItemText(i, ++j, "是");
		}
		else
		{
			m_listVip.SetItemText(i, ++j, "否");
		}
	}

	if (m_listVip.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		m_listVip.SetSelectionMark(0);
	}

	UpdateData(FALSE);

	m_ayVip.Copy(tmp_ay);
}

BOOL CDlgVip::CheckPwd()
{
	CDlgPasssword dlg;
	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	USER_LIST user_list;
	CUserData* pUser = GetUserData;
	pUser->GetAllData(user_list);
	int nCount = user_list.GetSize();
	int i = 0;
	UserInfo info;
	for (; i < nCount; i++)
	{
		info = user_list[i];
		if (info.m_strName == GETUSERNAME)
		{
			if (info.m_strPwd != dlg.m_strPassword)
			{
				MessageBox("密码错误！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
				return FALSE;
			}
			else
			{
				break;
			}
		}
	}
	return TRUE;
}

void CDlgVip::OnBnClickedBtnDel()
{
	if (!CheckPwd())
	{
		return ;
	}
	// TODO: 在此添加控件通知处理程序代码
	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要删除的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	CString strID		= m_listVip.GetItemText(iPos, 0);
	CString strName = m_listVip.GetItemText(iPos, 1);

	if(MessageBox("该会员的预定信息将被清空，确定吗？","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
	{
		return;
	}


	CMainDlg* pMainWnd = GETMAINWND;

	
	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->DeleteVip(strID))
	{
		ShowItemVip();
		pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_DEL_VIP;
		his_op_info.m_strInfo.Format("ID:%s, 姓名:%s"
			, strID.GetBuffer()
			, strName.GetBuffer());

		pOperate->Add(his_op_info);

	}
}

void CDlgVip::OnBnClickedBtnModify()
{
	// TODO: Add your control notification handler code here
	CDlgModifyVip dlg;
	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	dlg.m_strID		= m_listVip.GetItemText(iPos, 0);;

	dlg.DoModal();

	ShowItemVip();
	UpdateData(FALSE);
}

void CDlgVip::OnBnClickedBtnCharge()
{
	CDlgCharge dlg;

	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos)
	{
		iPos		    	= m_listVip.GetNextSelectedItem(pos);
		dlg.m_strID		= m_listVip.GetItemText(iPos, 0);
	}

	dlg.DoModal();
	ShowItemVip();
	UpdateData(FALSE);
}


VipInfo* CDlgVip::GetDelVipInfo(CString strVipID)
{
	//找到vip后查看他的个人信息
	int nVipCount = m_ayDelVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (m_ayDelVip[j].m_strID == strVipID)
		{
			break;
		}
	}

	if (j < nVipCount)
	{
		return &m_ayDelVip[j];
	}
	return NULL;
}
VipInfo* CDlgVip::GetVipInfo(CString strVipID)

{
	//找到vip后查看他的个人信息
	int nVipCount = m_ayVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (m_ayVip[j].m_strID == strVipID)
		{
			break;
		}
	}

	if (j < nVipCount)
	{
		return &m_ayVip[j];
	}

	return NULL; 

}

int CDlgVip::ChangeVipInfo(VipInfo& info)
{
	//修改vip信息，并且更新xml文件
	int nVipCount = m_ayVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (m_ayVip[j].m_strID == info.m_strID)
		{
			//找到id
			break;
		}
	}

	if (j >= nVipCount)
	{
		return ERROR_UNKNOW_ERROR;
	}

	m_ayVip[j] = info;//改内存中的值

	//改文件
	CMainDlg* pMainWnd = GETMAINWND;
	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->ChangeVipInfo(info))
	{
		ShowItemVip();
	}
	
	return 0;
}

void CDlgVip::OnNMDblclkListVip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	if (pNotifier->iItem < 0)
	{
		return;
	}

	CDlgModifyVip dlg;
	dlg.m_strID		= m_listVip.GetItemText(pNotifier->iItem, 0);

	dlg.DoModal();

	ShowItemVip();
	UpdateData(FALSE);

}

void CDlgVip::OnNMRClickListVip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;
	if (pNotifier->iItem == -1)
	{
		return;
	}

	m_strCurSelectVip = m_listVip.GetItemText(pNotifier->iItem,0);

	const VipInfo* p = GetVipInfo(m_strCurSelectVip);

	CMenu menu ,* pSubMenu; //定义下面要用到的cmenu对象

	if (p->m_bLock)
	{
		menu.LoadMenu(IDR_MENU_VIP_LOCKED);
	}
	else
	{
		menu.LoadMenu(IDR_MENU_VIP_UNLOCKED);
	}

	//获取第一个弹出菜单，所以第一个菜单必须有子菜单
	pSubMenu = menu.GetSubMenu(0);
	if (!ISADMIN)
	{
		pSubMenu->DeleteMenu(ID_32781,MF_BYCOMMAND);  
	}
	CPoint oPoint; //定义一个用于确定光标位置的位置
	GetCursorPos( &oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标

	//在指定位置显示弹出菜单
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CDlgVip::On32779()
{
	// TODO: 在此添加命令处理程序代码
	//充值
	CDlgCharge dlg;	
	dlg.m_strID	= m_strCurSelectVip;
	dlg.DoModal();
	ShowItemVip();
	UpdateData(FALSE);
}

void CDlgVip::On32781()
{
	// TODO: 在此添加命令处理程序代码
	//删除
	if (!CheckPwd())
	{
		return ;
	}

	if(MessageBox("该会员的预定信息将被清空，确定吗？","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
	{
		return;
	}

	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->DeleteVip(m_strCurSelectVip))
	{
		ShowItemVip();
		pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());
	}
}

void CDlgVip::On32780()
{
	// TODO: 在此添加命令处理程序代码
	//锁定会员
	const VipInfo* p = GetVipInfo(m_strCurSelectVip);
	VipInfo info = *p;
	info.m_bLock = TRUE;
	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->ChangeVipInfo(info))
	{
		ShowItemVip();

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_LOCK_VIP;
		his_op_info.m_strInfo.Format("ID:%s, 姓名:%s"
			, info.m_strID.GetBuffer()
			, info.m_strName.GetBuffer());

		pOperate->Add(his_op_info);

	}
}

void CDlgVip::On32783()
{
	// TODO: 在此添加命令处理程序代码
	const VipInfo* p = GetVipInfo(m_strCurSelectVip);

	VipInfo info = *p;
	info.m_bLock = FALSE;
	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->ChangeVipInfo(info))
	{
		ShowItemVip();

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_UNLOCK_VIP;
		his_op_info.m_strInfo.Format("ID:%s, 姓名:%s"
			, info.m_strID.GetBuffer()
			, info.m_strName.GetBuffer());

		pOperate->Add(his_op_info);
	}
}

void CDlgVip::On32782()
{
	// TODO: 在此添加命令处理程序代码
	//交易历史
	CDlgVipHistory dlg;
	dlg.m_strVipID = m_strCurSelectVip;
	dlg.DoModal();
}

int CDlgVip::NewID()
{
	return ++m_nMaxID;
}

void CDlgVip::resize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;    
	GetClientRect(&recta);     //取客户区大小 

	CSize sz = recta.Size();
	if (sz.cx == 0 && sz.cy == 0)
	{
		//最小化
		return;
	}

	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/m_ptOld.x;
	fsp[1]=(float)Newp.y/m_ptOld.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //左上角
	CPoint OldBRPoint,BRPoint; //右下角
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);  
		ScreenToClient(Rect);  
		OldTLPoint = Rect.TopLeft();  
		TLPoint.x = long(OldTLPoint.x*fsp[0]);  
		TLPoint.y = long(OldTLPoint.y*fsp[1]);  
		OldBRPoint = Rect.BottomRight();  
		BRPoint.x = long(OldBRPoint.x *fsp[0]);  
		BRPoint.y = long(OldBRPoint.y *fsp[1]);  
		Rect.SetRect(TLPoint,BRPoint);  
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	m_ptOld=Newp;

}

void CDlgVip::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		resize();
	}
}
void CDlgVip::On32785()
{
	// TODO: 在此添加命令处理程序代码
}

void CDlgVip::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ShowItemVip();
}
