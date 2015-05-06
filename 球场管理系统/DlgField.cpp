// DlgField.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgField.h"
#include "public.h"
#include "DlgBook.h"
#include "tinyxml.h"
#include "xmlbase.h"
#include "DlgBookInfo.h"
#include "MainDlg.h"
#include "DlgAddField.h"
#include "history.h"
#include "DlgCancelBook.h"
#include "DlgCharge.h"
#include "DlgBookKey.h"
#include "operatedata.h"
#include "app.h"

// CDlgField 对话框

IMPLEMENT_DYNAMIC(CDlgField, CDialog)

CDlgField::CDlgField(CWnd* pParent /*=NULL*/)
: CDialog(CDlgField::IDD, pParent)
, m_date(COleDateTime::GetCurrentTime())
{

}

CDlgField::~CDlgField()
{
}

void CDlgField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIELD, m_listField);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
}

BEGIN_MESSAGE_MAP(CDlgField, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FIELD, &CDlgField::OnNMCustomdrawListField)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FIELD, &CDlgField::OnNMRClickListField)
	ON_COMMAND(ID_BOOK, &CDlgField::OnBook)
	ON_COMMAND(ID_BOOK_INFO, &CDlgField::OnBookInfo)
	ON_COMMAND(ID_CANCEL_BOOK, &CDlgField::OnCancelBook)
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CDlgField::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgField::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgField::OnBnClickedButton1)
	ON_COMMAND(ID_BOOK_PAY, &CDlgField::OnBookPay)
	ON_COMMAND(ID_32799, &CDlgField::On32799)
END_MESSAGE_MAP()


// CDlgField 消息处理程序

BOOL CDlgField::OnInitDialog()
{
	m_pFieldData = GetFieldData;
	m_pPeriodData = GetPeriodData;
	
	CDialog::OnInitDialog();
	m_listField.SetExtendedStyle(LVS_EX_GRIDLINES/*|LVS_EX_FULLROWSELECT*/);  

	//

	m_listField.SetRowHeight(25);
	ShowItemField(GetDate());
	if (m_ayFieldInfo.GetSize() == 0)
	{
		AddTimeCol();
		m_listField.Invalidate();
	}

	SetPreBtnStatus();

	CRect rect;    
	GetClientRect(&rect);     //取客户区大小  
	m_ptOld.x=rect.right-rect.left;
	m_ptOld.y=rect.bottom-rect.top;

	return TRUE;
}

void CDlgField::SetPreBtnStatus()
{
	//暂时禁用这个功能
	return;

	CString strDate = GetDate();
	CTime tim(time(NULL));
	int nSelectDay = atoi(strDate.GetBuffer());
	strDate.Format("%04d%02d%02d", tim.GetYear(), tim.GetMonth(), tim.GetDay());
	int nToday = atoi(strDate.GetBuffer());
	if (nSelectDay <= nToday)
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
}

void CDlgField::AddTimeCol()
{
	int k=0;
	m_listField.InsertColumn(k++, _T("时间/球场"),	LVCFMT_CENTER, 60);
	
	m_ayTimeByIndex.RemoveAll();

	PeriodInfo periodInfo;
	m_pPeriodData->Read(periodInfo);
	int nStartMin = periodInfo.m_nStartMin;
	int nTimeCount = periodInfo.m_nHalfCount;

	if (!periodInfo.m_bInit)
	{
		//没初始化，显示全天时间
		nTimeCount = 48;
		nStartMin = 0;
	}

	for (int i = 0; i < nTimeCount; i++)
	{
		CString strTmp;
		int nStartMin2 = nStartMin + 30;
		strTmp.Format("%02d:%02d-%02d:%02d", nStartMin/60,nStartMin%60, nStartMin2/60,nStartMin2%60);
		nStartMin += 30;

		//m_listField.InsertColumn(k++, strTmp,	LVCFMT_CENTER, 36);
		m_listField.InsertItem(i+1, strTmp);
		m_listField.SetColumnWidth(0,150);
		m_ayTimeByIndex.Add(strTmp);
	}
}


VipInfo* GetVipInfo(VIP_LIST& aylist, CString strVipID)
{
	//找到vip后查看他的个人信息
	int nVipCount = aylist.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (aylist[j].m_strID == strVipID)
		{
			break;
		}
	}

	if (j < nVipCount)
	{
		return &aylist[j];
	}

	return NULL; 

}

