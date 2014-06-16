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
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgField::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgField::OnBnClickedBtnDel)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CDlgField::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgField::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgField::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgField 消息处理程序

BOOL CDlgField::OnInitDialog()
{
	m_nTimeCount = 17;
	if (DATA_FROM_TYPE == DATA_FROM_XML)
	{
		m_pFieldData = new CFieldDataXML(TL_GetModulePath(NULL) + "field.xml", m_nTimeCount);
	}
	else
	{
		m_pFieldData = NULL;
		ASSERT(FALSE && "数据来源不支持！");
		return FALSE;
	}

	GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	
	CDialog::OnInitDialog();
	m_listField.SetExtendedStyle(LVS_EX_GRIDLINES/*|LVS_EX_FULLROWSELECT*/);  

	int k=0;
	m_listField.InsertColumn(k++, _T("球场"),	LVCFMT_CENTER, 60);
	
	m_ayTimeByIndex.RemoveAll();
	int hour = 8;
	for (int i = 0; i < m_nTimeCount; i++)
	{
		CString strTmp;
		if (i%2 == 0)
		{
			hour++;
			strTmp.Format("%d:00-%d:30", hour , hour);
		}
		else
		{
			strTmp.Format("%d:30-%d:00", hour , hour + 1);
		}
		
		m_listField.InsertColumn(k++, strTmp,	LVCFMT_CENTER, 36);
		m_ayTimeByIndex.Add(strTmp);
	}

	m_listField.SetRowHeight(36);
	ShowItemField(GetDate());

	SetPreBtnStatus();

	return TRUE;
}

