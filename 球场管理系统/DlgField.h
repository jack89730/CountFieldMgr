#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "public.h"
#include "CFieldList.h"
#include "afxwin.h"
// CDlgField �Ի���

class CDlgField : public CDialog
{
	DECLARE_DYNAMIC(CDlgField)

public:
	CDlgField(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgField();

// �Ի�������
	enum { IDD = IDD_DLG_FIELD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl m_listField;//���б� //�ڴ�ֻ������������
	CFieldList m_listField;
	void ShowItemField(CString strDate);
	CString m_strXML;
	afx_msg void OnNMCustomdrawListField(NMHDR *pNMHDR, LRESULT *pResult);
	int m_nTimeCount;
	afx_msg void OnNMRClickListField(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBook();
	CArray<CString> m_ayTimeByIndex;
	//CArray<CString> m_ayField;
	int m_curSelectCol;
	int m_curSelectRow;
	afx_msg void OnBookInfo();
	afx_msg void OnCancelBook();
	CArray<OneFieldInfo> m_ayFieldInfo;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	OneTimeInfo* GetFieldInfo(CString strFieldID, int nTimeIndex);//nTimeIndex 0��ʼ���������е��±�
	int ChangeFieldInfo(CString strFieldID, OneTimeInfo& info);
	COleDateTime m_date;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	CString GetDate();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	int AddFieldDay(CString strFieldID);
	void SetPreBtnStatus();
};

