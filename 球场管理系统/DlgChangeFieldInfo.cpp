// DlgChangeFieldInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgChangeFieldInfo.h"
#include "MainDlg.h"

// CDlgChangeFieldInfo �Ի���

IMPLEMENT_DYNAMIC(CDlgChangeFieldInfo, CDialog)

CDlgChangeFieldInfo::CDlgChangeFieldInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangeFieldInfo::IDD, pParent)
	, m_strID(_T(""))
	, m_strName(_T(""))
{

}

CDlgChangeFieldInfo::~CDlgChangeFieldInfo()
{
}

void CDlgChangeFieldInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strID);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgChangeFieldInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgChangeFieldInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChangeFieldInfo ��Ϣ�������

void CDlgChangeFieldInfo::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CMainDlg* pMain = GETMAINWND;
	FieldInfo info;
	info.m_strFieldID = m_strID;
	info.m_strFieldName = m_strName;

	//ID�����ƶ������ظ�
	int nCount = pMain->m_pageField.m_ayFieldInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (m_strName == pMain->m_pageField.m_ayFieldInfo[i].m_strFieldName)
		{

			MessageBox("���������Ѿ�����!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	pMain->m_pageField.m_pFieldData->ChangeFieldInfo(info);
	OnOK();
}
