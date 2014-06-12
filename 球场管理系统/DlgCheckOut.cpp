// DlgCheckOut.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCheckOut.h"


// CDlgCheckOut 对话框

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


// CDlgCheckOut 消息处理程序

void CDlgCheckOut::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nMoney = atoi(m_strTotalMoney.GetBuffer());
	int nPay = atoi(m_strPay.GetBuffer());
	
	if (nPay < nMoney)
	{
		MessageBox("金额不足!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	CString strMsg;
	strMsg.Format("找零：%d", nPay - nMoney);
	MessageBox(strMsg, "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	OnOK();
}
