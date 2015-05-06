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
#include "history.h"
#include "operatedata.h"
#include "app.h"

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
	ON_WM_SIZE()
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

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_DEL_FOOD;
	CString strName = m_listFood.GetItemText(iPos, 1);
	his_op_info.m_strInfo.Format("ɾ����Ʒ:%s", strName.GetBuffer());

	pOperate->Add(his_op_info);

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
	dlg.m_strRemain = m_listFood.GetItemText(iPos, 3);

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
		m_listFood.SetItemText(i, ++j, m_ayFood[i].m_strRemain);
	}

	if (m_listFood.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(TRUE);
	}
	UpdateData(FALSE);
}

BOOL CDlgFood::OnInitDialog()
{
	m_pFoodData = GetFoodData;

	m_nMaxID = 0;
	CDialog::OnInitDialog();
	m_listFood.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	m_listBuy.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listFood.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 100);
	m_listFood.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 200);
	m_listFood.InsertColumn(k++, _T("�۸�"),	LVCFMT_CENTER, 120);
	m_listFood.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 120);

	k=0;
	m_listBuy.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 120);
	m_listBuy.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 120);
	m_listBuy.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 120);
	m_nCount = 1;

	ShowItemFood();

	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	if (m_listFood.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}

	CRect rect;    
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	m_ptOld.x=rect.right-rect.left;
	m_ptOld.y=rect.bottom-rect.top;

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

