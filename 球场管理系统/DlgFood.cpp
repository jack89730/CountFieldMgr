// DlgFood.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgFood.h"
#include "DlgAddFood.h"
#include "DlgModifyFood.h"
#include "DlgBuyFood.h"
#include "tinyxml.h"
#include "public.h"
#include <iostream>
#include "MainDlg.h"
#include "DlgCheckOut.h"
#include "fooddata.h"

// CDlgFood �Ի���

IMPLEMENT_DYNAMIC(CDlgFood, CDialog)

CDlgFood::CDlgFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFood::IDD, pParent)
	, m_nCount(0)
{

}

CDlgFood::~CDlgFood()
{
}

void CDlgFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listFood);
	DDX_Text(pDX, IDC_EDIT1, m_nCount);
	DDV_MinMaxInt(pDX, m_nCount, 0, 100);
	DDX_Control(pDX, IDC_LIST2, m_listBuy);
}


BEGIN_MESSAGE_MAP(CDlgFood, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgFood::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgFood::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CDlgFood::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFood::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgFood::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgFood::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgFood::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDlgFood ��Ϣ�������

void CDlgFood::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgAddFood dlg;
	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.DoModal();
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (dlg.IsSuccess())
	{
		ShowItemFood();
	}
}

void CDlgFood::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION	pos;
	int			iPos;
	pos = m_listFood.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫ�޸ĵļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listFood.GetNextSelectedItem(pos);
	CString strID		= m_listFood.GetItemText(iPos, 0);

	CMainDlg* pMainWnd = GETMAINWND;
	int ret = pMainWnd->m_pageFood.m_pFoodData->DeleteFood(strID);
	ShowItemFood();
}

void CDlgFood::OnBnClickedBtnModify()
{
	// TODO: Add your control notification handler code here
	CDlgModifyFood dlg;
	POSITION	pos;
	int			iPos;
	pos = m_listFood.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫ�޸ĵļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listFood.GetNextSelectedItem(pos);
	dlg.m_strID		= m_listFood.GetItemText(iPos, 0);
	dlg.m_strName		= m_listFood.GetItemText(iPos, 1);
	dlg.m_strPrice = m_listFood.GetItemText(iPos, 2);

	dlg.DoModal();

	ShowItemFood();
	UpdateData(FALSE);
}

void CDlgFood::ShowItemFood()
{
	//�ڴ�ֻ������������
	m_listFood.DeleteAllItems();
	m_ayFood.RemoveAll();

	CMainDlg* pMainWnd = GETMAINWND;

	//������������
	m_pFoodData->GetAllData(m_ayFood);

	m_nMaxID = 0;

	int nFoodCount = m_ayFood.GetCount();
	for (int i = 0; i < nFoodCount; i++)
	{
		int nID = atoi(m_ayFood[i].m_strID);
		if (nID > m_nMaxID)
		{
			m_nMaxID = nID;
		}
		int j = 0;
		m_listFood.InsertItem(i, m_ayFood[i].m_strID);
		m_listFood.SetItemText(i, ++j, m_ayFood[i].m_strName);
		m_listFood.SetItemText(i, ++j, m_ayFood[i].m_strPrice);
	}

	UpdateData(FALSE);
}

BOOL CDlgFood::OnInitDialog()
{
	if (DATA_FROM_TYPE == DATA_FROM_XML)
	{
		m_pFoodData = new CFoodDataXML(TL_GetModulePath(NULL) + "food.xml");
	}
	else
	{
		m_pFoodData = NULL;
		ASSERT(FALSE && "������Դ��֧�֣�");
		return FALSE;
	}

	m_nMaxID = 0;
	CDialog::OnInitDialog();
	m_listFood.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	m_listBuy.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listFood.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 30);
	m_listFood.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
	m_listFood.InsertColumn(k++, _T("�۸�"),	LVCFMT_CENTER, 60);

	k=0;
	m_listBuy.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 30);
	m_listBuy.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
	m_listBuy.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 60);
	m_nCount = 1;

	ShowItemFood();

	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	if (m_listFood.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
	return TRUE;
}

FoodInfo* CDlgFood::GetFoodInfo(CString strID)
{
	int nCount = m_ayFood.GetCount();

	int j = 0;
	for (; j < nCount; ++j)
	{
		if (m_ayFood[j].m_strID == strID)
		{
			break;
		}
	}

	if (j < nCount)
	{
		return &(m_ayFood[j]);
	}

	return NULL; 

}

