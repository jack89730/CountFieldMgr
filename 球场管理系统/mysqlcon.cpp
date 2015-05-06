#include "mysqlcon.h"
#include "tinyxml.h"
#include "public.h"
BOOL CMysqlConn::IsConnect()
{
	return m_bInit;
}

int CMysqlConn::Connect()
{
	if (!m_bInit)
	{
		mysql_init(&m_Conn);
		{
			TiXmlDocument myDocument;
			if (!myDocument.LoadFile(m_strConfigPath.GetBuffer()))
			{
				return ERROR_OPEN_FILE;
			}
			//��ø�Ԫ��
			TiXmlElement *RootElement = myDocument.RootElement();
			//��õ�һ���ڵ㡣
			TiXmlElement *pHost = RootElement->FirstChildElement("host");
			TiXmlElement *pPort = RootElement->FirstChildElement("port");
			TiXmlElement *pUser = RootElement->FirstChildElement("user");
			TiXmlElement *pPwd = RootElement->FirstChildElement("password");
			TiXmlElement *pDB = RootElement->FirstChildElement("database");

			m_strHost = pHost->FirstChild()->Value();
			m_strPort = pPort->FirstChild()->Value();
			m_strUser = pUser->FirstChild()->Value();
			m_strPwd = pPwd->FirstChild()->Value();
			m_strDBName = pDB->FirstChild()->Value();
		}
		

		if(mysql_real_connect(&m_Conn,m_strHost,m_strUser,m_strPwd,m_strDBName,atoi(m_strPort.GetBuffer()),NULL,0))
		{
			mysql_query(&m_Conn, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
			m_bInit = TRUE;
			return ERROR_OK;
		}
		else
		{
			MessageBox(NULL,_T("���ݿ�����ʧ�ܣ���ʹ�����ݿ��������޸����ã�"), _T("��ʾ"), MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			exit(1);
		}
	}

	return  ERROR_UNKNOW_ERROR;
}