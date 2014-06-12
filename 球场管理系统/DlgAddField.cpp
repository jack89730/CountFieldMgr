// DlgAddField.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgAddField.h"
#include "tinyxml.h"
#include "public.h"
#include "MainDlg.h"

// CDlgAddField �Ի���

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


// CDlgAddField ��Ϣ�������


BOOL CDlgAddField::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_bIsSuccess = FALSE;
	return TRUE;
}

void CDlgAddField::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bIsSuccess = FALSE;

	UpdateData(TRUE);

	//TODO: ÿ�����Ϳ��������ϸ������

	if(m_strName.IsEmpty())
	{
		MessageBox(_T("���Ʋ���Ϊ�գ�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_strName.GetLength() >= 5)
	{
		MessageBox(_T("����ҪС��5��Ӣ���ַ���"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
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
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;

	while(field)
	{
		CString strID = field->Attribute("id");
		if (strID == m_strName)
		{
			//��������ͬID
			MessageBox(_T("ID�Ѿ�����"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
		field = field->NextSiblingElement("field");
	} 

	//���һ���½ڵ�
	TiXmlElement *FieldElement = new TiXmlElement("field");
	RootElement->LinkEndChild(FieldElement);
	//����PersonԪ�ص����ԡ�
	FieldElement->SetAttribute("id", m_strName.GetBuffer());

	//�������
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
