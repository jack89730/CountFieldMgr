// DlgField.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgField.h"
#include "public.h"
#include "DlgBook.h"
#include "tinyxml.h"
#include "xmlbase.h"
#include "DlgBookInfo.h"
#include "MainDlg.h"
#include "DlgAddField.h"

// CDlgField �Ի���

IMPLEMENT_DYNAMIC(CDlgField, CDialog)

CDlgField::CDlgField(CWnd* pParent /*=NULL*/)
: CDialog(CDlgField::IDD, pParent)
, m_date(COleDateTime::GetCurrentTime())
{

}

CDlgField::~CDlgField()
{
}

void CDlgField::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIELD, m_listField);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_date);
}

BEGIN_MESSAGE_MAP(CDlgField, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_FIELD, &CDlgField::OnNMCustomdrawListField)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_FIELD, &CDlgField::OnNMRClickListField)
	ON_COMMAND(ID_BOOK, &CDlgField::OnBook)
	ON_COMMAND(ID_BOOK_INFO, &CDlgField::OnBookInfo)
	ON_COMMAND(ID_CANCEL_BOOK, &CDlgField::OnCancelBook)
	ON_WM_MOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_ADD, &CDlgField::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CDlgField::OnBnClickedBtnDel)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CDlgField::OnDtnDatetimechangeDatetimepicker1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgField::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgField::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlgField ��Ϣ�������

BOOL CDlgField::OnInitDialog()
{
	GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	m_strXML = TL_GetModulePath(NULL) + "field.xml";
	CDialog::OnInitDialog();
	m_listField.SetExtendedStyle(LVS_EX_GRIDLINES/*|LVS_EX_FULLROWSELECT*/);  

// 	for (int i = 1; i < 15; ++i)
// 	{
// 		m_listField.SetColumnWidth(i, 14);
// 	}

	int k=0;
	m_listField.InsertColumn(k++, _T("��"),	LVCFMT_CENTER, 60);
	m_nTimeCount = 17;
	m_ayTimeByIndex.RemoveAll();
	int hour = 8;
	for (int i = 0; i < m_nTimeCount; i++)
	{
		CString strTmp;
		if (i%2 == 0)
		{
			hour++;
			strTmp.Format("%d:00-%d:30", hour , hour);
		}
		else
		{
			strTmp.Format("%d:30-%d:00", hour , hour + 1);
		}
		
		m_listField.InsertColumn(k++, strTmp,	LVCFMT_CENTER, 36);
		m_ayTimeByIndex.Add(strTmp);

		//CRect rect;
		//GetDlgItem(IDC_LIST_FIELD)->GetWindowRect(&rect);
		//rect.top -= 20;
		//rect.left += i * 60;
		//rect.right += i*60;

		//CDC*pDC = GetDC();
		//pDC->DrawText("hello\nworld", &rect, 0);


	}
	//for (int i = 0; i < 10; ++i)
	//{
	//	CRect rect;
	//	CDC*pDC = GetDC();
	//	m_listField.GetSubItemRect(0,i,LVIR_BOUNDS,rect);
	//	rect.top -= 10;
	//	rect.bottom -= 10;
	//	pDC->DrawText("hello\nworld", &rect, 0);
	//}

	m_listField.SetRowHeight(36);
	ShowItemField(GetDate());

	SetPreBtnStatus();

	return TRUE;
}

void CDlgField::SetPreBtnStatus()
{
	CString strDate = GetDate();
	CTime tim(time(NULL));
	int nSelectDay = atoi(strDate.GetBuffer());
	strDate.Format("%04d%02d%02d", tim.GetYear(), tim.GetMonth(), tim.GetDay());
	int nToday = atoi(strDate.GetBuffer());
	if (nSelectDay <= nToday)
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	}
}

