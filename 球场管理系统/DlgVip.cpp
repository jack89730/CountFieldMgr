// DlgVip.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgVip.h"
#include "DlgAddVip.h"
#include "DlgModifyVip.h"
#include "DlgCharge.h"
#include "tinyxml.h"
#include "public.h"
#include <iostream>
#include "MainDlg.h"
using namespace std;

// CDlgVip 对话框

IMPLEMENT_DYNAMIC(CDlgVip, CDialog)

CDlgVip::CDlgVip(CWnd* pParent /*=NULL*/)
: CDialog(CDlgVip::IDD, pParent)
{

}

CDlgVip::~CDlgVip()
{
}

void CDlgVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIP, m_listVip);
}


BEGIN_MESSAGE_MAP(CDlgVip, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgVip::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgVip::OnBnClickedBtnDel)
	ON_BN_CLICKED(IDC_BTN_MODIFY, &CDlgVip::OnBnClickedBtnModify)
	ON_BN_CLICKED(IDC_BTN_CHARGE, &CDlgVip::OnBnClickedBtnCharge)
END_MESSAGE_MAP()


// CDlgVip 消息处理程序

BOOL CDlgVip::OnInitDialog()
{
	m_strXML = TL_GetModulePath(NULL) + "vip.xml";
	m_nMaxID = 0;

	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_listVip.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listVip.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("姓名"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("性别"),	LVCFMT_CENTER, 60);
	m_listVip.InsertColumn(k++, _T("电话"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("余额"),	LVCFMT_CENTER, 80);

	ShowItemVip();
	return TRUE;
}
void CDlgVip::OnBnClickedBtnAdd()
{
	CDlgAddVip dlg;

	dlg.m_strID.Format("%d", ++m_nMaxID);
	dlg.m_strXML = m_strXML;

	dlg.DoModal();

	if (dlg.IsSuccess())
	{
		ShowItemVip();
	}
}

void CDlgVip::ShowItemVip()
{

	do 
	{
		TiXmlDocument *myDocument = new TiXmlDocument(m_strXML.GetBuffer());
		if (!myDocument->LoadFile())
		{
			break;
		}

		m_listVip.DeleteAllItems();
		m_ayVip.RemoveAll();

		//获得根元素
		TiXmlElement *RootElement = myDocument->RootElement();
		//获得第一个vip节点。
		TiXmlElement *vip = RootElement->FirstChildElement();

		int i=0;
		while(vip)
		{
			CString strID = vip->Attribute("id");
			int nID = atoi(strID);

			if (nID > m_nMaxID)
			{
				m_nMaxID = nID;
			}

			TiXmlElement *NameElement = vip->FirstChildElement("name");
			TiXmlElement *SexElement = NameElement->NextSiblingElement("sex");
			TiXmlElement *PhoneElement = NameElement->NextSiblingElement("phone");
			TiXmlElement *MoneyElement = NameElement->NextSiblingElement("money");

			CString strName = NameElement->FirstChild()->Value();
			CString strSex = SexElement->FirstChild()->Value();
			CString strPhone = PhoneElement->FirstChild()->Value();
			CString strMoney = MoneyElement->FirstChild()->Value();

			VipInfo info;
			int j = 0;
			m_listVip.InsertItem(i, strID);
			m_listVip.SetItemText(i, ++j, strName);
			m_listVip.SetItemText(i, ++j, strSex);
			m_listVip.SetItemText(i, ++j, strPhone);
			m_listVip.SetItemText(i, ++j, strMoney);

			info.m_strID = strID;
			info.m_strSex = strSex;
			info.m_strPhone = strPhone;
			info.m_strMoney = strMoney;
			info.m_strName = strName;

			m_ayVip.Add(info);
			i++;
			vip = vip->NextSiblingElement("vip");

		} 
		if (!vip)
		{
			delete myDocument;
			break;
		}

	} while (1);

	if (m_listVip.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_MODIFY)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_MODIFY)->EnableWindow(TRUE);
		m_listVip.SetSelectionMark(0);
	}

	UpdateData(FALSE);
}

void CDlgVip::OnBnClickedBtnDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	CString strID		= m_listVip.GetItemText(iPos, 0);

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;
	while(vip)
	{
		CString strIDFind = vip->Attribute("id");
		if (strIDFind == strID)
		{
			//找到了
			break;
		}

		vip = vip->NextSiblingElement("vip");

	} 
	if (vip)
	{
		//找到了
		TiXmlNode *pParNode =  vip->Parent();
		if (NULL==pParNode)
		{
			return;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL!=pParentEle)
		{
			if(pParentEle->RemoveChild(vip))
			{
				myDocument.SaveFile();
				m_listVip.DeleteAllItems();
				ShowItemVip();
			}
		}
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
		MessageBox("请先选择要修改的记录！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	dlg.m_strID		= m_listVip.GetItemText(iPos, 0);
	dlg.m_strName		= m_listVip.GetItemText(iPos, 1);
	dlg.m_strSex		= m_listVip.GetItemText(iPos, 2);
	dlg.m_strPhone   	= m_listVip.GetItemText(iPos, 3);
	dlg.m_strMoney	    = m_listVip.GetItemText(iPos, 4);
	dlg.m_strXML = m_strXML;

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

VipInfo* CDlgVip::GetVipInfo(CString strVipID)
{
	//找到vip后查看他的个人信息
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
	//修改vip信息，并且更新xml文件
	VipInfo * pInfo = GetVipInfo(info.m_strID);
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
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;
	while(vip)
	{
		CString strID = vip->Attribute("id");
		if (strID == info.m_strID)
		{
			break;
		}
		vip = vip->NextSiblingElement("vip");
	} 

	if (vip)
	{
		//找到了，更新数据
		TiXmlElement *NameElement = vip->FirstChildElement("name");
		TiXmlElement *SexElement = NameElement->NextSiblingElement("sex");
		TiXmlElement *PhoneElement = NameElement->NextSiblingElement("phone");
		TiXmlElement *MoneyElement = NameElement->NextSiblingElement("money");

		NameElement->FirstChild()->SetValue(info.m_strName);
		SexElement->FirstChild()->SetValue(info.m_strSex);
		PhoneElement->FirstChild()->SetValue(info.m_strPhone);
		MoneyElement->FirstChild()->SetValue(info.m_strMoney);

		myDocument.SaveFile();
	}
	return 0;
}