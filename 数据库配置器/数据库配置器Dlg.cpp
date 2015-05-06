
// ���ݿ�������Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "���ݿ�������.h"
#include "���ݿ�������Dlg.h"
#include <mysql.h>
#include "sql.h"
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
END_MESSAGE_MAP()


// C���ݿ�������Dlg �Ի���




C���ݿ�������Dlg::C���ݿ�������Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(C���ݿ�������Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C���ݿ�������Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strHost);
	DDX_Text(pDX, IDC_EDIT2, m_nPort);
	DDX_Text(pDX, IDC_EDIT3, m_strUserName);
	DDX_Text(pDX, IDC_EDIT4, m_strPwd);
	DDX_Text(pDX, IDC_EDIT5, m_strDBName);
}

BEGIN_MESSAGE_MAP(C���ݿ�������Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_NEWDB, &C���ݿ�������Dlg::OnBnClickedBtnNewDB)
	ON_BN_CLICKED(ID_TEST, &C���ݿ�������Dlg::OnBnClickedTest)
	ON_BN_CLICKED(IDOK, &C���ݿ�������Dlg::OnBnClickedBtnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// C���ݿ�������Dlg ��Ϣ�������

BOOL C���ݿ�������Dlg::OnInitDialog()
{
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

	m_pConfig = GetDataBaseData;
	DatabaseInfo info;
	m_pConfig->Read(info);
	m_strDBName = info.m_strDBName;
	m_strHost = info.m_strHost;
	m_nPort = info.m_nPort;
	m_strUserName = info.m_strUserName;
	m_strPwd = info.m_strPwd;
	UpdateData(FALSE);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void C���ݿ�������Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void C���ݿ�������Dlg::OnPaint()
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
HCURSOR C���ݿ�������Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

afx_msg void C���ݿ�������Dlg::OnBnClickedBtnNewDB()
{
	UpdateData(TRUE);
	if (m_strHost.IsEmpty() || m_strDBName.IsEmpty() || m_strUserName.IsEmpty() || m_strPwd.IsEmpty() || m_nPort <= 0)
	{
		MessageBox("������Ϣ��Ϊ���������Ϊ��!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	MYSQL m_Conn;
	mysql_init(&m_Conn);
	if(mysql_real_connect(&m_Conn,m_strHost,m_strUserName,m_strPwd,NULL,m_nPort,NULL,0))
	{
		mysql_query(&m_Conn, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
	}
	else
	{
		MessageBox("IP,�˿�,�û���,���룺����!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if (mysql_select_db(&m_Conn, m_strDBName) == 0)
	{
		if (MessageBox(_T("���ݿ��Ѵ��ڣ��Ƿ��ؽ���"), _T("��ʾ"), MB_YESNO|MB_ICONINFORMATION|MB_TASKMODAL) != IDYES)
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
	mysql_query(&m_Conn, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
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

	MessageBox("���ݿ⽨�����!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	mysql_close(&m_Conn);

}

afx_msg void C���ݿ�������Dlg::OnBnClickedTest()
{
	UpdateData(TRUE);
	if (m_strHost.IsEmpty() || m_strDBName.IsEmpty() || m_strUserName.IsEmpty() || m_strPwd.IsEmpty() || m_nPort <= 0)
	{
		MessageBox("������Ϣ��Ϊ���������Ϊ��!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	MYSQL m_Conn;
	mysql_init(&m_Conn);
	if(mysql_real_connect(&m_Conn,m_strHost,m_strUserName,m_strPwd,NULL,m_nPort,NULL,0))
	{
		mysql_query(&m_Conn, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����

		MessageBox("IP,�˿�,�û���,���룺��ȷ!", "��ʾ", MB_ICONINFORMATION|MB_TASKMODAL);
	}
	else
	{
		MessageBox("IP,�˿�,�û���,���룺����!", "��ʾ", MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (mysql_select_db(&m_Conn, m_strDBName) == 0)
	{
		MessageBox("���ݿ�������ȷ!", "��ʾ", MB_ICONINFORMATION|MB_TASKMODAL);
	}
	else
	{
		MessageBox("���ݿ�����������!", "��ʾ", MB_ICONINFORMATION|MB_TASKMODAL);
		mysql_close(&m_Conn);
		return;
	}
	MessageBox("��ϲ�㣡���óɹ�!", "��ʾ", MB_ICONINFORMATION|MB_TASKMODAL);
	mysql_close(&m_Conn);
	
}

afx_msg void C���ݿ�������Dlg::OnBnClickedBtnOk()
{
	UpdateData(TRUE);
	if (m_strHost.IsEmpty() || m_strDBName.IsEmpty() || m_strUserName.IsEmpty() || m_strPwd.IsEmpty() || m_nPort <= 0)
	{
		MessageBox("������Ϣ��Ϊ���������Ϊ��!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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