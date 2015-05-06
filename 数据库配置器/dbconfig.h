#ifndef perioddata_h__
#define perioddata_h__
#include "stdafx.h"
#include "tinyxml.h"
#include "public.h"
#include "singleton.h"

#define GetDataBaseData &CTLSingleton<CDBConfigXML>::Instance()

typedef struct _tagDatabaseInfo
{
	CString m_strHost;
	int m_nPort;
	CString m_strUserName;
	CString m_strPwd;
	CString m_strDBName;
	_tagDatabaseInfo()
	{
		m_strHost = "localhost";
		m_nPort = 3306;
	}

}DatabaseInfo;

class CDBConfig
{
public:
	virtual int Set(const DatabaseInfo& info) = 0;
	virtual int Read(DatabaseInfo& info) = 0;
	virtual void Save() = 0;
};

class CDBConfigXML : public CDBConfig
{
public:
	CDBConfigXML()
	{
		m_strXML = TL_GetModulePath(NULL) + "database.cfg";
		m_bInit = FALSE;
	};
	int Set(const DatabaseInfo& info);
	int Read(DatabaseInfo& info);
	void Save();
private:
	TiXmlDocument myDocument;
	CString m_strXML;
	BOOL m_bInit;
};


#endif // perioddata_h__
