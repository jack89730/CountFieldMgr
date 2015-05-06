// DlgAddFood.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAddFood.h"
#include "tinyxml.h"
#include "fooddata.h"
#include "MainDlg.h"
#include "public.h"
// CDlgAddFood �Ի���

IMPLEMENT_DYNAMIC(CDlgAddFood, CDialog)

CDlgAddFood::CDlgAddFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddFood::IDD, pParent)
	, m_strName(_T(""))
	, m_strPrice(_T(""))
	, m_strID(_T(""))
	, m_strRemain(_T(""))
{

}

CDlgAddFood::~CDlgAddFood()
{
}

void CDlgAddFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_strPrice);
	DDX_Text(pDX, IDC_EDIT4, m_strRemain);
}


BEGIN_MESSAGE_MAP(CDlgAddFood, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddFood::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddFood ��Ϣ�������

BOOL CDlgAddFood::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	m_strRemain = "0";
	m_strPrice = "0";
	return TRUE;
}

void CDlgAddFood::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	//TODO: ÿ�����Ϳ��������ϸ������

	if(m_strName.IsEmpty())
	{
		MessageBox("���Ʋ���Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strPrice.GetBuffer())< 0)
	{
		MessageBox("�۸���Ϊ������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strRemain.GetBuffer()) < 0)
	{
		MessageBox("��治��Ϊ������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	FoodInfo info;
	info.m_strID = m_strID;
	info.m_strName = m_strName;
	info.m_strPrice = m_strPrice;
	info.m_strRemain = m_strRemain;

	CMainDlg* pMainWnd = GETMAINWND;
	int ret = pMainWnd->m_pageFood.m_pFoodData->AddFood(info);
	if (ret == ERROR_NAME_EXIST)
	{
		MessageBox("ID�Ѵ���", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	CDialog::OnOK();

	m_bIsSuccess = TRUE;
}

void CDlgAddFood::OnBnClickedCommand1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
