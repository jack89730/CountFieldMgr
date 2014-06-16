#include "CFieldList.h"
#include "MainDlg.h"
#include "public.h"

BEGIN_MESSAGE_MAP(CFieldList, CListCtrl)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()

BOOL CFieldList:: PreTranslateMessage(MSG* pMsg)
{
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CFieldList::OnMouseMove(UINT nFlags, CPoint point)
{
	CListCtrl::OnMouseMove(nFlags, point);

 	LVHITTESTINFO hi;
 	hi.pt = point;
 
 	if (SubItemHitTest(&hi) == -1)
 	{
 		return;
 	}
}

void CFieldList::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO hi;
	hi.pt = point;

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	if(SubItemHitTest(&hi) != -1)
	{
		pMainWnd->m_pageField.m_curSelectRow = hi.iItem;
		pMainWnd->m_pageField.m_curSelectCol = hi.iSubItem;
		pMainWnd->m_pageField.OnBook();
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

	CListCtrl::OnLButtonUp(nFlags, point);

	if (!m_bBeginDrag || m_nBeginDragItem < 0 || m_nBeginDragSubItem < 0)
	{
		return;
	}

	m_bBeginDrag = FALSE;

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
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	int nTimeCount = pMainWnd->m_pageField.m_nTimeCount;

	for (int col = 0; col <= nTimeCount; col++)
	{
		CString str = GetItemText(lpDrawItemStruct->itemID,col);
		CRect rect;
		CDC*pDC = GetDC();

		GetSubItemRect(lpDrawItemStruct->itemID, col, LVIR_BOUNDS, rect);

		UINT uCode=DT_LEFT;

		COLORREF kolor = RGB(255, 255, 255);

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

		if (col == 0)
		{
			pDC->DrawText(str, &rect, uCode);
			continue;
		}
		CBrush brush(kolor);
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
