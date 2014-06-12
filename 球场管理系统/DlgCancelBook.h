#pragma once

#include "resource.h"

// CDlgCancelBook 对话框

class CDlgCancelBook : public CDialog
{
	DECLARE_DYNAMIC(CDlgCancelBook)

public:
	CDlgCancelBook(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCancelBook();

// 对话框数据
	enum { IDD = IDD_DLG_CANCEL_BOOK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