void CDlgField::SetPreBtnStatus()
{
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

void CDlgField::ShowItemField(CString strDate)
{
	//内存只保留当天数据
	m_listField.DeleteAllItems();
	m_ayFieldInfo.RemoveAll();

	CMainDlg* pMainWnd = GETMAINWND;

	//读出所有数据
	pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(strDate);
	pMainWnd->m_pageField.m_pFieldData->GetAllData(m_ayFieldInfo);

	int nFieldCount = m_ayFieldInfo.GetCount();
	for (int row = 0; row < nFieldCount; row++)
	{
		m_listField.InsertItem(row, m_ayFieldInfo[row].m_strFieldID);
		int nTimeCount = m_ayFieldInfo[row].m_ayTimeInfo.size();
		for (int col = 0; col < nTimeCount; col++)
		{
			OneTimeInfo& one = m_ayFieldInfo[row].m_ayTimeInfo[col];
			if (one.m_bBusy)
			{
				m_listField.SetItemText(row, one.m_nTimeIndex + 1, FIELD_BUSY);
			}
			else
			{
				m_listField.SetItemText(row, one.m_nTimeIndex + 1, FIELD_IDLE);
			}
		}
	}

	if (m_listField.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		m_listField.SetSelectionMark(0);
	}

	UpdateData(FALSE);
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

	CListCtrl *CList =&m_listField;


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;
	char chstr[MAX_PATH];

	m_listField.GetItemText(pNotifier->iItem,pNotifier->iSubItem,chstr,MAX_PATH);

	m_curSelectRow = pNotifier->iItem;
	m_curSelectCol = pNotifier->iSubItem;

	if (m_curSelectCol < 0 || m_curSelectRow < 0)
	{
		return;
	}
	CString str=chstr;

	CMenu menu ,* pSubMenu; //定义下面要用到的cmenu对象

	if (IS_FIELD_BUSY(str))
	{
		menu.LoadMenu(IDR_MENU_BUSY);
	}
	else
	{
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
	int nTimeIndex = m_curSelectCol - 1;
	dlg.m_strBookTime = m_ayTimeByIndex[nTimeIndex];
	dlg.m_strFieldID = m_ayFieldInfo[m_curSelectRow].m_strFieldID;
	dlg.m_strXML = m_strXML;

	OneTimeInfo* p = GetFieldInfo(dlg.m_strFieldID, nTimeIndex);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldInfo(dlg.m_strFieldID, nTimeIndex);
	}

	int nIndexStart = nTimeIndex;
	while(p && p->m_bBusy == FALSE)
	{
		p = GetFieldInfo(dlg.m_strFieldID, --nIndexStart);
 	}
	nIndexStart++;

	p = GetFieldInfo(dlg.m_strFieldID, nIndexStart);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldInfo(dlg.m_strFieldID, nTimeIndex);
	}
	while (p && p->m_bBusy == FALSE)
	{
		CString strTime = m_ayTimeByIndex[nIndexStart];
		int nMin = strTime.Find('-');
		CString strStart = strTime.Mid(0, nMin);
		CString strStop = strTime.Mid(nMin+1);
		dlg.m_ayStartTimeStr.Add(strStart);
		dlg.m_ayStopTimeStr.Add(strStop);
		dlg.m_ayIndex.Add(nIndexStart);
		p = GetFieldInfo(dlg.m_strFieldID, ++nIndexStart);
	}

	int nIndexCount = dlg.m_ayIndex.GetSize();
	dlg.m_nTimeIndex = 0;
	for (int i = 0; i < nIndexCount; ++i)
	{
		if (dlg.m_ayIndex[i] < nTimeIndex)
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

	int nTimeIndex = m_curSelectCol - 1;
	CString strFieldID = m_ayFieldInfo[m_curSelectRow].m_strFieldID;

	CString strTitle;
	strTitle.Format("场地：%s 时间：%s", strFieldID.GetBuffer(), m_ayTimeByIndex[nTimeIndex]);
	dlg.m_strTitle = strTitle;

	CString strVipID;
	int n = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo.size();
	for (int i = 0; i < n; i++)
	{
		if (m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
		{
			strVipID = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_strVipID;
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
		, m_ayTimeByIndex[nTimeIndex]
	, info.m_strID.GetBuffer()
		, info.m_strName.GetBuffer()
		,info.m_strSex.GetBuffer()
		, info.m_strPhone.GetBuffer());

	MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	//取消使用bookinfo dlg
}


void CDlgField::OnCancelBook()
{
	// TODO: 在此添加命令处理程序代码

	int nTimeIndex = m_curSelectCol - 1;
	CString strFieldID = m_ayFieldInfo[m_curSelectRow].m_strFieldID;

	//查找预订人信息
	CString strVipID;
	int n = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo.size();
	for (int i = 0; i < n; i++)
	{
		if (m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
		{
			strVipID = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_strVipID;
		}
	}
	ASSERT(!strVipID.IsEmpty());

	//找到vip后查看他的个人信息
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	VipInfo* info  = pMainWnd->m_pageVip.GetVipInfo(strVipID);
	ASSERT(info);

	CString strMsg;

	strMsg.Format("场地：%s\n时间：%s\n预订人ID：%s\n姓名：%s\n电话：%s\n\n是否取消预订？"
		, strFieldID.GetBuffer()
		, m_ayTimeByIndex[nTimeIndex]
	, info->m_strID.GetBuffer()
		, info->m_strName.GetBuffer()
		, info->m_strPhone.GetBuffer());

	if(MessageBox(strMsg,"提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
		return;

	OneTimeInfo infoBook;

	OneTimeInfo* pinfo = GetFieldInfo(strFieldID, nTimeIndex);
	if (!pinfo)
	{
		return;
	}
	infoBook =  *pinfo;

	//交换信息
	infoBook.m_bBusy = FALSE;
	infoBook.m_strVipID = "";
	ChangeFieldInfo(strFieldID, infoBook);

	ShowItemField(GetDate());

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
	int			iPos;
	pos = m_listField.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要删除的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listField.GetNextSelectedItem(pos);
	CString strID		= m_listField.GetItemText(iPos, 0);

	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageField.m_pFieldData->DeleteField(strID);
}


OneTimeInfo* CDlgField::GetFieldInfo(CString strFieldID, int nTimeIndex)
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

int CDlgField::ChangeFieldInfo(CString strFieldID, OneTimeInfo& info)
{
	CMainDlg* pMainWnd = GETMAINWND;
	//修改信息，并且更新xml文件
	OneTimeInfo * pInfo = GetFieldInfo(strFieldID, info.m_nTimeIndex);
	if (!pInfo)
	{
		//如果field没有现在日期的数据，就会为空，那么添加这天
		pMainWnd->m_pageField.m_pFieldData->SetCurrentDate(GetDate());
		pMainWnd->m_pageField.m_pFieldData->AddFieldDay(strFieldID);
		UpdateFieldData();
		pInfo = GetFieldInfo(strFieldID, info.m_nTimeIndex);
	}

	*pInfo = info;//改内存中的值

	
	pMainWnd->m_pageField.m_pFieldData->ChangeFieldInfo(strFieldID, info);
	return 0;

}

void CDlgField::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	UpdateData(TRUE);
	CString strDate = GetDate();
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
	ShowItemField(strDate);
	SetPreBtnStatus();
}
