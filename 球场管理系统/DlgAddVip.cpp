// DlgAddVip.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAddVip.h"
#include "access.h"
#include "tinyxml.h"

// CDlgAddVip �Ի���

IMPLEMENT_DYNAMIC(CDlgAddVip, CDialog)

CDlgAddVip::CDlgAddVip(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddVip::IDD, pParent)
	, m_strName(_T(""))
	, m_strSex(_T(""))
	, m_strPhone(_T(""))
	, m_strMoney(_T(""))
	, m_strID(_T(""))
{

}

CDlgAddVip::~CDlgAddVip()
{
}

void CDlgAddVip::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strName);
	DDX_Text(pDX, IDC_EDIT2, m_strSex);
	DDX_Text(pDX, IDC_EDIT3, m_strPhone);
	DDX_Text(pDX, IDC_EDIT4, m_strMoney);
	DDX_Text(pDX, IDC_EDIT_ID, m_strID);
}


BEGIN_MESSAGE_MAP(CDlgAddVip, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddVip::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddVip ��Ϣ�������

BOOL CDlgAddVip::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddVip::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	if (m_strID.IsEmpty())
	{
		MessageBox("ID����Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if(m_strName.IsEmpty())
	{
		MessageBox("��������Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strSex.IsEmpty())
	{
		MessageBox("�Ա���Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strPhone.IsEmpty())
	{
		MessageBox( "�ֻ����벻��Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strMoney.GetBuffer())< 0)
	{
		MessageBox("����Ϊ������", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//�򲻿��ļ����½�һ�����ļ�
	if (!myDocument.LoadFile())
	{
		//CreateVipXML();
		return;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;

	while(vip)
	{
		CString strID = vip->Attribute("id");
		if (strID == m_strID)
		{
			//��������ͬID
			MessageBox("ID�Ѿ�����", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		vip = vip->NextSiblingElement("vip");
	} 

	//���һ���½ڵ�
	TiXmlElement *VipElement = new TiXmlElement("vip");
	RootElement->LinkEndChild(VipElement);
	//����PersonԪ�ص����ԡ�
	VipElement->SetAttribute("id", m_strID.GetBuffer());
	//����nameԪ�ء�sexԪ�ز����ӡ�
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *SexElement = new TiXmlElement("sex");
	TiXmlElement *PhoneElement = new TiXmlElement("phone");
	TiXmlElement *MoneyElement = new TiXmlElement("money");

	VipElement->LinkEndChild(NameElement);
	VipElement->LinkEndChild(SexElement);
	VipElement->LinkEndChild(PhoneElement);
	VipElement->LinkEndChild(MoneyElement);

	//����nameԪ�غ�sexԪ�ص����ݲ����ӡ�
	TiXmlText *NameContent = new TiXmlText(m_strName.GetBuffer());
	TiXmlText *SexContent = new TiXmlText(m_strSex.GetBuffer());
	TiXmlText *PhoneContent = new TiXmlText(m_strPhone.GetBuffer());
	TiXmlText *MoneyContent = new TiXmlText(m_strMoney.GetBuffer());

	NameElement->LinkEndChild(NameContent);
	SexElement->LinkEndChild(SexContent);
	PhoneElement->LinkEndChild(PhoneContent);
	MoneyElement->LinkEndChild(MoneyContent);

	myDocument.SaveFile();

	CDialog::OnOK();

	m_bIsSuccess = TRUE;

}

