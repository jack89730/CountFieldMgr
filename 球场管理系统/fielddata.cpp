#include "fielddata.h"
#include "tinyxml.h"
#include "public.h"
int CFieldDataXML::AddField(CString strFieldID)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//�򲻿��ļ����½�һ�����ļ�
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();
		return  ERROR_OPEN_FILE;
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
			return ERROR_ID_EXIST;
		}
		field = field->NextSiblingElement("field");
	} 

	//���һ���½ڵ�
	TiXmlElement *FieldElement = new TiXmlElement("field");
	RootElement->LinkEndChild(FieldElement);
	FieldElement->SetAttribute("id", strFieldID.GetBuffer());

	//�������
	TiXmlElement *DateElement = new TiXmlElement("date");
	FieldElement->LinkEndChild(DateElement);

	DateElement->SetAttribute("date", m_strDate.GetBuffer());


	for (int i = 0; i < m_nTimeCount; i++)
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
	return ERROR_OK;
}
	
int CFieldDataXML::DeleteField(CString strFieldID)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return ERROR_OPEN_FILE;
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

int CFieldDataXML::ChangeFieldInfo(CString strFieldID, OneTimeInfo& info)
{
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
	TiXmlElement *DateElement = field->FirstChildElement("date");
	while (DateElement != NULL && DateElement->Attribute("date") != m_strDate)
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
	else
	{
		TimeElement->SetAttribute("booker", "");
	}
	TimeElement->FirstChild()->SetValue(strBusy);

	myDocument.SaveFile();
	return ERROR_OK;
}

int CFieldDataXML::GetAllData(FIELD_LIST& data)
{
	//ASSERT(data.IsEmpty());

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return ERROR_OPEN_FILE;
	}

	//�ڴ�ֻ������������

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

		info_insert.m_strFieldID = strID;//���һ��field
		data.Add(info_insert);//���field ��ֻҪfield���ھ�Ҫ��ӣ����û�н��������vector<OneTimeInfo> m_ayTimeInfo == 0

		OneFieldInfo& info = data.GetAt(data.GetSize() - 1);


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

			TimeElement = TimeElement->NextSiblingElement("time");
			info.m_ayTimeInfo.push_back(tmInfo);//������ӵ�˳����xml����д˳������timeindex��vector�е��±겻һ����

		}
		i++;
		field = field->NextSiblingElement("field");

	} 

	myDocument.SaveFile();

	return ERROR_OK;
}

int CFieldDataXML::AddFieldDay(CString strFieldID)
{
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

	DateElement->SetAttribute("date", m_strDate.GetBuffer());

	for (int i = 0; i < m_nTimeCount; i++)
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

	return ERROR_OK;
}