#include "fooddata.h"
#include "tinyxml.h"
#include "public.h"

int CFoodDataXML::AddFood(FoodInfo& info)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	//�򲻿��ļ����½�һ�����ļ�
	if (!myDocument.LoadFile())
	{
		//CreateFoodXML();
		return ERROR_OPEN_FILE;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ���ڵ㡣
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;

	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == info.m_strID)
		{
			//��������ͬID
			return ERROR_ID_EXIST;
		}
		food = food->NextSiblingElement("food");
	}

	//���һ���½ڵ�
	TiXmlElement *FoodElement = new TiXmlElement("food");
	RootElement->LinkEndChild(FoodElement);
	//����PersonԪ�ص����ԡ�
	FoodElement->SetAttribute("id", info.m_strID.GetBuffer());
	//����nameԪ�ء�sexԪ�ز����ӡ�
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *PriceElement = new TiXmlElement("price");

	FoodElement->LinkEndChild(NameElement);
	FoodElement->LinkEndChild(PriceElement);

	//����nameԪ�غ�priceԪ�ص����ݲ����ӡ�
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

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��food�ڵ㡣
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;
	while(food)
	{
		CString strIDFind = food->Attribute("id");
		if (strIDFind == strID)
		{
			//�ҵ���
			break;
		}

		food = food->NextSiblingElement("food");
	} 
	if (food)
	{
		//�ҵ���
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
	//����һ��XML���ĵ�����
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return ERROR_OPEN_FILE;
	}

	//��ø�Ԫ��
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
		//�ҵ��ˣ���������
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

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ���ڵ㡣
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
