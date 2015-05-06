// DlgVipHistory.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgVipHistory.h"

#include "DlgFindVip.h"
#include "history.h"
#include "MainDlg.h"
#include "BasicExcel.hpp"
using namespace YExcel;
// CDlgVipHistory �Ի���

IMPLEMENT_DYNAMIC(CDlgVipHistory, CDialog)

CDlgVipHistory::CDlgVipHistory(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgVipHistory::IDD, pParent)
	, m_strVipID(_T(""))
	, m_timeStart(COleDateTime::GetCurrentTime())
	, m_timeStop(COleDateTime::GetCurrentTime())
{

}

CDlgVipHistory::~CDlgVipHistory()
{
}

void CDlgVipHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_timeStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_timeStop);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_COMBO1, m_comboVip);
}


BEGIN_MESSAGE_MAP(CDlgVipHistory, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgVipHistory::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgVipHistory::OnBnClickedButton2)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CDlgVipHistory::OnDtnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CDlgVipHistory::OnDtnDatetimechangeDatetimepicker2)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &CDlgVipHistory::OnCbnEditchangeCombo1)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &CDlgVipHistory::OnBnClickedBtnOutput)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgVipHistory::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlgVipHistory ��Ϣ�������

void CDlgVipHistory::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgFindVip dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strVipID = dlg.m_strResult;
		CMainDlg* pMainWnd = GETMAINWND;
		UpdateData(FALSE);
	}
}

BOOL CDlgVipHistory::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_list.InsertColumn(k++, _T("��ԱID"),	LVCFMT_CENTER, 80);
	m_list.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 200);
	m_list.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 200);
	m_list.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 200);
	m_list.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);
	m_list.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 60);
	m_list.InsertColumn(k++, _T("�ܶ�"),	LVCFMT_CENTER, 100);
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
	ShowAllVip();
	return TRUE;
}

#include<io.h>
#include<stdio.h>
void CDlgVipHistory::FindHistory()
{
	m_list.DeleteAllItems();
	if (DATA_FROM_TYPE == DATA_FROM_MYSQL)
	{
		CHisData* pVipHisData = GetHistoryData;
		pVipHisData->GetAllData(fastdelegate::MakeDelegate(this, &CDlgVipHistory::Hook), m_timeStart, m_timeStop);
		return;

	}
	BOOL bFindAll = FALSE;
	if (m_strVipID.IsEmpty())
	{
		bFindAll = TRUE;
	}
	CArray<CString> ayFile;
	CString strPath = TL_GetModulePath(NULL) + "viphistory" + PATHSPLITSTRING;
	CString strDir = strPath + "*";
	long Handle;
	struct _finddata_t FileInfo;
	if((Handle=_findfirst(strDir,&FileInfo))!=-1L)
	{
		while( _findnext(Handle,&FileInfo)==0)
		{
			CString strName = FileInfo.name;
			if (strName != "." &&  strName != "..")
			{
				ayFile.Add(strPath + strName);
			}
		}
		_findclose(Handle);
	}

	int n = ayFile.GetSize();
	for (int i = 0; i<n;i++)
	{
		CHisData* pVipHisData = NULL;
		if (DATA_FROM_TYPE == DATA_FROM_XML)
		{
			pVipHisData = new CHisDataXML(ayFile[i]);
		}
		else
		{
			pVipHisData = NULL;
			ASSERT(FALSE && "������Դ��֧�֣�");
			return;
		}
		pVipHisData->GetAllData(fastdelegate::MakeDelegate(this, &CDlgVipHistory::Hook), m_timeStart, m_timeStop);
		delete pVipHisData; 
	}
}

