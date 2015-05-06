// DlgCheckOut.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCheckOut.h"
#include "resource.h"
#include "DlgFindVip.h"
#include "MainDlg.h"
#include "public.h"
#include "operatedata.h"
#include "app.h"
// CDlgCheckOut 对话框

IMPLEMENT_DYNAMIC(CDlgCheckOut, CDialog)

CDlgCheckOut::CDlgCheckOut(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCheckOut::IDD, pParent)
	, m_strTotalMoney(_T(""))
	, m_strPay(_T(""))
	, m_strVip(_T(""))
{

}

CDlgCheckOut::~CDlgCheckOut()
{
}

void CDlgCheckOut::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTotalMoney);
	DDX_Text(pDX, IDC_EDIT2, m_strPay);
	DDX_Control(pDX, IDC_RADIO1, m_radiovip);
	DDX_Control(pDX, IDCANCEL, m_radiocash);
	DDX_Control(pDX, IDC_COMBO1, m_comboVip);
}


BEGIN_MESSAGE_MAP(CDlgCheckOut, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgCheckOut::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CDlgCheckOut::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDlgCheckOut::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BTN_FIND, &CDlgCheckOut::OnBnClickedBtnFind)
	ON_CBN_EDITCHANGE(IDC_COMBO1, &CDlgCheckOut::OnCbnEditchangeCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CDlgCheckOut::OnCbnKillfocusCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgCheckOut::OnCbnSelchangeCombo1)
ON_CBN_SELENDCANCEL(IDC_COMBO1, &CDlgCheckOut::OnCbnSelendcancelCombo1)
ON_CBN_CLOSEUP(IDC_COMBO1, &CDlgCheckOut::OnCbnCloseupCombo1)
ON_CBN_EDITUPDATE(IDC_COMBO1, &CDlgCheckOut::OnCbnEditupdateCombo1)
ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgCheckOut::OnCbnSelchangeCombo1)
//ON_CBN_SELENDOK(IDC_COMBO1, &CDlgCheckOut::OnCbnSelendokCombo1)
END_MESSAGE_MAP()


// CDlgCheckOut 消息处理程序

BOOL CDlgCheckOut::OnInitDialog()
{
	CDialog::OnInitDialog();
	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);//选
	((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(FALSE);


	//默认vip付款
	GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC_MONEY)->SetWindowText("余额:");
	ShowAllVip();

	
	GetDlgItem(IDC_COMBO1)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_VIP)->ShowWindow(TRUE);

	GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_MONEY)->ShowWindow(FALSE);
	
	return TRUE;

}

void CDlgCheckOut::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int nMoney = atoi(m_strTotalMoney.GetBuffer());


	CMainDlg* pMainWnd = GETMAINWND;
	BOOL bVip = FALSE;
	if (((CButton *)GetDlgItem(IDC_RADIO2))->GetCheck())
	{
		int nPay = atoi(m_strPay.GetBuffer());
		if (nPay < nMoney)
		{
			MessageBox("金额不足!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}

		CString strMsg;
		strMsg.Format("找零：%d", nPay - nMoney);
		MessageBox(strMsg, "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	}
	else
	{
		bVip = TRUE;
		//vip付款
		CString strVipId;
		GetDlgItemText(IDC_COMBO1, strVipId);
		int n = strVipId.Find(' ', 0);
		m_strVip = strVipId.Left(n);

		VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
		if (pInfo == NULL)
		{
			MessageBox("会员不存在", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		if (pInfo->m_bLock)
		{
			MessageBox("该会员已被锁定，请先解锁！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}

		int nPay = atoi(pInfo->m_strMoney.GetBuffer());
		if (nPay < nMoney)
		{
			MessageBox("金额不足!", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}

		pInfo->m_strMoney.Format("%d", nPay - nMoney);
		pMainWnd->m_pageVip.ChangeVipInfo(*pInfo);
		pMainWnd->m_pageVip.ShowItemVip();
		CString strMsg;
		strMsg.Format("付款成功，余额：%d", nPay - nMoney);
		MessageBox(strMsg, "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	}
	

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_BUYFOOD;
	if (bVip)
	{
		VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
		his_op_info.m_strInfo.Format("会员ID:%s,姓名:%s,消费金额:%d", pInfo->m_strID.GetBuffer(), pInfo->m_strName.GetBuffer(), nMoney);
	}
	else
	{
		int nPay = atoi(m_strPay.GetBuffer());
		his_op_info.m_strInfo.Format("现金付款:%d,总金额:%d,找零:%d", nPay, nMoney, nPay - nMoney);
	}
	
	pOperate->Add(his_op_info);

	OnOK();
}

void CDlgCheckOut::ShowAllVip()
{
	m_ayVip.RemoveAll();
	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++,row++)
	{
		if (ayVip[i].m_strLevelID == VIP_LEVEL_0)
		{
			//临时账号不显示
			continue;
		}
		CString str;
		str.Format("%s %s", ayVip[i].m_strID.GetBuffer(), ayVip[i].m_strName.GetBuffer());
		m_comboVip.AddString(str);
		m_ayVip.Add(ayVip[i]);
	}
}

void CDlgCheckOut::OnBnClickedRadio1()
{
	//vip
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_COMBO1)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_VIP)->ShowWindow(TRUE);

	GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_MONEY)->ShowWindow(FALSE);


	GetDlgItem(IDC_EDIT2)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT2)->SetWindowText("0");
	m_comboVip.SetCurSel(-1);
	GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
	GetDlgItem(IDC_STATIC_MONEY)->SetWindowText("余额:");
}

void CDlgCheckOut::OnBnClickedRadio2()
{
	//现金
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_COMBO1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC_VIP)->ShowWindow(FALSE);

	GetDlgItem(IDC_EDIT2)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_MONEY)->ShowWindow(TRUE);

	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->SetWindowText("0");
	m_comboVip.SetCurSel(-1);
	GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_STATIC_MONEY)->SetWindowText("现金:");
	m_strVip = "";
}

void CDlgCheckOut::OnBnClickedBtnFind()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgFindVip dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_strVip = dlg.m_strResult;
		CMainDlg* pMainWnd = GETMAINWND;
		VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
		m_strPay = pInfo->m_strMoney;
		UpdateData(FALSE);
	}
}

