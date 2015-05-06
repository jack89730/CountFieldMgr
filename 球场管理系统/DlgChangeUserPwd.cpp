// DlgChangeUserPwd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgChangeUserPwd.h"
#include "resource.h"
#include "userdata.h"
#include "app.h"
#include "operatedata.h"

// CDlgChangeUserPwd �Ի���

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


// CDlgChangeUserPwd ��Ϣ�������

void CDlgChangeUserPwd::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_strPwd1.IsEmpty() || m_strPwd2.IsEmpty())
	{
		MessageBox(_T("�����벻��Ϊ�գ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (m_strPwd1 != m_strPwd2)
	{
		MessageBox(_T("���������벻һ�£�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	//�ж��ǲ����޸ĳ�������Ա����
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
		//Ӧ���ǿ����ҵ��ģ���Ϊ�������û��б�
		if (info_this.m_strLevel == USER_LEVEL_ADMIN && m_strOldPwd != info_this.m_strPwd)
		{
			//��֤��������Ա����
			MessageBox(_T("ԭ���벻��ȷ��"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	else
	{
		if (m_strOldPwd != info_this.m_strPwd)
		{
			//��֤��������Ա����
			MessageBox(_T("ԭ���벻��ȷ��"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	
	info_this.m_strPwd = m_strPwd1;
	pData->Change(info_this);

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CHANGE_PWD;
	his_op_info.m_strInfo.Format("�޸��û�����:%s", m_strUserName.GetBuffer());
	pOperate->Add(his_op_info);

	OnOK();
}

BOOL CDlgChangeUserPwd::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (ISADMIN && m_bAdmin)
	{
		//����Ա�޸Ĺ���Ա�Լ�
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	}
	if (ISADMIN && !m_bAdmin)
	{
		//����Ա�޸���ͨ�û�
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT7)->EnableWindow(FALSE);
	}

	if (!ISADMIN && !m_bAdmin)
	{
		//��ͨ�û��޸��Լ�
		GetDlgItem(IDC_STATIC_MSG)->ShowWindow(TRUE);
		SetDlgItemText(IDC_STATIC_MSG, "���������ԭ���룬����ϵ��������Ա");
		GetDlgItem(IDC_EDIT7)->EnableWindow(TRUE);
	}

	return TRUE;
}