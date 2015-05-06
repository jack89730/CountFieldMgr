// DlgBook.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgBook.h"
#include "MainDlg.h"
#include "xmlbase.h"
#include "public.h"
#include "DlgFindVip.h"
#include "DlgCharge.h"
#include "operatedata.h"
#include "app.h"

// CDlgBook �Ի���

IMPLEMENT_DYNAMIC(CDlgBook, CDialog)

CDlgBook::CDlgBook(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBook::IDD, pParent)
	, m_strVipID(_T(""))
	, m_strFieldID(_T(""))
	, m_nYajin(0)
	, m_strTmpName(_T(""))
	, m_strTmpPhone(_T(""))
	, m_periodStop(COleDateTime::GetCurrentTime())
	, m_strPrice(_T(""))
{

}

CDlgBook::~CDlgBook()
{
}

void CDlgBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FIELD, m_strFieldName);
	DDX_Control(pDX, IDC_COMBO_START, m_comboStart);
	DDX_Control(pDX, IDC_COMBO_STOP, m_comboStop);
	DDX_Control(pDX, IDC_COMBO1, m_comboVip);
	DDX_Text(pDX, IDC_EDIT_YAJIN, m_nYajin);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strTmpName);
	DDX_Text(pDX, IDC_EDIT_PHONE, m_strTmpPhone);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_periodStop);
	DDX_Text(pDX, IDC_PRICE, m_strPrice);
	DDX_Control(pDX, IDC_COMBO3, m_comboPeriod);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DateTimePicker);
}


BEGIN_MESSAGE_MAP(CDlgBook, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgBook::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgBook::OnBnClickedButton1)
//	ON_CBN_EDITCHANGE(IDC_COMBO1, &CDlgBook::OnCbnEditchangeCombo1)
ON_CBN_EDITCHANGE(IDC_COMBO1, &CDlgBook::OnCbnEditchangeCombo1)
ON_BN_CLICKED(IDC_RADIO1, &CDlgBook::OnBnClickedRadio1)
ON_BN_CLICKED(IDC_RADIO2, &CDlgBook::OnBnClickedRadio2)
//ON_CBN_DROPDOWN(IDC_COMBO1, &CDlgBook::OnCbnDropdownCombo1)
ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgBook::OnCbnSelchangeCombo1)
ON_BN_CLICKED(IDC_RADIO3, &CDlgBook::OnBnClickedRadio3)
ON_BN_CLICKED(IDC_RADIO4, &CDlgBook::OnBnClickedRadio4)
ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgBook::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// CDlgBook ��Ϣ�������

