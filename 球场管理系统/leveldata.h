#ifndef leveldata_h__
#define leveldata_h__

#include "stdafx.h"
#include <vector>
#include "tinyxml.h"
#include "public.h"
#include "singleton.h"
#include "mysqlcon.h"

//#define GetLevelData &CTLSingleton<CLevelDataXML>::Instance()
#define GetLevelData &CTLSingleton<CLevelDataMysql>::Instance()

//一个时间段的内容
typedef struct _tagOnePriceInfo
{
	CString m_strID;
	CString m_strName;//金
	CString m_strPriceLow;
	CString m_strPriceHigh;

}OnePriceInfo;

typedef CArray<OnePriceInfo> PRICE_LIST;
class CLevelData
{
public:
	virtual int AddPrice(OnePriceInfo& info) = 0;
	virtual int DeletePrice(CString strLevel) = 0;
	virtual int ChangePriceInfo(OnePriceInfo& info) = 0;
	virtual int GetAllData(PRICE_LIST& data) = 0;
	virtual void Save() = 0;
};

class CLevelDataXML : public CLevelData
{
public:
	CLevelDataXML():CLevelData()
	{
		m_strXML = TL_GetModulePath(NULL) + "level.xml";
		m_bInit = FALSE;
	};
	int AddPrice(OnePriceInfo& info);
	int DeletePrice(CString strLevel);
	int ChangePriceInfo(OnePriceInfo& info);
	int GetAllData(PRICE_LIST& data);
	void Save();
private:
	TiXmlDocument myDocument;
	CString m_strXML;
	BOOL m_bInit;
};

class CLevelDataMysql : public CLevelData
{
public:
	CLevelDataMysql():CLevelData()
	{
	};
	int AddPrice(OnePriceInfo& info);
	int DeletePrice(CString strLevel);
	int ChangePriceInfo(OnePriceInfo& info);
	int GetAllData(PRICE_LIST& data);
	void Save();
private:
	CMysqlConn m_mysql;
};

#endif // leveldata_h__
