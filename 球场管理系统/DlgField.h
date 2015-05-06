#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "public.h"
#include "CFieldList.h"
#include "afxwin.h"
#include "fielddata.h"
#include "perioddata.h"
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
	void resize();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT		m_ptOld;
	CFieldList m_listField;
	void ShowItemField(CString strDate = "");
	CString m_strXML;
	afx_msg void OnNMCustomdrawListField(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnNMRClickListField(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBook();
	
	int GetTimeIndexByTime(time_t timeIn);
	CArray<CString> m_ayTimeByIndex;
	int m_curSelectField;
	int m_curSelectTimeIndex;
	afx_msg void OnBookInfo();
	afx_msg void OnCancelBook();
	CArray<OneFieldBookInfo> m_ayFieldInfo;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnAdd();
	afx_msg void OnBnClickedBtnDel();
	BOOL GetFieldInfo(CString strID, FieldInfo& out);
	OneTimeBookInfo* GetFieldBookInfo(CString strFieldID, int nTimeIndex, BOOL bAdd = FALSE);//nTimeIndex 0��ʼ���������е��±�
	OneTimeBookInfo* GetFieldBookInfo(int nFieldIndex, int nTimeIndex, BOOL bAdd = FALSE);//nTimeIndex 0��ʼ���������е��±�
	int ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info);
	COleDateTime m_date;
	afx_msg void OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult);
	CString GetDate();
	BOOL IsWeekend(CString strDate);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	int AddFieldDay(CString strFieldID);
	void SetPreBtnStatus();
	CFieldData* m_pFieldData;
	void UpdateFieldData();
	void Resize();
	void AddTimeCol();
	void MoveX(int nID, int len);
	afx_msg void OnBookPay();
	CPeriodData* m_pPeriodData;
	afx_msg void On32799();//�����
};