void CDlgField::ShowItemField(CString strDate)
{
	int nBegin = GetTickCount();

	if (strDate.IsEmpty())
	{
		strDate = GetDate();
	}
	m_listField.SetRedraw(FALSE);

	//内存只保留当天数据
	m_listField.DeleteAllItems();
	while (m_listField.DeleteColumn(0));
	
	m_ayFieldInfo.RemoveAll();

	CMainDlg* pMainWnd = GETMAINWND;

	//读出所有数据
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(strDate);
	pMainWnd->m_pageField.m_pFieldData->GetAllData(m_ayFieldInfo);

	int nFieldCount = m_ayFieldInfo.GetCount();
	if (nFieldCount == 0)
	{
		m_listField.SetRedraw(TRUE);
		return;
	}
	AddTimeCol();

	VIP_LIST viplist;
	VIP_LIST vipdel;
	CVipData* pVipData = NULL;
	pVipData = GetVipData;
	pVipData->GetAllData(viplist, vipdel);

	for (int col = 0; col < nFieldCount; col++)
	{
		m_listField.InsertColumn(col + 1, m_ayFieldInfo[col].m_strFieldName);
		//禁止改变宽度
		CHeaderCtrl*   pHeaderCtrl   =   (CHeaderCtrl*)m_listField.GetHeaderCtrl();   
		pHeaderCtrl->EnableWindow(FALSE); 

		m_listField.SetColumnWidth(col + 1, 80);
		int nTimeCount = m_ayFieldInfo[col].m_ayTimeInfo.size();

		for (int row = 0; row < nTimeCount; row++)
		{
			OneTimeBookInfo& one = m_ayFieldInfo[col].m_ayTimeInfo[row];
			if (one.m_bBusy && GetVipInfo(viplist, one.m_strVipID) == NULL)
			{
				one.m_bBusy = FALSE;
				one.m_strVipID = "0";
				m_pFieldData->ChangeFieldBookInfo(m_ayFieldInfo[col].m_strFieldID, one);
			}
			//if (one.m_bBusy)
			//{
			//	m_listField.SetItemText(one.m_nTimeIndex, col + 1 , FIELD_BUSY);
			//}
			//else
			//{
			//	m_listField.SetItemText(one.m_nTimeIndex, col + 1, FIELD_IDLE);
			//}
		}
	}

	if (m_listField.GetItemCount() == 0)
	{
	}
	else
	{
		m_listField.SetSelectionMark(0);
	}

	//更新内容
	m_listField.SetRedraw(TRUE);
	m_listField.Invalidate();
	//m_listField.UpdateWindow();

	UpdateData(FALSE);
	int nEnd = GetTickCount();
	TRACE("\nShowItemField cost:%d\n", nEnd - nBegin);
}
void CDlgField::OnNMCustomdrawListField(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (pNMCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:

		// 		int nFieldCount = m_listField.GetItemCount();
		// 		for (int row = 0; row < nFieldCount; row++)
		// 		{
		// 
		// 			for (int col = 1; col <= m_nTimeCount + 1; col++)
		// 			{
		// 
		// 				if (col == m_nTimeCount)
		// 				{
		// 					col = m_nTimeCount;
		// 				}
		// 				CString str = m_listField.GetItemText(row,col);
		// 				CRect rect;
		// 				CDC*pDC = CDC::FromHandle(pNMCD->nmcd.hdc);
		// 
		// 				m_listField.GetSubItemRect(row,col,LVIR_BOUNDS,rect);
		// 
		// 				UINT uCode=DT_LEFT;
		// 
		// 				COLORREF kolor = RGB(255,255,255);
		// 
		// 				if (str == FIELD_BUSY)
		// 				{
		// 					kolor = FIELD_BUSY_COLOR;
		// 				}
		// 				else if (str == FIELD_IDLE)
		// 				{
		// 					kolor = FIELD_IDLE_COLOR;
		// 				}
		// 				else
		// 				{
		// 					kolor = FIELD_NULL_COLOR;
		// 				}
		// 
		// 				CBrush brush(kolor);
		// 				pDC->FillRect(&rect,&brush);
		// 				//rect.OffsetRect(10,0);
		// 				//pDC->DrawText(str,&rect,uCode);
		// 			}
		// 	}
		break;
	}
	*pResult = 0;
}

