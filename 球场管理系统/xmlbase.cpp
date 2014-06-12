#include "xmlbase.h"
#include "tinyxml.h"

BOOL EditFieldStatus(CString strPath, CString strFieldID, CString strVipID, int nTimeIndex, CString strValue)
{
	TiXmlDocument myDocument(strPath.GetBuffer());
	if (!myDocument.LoadFile())
	{
		return FALSE;
	}

	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument.RootElement();
	//��õ�һ��vip�ڵ㡣
	TiXmlElement *field = RootElement->FirstChildElement();

	int i = 0;//��


	while(field)
	{
		int j = 0;//��

		//�ҵ�field���
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
		//û�ҵ����ش�
		return FALSE;
	}

	//�ҵ�ʱ���±�
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