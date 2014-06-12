// DlgBook.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgBook.h"
#include "MainDlg.h"
#include "xmlbase.h"
#include "public.h"

// CDlgBook 对话框

IMPLEMENT_DYNAMIC(CDlgBook, CDialog)

CDlgBook::CDlgBook(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBook::IDD, pParent)
	, m_strBookTime(_T(""))
	, m_strVipID(_T(""))
	, m_strFieldID(_T(""))
{

}

CDlgBook::~CDlgBook()
{
}

void CDlgBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_strVipID);
	DDX_Text(pDX, IDC_EDIT_FIELD, m_strFieldID);
	DDX_Control(pDX, IDC_COMBO_START, m_comboStart);
	DDX_Control(pDX, IDC_COMBO_STOP, m_comboStop);
}


BEGIN_MESSAGE_MAP(CDlgBook, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBook::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgBook 消息处理程序

void CDlgBook::OnBnClickedOk()
{
	m_bSuccess = FALSE;
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_strVipID.IsEmpty())
	{
		MessageBox("会员ID不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	//查找是否存在该会员
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	int nVipCount = pMainWnd->m_pageVip.m_ayVip.GetCount();

	BOOL bFind = FALSE;
	for (int i = 0; i < nVipCount; ++i)
	{
		if (pMainWnd->m_pageVip.m_ayVip[i].m_strID == m_strVipID)
		{
			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
	{
		MessageBox("会员ID不存在！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	//预订时间段
	int nIndexStart = m_ayIndex[m_comboStart.GetCurSel()];
	int nIndexStop = m_ayIndex[m_comboStop.GetCurSel()];
	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		OneTimeInfo info;

		OneTimeInfo* pinfo = pMainWnd->m_pageField.GetFieldInfo(m_strFieldID, i);
		if (!pinfo)
		{
			pMainWnd->m_pageField.AddFieldDay(m_strFieldID);
			pinfo = pMainWnd->m_pageField.GetFieldInfo(m_strFieldID, i);
		}
		info =  *pinfo;

		//交换信息
		info.m_bBusy = TRUE;
		info.m_strVipID = m_strVipID;
		pMainWnd->m_pageField.ChangeFieldInfo(m_strFieldID, info);
	}


	pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

	OnOK();
}

BOOL CDlgBook::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nCount = m_ayStartTimeStr.GetSize();
	m_comboStart.ResetContent();
	m_comboStop.ResetContent();
	for (int i = 0; i < nCount; ++i )
	{
		m_comboStart.AddString(m_ayStartTimeStr[i]);
		m_comboStop.AddString(m_ayStopTimeStr[i]);
	}
	m_comboStart.SetCurSel(m_nTimeIndex);
	m_comboStop.SetCurSel(m_nTimeIndex);
	return TRUE;
}
