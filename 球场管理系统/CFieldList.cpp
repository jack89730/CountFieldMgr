#include "CFieldList.h"
#include "MainDlg.h"

#include "public.h"

BEGIN_MESSAGE_MAP(CFieldList, CListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

BOOL CFieldList:: PreTranslateMessage(MSG* pMsg)
{
	return CListCtrl::PreTranslateMessage(pMsg);
}

// BOOL CFieldList::PreCreateWindow(CREATESTRUCT& cs)
// {
// 	// 创建自己的窗口类，窗口不设置光标，以便根据需要进行设置 
// 	if (cs.lpszClass == NULL)
// 		cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS);
// 	//return CListCtrl::PreCreateWindow(cs);
// 	return TRUE;
// }

void CFieldList::OnMouseMove(UINT nFlags, CPoint point)
{
	CListCtrl::OnMouseMove(nFlags, point);

 	LVHITTESTINFO hi;
 	hi.pt = point;
 
 	if (SubItemHitTest(&hi) == -1)
 	{
 		//m_bBeginDrag = FALSE;
 		return;
 	}
// 
// 	int iItem = hi.iItem;
// 	int iSubItem = hi.iSubItem;
// 
// 
//  	if (!m_bBeginDrag)
//  	{
//  		return;
//  	}
// 
//  	CRect rect;
//  	GetSubItemRect(m_nBeginDragItem,m_nBeginDragSubItem,LVIR_BOUNDS,rect);
// 	rect.top += 10;
// 	rect.bottom -= 10;
// 	rect.left += 10;
// 	rect.right -= 10;
//  	rect.MoveToXY(point);
// 
//  
//  	CDC* pDC = GetDC();
//  	pDC->SetROP2(R2_NOT);
//  
//  	CGdiObject *pOldBrush = pDC->SelectStockObject(GRAY_BRUSH);
//  
//  	pDC->Rectangle(m_oldrect);
//  	pDC->Rectangle(rect);
//  
//  	pDC->SelectObject(pOldBrush);
//  
//  	m_oldrect = rect;
//  	ReleaseDC(pDC);

}

void CFieldList::OnLButtonDown(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hi;
	hi.pt = point;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	if(SubItemHitTest(&hi) != -1 )
	{
		m_nBeginDragItem = hi.iItem;
		m_nBeginDragSubItem = hi.iSubItem;
		CString strDelete;
		strDelete.Format("删除%s", pMainWnd->m_pageField.m_ayFieldInfo[hi.iItem].m_strFieldID.GetBuffer());
		pMainWnd->m_pageField.GetDlgItem(IDC_BTN_DEL)->SetWindowText(strDelete.GetBuffer());
		pMainWnd->m_pageField.GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		m_bBeginDrag = TRUE;
	}
	else
	{
		m_bBeginDrag = FALSE;
		pMainWnd->m_pageField.GetDlgItem(IDC_BTN_DEL)->SetWindowText("删除");
		pMainWnd->m_pageField.GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		return;
	}

	int iItem = hi.iItem;
	int iSubItem = hi.iSubItem;

	SetSelectionMark(iItem);  // nItemIdx为高亮显示的行号；
	SetFocus();
	EnsureVisible( iItem, TRUE );
	SetItemState( iItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );

// 	CString strTxt = GetItemText(iItem, iSubItem);
// 	if (strTxt == "闲")
// 	{
// 		HCURSOR hCur = LoadCursor( NULL , IDC_HAND);
// 		::SetCursor(hCur);
// 	}

	//CListCtrl::OnLButtonDown(nFlags, point);
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

	int iItem = hi.iItem;
	int iSubItem = hi.iSubItem;


// 	CString strTxt = GetItemText(iItem, iSubItem);
// 	if (strTxt == "闲")
// 	{
// 		HCURSOR hCur = LoadCursor( NULL , IDC_HAND) ;
// 		::SetCursor(hCur);
// 	}

	CListCtrl::OnLButtonUp(nFlags, point);

	if (!m_bBeginDrag || m_nBeginDragItem < 0 || m_nBeginDragSubItem < 0)
	{
		return;
	}

	m_bBeginDrag = FALSE;
// 
// 	//擦出旧图形
// 	CDC* pDC = GetDC();
// 	pDC->SetROP2(R2_NOT);
// 
// 	CGdiObject *pOldBrush = pDC->SelectStockObject(NULL_BRUSH);
// 
// 	pDC->Rectangle(m_oldrect);
// 
// 	pDC->SelectObject(pOldBrush);
// 
// 	ReleaseDC(pDC);
// 
// 	m_oldrect.SetRect(-1,-1,-1,-1);

// 	LVHITTESTINFO hi;
// 	hi.pt = point;
// 
	if (SubItemHitTest(&hi) == -1)
	{
		m_bBeginDrag = FALSE;
		return;
	}

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	OneTimeInfo info_begin;
	OneTimeInfo info_end;

	OneTimeInfo* pbegin = pMainWnd->m_pageField.GetFieldInfo(pMainWnd->m_pageField.m_ayFieldInfo[m_nBeginDragItem].m_strFieldID, m_nBeginDragSubItem - 1);
	if (!pbegin)
	{
		return;
	}
	info_begin =  *pbegin;

	OneTimeInfo* pend = pMainWnd->m_pageField.GetFieldInfo(pMainWnd->m_pageField.m_ayFieldInfo[iItem].m_strFieldID, iSubItem - 1);
	if (!pend)
	{
		return;
	}
	info_end = *pend;

	//如果前后两个都是空闲的不需要交换了
	if (info_begin.m_bBusy == FALSE && info_end.m_bBusy == FALSE)
	{
		return;
	}

	if (MessageBox(_T("是否交换预订信息！"), _T("提示"), MB_YESNO|MB_ICONINFORMATION|MB_TASKMODAL) != IDYES)
	{
		return;
	}

	int IndexBegin = info_begin.m_nTimeIndex;
	int IndexEnd = info_end.m_nTimeIndex;

	OneTimeInfo tmp;
	tmp = info_begin;
	info_begin = info_end;
	info_end = tmp;

	info_begin.m_nTimeIndex = IndexBegin;
	info_end.m_nTimeIndex = IndexEnd;

	//交换信息
	pMainWnd->m_pageField.ChangeFieldInfo(pMainWnd->m_pageField.m_ayFieldInfo[m_nBeginDragItem].m_strFieldID, info_begin);
	pMainWnd->m_pageField.ChangeFieldInfo(pMainWnd->m_pageField.m_ayFieldInfo[iItem].m_strFieldID, info_end);

	pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

}

void CFieldList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
// 	TCHAR lpBuffer[256];
// 
// 	LV_ITEM lvi;
// 
// 	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
// 	lvi.iItem = lpDrawItemStruct->itemID ; 
// 	lvi.iSubItem = 0;
// 	lvi.pszText = lpBuffer ;
// 	lvi.cchTextMax = sizeof(lpBuffer);
// 	VERIFY(GetItem(&lvi));
// 
// 	LV_COLUMN lvc, lvcprev ;
// 	::ZeroMemory(&lvc, sizeof(lvc));
// 	::ZeroMemory(&lvcprev, sizeof(lvcprev));
// 	lvc.mask = LVCF_WIDTH | LVCF_FMT;
// 	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;
// 
// 	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
// 	{
// 		if ( nCol > 0 ) 
// 		{
// 			// Get Previous Column Width in order to move the next display item
// 			GetColumn(nCol-1, &lvcprev) ;
// 			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
// 			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left ; 
// 		}
// 
// 		// Get the text 
// 		::ZeroMemory(&lvi, sizeof(lvi));
// 		lvi.iItem = lpDrawItemStruct->itemID;
// 		lvi.mask = LVIF_TEXT | LVIF_PARAM;
// 		lvi.iSubItem = nCol;
// 		lvi.pszText = lpBuffer;
// 		lvi.cchTextMax = sizeof(lpBuffer);
// 		VERIFY(GetItem(&lvi));
// 
// 		CDC* pDC;
// 		pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
// 
// 		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
// 		{
// 			pDC->FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT)) ; 
// 			pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
// 		}
// 		else
// 		{
// 			COLORREF col = RGB(255,0,0);
// 			CRect rect = lpDrawItemStruct->rcItem;
// 			rect.bottom -= 10;
// 			rect.top += 10;
// 			rect.left += 10;
// 			rect.right -= 10;
// 			//rect.SetRect();
// 			TRACE("%d,%d,%d,%d\n", rect.top, rect.bottom, rect.left, rect.right);
// 			pDC->FillSolidRect(&rect, col/*GetSysColor(COLOR_WINDOW)*/);
// 			pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT)); 
// 		}
// 
// 		pDC->SelectObject(GetStockObject(DEFAULT_GUI_FONT));
// 
// 		UINT   uFormat    = DT_LEFT ;
// 
// 		//::DrawText(lpDrawItemStruct->hDC, lpBuffer, strlen(lpBuffer), 
// 		//	&lpDrawItemStruct->rcItem, uFormat) ;
// 
// 		pDC->SelectStockObject(SYSTEM_FONT) ;
// 	}

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	int nTimeCount = pMainWnd->m_pageField.m_nTimeCount;

	for (int col = 0; col <= nTimeCount; col++)
	{
		CString str = GetItemText(lpDrawItemStruct->itemID,col);
		CRect rect;
		CDC*pDC = GetDC();

		GetSubItemRect(lpDrawItemStruct->itemID,col,LVIR_BOUNDS,rect);

		UINT uCode=DT_LEFT;

		COLORREF kolor = RGB(255,255,255);

		if (str == FIELD_BUSY)
		{
			kolor = 3000;
		}
		else/* if (str == FIELD_IDLE)*/
		{
			kolor = 50000;
		}

		rect.bottom -= 10;
		rect.top += 10;
		rect.left += 10;
		rect.right -= 10;

		CBrush brush(kolor);
		//pDC->FillSolidRect(&rect, kolor/*GetSysColor(COLOR_WINDOW)*/);
		if (col == 0)
		{
			pDC->DrawText(str,&rect,uCode);
			continue;
		}
		//pDC->FillRect(&rect,&brush);
		//rect.OffsetRect(10,0);
		//pDC->DrawText(str,&rect,uCode);

		//POINT pt ;
		//画按钮的外边框，它是一个半径为5的圆角矩形
		//pt.x =50;
		//pt.y =50;
		//CPen pen;
		//pen.CreatePen(0, 0, kolor);
		//CPen* hOldPen = pDC->SelectObject(&pen);
		//pDC->RoundRect(&rect, pt);
		//pDC->SelectObject(hOldPen);
		//rect.DeflateRect(CSize(GetSystemMetrics(SM_CXEDGE), GetSystemMetrics(SM_CYEDGE)));
		pDC->FillRect(rect,&brush );
		brush.DeleteObject();

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