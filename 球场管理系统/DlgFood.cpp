// DlgFood.cpp : 实现文件
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

// CDlgFood 对话框

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


// CDlgFood 消息处理程序

void CDlgFood::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgAddFood dlg;
	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.DoModal();
	// TODO: 在此添加控件通知处理程序代码
	if (dlg.IsSuccess())
	{
		ShowItemFood();
	}
}

void CDlgFood::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION	pos;
	int			iPos;
	pos = m_listFood.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
	his_op_info.m_strInfo.Format("删除商品:%s", strName.GetBuffer());

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
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
	//内存只保留当天数据
	m_listFood.DeleteAllItems();
	m_ayFood.RemoveAll();

	CMainDlg* pMainWnd = GETMAINWND;

	//读出所有数据
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
	m_listFood.InsertColumn(k++, _T("名称"),	LVCFMT_CENTER, 200);
	m_listFood.InsertColumn(k++, _T("价格"),	LVCFMT_CENTER, 120);
	m_listFood.InsertColumn(k++, _T("库存"),	LVCFMT_CENTER, 120);

	k=0;
	m_listBuy.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 120);
	m_listBuy.InsertColumn(k++, _T("名称"),	LVCFMT_CENTER, 120);
	m_listBuy.InsertColumn(k++, _T("数量"),	LVCFMT_CENTER, 120);
	m_nCount = 1;

	ShowItemFood();

	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	if (m_listFood.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}

	CRect rect;    
	GetClientRect(&rect);     //取客户区大小  
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
	//修改Food信息，并且更新xml文件
	FoodInfo * pInfo = GetFoodInfo(info.m_strID);
	if (!pInfo)
	{
		return -1;
	}
	*pInfo = info;//改内存中的值

	m_pFoodData->ChangeFoodInfo(info);
	return 0;
}
void CDlgFood::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i = m_listFood.GetSelectionMark();  //获取信息位置

	if (i == -1)  //如果未选则提示
	{
		MessageBox("请选择你要的商品！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}

	CString PointMenuID   = m_listFood.GetItemText(i, 0);
	CString PointMenuName  = m_listFood.GetItemText(i, 1);
	CString PointMenuPrice  = m_listFood.GetItemText(i, 2);
	CString PointMenuRemain  = m_listFood.GetItemText(i, 3);
	int nRemain = atoi(PointMenuRemain.GetBuffer());

	CString PointMenuIDSelect;
	int n = m_listBuy.GetItemCount();   //获取行数目
	int nInsert = n;
	for (i = 0; i < n; i++)
	{
		PointMenuIDSelect = m_listBuy.GetItemText(i, 0);  //附值给对比临时变量
		if (PointMenuIDSelect == PointMenuID)  //判断是否已有
		{
			nInsert = i;
			break;
		}
	}

	if(m_nCount<=0)
	{
		MessageBox("数量要大于0！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if(nRemain - m_nCount < 0)
	{
		MessageBox("库存不够！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	nRemain -= m_nCount;

	//FoodInfo* p = GetFoodInfo(PointMenuID);
	//p->m_strRemain.Format("%d", nRemain);
	PointMenuRemain.Format("%d", nRemain);
	m_listFood.SetItemText(m_listFood.GetSelectionMark(), 3, PointMenuRemain);
	//ChangeFoodInfo(*p);//结算才真正修改配置文件

	if (nInsert < n)
	{
		//买的是买过的
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
	// TODO: 在此添加控件通知处理程序代码
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


		//查询价格
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

				ayHisFood.Add(hisfood);//添加一条历史记录

				break;
			}
		}
	}

	CDlgCheckOut dlg;
	dlg.m_strTotalMoney.Format("%d", nTotalMoney);
	if (dlg.DoModal() == IDOK)
	{
		//结算成功，修改库存
		int nBuy = m_listBuy.GetItemCount();   //获取行数目
		int nFoodCount = m_listFood.GetItemCount();
		for (int i = 0; i < nBuy; ++i)
		{
			CString strID = m_listBuy.GetItemText(i, 0);
			for (int j = 0; j < nFoodCount; j++)
			{
				//找到商品列表中的库存
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

		//写入消费记录
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
			ASSERT(FALSE && "数据来源不支持！");
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
	// TODO: 在此添加控件通知处理程序代码
	m_listBuy.DeleteAllItems();
	m_nCount = 1;
	ShowItemFood();
	UpdateData(FALSE);
}

void CDlgFood::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int iBuy = m_listBuy.GetSelectionMark();  //获取信息位置

	if (iBuy == -1)  //如果未选则提示
	{
		MessageBox("请先选择您要取消的菜！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}
	if(m_nCount<=0)
	{
		MessageBox("数量要大于0！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	CString MenuID   = m_listBuy.GetItemText(iBuy, 0);//取消的ID

	CString FoodMenuIDSelect;
	int n = m_listFood.GetItemCount();   //获取行数目
	int nIndex = n;
	for (int i = 0; i < n; i++)
	{
		FoodMenuIDSelect = m_listFood.GetItemText(i, 0);  //附值给对比临时变量
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
	POINT Newp; //获取现在对话框的大小
	CRect recta;    
	GetClientRect(&recta);     //取客户区大小  

	CSize sz = recta.Size();
	if (sz.cx == 0 && sz.cy == 0)
	{
		//最小化
		return;
	}


	Newp.x=recta.right-recta.left;
	Newp.y=recta.bottom-recta.top;
	fsp[0]=(float)Newp.x/m_ptOld.x;
	fsp[1]=(float)Newp.y/m_ptOld.y;

	CRect Rect;
	int woc;
	CPoint OldTLPoint,TLPoint; //左上角
	CPoint OldBRPoint,BRPoint; //右下角
	HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件  
	while(hwndChild)    
	{    
		woc=::GetDlgCtrlID(hwndChild);//取得ID
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