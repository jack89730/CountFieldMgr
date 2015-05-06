#include "vipdata.h"
#include "tinyxml.h"
#include "public.h"

int CVipDataXML::AddVip(VipInfo& info)
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
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;

	while(vip)
	{
		CString strID = vip->Attribute("id");
		if (strID == info.m_strID)
		{
			//不能有相同ID
			return ERROR_NAME_EXIST;
		}
		vip = vip->NextSiblingElement("vip");
	} 

	//添加一个新节点
	TiXmlElement *VipElement = new TiXmlElement("vip");
	RootElement->LinkEndChild(VipElement);
	//设置Person元素的属性。
	VipElement->SetAttribute("id", info.m_strID.GetBuffer());
	//创建name元素、sex元素并连接。
	TiXmlElement *NameElement = new TiXmlElement("name");
	TiXmlElement *SexElement = new TiXmlElement("sex");
	TiXmlElement *PhoneElement = new TiXmlElement("phone");
	TiXmlElement *MoneyElement = new TiXmlElement("money");
	TiXmlElement *LevelElement = new TiXmlElement("level");
	TiXmlElement *LockElement = new TiXmlElement("lock");

	VipElement->LinkEndChild(NameElement);
	VipElement->LinkEndChild(SexElement);
	VipElement->LinkEndChild(PhoneElement);
	VipElement->LinkEndChild(MoneyElement);
	VipElement->LinkEndChild(LevelElement);
	VipElement->LinkEndChild(LockElement);

	//设置name元素和sex元素的内容并连接。
	TiXmlText *NameContent = new TiXmlText(info.m_strName.GetBuffer());
	TiXmlText *SexContent = new TiXmlText(info.m_strSex.GetBuffer());
	TiXmlText *PhoneContent = new TiXmlText(info.m_strPhone.GetBuffer());
	TiXmlText *MoneyContent = new TiXmlText(info.m_strMoney.GetBuffer());
	TiXmlText *LevelContent = new TiXmlText(info.m_strLevelID.GetBuffer());
	CString strLock = "0";
	if (info.m_bLock)
	{
		strLock = "1";
	}
	TiXmlText *LockContent = new TiXmlText(strLock.GetBuffer());

	NameElement->LinkEndChild(NameContent);
	SexElement->LinkEndChild(SexContent);
	PhoneElement->LinkEndChild(PhoneContent);
	MoneyElement->LinkEndChild(MoneyContent);
	LevelElement->LinkEndChild(LevelContent);
	LockElement->LinkEndChild(LockContent);

	//myDocument.SaveFile();
	return ERROR_OK;
}

void CVipDataXML::Save()
{
	if (m_bInit)
	{
		myDocument.SaveFile();
	}
}

int CVipDataXML::DeleteVip(CString strVipID)
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
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;
	while(vip)
	{
		CString strIDFind = vip->Attribute("id");
		if (strIDFind == strVipID)
		{
			//找到了
			break;
		}

		vip = vip->NextSiblingElement("vip");

	} 
	if (vip)
	{
		//找到了
		TiXmlNode *pParNode =  vip->Parent();
		if (NULL == pParNode)
		{
			return ERROR_UNKNOW_ERROR;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL != pParentEle)
		{
			pParentEle->RemoveChild(vip);
			//myDocument.SaveFile();
			return ERROR_OK;
		}
	}
	return ERROR_UNKNOW_ERROR;
}

int CVipDataXML::ChangeVipInfo(VipInfo& info)
{
	//创建一个XML的文档对象。
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
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;
	while(vip)
	{
		CString strID = vip->Attribute("id");
		if (strID == info.m_strID)
		{
			break;
		}
		vip = vip->NextSiblingElement("vip");
	} 

	if (vip)
	{
		//找到了，更新数据
		TiXmlElement *NameElement = vip->FirstChildElement("name");
		TiXmlElement *SexElement = NameElement->NextSiblingElement("sex");
		TiXmlElement *PhoneElement = NameElement->NextSiblingElement("phone");
		TiXmlElement *MoneyElement = NameElement->NextSiblingElement("money");
		TiXmlElement *LevelElement = NameElement->NextSiblingElement("level");
		TiXmlElement *LockElement = NameElement->NextSiblingElement("lock");

		NameElement->FirstChild()->SetValue(info.m_strName);
		SexElement->FirstChild()->SetValue(info.m_strSex);
		PhoneElement->FirstChild()->SetValue(info.m_strPhone);
		MoneyElement->FirstChild()->SetValue(info.m_strMoney);
		LevelElement->FirstChild()->SetValue(info.m_strLevelID);
		CString strLock = "0";
		if (info.m_bLock)
		{
			strLock = "1";
		}
		LockElement->FirstChild()->SetValue(strLock);

		//myDocument.SaveFile();
		return ERROR_OK;
	}
	return ERROR_UNKNOW_ERROR;
}

int CVipDataXML::GetAllData(VIP_LIST& data, VIP_LIST& del)
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
	TiXmlElement *vip = RootElement->FirstChildElement();

	int i=0;
	while(vip)
	{
		CString strID = vip->Attribute("id");
		TiXmlElement *NameElement = vip->FirstChildElement("name");
		TiXmlElement *SexElement = NameElement->NextSiblingElement("sex");
		TiXmlElement *PhoneElement = NameElement->NextSiblingElement("phone");
		TiXmlElement *MoneyElement = NameElement->NextSiblingElement("money");
		TiXmlElement *LevelElement = NameElement->NextSiblingElement("level");
		TiXmlElement *LockElement = NameElement->NextSiblingElement("lock");

		CString strName = NameElement->FirstChild()->Value();
		CString strSex = SexElement->FirstChild()->Value();
		CString strPhone = PhoneElement->FirstChild()->Value();
		CString strMoney = MoneyElement->FirstChild()->Value();
		CString strLevel = LevelElement->FirstChild()->Value();
		CString strLock = LockElement->FirstChild()->Value();

		VipInfo info;

		info.m_strID = strID;
		info.m_strSex = strSex;
		info.m_strPhone = strPhone;
		info.m_strMoney = strMoney;
		info.m_strName = strName;
		info.m_strLevelID = strLevel;
		info.m_bLock = atoi(strLock);

		data.Add(info);
		vip = vip->NextSiblingElement("vip");
	} 
	return ERROR_OK;
}
