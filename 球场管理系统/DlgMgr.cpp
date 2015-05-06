// DlgMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgMgr.h"
#include "public.h"
#include "DlgModifyPrice.h"
#include "MainDlg.h"
#include "DlgFoodMgr.h"
#include "history.h"
#include "DlgFieldMgr.h"
#include "DlgVipHistory.h"
#include "DlgNewPassword.h"
#include "DlgPasssword.h"
#include<iomanip>
#include<fstream>
#include "DlgChangePwd.h"
#include "DlgUserMgr.h"
#include "DlgInit.h"
#include "DlgAddVipLevel.h"
#include "app.h"
#include "operatedata.h"
#include "DlgOperate.h"

// CDlgMgr �Ի���

IMPLEMENT_DYNAMIC(CDlgMgr, CDialog)

CDlgMgr::CDlgMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMgr::IDD, pParent)
	, m_nInitFieldCount(0)
	, m_dateStart(COleDateTime::GetCurrentTime())
	, m_dateStop(COleDateTime::GetCurrentTime())
{

}

CDlgMgr::~CDlgMgr()
{
}

void CDlgMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listVipPrice);
	DDX_Control(pDX, IDC_COMBO1, m_comboStart);
	DDX_Control(pDX, IDC_COMBO3, m_comboStop);
	DDX_Control(pDX, IDC_CHECK1, m_checkWeekend);
	DDX_Control(pDX, IDC_CHECK2, m_checkDisableEdit);
	DDX_Control(pDX, IDC_STATIC_shiijan, m_timemgr);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_dateStart);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_dateStop);
	DDX_Control(pDX, IDC_TYPE, m_ComboType);
	DDX_Control(pDX, IDC_VIP, m_comboVip);
	DDX_Control(pDX, IDC_LIST2, m_listReport);
	DDX_Control(pDX, IDC_NAME_BY_TYPE, m_comboNameByType);
}


BEGIN_MESSAGE_MAP(CDlgMgr, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgMgr::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgMgr::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK1, &CDlgMgr::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlgMgr::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CDlgMgr::OnCbnSelchangeCombo3)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_INITFIELD, &CDlgMgr::OnBnClickedBtnInitfield)
	ON_BN_CLICKED(IDC_BTN_FOOD_MGR, &CDlgMgr::OnBnClickedBtnFoodMgr)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgMgr::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BTN_FIELD_MGR, &CDlgMgr::OnBnClickedBtnFieldMgr)
	ON_BN_CLICKED(IDC_BTN_FOOD_MGR2, &CDlgMgr::OnBnClickedBtnVipMgr)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlgMgr::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BTN_USER_MGR, &CDlgMgr::OnBnClickedBtnUserMgr)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDlgMgr::OnNMRClickList1)
	ON_COMMAND(ID_ADD_VIP_LEVEL, &CDlgMgr::OnAddVipLevel)
	ON_COMMAND(ID_DEL_VIP_LEVEL, &CDlgMgr::OnDelVipLevel)
	ON_CBN_SELCHANGE(IDC_TYPE, &CDlgMgr::OnCbnSelchangeType)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CDlgMgr::OnDtnDatetimechangeDatetimepicker1)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CDlgMgr::OnDtnDatetimechangeDatetimepicker2)
	ON_CBN_EDITCHANGE(IDC_NAME_BY_TYPE, &CDlgMgr::OnCbnEditchangeNameByType)
	ON_CBN_SELCHANGE(IDC_VIP, &CDlgMgr::OnCbnSelchangeVip)
	ON_BN_CLICKED(IDC_BTN_ACTION, &CDlgMgr::OnBnClickedBtnAction)
END_MESSAGE_MAP()