void CDlgVipHistory::Hook(int nType, void* pData)
{
	CMainDlg* pMain = GETMAINWND;

	if (nType == HIS_TYPE_CHARGE)
	{
		//m_list.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 100);
		//m_list.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 60);
		//m_list.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 120);
		//m_list.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);
		//m_list.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);


		HisChargeInfo* p = (HisChargeInfo*)pData;

		VipInfo* pVip = pMain->m_pageVip.GetVipInfo(p->m_strVipID);
		if (!pVip || pVip->m_strLevelID == VIP_LEVEL_0)
		{
			//ɾ���Ļ�Ա����ʾ
			return;
		}

		if (!m_strVipID.IsEmpty())
		{
			if (p->m_strVipID != m_strVipID)
			{
				return;
			}
		}
		int j = 0;
		int i = m_list.GetItemCount();

		m_list.InsertItem(i, p->m_strVipID);

		CString strTime;
		strTime.Format("%s-%s", p->m_time.m_strDate.GetBuffer(), p->m_time.m_strTime.GetBuffer());

		m_list.SetItemText(i, ++j, strTime);
		m_list.SetItemText(i, ++j, "��ֵ");
		m_list.SetItemText(i, ++j, "��");
		m_list.SetItemText(i, ++j, p->m_strMoney);
		m_list.SetItemText(i, ++j, "1");
		m_list.SetItemText(i, ++j, p->m_strMoney);//�ܶ�

	}
	else if (nType == HIS_TYPE_BUYFOOD)
	{
		HisBuyFoodInfo* p = (HisBuyFoodInfo*)pData;

		VipInfo* pVip = pMain->m_pageVip.GetVipInfo(p->m_strVipID);
		if (!pVip  || pVip->m_strLevelID == VIP_LEVEL_0)
		{
			//ɾ���Ļ�Ա����ʾ
			return;
		}

		if (!m_strVipID.IsEmpty())
		{
			if (p->m_strVipID != m_strVipID)
			{
				return;
			}
		}
		int j = 0;
		int i = m_list.GetItemCount();

		m_list.InsertItem(i, p->m_strVipID);

		CString strTime;
		strTime.Format("%s-%s", p->m_time.m_strDate.GetBuffer(), p->m_time.m_strTime.GetBuffer());

		m_list.SetItemText(i, ++j, strTime);


		m_list.SetItemText(i, ++j, "������Ʒ");

		FoodInfo* pFood = pMain->m_pageFood.GetFoodInfo(p->m_strFoodID);
		if (pFood == NULL)
		{
			m_list.SetItemText(i, ++j, p->m_strFoodName);
		}
		else
		{
			m_list.SetItemText(i, ++j, pFood->m_strName);
		}
		
		m_list.SetItemText(i, ++j, p->m_strOnePrice);
		m_list.SetItemText(i, ++j, p->m_strCount);
		CString strTotal;
		strTotal.Format("%d", atoi(p->m_strOnePrice)*atoi(p->m_strCount));
		m_list.SetItemText(i, ++j, strTotal);
	}
	else if (nType == HIS_TYPE_FIELD)
	{
		HisBuyFieldInfo* p = (HisBuyFieldInfo*)pData;

		VipInfo* pVip = pMain->m_pageVip.GetVipInfo(p->m_strVipID);
		if (!pVip  || pVip->m_strLevelID == VIP_LEVEL_0)
		{
			//ɾ���Ļ�Ա����ʾ
			return;
		}

		if (!m_strVipID.IsEmpty())
		{
			if (p->m_strVipID != m_strVipID)
			{
				return;
			}
		}

		int j = 0;
		int i = m_list.GetItemCount();

		m_list.InsertItem(i, p->m_strVipID);

		CString strTime;
		strTime.Format("%s-%s", p->m_time.m_strDate.GetBuffer(), p->m_time.m_strTime.GetBuffer());

		m_list.SetItemText(i, ++j, strTime);
		m_list.SetItemText(i, ++j, "��");
		CString strName;
		strName.Format("%s(%s)", p->m_strFieldID.GetBuffer(), p->m_strTime.GetBuffer());
		m_list.SetItemText(i, ++j, strName);
		m_list.SetItemText(i, ++j, p->m_strMoney); 
		m_list.SetItemText(i, ++j, "1");
		m_list.SetItemText(i, ++j, p->m_strMoney);
	}
}

