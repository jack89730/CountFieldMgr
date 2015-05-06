// DlgUserMgr.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgUserMgr.h"
#include "public.h"
#include "userdata.h"
#include "DlgAddUser.h"
#include "app.h"
#include "DlgChangeUserPwd.h"
#include "operatedata.h"
// CDlgUserMgr 对话框

IMPLEMENT_DYNAMIC(CDlgUserMgr, CDialog)

CDlgUserMgr::CDlgUserMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserMgr::IDD, pParent)
{

}

CDlgUserMgr::~CDlgUserMgr()
{
}

void CDlgUserMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listUser);
}


BEGIN_MESSAGE_MAP(CDlgUserMgr, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgUserMgr::OnNMRClickList1)
	ON_COMMAND(ID_USER_ADD, &CDlgUserMgr::OnUserAdd)
	ON_COMMAND(ID_USER_PWD, &CDlgUserMgr::OnUserPwd)
	ON_COMMAND(ID_USE_DEL, &CDlgUserMgr::OnUseDel)
END_MESSAGE_MAP()


// CDlgUserMgr 消息处理程序
BOOL CDlgUserMgr::OnInitDialog()
{
	m_pUserData = GetUserData;
	CDialog::OnInitDialog();
	m_listUser.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	int k=0;
	m_listUser.InsertColumn(k++, _T("用户名"),	LVCFMT_CENTER, 200);
	m_listUser.InsertColumn(k++, _T("等级"),	LVCFMT_CENTER, 200);
	ShowAllUser();
	m_nCurSelectIndex = -1;
	return TRUE;
}

void CDlgUserMgr::ShowAllUser(CString strFilter)
{
	m_ayUserList.RemoveAll();
	m_listUser.DeleteAllItems();
	CUserData* pUser = GetUserData;
	pUser->GetAllData(m_ayUserList);
	int nCount = m_ayUserList.GetSize();
	int i = 0;
	BOOL bAdmin = ISADMIN;
	CString strUserName = GETUSERNAME;
	if (bAdmin)
	{
		for (; i < nCount; i++)
		{		
			int j = 0;
			m_listUser.InsertItem(i, m_ayUserList[i].m_strName);
			if (m_ayUserList[i].m_strLevel == USER_LEVEL_ADMIN)
			{
				m_listUser.SetItemText(i, ++j, "1(超级管理员)");
			}
			else
			{
				m_listUser.SetItemText(i, ++j, "2(普通管理员)");
			}
		}
	}
	else
	{
		//不是超级管理员只能显示自己
		UserInfo self;
		for (; i < nCount; i++)
		{		
			if (m_ayUserList[i].m_strName == strUserName)
			{
				self = m_ayUserList[i];
				int j = 0;
				m_listUser.InsertItem(0, m_ayUserList[i].m_strName);
				m_listUser.SetItemText(0, ++j, "2(普通管理员)");
			}
		}
		m_ayUserList.RemoveAll();
		m_ayUserList.Add(self);
	}

}
void CDlgUserMgr::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	m_nCurSelectIndex = pNotifier->iItem;
	
	if (pNotifier->iItem < 0)
	{
		CMenu menu ,* pSubMenu; //定义下面要用到的cmenu对象
		menu.LoadMenu(IDR_MENU_USER_ADD);
		pSubMenu = menu.GetSubMenu(0);
		CPoint oPoint;
		GetCursorPos( &oPoint);
		//在指定位置显示弹出菜单
		pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
		return;
	}

	int nMenuId = IDR_MENU_USERMGR;
	if (!ISADMIN)
	{
		nMenuId = IDR_MENU_USERMGR_COMMON;
	}
	
	CMenu menu ,* pSubMenu; //定义下面要用到的cmenu对象
	menu.LoadMenu(nMenuId);
	//获取第一个弹出菜单，所以第一个菜单必须有子菜单
	pSubMenu = menu.GetSubMenu(0);
	CPoint oPoint; //定义一个用于确定光标位置的位置
	GetCursorPos( &oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标
	//在指定位置显示弹出菜单
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
}

void CDlgUserMgr::OnUserAdd()
{
	// TODO: 在此添加命令处理程序代码
	CDlgAddUser dlg;
	dlg.DoModal();
	ShowAllUser();
}

void CDlgUserMgr::OnUserPwd()
{
	// TODO: 在此添加命令处理程序代码
	CDlgChangeUserPwd dlg;
	UserInfo& info = m_ayUserList[m_nCurSelectIndex];
	dlg.m_strUserName = info.m_strName;
	dlg.m_bAdmin = info.m_strLevel == USER_LEVEL_ADMIN;
	dlg.DoModal();
}

void CDlgUserMgr::OnUseDel()
{
	// TODO: 在此添加命令处理程序代码
	UserInfo info = m_ayUserList[m_nCurSelectIndex];
	if (info.m_strLevel == USER_LEVEL_ADMIN)
	{
		MessageBox("超级管理员不能删除！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (info.m_strName == GETUSERNAME)
	{
		MessageBox("不能删除自己！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	m_ayUserList.RemoveAt(m_nCurSelectIndex);
	m_nCurSelectIndex = -1;
	m_pUserData->Delete(info.m_strName);

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_DEL_USER;
	his_op_info.m_strInfo.Format("删除用户:%s", info.m_strName.GetBuffer());
	pOperate->Add(his_op_info);

	ShowAllUser();

}
