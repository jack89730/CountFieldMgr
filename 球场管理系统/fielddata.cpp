#include "fielddata.h"
#include "tinyxml.h"
#include "public.h"
int CFieldDataXML::AddField(CString strFieldID)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//打不开文件就新建一个空文件
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();
		return  ERROR_OPEN_FILE;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个节点。
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

	//添加一个新节点
	TiXmlElement *FieldElement = new TiXmlElement("field");
	RootElement->LinkEndChild(FieldElement);
	FieldElement->SetAttribute("id", strFieldID.GetBuffer());

	//添加日期
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

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个field节点。
	TiXmlElement *field = RootElement->FirstChildElement();

	int i=0;
	while(field)
	{
		CString strIDFind = field->Attribute("id");
		if (strIDFind == strFieldID)
		{
			//找到了
			break;
		}

		field = field->NextSiblingElement("field");
	} 
	if (field)
	{
		//找到了
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
	//创建一个XML的文档对象。
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return -1;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
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


	//查找日期
	TiXmlElement *DateElement = field->FirstChildElement("date");
	while (DateElement != NULL && DateElement->Attribute("date") != m_strDate)
	{
		DateElement = DateElement->NextSiblingElement("date");
	}

	ASSERT(DateElement);

	//找到了，更新数据
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

	//内存只保留当天数据

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *field = RootElement->FirstChildElement();

	int i = 0;//行

	OneTimeInfo tmInfo;

	while(field)
	{
		OneFieldInfo info_insert;
		CString strID = field->Attribute("id");

		info_insert.m_strFieldID = strID;//添加一个field
		data.Add(info_insert);//添加field ，只要field存在就要添加，如果没有今天的数据vector<OneTimeInfo> m_ayTimeInfo == 0

		OneFieldInfo& info = data.GetAt(data.GetSize() - 1);


		//查找日期
		TiXmlElement *DateElement = field->FirstChildElement("date");
		while (DateElement != NULL && DateElement->Attribute("date") != m_strDate)
		{
			DateElement = DateElement->NextSiblingElement("date");
		}

		if (DateElement == NULL)
		{
			//没读到日期的数据
			field = field->NextSiblingElement("field");
			i++;
			continue;
		}
		//找到了日期

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
			info.m_ayTimeInfo.push_back(tmInfo);//这里添加的顺序是xml中书写顺序，所以timeindex和vector中的下标不一样。

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
	//打不开文件就新建一个空文件
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();//还没实现
		return -1;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个节点。
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

	ASSERT(field);//应该能找到才对
	TiXmlElement *FieldElement = field;
	if (field == NULL)
	{
		//添加一个新节点
		FieldElement = new TiXmlElement("field");
		RootElement->LinkEndChild(FieldElement);
		//设置Person元素的属性。
		FieldElement->SetAttribute("id", strFieldID.GetBuffer());
	}

	//添加日期
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