#pragma once
#include "resource.h"

// CDlgBookKey 对话框

class CDlgBookKey : public CDialog
{
	DECLARE_DYNAMIC(CDlgBookKey)

public:
	CDlgBookKey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBookKey();

// 对话框数据
	enum { IDD = IDD_DLG_ADD_KEY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strKey;
	afx_msg void OnBnClickedOk();
};
