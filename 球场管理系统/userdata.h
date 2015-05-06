#ifndef userdata_h__
#define userdata_h__

#include "stdafx.h"

#include "mysqlcon.h"
#include <vector>
#include "tinyxml.h"
#include "singleton.h"
#include "public.h"


//#define GetUserData &CTLSingleton<CUserDataXML>::Instance()
#define GetUserData &CTLSingleton<CUserDataMysql>::Instance()

#define USER_LEVEL_ADMIN "1"
#define USER_LEVEL_COMMON "2"

typedef struct _tagUserInfo
{
	CString m_strName;
	CString m_strPwd;
	CString m_strLevel;
	_tagUserInfo()
	{
		m_strLevel = USER_LEVEL_COMMON;
	}
}UserInfo;

//所有场地的所有数据
typedef CArray<UserInfo> USER_LIST;

class CUserData
{
public:
	virtual int Add(UserInfo& info) = 0;
	virtual int Delete(CString strName) = 0;
	virtual int Change(UserInfo& info) = 0;
	virtual int GetAllData(USER_LIST& data) = 0;
	virtual void Save() = 0;
};

class CUserDataXML : public CUserData
{
public:
	CUserDataXML():CUserData()
	{
		m_strXML = TL_GetModulePath(NULL) + "user.xml";
		m_bInit = FALSE;
	}
	int Add(UserInfo& info);
	int Delete(CString strName);
	int Change(UserInfo& info);
	int GetAllData(USER_LIST& data);
	void Save();
private:
	CString m_strXML;
	TiXmlDocument myDocument;
	BOOL m_bInit;
};

class CUserDataMysql : public CUserData
{
public:
	CUserDataMysql():CUserData()
	{
	}
	int Add(UserInfo& info);
	int Delete(CString strName);
	int Change(UserInfo& info);
	int GetAllData(USER_LIST& data);
	void Save();
private:
	CMysqlConn m_mysql;
};


#endif // userdata_h__