void CDlgField::ShowItemField(CString strDate)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer()); 
	if (!myDocument.LoadFile())
	{
		return;
	}

	//�ڴ�ֻ������������
	m_listField.DeleteAllItems();
	m_ayFieldInfo.RemoveAll();

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i = 0;//��

	OneTimeInfo tmInfo;

	while(field)
	{
		OneFieldInfo info_insert;
		CString strID = field->Attribute("id");

		m_listField.InsertItem(i, strID);
		info_insert.m_strFieldID = strID;//���һ��field
		m_ayFieldInfo.Add(info_insert);//���field��ֻҪfield���ھ�Ҫ��ӣ����û�н��������vector<OneTimeInfo> m_ayTimeInfo == 0

		OneFieldInfo& info = m_ayFieldInfo.GetAt(m_ayFieldInfo.GetSize() - 1);


		//��������
		TiXmlElement *DateElement = field->FirstChildElement("date");
		while (DateElement != NULL && DateElement->Attribute("date") != strDate)
		{
			DateElement = DateElement->NextSiblingElement("date");
		}

		if (DateElement == NULL)
		{
			//û�������ڵ�����
			field = field->NextSiblingElement("field");
			i++;
			continue;
		}
		//�ҵ�������

		TiXmlElement *TimeElement = DateElement->FirstChildElement("time");

		while (TimeElement)
		{
			BOOL bIsBusy = atoi(TimeElement->FirstChild()->Value());

			if (bIsBusy)
			{
				tmInfo.m_strVipID = TimeElement->Attribute("booker");
			}
			else
			{
				TimeElement->SetAttribute("booker", "");
			}

			tmInfo.m_bBusy = bIsBusy;
			int nIndex = atoi(TimeElement->Attribute("index"));
			tmInfo.m_nTimeIndex = nIndex;

			if (bIsBusy)
			{
				m_listField.SetItemText(i, nIndex+1, FIELD_BUSY);
			}
			else
			{
				m_listField.SetItemText(i, nIndex+1, FIELD_IDLE);
			}
			TimeElement = TimeElement->NextSiblingElement("time");
			info.m_ayTimeInfo.push_back(tmInfo);//������ӵ�˳����xml����д˳������timeindex��vector�е��±겻һ����

		}
		i++;
		field = field->NextSiblingElement("field");

	} 

	myDocument.SaveFile();

	if (m_listField.GetItemCount() == 0)
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BTN_DEL)->EnableWindow(TRUE);
		m_listField.SetSelectionMark(0);
	}

	UpdateData(FALSE);
}
void CDlgField::OnNMCustomdrawListField(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);

	switch (pNMCD->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		return;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		return;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:

		// 		int nFieldCount = m_listField.GetItemCount();
		// 		for (int row = 0; row < nFieldCount; row++)
		// 		{
		// 
		// 			for (int col = 1; col <= m_nTimeCount + 1; col++)
		// 			{
		// 
		// 				if (col == m_nTimeCount)
		// 				{
		// 					col = m_nTimeCount;
		// 				}
		// 				CString str = m_listField.GetItemText(row,col);
		// 				CRect rect;
		// 				CDC*pDC = CDC::FromHandle(pNMCD->nmcd.hdc);
		// 
		// 				m_listField.GetSubItemRect(row,col,LVIR_BOUNDS,rect);
		// 
		// 				UINT uCode=DT_LEFT;
		// 
		// 				COLORREF kolor = RGB(255,255,255);
		// 
		// 				if (str == FIELD_BUSY)
		// 				{
		// 					kolor = FIELD_BUSY_COLOR;
		// 				}
		// 				else if (str == FIELD_IDLE)
		// 				{
		// 					kolor = FIELD_IDLE_COLOR;
		// 				}
		// 				else
		// 				{
		// 					kolor = FIELD_NULL_COLOR;
		// 				}
		// 
		// 				CBrush brush(kolor);
		// 				pDC->FillRect(&rect,&brush);
		// 				//rect.OffsetRect(10,0);
		// 				//pDC->DrawText(str,&rect,uCode);
		// 			}
		// 	}
		break;
	}
	*pResult = 0;
}

