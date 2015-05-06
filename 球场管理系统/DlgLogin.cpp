// DlgLogin.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgLogin.h"
#include "public.h"
#include "userdata.h"
#include "operatedata.h"

// CDlgLogin 对话框


IMPLEMENT_DYNAMIC(CDlgLogin, CDialog)

CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
	, m_strUserName(_T(""))
	, m_strPwd(_T(""))
{

}

CDlgLogin::~CDlgLogin()
{
}

void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_EDIT_PWD, m_strPwd);
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgLogin::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgLogin 消息处理程序

void CDlgLogin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strUserName.IsEmpty() || m_strPwd.IsEmpty())
	{
		MessageBox(_T("用户名密码为空！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
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
		if (info.m_strName == m_strUserName)
		{
			if (info.m_strPwd != m_strPwd)
			{
				MessageBox("用户名或密码错误！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
				return;
			}
			else
			{
				break;
			}
		}
	}

	if (i < nCount)
	{
		//登录成功
		m_bSuccess = TRUE;
		m_strLevel = info.m_strLevel;

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo info;
		info.m_strName = m_strUserName;
		info.m_strType = OPERATE_TYPE_LOGIN;
		info.m_strInfo =  "登录成功";
		pOperate->Add(info);
	}
	else
	{
		MessageBox("用户名或密码错误！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	
	OnOK();
}

BOOL CDlgLogin::OnInitDialog()
{
	m_bSuccess = FALSE;
	return TRUE;
}