void CDlgVipHistory::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString strDate;
	strDate.Format("%04d%02d%02d", m_timeStart.GetYear(), m_timeStart.GetMonth(), m_timeStart.GetDay());
	int nDateStart = atoi(strDate);

	strDate.Format("%04d%02d%02d", m_timeStop.GetYear(), m_timeStop.GetMonth(), m_timeStop.GetDay());
	int nDateStop = atoi(strDate);
	if (nDateStart > nDateStop)
	{
		GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
		return;
	}

	char chStr[100] = {0};
	CString str;
	GetDlgItemText(IDC_COMBO1, str);
	int n = str.Find(' ', 0);
	m_strVipID = str.Left(n);
	if (str == "ȫ��")
	{
		m_strVipID = "";
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

void CDlgVipHistory::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_list.DeleteAllItems();
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
}

void CDlgVipHistory::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_list.DeleteAllItems();
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
}

void CDlgVipHistory::ShowAllVip()
{
	m_comboVip.ResetContent();
	m_comboVip.AddString("ȫ��");
	m_comboVip.SetCurSel(0);
	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++,row++)
	{
		CString& strTmpID = ayVip[i].m_strID;
		CString& strTmpName = ayVip[i].m_strName;
		const VipInfo* p = pMainWnd->m_pageVip.GetVipInfo(strTmpID);
 		if (p->m_strLevelID == VIP_LEVEL_0)
 		{
 			continue;
 		}
		CString str;
		str.Format("%s %s", strTmpID.GetBuffer(), strTmpName.GetBuffer());
		m_comboVip.AddString(str);
	}
}
void CDlgVipHistory::OnCbnEditchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strKey;
	GetDlgItemText(IDC_COMBO1, strKey);
	m_comboVip.ResetContent();
	m_comboVip.AddString("ȫ��");

	
	//m_comboVip.ShowDropDown(FALSE);//�ᵼ��������ַ�����ʧ�ٳ���

	if (strKey.IsEmpty())
	{
		ShowAllVip();
		HCURSOR hCur = LoadCursor(NULL , IDC_ARROW);
		::SetCursor(hCur);
		m_comboVip.SetCurSel(-1);
		m_comboVip.ShowDropDown(TRUE);
		return;
	}

	SetDlgItemText(IDC_COMBO1, strKey);
	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++)
	{
		CString& strTmpID = ayVip[i].m_strID;
		CString& strTmpName = ayVip[i].m_strName;
		if (ayVip[i].m_strLevelID == VIP_LEVEL_0)
		{
			continue;
		}

		if (strTmpID.Find(strKey) != -1 || strTmpName.Find(strKey) != -1)
		{
			//�ҵ���
			CString str;
			str.Format("%s %s", strTmpID.GetBuffer(), strTmpName.GetBuffer());
			m_comboVip.AddString(str);
		}
	}


	HCURSOR hCur = LoadCursor(NULL , IDC_ARROW) ;
	::SetCursor(hCur);
	m_comboVip.SetCurSel(-1);
	m_comboVip.ShowDropDown(TRUE);

	SetDlgItemText(IDC_COMBO1, strKey);
	m_comboVip.SetEditSel(strKey.GetLength(),strKey.GetLength());
}

CStringW GetUnicodeString(const char* pSrc)
{
	size_t nLength = strlen(pSrc);
	CStringW str;
	mbstowcs(str.GetBuffer(nLength + 1), pSrc, nLength);
	str.GetBuffer()[nLength] = 0;
	return str;
}

void CDlgVipHistory::OnBnClickedBtnOutput()
{
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
	strTitle = "ʱ��,���,����,����,����,�ܶ�\n";
	strContent += strTitle;
	int nCount = m_list.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		CString strOne;
		for (int j = 0; j < 6; ++j)
		{
			strOne +=m_list.GetItemText(i,j);
			if (j != 5)
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

void CDlgVipHistory::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_list.DeleteAllItems();
	UpdateData(TRUE);
	GetDlgItem(IDC_BTN_OUTPUT)->EnableWindow(FALSE);
}