void CDlgField::OnNMRClickListField(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CListCtrl *CList =&m_listField;


	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;
	char chstr[MAX_PATH];

	m_listField.GetItemText(pNotifier->iItem,pNotifier->iSubItem,chstr,MAX_PATH);

	m_curSelectRow = pNotifier->iItem;
	m_curSelectCol = pNotifier->iSubItem;

	if (m_curSelectCol < 0 || m_curSelectRow < 0)
	{
		return;
	}
	CString str=chstr;

	CMenu menu ,* pSubMenu; //��������Ҫ�õ���cmenu����

	if (IS_FIELD_BUSY(str))
	{
		menu.LoadMenu(IDR_MENU_BUSY);
	}
	else/* if (IS_FIELD_IDLE(str))*/
	{
		menu.LoadMenu(IDR_MENU_IDLE);
	}
	//else
	//{
	//	return;
	//}

	//��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵�
	pSubMenu = menu.GetSubMenu(0);
	CPoint oPoint; //����һ������ȷ�����λ�õ�λ��
	GetCursorPos( &oPoint); //��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	//��ָ��λ����ʾ�����˵�
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this);

	*pResult = 0;
}

void CDlgField::OnBook()
{
	// TODO: �ڴ���������������
	CDlgBook dlg;
	int nTimeIndex = m_curSelectCol - 1;
	dlg.m_strBookTime = m_ayTimeByIndex[m_curSelectCol - 1];
	dlg.m_strFieldID = m_ayFieldInfo[m_curSelectRow].m_strFieldID;
	dlg.m_strXML = m_strXML;

	OneTimeInfo* p = GetFieldInfo(dlg.m_strFieldID, nTimeIndex);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldInfo(dlg.m_strFieldID, nTimeIndex);
	}

	int nIndexStart = nTimeIndex;
	while(p && p->m_bBusy == FALSE)
	{
		p = GetFieldInfo(dlg.m_strFieldID, --nIndexStart);
 	}
	nIndexStart++;

	p = GetFieldInfo(dlg.m_strFieldID, nIndexStart);
	if (p == NULL)
	{
		AddFieldDay(dlg.m_strFieldID);
		p = GetFieldInfo(dlg.m_strFieldID, nTimeIndex);
	}
	while (p && p->m_bBusy == FALSE)
	{
		CString strTime = m_ayTimeByIndex[nIndexStart];
		int nMin = strTime.Find('-');
		CString strStart = strTime.Mid(0, nMin);
		CString strStop = strTime.Mid(nMin+1);
		dlg.m_ayStartTimeStr.Add(strStart);
		dlg.m_ayStopTimeStr.Add(strStop);
		dlg.m_ayIndex.Add(nIndexStart);
		p = GetFieldInfo(dlg.m_strFieldID, ++nIndexStart);
	}

	int nIndexCount = dlg.m_ayIndex.GetSize();
	dlg.m_nTimeIndex = 0;
	for (int i = 0; i < nIndexCount; ++i)
	{
		if (dlg.m_ayIndex[i] < nTimeIndex)
		{
			dlg.m_nTimeIndex ++;
		}
		else
		{
			break;
		}
		
	}
	dlg.m_nTimeIndex++;
	dlg.DoModal();
	if (dlg.m_bSuccess)
	{
		ShowItemField(GetDate());
	}
}


