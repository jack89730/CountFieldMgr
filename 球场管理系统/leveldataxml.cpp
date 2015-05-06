#include "leveldata.h"
#include "public.h"
#include "tinyxml.h"

int CLevelDataXML::AddPrice(OnePriceInfo& info)
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
	TiXmlElement *price = RootElement->FirstChildElement();

	int i=0;

	while(price)
	{
		CString strName = price->Attribute("name");
		CString strID = price->Attribute("id");
		if (strName == info.m_strName || strID  == info.m_strID)
		{
			//��������ͬID
			return ERROR_NAME_EXIST;
		}
		price = price->NextSiblingElement("level");
	} 

	//���һ���½ڵ�
	TiXmlElement *PriceElement = new TiXmlElement("level");
	RootElement->LinkEndChild(PriceElement);
	//����PersonԪ�ص����ԡ�
	PriceElement->SetAttribute("name", info.m_strName.GetBuffer());
	PriceElement->SetAttribute("id", info.m_strID.GetBuffer());

	TiXmlElement *LowElement = new TiXmlElement("price_low");
	TiXmlElement *HighElement = new TiXmlElement("price_high");

	PriceElement->LinkEndChild(LowElement);
	PriceElement->LinkEndChild(HighElement);

	TiXmlText *LowContent = new TiXmlText(info.m_strPriceLow.GetBuffer());
	TiXmlText *HighContent = new TiXmlText(info.m_strPriceHigh.GetBuffer());

	LowElement->LinkEndChild(LowContent);
	HighElement->LinkEndChild(HighContent);

	//myDocument.SaveFile();
	return ERROR_OK;
}

int CLevelDataXML::DeletePrice(CString strID)
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
	//��õ�һ��price�ڵ㡣
	TiXmlElement *price = RootElement->FirstChildElement();

	int i=0;
	while(price)
	{
		CString strName = price->Attribute("id");
		if (strName == strID)
		{
			//�ҵ���
			break;
		}

		price = price->NextSiblingElement("level");

	} 
	if (price)
	{
		//�ҵ���
		TiXmlNode *pParNode =  price->Parent();
		if (NULL == pParNode)
		{
			return ERROR_UNKNOW_ERROR;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL != pParentEle)
		{
			pParentEle->RemoveChild(price);
			//myDocument.SaveFile();
			return ERROR_OK;
		}
	}
	return ERROR_UNKNOW_ERROR;
}

int CLevelDataXML::ChangePriceInfo(OnePriceInfo& info)
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
	//��õ�һ��price�ڵ㡣
	TiXmlElement *price = RootElement->FirstChildElement();

	int i=0;
	while(price)
	{
		CString strId = price->Attribute("id");
		if (strId == info.m_strID)
		{
			break;
		}
		price = price->NextSiblingElement("level");
	} 

	if (price)
	{
		//�ҵ��ˣ���������
		TiXmlElement *LowElement = price->FirstChildElement("price_low");
		TiXmlElement *HighElement = LowElement->NextSiblingElement("price_high");
		price->SetAttribute("name", info.m_strName);

		LowElement->FirstChild()->SetValue(info.m_strPriceLow);
		HighElement->FirstChild()->SetValue(info.m_strPriceHigh);

		//myDocument.SaveFile();
		return ERROR_OK;
	}
	return ERROR_UNKNOW_ERROR;
}

int comp(const void *p1,const void *p2)
{
	OnePriceInfo* pL = (OnePriceInfo*)p1;
	OnePriceInfo* pR = (OnePriceInfo*)p2;
	return atoi(pL->m_strID) - atoi(pR->m_strID);
}

void bubble_sort(PRICE_LIST& List)//nΪ����a��Ԫ�ظ���
{
	int n = List.GetSize();
	int i,j;
	OnePriceInfo temp;
	for(j=0;j<n-1;j++)
		for(i=0;i<n-1-j;i++)
		{
			if(atoi(List[i].m_strID)>atoi(List[i+1].m_strID))//����Ԫ�ش�С����������
			{
				temp=List[i];
				List[i]=List[i+1];
				List[i+1]=temp;
			}
		}
}

int CLevelDataXML::GetAllData(PRICE_LIST& data)
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
	//��õ�һ��price�ڵ㡣
	TiXmlElement *price = RootElement->FirstChildElement();

	int i=0;
	while(price)
	{
		OnePriceInfo info;
		info.m_strName = price->Attribute("name");
		info.m_strID = price->Attribute("id");
		TiXmlElement *LowElement = price->FirstChildElement("price_low");
		TiXmlElement *HighElement = LowElement->NextSiblingElement("price_high");

		info.m_strPriceLow = LowElement->FirstChild()->Value();
		info.m_strPriceHigh = HighElement->FirstChild()->Value();

		data.Add(info);
		price = price->NextSiblingElement("level");
	} 
	//qsort(void *base,int nelem,int width,int (*fcmp)(const void *,const void *));
	/*qsort(&data, data.GetSize(), sizeof(OnePriceInfo), comp);*/
	bubble_sort(data);
	return ERROR_OK;
}

void CLevelDataXML::Save()
{
	myDocument.SaveFile();
}