void CDlgField::OnNMRClickListField(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;
	char chstr[MAX_PATH];

	m_listField.GetItemText(pNotifier->iItem,pNotifier->iSubItem,chstr,MAX_PATH);

	m_curSelectTimeIndex = pNotifier->iItem;
	m_curSelectField = pNotifier->iSubItem - 1;

	if (m_curSelectTimeIndex < 0 || m_curSelectField < 0)
	{
		return;
	}
	CString str=chstr;

	CMenu menu ,* pSubMenu; //定义下面要用到的cmenu对象

	OneTimeBookInfo* pInfo = GetFieldBookInfo(m_curSelectField, m_curSelectTimeIndex);
	if (pInfo->m_bBusy == FIELD_PAYED_DATA)
	{
		return;
	}
	if (pInfo->m_bBusy == FIELD_BUSY_DATA)
	{
		menu.LoadMenu(IDR_MENU_BUSY);
	}
	else
	{
		time_t nowtime_t = time(NULL);
		int nCurTimeIndex = GetTimeIndexByTime(nowtime_t);
		CTime nowTime(nowtime_t);
		CString strTime;
		strTime.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
		bool isPast = false;
		bool isToday = false;
		if (GetDate() < strTime)
		{
			//before today;
			isPast = true;
		}
		if (GetDate() == strTime)
		{
			isToday = true;
		}

		if (isPast || (isToday && m_curSelectTimeIndex < nCurTimeIndex))
		{
			return;
		}
		menu.LoadMenu(IDR_MENU_IDLE);
	}

	//获取第一个弹出菜单，所以第一个菜单必须有子菜单
	pSubMenu = menu.GetSubMenu(0);
	CPoint oPoint; //定义一个用于确定光标位置的位置
	GetCursorPos( &oPoint); //获取当前光标的位置，以便使得菜单可以跟随光标

	//在指定位置显示弹出菜单
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CDlgField::OnBook()
{
	// TODO: 在此添加命令处理程序代码
	CDlgBook dlg;
	dlg.m_strFieldID = m_ayFieldInfo[m_curSelectField].m_strFieldID;
	dlg.m_strFieldName = m_ayFieldInfo[m_curSelectField].m_strFieldName;
	dlg.m_strXML = m_strXML;

	OneTimeBookInfo* p = GetFieldBookInfo(dlg.m_strFieldID, m_curSelectTimeIndex);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldBookInfo(dlg.m_strFieldID, m_curSelectTimeIndex);
	}

	int nIndexStart = m_curSelectTimeIndex;
	while(p && p->m_bBusy == FALSE)
	{
		p = GetFieldBookInfo(dlg.m_strFieldID, --nIndexStart);
 	}
	nIndexStart++;

	p = GetFieldBookInfo(dlg.m_strFieldID, nIndexStart);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldBookInfo(dlg.m_strFieldID, m_curSelectTimeIndex);
	}

	//gray
	time_t nowtime_t = time(NULL);
	int nCurTimeIndex = GetTimeIndexByTime(nowtime_t);
	CTime cTimeIn(nowtime_t);
	CString strToday;
	strToday.Format("%04d%02d%02d",cTimeIn.GetYear(), cTimeIn.GetMonth(), cTimeIn.GetDay());//16:30
	CString strSelectDay = GetDate();
	while (p && p->m_bBusy == FALSE)
	{
		//if gray do not add
		if (strToday < strSelectDay)
		{
			CString strTime = m_ayTimeByIndex[nIndexStart];
			int nMin = strTime.Find('-');
			CString strStart = strTime.Mid(0, nMin);
			CString strStop = strTime.Mid(nMin+1);
			dlg.m_ayStartTimeStr.Add(strStart);
			dlg.m_ayStopTimeStr.Add(strStop);
			dlg.m_ayIndex.Add(nIndexStart);
		}
		else if (nIndexStart >= nCurTimeIndex)
		{
			CString strTime = m_ayTimeByIndex[nIndexStart];
			int nMin = strTime.Find('-');
			CString strStart = strTime.Mid(0, nMin);
			CString strStop = strTime.Mid(nMin+1);
			dlg.m_ayStartTimeStr.Add(strStart);
			dlg.m_ayStopTimeStr.Add(strStop);
			dlg.m_ayIndex.Add(nIndexStart);
		}
		
		p = GetFieldBookInfo(dlg.m_strFieldID, ++nIndexStart);
	}

	int nIndexCount = dlg.m_ayIndex.GetSize();
	dlg.m_nTimeIndex = 0;
	for (int i = 0; i < nIndexCount; ++i)
	{
		if (dlg.m_ayIndex[i] < m_curSelectTimeIndex)
		{
			dlg.m_nTimeIndex ++;
		}
		else
		{
			break;
		}
		
	}
	//dlg.m_nTimeIndex++;
	dlg.DoModal();
	if (dlg.m_bSuccess)
	{
		ShowItemField(GetDate());
	}
}