void CDlgBook::OnBnClickedOk()
{
	m_bSuccess = FALSE;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	BOOL bVip = ((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();//ѡ
	BOOL bAutoPrice = ((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck();//�Ƿ��ֶ��Ƽ�

	CMainDlg* pMainWnd = GETMAINWND;

	if (bVip)
	{
		//GetDlgItemText(IDC_COMBO1, m_strVipID);
		if (m_strVipID.IsEmpty())
		{
			MessageBox("��ԱID����Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		//�����Ƿ���ڸû�Ա

		int nVipCount = pMainWnd->m_pageVip.m_ayVip.GetCount();

		BOOL bFind = FALSE;
		int i = 0;

		for (; i < nVipCount; ++i)
		{
			if (pMainWnd->m_pageVip.m_ayVip[i].m_strID == m_strVipID)
			{
				bFind = TRUE;
				break;
			}
		}

		if (!bFind)
		{
			MessageBox("��ԱID�����ڣ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		if (pMainWnd->m_pageVip.m_ayVip[i].m_bLock)
		{
			MessageBox("�û�Ա�ѱ����������Ƚ�����", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	else
	{
		//20140626����Ϊ�ǻ�Ա������ֱ�ӱ�ɻ�Ա�����Զ�ע���Ա���ȼ�Ϊ�ǻ�Ա
		if (m_strTmpName.IsEmpty() || m_strTmpPhone.IsEmpty())
		{
			MessageBox("����д�������ֻ���", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}

		VipInfo info;
		int id = pMainWnd->m_pageVip.NewID();
		info.m_strID.Format("%d", id);

		info.m_strLevelID = VIP_LEVEL_0;
		m_strVipID = info.m_strID;
		info.m_strName = m_strTmpName;
		info.m_strPhone = m_strTmpPhone;
		info.m_strMoney.Format("%d", m_nYajin);
		pMainWnd->m_pageVip.m_pVipData->AddVip(info);
		pMainWnd->m_pageVip.ShowItemVip();
	}

	//Ԥ��ʱ���
	int nIndexStart = m_ayIndex[m_comboStart.GetCurSel()];
	int nIndexStop = m_ayIndex[m_comboStop.GetCurSel()];

	//�������Ԥ��60��
	CTime nowTime(time(NULL));
	CTime stopTime(m_periodStop.GetYear(), m_periodStop.GetMonth(), m_periodStop.GetDay(), m_periodStop.GetHour(), m_periodStop.GetMinute(), m_periodStop.GetSecond());
	//CTimeSpan subTime = stopTime - nowTime;
	//int nDay = subTime.GetDays();
	//if (nDay < 0 || nDay > 60)
	//{
	//	MessageBox("����Ԥ�����֧��60�죡", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	//	return;
	//}

	int nAdd = 0;
	if (m_comboPeriod.GetCurSel() == 1)
	{
		//everyday
		nAdd = 1;
	}
	else
	{
		//weekly
		nAdd = 7;
	}

	VipInfo* pVip = pMainWnd->m_pageVip.GetVipInfo(m_strVipID);

	//ͳ��һ��һ������Ǯ
	int nTotalMoney = 0;
	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		CString strDay;
		CTime tmpNowTime = nowTime;
		strDay.Format("%04d%02d%02d", tmpNowTime.GetYear(), tmpNowTime.GetMonth(), tmpNowTime.GetDay());
		int nNowDate = atoi(strDay);
		strDay.Format("%04d%02d%02d", stopTime.GetYear(), stopTime.GetMonth(), stopTime.GetDay());
		int nStopDate = atoi(strDay);
		
		while(nNowDate <= nStopDate)
		{
			CString strDay;
			strDay.Format("%04d%02d%02d", tmpNowTime.GetYear(), tmpNowTime.GetMonth(), tmpNowTime.GetDay());

			CString strPrice;
			if (bAutoPrice)
			{
				strPrice = pMainWnd->m_pageMgr.GetPriceByLevelAndTime(pVip->m_strLevelID, strDay, i);
			}
			else
			{
				strPrice = m_strPrice;
			}

			nTotalMoney += atoi(strPrice);
			CTimeSpan tmp(nAdd, 0, 0, 0);//����
			tmpNowTime += tmp;

			strDay.Format("%04d%02d%02d", tmpNowTime.GetYear(), tmpNowTime.GetMonth(), tmpNowTime.GetDay());
			nNowDate = atoi(strDay);
		}
	}
	if (bVip && atoi(pVip->m_strMoney) < nTotalMoney)
	{
		CString str;
		str.Format("Ӧ��Ѻ��%dԪ\n��%sԪ\n���㣬���ֵ%dԪ��", nTotalMoney, pVip->m_strMoney.GetBuffer()
			,nTotalMoney - atoi(pVip->m_strMoney));
		if(MessageBox(str,"��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
			return;

		CDlgCharge dlg;
		dlg.m_strID		= pVip->m_strID;
		dlg.DoModal();
		return;
	}


	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		OneTimeBookInfo info;

		OneTimeBookInfo* pinfo = pMainWnd->m_pageField.GetFieldBookInfo(m_strFieldID, i);
		if (!pinfo)
		{
			pMainWnd->m_pageField.AddFieldDay(m_strFieldID);
			pinfo = pMainWnd->m_pageField.GetFieldBookInfo(m_strFieldID, i);
		}
		info =  *pinfo;

		//������Ϣ
		info.m_bBusy = FIELD_BUSY_DATA;
		info.m_strVipID = m_strVipID;

		//Ԥ���ļ۸�
		if (bAutoPrice)
		{
			info.m_bAutoPrice = TRUE;
			info.m_strPrice = "0";
		}
		else
		{
			info.m_bAutoPrice = FALSE;
			info.m_strPrice = m_strPrice;
		}


		int nPeriod = m_comboPeriod.GetCurSel();
		if (nPeriod == 0)
		{
			pMainWnd->m_pageField.m_pFieldData->ChangeFieldBookInfo(m_strFieldID, info);
			continue;
		}

		CTime nowTime2(time(NULL));

		CString strDay;
		strDay.Format("%04d%02d%02d", nowTime2.GetYear(), nowTime2.GetMonth(), nowTime2.GetDay());
		int nNowDate = atoi(strDay);
		strDay.Format("%04d%02d%02d", stopTime.GetYear(), stopTime.GetMonth(), stopTime.GetDay());
		int nStopDate = atoi(strDay);

		while(nNowDate <= nStopDate)
		{
			CString strDay;
			strDay.Format("%04d%02d%02d", nowTime2.GetYear(), nowTime2.GetMonth(), nowTime2.GetDay());
			pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(strDay);
			pMainWnd->m_pageField.m_pFieldData->ChangeFieldBookInfo(m_strFieldID, info);
			CTimeSpan tmp(nAdd, 0, 0, 0);//����
			nowTime2 += tmp;

			strDay.Format("%04d%02d%02d", nowTime2.GetYear(), nowTime2.GetMonth(), nowTime2.GetDay());
			nNowDate = atoi(strDay);
		}

	}

	pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

	//������¼
	HisOperateInfo his_op_info;

	CString strField;
	GetDlgItemText(IDC_EDIT_FIELD, strField);
	CString strStart;
	GetDlgItemText(IDC_COMBO_START, strStart);
	CString strStop;
	GetDlgItemText(IDC_COMBO_STOP, strStop);
	CString strCount;
	GetDlgItemText(IDC_COMBO3, strCount);
	CString strEndDate;
	GetDlgItemText(IDC_DATETIMEPICKER1, strEndDate);

	CString strVip;
	if (bVip)
	{
		strVip.Format("��ԱID:%s,����:%s", pVip->m_strID.GetBuffer(), pVip->m_strName.GetBuffer());
	}
	else
	{
		strVip.Format("�ǻ�Ա,����:%s,�绰:%s", m_strTmpName.GetBuffer(), m_strTmpPhone.GetBuffer());
	}
	CString strPrice;
	if (bAutoPrice)
	{
		strPrice = "ϵͳ����";
	}
	else
	{
		strPrice.Format("�ֶ�����:%sԪ", m_strPrice.GetBuffer());
	}

	CString strTime;
	if (m_comboPeriod.GetCurSel() == 0)
	{
		//����
		CTime nowTime(time(NULL));
		strTime.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
	}
	else
	{
		CTime nowTime(time(NULL));
		CString strDay;
		strDay.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
		int nNowDate = atoi(strDay);
		strDay.Format("%04d%02d%02d", stopTime.GetYear(), stopTime.GetMonth(), stopTime.GetDay());
		int nStopDate = atoi(strDay);
		strTime.Format("%d-%d", nNowDate, nStopDate);
	}

	his_op_info.m_strInfo.Format("%s,����:%s,ʱ��:%s-%s,����:%s, Ԥ������:%s,����:%s"
		, strVip.GetBuffer()
		, strField.GetBuffer()
		, strStart.GetBuffer() 
		, strStop.GetBuffer() 
		, strCount.GetBuffer()
		, strTime.GetBuffer()
		, strPrice.GetBuffer());
	
	CHisOperateData* pOperate = GetOperateData;
	
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_BOOK_FIELD;
	pOperate->Add(his_op_info);

	OnOK();
}

BOOL CDlgBook::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Ĭ�϶���
	SetDlgItemText(IDC_PRICE, "0");
	GetDlgItem(IDC_PRICE)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);

	m_comboPeriod.SetCurSel(0);

	CString strMsg;
	CMainDlg* pMain = GETMAINWND;
	//PeriodInfo& period = pMain->m_pageMgr.m_periodInfo;
	/*if (period.m_bWeekend)
	{
		CTime nowTime(time(NULL));
		int day = nowTime.GetDayOfWeek();
		if (day == 1 || day == 7)
		{
			strMsg.Format("��������ĩȫ��Ϊ�ƽ�ʱ��");
		}
	}

	if (strMsg.IsEmpty())
	{
		strMsg.Format("�ƽ�ʱ��Ϊ��%d:00-%d:00", INDEX_TO_CLOCK(period.m_nStartIndex), INDEX_TO_CLOCK(period.m_nStopIndex));
	}*/

	GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YAJIN)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);//ѡvip

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
	ShowAllVip();
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(FALSE);
	m_bSuccess = FALSE;

	CTime tCurrentTime= CTime::GetCurrentTime();
	CTimeSpan timespanOneMonth(60,0,0,0); //��������Ϊ��ǰ�����ƺ�60��
	CTime tEndTime = tCurrentTime +timespanOneMonth;
	m_DateTimePicker.SetRange(&tCurrentTime,&tEndTime);
	CString strDate = pMain->m_pageField.GetDate();
	int nYear = 0;
	int nMonth = 0;
	int nDay = 0;
	sscanf(strDate.GetBuffer(), "%04d%02d%02d", &nYear, &nMonth
		, &nDay);
	COleDateTime now;
	now.SetDate(nYear, nMonth, nDay);
	m_DateTimePicker.SetTime(now);
	return TRUE;
}

void CDlgBook::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgFindVip dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strVipID = dlg.m_strResult;
		CMainDlg* pMainWnd = GETMAINWND;
		UpdateData(FALSE);
	}
}


void CDlgBook::ShowAllVip()
{
	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++,row++)
	{
		if (ayVip[i].m_strLevelID == VIP_LEVEL_0)
		{
			//��ʱ�˺Ų���ʾ
			//continue;
		}
		CString str;
		str.Format("%s %s", ayVip[i].m_strID.GetBuffer(), ayVip[i].m_strName.GetBuffer());
		m_comboVip.AddString(str);
	}
}

//void CDlgBook::OnCbnEditchangeCombo1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	UpdateData(TRUE);
//	m_comboVip;
//}

void CDlgBook::OnCbnEditchangeCombo1()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strKey;
	GetDlgItemText(IDC_COMBO1, strKey);

	m_comboVip.ResetContent();
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
		//���ֻ���idƥ�䶼����
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

void CDlgBook::OnBnClickedRadio1()
{
	//��Ա��ʽ
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YAJIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(FALSE);
}

void CDlgBook::OnBnClickedRadio2()
{
	CMainDlg* pMainWnd = GETMAINWND;
	OnePriceInfo* pPrice = pMainWnd->m_pageMgr.GetPriceInfo(VIP_LEVEL_0);
	if (pPrice == NULL)
	{
		MessageBox("����ӻ�Ա�ȼ�0���ǻ�Ա�ȼ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_YAJIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(TRUE);
}

//void CDlgBook::OnCbnDropdownCombo1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	//GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(TRUE);
//}

void CDlgBook::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int nIndex = m_comboVip.GetCurSel();  
	int nCount = m_comboVip.GetCount();  
	CString str;
	if (nIndex != CB_ERR)  
	{  
		m_comboVip.GetLBText(nIndex, str);  
	}  
	int n = str.Find(' ', 0);
	m_strVipID = str.Left(n);
	CMainDlg* pMainWnd = GETMAINWND;
	VipInfo* pVipInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVipID);
	OnePriceInfo* pPrice = pMainWnd->m_pageMgr.GetPriceInfo(pVipInfo->m_strLevelID);
	ASSERT(pPrice);
}

void CDlgBook::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//Ĭ�϶���
	SetDlgItemText(IDC_PRICE, "0");
	GetDlgItem(IDC_PRICE)->EnableWindow(FALSE);
}

void CDlgBook::OnBnClickedRadio4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetDlgItemText(IDC_PRICE, "0");
	GetDlgItem(IDC_PRICE)->EnableWindow(TRUE);
}

void CDlgBook::OnCbnSelchangeCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_comboPeriod.GetCurSel() > 0)
	{
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
	}
}
