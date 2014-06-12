// DlgAddField.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgAddField.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"

// CDlgAddField 对话框

IMPLEMENT_DYNAMIC(CDlgAddField, CDialog)

CDlgAddField::CDlgAddField(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddField::IDD, pParent)
	, m_strName(_T(""))
{

}

CDlgAddField::~CDlgAddField()
{
}

void CDlgAddField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
}


BEGIN_MESSAGE_MAP(CDlgAddField, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgAddField::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAddField 消息处理程序


BOOL CDlgAddField::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddField::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	//TODO: 每种类型可以添加详细的限制

	if(m_strName.IsEmpty())
	{
		MessageBox(_T("名称不能为空！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strName.GetLength() >= 5)
	{
		MessageBox(_T("名称要小于5个英文字符！"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;

	while(field)
	{
		CString strID = field->Attribute("id");
		if (strID == m_strName)
		{
			//不能有相同ID
			MessageBox(_T("ID已经存在"), _T("提示"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		field = field->NextSiblingElement("field");
	} 

	//添加一个新节点
	TiXmlElement *FieldElement = new TiXmlElement("field");
	RootElement->LinkEndChild(FieldElement);
	//设置Person元素的属性。
	FieldElement->SetAttribute("id", m_strName.GetBuffer());

	//添加日期
	TiXmlElement *DateElement = new TiXmlElement("date");
	FieldElement->LinkEndChild(DateElement);

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	CString strDate;
	strDate.Format("%04d%02d%02d", pMainWnd->m_pageField.m_date.GetYear()
		, pMainWnd->m_pageField.m_date.GetMonth()
		, pMainWnd->m_pageField.m_date.GetDay());

	DateElement->SetAttribute("date", strDate.GetBuffer());


	int nTimeCount = pMainWnd->m_pageField.m_nTimeCount;
	for (int i = 0; i < nTimeCount; i++)
	{
		TiXmlElement *TimeElement = new TiXmlElement("time");
		
		CString strIndex;
		strIndex.Format("%d", i);
		TimeElement->SetAttribute("index", strIndex.GetBuffer());
		TimeElement->SetAttribute("booker", "");
		DateElement->LinkEndChild(TimeElement);

		TiXmlText *TimeContent = new TiXmlText("0");
		TimeElement->LinkEndChild(TimeContent);
	}

	myDocument.SaveFile();

	CDialog::OnOK();

	m_bIsSuccess = TRUE;
}
