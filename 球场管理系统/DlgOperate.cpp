// DlgOperate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgOperate.h"
#include "MainDlg.h"
#include "app.h"


// CDlgOperate �Ի���

IMPLEMENT_DYNAMIC(CDlgOperate, CDialog)

CDlgOperate::CDlgOperate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOperate::IDD, pParent)
	, m_StartTime(COleDateTime::GetCurrentTime())
	, m_StopTime(COleDateTime::GetCurrentTime())
{

}

CDlgOperate::~CDlgOperate()
{
}

void CDlgOperate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_StopTime);
	DDX_Control(pDX, IDC_COMBO1, m_comboUsers);
	DDX_Control(pDX, IDC_LIST2, m_list);
}


BEGIN_MESSAGE_MAP(CDlgOperate, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgOperate::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &CDlgOperate::OnBnClickedBtnOutput)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgOperate::OnCbnSelchangeCombo1)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &CDlgOperate::OnCbnEditchangeCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CDlgOperate::OnCbnKillfocusCombo1)
END_MESSAGE_MAP()


// CDlgOperate ��Ϣ�������

void CDlgOperate::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strDate;
	strDate.Format("%04d%02d%02d", m_StartTime.GetYear(), m_StartTime.GetMonth(), m_StartTime.GetDay());
	int nDateStart = atoi(strDate);

	strDate.Format("%04d%02d%02d", m_StopTime.GetYear(), m_StopTime.GetMonth(), m_StopTime.GetDay());
	int nDateStop = atoi(strDate);
	if (nDateStart > nDateStop)
	{
		GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
		return;
	}

	GetDlgItemText(IDC_COMBO1, m_strUserName);
	if (m_strUserName == "ȫ��")
	{
		m_strUserName = "";
	}
	
	FindHistory();
	if (m_list.GetItemCount())
	{
		GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
	}

}

BOOL CDlgOperate::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_list.InsertColumn(k++, _T("�û���"),	LVCFMT_CENTER, 120);
	m_list.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 180);
	m_list.InsertColumn(k++, _T("��������"),	LVCFMT_CENTER, 150);
	m_list.InsertColumn(k++, _T("��������"),	LVCFMT_CENTER, 900);

	ShowAllUser();

	return TRUE;
}

void CDlgOperate::Hook(HisOperateInfo* pData)
{
	int j = 0;
	int i = m_list.GetItemCount();

	m_list.InsertItem(i, pData->m_strName);

	m_list.SetItemText(i, ++j, pData->m_addTime);
	m_list.SetItemText(i, ++j, pData->m_strType);
	m_list.SetItemText(i, ++j, pData->m_strInfo);
}

void CDlgOperate::FindHistory()
{
	m_list.DeleteAllItems();
	if (DATA_FROM_TYPE == DATA_FROM_MYSQL)
	{
		CHisOperateData* pHisData = GetOperateData;
		pHisData->GetAllData(fastdelegate::MakeDelegate(this, &CDlgOperate::Hook), m_StartTime, m_StopTime, m_strUserName);
	}
}


void CDlgOperate::ShowAllUser()
{
	m_ayUsers.RemoveAll();
	m_comboUsers.ResetContent();
	m_comboUsers.AddString("ȫ��");
	m_comboUsers.SetCurSel(0);
	CMainDlg* pMainWnd = GETMAINWND;

	CUserData* pUser = GetUserData;
	pUser->GetAllData(m_ayUsers);
	int nCount = m_ayUsers.GetSize();
	int i = 0;
	BOOL bAdmin = ISADMIN;
	CString strUserName = GETUSERNAME;

	int row = 0;
	for (int i = 0; i < nCount; i++,row++)
	{
		m_comboUsers.AddString(m_ayUsers[i].m_strName);
	}
}

void CDlgOperate::OnBnClickedBtnOutput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(FALSE,_T("*.csv"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"csv file (*.csv)|*.csv||",this);
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	CString strFile = fileDlg.GetFileName();
	CString strPath = fileDlg.GetFolderPath();
	CString strSaveFile = strPath + PATHSPLITSTRING + strFile;


	CString strContent;
	CString strTitle;
	strTitle = "�û���,ʱ��,��������,��������\n";
	strContent += strTitle;
	int nCount = m_list.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		CString strOne;
		for (int j = 0; j < 4; ++j)
		{
			strOne +=m_list.GetItemText(i,j);
			if (j != 3)
			{
				strOne += ",";
			}
			else
			{
				strOne += "\n";
			}
		}
		strContent += strOne;
	}

	FILE *fp;
	fp=fopen(strSaveFile.GetBuffer(),"w+");
	fputs(strContent.GetBuffer(), fp);
	fclose(fp);

}

void CDlgOperate::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
}

void CDlgOperate::OnCbnEditchangeCombo1()
{
	CString strKey;
	GetDlgItemText(IDC_COMBO1, strKey);
	m_comboUsers.ResetContent();
	m_comboUsers.AddString("ȫ��");


	//m_comboVip.ShowDropDown(FALSE);//�ᵼ��������ַ�����ʧ�ٳ���

	if (strKey.IsEmpty())
	{
		ShowAllUser();
		HCURSOR hCur = LoadCursor(NULL , IDC_ARROW);
		::SetCursor(hCur);
		m_comboUsers.SetCurSel(-1);
		m_comboUsers.ShowDropDown(TRUE);
		return;
	}

	SetDlgItemText(IDC_COMBO1, strKey);
	CMainDlg* pMainWnd = GETMAINWND;

	USER_LIST ayUsers;
	m_ayUsers.RemoveAll();
	CUserData* pUser = GetUserData;
	pUser->GetAllData(ayUsers);

	int nCount = ayUsers.GetSize();
	int row = 0;
	for (int i = 0; i < nCount; i++)
	{
		CString& strTmpName = ayUsers[i].m_strName;

		if (strTmpName.Find(strKey) != -1)
		{
			//�ҵ���
			m_comboUsers.AddString(strTmpName);
			m_ayUsers.Add(ayUsers[i]);
		}
	}

	HCURSOR hCur = LoadCursor(NULL , IDC_ARROW) ;
	::SetCursor(hCur);
	m_comboUsers.SetCurSel(-1);
	//m_comboUsers.ShowDropDown(TRUE);

	SetDlgItemText(IDC_COMBO1, strKey);
	m_comboUsers.SetEditSel(strKey.GetLength(),strKey.GetLength());
}

void CDlgOperate::OnCbnKillfocusCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
