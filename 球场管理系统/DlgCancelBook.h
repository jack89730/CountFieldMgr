#pragma once

#include "resource.h"

// CDlgCancelBook �Ի���

class CDlgCancelBook : public CDialog
{
	DECLARE_DYNAMIC(CDlgCancelBook)

public:
	CDlgCancelBook(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCancelBook();

// �Ի�������
	enum { IDD = IDD_DLG_CANCEL_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