int CDlgFood::ChangeFoodInfo(FoodInfo& info)
{
	//�޸�Food��Ϣ�����Ҹ���xml�ļ�
	FoodInfo * pInfo = GetFoodInfo(info.m_strID);
	if (!pInfo)
	{
		return -1;
	}
	*pInfo = info;//���ڴ��е�ֵ

	m_pFoodData->ChangeFoodInfo(info);
	return 0;
}
void CDlgFood::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int i = m_listFood.GetSelectionMark();  //��ȡ��Ϣλ��

	if (i == -1)  //���δѡ����ʾ
	{
		MessageBox("����ѡ����Ҫ��Ĳˣ�","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}

	CString PointMenuID   = m_listFood.GetItemText(i, 0);
	CString PointMenuName  = m_listFood.GetItemText(i, 1);
	CString PointMenuPrice  = m_listFood.GetItemText(i, 2);
	CString PointMenuIDSelect;
	int n = m_listBuy.GetItemCount();   //��ȡ����Ŀ
	int nInsert = n;
	for (i = 0; i < n; i++)
	{
		PointMenuIDSelect = m_listBuy.GetItemText(i, 0);  //��ֵ���Ա���ʱ����
		if (PointMenuIDSelect == PointMenuID)  //�ж��Ƿ�����
		{
			nInsert = i;
			break;
		}
	}

	if(m_nCount<=0)
	{
		MessageBox("����Ҫ����0��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if (nInsert < n)
	{
		//����������
		CString strCount = m_listBuy.GetItemText(nInsert, 2);
		CString strNewCount;
		strNewCount.Format("%d", atoi(strCount.GetBuffer())+m_nCount);
		m_listBuy.SetItemText(nInsert, 2, strNewCount);
	}
	else
	{
		int j = 0;
		m_listBuy.InsertItem(nInsert, PointMenuID);
		m_listBuy.SetItemText(nInsert, ++j, PointMenuName);
		
		CString strNewCount;
		strNewCount.Format("%d", m_nCount);
		m_listBuy.SetItemText(nInsert, ++j, strNewCount);
	}

	GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

void CDlgFood::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int nCount = m_listBuy.GetItemCount();
	if (nCount == 0)
	{
		return;
	}

	int nTotalMoney = 0;
	for (int i = 0; i < nCount; ++i)
	{
		CString strID = m_listBuy.GetItemText(i, 0);
		int nBuyCount = atoi(m_listBuy.GetItemText(i, 2));

		//��ѯ�۸�
		int nFoodCount = m_listFood.GetItemCount();
		for (int j = 0; j < nFoodCount; ++j)
		{
			CString strtmp = m_listFood.GetItemText(j, 0);
			if (strID == strtmp)
			{
				int nPrice = atoi(m_listFood.GetItemText(j, 2));
				nTotalMoney += nPrice * nBuyCount;
				break;
			}
		}
	}

	CDlgCheckOut dlg;
	dlg.m_strTotalMoney.Format("%d", nTotalMoney);
	if (dlg.DoModal() == IDOK)
	{
		m_listBuy.DeleteAllItems();
	}
}

void CDlgFood::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listBuy.DeleteAllItems();
	m_nCount = 1;
	UpdateData(FALSE);
}

void CDlgFood::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int i = m_listBuy.GetSelectionMark();  //��ȡ��Ϣλ��

	if (i == -1)  //���δѡ����ʾ
	{
		MessageBox("����ѡ����Ҫȡ���Ĳˣ�","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}
	if(m_nCount<=0)
	{
		MessageBox("����Ҫ����0��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	//����������
	CString strCount = m_listBuy.GetItemText(i, 2);
	CString strNewCount;
	strNewCount.Format("%d", atoi(strCount.GetBuffer()) - m_nCount);
	if (atoi(strNewCount.GetBuffer()) <= 0)
	{
		m_listBuy.DeleteItem(i);
		UpdateData(FALSE);

		if (m_listBuy.GetItemCount() == 0)
		{
			GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		}
		return;
	}
	m_listBuy.SetItemText(i, 2, strNewCount);
	UpdateData(FALSE);
}
