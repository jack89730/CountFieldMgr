#include "CFieldList.h"
#include "MainDlg.h"
#include "public.h"
#include "operatedata.h"
#include "app.h"

BEGIN_MESSAGE_MAP(CFieldList, CListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MEASUREITEM_REFLECT()
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
END_MESSAGE_MAP()

BOOL CFieldList:: PreTranslateMessage(MSG* pMsg)
{
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CFieldList::OnMouseMove(UINT nFlags, CPoint point)
{
	CListCtrl::OnMouseMove(nFlags, point);

	CMainDlg* pMain = GETMAINWND;

 	LVHITTESTINFO hi;
 	hi.pt = point;
 
 	if (SubItemHitTest(&hi) == -1 || !m_bBeginDrag || hi.iSubItem < 1)
 	{
 		return;
 	}

	//if (hi.iItem != m_nLastDragTimeIndex && hi.iSubItem - 1 != m_nLastDragFieldIndex)
	//{
	//	TRACE("OnMouseMove Changed\n");
	//	//选中的单位发生变化
	//	pMain->m_pageField.m_ayFieldInfo[m_nLastDragFieldIndex].m_ayTimeInfo[m_nLastDragTimeIndex] = m_LastInfoBackup;//恢复原始数据
	//	m_nLastDragFieldIndex = hi.iSubItem - 1;
	//	m_nLastDragTimeIndex = hi.iItem;
	//	m_LastInfoBackup = pMain->m_pageField.m_ayFieldInfo[m_nLastDragFieldIndex].m_ayTimeInfo[m_nLastDragTimeIndex];

	//	//让开始位置和现在位置交换信息
	//	pMain->m_pageField.m_ayFieldInfo[m_nLastDragFieldIndex].m_ayTimeInfo[m_nLastDragTimeIndex] = m_beginDragInfo;
	//	pMain->m_pageField.m_ayFieldInfo[m_nBeginDragFieldIndex].m_ayTimeInfo[m_nBeginDragTimeIndex] =  m_LastInfoBackup;

	//}
	//static CPoint oldPoint;
	//static CRect oldRect;
	//static CString strOld;

	//CDC*pDC = GetDC();
	//CRect rect;
	//rect.left = point.x + 10;
	//rect.top = point.y + 10;
	//rect.right = rect.left + 50;
	//rect.bottom = rect.top + 50;
	//UINT uCode=DT_LEFT;

	//int old = pDC->SetROP2(R2_NOTXORPEN);

	//pDC->TextOut(oldPoint.x, oldPoint.y, "Hello world!");
	//pDC->TextOut(point.x, point.y, "Hello world!");
	////pDC->DrawText("Hello world!", &rect, uCode);
	//oldPoint = point;
	//oldRect = rect;
	//	pDC->SetROP2(old);
}

void CFieldList::OnLButtonDblClk(UINT nFlags, CPoint point)
{

	LVHITTESTINFO hi;
	hi.pt = point;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	if(SubItemHitTest(&hi) != -1)
	{
		pMainWnd->m_pageField.m_curSelectTimeIndex = hi.iItem;
		pMainWnd->m_pageField.m_curSelectField = hi.iSubItem - 1;
		if (hi.iSubItem <= 0)
		{
			return;
		}
		OneTimeBookInfo* p = pMainWnd->m_pageField.GetFieldBookInfo(hi.iSubItem - 1, hi.iItem, TRUE);
		if (p->m_bBusy)
		{
			pMainWnd->m_pageField.OnBookInfo();
		}
		else
		{
			time_t nowtime_t = time(NULL);
			int nCurTimeIndex = pMainWnd->m_pageField.GetTimeIndexByTime(nowtime_t);
			CTime nowTime(nowtime_t);
			CString strTime;
			strTime.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
			bool isPast = false;
			bool isToday = false;
			if (pMainWnd->m_pageField.GetDate() < strTime)
			{
				//before today;
				isPast = true;
			}
			if (pMainWnd->m_pageField.GetDate() == strTime)
			{
				isToday = true;
			}

			if (isPast || (isToday && pMainWnd->m_pageField.m_curSelectTimeIndex < nCurTimeIndex))
			{
				return;
			}
			pMainWnd->m_pageField.OnBook();
		}
	}
}
void CFieldList::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hi;
	hi.pt = point;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	if(SubItemHitTest(&hi) != -1 )
	{
		TRACE("begin drag\n");
		m_nBeginDragTimeIndex = hi.iItem;
		m_nBeginDragFieldIndex = hi.iSubItem - 1;


		//m_beginDragInfo = pMainWnd->m_pageField.m_ayFieldInfo[m_nBeginDragFieldIndex].m_ayTimeInfo[m_nBeginDragTimeIndex];

		//m_nLastDragTimeIndex = m_nBeginDragTimeIndex;
		//m_nLastDragFieldIndex = m_nBeginDragFieldIndex;
		//m_LastInfoBackup = m_beginDragInfo;

		m_bBeginDrag = TRUE;
	}
	else
	{
		m_bBeginDrag = FALSE;
		return;
	}

	int iItem = hi.iItem;
	int iSubItem = hi.iSubItem;

	SetSelectionMark(iItem);  // nItemIdx为高亮显示的行号；
	SetFocus();
	EnsureVisible( iItem, TRUE );
	SetItemState( iItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
}


void CFieldList:: OnLButtonUp(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hi;
	hi.pt = point;

	if (SubItemHitTest(&hi) == -1)
	{
		m_bBeginDrag = FALSE;
		return;
	}

	int iEndTimeIndex = hi.iItem;
	int iEndFieldIndex = hi.iSubItem - 1;

	if ((iEndTimeIndex == m_nBeginDragTimeIndex) && (iEndFieldIndex == m_nBeginDragFieldIndex))
	{
		return;
	}

	CListCtrl::OnLButtonUp(nFlags, point);

	if (!m_bBeginDrag || m_nBeginDragTimeIndex < 0 || m_nBeginDragFieldIndex < 0)
	{
		m_bBeginDrag = FALSE;
		return;
	}

	m_bBeginDrag = FALSE;
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	OneTimeBookInfo info_begin;
	OneTimeBookInfo info_end;

	OneTimeBookInfo* pbegin = pMainWnd->m_pageField.GetFieldBookInfo(pMainWnd->m_pageField.m_ayFieldInfo[m_nBeginDragFieldIndex].m_strFieldID, m_nBeginDragTimeIndex);
	if (!pbegin)
	{
		return;
	}
	info_begin =  *pbegin;

	OneTimeBookInfo* pend = pMainWnd->m_pageField.GetFieldBookInfo(pMainWnd->m_pageField.m_ayFieldInfo[iEndFieldIndex].m_strFieldID, iEndTimeIndex);
	if (!pend)
	{
		return;
	}
	info_end = *pend;

	//如果前后两个都是空闲的不需要交换了
	if (info_begin.m_bBusy == FIELD_IDLE_DATA && info_end.m_bBusy == FIELD_IDLE_DATA)
	{
		return;
	}

	if (pbegin->m_bBusy == FIELD_PAYED_DATA || pend->m_bBusy == FIELD_PAYED_DATA)
	{
		MessageBox(_T("结账后信息不可交换！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (MessageBox(_T("是否交换预订信息！"), _T("提示"), MB_YESNO|MB_ICONINFORMATION|MB_TASKMODAL) != IDYES)
	{
		return;
	}

	int IndexBegin = info_begin.m_nTimeIndex;
	int IndexEnd = info_end.m_nTimeIndex;

	OneTimeBookInfo tmp;
	tmp = info_begin;
	info_begin = info_end;
	info_end = tmp;

	info_begin.m_nTimeIndex = IndexBegin;
	info_end.m_nTimeIndex = IndexEnd;

	//交换信息
	pMainWnd->m_pageField.ChangeFieldBookInfo(pMainWnd->m_pageField.m_ayFieldInfo[m_nBeginDragFieldIndex].m_strFieldID, info_begin);
	pMainWnd->m_pageField.ChangeFieldBookInfo(pMainWnd->m_pageField.m_ayFieldInfo[iEndFieldIndex].m_strFieldID, info_end);

	pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

	m_nBeginDragFieldIndex = -1;
	m_nBeginDragTimeIndex = -1;
	m_bBeginDrag = FALSE;

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_SWAP_BOOK_INFO;

	his_op_info.m_strInfo.Format("预定日期:%s, 场地1:%s,时段:%s, 场地2:%s,时段:%s"
		, pMainWnd->m_pageField.GetDate().GetBuffer()
		, pMainWnd->m_pageField.m_ayFieldInfo[m_nBeginDragFieldIndex].m_strFieldName.GetBuffer()
		, pMainWnd->m_pageField.m_ayTimeByIndex[info_begin.m_nTimeIndex].GetBuffer()
		, pMainWnd->m_pageField.m_ayFieldInfo[iEndFieldIndex].m_strFieldID.GetBuffer()
		, pMainWnd->m_pageField.m_ayTimeByIndex[info_end.m_nTimeIndex].GetBuffer());

	pOperate->Add(his_op_info);

}

void CFieldList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	int nFieldCount = pMainWnd->m_pageField.m_ayFieldInfo.GetSize();

	CMainDlg* pMain = GETMAINWND;
	time_t nowtime_t = time(NULL);
	int nCurTimeIndex = pMain->m_pageField.GetTimeIndexByTime(nowtime_t);
	CTime nowTime(nowtime_t);
	CString strTime;
	strTime.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
	bool isPast = false;
	bool isToday = false;
	if (pMainWnd->m_pageField.GetDate() < strTime)
	{
		//before today;
		isPast = true;
	}
	if (pMainWnd->m_pageField.GetDate() == strTime)
	{
		isToday = true;
	}

	for (int col = 0; col <= nFieldCount; col++)//0列是时间列
	{
		TRACE("Draw Item field=%d timeIndex=%d\n", col - 1, lpDrawItemStruct->itemID);
		CRect rect;
		CDC*pDC = GetDC();

		GetSubItemRect(lpDrawItemStruct->itemID, col, LVIR_BOUNDS, rect);
		rect.bottom -= 3;
		rect.top += 3;
		rect.left += 3;
		rect.right -= 3;

		UINT uCode = DT_CENTER;

		if (col == 0)
		{
			CString str = GetItemText(lpDrawItemStruct->itemID,col);
			uCode = DT_LEFT;
			pDC->DrawText(str, &rect, uCode);
			continue;
		}

		int nTimeIndex = lpDrawItemStruct->itemID;
		OneTimeBookInfo* pFieldInfo = pMain->m_pageField.GetFieldBookInfo(col - 1, lpDrawItemStruct->itemID, TRUE);


		COLORREF kolor = RGB(255, 255, 255);

		if (pFieldInfo->m_bBusy == FIELD_BUSY_DATA)
		{
			kolor = 3000;
		}
		else if (pFieldInfo->m_bBusy == FIELD_IDLE_DATA)
		{
			if (isPast || (isToday && nTimeIndex < nCurTimeIndex))
			{
				//show history
				//#CDCDC1
				kolor = RGB(205,205, 193);
			}
			else
			{
				kolor = 50000;
			}
			
		}

		else if (pFieldInfo->m_bBusy == FIELD_PAYED_DATA)
		{
			kolor = 150000;
		}

		CBrush brush(kolor);
		pDC->FillRect(rect, &brush);
		brush.DeleteObject();

		if (pFieldInfo->m_bBusy == FIELD_BUSY_DATA || pFieldInfo->m_bBusy == FIELD_PAYED_DATA)
		{
			VipInfo* pVip = pMainWnd->m_pageVip.GetVipInfo(pFieldInfo->m_strVipID);
			//if (pVip == NULL)
			//{
			//	OneTimeBookInfo tmp_info = *pFieldInfo;
			//	tmp_info.m_bBusy = FIELD_IDLE_DATA;
			//	tmp_info.m_strVipID = "";
			//	pMainWnd->m_pageField.ChangeFieldBookInfo();
			//}
			ASSERT(pVip);

			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(255,255,255));
			pDC->DrawText(pVip->m_strName, &rect, uCode);
			//continue;
		}

		ReleaseDC(pDC);
	}
}

void CFieldList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)  
{  
	lpMeasureItemStruct->itemHeight = m_nRowHeight;
}

void CFieldList::SetRowHeight(int nHeight)  
{  
	m_nRowHeight = nHeight;  
	//如果没有下面的代码，设置并不能奏效  
	CRect rcWin;  
	GetWindowRect(&rcWin);  
	WINDOWPOS wp;  
	wp.hwnd = m_hWnd;  
	wp.cx = rcWin.Width();  
	wp.cy = rcWin.Height();  
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;  
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

#include <locale.h>

BOOL CFieldList::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	CString tooltip = GetToolTipText(nRow, nCol);
	if (tooltip.IsEmpty())
		return FALSE;

	// Non-unicode applications can receive requests for tooltip-text in unicode
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
	{
		MultiByteToWideChar(CP_ACP, 0, tooltip.GetBuffer(), -1, pTTTW->szText, sizeof(pTTTW->szText)/sizeof(WCHAR));
		//mbstowcs(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
	}
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
#endif
	// If wanting to display a tooltip which is longer than 80 characters,
	// then one must allocate the needed text-buffer instead of using szText,
	// and point the TOOLTIPTEXT::lpszText to this text-buffer.
	// When doing this, then one is required to release this text-buffer again
	return TRUE;
}

namespace {
	LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR classList, LPCWSTR subApp, LPCWSTR idlist)
	{
		LRESULT lResult = S_FALSE;
		HMODULE hinstDll;
		BOOL (WINAPI *pIsThemeActive)();
		HRESULT (WINAPI *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		HANDLE (WINAPI *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		HRESULT (WINAPI *pCloseThemeData)(HANDLE hTheme);

		// Check if running on Windows XP or newer
		hinstDll = ::LoadLibrary(_T("UxTheme.dll"));
		if (hinstDll)
		{
			// Check if theme service is running
			(FARPROC&)pIsThemeActive = ::GetProcAddress( hinstDll, "IsThemeActive" );
			if( pIsThemeActive && pIsThemeActive() )
			{
				(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, "OpenThemeData");
				(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, "CloseThemeData");
				(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, "SetWindowTheme");
				if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)			
				{
					// Check is themes is available for the application
					HANDLE hTheme = pOpenThemeData(hwnd,classList);
					if (hTheme!=NULL)
					{
						VERIFY(pCloseThemeData(hTheme)==S_OK);
						// Enable Windows Theme Style
						lResult = pSetWindowTheme(hwnd, subApp, idlist);
					}
				}
			}
			::FreeLibrary(hinstDll);
		}
		return lResult;
	}
}

void CFieldList::PreSubclassWindow()
{

	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);

	// Enable Vista-look if possible
	EnableWindowTheme(m_hWnd, L"ListView", L"Explorer", NULL);


	// Disable the CToolTipCtrl of CListCtrl so it won't disturb the CWnd tooltip
	GetToolTips()->Activate(FALSE);

	// Activates the standard CWnd tooltip functionality
	VERIFY( EnableToolTips(TRUE) );
}

void CFieldList::CellHitTest(const CPoint& pt, int& nRow, int& nCol) const
{
	nRow = -1;
	nCol = -1;

	LVHITTESTINFO lvhti = {0};
	lvhti.pt = pt;
	nRow = ListView_SubItemHitTest(m_hWnd, &lvhti);	// SubItemHitTest is non-const
	nCol = lvhti.iSubItem;
	if (!(lvhti.flags & LVHT_ONITEMLABEL))
		nRow = -1;
}

bool CFieldList::ShowToolTip(const CPoint& pt) const
{
	// Lookup up the cell
	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	if (nRow!=-1 && nCol!=-1)
		return true;
	else
		return false;
}

CString CFieldList::GetToolTipText(int nRow, int nCol)
{
	if (nRow!=-1 && nCol > 0)
	{
		CString strMsg;
		CMainDlg* pMain = GETMAINWND;
		CString strFieldID = pMain->m_pageField.m_ayFieldInfo[nCol - 1].m_strFieldID;
		OneTimeBookInfo* p = pMain->m_pageField.GetFieldBookInfo(strFieldID, nRow);
		if (p)
		{
			VipInfo* pVip = pMain->m_pageVip.GetVipInfo(p->m_strVipID);
			if (p->m_bBusy == FIELD_BUSY_DATA)
			{
				
				if (pVip->m_strLevelID == VIP_LEVEL_0)
				{
					strMsg.Format("非会员,姓名:%s,电话%s", pVip->m_strName.GetBuffer(), pVip->m_strPhone.GetBuffer());
				}
				else
				{
					strMsg.Format("会员ID:%s,姓名:%s", pVip->m_strID.GetBuffer(), pVip->m_strName.GetBuffer());
				}
				
				if (!p->m_strBoxKey.IsEmpty())
				{
					CString strTmp;
					strTmp.Format(",柜子号:%s", p->m_strBoxKey.GetBuffer());
					strMsg += strTmp;
				}
			}
			else if (p->m_bBusy == FIELD_PAYED_DATA)
			{
				if (pVip->m_strLevelID == VIP_LEVEL_0)
				{
					strMsg.Format("已付款,非会员,姓名:%s,电话%s", pVip->m_strName.GetBuffer(), pVip->m_strPhone.GetBuffer());
				}
				else
				{
					strMsg.Format("已付款,会员ID:%s,姓名:%s", pVip->m_strID.GetBuffer(), pVip->m_strName.GetBuffer());
				}
			}
			else
			{
				strMsg = "空闲，可预订";
			}
			return strMsg;
		}
	}
	
	return CString("");
}



INT_PTR CFieldList::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	if (!ShowToolTip(pt))
		return -1;

	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	//Get the client (area occupied by this control
	RECT rcClient;
	GetClientRect( &rcClient );

	//Fill in the TOOLINFO structure
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT) (nRow * 1000 + nCol);
	pTI->lpszText = LPSTR_TEXTCALLBACK;	// Send TTN_NEEDTEXT when tooltip should be shown
	pTI->rect = rcClient;

	return pTI->uId; // Must return a unique value for each cell (Marks a new tooltip)
}