FoodInfo* CDlgFood::GetFoodInfoByName(CString strName)
{
	int nCount = m_ayFood.GetCount();

	int j = 0;
	for (; j < nCount; ++j)
	{
		if (m_ayFood[j].m_strName == strName)
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
		MessageBox("��ѡ����Ҫ����Ʒ��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}

	CString PointMenuID   = m_listFood.GetItemText(i, 0);
	CString PointMenuName  = m_listFood.GetItemText(i, 1);
	CString PointMenuPrice  = m_listFood.GetItemText(i, 2);
	CString PointMenuRemain  = m_listFood.GetItemText(i, 3);
	int nRemain = atoi(PointMenuRemain.GetBuffer());

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
	if(nRemain - m_nCount < 0)
	{
		MessageBox("��治����","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	nRemain -= m_nCount;

	//FoodInfo* p = GetFoodInfo(PointMenuID);
	//p->m_strRemain.Format("%d", nRemain);
	PointMenuRemain.Format("%d", nRemain);
	m_listFood.SetItemText(m_listFood.GetSelectionMark(), 3, PointMenuRemain);
	//ChangeFoodInfo(*p);//����������޸������ļ�

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

	CArray<HisBuyFoodInfo> ayHisFood;

	CTime nowTime(time(NULL));

	int nTotalMoney = 0;
	for (int i = 0; i < nCount; ++i)
	{
		HisBuyFoodInfo hisfood;
		hisfood.m_time.m_strDate.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
		hisfood.m_time.m_strTime.Format("%02d:%02d:%02d", nowTime.GetHour(), nowTime.GetMinute(), nowTime.GetSecond());
		
		hisfood.m_strFoodName =  m_listBuy.GetItemText(i, 1);
		hisfood.m_strFoodID = GetFoodInfoByName(hisfood.m_strFoodName)->m_strID;
		hisfood.m_strCount = m_listBuy.GetItemText(i, 2);
		CString strID = m_listBuy.GetItemText(i, 0);
		int nBuyCount = atoi(m_listBuy.GetItemText(i, 2));


		//��ѯ�۸�
		int nFoodCount = m_listFood.GetItemCount();
		for (int j = 0; j < nFoodCount; ++j)
		{
			CString strtmp = m_listFood.GetItemText(j, 0);
			if (strID == strtmp)
			{
				CString strPrice = m_listFood.GetItemText(j, 2);
				int nPrice = atoi(strPrice);
				nTotalMoney += nPrice * nBuyCount;
				hisfood.m_strOnePrice = strPrice;

				ayHisFood.Add(hisfood);//���һ����ʷ��¼

				break;
			}
		}
	}

	CDlgCheckOut dlg;
	dlg.m_strTotalMoney.Format("%d", nTotalMoney);
	if (dlg.DoModal() == IDOK)
	{
		//����ɹ����޸Ŀ��
		int nBuy = m_listBuy.GetItemCount();   //��ȡ����Ŀ
		int nFoodCount = m_listFood.GetItemCount();
		for (int i = 0; i < nBuy; ++i)
		{
			CString strID = m_listBuy.GetItemText(i, 0);
			for (int j = 0; j < nFoodCount; j++)
			{
				//�ҵ���Ʒ�б��еĿ��
				CString strtmp = m_listFood.GetItemText(j, 0);
				if (strID == strtmp)
				{
					CString strRemain = m_listFood.GetItemText(j, 3);
					FoodInfo* p = GetFoodInfo(strID);
					p->m_strRemain = strRemain;
					ChangeFoodInfo(*p);
				}
			}
			
		}
		
		m_listBuy.DeleteAllItems();

		//д�����Ѽ�¼
		CHisData* pVipHisData = NULL;
		if (DATA_FROM_TYPE == DATA_FROM_XML)
		{
			pVipHisData = new CHisDataXML(TL_GetModulePath(NULL) + 
				"viphistory" + PATHSPLITSTRING + dlg.m_strVip + ".xml");
		}
		else if (DATA_FROM_TYPE == DATA_FROM_MYSQL)
		{
			pVipHisData = GetHistoryData;
		}
		else
		{
			pVipHisData = NULL;
			ASSERT(FALSE && "������Դ��֧�֣�");
			return;
		}

		for (int i = 0; i < ayHisFood.GetSize(); i++)
		{
			ayHisFood[i].m_strVipID = dlg.m_strVip;
			pVipHisData->AddBuyFood(ayHisFood[i]);
		}

		if (DATA_FROM_TYPE == DATA_FROM_XML)
		{
			delete pVipHisData;
		}
		
	}
}

void CDlgFood::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listBuy.DeleteAllItems();
	m_nCount = 1;
	ShowItemFood();
	UpdateData(FALSE);
}

void CDlgFood::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int iBuy = m_listBuy.GetSelectionMark();  //��ȡ��Ϣλ��

	if (iBuy == -1)  //���δѡ����ʾ
	{
		MessageBox("����ѡ����Ҫȡ���Ĳˣ�","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}
	if(m_nCount<=0)
	{
		MessageBox("����Ҫ����0��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	CString MenuID   = m_listBuy.GetItemText(iBuy, 0);//ȡ����ID

	CString FoodMenuIDSelect;
	int n = m_listFood.GetItemCount();   //��ȡ����Ŀ
	int nIndex = n;
	for (int i = 0; i < n; i++)
	{
		FoodMenuIDSelect = m_listFood.GetItemText(i, 0);  //��ֵ���Ա���ʱ����
		if (FoodMenuIDSelect == MenuID)
		{
			nIndex = i;
			break;
		}
	}

	CString strFoodRemain = m_listFood.GetItemText(nIndex, 3);
	CString strFoodNewRemain;
	strFoodNewRemain.Format("%d", atoi(strFoodRemain) + m_nCount);
	m_listFood.SetItemText(nIndex, 3, strFoodNewRemain);
	
	CString strCount = m_listBuy.GetItemText(iBuy, 2);
	CString strNewCount;
	strNewCount.Format("%d", atoi(strCount.GetBuffer()) - m_nCount);
	if (atoi(strNewCount.GetBuffer()) <= 0)
	{
		m_listBuy.DeleteItem(iBuy);
		m_listBuy.SetSelectionMark(-1);
		UpdateData(FALSE);

		if (m_listBuy.GetItemCount() == 0)
		{
			GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
		}
		return;
	}
	m_listBuy.SetItemText(iBuy, 2, strNewCount);
	UpdateData(FALSE);
}

void CDlgFood::resize()
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

void CDlgFood::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED)
	{
		resize();
	}
}