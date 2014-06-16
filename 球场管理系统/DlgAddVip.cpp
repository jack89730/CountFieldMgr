// DlgAddVip.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAddVip.h"
#include "access.h"
#include "tinyxml.h"

// CDlgAddVip 对话框

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


// CDlgAddVip 消息处理程序

BOOL CDlgAddVip::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddVip::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	if (m_strID.IsEmpty())
	{
		MessageBox("ID不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	if(m_strName.IsEmpty())
	{
		MessageBox("姓名不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strSex.IsEmpty())
	{
		MessageBox("性别不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strPhone.IsEmpty())
	{
		MessageBox( "手机号码不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strMoney.GetBuffer())< 0)
	{
		MessageBox("余额不能为负数！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//打不开文件就新建一个空文件
	if (!myDocument.LoadFile())
	{
		//CreateVipXML();
		return;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;

	while(vip)
	{
		CString strID = vip->Attribute("id");
		if (strID == m_strID)
		{
			//不能有相同ID
			MessageBox("ID已经存在", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		vip = vip->NextSiblingElement("vip");
	} 

	//添加一个新节点
	TiXmlElement *VipElement = new TiXmlElement("vip");
	RootElement->LinkEndChild(VipElement);
	//设置Person元素的属性。
	VipElement->SetAttribute("id", m_strID.GetBuffer());
	//创建name元素、sex元素并连接。
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *SexElement = new TiXmlElement("sex");
	TiXmlElement *PhoneElement = new TiXmlElement("phone");
	TiXmlElement *MoneyElement = new TiXmlElement("money");

	VipElement->LinkEndChild(NameElement);
	VipElement->LinkEndChild(SexElement);
	VipElement->LinkEndChild(PhoneElement);
	VipElement->LinkEndChild(MoneyElement);

	//设置name元素和sex元素的内容并连接。
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

