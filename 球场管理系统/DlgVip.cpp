// DlgVip.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgVip.h"
#include "DlgAddVip.h"
#include "DlgModifyVip.h"
#include "DlgCharge.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"
#include "DlgVipHistory.h"
#include <iostream>
using namespace std;
#include "app.h"
#include "public.h"
#include "DlgPasssword.h"
#include "operatedata.h"

// CDlgVip �Ի���

IMPLEMENT_DYNAMIC(CDlgVip, CDialog)

CDlgVip::CDlgVip(CWnd* pParent /*=NULL*/)
: CDialog(CDlgVip::IDD, pParent)
, m_strFilter(_T(""))
{

}

CDlgVip::~CDlgVip()
{
}

void CDlgVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIP, m_listVip);
	DDX_Text(pDX, IDC_EDIT2, m_strFilter);
}


BEGIN_MESSAGE_MAP(CDlgVip, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgVip::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgVip::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CDlgVip::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_CHARGE, &CDlgVip::OnBnClickedBtnCharge)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VIP, &CDlgVip::OnNMDblclkListVip)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_VIP, &CDlgVip::OnNMRClickListVip)
	ON_COMMAND(ID_32779, &CDlgVip::On32779)
	ON_COMMAND(ID_32781, &CDlgVip::On32781)
	ON_COMMAND(ID_32780, &CDlgVip::On32780)
	ON_COMMAND(ID_32783, &CDlgVip::On32783)
	ON_COMMAND(ID_32782, &CDlgVip::On32782)
	ON_WM_SIZE()
	ON_COMMAND(ID_32785, &CDlgVip::On32785)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgVip::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CDlgVip ��Ϣ�������

BOOL CDlgVip::OnInitDialog()
{
	CRect rect;    
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	m_ptOld.x=rect.right-rect.left;
	m_ptOld.y=rect.bottom-rect.top;

	m_pVipData = GetVipData;

	m_nMaxID = 0;

	CDialog::OnInitDialog();

	m_listVip.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listVip.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("�Ա�"),	LVCFMT_CENTER, 60);
	m_listVip.InsertColumn(k++, _T("�绰"),	LVCFMT_CENTER, 150);
	if (ISADMIN)
	{
		m_listVip.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 120);
	}
	m_listVip.InsertColumn(k++, _T("�ȼ�"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 120);

	ShowItemVip();
	if (ISADMIN)
	{
		GetDlgItem(IDC_BTN_DEL)->ShowWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->ShowWindow(FALSE);
	}

	return TRUE;
}

void CDlgVip::OnBnClickedBtnAdd()
{
	CDlgAddVip dlg;

	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.DoModal();

	if (dlg.IsSuccess())
	{
		ShowItemVip();
	}
	else
	{
		--m_nMaxID;
	}
}

void CDlgVip::ShowItemVip()
{
	UpdateData(TRUE);

	m_listVip.DeleteAllItems();
	m_ayVip.RemoveAll();
	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK != pMainWnd->m_pageVip.m_pVipData->GetAllData(m_ayVip, m_ayDelVip))
	{
		return;
	}

	m_nMaxID = 0;
	//int nVipCount = m_ayVip.GetSize();
	CArray<VipInfo> tmp_ay;
	tmp_ay.Copy(m_ayVip);
	for (int i = 0; i < m_ayVip.GetSize(); i++)
	{
		VipInfo& info = m_ayVip[i];
		int j = 0;
		int nID = atoi(info.m_strID.GetBuffer());
		if (nID > m_nMaxID)
		{
			m_nMaxID = nID;
		}
		//��ʱ�ǻ�Ա����ʾ
		if (info.m_strLevelID == VIP_LEVEL_0)
		{
			m_ayVip.RemoveAt(i);
			i--;
			continue;
		}

		if (!m_strFilter.IsEmpty())
		{
			if (info.m_strID.Find(m_strFilter) == -1 && info.m_strName.Find(m_strFilter) == -1)
			{
				m_ayVip.RemoveAt(i);
				i--;
				continue;
			}
		}
		m_listVip.InsertItem(i, info.m_strID);
		m_listVip.SetItemText(i, ++j, info.m_strName);
		m_listVip.SetItemText(i, ++j, info.m_strSex);
		m_listVip.SetItemText(i, ++j, info.m_strPhone);
		if (ISADMIN)
		{
			m_listVip.SetItemText(i, ++j, info.m_strMoney);
		}
		
		
		CString strLevelName = pMainWnd->m_pageMgr.GetLevelNameByID(info.m_strLevelID);
		CString strTmp;
		strTmp.Format("%s-%s", info.m_strLevelID.GetBuffer(), strLevelName.GetBuffer());
		m_listVip.SetItemText(i, ++j, strTmp);
		if (info.m_bLock)
		{
			m_listVip.SetItemText(i, ++j, "��");
		}
		else
		{
			m_listVip.SetItemText(i, ++j, "��");
		}
	}

	if (m_listVip.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		m_listVip.SetSelectionMark(0);
	}

	UpdateData(FALSE);

	m_ayVip.Copy(tmp_ay);
}

