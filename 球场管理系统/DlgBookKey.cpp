// DlgBookKey.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgBookKey.h"


// CDlgBookKey �Ի���

IMPLEMENT_DYNAMIC(CDlgBookKey, CDialog)

CDlgBookKey::CDlgBookKey(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBookKey::IDD, pParent)
	, m_strKey(_T(""))
{

}

CDlgBookKey::~CDlgBookKey()
{
}

void CDlgBookKey::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strKey);
}


BEGIN_MESSAGE_MAP(CDlgBookKey, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBookKey::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBookKey ��Ϣ�������

void CDlgBookKey::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (m_strKey.IsEmpty())
	{
		MessageBox("���ӺŲ���Ϊ��!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	OnOK();
}
