#include "history.h"
#include "tinyxml.h"
#include "public.h"

int CHisDataXML::Base(TiXmlDocument& myDocument, TiXmlElement *&pParent, HisTimeInfo& info)
{
	//打不开文件就新建一个空文件
	if (!myDocument.LoadFile())
	{
		CreateXML();
		if (!myDocument.LoadFile())
		{
			return ERROR_OPEN_FILE;
		}
	}

	//到这里保证是至少有根元素

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *date = RootElement->FirstChildElement();
	while(date)
	{
		//找到对应的日期，找不到就为null
		CString strDate = date->Attribute("date");
		if (strDate == info.m_strDate)
		{
			break;
		}
		date = date->NextSiblingElement("date");
	}

	//添加一个date节点
	if (!date)
	{
		date = new TiXmlElement("date");
		RootElement->LinkEndChild(date);
		date->SetAttribute("date", info.m_strDate);
	}

	//添加一个新节点
	pParent = new TiXmlElement("info");
	date->LinkEndChild(pParent);

	//pParent->SetAttribute("type", HIS_TYPE_CHARGE);
	pParent->SetAttribute("time", info.m_strTime);

	return ERROR_OK;
}
int CHisDataXML::AddCharge(HisChargeInfo& info)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	TiXmlElement *pParent = NULL;
	int ret = Base(myDocument, pParent, info.m_time);
	if (ret != ERROR_OK)
	{
		return ret;
	}
	pParent->SetAttribute("type", HIS_TYPE_CHARGE);
	//添加info的子节点信息
	TiXmlElement *MoneyElement = new TiXmlElement("money");
	pParent->LinkEndChild(MoneyElement);
	TiXmlText *MoneyContent = new TiXmlText(info.m_strMoney);
	MoneyElement->LinkEndChild(MoneyContent);

	TiXmlElement *VipElement = new TiXmlElement("vipid");
	pParent->LinkEndChild(VipElement);
	TiXmlText *VipContent = new TiXmlText(info.m_strVipID);
	VipElement->LinkEndChild(VipContent);

	myDocument.SaveFile();
	return ERROR_OK;
}

int CHisDataXML::AddBuyFood(HisBuyFoodInfo& info)
{
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	TiXmlElement *pParent = NULL;
	int ret = Base(myDocument, pParent, info.m_time);
	if (ret != ERROR_OK)
	{
		return ret;
	}

	//<info time="10:52:39" type="1">
	//	<name>可乐</name>
	//	<oneprice>10</oneprice>
	//	<count>2</count>
	//</info>

	pParent->SetAttribute("type", HIS_TYPE_BUYFOOD);

	//添加info的子节点信息
	TiXmlElement *NameElement = new TiXmlElement("name");
	pParent->LinkEndChild(NameElement);
	TiXmlText *NameContent = new TiXmlText(info.m_strFoodName);
	NameElement->LinkEndChild(NameContent);

	TiXmlElement *OnePriceElement = new TiXmlElement("oneprice");
	pParent->LinkEndChild(OnePriceElement);
	TiXmlText *OnePriceContent = new TiXmlText(info.m_strOnePrice);
	OnePriceElement->LinkEndChild(OnePriceContent);

	TiXmlElement *CountElement = new TiXmlElement("count");
	pParent->LinkEndChild(CountElement);
	TiXmlText *CountContent = new TiXmlText(info.m_strCount);
	CountElement->LinkEndChild(CountContent);

	TiXmlElement *VipElement = new TiXmlElement("vipid");
	pParent->LinkEndChild(VipElement);
	TiXmlText *VipContent = new TiXmlText(info.m_strVipID);
	VipElement->LinkEndChild(VipContent);

	myDocument.SaveFile();
	return ERROR_OK;
}

