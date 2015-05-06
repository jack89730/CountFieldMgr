// DlgAddVip.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAddVip.h"
#include "access.h"
#include "tinyxml.h"
#include "MainDlg.h"

// CDlgAddVip �Ի���

IMPLEMENT_DYNAMIC(CDlgAddVip, CDialog)

CDlgAddVip::CDlgAddVip(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddVip::IDD, pParent)
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strPhone(_T(""))
	, m_strMoney(_T(""))
	, m_strID(_T(""))
{

}

CDlgAddVip::~CDlgAddVip()
{
}

void CDlgAddVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT3, m_strPhone);
	DDX_Text(pDX, IDC_EDIT4, m_strMoney);
	DDX_Control(pDX, IDC_COMBO1, m_comboLevel);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboSex);
}


BEGIN_MESSAGE_MAP(CDlgAddVip, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddVip::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddVip ��Ϣ�������

BOOL CDlgAddVip::OnInitDialog()
{
	CDialog::OnInitDialog();
	CLevelData* pPrice = GetLevelData;
	pPrice->GetAllData(m_ayPriceList);
	int n = m_ayPriceList.GetSize();
	int nDelIndex = -1;
	for (int i = 0; i < n; i++)
	{
		if (m_ayPriceList[i].m_strID == VIP_LEVEL_0)
		{
			//�ȼ�Ϊ0 ֻ���ǻ�ԱԤ������ʱʹ�ã������ֶ���ӻ�Ա
			nDelIndex = i;
			continue;
		}
		CString str;
		str.Format("%s-%s", m_ayPriceList[i].m_strID.GetBuffer(), m_ayPriceList[i].m_strName.GetBuffer());
		m_comboLevel.AddString(str);
	}
	if (nDelIndex > -1)
	{
		m_ayPriceList.RemoveAt(nDelIndex);
	}
	m_bIsSuccess = FALSE;
	m_comboLevel.SetCurSel(0);
	m_comboSex.SetCurSel(0);
	m_strMoney = "0";
	UpdateData(FALSE);
	return TRUE;
}

void CDlgAddVip::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	if (m_strID.IsEmpty())
	{
		MessageBox("ID����Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if(m_strName.IsEmpty())
	{
		MessageBox("��������Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	GetDlgItemText(IDC_COMBO_SEX, m_strSex);

	if(m_strSex.IsEmpty())
	{
		MessageBox("�Ա���Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strPhone.IsEmpty())
	{
		MessageBox( "�ֻ����벻��Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strMoney.IsEmpty() || atoi(m_strMoney.GetBuffer())< 0)
	{
		MessageBox("����Ϊ�ջ�����", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	
	CMainDlg* pMainWnd = GETMAINWND;

	VipInfo info;
	info.m_strID = m_strID;
	info.m_strName = m_strName;
	info.m_strMoney = m_strMoney;
	if (atoi(m_strMoney) == 0)
	{
		info.m_strMoney = "0";
	}
	info.m_strPhone = m_strPhone;
	info.m_strSex = m_strSex;
	int nSel = m_comboLevel.GetCurSel();
	if (m_comboLevel.GetCount() == 0)
	{
		MessageBox("����ӻ�Ա�ȼ���", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (nSel == -1)
	{
		MessageBox("��ѡ���Ա�ȼ���", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	info.m_strLevelID = m_ayPriceList[nSel].m_strID;
	
	info.m_bLock = FALSE;
	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->AddVip(info))
	{
		m_bIsSuccess = TRUE;
	}

	CDialog::OnOK();


}

