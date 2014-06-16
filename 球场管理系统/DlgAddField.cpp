// DlgAddField.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAddField.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"

// CDlgAddField �Ի���

IMPLEMENT_DYNAMIC(CDlgAddField, CDialog)

CDlgAddField::CDlgAddField(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddField::IDD, pParent)
	, m_strName(_T(""))
{

}

CDlgAddField::~CDlgAddField()
{
}

void CDlgAddField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgAddField, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddField::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddField ��Ϣ�������


BOOL CDlgAddField::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddField::OnBnClickedOk()
{
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	if(m_strName.IsEmpty())
	{
		MessageBox(_T("���Ʋ���Ϊ�գ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strName.GetLength() >= 5)
	{
		MessageBox(_T("����ҪС��5��Ӣ���ַ���"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageField.m_pFieldData->AddField(m_strName))
	{
		m_bIsSuccess = TRUE;
	}

	CDialog::OnOK();
}
