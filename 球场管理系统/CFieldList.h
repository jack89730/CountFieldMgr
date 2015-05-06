#ifndef CFieldList_h__
#define CFieldList_h__

#include "stdafx.h"
#include "fielddata.h"
class CFieldList : public CListCtrl
{
	//Ìí¼Ótooltip
public:
	CFieldList()
	{
		m_bBeginDrag = FALSE;
	}
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	virtual void PreSubclassWindow();

	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	bool ShowToolTip(const CPoint& pt) const;
	CString GetToolTipText(int nRow, int nCol);

	INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMyListCtrl)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void DrawItem(LPDRAWITEMSTRUCT);
	void SetRowHeight(int nHeight);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_nBeginDragTimeIndex;
	int m_nBeginDragFieldIndex;
	BOOL m_bBeginDrag;
	CRect m_oldrect;
	int m_nRowHeight;

	int m_nLastDragTimeIndex;
	int m_nLastDragFieldIndex;
	OneTimeBookInfo m_LastInfoBackup;
	OneTimeBookInfo m_beginDragInfo;
};

#endif // CFieldList_h__
