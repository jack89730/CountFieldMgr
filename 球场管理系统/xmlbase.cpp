#include "xmlbase.h"
#include "tinyxml.h"

BOOL EditFieldStatus(CString strPath, CString strFieldID, CString strVipID, int nTimeIndex, CString strValue)
{
	TiXmlDocument myDocument(strPath.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return FALSE;
	}

	//获得根元素
	TiXmlElement *RootElement = myDocument.RootElement();
	//获得第一个vip节点。
	TiXmlElement *field = RootElement->FirstChildElement();

	int i = 0;//行


	while(field)
	{
		int j = 0;//列

		//找到field编号
		CString strID = field->Attribute("id");
		if (strID != strFieldID)
		{
			field = field->NextSiblingElement("field");
			continue;
		}
		break;
	}
	if (!field)
	{
		//没找到返回错
		return FALSE;
	}

	//找到时间下标
	TiXmlElement *TimeElement = field->FirstChildElement("time");

	while (TimeElement)
	{
		CString strIndex = TimeElement->Attribute("index");
		if (atoi(strIndex.GetBuffer()) != nTimeIndex)
		{
			TimeElement = TimeElement->NextSiblingElement("time");
			continue;
		}
		TimeElement->FirstChild()->SetValue(strValue);
		TimeElement->SetAttribute("booker", strVipID);
		myDocument.SaveFile();
		return TRUE;
	}

	return FALSE;
}