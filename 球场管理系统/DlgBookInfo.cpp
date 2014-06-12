// DlgBookInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgBookInfo.h"


// CDlgBookInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgBookInfo, CDialog)

CDlgBookInfo::CDlgBookInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBookInfo::IDD, pParent)
	, m_strVipID(_T(""))
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strPhone(_T(""))
{

}

CDlgBookInfo::~CDlgBookInfo()
{
}

void CDlgBookInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_strVipID);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_SEX, m_strSex);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strPhone);
}


BEGIN_MESSAGE_MAP(CDlgBookInfo, CDialog)
END_MESSAGE_MAP()


// CDlgBookInfo ��Ϣ�������
BOOL CDlgBookInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_strTitle);
	return TRUE;
}