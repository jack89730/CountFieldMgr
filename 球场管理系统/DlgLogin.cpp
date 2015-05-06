// DlgLogin.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgLogin.h"
#include "public.h"
#include "userdata.h"
#include "operatedata.h"

// CDlgLogin �Ի���


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


// CDlgLogin ��Ϣ�������

void CDlgLogin::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strUserName.IsEmpty() || m_strPwd.IsEmpty())
	{
		MessageBox(_T("�û�������Ϊ�գ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
				MessageBox("�û������������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
		//��¼�ɹ�
		m_bSuccess = TRUE;
		m_strLevel = info.m_strLevel;

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo info;
		info.m_strName = m_strUserName;
		info.m_strType = OPERATE_TYPE_LOGIN;
		info.m_strInfo =  "��¼�ɹ�";
		pOperate->Add(info);
	}
	else
	{
		MessageBox("�û������������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	
	OnOK();
}

BOOL CDlgLogin::OnInitDialog()
{
	m_bSuccess = FALSE;
	return TRUE;
}