#include "fooddata.h"
#include "tinyxml.h"
#include "public.h"

int CFoodDataXML::AddFood(FoodInfo& info)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//打不开文件就新建一个空文件
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();
		return ERROR_OPEN_FILE;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个节点。
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;

	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == info.m_strID)
		{
			//不能有相同ID
			return ERROR_ID_EXIST;
		}
		food = food->NextSiblingElement("food");
	}

	//添加一个新节点
	TiXmlElement *FoodElement = new TiXmlElement("food");
	RootElement->LinkEndChild(FoodElement);
	//设置Person元素的属性。
	FoodElement->SetAttribute("id", info.m_strID.GetBuffer());
	//创建name元素、sex元素并连接。
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *PriceElement = new TiXmlElement("price");

	FoodElement->LinkEndChild(NameElement);
	FoodElement->LinkEndChild(PriceElement);

	//设置name元素和price元素的内容并连接。
	TiXmlText *NameContent = new TiXmlText(info.m_strName.GetBuffer());
	TiXmlText *PriceContent = new TiXmlText(info.m_strPrice.GetBuffer());
	NameElement->LinkEndChild(NameContent);
	PriceElement->LinkEndChild(PriceContent);

	myDocument.SaveFile();

	return ERROR_OK;
}

int CFoodDataXML::DeleteFood(CString strID)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return ERROR_OPEN_FILE;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个food节点。
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;
	while(food)
	{
		CString strIDFind = food->Attribute("id");
		if (strIDFind == strID)
		{
			//找到了
			break;
		}

		food = food->NextSiblingElement("food");
	} 
	if (food)
	{
		//找到了
		TiXmlNode *pParNode =  food->Parent();
		if (NULL == pParNode)
		{
			return ERROR_UNKNOW_ERROR;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL != pParentEle)
		{
			if(pParentEle->RemoveChild(food))
			{
				myDocument.SaveFile();
			}
		}
	}
	return ERROR_OK;
}

int CFoodDataXML::ChangeFoodInfo(FoodInfo& info)
{
	//创建一个XML的文档对象。
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return ERROR_OPEN_FILE;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;
	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == info.m_strID)
		{
			break;
		}
		food = food->NextSiblingElement("food");
	} 

	if (food)
	{
		//找到了，更新数据
		TiXmlElement *NameElement = food->FirstChildElement("name");
		TiXmlElement *PriceElement = NameElement->NextSiblingElement("price");

		NameElement->FirstChild()->SetValue(info.m_strName);
		PriceElement->FirstChild()->SetValue(info.m_strPrice);

		myDocument.SaveFile();
	}
	return ERROR_OK;
}

int CFoodDataXML::GetAllData(FOOD_LIST& data)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return ERROR_OPEN_FILE;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个节点。
	TiXmlElement *food = RootElement->FirstChildElement();

	while(food)
	{
		CString strID = food->Attribute("id");

		TiXmlElement *NameElement = food->FirstChildElement("name");
		TiXmlElement *PriceElement = NameElement->NextSiblingElement("price");

		CString strName = NameElement->FirstChild()->Value();
		CString strPrice = PriceElement->FirstChild()->Value();

		FoodInfo tmp;

		tmp.m_strID = strID;
		tmp.m_strName = strName;
		tmp.m_strPrice = strPrice;

		data.Add(tmp);
		food = food->NextSiblingElement("food");
	} 
	return ERROR_OK;
}
