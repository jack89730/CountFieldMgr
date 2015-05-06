
// 数据库配置器Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "数据库配置器.h"
#include "数据库配置器Dlg.h"
#include <mysql.h>
#include "sql.h"
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
END_MESSAGE_MAP()


// C数据库配置器Dlg 对话框




C数据库配置器Dlg::C数据库配置器Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C数据库配置器Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C数据库配置器Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strHost);
	DDX_Text(pDX, IDC_EDIT2, m_nPort);
	DDX_Text(pDX, IDC_EDIT3, m_strUserName);
	DDX_Text(pDX, IDC_EDIT4, m_strPwd);
	DDX_Text(pDX, IDC_EDIT5, m_strDBName);
}

BEGIN_MESSAGE_MAP(C数据库配置器Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_NEWDB, &C数据库配置器Dlg::OnBnClickedBtnNewDB)
	ON_BN_CLICKED(ID_TEST, &C数据库配置器Dlg::OnBnClickedTest)
	ON_BN_CLICKED(IDOK, &C数据库配置器Dlg::OnBnClickedBtnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// C数据库配置器Dlg 消息处理程序

BOOL C数据库配置器Dlg::OnInitDialog()
{
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

	m_pConfig = GetDataBaseData;
	DatabaseInfo info;
	m_pConfig->Read(info);
	m_strDBName = info.m_strDBName;
	m_strHost = info.m_strHost;
	m_nPort = info.m_nPort;
	m_strUserName = info.m_strUserName;
	m_strPwd = info.m_strPwd;
	UpdateData(FALSE);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void C数据库配置器Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C数据库配置器Dlg::OnPaint()
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
HCURSOR C数据库配置器Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

afx_msg void C数据库配置器Dlg::OnBnClickedBtnNewDB()
{
	UpdateData(TRUE);
	if (m_strHost.IsEmpty() || m_strDBName.IsEmpty() || m_strUserName.IsEmpty() || m_strPwd.IsEmpty() || m_nPort <= 0)
	{
		MessageBox("所有信息都为必填项，不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	MYSQL m_Conn;
	mysql_init(&m_Conn);
	if(mysql_real_connect(&m_Conn,m_strHost,m_strUserName,m_strPwd,NULL,m_nPort,NULL,0))
	{
		mysql_query(&m_Conn, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文
	}
	else
	{
		MessageBox("IP,端口,用户名,密码：错误!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (mysql_select_db(&m_Conn, m_strDBName) == 0)
	{
		if (MessageBox(_T("数据库已存在，是否重建！"), _T("提示"), MB_YESNO|MB_ICONINFORMATION|MB_TASKMODAL) != IDYES)
		{
			mysql_close(&m_Conn);
			return;
		}
		else
		{
			CString strDrop;
			strDrop.Format("drop database %s", m_strDBName.GetBuffer());
			int ret = mysql_query(&m_Conn,strDrop.GetBuffer());
		}
	}

	CString strSql;
	strSql.Format("create database %s character set gbk;", m_strDBName.GetBuffer());
	int ret = 0;
	ret = mysql_query(&m_Conn,strSql.GetBuffer());
	mysql_select_db(&m_Conn, m_strDBName);
	mysql_query(&m_Conn, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文
	ret = mysql_query(&m_Conn,strSql.GetBuffer());


	ret = mysql_query(&m_Conn,CREATE_VIP);
	ret = mysql_query(&m_Conn,CREATE_FOOD);
	ret = mysql_query(&m_Conn,CREATE_USER);
	ret = mysql_query(&m_Conn,INSERT_ADMIN);
	ret = mysql_query(&m_Conn,CREATE_LEVEL);
	ret = mysql_query(&m_Conn,INSERT_LEVEL);
	ret = mysql_query(&m_Conn,CREATE_PERIOD);
	ret = mysql_query(&m_Conn,INSERT_PERIOD);
	ret = mysql_query(&m_Conn,CREATE_HISTORY);
	ret = mysql_query(&m_Conn,CREATE_FIELD);
	ret = mysql_query(&m_Conn,CREATE_TIMEINDEX);
	ret = mysql_query(&m_Conn,CREATE_BOOKINFO);
	ret = mysql_query(&m_Conn,CREATE_OPERATE);

	MessageBox("数据库建立完成!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	mysql_close(&m_Conn);

}

afx_msg void C数据库配置器Dlg::OnBnClickedTest()
{
	UpdateData(TRUE);
	if (m_strHost.IsEmpty() || m_strDBName.IsEmpty() || m_strUserName.IsEmpty() || m_strPwd.IsEmpty() || m_nPort <= 0)
	{
		MessageBox("所有信息都为必填项，不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	MYSQL m_Conn;
	mysql_init(&m_Conn);
	if(mysql_real_connect(&m_Conn,m_strHost,m_strUserName,m_strPwd,NULL,m_nPort,NULL,0))
	{
		mysql_query(&m_Conn, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文

		MessageBox("IP,端口,用户名,密码：正确!", "提示", MB_ICONINFORMATION|MB_TASKMODAL);
	}
	else
	{
		MessageBox("IP,端口,用户名,密码：错误!", "提示", MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (mysql_select_db(&m_Conn, m_strDBName) == 0)
	{
		MessageBox("数据库名：正确!", "提示", MB_ICONINFORMATION|MB_TASKMODAL);
	}
	else
	{
		MessageBox("数据库名：不存在!", "提示", MB_ICONINFORMATION|MB_TASKMODAL);
		mysql_close(&m_Conn);
		return;
	}
	MessageBox("恭喜你！配置成功!", "提示", MB_ICONINFORMATION|MB_TASKMODAL);
	mysql_close(&m_Conn);
	
}

afx_msg void C数据库配置器Dlg::OnBnClickedBtnOk()
{
	UpdateData(TRUE);
	if (m_strHost.IsEmpty() || m_strDBName.IsEmpty() || m_strUserName.IsEmpty() || m_strPwd.IsEmpty() || m_nPort <= 0)
	{
		MessageBox("所有信息都为必填项，不能为空!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	DatabaseInfo info;
	info.m_strHost = m_strHost;
	info.m_nPort = m_nPort;
	info.m_strDBName = m_strDBName;
	info.m_strUserName = m_strUserName;
	info.m_strPwd = m_strPwd;
	m_pConfig->Set(info);
	CDialog::OnOK();
}