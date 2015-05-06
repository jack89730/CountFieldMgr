// DlgBook.cpp : 实现文件
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

// CDlgBook 对话框

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


// CDlgBook 消息处理程序

void CDlgBook::OnBnClickedOk()
{
	m_bSuccess = FALSE;
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	BOOL bVip = ((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck();//选
	BOOL bAutoPrice = ((CButton *)GetDlgItem(IDC_RADIO3))->GetCheck();//是否手动计价

	CMainDlg* pMainWnd = GETMAINWND;

	if (bVip)
	{
		//GetDlgItemText(IDC_COMBO1, m_strVipID);
		if (m_strVipID.IsEmpty())
		{
			MessageBox("会员ID不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		//查找是否存在该会员

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
			MessageBox("会员ID不存在！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		if (pMainWnd->m_pageVip.m_ayVip[i].m_bLock)
		{
			MessageBox("该会员已被锁定，请先解锁！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	else
	{
		//20140626，改为非会员在这里直接变成会员，即自动注册会员，等级为非会员
		if (m_strTmpName.IsEmpty() || m_strTmpPhone.IsEmpty())
		{
			MessageBox("请填写姓名和手机！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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

	//预订时间段
	int nIndexStart = m_ayIndex[m_comboStart.GetCurSel()];
	int nIndexStop = m_ayIndex[m_comboStop.GetCurSel()];

	//最多批量预订60天
	CTime nowTime(time(NULL));
	CTime stopTime(m_periodStop.GetYear(), m_periodStop.GetMonth(), m_periodStop.GetDay(), m_periodStop.GetHour(), m_periodStop.GetMinute(), m_periodStop.GetSecond());
	//CTimeSpan subTime = stopTime - nowTime;
	//int nDay = subTime.GetDays();
	//if (nDay < 0 || nDay > 60)
	//{
	//	MessageBox("周期预订最多支持60天！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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

	//统计一下一共多少钱
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
			CTimeSpan tmp(nAdd, 0, 0, 0);//加天
			tmpNowTime += tmp;

			strDay.Format("%04d%02d%02d", tmpNowTime.GetYear(), tmpNowTime.GetMonth(), tmpNowTime.GetDay());
			nNowDate = atoi(strDay);
		}
	}
	if (bVip && atoi(pVip->m_strMoney) < nTotalMoney)
	{
		CString str;
		str.Format("应付押金：%d元\n余额：%s元\n余额不足，请充值%d元！", nTotalMoney, pVip->m_strMoney.GetBuffer()
			,nTotalMoney - atoi(pVip->m_strMoney));
		if(MessageBox(str,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
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

		//交换信息
		info.m_bBusy = FIELD_BUSY_DATA;
		info.m_strVipID = m_strVipID;

		//预订的价格
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
			CTimeSpan tmp(nAdd, 0, 0, 0);//加天
			nowTime2 += tmp;

			strDay.Format("%04d%02d%02d", nowTime2.GetYear(), nowTime2.GetMonth(), nowTime2.GetDay());
			nNowDate = atoi(strDay);
		}

	}

	pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

	//操作记录
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
		strVip.Format("会员ID:%s,姓名:%s", pVip->m_strID.GetBuffer(), pVip->m_strName.GetBuffer());
	}
	else
	{
		strVip.Format("非会员,姓名:%s,电话:%s", m_strTmpName.GetBuffer(), m_strTmpPhone.GetBuffer());
	}
	CString strPrice;
	if (bAutoPrice)
	{
		strPrice = "系统定价";
	}
	else
	{
		strPrice.Format("手动定价:%s元", m_strPrice.GetBuffer());
	}

	CString strTime;
	if (m_comboPeriod.GetCurSel() == 0)
	{
		//单次
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

	his_op_info.m_strInfo.Format("%s,场地:%s,时间:%s-%s,次数:%s, 预定日期:%s,单价:%s"
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

	//默认定价
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
			strMsg.Format("今日是周末全天为黄金时段");
		}
	}

	if (strMsg.IsEmpty())
	{
		strMsg.Format("黄金时段为：%d:00-%d:00", INDEX_TO_CLOCK(period.m_nStartIndex), INDEX_TO_CLOCK(period.m_nStopIndex));
	}*/

	GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YAJIN)->EnableWindow(FALSE);
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);//选vip

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
	CTimeSpan timespanOneMonth(60,0,0,0); //这里设置为当前日期推后60天
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
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
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
			//临时账号不显示
			//continue;
		}
		CString str;
		str.Format("%s %s", ayVip[i].m_strID.GetBuffer(), ayVip[i].m_strName.GetBuffer());
		m_comboVip.AddString(str);
	}
}

//void CDlgBook::OnCbnEditchangeCombo1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	UpdateData(TRUE);
//	m_comboVip;
//}

void CDlgBook::OnCbnEditchangeCombo1()
{
	
	// TODO: 在此添加控件通知处理程序代码
	CString strKey;
	GetDlgItemText(IDC_COMBO1, strKey);

	m_comboVip.ResetContent();
	//m_comboVip.ShowDropDown(FALSE);//会导致输入框字符先消失再出现

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
		//名字或者id匹配都可以
		if (ayVip[i].m_strLevelID == VIP_LEVEL_0)
		{
			continue;
		}
		if (strTmpID.Find(strKey) != -1 || strTmpName.Find(strKey) != -1)
		{
			//找到了
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
	//会员方式
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox("请添加会员等级0，非会员等级", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_YAJIN)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(TRUE);
}

//void CDlgBook::OnCbnDropdownCombo1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//GetDlgItem(IDC_EDIT_PHONE)->EnableWindow(TRUE);
//}

void CDlgBook::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

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
	// TODO: 在此添加控件通知处理程序代码
	//默认定价
	SetDlgItemText(IDC_PRICE, "0");
	GetDlgItem(IDC_PRICE)->EnableWindow(FALSE);
}

void CDlgBook::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemText(IDC_PRICE, "0");
	GetDlgItem(IDC_PRICE)->EnableWindow(TRUE);
}

void CDlgBook::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboPeriod.GetCurSel() > 0)
	{
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(FALSE);
	}
}
