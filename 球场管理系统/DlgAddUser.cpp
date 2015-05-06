// DlgAddUser.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAddUser.h"
#include "userdata.h"
#include "operatedata.h"
#include "app.h"

// CDlgAddUser 对话框

IMPLEMENT_DYNAMIC(CDlgAddUser, CDialog)

CDlgAddUser::CDlgAddUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddUser::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPwd1(_T(""))
	, m_strPwd2(_T(""))
{

}

CDlgAddUser::~CDlgAddUser()
{
}

void CDlgAddUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUserName);
	DDX_Text(pDX, IDC_EDIT2, m_strPwd1);
	DDX_Text(pDX, IDC_EDIT3, m_strPwd2);
}


BEGIN_MESSAGE_MAP(CDlgAddUser, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddUser::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddUser 消息处理程序

void CDlgAddUser::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strUserName.IsEmpty() || m_strPwd1.IsEmpty() || m_strPwd2.IsEmpty())
	{
		MessageBox(_T("用户名和密码不能为空！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (m_strPwd1 != m_strPwd2)
	{
		MessageBox(_T("两次密码不一致！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	CUserData* pData = GetUserData;
	UserInfo info;
	info.m_strLevel = USER_LEVEL_COMMON;
	info.m_strName = m_strUserName;
	info.m_strPwd = m_strPwd1;
	int nRet = pData->Add(info);
	if (nRet == ERROR_NAME_EXIST)
	{
		MessageBox(_T("用户名已存在！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	
	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_ADD_USER;
	his_op_info.m_strInfo.Format("添加用户:%s", m_strUserName.GetBuffer());
	pOperate->Add(his_op_info);

	OnOK();
}
