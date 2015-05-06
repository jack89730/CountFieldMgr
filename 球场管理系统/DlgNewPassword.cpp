// DlgNewPassword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgNewPassword.h"

#include<iomanip>
#include<fstream>
#include "public.h"
using namespace std;

// CDlgNewPassword �Ի���

IMPLEMENT_DYNAMIC(CDlgNewPassword, CDialog)

CDlgNewPassword::CDlgNewPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewPassword::IDD, pParent)
	, m_strPassword1(_T(""))
	, m_strPassword2(_T(""))
{

}

CDlgNewPassword::~CDlgNewPassword()
{
}

void CDlgNewPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPassword1);
	DDX_Text(pDX, IDC_EDIT2, m_strPassword2);
}


BEGIN_MESSAGE_MAP(CDlgNewPassword, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgNewPassword::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewPassword ��Ϣ�������

void CDlgNewPassword::OnBnClickedOk()
{
	UpdateData(TRUE);

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_strPassword1.IsEmpty())
	{
		MessageBox(_T("���������룡"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (m_strPassword1 != m_strPassword2)
	{
		MessageBox(_T("�������벻һ�£�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	OnOK();
}
