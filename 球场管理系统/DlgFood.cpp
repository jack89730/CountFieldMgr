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
	ON_BN_CLICKED(IDC_BTN_BUY, &CDlgFood::OnBnClickedBtnBuy)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFood::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgFood::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlgFood::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgFood::OnBnClickedButton4)
END_MESSAGE_MAP()


// CDlgFood 消息处理程序

void CDlgFood::OnBnClickedBtnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgAddFood dlg;
	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.m_strXML = m_strXML;
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

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个food节点。
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;
	while(food)
	{
		CString strIDFind = food->Attribute("id");
		if (strIDFind == strID)
		{
			//找到了
			break;
		}

		food = food->NextSiblingElement("food");

	} 
	if (food)
	{
		//找到了
		TiXmlNode *pParNode =  food->Parent();
		if (NULL==pParNode)
		{
			return;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL!=pParentEle)
		{
			if(pParentEle->RemoveChild(food))
			{
				myDocument.SaveFile();
				m_listFood.DeleteAllItems();
				ShowItemFood();
			}
		}
	}
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
	dlg.m_strXML = m_strXML;

	dlg.DoModal();

	ShowItemFood();
	UpdateData(FALSE);
}

void CDlgFood::ShowItemFood()
{
	do 
	{
		TiXmlDocument *myDocument = new TiXmlDocument(m_strXML.GetBuffer());
		if (!myDocument->LoadFile())
		{
			break;
		}

		m_listFood.DeleteAllItems();
		m_ayFood.RemoveAll();

		//获得根元素
		TiXmlElement *RootElement = myDocument->RootElement();
		//获得第一个vip节点。
		TiXmlElement *food = RootElement->FirstChildElement();

		int i=0;
		while(food)
		{
			CString strID = food->Attribute("id");
			int nID = atoi(strID);

			if (nID > m_nMaxID)
			{
				m_nMaxID = nID;
			}

			TiXmlElement *NameElement = food->FirstChildElement("name");
			TiXmlElement *PriceElement = NameElement->NextSiblingElement("price");

			CString strName = NameElement->FirstChild()->Value();
			CString strPrice = PriceElement->FirstChild()->Value();

			FoodInfo info;
			int j = 0;
			m_listFood.InsertItem(i, strID);
			m_listFood.SetItemText(i, ++j, strName);
			m_listFood.SetItemText(i, ++j, strPrice);

			info.m_strID = strID;
			info.m_strPrice = strPrice;

			m_ayFood.Add(info);
			i++;
			food = food->NextSiblingElement("food");

		} 
		if (!food)
		{
			delete myDocument;
			break;
		}

	} while (1);

	if (m_listFood.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MODIFY)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MODIFY)->EnableWindow(TRUE);
		m_listFood.SetSelectionMark(0);
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
	m_nMaxID = 0;
	m_strXML = TL_GetModulePath(NULL) + "food.xml";
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_listFood.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	m_listBuy.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listFood.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 30);
	m_listFood.InsertColumn(k++, _T("名称"),	LVCFMT_CENTER, 100);
	m_listFood.InsertColumn(k++, _T("价格"),	LVCFMT_CENTER, 60);

	k=0;
	m_listBuy.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 30);
	m_listBuy.InsertColumn(k++, _T("名称"),	LVCFMT_CENTER, 100);
	m_listBuy.InsertColumn(k++, _T("数量"),	LVCFMT_CENTER, 60);
	m_nCount = 1;

	ShowItemFood();

	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	if (m_listFood.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	}
	return TRUE;
}

void CDlgFood::OnBnClickedBtnBuy()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgBuyFood dlg;
	dlg.DoModal();

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
	//修改Food信息，并且更新xml文件
	FoodInfo * pInfo = GetFoodInfo(info.m_strID);
	if (!pInfo)
	{
		return -1;
	}
	*pInfo = info;//改内存中的值

	//改文件

	//创建一个XML的文档对象。
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return -1;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;
	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == info.m_strID)
		{
			break;
		}
		food = food->NextSiblingElement("vip");
	} 

	if (food)
	{
		//找到了，更新数据
		TiXmlElement *NameElement = food->FirstChildElement("name");
		TiXmlElement *PriceElement = NameElement->NextSiblingElement("price");

		NameElement->FirstChild()->SetValue(info.m_strName);
		PriceElement->FirstChild()->SetValue(info.m_strPrice);

		myDocument.SaveFile();
	}
	return 0;
}
void CDlgFood::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i = m_listFood.GetSelectionMark();  //获取信息位置

	if (i == -1)  //如果未选则提示
	{
		MessageBox("请先选择您要点的菜！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}

	CString PointMenuID   = m_listFood.GetItemText(i, 0);
	CString PointMenuName  = m_listFood.GetItemText(i, 1);
	CString PointMenuPrice  = m_listFood.GetItemText(i, 2);
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

	int nTotalMoney = 0;
	for (int i = 0; i < nCount; ++i)
	{
		CString strID = m_listBuy.GetItemText(i, 0);
		int nBuyCount = atoi(m_listBuy.GetItemText(i, 2));

		//查询价格
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
	// TODO: 在此添加控件通知处理程序代码
	m_listBuy.DeleteAllItems();
	m_nCount = 1;
	UpdateData(FALSE);
}

void CDlgFood::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i = m_listBuy.GetSelectionMark();  //获取信息位置

	if (i == -1)  //如果未选则提示
	{
		MessageBox("请先选择您要取消的菜！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}
	if(m_nCount<=0)
	{
		MessageBox("数量要大于0！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	//买的是买过的
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
