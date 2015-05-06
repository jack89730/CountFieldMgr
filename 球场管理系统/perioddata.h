#ifndef perioddata_h__
#define perioddata_h__
#include "stdafx.h"
#include "tinyxml.h"
#include "public.h"
#include "mysqlcon.h"

//#define GetPeriodData &CTLSingleton<CPeriodDataXML>::Instance()
#define GetPeriodData &CTLSingleton<CPeriodDataMysql>::Instance()

typedef struct _tagPeriodInfo
{
	BOOL m_bInit;
	int m_nStartMin;
	int m_nHalfCount;
	int m_nGoldStart;
	int m_nGoldStop;
	BOOL m_bWeekend;

}PeriodInfo;

class CPeriodData
{
public:
	virtual int Set(const PeriodInfo& info) = 0;
	virtual int Read(PeriodInfo& info) = 0;
	virtual void Save() = 0;
};

class CPeriodDataXML : public CPeriodData
{
public:
	CPeriodDataXML()
	{
		m_strXML = TL_GetModulePath(NULL) + "period.xml";
		m_bInit = FALSE;
	};
	int Set(const PeriodInfo& info);
	int Read(PeriodInfo& info);
	void Save();
private:
	TiXmlDocument myDocument;
	CString m_strXML;
	BOOL m_bInit;
};

class CPeriodDataMysql : public CPeriodData
{
public:
	CPeriodDataMysql()
	{
	};
	int Set(const PeriodInfo& info);
	int Read(PeriodInfo& info);
	void Save();
private:
	CMysqlConn m_mysql;
};

#endif // perioddata_h__
