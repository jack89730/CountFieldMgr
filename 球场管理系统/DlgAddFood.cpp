// DlgAddFood.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAddFood.h"
#include "tinyxml.h"

// CDlgAddFood �Ի���

IMPLEMENT_DYNAMIC(CDlgAddFood, CDialog)

CDlgAddFood::CDlgAddFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddFood::IDD, pParent)
	, m_strName(_T(""))
	, m_strPrice(_T(""))
	, m_strID(_T(""))
{

}

CDlgAddFood::~CDlgAddFood()
{
}

void CDlgAddFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_strPrice);
	DDX_Text(pDX, IDC_EDIT3, m_strID);
}


BEGIN_MESSAGE_MAP(CDlgAddFood, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddFood::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddFood ��Ϣ�������

BOOL CDlgAddFood::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddFood::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	//TODO: ÿ�����Ϳ��������ϸ������

	if(m_strName.IsEmpty())
	{
		MessageBox("���Ʋ���Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strPrice.GetBuffer())< 0)
	{
		MessageBox("�۸���Ϊ������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//�򲻿��ļ����½�һ�����ļ�
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();
		return;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ���ڵ㡣
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;

	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == m_strID)
		{
			//��������ͬID
			MessageBox("ID�Ѿ�����", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		food = food->NextSiblingElement("vip");
	} 

	//���һ���½ڵ�
	TiXmlElement *FoodElement = new TiXmlElement("food");
	RootElement->LinkEndChild(FoodElement);
	//����PersonԪ�ص����ԡ�
	FoodElement->SetAttribute("id", m_strID.GetBuffer());
	//����nameԪ�ء�sexԪ�ز����ӡ�
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *PriceElement = new TiXmlElement("price");

	FoodElement->LinkEndChild(NameElement);
	FoodElement->LinkEndChild(PriceElement);

	//����nameԪ�غ�priceԪ�ص����ݲ����ӡ�
	TiXmlText *NameContent = new TiXmlText(m_strName.GetBuffer());
	TiXmlText *PriceContent = new TiXmlText(m_strPrice.GetBuffer());
	NameElement->LinkEndChild(NameContent);
	PriceElement->LinkEndChild(PriceContent);

	myDocument.SaveFile();

	CDialog::OnOK();

	m_bIsSuccess = TRUE;
}

void CDlgAddFood::OnBnClickedCommand1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
