// DlgPasssword.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgPasssword.h"


// CDlgPasssword �Ի���

IMPLEMENT_DYNAMIC(CDlgPasssword, CDialog)

CDlgPasssword::CDlgPasssword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPasssword::IDD, pParent)
	, m_strPassword(_T(""))
{

}

CDlgPasssword::~CDlgPasssword()
{
}

void CDlgPasssword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strPassword);
}


BEGIN_MESSAGE_MAP(CDlgPasssword, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPasssword::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPasssword ��Ϣ�������

void CDlgPasssword::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}
