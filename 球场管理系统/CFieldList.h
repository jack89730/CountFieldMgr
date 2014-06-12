#ifndef CFieldList_h__
#define CFieldList_h__

#include "stdafx.h"
class CFieldList : public CListCtrl
{
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
public:

	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMyListCtrl)

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void DrawItem(LPDRAWITEMSTRUCT);
	void SetRowHeight(int nHeight);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_nBeginDragItem;
	int m_nBeginDragSubItem;
	BOOL m_bBeginDrag;
	CRect m_oldrect;
	int m_nRowHeight;

	
};

#endif // CFieldList_h__