void CDlgField::OnBookInfo()
{
	CDlgBookInfo dlg;

	CString strFieldID = m_ayFieldInfo[m_curSelectField].m_strFieldID;

	CString strTitle;
	strTitle.Format("场地：%s 时间：%s", strFieldID.GetBuffer(), m_ayTimeByIndex[m_curSelectTimeIndex]);
	dlg.m_strTitle = strTitle;

	CString strVipID;
	OneTimeBookInfo bookInfo;
	int n = m_ayFieldInfo[m_curSelectField].m_ayTimeInfo.size();
	for (int i = 0; i < n; i++)
	{
		if (m_ayFieldInfo[m_curSelectField].m_ayTimeInfo[i].m_nTimeIndex == m_curSelectTimeIndex)
		{
			strVipID = m_ayFieldInfo[m_curSelectField].m_ayTimeInfo[i].m_strVipID;
			bookInfo = m_ayFieldInfo[m_curSelectField].m_ayTimeInfo[i];
		}
	}
	ASSERT(!strVipID.IsEmpty());

	//找到vip后查看他的个人信息
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	int nVipCount = pMainWnd->m_pageVip.m_ayVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (pMainWnd->m_pageVip.m_ayVip[j].m_strID == strVipID)
		{
			break;
		}
	}

	ASSERT(j < nVipCount);

	VipInfo& info = pMainWnd->m_pageVip.m_ayVip[j];

	CString strMsg;

	strMsg.Format("场地：%s\n时间：%s\n预订人ID：%s\n姓名：%s\n性别：%s\n电话：%s"
		, strFieldID.GetBuffer()
		, m_ayTimeByIndex[m_curSelectTimeIndex]
	, info.m_strID.GetBuffer()
		, info.m_strName.GetBuffer()
		,info.m_strSex.GetBuffer()
		, info.m_strPhone.GetBuffer());

	if (!bookInfo.m_strBoxKey.IsEmpty())
	{
		CString strTmp;
		strTmp.Format("\n柜子号:%s", bookInfo.m_strBoxKey.GetBuffer());
		strMsg += strTmp;
	}
	else
	{
		strMsg += "\n柜子号:无";
	}

	MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	//取消使用bookinfo dlg
}


void CDlgField::OnCancelBook()
{
	// TODO: 在此添加命令处理程序代码

// 	int nTimeIndex = m_curSelectTimeIndex;
// 	CString strFieldID = m_ayFieldInfo[m_curSelectField].m_strFieldID;
// 
// 	//查找预订人信息
// 	CString strVipID;
// 	int n = m_ayFieldInfo[m_curSelectField].m_ayTimeInfo.size();
// 	for (int i = 0; i < n; i++)
// 	{
// 		if (m_ayFieldInfo[m_curSelectField].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
// 		{
// 			strVipID = m_ayFieldInfo[m_curSelectField].m_ayTimeInfo[i].m_strVipID;
// 		}
// 	}
// 	ASSERT(!strVipID.IsEmpty());
// 
// 	//找到vip后查看他的个人信息
// 	CWnd* p = AfxGetApp()->GetMainWnd();
// 	CMainDlg* pMainWnd = (CMainDlg*)p;
// 	VipInfo* info  = pMainWnd->m_pageVip.GetVipInfo(strVipID);
// 	ASSERT(info);
// 
// 	CString strMsg;
// 
// 	strMsg.Format("场地：%s\n时间：%s\n预订人ID：%s\n姓名：%s\n电话：%s\n\n是否取消预订？"
// 		, strFieldID.GetBuffer()
// 		, m_ayTimeByIndex[nTimeIndex]
// 	, info->m_strID.GetBuffer()
// 		, info->m_strName.GetBuffer()
// 		, info->m_strPhone.GetBuffer());
// 
// 	if(MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
// 		return;
// 
// 	OneTimeBookInfo infoBook;
// 
// 	OneTimeBookInfo* pinfo = GetFieldBookInfo(strFieldID, nTimeIndex);
// 	if (!pinfo)
// 	{
// 		return;
// 	}
// 	infoBook =  *pinfo;
// 
// 	//交换信息
// 	infoBook.m_bBusy = FALSE;
// 	infoBook.m_strVipID = "";
// 	ChangeFieldBookInfo(strFieldID, infoBook);
// 
// 	ShowItemField(GetDate());


	CDlgCancelBook dlg;
	dlg.m_strFieldID = m_ayFieldInfo[m_curSelectField].m_strFieldID;
	dlg.m_strFieldName = m_ayFieldInfo[m_curSelectField].m_strFieldName;
	dlg.m_strXML = m_strXML;

	OneTimeBookInfo* p = GetFieldBookInfo(dlg.m_strFieldID, m_curSelectTimeIndex);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldBookInfo(dlg.m_strFieldID, m_curSelectTimeIndex);
	}

	//右键点到取消，说明点击的位置是忙。
	CString strVipId = p->m_strVipID;
	dlg.m_strVipID = strVipId;
	int nIndexStart = m_curSelectTimeIndex;
	while(p && p->m_bBusy == FIELD_BUSY_DATA && p->m_strVipID == strVipId)
	{
		p = GetFieldBookInfo(dlg.m_strFieldID, --nIndexStart);
	}
	nIndexStart++;

	p = GetFieldBookInfo(dlg.m_strFieldID, nIndexStart);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldBookInfo(dlg.m_strFieldID, m_curSelectTimeIndex);
	}
	while (p && p->m_bBusy == FIELD_BUSY_DATA && p->m_strVipID == strVipId)
	{
		CString strTime = m_ayTimeByIndex[nIndexStart];
		int nMin = strTime.Find('-');
		CString strStart = strTime.Mid(0, nMin);
		CString strStop = strTime.Mid(nMin+1);
		dlg.m_ayStartTimeStr.Add(strStart);
		dlg.m_ayStopTimeStr.Add(strStop);
		dlg.m_ayIndex.Add(nIndexStart);
		p = GetFieldBookInfo(dlg.m_strFieldID, ++nIndexStart);
	}

	int nIndexCount = dlg.m_ayIndex.GetSize();
	dlg.m_nTimeIndex = 0;
	for (int i = 0; i < nIndexCount; ++i)
	{
		if (dlg.m_ayIndex[i] < m_curSelectTimeIndex)
		{
			dlg.m_nTimeIndex ++;
		}
		else
		{
			break;
		}

	}
	//dlg.m_nTimeIndex++;
	dlg.DoModal();
	if (dlg.m_bSuccess)
	{
		ShowItemField(GetDate());
	}


}

