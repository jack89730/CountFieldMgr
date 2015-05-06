// DlgCancelBook.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCancelBook.h"
#include "MainDlg.h"
#include "operatedata.h"
#include "app.h"

// CDlgCancelBook 对话框

IMPLEMENT_DYNAMIC(CDlgCancelBook, CDialog)

CDlgCancelBook::CDlgCancelBook(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCancelBook::IDD, pParent)
{

}

CDlgCancelBook::~CDlgCancelBook()
{
}

void CDlgCancelBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboStart);
	DDX_Control(pDX, IDC_COMBO2, m_comboStop);
}


BEGIN_MESSAGE_MAP(CDlgCancelBook, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCancelBook::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCancelBook 消息处理程序

BOOL CDlgCancelBook::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nCount = m_ayStartTimeStr.GetSize();
	m_comboStart.ResetContent();
	m_comboStop.ResetContent();
	for (int i = 0; i < nCount; ++i)
	{
		m_comboStart.AddString(m_ayStartTimeStr[i]);
		m_comboStop.AddString(m_ayStopTimeStr[i]);
	}
	m_comboStart.SetCurSel(m_nTimeIndex);
	m_comboStop.SetCurSel(m_nTimeIndex);
	CString strMsg;
	CMainDlg* pMain = GETMAINWND;
	VipInfo* pVip = pMain->m_pageVip.GetVipInfo(m_strVipID);
	strMsg.Format("预订人：%s", pVip->m_strName.GetBuffer());
	SetDlgItemText(IDC_STATIC_VIP, strMsg);
	return TRUE;
}
void CDlgCancelBook::OnBnClickedOk()
{
	//预订时间段
	CMainDlg* pMainWnd = GETMAINWND;

	int nIndexStart = m_ayIndex[m_comboStart.GetCurSel()];
	int nIndexStop = m_ayIndex[m_comboStop.GetCurSel()];
	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		OneTimeBookInfo info;
		OneTimeBookInfo* pinfo = pMainWnd->m_pageField.GetFieldBookInfo(m_strFieldID, i);
		info =  *pinfo;
		info.m_bBusy = FIELD_IDLE_DATA;
		info.m_strVipID = "0";
		info.m_bAutoPrice = TRUE;
		info.m_strPrice = "0";
		pMainWnd->m_pageField.ChangeFieldBookInfo(m_strFieldID, info);
	}
	// TODO: 在此添加控件通知处理程序代码

	CString strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[nIndexStart];
	CString strStart = strTmp.Left(strTmp.Find('-'));

	strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[nIndexStop];
	CString strStop =strTmp.Right(strTmp.Find('-'));

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CANCEL_FIELD;
	his_op_info.m_strInfo.Format("场地:%s,日期:%s,时段:%s-%s,", m_strFieldID.GetBuffer(), pMainWnd->m_pageField.GetDate().GetBuffer(), strStart.GetBuffer(), strStop.GetBuffer());
	
	VipInfo* pVip = pMainWnd->m_pageVip.GetVipInfo(m_strVipID);
	CString strTmpVip;
	if (pVip->m_strLevelID == VIP_LEVEL_0)
	{
		strTmpVip.Format("非会员,姓名:%s,电话:%s", pVip->m_strName.GetBuffer(), pVip->m_strPhone.GetBuffer());
	}
	else
	{
		strTmpVip.Format("会员ID:%s,姓名:%s,电话:%s", pVip->m_strID.GetBuffer(), pVip->m_strName.GetBuffer(), pVip->m_strPhone.GetBuffer());
	}
	his_op_info.m_strInfo += strTmpVip;
	pOperate->Add(his_op_info);

	OnOK();
}