int CHisDataXML::AddBuyField(HisBuyFieldInfo& info)
{

	TiXmlDocument myDocument(m_strXML.GetBuffer());
	TiXmlElement *pParent = NULL;
	int ret = Base(myDocument, pParent, info.m_time);
	if (ret != ERROR_OK)
	{
		return ret;
	}

	/*<info time="10:54:39" type="2">
		<field>101</field>
		<time>9:00-9:30</time>
		<money>100</money>
	</info>*/

	pParent->SetAttribute("type", HIS_TYPE_FIELD);

	//添加info的子节点信息
	TiXmlElement *FieldElement = new TiXmlElement("field");
	pParent->LinkEndChild(FieldElement);
	TiXmlText *FieldContent = new TiXmlText(info.m_strFieldID);
	FieldElement->LinkEndChild(FieldContent);

	TiXmlElement *TimeElement = new TiXmlElement("time");
	pParent->LinkEndChild(TimeElement);
	TiXmlText *TimeContent = new TiXmlText(info.m_strTime);
	TimeElement->LinkEndChild(TimeContent);

	TiXmlElement *MoneyElement = new TiXmlElement("money");
	pParent->LinkEndChild(MoneyElement);
	TiXmlText *MoneyContent = new TiXmlText(info.m_strMoney);
	MoneyElement->LinkEndChild(MoneyContent);

	TiXmlElement *VipElement = new TiXmlElement("vipid");
	pParent->LinkEndChild(VipElement);
	TiXmlText *VipContent = new TiXmlText(info.m_strVipID);
	VipElement->LinkEndChild(VipContent);

	myDocument.SaveFile();
	return ERROR_OK;
}

void CHisDataXML::CreateXML()
{
	TiXmlDocument myDocument(m_strXML);        //创建一个根结点
	TiXmlDeclaration *pDeclaration=new TiXmlDeclaration("1.0","gb2312","yes");   //创建xml文件头
	myDocument.LinkEndChild(pDeclaration);           //加入文件头
	TiXmlElement *root = new TiXmlElement("history");       //创建一个节点
	myDocument.LinkEndChild(root); 
	myDocument.SaveFile();
}

void CHisDataXML::GetAllData(HookGetHisData hookFunc, COleDateTime tStart, COleDateTime tStop)
{
	//打不开文件就新建一个空文件
	TiXmlDocument myDocument(m_strXML.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return;
	}

	//到这里保证是至少有根元素

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *date = RootElement->FirstChildElement();
	while(date)
	{
		//找到对应的日期，找不到就为null
		CString strDate = date->Attribute("date");
		TiXmlElement* pInfo = date->FirstChildElement("info");
		while (pInfo)
		{
			HisChargeInfo charge_info;
			charge_info.m_time.m_strDate = strDate;
			charge_info.m_time.m_strTime = pInfo->Attribute("time");
			HisBuyFieldInfo field_info;
			field_info.m_time.m_strDate = strDate;
			field_info.m_time.m_strTime = pInfo->Attribute("time");
			HisBuyFoodInfo food_info;
			food_info.m_time.m_strDate = strDate;
			food_info.m_time.m_strTime = pInfo->Attribute("time");

			CString strType = pInfo->Attribute("type");
			if (strType == HIS_STR_CHARGE)
			{
				TiXmlElement* pMoney = pInfo->FirstChildElement();
				charge_info.m_strMoney = pMoney->FirstChild()->Value();
				hookFunc(HIS_TYPE_CHARGE, &charge_info);
			}
			else if (strType == HIS_STR_BUYFOOD)
			{
				TiXmlElement* pName = pInfo->FirstChildElement("name");
				food_info.m_strFoodName = pName->FirstChild()->Value();
				TiXmlElement* pOnePrice = pName->NextSiblingElement("oneprice");
				food_info.m_strOnePrice = pOnePrice->FirstChild()->Value();

				TiXmlElement* pCount = pName->NextSiblingElement("count");
				food_info.m_strCount = pCount->FirstChild()->Value();

				hookFunc(HIS_TYPE_BUYFOOD, &food_info);
			}
			else if (strType == HIS_STR_FIELD)
			{
				TiXmlElement* pField = pInfo->FirstChildElement("field");
				field_info.m_strFieldID = pField->FirstChild()->Value();
				TiXmlElement* pTime = pField->NextSiblingElement("time");
				field_info.m_strTime = pTime->FirstChild()->Value();
				TiXmlElement* pMoney = pTime->NextSiblingElement("money");
				field_info.m_strMoney = pMoney->FirstChild()->Value();

				TiXmlElement* pVip = pTime->NextSiblingElement("vipid");
				field_info.m_strVipID = pVip->FirstChild()->Value();

				hookFunc(HIS_TYPE_FIELD, &field_info);
			}

			pInfo = pInfo->NextSiblingElement("info");
		}
		date = date->NextSiblingElement("date");
	}

}