void CDlgField::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgField::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CDlgField::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CDlgField::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgField::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgField::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgAddField dlg;
	dlg.m_strXML = m_strXML;
	dlg.DoModal();
	if (dlg.IsSuccess())
	{
		ShowItemField(GetDate());
	}
}

void CDlgField::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION	pos;
	pos = m_listField.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要删除的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	CMainDlg* pMainWnd = GETMAINWND;
	int index = pMainWnd->m_pageField.m_listField.m_nBeginDragFieldIndex;
	CString strID = pMainWnd->m_pageField.m_ayFieldInfo[index].m_strFieldID;
	pMainWnd->m_pageField.m_pFieldData->DeleteField(strID);
	ShowItemField(GetDate());
}

OneTimeBookInfo* CDlgField::GetFieldBookInfo(int nFieldIndex, int nTimeIndex, BOOL bAdd)
{
	return GetFieldBookInfo(m_ayFieldInfo[nFieldIndex].m_strFieldID, nTimeIndex, bAdd);
}

OneTimeBookInfo* CDlgField::GetFieldBookInfo(CString strFieldID, int nTimeIndex, BOOL bAdd)
{
	int nFieldCount = m_ayFieldInfo.GetCount();

 	int j = 0;
	for (; j < nFieldCount; ++j)
	{
		if (m_ayFieldInfo[j].m_strFieldID == strFieldID)
		{
			break;
		}
	}

	if (j == nFieldCount)
	{
		return NULL;
	}

	int nTimeCount = m_ayFieldInfo[j].m_ayTimeInfo.size();
	if (bAdd && nTimeCount == 0)
	{
		AddFieldDay(strFieldID);
		nTimeCount = m_ayFieldInfo[j].m_ayTimeInfo.size();
	}

	for (int i = 0; i < nTimeCount; ++i)
	{
		if (m_ayFieldInfo[j].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
		{
			return &m_ayFieldInfo[j].m_ayTimeInfo[i];
		}
	}
	return NULL;
}

int CDlgField::AddFieldDay(CString strFieldID)
{
	UpdateData(TRUE);

	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(GetDate());
	pMainWnd->m_pageField.m_pFieldData->AddFieldDay(strFieldID);

	UpdateFieldData();

	return 0;
}

void CDlgField::UpdateFieldData()
{
	CMainDlg* pMainWnd = GETMAINWND;
	//读出所有数据
	m_ayFieldInfo.RemoveAll();
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(GetDate());
	pMainWnd->m_pageField.m_pFieldData->GetAllData(m_ayFieldInfo);
}

int CDlgField::ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info)
{
	CMainDlg* pMainWnd = GETMAINWND;
	//修改信息，并且更新xml文件
	OneTimeBookInfo * pInfo = GetFieldBookInfo(strFieldID, info.m_nTimeIndex);
	if (!pInfo)
	{
		//如果field没有现在日期的数据，就会为空，那么添加这天
		pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(GetDate());
		pMainWnd->m_pageField.m_pFieldData->AddFieldDay(strFieldID);
		UpdateFieldData();
		pInfo = GetFieldBookInfo(strFieldID, info.m_nTimeIndex);
	}

	*pInfo = info;//改内存中的值

	
	pMainWnd->m_pageField.m_pFieldData->ChangeFieldBookInfo(strFieldID, info);
	return 0;

}

void CDlgField::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	UpdateData(TRUE);
	CString strDate = GetDate();
	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(strDate);
	ShowItemField(strDate);
	SetPreBtnStatus();
}