BOOL CDlgVip::CheckPwd()
{
	CDlgPasssword dlg;
	if (dlg.DoModal() != IDOK)
	{
		return FALSE;
	}

	USER_LIST user_list;
	CUserData* pUser = GetUserData;
	pUser->GetAllData(user_list);
	int nCount = user_list.GetSize();
	int i = 0;
	UserInfo info;
	for (; i < nCount; i++)
	{
		info = user_list[i];
		if (info.m_strName == GETUSERNAME)
		{
			if (info.m_strPwd != dlg.m_strPassword)
			{
				MessageBox("�������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
				return FALSE;
			}
			else
			{
				break;
			}
		}
	}
	return TRUE;
}

void CDlgVip::OnBnClickedBtnDel()
{
	if (!CheckPwd())
	{
		return ;
	}
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫɾ���ļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	CString strID		= m_listVip.GetItemText(iPos, 0);
	CString strName = m_listVip.GetItemText(iPos, 1);

	if(MessageBox("�û�Ա��Ԥ����Ϣ������գ�ȷ����","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
	{
		return;
	}


	CMainDlg* pMainWnd = GETMAINWND;

	
	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->DeleteVip(strID))
	{
		ShowItemVip();
		pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_DEL_VIP;
		his_op_info.m_strInfo.Format("ID:%s, ����:%s"
			, strID.GetBuffer()
			, strName.GetBuffer());

		pOperate->Add(his_op_info);

	}
}

void CDlgVip::OnBnClickedBtnModify()
{
	// TODO: Add your control notification handler code here
	CDlgModifyVip dlg;
	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫ�޸ĵļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	dlg.m_strID		= m_listVip.GetItemText(iPos, 0);;

	dlg.DoModal();

	ShowItemVip();
	UpdateData(FALSE);
}

void CDlgVip::OnBnClickedBtnCharge()
{
	CDlgCharge dlg;

	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos)
	{
		iPos		    	= m_listVip.GetNextSelectedItem(pos);
		dlg.m_strID		= m_listVip.GetItemText(iPos, 0);
	}

	dlg.DoModal();
	ShowItemVip();
	UpdateData(FALSE);
}


VipInfo* CDlgVip::GetDelVipInfo(CString strVipID)
{
	//�ҵ�vip��鿴���ĸ�����Ϣ
	int nVipCount = m_ayDelVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (m_ayDelVip[j].m_strID == strVipID)
		{
			break;
		}
	}

	if (j < nVipCount)
	{
		return &m_ayDelVip[j];
	}
	return NULL;
}
VipInfo* CDlgVip::GetVipInfo(CString strVipID)

{
	//�ҵ�vip��鿴���ĸ�����Ϣ
	int nVipCount = m_ayVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (m_ayVip[j].m_strID == strVipID)
		{
			break;
		}
	}

	if (j < nVipCount)
	{
		return &m_ayVip[j];
	}

	return NULL; 

}

int CDlgVip::ChangeVipInfo(VipInfo& info)
{
	//�޸�vip��Ϣ�����Ҹ���xml�ļ�
	int nVipCount = m_ayVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (m_ayVip[j].m_strID == info.m_strID)
		{
			//�ҵ�id
			break;
		}
	}

	if (j >= nVipCount)
	{
		return ERROR_UNKNOW_ERROR;
	}

	m_ayVip[j] = info;//���ڴ��е�ֵ

	//���ļ�
	CMainDlg* pMainWnd = GETMAINWND;
	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->ChangeVipInfo(info))
	{
		ShowItemVip();
	}
	
	return 0;
}