void CDlgField::OnBookInfo()
{
	CDlgBookInfo dlg;

	int nTimeIndex = m_curSelectCol - 1;
	CString strFieldID = m_ayFieldInfo[m_curSelectRow].m_strFieldID;

	CString strTitle;
	strTitle.Format("���أ�%s ʱ�䣺%s", strFieldID.GetBuffer(), m_ayTimeByIndex[nTimeIndex]);
	dlg.m_strTitle = strTitle;

	CString strVipID;
	int n = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo.size();
	for (int i = 0; i < n; i++)
	{
		if (m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
		{
			strVipID = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_strVipID;
		}
	}
	ASSERT(!strVipID.IsEmpty());

	//�ҵ�vip��鿴���ĸ�����Ϣ
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	int nVipCount = pMainWnd->m_pageVip.m_ayVip.GetCount();

	int j = 0;
	for (; j < nVipCount; ++j)
	{
		if (pMainWnd->m_pageVip.m_ayVip[j].m_strID == strVipID)
		{
			break;
		}
	}

	ASSERT(j < nVipCount);

	VipInfo& info = pMainWnd->m_pageVip.m_ayVip[j];

	CString strMsg;

	strMsg.Format("���أ�%s\nʱ�䣺%s\nԤ����ID��%s\n������%s\n�Ա�%s\n�绰��%s"
		, strFieldID.GetBuffer()
		, m_ayTimeByIndex[nTimeIndex]
	, info.m_strID.GetBuffer()
		, info.m_strName.GetBuffer()
		,info.m_strSex.GetBuffer()
		, info.m_strPhone.GetBuffer());

	MessageBox(strMsg,"��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
	//ȡ��ʹ��bookinfo dlg
}


void CDlgField::OnCancelBook()
{
	// TODO: �ڴ���������������

	int nTimeIndex = m_curSelectCol - 1;
	CString strFieldID = m_ayFieldInfo[m_curSelectRow].m_strFieldID;

	//����Ԥ������Ϣ
	CString strVipID;
	int n = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo.size();
	for (int i = 0; i < n; i++)
	{
		if (m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
		{
			strVipID = m_ayFieldInfo[m_curSelectRow].m_ayTimeInfo[i].m_strVipID;
		}
	}
	ASSERT(!strVipID.IsEmpty());

	//�ҵ�vip��鿴���ĸ�����Ϣ
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	VipInfo* info  = pMainWnd->m_pageVip.GetVipInfo(strVipID);
	ASSERT(info);

	CString strMsg;

	strMsg.Format("���أ�%s\nʱ�䣺%s\nԤ����ID��%s\n������%s\n�绰��%s\n\n�Ƿ�ȡ��Ԥ����"
		, strFieldID.GetBuffer()
		, m_ayTimeByIndex[nTimeIndex]
	, info->m_strID.GetBuffer()
		, info->m_strName.GetBuffer()
		, info->m_strPhone.GetBuffer());

	if(MessageBox(strMsg,"��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL|MB_YESNO) == IDNO)
		return;

	OneTimeInfo infoBook;

	OneTimeInfo* pinfo = GetFieldInfo(strFieldID, nTimeIndex);
	if (!pinfo)
	{
		return;
	}
	infoBook =  *pinfo;

	//������Ϣ
	infoBook.m_bBusy = FALSE;
	infoBook.m_strVipID = "";
	ChangeFieldInfo(strFieldID, infoBook);

	ShowItemField(GetDate());

}

void CDlgField::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnMouseMove(nFlags, point);
}

void CDlgField::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnNcLButtonDown(nHitTest, point);
}

void CDlgField::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CDlgField::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgField::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgField::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgAddField dlg;
	dlg.m_strXML = m_strXML;
	dlg.DoModal();
	if (dlg.IsSuccess())
	{
		ShowItemField(GetDate());
	}
}

void CDlgField::OnBnClickedBtnDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION	pos;
	int			iPos;
	pos = m_listField.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫɾ���ļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listField.GetNextSelectedItem(pos);
	CString strID		= m_listField.GetItemText(iPos, 0);

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��field�ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;
	while(field)
	{
		CString strIDFind = field->Attribute("id");
		if (strIDFind == strID)
		{
			//�ҵ���
			break;
		}

		field = field->NextSiblingElement("field");
	} 
	if (field)
	{
		//�ҵ���
		TiXmlNode *pParNode =  field->Parent();
		if (NULL==pParNode)
		{
			return;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL!=pParentEle)
		{
			if(pParentEle->RemoveChild(field))
			{
				myDocument.SaveFile();
				m_listField.DeleteAllItems();
				ShowItemField(GetDate());
			}
		}
	}
}

OneTimeInfo* CDlgField::GetFieldInfo(CString strFieldID, int nTimeIndex)
{
	int nFieldCount = m_ayFieldInfo.GetCount();

	int j = 0;
	for (; j < nFieldCount; ++j)
	{
		if (m_ayFieldInfo[j].m_strFieldID == strFieldID)
		{
			break;
		}
	}

	if (j == nFieldCount)
	{
		return NULL;
	}

	int nTimeCount = m_ayFieldInfo[j].m_ayTimeInfo.size();

	for (int i = 0; i < nTimeCount; ++i)
	{
		if (m_ayFieldInfo[j].m_ayTimeInfo[i].m_nTimeIndex == nTimeIndex)
		{
			return &m_ayFieldInfo[j].m_ayTimeInfo[i];
		}
	}
	return NULL;
}

int CDlgField::AddFieldDay(CString strFieldID)
{
	UpdateData(TRUE);
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//�򲻿��ļ����½�һ�����ļ�
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();//��ûʵ��
		return -1;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ���ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;

	while(field)
	{
		CString strID = field->Attribute("id");
		if (strID == strFieldID)
		{
			break;
		}
		field = field->NextSiblingElement("field");
	}

	ASSERT(field);//Ӧ�����ҵ��Ŷ�
	TiXmlElement *FieldElement = field;
	if (field == NULL)
	{
		//���һ���½ڵ�
		FieldElement = new TiXmlElement("field");
		RootElement->LinkEndChild(FieldElement);
		//����PersonԪ�ص����ԡ�
		FieldElement->SetAttribute("id", strFieldID.GetBuffer());
	}

	//�������
	TiXmlElement *DateElement = new TiXmlElement("date");
	FieldElement->LinkEndChild(DateElement);

	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;

	DateElement->SetAttribute("date", GetDate().GetBuffer());


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

	ShowItemField(GetDate());

	return 0;
}

int CDlgField::ChangeFieldInfo(CString strFieldID, OneTimeInfo& info)
{
	//�޸���Ϣ�����Ҹ���xml�ļ�
	OneTimeInfo * pInfo = GetFieldInfo(strFieldID, info.m_nTimeIndex);
	if (!pInfo)
	{
		//���fieldû���������ڵ����ݣ��ͻ�Ϊ�գ���ô�������
		AddFieldDay(strFieldID);
		pInfo = GetFieldInfo(strFieldID, info.m_nTimeIndex);
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
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;
	while(field)
	{
		CString strID = field->Attribute("id");
		if (strID == strFieldID)
		{
			break;
		}
		field = field->NextSiblingElement("field");
	} 

	ASSERT(field);


	//��������
	CString strDate = GetDate();
	TiXmlElement *DateElement = field->FirstChildElement("date");
	while (DateElement != NULL && DateElement->Attribute("date") != strDate)
	{
		DateElement = DateElement->NextSiblingElement("date");
	}

	ASSERT(DateElement);
	//�ҵ��ˣ���������
	TiXmlElement *TimeElement = DateElement->FirstChildElement("time");
	while(atoi(TimeElement->Attribute("index")) != info.m_nTimeIndex)
	{
		TimeElement = TimeElement->NextSiblingElement("time");
	}
	CString strBusy = "0";
	if (info.m_bBusy)
	{
		strBusy = "1";
		TimeElement->SetAttribute("booker", info.m_strVipID);
	}
	TimeElement->FirstChild()->SetValue(strBusy);
	
	myDocument.SaveFile();
	return 0;

}

void CDlgField::OnDtnDatetimechangeDatetimepicker1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UpdateData(TRUE);
	CString strDate = GetDate();
	ShowItemField(strDate);
	SetPreBtnStatus();
}

CString CDlgField::GetDate()
{
	UpdateData(TRUE);
	CString strDate;
	strDate.Format("%04d%02d%02d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay());
	return strDate;
}
void CDlgField::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_date -= 1;
	CString strDate;
	strDate.Format("%04d%02d%02d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay());
	UpdateData(FALSE);
	ShowItemField(strDate);
	SetPreBtnStatus();
}

void CDlgField::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_date += 1;
	CString strDate;
	strDate.Format("%04d%02d%02d", m_date.GetYear(), m_date.GetMonth(), m_date.GetDay());
	UpdateData(FALSE);
	ShowItemField(strDate);
	SetPreBtnStatus();
}
