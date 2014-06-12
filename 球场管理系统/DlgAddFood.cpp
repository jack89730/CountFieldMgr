// DlgAddFood.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAddFood.h"
#include "tinyxml.h"

// CDlgAddFood 对话框

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


// CDlgAddFood 消息处理程序

BOOL CDlgAddFood::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddFood::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	//TODO: 每种类型可以添加详细的限制

	if(m_strName.IsEmpty())
	{
		MessageBox("名称不能为空！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(atoi(m_strPrice.GetBuffer())< 0)
	{
		MessageBox("价格不能为负数！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//打不开文件就新建一个空文件
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();
		return;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个节点。
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;

	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == m_strID)
		{
			//不能有相同ID
			MessageBox("ID已经存在", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		food = food->NextSiblingElement("vip");
	} 

	//添加一个新节点
	TiXmlElement *FoodElement = new TiXmlElement("food");
	RootElement->LinkEndChild(FoodElement);
	//设置Person元素的属性。
	FoodElement->SetAttribute("id", m_strID.GetBuffer());
	//创建name元素、sex元素并连接。
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *PriceElement = new TiXmlElement("price");

	FoodElement->LinkEndChild(NameElement);
	FoodElement->LinkEndChild(PriceElement);

	//设置name元素和price元素的内容并连接。
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
	// TODO: 在此添加控件通知处理程序代码
}
