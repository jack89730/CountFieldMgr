#include "fielddata.h"
#include "tinyxml.h"
#include "public.h"
#include "perioddata.h"
int CFieldDataXML::AddField(CString strFieldName)
{
	CPeriodData* pPeriod = GetPeriodData;
	PeriodInfo periodInfo;
	pPeriod->Read(periodInfo);
	if (!periodInfo.m_bInit)
	{
		ASSERT(FALSE && "��û��ʼ��������ӳ���");
		return ERROR_UNKNOW_ERROR;
	}
	if (!m_bInit)
	{
		if (!myDocument.LoadFile(m_strXML.GetBuffer()))
		{
			return ERROR_OPEN_FILE;
		}
		m_bInit = TRUE;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ���ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	while(field)
	{
		CString strName = field->Attribute("name");
		if (strName == strFieldName)
		{
			return ERROR_NAME_EXIST;
		}
		field = field->NextSiblingElement("field");
	} 

	CString strID;
	strID.Format("%d", ++m_nMaxId);

	//���һ���½ڵ�
	TiXmlElement *FieldElement = new TiXmlElement("field");
	RootElement->LinkEndChild(FieldElement);
	FieldElement->SetAttribute("id", strID.GetBuffer());
	FieldElement->SetAttribute("name", strFieldName.GetBuffer());

	//�������
	TiXmlElement *DateElement = new TiXmlElement("date");
	FieldElement->LinkEndChild(DateElement);

	DateElement->SetAttribute("date", m_strDate.GetBuffer());


	for (int i = 0; i < periodInfo.m_nHalfCount; i++)
	{
		TiXmlElement *TimeElement = new TiXmlElement("time");

		CString strIndex;
		strIndex.Format("%d", i);
		TimeElement->SetAttribute("index", strIndex.GetBuffer());
		TimeElement->SetAttribute("booker", "");
		TimeElement->SetAttribute("bookprice", "0");
		DateElement->LinkEndChild(TimeElement);

		TiXmlText *TimeContent = new TiXmlText("0");
		TimeElement->LinkEndChild(TimeContent);
	}

	//myDocument.SaveFile();
	return ERROR_OK;
}
	
int CFieldDataXML::DeleteField(CString strFieldID)
{
	if (!m_bInit)
	{
		if (!myDocument.LoadFile(m_strXML.GetBuffer()))
		{
			return ERROR_OPEN_FILE;
		}
		m_bInit = TRUE;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��field�ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;
	while(field)
	{
		CString strIDFind = field->Attribute("id");
		if (strIDFind == strFieldID)
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
		if (NULL == pParNode)
		{
			return ERROR_UNKNOW_ERROR;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL != pParentEle)
		{
			if(pParentEle->RemoveChild(field))
			{
				myDocument.SaveFile();
			}
		}
	}
	return ERROR_OK;
}

int CFieldDataXML::ChangeFieldInfo(FieldInfo& info)
{
	if (!m_bInit)
	{
		if (!myDocument.LoadFile(m_strXML.GetBuffer()))
		{
			return ERROR_OPEN_FILE;
		}
		m_bInit = TRUE;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��field�ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;
	while(field)
	{
		CString strID = field->Attribute("id");
		if (strID == info.m_strFieldID)
		{
			break;
		}
		field = field->NextSiblingElement("field");
	}

	ASSERT(field);

	field->SetAttribute("name", info.m_strFieldName);
	return ERROR_OK;
}

int CFieldDataXML::ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info)
{
	if (!m_bInit)
	{
		if (!myDocument.LoadFile(m_strXML.GetBuffer()))
		{
			return ERROR_OPEN_FILE;
		}
		m_bInit = TRUE;
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
	TiXmlElement *DateElement = field->FirstChildElement("date");
	while (DateElement != NULL && DateElement->Attribute("date") != m_strDate)
	{
		DateElement = DateElement->NextSiblingElement("date");
	}
	if (DateElement == NULL)
	{
		AddFieldDay(strFieldID);
		DateElement = field->FirstChildElement("date");
		while (DateElement != NULL && DateElement->Attribute("date") != m_strDate)
		{
			DateElement = DateElement->NextSiblingElement("date");
		}
	}
	//ASSERT(DateElement);

	//�ҵ��ˣ���������
	TiXmlElement *TimeElement = DateElement->FirstChildElement("time");
	while(atoi(TimeElement->Attribute("index")) != info.m_nTimeIndex)
	{
		TimeElement = TimeElement->NextSiblingElement("time");
	}
	CString strBusy = "0";
	if (info.m_bBusy)
	{
		strBusy.Format("%d", info.m_bBusy);
		TimeElement->SetAttribute("booker", info.m_strVipID);
		TimeElement->SetAttribute("bookprice", info.m_strPrice);
	}
	else
	{
		TimeElement->SetAttribute("booker", "");
		TimeElement->SetAttribute("bookprice", "0");
	}

	//�Ƽ۷�ʽ���Զ�����ȡ��ǰϵͳ�趨���ֶ�����Ԥ��ʱ��д�ļ۸�
	if (info.m_bAutoPrice)
	{
		TimeElement->SetAttribute("autoprice", "1");
	}
	else
	{
		TimeElement->SetAttribute("autoprice", "0");
	}

	TimeElement->FirstChild()->SetValue(strBusy);

	//myDocument.SaveFile();
	return ERROR_OK;
}

int CFieldDataXML::GetAllData(FIELD_LIST& data)
{
	//ASSERT(data.IsEmpty());

	int nBegin = GetTickCount();
	if (!m_bInit)
	{
		if (!myDocument.LoadFile(m_strXML.GetBuffer()))
		{
			return ERROR_OPEN_FILE;
		}
		m_bInit = TRUE;
	}
	

	//�ڴ�ֻ������������

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i = 0;//��

	OneTimeBookInfo tmInfo;

	while(field)
	{
		OneFieldBookInfo info_insert;
		CString strID = field->Attribute("id");
		int  nID = atoi(strID);
		if (nID > m_nMaxId)
		{
			m_nMaxId = nID;
		}

		CString strName = field->Attribute("name");

		info_insert.m_strFieldID = strID;//���һ��field
		info_insert.m_strFieldName = strName;

		data.Add(info_insert);//���field ��ֻҪfield���ھ�Ҫ��ӣ����û�н��������vector<OneTimeBookInfo> m_ayTimeInfo == 0

		OneFieldBookInfo& info = data.GetAt(data.GetSize() - 1);


		//��������
		TiXmlElement *DateElement = field->FirstChildElement("date");
		while (DateElement != NULL && DateElement->Attribute("date") != m_strDate)
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
			//����BOOL
			int bIsBusy = atoi(TimeElement->FirstChild()->Value());

			if (bIsBusy)
			{
				tmInfo.m_strVipID = TimeElement->Attribute("booker");
				tmInfo.m_bAutoPrice = atoi(TimeElement->Attribute("autoprice"));
				if (tmInfo.m_bAutoPrice)
				{
					TimeElement->SetAttribute("bookprice", "0");
				}
				tmInfo.m_strPrice = TimeElement->Attribute("bookprice");
			}
			else
			{
				TimeElement->SetAttribute("booker", "");
				TimeElement->SetAttribute("bookprice", "0");
				TimeElement->SetAttribute("autoprice", "1");
			}

			tmInfo.m_bBusy = bIsBusy;
			int nIndex = atoi(TimeElement->Attribute("index"));
			tmInfo.m_nTimeIndex = nIndex;

			TimeElement = TimeElement->NextSiblingElement("time");
			info.m_ayTimeInfo.push_back(tmInfo);//������ӵ�˳����xml����д˳������timeindex��vector�е��±겻һ����

		}
		i++;
		field = field->NextSiblingElement("field");

	} 

	//myDocument.SaveFile();

	int nEnd = GetTickCount();
	TRACE("\nGetAllData��ʱ��%d\n",nEnd - nBegin);
	return ERROR_OK;
}

int CFieldDataXML::AddFieldDay(CString strFieldID)
{

	int nBegin = GetTickCount();
	if (!m_bInit)
	{
		if (!myDocument.LoadFile(m_strXML.GetBuffer()))
		{
			return ERROR_OPEN_FILE;
		}
		m_bInit = TRUE;
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

	DateElement->SetAttribute("date", m_strDate.GetBuffer());

	CPeriodData* pPeriod = GetPeriodData;
	PeriodInfo periodInfo;
	pPeriod->Read(periodInfo);
	for (int i = 0; i < periodInfo.m_nHalfCount; i++)
	{
		TiXmlElement *TimeElement = new TiXmlElement("time");

		CString strIndex;
		strIndex.Format("%d", i);
		TimeElement->SetAttribute("index", strIndex.GetBuffer());
		TimeElement->SetAttribute("booker", "");
		TimeElement->SetAttribute("autoprice", "1");
		TimeElement->SetAttribute("bookprice", "0");
		DateElement->LinkEndChild(TimeElement);

		TiXmlText *TimeContent = new TiXmlText("0");
		TimeElement->LinkEndChild(TimeContent);
	}

	//myDocument.SaveFile();

	return ERROR_OK;
}

void CFieldDataXML::Save()
{
	if (m_bInit)
	{
		myDocument.SaveFile();
	}
}