void CDlgMgr::OnTimer(UINT_PTR nIDEvent)   //��ʾ����ʱ��
{
	// TODO: Add your message handler code here and/or call default

	//û�г��زſ��Գ�ʼ��
	CMainDlg* pMainWnd = GETMAINWND;
	int nField = pMainWnd->m_pageField.m_ayFieldInfo.GetSize();
	if (nField)
	{
		GetDlgItem(IDC_BTN_INITFIELD)->EnableWindow(FALSE);
		
	}
	else
	{
		GetDlgItem(IDC_BTN_INITFIELD)->EnableWindow(TRUE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgMgr::ShowGoldTime()
{
	m_comboStart.ResetContent();
	m_comboStop.ResetContent();

	m_pPeriodData->Read(m_periodInfo);

	if (m_periodInfo.m_bInit)
	{
		int nTimeCount = m_periodInfo.m_nHalfCount;
		int nStartMin = m_periodInfo.m_nStartMin;
		for (int i = 0; i < nTimeCount+1; i++)
		{
			CString strTmp;
			strTmp.Format("%02d:%02d", nStartMin/60,nStartMin%60);
			nStartMin += 30;
			m_comboStart.AddString(strTmp);
			m_comboStop.AddString(strTmp);
		}

		m_pPeriodData->Read(m_periodInfo);
		m_comboStart.SetCurSel(m_periodInfo.m_nGoldStart);
		m_comboStop.SetCurSel(m_periodInfo.m_nGoldStop);
		m_checkWeekend.SetCheck(m_periodInfo.m_bWeekend);
		m_checkDisableEdit.SetCheck(TRUE);
		m_comboStart.EnableWindow(FALSE);
		m_comboStop.EnableWindow(FALSE);
		m_checkWeekend.EnableWindow(FALSE);
	}
	else
	{
		m_checkDisableEdit.SetCheck(TRUE);
		m_comboStart.EnableWindow(FALSE);
		m_comboStop.EnableWindow(FALSE);
		m_checkWeekend.SetCheck(FALSE);
		m_checkWeekend.EnableWindow(FALSE);
	}
}

// CDlgMgr ��Ϣ�������
BOOL CDlgMgr::OnInitDialog()
{
	m_nSelectType = SELECT_TYPE_ALL;
	m_nCurSelectLevelIndex = -1;
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);

	m_strPasswordFile = TL_GetModulePath(NULL) + "password";

	m_pLevelData = GetLevelData;
	m_pPeriodData = GetPeriodData;

	CDialog::OnInitDialog();

	m_listVipPrice.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listVipPrice.InsertColumn(k++, _T("�ȼ�"),	LVCFMT_CENTER, 200);
	m_listVipPrice.InsertColumn(k++, _T("�ȼ�����"),	LVCFMT_CENTER, 200);
	m_listVipPrice.InsertColumn(k++, _T("��ͨʱ�μ۸�"),	LVCFMT_CENTER, 250);
	m_listVipPrice.InsertColumn(k++, _T("�ƽ�ʱ�μ۸�"),	LVCFMT_CENTER, 250);

	ShowPriceList();

	CMainDlg* pMain = GETMAINWND;
	
	//����ʱ��˵����ʼ���ɹ���
	ShowGoldTime();
	
	UpdateData(FALSE);
	
	CRect rect;    
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	m_ptOld.x=rect.right-rect.left;
	m_ptOld.y=rect.bottom-rect.top;

	SetTimer(1, 1000, NULL);

	ShowFieldAndFoodCombo();
	UpdateVipLevelCombo();

	m_listReport.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);  
	m_listReport.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 80);
	m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 200);
	m_listReport.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 120);
	m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);
	m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);
	m_listReport.InsertColumn(k++, _T("�ܶ�"),	LVCFMT_CENTER, 80);
	m_listReport.InsertColumn(k++, _T("����ʱ��"),	LVCFMT_CENTER, 200);
	m_listReport.InsertColumn(k++, _T("������ID"),	LVCFMT_CENTER, 100);
	m_listReport.InsertColumn(k++, _T("����������"),	LVCFMT_CENTER, 150);
	m_listReport.InsertColumn(k++, _T("��Ա�ȼ�"),	LVCFMT_CENTER, 100);

	m_ComboType.SetCurSel(0);

	if (!ISADMIN)
	{
		int woc;
		HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�  
		while(hwndChild)    
		{    
			woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
			if (woc != IDC_BTN_USER_MGR && woc != IDC_STATIC_USERS)
			{
				GetDlgItem(woc)->EnableWindow(FALSE);  
			} 
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
		}
	}
	
	return TRUE;
}	

