// DlgChangeUserPwd.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgChangeUserPwd.h"
#include "resource.h"
#include "userdata.h"
#include "app.h"
#include "operatedata.h"

// CDlgChangeUserPwd 对话框

IMPLEMENT_DYNAMIC(CDlgChangeUserPwd, CDialog)

CDlgChangeUserPwd::CDlgChangeUserPwd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangeUserPwd::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strOldPwd(_T(""))
	, m_strPwd1(_T(""))
	, m_strPwd2(_T(""))
{

}

CDlgChangeUserPwd::~CDlgChangeUserPwd()
{
}

void CDlgChangeUserPwd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUserName);
	DDX_Text(pDX, IDC_EDIT7, m_strOldPwd);
	DDX_Text(pDX, IDC_EDIT3, m_strPwd1);
	DDX_Text(pDX, IDC_EDIT4, m_strPwd2);
}


BEGIN_MESSAGE_MAP(CDlgChangeUserPwd, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgChangeUserPwd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChangeUserPwd 消息处理程序

void CDlgChangeUserPwd::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_strPwd1.IsEmpty() || m_strPwd2.IsEmpty())
	{
		MessageBox(_T("新密码不能为空！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (m_strPwd1 != m_strPwd2)
	{
		MessageBox(_T("两次新密码不一致！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	//判断是不是修改超级管理员密码
	CUserData* pData = GetUserData;
	UserInfo info_this;
	USER_LIST user_list;
	pData->GetAllData(user_list);
	int nCount = user_list.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (m_strUserName == user_list[i].m_strName)
		{
			info_this = user_list[i];
			break;
		}
	}
	if (ISADMIN)
	{
		//应该是可以找到的，因为击的是用户列表
		if (info_this.m_strLevel == USER_LEVEL_ADMIN && m_strOldPwd != info_this.m_strPwd)
		{
			//验证超级管理员密码
			MessageBox(_T("原密码不正确！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	else
	{
		if (m_strOldPwd != info_this.m_strPwd)
		{
			//验证超级管理员密码
			MessageBox(_T("原密码不正确！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	
	info_this.m_strPwd = m_strPwd1;
	pData->Change(info_this);

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CHANGE_PWD;
	his_op_info.m_strInfo.Format("修改用户密码:%s", m_strUserName.GetBuffer());
	pOperate->Add(his_op_info);

	OnOK();
}

BOOL CDlgChangeUserPwd::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (ISADMIN && m_bAdmin)
	{
		//管理员修改管理员自己
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	}
	if (ISADMIN && !m_bAdmin)
	{
		//管理员修改普通用户
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	}

	if (!ISADMIN && !m_bAdmin)
	{
		//普通用户修改自己
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(TRUE);
		SetDlgItemText(IDC_STATIC_MSG, "如果你忘记原密码，请联系超级管理员");
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	}

	return TRUE;
}