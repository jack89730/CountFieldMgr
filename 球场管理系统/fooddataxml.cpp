#include "fooddata.h"
#include "tinyxml.h"
#include "public.h"

int CFoodDataXML::AddFood(FoodInfo& info)
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
	//��õ�һ���ڵ㡣
	TiXmlElement *food = RootElement->FirstChildElement();

	int i=0;

	while(food)
	{
		CString strID = food->Attribute("id");
		if (strID == info.m_strID)
		{
			//��������ͬID
			return ERROR_NAME_EXIST;
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
	TiXmlElement *RemainElement = new TiXmlElement("remain");

	FoodElement->LinkEndChild(NameElement);
	FoodElement->LinkEndChild(PriceElement);
	FoodElement->LinkEndChild(RemainElement);

	//����nameԪ�غ�priceԪ�ص����ݲ����ӡ�
	TiXmlText *NameContent = new TiXmlText(info.m_strName.GetBuffer());
	TiXmlText *PriceContent = new TiXmlText(info.m_strPrice.GetBuffer());
	TiXmlText *RemainContent = new TiXmlText(info.m_strRemain.GetBuffer());
	NameElement->LinkEndChild(NameContent);
	PriceElement->LinkEndChild(PriceContent);
	RemainElement->LinkEndChild(RemainContent);

	//myDocument.SaveFile();

	return ERROR_OK;
}

int CFoodDataXML::DeleteFood(CString strID)
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
				//myDocument.SaveFile();
			}
		}
	}
	return ERROR_OK;
}

int CFoodDataXML::ChangeFoodInfo(FoodInfo& info)
{
	//����һ��XML���ĵ�����
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
		TiXmlElement *RemainElement = NameElement->NextSiblingElement("remain");

		NameElement->FirstChild()->SetValue(info.m_strName);
		PriceElement->FirstChild()->SetValue(info.m_strPrice);
		RemainElement->FirstChild()->SetValue(info.m_strRemain);

		//myDocument.SaveFile();
	}
	return ERROR_OK;
}

int CFoodDataXML::GetAllData(FOOD_LIST& data)
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
	//��õ�һ���ڵ㡣
	TiXmlElement *food = RootElement->FirstChildElement();

	while(food)
	{
		CString strID = food->Attribute("id");

		TiXmlElement *NameElement = food->FirstChildElement("name");
		TiXmlElement *PriceElement = NameElement->NextSiblingElement("price");
		TiXmlElement *RemainElement = NameElement->NextSiblingElement("remain");

		CString strName = NameElement->FirstChild()->Value();
		CString strPrice = PriceElement->FirstChild()->Value();
		CString strRemain = RemainElement->FirstChild()->Value();

		FoodInfo tmp;

		tmp.m_strID = strID;
		tmp.m_strName = strName;
		tmp.m_strPrice = strPrice;
		tmp.m_strRemain = strRemain;

		data.Add(tmp);
		food = food->NextSiblingElement("food");
	} 
	return ERROR_OK;
}

void CFoodDataXML::Save()
{
	myDocument.SaveFile();
}