void CDlgMgr::UpdateVipLevelCombo()
{
	m_comboVip.ResetContent();
	m_comboVip.AddString("ȫ��");
	int n = m_ayPrice.GetSize();
	int nSel = 0;
	for (int i = 0; i < n; i++)
	{
		CString str;
		str.Format("%s-%s", m_ayPrice[i].m_strID.GetBuffer(), m_ayPrice[i].m_strName.GetBuffer());
		m_comboVip.AddString(str);
	}
	m_comboVip.SetCurSel(0);
}
void CDlgMgr::ShowFieldAndFoodCombo()
{
	m_comboNameByType.ResetContent();
	m_aySelectID.RemoveAll();
	CMainDlg* pMain = GETMAINWND;
	int nField = pMain->m_pageField.m_ayFieldInfo.GetSize();
	m_comboNameByType.AddString("ȫ��");
	m_comboNameByType.SetCurSel(0);
	if (m_nSelectType == SELECT_TYPE_ALL)
	{
		return;
	}

	if (m_nSelectType == SELECT_TYPE_FIELD)
	{
		for (int i = 0; i < nField; i++)
		{
			m_comboNameByType.AddString(pMain->m_pageField.m_ayFieldInfo[i].m_strFieldName);
			m_aySelectID.Add(pMain->m_pageField.m_ayFieldInfo[i].m_strFieldID);
		}
	}
	int nCout = pMain->m_pageFood.m_ayFood.GetSize();
	if (m_nSelectType == SELECT_TYPE_FOOD)
	{
		for (int i = 0; i < nCout; i++)
		{
			m_comboNameByType.AddString(pMain->m_pageFood.m_ayFood[i].m_strName);
			m_aySelectID.Add(pMain->m_pageFood.m_ayFood[i].m_strID);
		}
	}
	
}
void CDlgMgr::ShowPriceList()
{
	m_listVipPrice.DeleteAllItems();
	m_ayPrice.RemoveAll();

	if (ERROR_OK != m_pLevelData->GetAllData(m_ayPrice))
	{
		return;
	}

	int nCount = m_ayPrice.GetSize();
	for (int i = 0; i < nCount; i++)
	{
		OnePriceInfo& info = m_ayPrice[i];
		int j = 0;
		m_listVipPrice.InsertItem(i, info.m_strID);
		m_listVipPrice.SetItemText(i, ++j, info.m_strName);
		m_listVipPrice.SetItemText(i, ++j, info.m_strPriceLow);
		m_listVipPrice.SetItemText(i, ++j, info.m_strPriceHigh);
	}
	UpdateVipLevelCombo();
}
void CDlgMgr::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	if (pNotifier->iItem < 0)
	{
		return;
	}

	CDlgModifyPrice dlg;
	dlg.m_strID = m_listVipPrice.GetItemText(pNotifier->iItem, 0);
	dlg.m_strLevelName = m_listVipPrice.GetItemText(pNotifier->iItem, 1);
	dlg.m_strLowPrice = m_listVipPrice.GetItemText(pNotifier->iItem, 2);
	dlg.m_strHighPrice = m_listVipPrice.GetItemText(pNotifier->iItem, 3);
	dlg.DoModal();
	ShowPriceList();
}

OnePriceInfo* CDlgMgr::GetPriceInfo(CString strID)
{
	int nCount = m_ayPrice.GetCount();

	int j = 0;
	for (; j < nCount; ++j)
	{
		if (m_ayPrice[j].m_strID == strID)
		{
			break;
		}
	}

	if (j < nCount)
	{
		return &m_ayPrice[j];
	}

	return NULL; 
}

