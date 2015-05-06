#include "perioddata.h"
#include "public.h"
#include "tinyxml.h"

int CPeriodDataXML::Set(const PeriodInfo& info)
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
	TiXmlElement *pInit = RootElement->FirstChildElement("init");
	CString strTmp;
	strTmp.Format("%d", info.m_bInit);
	pInit->FirstChild()->SetValue(strTmp);

	TiXmlElement *pStartMin = RootElement->FirstChildElement("startmin");
	strTmp.Format("%d", info.m_nStartMin);
	pStartMin->FirstChild()->SetValue(strTmp);

	TiXmlElement *pHalfCount = RootElement->FirstChildElement("halfcount");
	strTmp.Format("%d", info.m_nHalfCount);
	pHalfCount->FirstChild()->SetValue(strTmp);

	TiXmlElement *pGoldStart = RootElement->FirstChildElement("goldstart");
	strTmp.Format("%d", info.m_nGoldStart);
	pGoldStart->FirstChild()->SetValue(strTmp);

	TiXmlElement *pGoldStop = RootElement->FirstChildElement("goldstop");
	strTmp.Format("%d", info.m_nGoldStop);
	pGoldStop->FirstChild()->SetValue(strTmp);

	TiXmlElement *pWeekend = RootElement->FirstChildElement("weekend");
	strTmp.Format("%d", info.m_bWeekend);
	pWeekend->FirstChild()->SetValue(strTmp);

	//myDocument.SaveFile();
	return ERROR_OK;
}

int CPeriodDataXML::Read(PeriodInfo& info)
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
	TiXmlElement *pInit = RootElement->FirstChildElement("init");
	TiXmlElement *pStartMin = RootElement->FirstChildElement("startmin");
	TiXmlElement *pHalfCount = RootElement->FirstChildElement("halfcount");
	TiXmlElement *pGoldStart = RootElement->FirstChildElement("goldstart");
	TiXmlElement *pGoldStop = RootElement->FirstChildElement("goldstop");
	TiXmlElement *pWeekend = RootElement->FirstChildElement("weekend");

	info.m_bInit = atoi(pInit->FirstChild()->Value());
	info.m_nStartMin = atoi(pStartMin->FirstChild()->Value());
	info.m_nHalfCount = atoi(pHalfCount->FirstChild()->Value());
	info.m_nGoldStart =  atoi(pGoldStart->FirstChild()->Value());
	info.m_nGoldStop = atoi(pGoldStop->FirstChild()->Value());
	info.m_bWeekend =  atoi(pWeekend->FirstChild()->Value());

	return ERROR_OK;
}

void CPeriodDataXML::Save()
{
	myDocument.SaveFile();
}
