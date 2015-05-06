// DlgInit.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgInit.h"
#include "MainDlg.h"

// CDlgInit 对话框

IMPLEMENT_DYNAMIC(CDlgInit, CDialog)

CDlgInit::CDlgInit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInit::IDD, pParent)
	, m_nFieldCount(0)
{

}

CDlgInit::~CDlgInit()
{
}

void CDlgInit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboStart);
	DDX_Control(pDX, IDC_COMBO3, m_comboStop);
	DDX_Text(pDX, IDC_EDIT1, m_nFieldCount);
}


BEGIN_MESSAGE_MAP(CDlgInit, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInit::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgInit 消息处理程序

void CDlgInit::OnBnClickedOk()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	int nStart = m_comboStart.GetCurSel();
	int nStop = m_comboStop.GetCurSel();
	if (nStop <=  nStart)
	{
		MessageBox("结束时间要大于开始时间！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (m_nFieldCount <= 0)
	{
		MessageBox("请初始化最少一个场地！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	CMainDlg* pMain = GETMAINWND;
	CPeriodData* pPeriod = GetPeriodData;
	PeriodInfo  periodInfo;
	pPeriod->Read(periodInfo);
	periodInfo.m_bInit = TRUE;
	periodInfo.m_nHalfCount = nStop - nStart;
	periodInfo.m_nStartMin = nStart*30;
	periodInfo.m_nGoldStart = 0;
	periodInfo.m_nGoldStop = periodInfo.m_nHalfCount;//先把全天设置为黄金时段
	periodInfo.m_bWeekend = FALSE;
// 	if (m_checkWeekend.GetCheck())
// 	{
// 		periodInfo.m_bWeekend = TRUE;
// 	}
// 	else
// 	{
// 		periodInfo.m_bWeekend = FALSE;
// 	}
	pPeriod->Set(periodInfo);

	for (int i = 0; i < m_nFieldCount; i++)
	{
		CString strField;
		strField.Format("%d", i+1);
		pMain->m_pageField.m_pFieldData->AddField(strField);
	}
	pMain->m_pageField.ShowItemField();

	OnOK();
}

BOOL CDlgInit::OnInitDialog()
{
	CDialog::OnInitDialog();
	for (int i = 0; i <=24; i++)
	{
		CString strTime;
		strTime.Format("%d:00", i);
		
		m_comboStart.AddString(strTime);
		m_comboStop.AddString(strTime);
		
		if (i != 24)
		{
			strTime.Format("%d:30", i);
			m_comboStart.AddString(strTime);
			m_comboStop.AddString(strTime);
		}
	}
	return TRUE;
}