int CDlgMgr::ChangePriceInfo(OnePriceInfo& info)
{
	OnePriceInfo * pInfo = GetPriceInfo(info.m_strID);
	if (!pInfo)
	{
		return -1;
	}
	

	if (ERROR_OK == m_pLevelData->ChangePriceInfo(info))
	{
		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_CHANGE_VIP_LEVEL;
		his_op_info.m_strInfo.Format("�ȼ���ֵ:%s", info.m_strID.GetBuffer());
		if (info.m_strName != pInfo->m_strName)
		{
			CString strTmp;
			strTmp.Format(";������:%s,������:%s", pInfo->m_strName.GetBuffer(), info.m_strName.GetBuffer());
			his_op_info.m_strInfo += strTmp;
		}

		if (info.m_strPriceHigh != pInfo->m_strPriceHigh)
		{
			CString strTmp;
			strTmp.Format(";�ɻƽ�۸�:%s,�»ƽ�۸�:%s", pInfo->m_strPriceHigh.GetBuffer(), info.m_strPriceHigh.GetBuffer());
			his_op_info.m_strInfo += strTmp;
		}

		if (info.m_strPriceLow!= pInfo->m_strPriceLow)
		{
			CString strTmp;
			strTmp.Format(";�ɷǻƽ�۸�:%s,�·ǻƽ�۸�:%s", pInfo->m_strPriceLow.GetBuffer(), info.m_strPriceLow.GetBuffer());
			his_op_info.m_strInfo += strTmp;
		}

		pOperate->Add(his_op_info);

		*pInfo = info;//���ڴ��е�ֵ
		ShowPriceList();
		return ERROR_OK;
	}



	return ERROR_UNKNOW_ERROR;
}

void CDlgMgr::OnBnClickedCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_checkDisableEdit.GetCheck())
	{
		//����
		m_comboStart.EnableWindow(FALSE);
		m_comboStop.EnableWindow(FALSE);
		m_checkWeekend.EnableWindow(FALSE);
		UpdateData(FALSE);
	}
	else
	{
		m_comboStart.EnableWindow(TRUE);
		m_comboStop.EnableWindow(TRUE);
		m_checkWeekend.EnableWindow(TRUE);
		UpdateData(FALSE);
	}
}

void CDlgMgr::OnBnClickedCheck1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_checkWeekend.GetCheck())
	{
		m_periodInfo.m_bWeekend = TRUE;
	}
	else
	{
		m_periodInfo.m_bWeekend = FALSE;
	}
	m_pPeriodData->Set(m_periodInfo);
}

void CDlgMgr::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nStart = m_comboStart.GetCurSel();
	int nStop = m_comboStop.GetCurSel();
	if (nStart >= nStop)
	{
		MessageBox("��ʼʱ��ҪС�ڽ���ʱ�䣡", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		m_comboStart.SetCurSel(m_periodInfo.m_nGoldStart);
		m_comboStop.SetCurSel(m_periodInfo.m_nGoldStop);
		UpdateData(FALSE);
		return;
	}
	m_periodInfo.m_nGoldStart = nStart;
	m_periodInfo.m_nGoldStop = nStop;
	m_pPeriodData->Set(m_periodInfo);
}

void CDlgMgr::OnCbnSelchangeCombo3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nStart = m_comboStart.GetCurSel();
	int nStop = m_comboStop.GetCurSel();
	if (nStart >= nStop)
	{
		MessageBox("��ʼʱ��ҪС�ڽ���ʱ�䣡", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		m_comboStart.SetCurSel(m_periodInfo.m_nGoldStart);
		m_comboStop.SetCurSel(m_periodInfo.m_nGoldStop);
		UpdateData(FALSE);
		return;
	}
	m_periodInfo.m_nGoldStart = nStart;
	m_periodInfo.m_nGoldStop = nStop;
	m_pPeriodData->Set(m_periodInfo);
}

BOOL CDlgMgr::IsGoldTime(int index)
{
	if (m_periodInfo.m_bWeekend)
	{
		CTime nowTime(time(NULL));
		int day = nowTime.GetDayOfWeek();
		if (day == 1 || day == 7)
		{
			return TRUE;
		}
	}

	if (index >= m_periodInfo.m_nGoldStart && index <= m_periodInfo.m_nGoldStop)
	{
		return TRUE;
	}
	return FALSE;
}

