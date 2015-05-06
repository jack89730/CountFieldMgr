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
		ASSERT(FALSE && "还没初始化不能添加场地");
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

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个节点。
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

	//添加一个新节点
	TiXmlElement *FieldElement = new TiXmlElement("field");
	RootElement->LinkEndChild(FieldElement);
	FieldElement->SetAttribute("id", strID.GetBuffer());
	FieldElement->SetAttribute("name", strFieldName.GetBuffer());

	//添加日期
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

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个field节点。
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

	//找到了，更新数据
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

	//计价方式，自动就是取当前系统设定，手动就是预订时填写的价格
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
	

	//内存只保留当天数据

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *field = RootElement->FirstChildElement();

	int i = 0;//行

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

		info_insert.m_strFieldID = strID;//添加一个field
		info_insert.m_strFieldName = strName;

		data.Add(info_insert);//添加field ，只要field存在就要添加，如果没有今天的数据vector<OneTimeBookInfo> m_ayTimeInfo == 0

		OneFieldBookInfo& info = data.GetAt(data.GetSize() - 1);


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
			//不是BOOL
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
			info.m_ayTimeInfo.push_back(tmInfo);//这里添加的顺序是xml中书写顺序，所以timeindex和vector中的下标不一样。

		}
		i++;
		field = field->NextSiblingElement("field");

	} 

	//myDocument.SaveFile();

	int nEnd = GetTickCount();
	TRACE("\nGetAllData耗时：%d\n",nEnd - nBegin);
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