void CDlgCheckOut::OnCbnEditchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	
	CString strKey;
	GetDlgItemText(IDC_COMBO1, strKey);

	m_comboVip.ResetContent();
	//m_comboVip.ShowDropDown(FALSE);//会导致输入框字符先消失再出现

	if (strKey.IsEmpty())
	{
		ShowAllVip();
		HCURSOR hCur = LoadCursor(NULL , IDC_ARROW);
		::SetCursor(hCur);
		m_comboVip.SetCurSel(-1);
		m_comboVip.ShowDropDown(TRUE);
		return;
	}

	SetDlgItemText(IDC_COMBO1, strKey);
	CMainDlg* pMainWnd = GETMAINWND;

	CArray<VipInfo>& ayVip = pMainWnd->m_pageVip.m_ayVip;
	int nVipCount = ayVip.GetSize();
	int row = 0;
	for (int i = 0; i < nVipCount; i++)
	{
		CString& strTmpID = ayVip[i].m_strID;
		CString& strTmpName = ayVip[i].m_strName;
		//名字或者id匹配都可以
		if (strTmpID.Find(VIP_TMP_NAME) != -1)
		{
			continue;
		}
		if (strTmpID.Find(strKey) != -1 || strTmpName.Find(strKey) != -1)
		{
			//找到了
			m_comboVip.AddString(strTmpID + " " + strTmpName);
		}
	}


	HCURSOR hCur = LoadCursor(NULL , IDC_ARROW) ;
	::SetCursor(hCur);
	m_comboVip.SetCurSel(-1);
	m_comboVip.ShowDropDown(TRUE);

	SetDlgItemText(IDC_COMBO1, strKey);
	m_comboVip.SetEditSel(strKey.GetLength(),strKey.GetLength());
}

void CDlgCheckOut::OnCbnKillfocusCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}

//void CDlgCheckOut::OnCbnSelchangeCombo1()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//}

void CDlgCheckOut::OnCbnSelendcancelCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
 	UpdateData(TRUE);
 
	CString strVip;
	GetDlgItemText(IDC_COMBO1, strVip);
	m_strVip = strVip.Left(strVip.Find(' '));
 	CMainDlg* pMainWnd = GETMAINWND;
 	VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
 	if (pInfo)
 	{
 		SetDlgItemText(IDC_EDIT2, pInfo->m_strMoney);
 	}
 	else
 	{
 		SetDlgItemText(IDC_EDIT2, "0");
 	}
}

void CDlgCheckOut::OnCbnCloseupCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
 	UpdateData(TRUE);
 
	CString strVip;
 	GetDlgItemText(IDC_COMBO1, strVip);
	m_strVip = strVip.Left(strVip.Find(' '));
 	CMainDlg* pMainWnd = GETMAINWND;
 	VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
 	if (pInfo)
 	{
 		SetDlgItemText(IDC_EDIT2, pInfo->m_strMoney);
 	}
 	else
 	{
 		SetDlgItemText(IDC_EDIT2, "0");
 	}
}

void CDlgCheckOut::OnCbnEditupdateCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CString strVip;
	GetDlgItemText(IDC_COMBO1, strVip);
	m_strVip = strVip.Left(strVip.Find(' '));
	CMainDlg* pMainWnd = GETMAINWND;
	VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
	if (pInfo)
	{
		SetDlgItemText(IDC_EDIT2, pInfo->m_strMoney);
	}
	else
	{
		SetDlgItemText(IDC_EDIT2, "0");
	}
}

void CDlgCheckOut::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

	int nIndex = m_comboVip.GetCurSel();  
	int nCount = m_comboVip.GetCount();  
	if ((nIndex != CB_ERR) && (nCount > 1))  
	{  
		CString strVipId;
		GetDlgItemText(IDC_COMBO1, strVipId);
		int n = strVipId.Find(' ', 0);
		m_strVip = strVipId.Left(n);
		
	}  

	CMainDlg* pMainWnd = GETMAINWND;
	VipInfo* pInfo = pMainWnd->m_pageVip.GetVipInfo(m_strVip);
	if (pInfo)
	{
		SetDlgItemText(IDC_EDIT2, pInfo->m_strMoney);
	}
	else
	{
		SetDlgItemText(IDC_EDIT2, "0");
	}
}

//void CDlgCheckOut::OnCbnSelendokCombo1()
//{
//}