void CDlgMgr::resize()
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;    
	GetClientRect(&recta);     //ȡ�ͻ�����С  

	CSize sz = recta.Size();
	if (sz.cx == 0 && sz.cy == 0)
	{
		//��С��
		return;
	}

	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/m_ptOld.x;
	fsp[1]=(float)Newp.y/m_ptOld.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //���Ͻ�
	CPoint OldBRPoint,BRPoint; //���½�
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //�г����пؼ�  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//ȡ��ID
		GetDlgItem(woc)->GetWindowRect(Rect);  
		ScreenToClient(Rect);  
		OldTLPoint = Rect.TopLeft();  
		TLPoint.x = long(OldTLPoint.x*fsp[0]);  
		TLPoint.y = long(OldTLPoint.y*fsp[1]);  
		OldBRPoint = Rect.BottomRight();  
		BRPoint.x = long(OldBRPoint.x *fsp[0]);  
		BRPoint.y = long(OldBRPoint.y *fsp[1]);  
		Rect.SetRect(TLPoint,BRPoint);  
		if (woc == IDC_COMBO1 || woc == IDC_COMBO3 || woc == IDC_NAME_BY_TYPE || woc == IDC_VIP || woc == IDC_TYPE)
		{
			Rect.bottom += 100;
		}
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	m_ptOld=Newp;

}

void CDlgMgr::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		resize();
	}
}
void CDlgMgr::OnBnClickedBtnInitfield()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDlgInit dlg;
	dlg.DoModal();
	ShowGoldTime();

}

void CDlgMgr::OnBnClickedBtnFoodMgr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgFoodMgr dlg;
	dlg.DoModal();
	ShowFieldAndFoodCombo();
}

void CDlgMgr::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	CHisData* pHisData = NULL;
	if (DATA_FROM_TYPE == DATA_FROM_MYSQL)
	{
		pHisData = GetHistoryData;
	}
	else
	{
		pHisData = NULL;
		ASSERT(FALSE && "������Դ��֧�֣�");
		return;
	}
	m_listReport.DeleteAllItems();

	pHisData->GetAllData(fastdelegate::MakeDelegate(this, &CDlgMgr::Hook), m_dateStart, m_dateStop);
	if (m_listReport.GetItemCount())
	{
		GetDlgItem(IDC_BUTTON5)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
	}
}

