#pragma once
#include "resource.h"

// CDlgCharge �Ի���

class CDlgCharge : public CDialog
{
	DECLARE_DYNAMIC(CDlgCharge)

public:
	CDlgCharge(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCharge();

// �Ի�������
	enum { IDD = IDD_DLG_CHARGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CString m_strID;
	int m_nMoney;//��ǰ����ֵǰ
	int m_nCharge;//�·���ֵ���
	int m_nAfterCharge;
	int m_nTmpMoney;//�Ϸ���ֵ���
	afx_msg void OnBnClickedOk2();
	BOOL m_bIsSuccess;
	BOOL IsSuccess(){return m_bIsSuccess;};
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