void CDlgVip::OnNMDblclkListVip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	if (pNotifier->iItem < 0)
	{
		return;
	}

	CDlgModifyVip dlg;
	dlg.m_strID		= m_listVip.GetItemText(pNotifier->iItem, 0);

	dlg.DoModal();

	ShowItemVip();
	UpdateData(FALSE);

}

void CDlgVip::OnNMRClickListVip(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;
	if (pNotifier->iItem == -1)
	{
		return;
	}

	m_strCurSelectVip = m_listVip.GetItemText(pNotifier->iItem,0);

	const VipInfo* p = GetVipInfo(m_strCurSelectVip);

	CMenu menu ,* pSubMenu; //��������Ҫ�õ���cmenu����

	if (p->m_bLock)
	{
		menu.LoadMenu(IDR_MENU_VIP_LOCKED);
	}
	else
	{
		menu.LoadMenu(IDR_MENU_VIP_UNLOCKED);
	}

	//��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵�
	pSubMenu = menu.GetSubMenu(0);
	if (!ISADMIN)
	{
		pSubMenu->DeleteMenu(ID_32781,MF_BYCOMMAND);  
	}
	CPoint oPoint; //����һ������ȷ�����λ�õ�λ��
	GetCursorPos( &oPoint); //��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	//��ָ��λ����ʾ�����˵�
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CDlgVip::On32779()
{
	// TODO: �ڴ���������������
	//��ֵ
	CDlgCharge dlg;	
	dlg.m_strID	= m_strCurSelectVip;
	dlg.DoModal();
	ShowItemVip();
	UpdateData(FALSE);
}

void CDlgVip::On32781()
{
	// TODO: �ڴ���������������
	//ɾ��
	if (!CheckPwd())
	{
		return ;
	}

	if(MessageBox("�û�Ա��Ԥ����Ϣ������գ�ȷ����","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
	{
		return;
	}

	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->DeleteVip(m_strCurSelectVip))
	{
		ShowItemVip();
		pMainWnd->m_pageField.ShowItemField(pMainWnd->m_pageField.GetDate());
	}
}

void CDlgVip::On32780()
{
	// TODO: �ڴ���������������
	//������Ա
	const VipInfo* p = GetVipInfo(m_strCurSelectVip);
	VipInfo info = *p;
	info.m_bLock = TRUE;
	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->ChangeVipInfo(info))
	{
		ShowItemVip();

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_LOCK_VIP;
		his_op_info.m_strInfo.Format("ID:%s, ����:%s"
			, info.m_strID.GetBuffer()
			, info.m_strName.GetBuffer());

		pOperate->Add(his_op_info);

	}
}

void CDlgVip::On32783()
{
	// TODO: �ڴ���������������
	const VipInfo* p = GetVipInfo(m_strCurSelectVip);

	VipInfo info = *p;
	info.m_bLock = FALSE;
	CMainDlg* pMainWnd = GETMAINWND;

	if (ERROR_OK == pMainWnd->m_pageVip.m_pVipData->ChangeVipInfo(info))
	{
		ShowItemVip();

		CHisOperateData* pOperate = GetOperateData;
		HisOperateInfo his_op_info;
		his_op_info.m_strName = GETUSERNAME;
		his_op_info.m_strType = OPERATE_TYPE_UNLOCK_VIP;
		his_op_info.m_strInfo.Format("ID:%s, ����:%s"
			, info.m_strID.GetBuffer()
			, info.m_strName.GetBuffer());

		pOperate->Add(his_op_info);
	}
}

void CDlgVip::On32782()
{
	// TODO: �ڴ���������������
	//������ʷ
	CDlgVipHistory dlg;
	dlg.m_strVipID = m_strCurSelectVip;
	dlg.DoModal();
}

int CDlgVip::NewID()
{
	return ++m_nMaxID;
}

void CDlgVip::resize()
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
		GetDlgItem(woc)->MoveWindow(Rect,TRUE);
		hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);    
	}
	m_ptOld=Newp;

}

void CDlgVip::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		resize();
	}
}
void CDlgVip::On32785()
{
	// TODO: �ڴ���������������
}

void CDlgVip::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	ShowItemVip();
}