void CDlgMgr::Hook(int nType, void* pData)
{
	UpdateData(TRUE);

	if (nType == HIS_TYPE_CHARGE)
	{
		return;
	}
	if (m_nSelectType == SELECT_TYPE_FIELD && nType != HIS_TYPE_FIELD)
	{
		return;
	}
	if (m_nSelectType == SELECT_TYPE_FOOD && nType != HIS_TYPE_BUYFOOD)
	{
		return;
	}

	CMainDlg* pMain = GETMAINWND;

	if (nType == HIS_TYPE_FIELD)
	{
		HisBuyFieldInfo* p = (HisBuyFieldInfo*)pData;

		//����������
		int nComboIndex = m_comboNameByType.GetCurSel();
		if (nComboIndex)
		{
			//ѡ�Ĳ���ȫ��
			CString strField;
			strField = m_aySelectID[nComboIndex-1];
			if (p->m_strFieldID != strField)
			{
				return;
			}
		}

		BOOL bDel = FALSE;
		VipInfo* pVipInfo = pMain->m_pageVip.GetVipInfo(p->m_strVipID);
		if (pVipInfo==NULL)
		{
			pVipInfo = pMain->m_pageVip.GetDelVipInfo(p->m_strVipID);
			if (pVipInfo == NULL)
			{
				return;
			}
			bDel = TRUE;
		}
		FieldInfo fieldInfo;
		if (pMain->m_pageField.GetFieldInfo(p->m_strFieldID, fieldInfo)==FALSE)
		{
			//ɾ���ĳ��ز���ʾ
			return;
		}
		

		int nComVip = m_comboVip.GetCurSel();
		if (nComVip)
		{
			//ѡ�Ĳ���ȫ��
			CString strVipLevel = m_ayPrice[nComVip - 1].m_strID;
			if (pVipInfo->m_strLevelID != strVipLevel)
			{
				return;
			}
		}


		//m_listReport.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);  
		//m_listReport.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 200);
		//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("�ܶ�"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("����ʱ��"),	LVCFMT_CENTER, 260);
		//m_listReport.InsertColumn(k++, _T("������ID"),	LVCFMT_CENTER, 200);
		//m_listReport.InsertColumn(k++, _T("����������"),	LVCFMT_CENTER, 200);
		//m_listReport.InsertColumn(k++, _T("��Ա�ȼ�"),	LVCFMT_CENTER, 100);


		int j = 0;
		int i = m_listReport.GetItemCount();
		m_listReport.InsertItem(i, "����");
		m_listReport.SetItemText(i, ++j, fieldInfo.m_strFieldName);
		m_listReport.SetItemText(i, ++j, p->m_strTime);
		m_listReport.SetItemText(i, ++j, p->m_strMoney);
		m_listReport.SetItemText(i, ++j, "1");
		m_listReport.SetItemText(i, ++j, p->m_strMoney);//�ܶ�
		CString strPayTime;
		strPayTime.Format("%s-%s", p->m_time.m_strDate.GetBuffer(), p->m_time.m_strTime.GetBuffer());
		m_listReport.SetItemText(i, ++j, strPayTime);
		if (bDel)
		{
			CString str;
			str = p->m_strVipID + "(��ɾ)";
			m_listReport.SetItemText(i, ++j, str);
		}
		else
		{
			m_listReport.SetItemText(i, ++j, p->m_strVipID);
		}
		m_listReport.SetItemText(i, ++j, pVipInfo->m_strName);
		m_listReport.SetItemText(i, ++j, pVipInfo->m_strLevelID);
	}

	if (nType == HIS_TYPE_BUYFOOD)
	{
		HisBuyFoodInfo* p = (HisBuyFoodInfo*)pData;

		//����������
		int nComboIndex = m_comboNameByType.GetCurSel();
		if (nComboIndex)
		{
			//ѡ�Ĳ���ȫ��
			CString strFood;
			FoodInfo* pFood = pMain->m_pageFood.GetFoodInfo(p->m_strFoodID);
			if (pFood)
			{
				//ûɾ��
				p->m_strFoodName = pFood->m_strName;//ȡ���µ����֣���ֹ������
			}
			m_comboNameByType.GetLBText(nComboIndex, strFood);
			if (p->m_strFoodName != strFood)
			{
				return;
			}
		}

		CMainDlg* pMain = GETMAINWND;
		VipInfo* pVipInfo = pMain->m_pageVip.GetVipInfo(p->m_strVipID);
		if (pVipInfo==NULL && !p->m_strVipID.IsEmpty())//vipid is null,payed by cash
		{
			//ɾ���Ļ�Ա����ʾ
			return;
		}

		VipInfo tmp_vip;
		tmp_vip.m_strID = "��";
		tmp_vip.m_strName = "�ֽ�";
		tmp_vip.m_strLevelID = "0";
		if (pVipInfo == NULL)
		{
			pVipInfo = &tmp_vip;
		}
		int nComVip = m_comboVip.GetCurSel();

		if (nComVip)
		{
			//ѡ�Ĳ���ȫ��
			CString strVipLevel = m_ayPrice[nComVip - 1].m_strID;
			if (pVipInfo->m_strLevelID != strVipLevel)
			{
				return;
			}
		}


		//m_listReport.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);  
		//m_listReport.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 200);
		//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("�ܶ�"),	LVCFMT_CENTER, 100);
		//m_listReport.InsertColumn(k++, _T("����ʱ��"),	LVCFMT_CENTER, 260);
		//m_listReport.InsertColumn(k++, _T("������ID"),	LVCFMT_CENTER, 200);
		//m_listReport.InsertColumn(k++, _T("����������"),	LVCFMT_CENTER, 200);
		//m_listReport.InsertColumn(k++, _T("��Ա�ȼ�"),	LVCFMT_CENTER, 100);

		int j = 0;
		int i = m_listReport.GetItemCount();
		m_listReport.InsertItem(i, "��Ʒ");//���

		m_listReport.SetItemText(i, ++j, p->m_strFoodName);//����
		m_listReport.SetItemText(i, ++j, "��");//ʱ��
		m_listReport.SetItemText(i, ++j, p->m_strOnePrice);
		m_listReport.SetItemText(i, ++j, p->m_strCount);
		CString strTotal;
		strTotal.Format("%d", atoi(p->m_strOnePrice)*atoi(p->m_strCount));
		m_listReport.SetItemText(i, ++j, strTotal);//�ܶ�
		CString strPayTime;
		strPayTime.Format("%s-%s", p->m_time.m_strDate.GetBuffer(), p->m_time.m_strTime.GetBuffer());
		m_listReport.SetItemText(i, ++j, strPayTime);
		m_listReport.SetItemText(i, ++j, pVipInfo->m_strID);
		m_listReport.SetItemText(i, ++j, pVipInfo->m_strName);
		m_listReport.SetItemText(i, ++j, pVipInfo->m_strLevelID);
	}
}