CString CDlgField::GetDate()
{
	UpdateData(TRUE);
	CString strDate;
	strDate.Format("%04d%02d%02d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay());
	return strDate;
}
void CDlgField::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_date -= 1;
	CString strDate;
	strDate.Format("%04d%02d%02d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay());
	UpdateData(FALSE);
	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(strDate);
	ShowItemField(strDate);
	SetPreBtnStatus();
}

void CDlgField::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_date += 1;
	CString strDate;
	strDate.Format("%04d%02d%02d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay());
	UpdateData(FALSE);
	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(strDate);
	ShowItemField(strDate);
	SetPreBtnStatus();
}

void CDlgField::MoveX(int nID, int len)
{
	CRect rect;
	GetDlgItem(nID)->GetClientRect(&rect);
	rect.left += len;
	rect.right += len;
	GetDlgItem(nID)->MoveWindow(&rect, TRUE);
}


void CDlgField::OnBookPay()
{
	// TODO: 在此添加命令处理程序代码
	//付款
	CString strFieldID = m_ayFieldInfo[m_curSelectField].m_strFieldID;
	OneTimeBookInfo* p = GetFieldBookInfo(strFieldID, m_curSelectTimeIndex);
	ASSERT(p);
	
	if (!p->m_strBoxKey.IsEmpty())
	{
		CString strKeyMsg;
		strKeyMsg.Format("请归还柜子钥匙:%s", p->m_strBoxKey.GetBuffer());
		if(MessageBox(strKeyMsg,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
			return;
	}


	int nIndexStart = m_curSelectTimeIndex;
	OneTimeBookInfo* pre = p;
	//往前找同一个人预订的
	while(pre && pre->m_bBusy == TRUE && pre->m_strVipID == p->m_strVipID)
	{
		pre = GetFieldBookInfo(strFieldID, --nIndexStart);
	}
	nIndexStart++;

	int nIndexStop = m_curSelectTimeIndex;
	OneTimeBookInfo* after = p;
	//往后找同一个人预订的
	while (after && after->m_bBusy == TRUE && after->m_strVipID == p->m_strVipID)
	{
		after = GetFieldBookInfo(strFieldID, ++nIndexStop);
	}
	--nIndexStop;

	//付费区间为[nIndexStart, nIndexStop],闭区间
	CMainDlg* pMainWnd = GETMAINWND;
	VipInfo* pVip = pMainWnd->m_pageVip.GetVipInfo(p->m_strVipID);
	CString strVipID = pVip->m_strID;

	CString strMsg;
	int nTotalMoney = 0;
	CString strTmp;

	PeriodInfo period;
	pMainWnd->m_pageMgr.m_pPeriodData->Read(period);


	CTime nowTime(time(NULL));

	CArray<HisBuyFieldInfo> ayHisInfo;
	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		OneTimeBookInfo* pOneTime = pMainWnd->m_pageField.GetFieldBookInfo(strFieldID, i);

		HisBuyFieldInfo hisinfo;
		hisinfo.m_strVipID = strVipID;
		hisinfo.m_strFieldID = strFieldID;
		hisinfo.m_strTime = m_ayTimeByIndex[i].GetBuffer();
		hisinfo.m_time.m_strDate.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
		hisinfo.m_time.m_strTime.Format("%02d:%02d:%02d", nowTime.GetHour(), nowTime.GetMinute(), nowTime.GetSecond());
		//黄金时间段内，或者周末为黄金
		if (pOneTime->m_bAutoPrice)
		{
			CString strPrice = pMainWnd->m_pageMgr.GetPriceByLevelAndTime(pVip->m_strLevelID, GetDate(), i);
			nTotalMoney += atoi(strPrice);
			strTmp.Format("%s:\t%s元\n", m_ayTimeByIndex[i].GetBuffer(), strPrice.GetBuffer());
			hisinfo.m_strMoney = strPrice;
		}
		else
		{
			hisinfo.m_strMoney = pOneTime->m_strPrice;
			nTotalMoney += atoi(hisinfo.m_strMoney);
		}

		strTmp.Format("%s:\t%s元\n", m_ayTimeByIndex[i].GetBuffer(), hisinfo.m_strMoney);
		strMsg += strTmp;
		ayHisInfo.Add(hisinfo);
	}

	if (atoi(pVip->m_strMoney) < nTotalMoney && pVip->m_strLevelID != VIP_LEVEL_0)
	{
		//钱不够的，会员充值
		CString str;
		str.Format("应付：%d元\n余额：%s元\n余额不足，请充值%d元！", nTotalMoney, pVip->m_strMoney.GetBuffer()
			,nTotalMoney - atoi(pVip->m_strMoney));
		if(MessageBox(str,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
			return;
		CDlgCharge dlg;
		dlg.m_strID		= pVip->m_strID;
		dlg.DoModal();
		return;
	}

	if (pVip->m_strLevelID == VIP_LEVEL_0)
	{
		CString str;
		if (atoi(pVip->m_strMoney) < nTotalMoney)
		{
			str.Format("应付%d元，余额%s元,需补交%d元", nTotalMoney, pVip->m_strMoney.GetBuffer()
				,nTotalMoney - atoi(pVip->m_strMoney));
		}
		else
		{
			str.Format("应付%d元，余额%s元,找零%d元", nTotalMoney, pVip->m_strMoney.GetBuffer()
				,atoi(pVip->m_strMoney) - nTotalMoney);
		}
		strMsg += str;
	}
	else
	{
		strTmp.Format("\n共 %d 元", nTotalMoney);
		strMsg += strTmp;
	}

	if (MessageBox(strMsg, _T("提示"), MB_YESNO|MB_ICONINFORMATION|MB_TASKMODAL) != IDYES)
	{
		return;
	}

	//扣费
	CString strMoney;
	strMoney.Format("%d", atoi(pVip->m_strMoney) - nTotalMoney);
	pVip->m_strMoney = strMoney;
	if (pVip->m_strLevelID == VIP_LEVEL_0)
	{
		pVip->m_strMoney = "0";
	}

	VipInfo info = *pVip;
	pMainWnd->m_pageVip.ChangeVipInfo(info);
	pVip = pMainWnd->m_pageVip.GetVipInfo(p->m_strVipID);
	
	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		OneTimeBookInfo* p = GetFieldBookInfo(strFieldID, i);
		p->m_bBusy = FIELD_PAYED_DATA;
		ChangeFieldBookInfo(strFieldID, *p);
	}

	//写入消费记录
	CHisData* pVipHisData = NULL;
	CHisData* pFieldHisData = NULL;
	if (DATA_FROM_TYPE == DATA_FROM_XML)
	{
		pFieldHisData = new CHisDataXML(TL_GetModulePath(NULL) + 
			"fieldhistory" + PATHSPLITSTRING + "fieldhistory" + ".xml");
	} 
	else if (DATA_FROM_TYPE == DATA_FROM_MYSQL)
	{
		pFieldHisData = GetHistoryData;
	}
	else
	{
		pFieldHisData = NULL;
		ASSERT(FALSE && "数据来源不支持！");
		return;
	}

	for (int i = 0; i < ayHisInfo.GetSize(); i++)
	{
		pFieldHisData->AddBuyField(ayHisInfo[i]);
	}

	if (DATA_FROM_TYPE == DATA_FROM_XML)
	{
		delete pVipHisData;
		delete pFieldHisData;
	}


	//操作记录
	strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[nIndexStart];
	CString strStart = strTmp.Left(strTmp.Find('-'));

	strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[nIndexStop];
	CString strStop =strTmp.Right(strTmp.Find('-'));

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_PAY_FIELD;

	if (pVip->m_strLevelID != VIP_LEVEL_0)
	{
		his_op_info.m_strInfo.Format("场地:%s,日期:%s,时段:%s-%s,会员ID:%s"
			, strFieldID.GetBuffer()
			, pMainWnd->m_pageField.GetDate().GetBuffer()
			, strStart.GetBuffer()
			, strStop.GetBuffer()
			, pVip->m_strID.GetBuffer());
	}
	else
	{
		his_op_info.m_strInfo.Format("场地:%s,日期:%s,时段:%s-%s,非会员,姓名:%s,电话:%s"
			, strFieldID.GetBuffer()
			, pMainWnd->m_pageField.GetDate().GetBuffer()
			, strStart.GetBuffer()
			, strStop.GetBuffer()
			, pVip->m_strName.GetBuffer()
			, pVip->m_strPhone.GetBuffer());
	}

	if (p->m_strBoxKey.IsEmpty())
	{
		his_op_info.m_strInfo += ",柜子号:未租";
	}
	else
	{
		his_op_info.m_strInfo += ",柜子号:";
		his_op_info.m_strInfo += p->m_strBoxKey;
	}
	
	pOperate->Add(his_op_info);


	ShowItemField(GetDate());
}

void CDlgField::resize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小
	CRect recta;    
	GetClientRect(&recta);     //取客户区大小 
	CSize sz = recta.Size();
	if (sz.cx == 0 && sz.cy == 0)
	{
		//最小化
		return;
	}
	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/m_ptOld.x;
	fsp[1]=(float)Newp.y/m_ptOld.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //左上角
	CPoint OldBRPoint,BRPoint; //右下角
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//取得ID
		GetDlgItem(woc)->GetWindowRect(Rect);  
		ScreenToClient(Rect);  
		OldTLPoint = Rect.TopLeft();  
		TLPoint.x = long(OldTLPoint.x*fsp[0]);  
		TLPoint.y = long(OldTLPoint.y*fsp[1]);  
		OldBRPoint = Rect.BottomRight();  
		BRPoint.x = long(OldBRPoint.x *fsp[0]);  
		BRPoint.y = long(OldBRPoint.y *fsp[1]);  
		Rect.SetRect(TLPoint,BRPoint);  
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	m_ptOld=Newp;

}

void CDlgField::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		resize();
	}
}

