
// �򳡹���ϵͳDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "app.h"
#include "MainDlg.h"
#include "userdata.h"
#include "leveldata.h"
#include "fielddata.h"
#include "vipdata.h"
#include "fooddata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMainDlg �Ի���


CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_TAB_MAIN, m_tab);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, ID_TAB_MAIN, &CMainDlg::OnTcnSelchangeTabMain)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CMainDlg ��Ϣ�������

BOOL CMainDlg::OnInitDialog()
{
	AnimateWindow(200,AW_SLIDE| AW_ACTIVATE|AW_BLEND|AW_HIDE|AW_CENTER);
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//ΪTab Control��������ҳ��
	m_tab.InsertItem(0, _T("��Ԥ��"));
	m_tab.InsertItem(1, _T("��Ʒ����"));
	m_tab.InsertItem(2, _T("��Ա����"));
	m_tab.InsertItem(3, _T("ϵͳ����"));
	
	m_tab.SetItemSize(CSize(250,25));

	//�����Ի���
	m_pageField.Create(IDD_DLG_FIELD, &m_tab);
	m_pageMgr.Create(IDD_DLG_MGR, &m_tab);
	m_pageFood.Create(IDD_DLG_FOOD, &m_tab);
	m_pageVip.Create(IDD_DLG_VIP, &m_tab);
	
	//�趨��Tab����ʾ�ķ�Χ
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 0;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_pageMgr.MoveWindow(&rc);
	m_pageField.MoveWindow(&rc);
	m_pageFood.MoveWindow(&rc);
	m_pageVip.MoveWindow(&rc);
	


	//�ѶԻ������ָ�뱣������
	m_pDialog[0] = &m_pageField;
	m_pDialog[1] = &m_pageFood;
	m_pDialog[2] = &m_pageVip;
	m_pDialog[3] = &m_pageMgr;

	//��ʾ��ʼҳ��
	m_pDialog[3]->ShowWindow(SW_HIDE);
	m_pDialog[0]->ShowWindow(SW_SHOW);
	m_pDialog[1]->ShowWindow(SW_HIDE);
	m_pDialog[2]->ShowWindow(SW_HIDE);
	
	//���浱ǰѡ��
	m_CurSelTab = 0;

	SetTimer(1, 1000, NULL);

	CString sDate;
	CTime Today=CTime::GetCurrentTime();
	sDate=Today.Format("%Y��%m��%d��");
	int week=Today.GetDayOfWeek();//��ȡ���ڼ���1Ϊ�����죬7Ϊ������
	switch(week)
	{
	case 1:{sDate=sDate+"  ������";break;}
	case 2:{sDate=sDate+"  ����һ";break;}
	case 3:{sDate=sDate+"  ���ڶ�";break;}
	case 4:{sDate=sDate+"  ������";break;}
	case 5:{sDate=sDate+"  ������";break;}
	case 6:{sDate=sDate+"  ������";break;}
	case 7:{sDate=sDate+"  ������";break;}
	} 
	CTime tm = CTime::GetCurrentTime();
	CString str = tm.Format("%H:%M:%S");

	SetDlgItemText(IDC_TIME,sDate); 
	SetDlgItemText(IDC_WEEK,str);

	CString strUser;
	if (ISADMIN)
	{
		strUser.Format("��������Ա��%s", GETUSERNAME.GetBuffer());
	}
	else
	{
		strUser.Format("��ͨ����Ա��%s", GETUSERNAME.GetBuffer());
	}
	
	

	ShowWindow(SW_MAXIMIZE);
	SetDlgItemText(IDC_USER, strUser.GetBuffer());
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMainDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	//�ѵ�ǰ��ҳ����������
	m_pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//�õ��µ�ҳ������
	m_CurSelTab = m_tab.GetCurSel();
	//���µ�ҳ����ʾ����
	m_pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;

}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)   //��ʾ����ʱ��
{
	// TODO: Add your message handler code here and/or call default
	CString sDate;
	CTime Today=CTime::GetCurrentTime();
	sDate=Today.Format("%Y��%m��%d��");
	int week=Today.GetDayOfWeek();//��ȡ���ڼ���1Ϊ�����죬7Ϊ������
	switch(week)
	{
	case 1:{sDate=sDate+"  ������";break;}
	case 2:{sDate=sDate+"  ����һ";break;}
	case 3:{sDate=sDate+"  ���ڶ�";break;}
	case 4:{sDate=sDate+"  ������";break;}
	case 5:{sDate=sDate+"  ������";break;}
	case 6:{sDate=sDate+"  ������";break;}
	case 7:{sDate=sDate+"  ������";break;}
	} 
	CTime tm = CTime::GetCurrentTime();
	CString str = tm.Format("%H:%M:%S");

	SetDlgItemText(IDC_TIME,sDate); 
	SetDlgItemText(IDC_WEEK,str);

	static int nLast = -1;

	int nMin = tm.GetMinute();
	if ((nMin == 0 || nMin == 30) && nLast != nMin)
	{
		m_pageField.ShowItemField();
		nLast = nMin;
	}
	CDialog::OnTimer(nIDEvent);
}

void CMainDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	//m_chartTest2.OnSize(cx,cy);
	//�ؼ���ͼ�����Ŵ��ڵĴ�С�ı���ı�
	if(m_tab.GetSafeHwnd())
		m_tab.MoveWindow(0,45,cx,cy);

	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_pageField.MoveWindow(&rc);
	m_pageFood.MoveWindow(&rc);
	m_pageVip.MoveWindow(&rc);
	m_pageMgr.MoveWindow(&rc);

}

void CMainDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(MessageBox("�Ƿ��˳���","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
		return;
	CDialog::OnClose();
	CVipData* pVip = GetVipData;
	pVip->Save();
	CFieldData* pField = GetFieldData;
	pField->Save();

	CUserData* pUser = GetUserData;
	pUser->Save();

	CFoodData* pFood = GetFoodData;
	pFood->Save();

	CPeriodData* pPeriod = GetPeriodData;
	pPeriod->Save();

	CLevelData* pPrice = GetLevelData;
	pPrice->Save();

}
