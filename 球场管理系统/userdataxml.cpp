#include "userdata.h"
#include "stdafx.h"
#include <vector>
#include "singleton.h"
#include "public.h"
#include "tinyxml.h"

int CUserDataXML::Add(UserInfo& info)
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
	//获得第一个节点。
	TiXmlElement *user = RootElement->FirstChildElement();

	int i = 0;

	while(user)
	{
		CString strName = user->Attribute("name");
		if (strName == info.m_strName)
		{
			//不能有相同名字
			return ERROR_NAME_EXIST;
		}
		user = user->NextSiblingElement("user");
	}

	//添加一个新节点
	TiXmlElement *UserElement = new TiXmlElement("user");
	RootElement->LinkEndChild(UserElement);
	UserElement->SetAttribute("name", info.m_strName);
	UserElement->SetAttribute("pwd", info.m_strPwd);
	UserElement->SetAttribute("level", info.m_strLevel);

	//myDocument.SaveFile();
	return ERROR_OK;
}

int CUserDataXML::Delete(CString strName)
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
	TiXmlElement *user = RootElement->FirstChildElement();

	int i=0;
	while(user)
	{
		CString strIDFind = user->Attribute("name");
		if (strIDFind == strName)
		{
			//找到了
			break;
		}

		user = user->NextSiblingElement("user");

	} 
	if (user)
	{
		//找到了
		TiXmlNode *pParNode =  user->Parent();
		if (NULL == pParNode)
		{
			return ERROR_UNKNOW_ERROR;
		}

		TiXmlElement* pParentEle = pParNode->ToElement();
		if (NULL != pParentEle)
		{
			pParentEle->RemoveChild(user);
			//myDocument.SaveFile();
			return ERROR_OK;
		}
	}
	return ERROR_UNKNOW_ERROR;
}

int CUserDataXML::Change( UserInfo& info )
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
	//获得第一个user节点。
	TiXmlElement *user = RootElement->FirstChildElement();

	int i=0;
	while(user)
	{
		CString strName = user->Attribute("name");
		if (strName == info.m_strName)
		{
			break;
		}
		user = user->NextSiblingElement("user");
	} 

	if (user)
	{
		//找到了，更新数据
		user->SetAttribute("pwd", info.m_strPwd);
		user->SetAttribute("level", info.m_strLevel);
		//myDocument.SaveFile();
		return ERROR_OK;
	}
	return ERROR_UNKNOW_ERROR;
}

int CUserDataXML::GetAllData( USER_LIST& data )
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
	TiXmlElement *user = RootElement->FirstChildElement();

	int i=0;
	while(user)
	{
		CString strName = user->Attribute("name");
		CString strPwd = user->Attribute("pwd");
		CString strLevel = user->Attribute("level");

		UserInfo info;

		info.m_strName = strName;
		info.m_strPwd = strPwd;
		info.m_strLevel = strLevel;

		data.Add(info);
		user = user->NextSiblingElement("user");
	} 
	return ERROR_OK;
}

void CUserDataXML::Save()
{
	if (m_bInit)
	{
		myDocument.SaveFile();
	}
}
