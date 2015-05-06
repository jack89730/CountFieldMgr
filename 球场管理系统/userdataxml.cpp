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

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ���ڵ㡣
	TiXmlElement *user = RootElement->FirstChildElement();

	int i = 0;

	while(user)
	{
		CString strName = user->Attribute("name");
		if (strName == info.m_strName)
		{
			//��������ͬ����
			return ERROR_NAME_EXIST;
		}
		user = user->NextSiblingElement("user");
	}

	//���һ���½ڵ�
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

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
	TiXmlElement *user = RootElement->FirstChildElement();

	int i=0;
	while(user)
	{
		CString strIDFind = user->Attribute("name");
		if (strIDFind == strName)
		{
			//�ҵ���
			break;
		}

		user = user->NextSiblingElement("user");

	} 
	if (user)
	{
		//�ҵ���
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
	//��õ�һ��user�ڵ㡣
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
		//�ҵ��ˣ���������
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

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
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