BOOL CDlgField::GetFieldInfo(CString strID, FieldInfo& out)
{
	int n = m_ayFieldInfo.GetSize();
	FieldInfo info;
	for (int i = 0; i < n; i++)
	{
		if (m_ayFieldInfo[i].m_strFieldID == strID)
		{
			out.m_strFieldID = strID;
			out.m_strFieldName = m_ayFieldInfo[i].m_strFieldName;
			return TRUE;
		}
	}
	return FALSE;
}
void CDlgField::On32799()
{
	// TODO: 在此添加命令处理程序代码
	//租用柜子
	CString strFieldID = m_ayFieldInfo[m_curSelectField].m_strFieldID;
	OneTimeBookInfo* p = GetFieldBookInfo(m_curSelectField, m_curSelectTimeIndex);
	if (p == NULL)
	{
		AddFieldDay(strFieldID);
		p = GetFieldBookInfo(m_curSelectField, m_curSelectTimeIndex);
	}

	CDlgBookKey dlg;
	dlg.m_strKey = p->m_strBoxKey;
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	int nIndexStart = m_curSelectTimeIndex;
	OneTimeBookInfo* pre = p;
	//往前找同一个人预订的
	while(pre && pre->m_bBusy == TRUE && pre->m_strVipID == p->m_strVipID)
	{
		pre = GetFieldBookInfo(strFieldID, --nIndexStart);
	}
	nIndexStart++;

	int nIndexStop = m_curSelectTimeIndex;
	OneTimeBookInfo* after = p;
	//往后找同一个人预订的
	while (after && after->m_bBusy == TRUE && after->m_strVipID == p->m_strVipID)
	{
		after = GetFieldBookInfo(strFieldID, ++nIndexStop);
	}
	--nIndexStop;

	//付费区间为[nIndexStart, nIndexStop],闭区间
	CMainDlg* pMainWnd = GETMAINWND;
	for (int i = nIndexStart; i <= nIndexStop; i++)
	{
		p = GetFieldBookInfo(strFieldID, i);
		OneTimeBookInfo info = *p;
		info.m_strBoxKey = dlg.m_strKey;
		ChangeFieldBookInfo(strFieldID, info);
	}

	//操作记录
	CString strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[nIndexStart];
	CString strStart = strTmp.Left(strTmp.Find('-'));

	strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[nIndexStop];
	CString strStop =strTmp.Right(strTmp.Find('-'));

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_PAY_FIELD;

	VipInfo* pVip = pMainWnd->m_pageVip.GetVipInfo(p->m_strVipID);
	if (pVip->m_strLevelID != VIP_LEVEL_0)
	{
		his_op_info.m_strInfo.Format("场地:%s,日期:%s,时段:%s-%s,租用了柜:%s,会员ID:%s"
			, strFieldID.GetBuffer()
			, pMainWnd->m_pageField.GetDate().GetBuffer()
			, strStart.GetBuffer()
			, strStop.GetBuffer()
			, dlg.m_strKey.GetBuffer()
			, p->m_strVipID.GetBuffer());
	}
	else
	{
		his_op_info.m_strInfo.Format("场地:%s,日期:%s,时段:%s-%s,租用了柜:%s,非会员,姓名:%s,电话:%s"
			, strFieldID.GetBuffer()
			, pMainWnd->m_pageField.GetDate().GetBuffer()
			, strStart.GetBuffer()
			, strStop.GetBuffer()
			, dlg.m_strKey.GetBuffer()
			, pVip->m_strName.GetBuffer()
			, pVip->m_strPhone.GetBuffer());
	}

	pOperate->Add(his_op_info);

}

int CDlgField::GetTimeIndexByTime(time_t timeIn)
{
	CTime cTimeIn(timeIn);
	CString strTime;
	int Min = cTimeIn.GetMinute();//16:45
	Min = Min/30*30;
	strTime.Format("%02d:%02d",cTimeIn.GetHour(), Min);//16:30

	int nCount = m_ayTimeByIndex.GetSize();
	int i = 0;
	for (; i < nCount; i++)
	{
		CString& str = m_ayTimeByIndex[i];
		int nIndex = str.Find('-');
		CString strF = str.Left(nIndex);
		CString strB = str.Right(str.GetLength() - nIndex);
		if (strF >= strTime || strB >= strTime)
		{
			break;
		}
	}
	return i;

}