BOOL CDlgMgr::CheckPassword()
{
	//����Ա�еȼ��ˣ�����������ˡ�
	return TRUE;
	////���벻��ȷ����ʾ�ں�������ʾ���ⲿ��֪��
	//CString strPass = ReadPassword();
	//if (strPass.IsEmpty())
	//{
	//	//û������
	//	CDlgNewPassword dlg;
	//	if (dlg.DoModal() != IDOK)
	//	{
	//		return FALSE;
	//	}

	//	if (dlg.m_strPassword1.GetLength() > 50 || dlg.m_strPassword1.GetLength() < 6)
	//	{

	//		MessageBox(_T("���볤��ӦΪ6-50���ַ���"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	//		return FALSE;
	//	}

	//	SetPassword(dlg.m_strPassword1);
	//}

	//CDlgPasssword dlg;
	//if (dlg.DoModal() != IDOK)
	//{
	//	return FALSE;
	//}
	//if (dlg.m_strPassword != ReadPassword())
	//{
	//	MessageBox("���벻��ȷ��", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	//	return FALSE;
	//}
	return TRUE;
}

void CDlgMgr::OnBnClickedBtnFieldMgr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ�������

	CDlgFieldMgr dlg;
	dlg.DoModal();
}

void CDlgMgr::OnBnClickedBtnVipMgr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//������ʷ
	CDlgVipHistory dlg;
	dlg.DoModal();
}


void CDlgMgr::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog fileDlg(FALSE,_T("*.csv"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,"csv file (*.csv)|*.csv||",this);
	if (fileDlg.DoModal() != IDOK)
	{
		return;
	}
	CString strFile = fileDlg.GetFileName();
	CString strPath = fileDlg.GetFolderPath();
	CString strSaveFile = strPath + PATHSPLITSTRING + strFile;


	CString strContent;
	CString strTitle;

	//m_listReport.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 80);
	//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 200);
	//m_listReport.InsertColumn(k++, _T("ʱ��"),	LVCFMT_CENTER, 120);
	//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);
	//m_listReport.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 80);
	//m_listReport.InsertColumn(k++, _T("�ܶ�"),	LVCFMT_CENTER, 80);
	//m_listReport.InsertColumn(k++, _T("����ʱ��"),	LVCFMT_CENTER, 200);
	//m_listReport.InsertColumn(k++, _T("������ID"),	LVCFMT_CENTER, 100);
	//m_listReport.InsertColumn(k++, _T("����������"),	LVCFMT_CENTER, 150);
	//m_listReport.InsertColumn(k++, _T("��Ա�ȼ�"),	LVCFMT_CENTER, 100);

	strTitle = "���,����,ʱ��,����,����,�ܶ�,����ʱ��,������ID,����������,��Ա�ȼ�\n";
	strContent += strTitle;
	int nCount = m_listReport.GetItemCount();
	for (int i = 0; i < nCount; ++i)
	{
		CString strOne;
		for (int j = 0; j < 10; ++j)
		{
			strOne +=m_listReport.GetItemText(i,j);
			if (j != 9)
			{
				strOne += ",";
			}
			else
			{
				strOne += "\n";
			}
		}
		strContent += strOne;
	}

	FILE *fp;
	fp=fopen(strSaveFile.GetBuffer(),"w+");
	fputs(strContent.GetBuffer(), fp);
	fclose(fp);
}

