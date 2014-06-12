// DlgCheckOut.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgCheckOut.h"


// CDlgCheckOut �Ի���

IMPLEMENT_DYNAMIC(CDlgCheckOut, CDialog)

CDlgCheckOut::CDlgCheckOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCheckOut::IDD, pParent)
	, m_strTotalMoney(_T(""))
	, m_strPay(_T(""))
{

}

CDlgCheckOut::~CDlgCheckOut()
{
}

void CDlgCheckOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTotalMoney);
	DDX_Text(pDX, IDC_EDIT2, m_strPay);
}


BEGIN_MESSAGE_MAP(CDlgCheckOut, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCheckOut::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCheckOut ��Ϣ�������

void CDlgCheckOut::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nMoney = atoi(m_strTotalMoney.GetBuffer());
	int nPay = atoi(m_strPay.GetBuffer());
	
	if (nPay < nMoney)
	{
		MessageBox("����!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	CString strMsg;
	strMsg.Format("���㣺%d", nPay - nMoney);
	MessageBox(strMsg, "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	OnOK();
}
