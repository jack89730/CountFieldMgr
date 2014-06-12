// DlgVip.cpp : ʵ���ļ�
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

// CDlgVip �Ի���

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


// CDlgVip ��Ϣ�������

BOOL CDlgVip::OnInitDialog()
{
	m_strXML = TL_GetModulePath(NULL) + "vip.xml";
	m_nMaxID = 0;

	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_listVip.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);   
	int k=0;
	m_listVip.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("����"),	LVCFMT_CENTER, 100);
	m_listVip.InsertColumn(k++, _T("�Ա�"),	LVCFMT_CENTER, 60);
	m_listVip.InsertColumn(k++, _T("�绰"),	LVCFMT_CENTER, 120);
	m_listVip.InsertColumn(k++, _T("���"),	LVCFMT_CENTER, 80);

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

		//��ø�Ԫ��
		TiXmlElement *RootElement = myDocument->RootElement();
		//��õ�һ��vip�ڵ㡣
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION	pos;
	int			iPos;
	pos = m_listVip.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫ�޸ĵļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listVip.GetNextSelectedItem(pos);
	CString strID		= m_listVip.GetItemText(iPos, 0);

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;
	while(vip)
	{
		CString strIDFind = vip->Attribute("id");
		if (strIDFind == strID)
		{
			//�ҵ���
			break;
		}

		vip = vip->NextSiblingElement("vip");

	} 
	if (vip)
	{
		//�ҵ���
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
		MessageBox("����ѡ��Ҫ�޸ĵļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
	VipInfo * pInfo = GetVipInfo(info.m_strID);
	if (!pInfo)
	{
		return -1;
	}
	*pInfo = info;//���ڴ��е�ֵ

	//���ļ�

	//����һ��XML���ĵ�����
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return -1;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
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
		//�ҵ��ˣ���������
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