CString CDlgMgr::GetPriceByLevelAndTime(CString strLevel, CString strDate, int nTimeIndex)
{
	OnePriceInfo* pPrice = GetPriceInfo(strLevel);
	BOOL bHgih = FALSE;
	if (IsWeekend(strDate))
	{
		//��ĩ
		bHgih = m_periodInfo.m_bWeekend;
	}

	if ((nTimeIndex >= m_periodInfo.m_nGoldStart && nTimeIndex <= m_periodInfo.m_nGoldStop) || bHgih)
	{
		bHgih = TRUE;
	}
	else
	{
		bHgih = FALSE;
	}

	if (bHgih)
	{
		return pPrice->m_strPriceHigh;
	}
	else
	{
		return pPrice->m_strPriceLow;
	}
	
}

void CDlgMgr::OnBnClickedBtnUserMgr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgUserMgr dlg;
	dlg.DoModal();
}

void CDlgMgr::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	int nID = IDR_MENU_VIP_LEVEL;
	m_nCurSelectLevelIndex = pNotifier->iItem;
	if (pNotifier->iItem < 0)
	{
		nID = IDR_MENU_VIP_LEVEL_EMPTY;
		//return;
	}

	CMenu menu ,* pSubMenu; //��������Ҫ�õ���cmenu����

	menu.LoadMenu(nID);

	//��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵�
	pSubMenu = menu.GetSubMenu(0);
	CPoint oPoint; //����һ������ȷ�����λ�õ�λ��
	GetCursorPos( &oPoint); //��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	//��ָ��λ����ʾ�����˵�
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
	//pSubMenu->EnableMenuItem(ID_DEL_VIP_LEVEL,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED); //������˵���ʹ��

	*pResult = 0;
}

void CDlgMgr::OnAddVipLevel()
{
	// TODO: �ڴ���������������
	CDlgAddVipLevel dlg;
	dlg.DoModal();
	ShowPriceList();
}

void CDlgMgr::OnDelVipLevel()
{
	// TODO: �ڴ���������������

	CMainDlg* pMainWnd = GETMAINWND;
	CString strDel = m_ayPrice[m_nCurSelectLevelIndex].m_strID;
	if (strDel == VIP_LEVEL_0)
	{
		MessageBox("�ȼ�0����ɾ��!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	int nVip = pMainWnd->m_pageVip.m_ayVip.GetSize();;

	for (int i = 0; i < nVip; i++)
	{
		if (pMainWnd->m_pageVip.m_ayVip[i].m_strLevelID == strDel)
		{
			MessageBox("�õȼ��л�Ա����ɾ��!", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_DEL_VIP_LEVEL;
	his_op_info.m_strInfo.Format("��ֵ:%s,����:%s"
		, m_ayPrice[m_nCurSelectLevelIndex].m_strID.GetBuffer()
		, m_ayPrice[m_nCurSelectLevelIndex].m_strName.GetBuffer());

	pOperate->Add(his_op_info);

	
	int ret = m_pLevelData->DeletePrice(m_ayPrice[m_nCurSelectLevelIndex].m_strID);
	ShowPriceList();



}

CString CDlgMgr::GetLevelNameByID(CString strID)
{
	int n = m_ayPrice.GetSize();
	for (int i = 0; i < n; i++)
	{
		if (m_ayPrice[i].m_strID == strID)
		{
			return m_ayPrice[i].m_strName;
		}
	}
	return "";
}
void CDlgMgr::OnCbnSelchangeType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nSelectType  = m_ComboType.GetCurSel();
	ShowFieldAndFoodCombo();
	m_listReport.DeleteAllItems();
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
}

void CDlgMgr::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_listReport.DeleteAllItems();
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
}

void CDlgMgr::OnDtnDatetimechangeDatetimepicker2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	m_listReport.DeleteAllItems();
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
}

void CDlgMgr::OnCbnEditchangeNameByType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listReport.DeleteAllItems();
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
}

void CDlgMgr::OnCbnSelchangeVip()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listReport.DeleteAllItems();
	GetDlgItem(IDC_BUTTON5)->EnableWindow(FALSE);
}

void CDlgMgr::OnBnClickedBtnAction()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgOperate dlg;
	dlg.DoModal();
}
