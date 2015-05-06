#include "dbconfig.h"
#include "public.h"
#include "tinyxml.h"

int CDBConfigXML::Set(const DatabaseInfo& info)
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
	TiXmlElement *pHost = RootElement->FirstChildElement("host");
	pHost->FirstChild()->SetValue(info.m_strHost);

	TiXmlElement *pPort = RootElement->FirstChildElement("port");
	CString strTmp;
	strTmp.Format("%d", info.m_nPort);
	pPort->FirstChild()->SetValue(strTmp);

	TiXmlElement *pUser = RootElement->FirstChildElement("user");
	pUser->FirstChild()->SetValue(info.m_strUserName);

	TiXmlElement *pPwd = RootElement->FirstChildElement("password");
	pPwd->FirstChild()->SetValue(info.m_strPwd);

	TiXmlElement *pDBName = RootElement->FirstChildElement("database");
	pDBName->FirstChild()->SetValue(info.m_strDBName);

	myDocument.SaveFile();
	return ERROR_OK;
}

int CDBConfigXML::Read(DatabaseInfo& info)
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
	TiXmlElement *pHost = RootElement->FirstChildElement("host");
	info.m_strHost = pHost->FirstChild()->Value();
	TiXmlElement *pPort = RootElement->FirstChildElement("port");
	info.m_nPort = atoi(pPort->FirstChild()->Value());
	TiXmlElement *pUser = RootElement->FirstChildElement("user");
	info.m_strUserName = pUser->FirstChild()->Value();
	TiXmlElement *pPwd = RootElement->FirstChildElement("password");
	info.m_strPwd = pPwd->FirstChild()->Value();
	TiXmlElement *pDBName = RootElement->FirstChildElement("database");
	info.m_strDBName = pDBName->FirstChild()->Value();

	return ERROR_OK;
}

void CDBConfigXML::Save()
{
	myDocument.SaveFile();
}
