// DlgInputFood.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgInputFood.h"


// CDlgInputFood �Ի���

IMPLEMENT_DYNAMIC(CDlgInputFood, CDialog)

CDlgInputFood::CDlgInputFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputFood::IDD, pParent)
	, m_strName(_T(""))
	, m_nCount(0)
{

}

CDlgInputFood::~CDlgInputFood()
{
}

void CDlgInputFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_nCount);
}


BEGIN_MESSAGE_MAP(CDlgInputFood, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInputFood::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInputFood ��Ϣ�������

void CDlgInputFood::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (m_nCount <= 0)
	{
		MessageBox("����Ҫ����0��", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
