
// 球场管理系统Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "app.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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
END_MESSAGE_MAP()


// CMainDlg 对话框


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
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, ID_TAB_MAIN, &CMainDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	AnimateWindow(200,AW_SLIDE| AW_ACTIVATE|AW_BLEND|AW_HIDE|AW_CENTER);
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//为Tab Control增加两个页面
	m_tab.InsertItem(0, _T("球场"));
	m_tab.InsertItem(1, _T("商品"));
	m_tab.InsertItem(2, _T("会员"));

	//创建对话框
	m_pageField.Create(IDD_DLG_FIELD, &m_tab);
	m_pageFood.Create(IDD_DLG_FOOD, &m_tab);
	m_pageVip.Create(IDD_DLG_VIP, &m_tab);

	//设定在Tab内显示的范围
	CRect rc;
	m_tab.GetClientRect(rc);
	rc.top += 20;
	rc.bottom -= 0;
	rc.left += 0;
	rc.right -= 0;
	m_pageField.MoveWindow(&rc);
	m_pageFood.MoveWindow(&rc);
	m_pageVip.MoveWindow(&rc);

	//把对话框对象指针保存起来
	m_pDialog[0] = &m_pageField;
	m_pDialog[1] = &m_pageFood;
	m_pDialog[2] = &m_pageVip;

	//显示初始页面
	m_pDialog[0]->ShowWindow(SW_SHOW);
	m_pDialog[1]->ShowWindow(SW_HIDE);
	m_pDialog[2]->ShowWindow(SW_HIDE);
	//保存当前选择
	m_CurSelTab = 0;

	SetTimer(1, 1000, NULL);

	CString sDate;
	CTime Today=CTime::GetCurrentTime();
	sDate=Today.Format("%Y年%m月%d日");
	int week=Today.GetDayOfWeek();//获取星期几，1为星期天，7为星期六
	switch(week)
	{
	case 1:{sDate=sDate+"  星期日";break;}
	case 2:{sDate=sDate+"  星期一";break;}
	case 3:{sDate=sDate+"  星期二";break;}
	case 4:{sDate=sDate+"  星期三";break;}
	case 5:{sDate=sDate+"  星期四";break;}
	case 6:{sDate=sDate+"  星期五";break;}
	case 7:{sDate=sDate+"  星期六";break;}
	} 
	CTime tm = CTime::GetCurrentTime();
	CString str = tm.Format("%H:%M:%S");

	SetDlgItemText(IDC_TIME,sDate); 
	SetDlgItemText(IDC_WEEK,str);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMainDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//把当前的页面隐藏起来
	m_pDialog[m_CurSelTab]->ShowWindow(SW_HIDE);
	//得到新的页面索引
	m_CurSelTab = m_tab.GetCurSel();
	//把新的页面显示出来
	m_pDialog[m_CurSelTab]->ShowWindow(SW_SHOW);
	*pResult = 0;

}

void CMainDlg::OnTimer(UINT nIDEvent)   //显示本地时间
{
	// TODO: Add your message handler code here and/or call default
	CString sDate;
	CTime Today=CTime::GetCurrentTime();
	sDate=Today.Format("%Y年%m月%d日");
	int week=Today.GetDayOfWeek();//获取星期几，1为星期天，7为星期六
	switch(week)
	{
	case 1:{sDate=sDate+"  星期日";break;}
	case 2:{sDate=sDate+"  星期一";break;}
	case 3:{sDate=sDate+"  星期二";break;}
	case 4:{sDate=sDate+"  星期三";break;}
	case 5:{sDate=sDate+"  星期四";break;}
	case 6:{sDate=sDate+"  星期五";break;}
	case 7:{sDate=sDate+"  星期六";break;}
	} 
	CTime tm = CTime::GetCurrentTime();
	CString str = tm.Format("%H:%M:%S");

	SetDlgItemText(IDC_TIME,sDate); 
	SetDlgItemText(IDC_WEEK,str);
	CDialog::OnTimer(nIDEvent);
}
