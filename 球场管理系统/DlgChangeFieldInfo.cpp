// DlgChangeFieldInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgChangeFieldInfo.h"
#include "MainDlg.h"

// CDlgChangeFieldInfo 对话框

IMPLEMENT_DYNAMIC(CDlgChangeFieldInfo, CDialog)

CDlgChangeFieldInfo::CDlgChangeFieldInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChangeFieldInfo::IDD, pParent)
	, m_strID(_T(""))
	, m_strName(_T(""))
{

}

CDlgChangeFieldInfo::~CDlgChangeFieldInfo()
{
}

void CDlgChangeFieldInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strID);
	DDX_Text(pDX, IDC_EDIT2, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgChangeFieldInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgChangeFieldInfo::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgChangeFieldInfo 消息处理程序

void CDlgChangeFieldInfo::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CMainDlg* pMain = GETMAINWND;
	FieldInfo info;
	info.m_strFieldID = m_strID;
	info.m_strFieldName = m_strName;

	//ID和名称都不能重复
	int nCount = pMain->m_pageField.m_ayFieldInfo.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		if (m_strName == pMain->m_pageField.m_ayFieldInfo[i].m_strFieldName)
		{

			MessageBox("场地名称已经存在!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}
	pMain->m_pageField.m_pFieldData->ChangeFieldInfo(info);
	OnOK();
}
