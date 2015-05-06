#ifndef fielddata_h__
#define fielddata_h__
#include "stdafx.h"
#include <vector>
#include "tinyxml.h"
#include "singleton.h"
#include "public.h"
#include "mysqlcon.h"

//#define GetFieldData &CTLSingleton<CFieldDataXML>::Instance();
#define GetFieldData &CTLSingleton<CFieldDataMysql>::Instance();

//一个时间段的内容
typedef struct _tagOneTimeInfo
{
	CString m_strVipID;
	CString m_strPrice;
	int m_bBusy;
	int m_nTimeIndex;
	BOOL m_bAutoPrice;
	CString m_strBoxKey;
	_tagOneTimeInfo()
	{
		m_strPrice = "0";
		m_bAutoPrice = FALSE;
	}
}OneTimeBookInfo;

//一个场地所有时的数据
typedef struct _tagOneFieldBookInfo
{
	CString m_strFieldID;
	CString m_strFieldName;
	std::vector<OneTimeBookInfo> m_ayTimeInfo;

}OneFieldBookInfo;

typedef struct _tagFieldInfo
{
	CString m_strFieldID;
	CString m_strFieldName;
}FieldInfo;
//所有场地的所有数据
typedef CArray<OneFieldBookInfo> FIELD_LIST;

class CFieldData
{
public:
	CFieldData(){};
	virtual int AddField(CString strFieldID) = 0;
	virtual int DeleteField(CString strFieldID) = 0;
	virtual int ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info) = 0;
	virtual int ChangeFieldInfo(FieldInfo& info) = 0;
	virtual int GetAllData(FIELD_LIST& data) = 0;
	virtual int AddFieldDay(CString strFieldID) = 0;
	virtual void Save() = 0;

	void SetCurrentDate(CString strDate){m_strDate = strDate;};
	CString GetCurrentDate(){return m_strDate;};

protected:
	CString m_strDate;
};

class CFieldDataXML : public CFieldData
{
public:
	CFieldDataXML():CFieldData()
	{
		m_strXML = TL_GetModulePath(NULL) + "field.xml";
		m_bInit = FALSE;
		m_nMaxId = 0;
	}
	int AddField(CString strFieldName);
	int DeleteField(CString strFieldID);
	int ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info);
	int ChangeFieldInfo(FieldInfo& info);
	int GetAllData(FIELD_LIST& data);
	int AddFieldDay(CString strFieldID);
	void Save();
private:
	TiXmlDocument myDocument;
	CString m_strXML;
	BOOL m_bInit;
	int m_nMaxId;
};


class CFieldDataMysql : public CFieldData
{
public:
	CFieldDataMysql()
	{
	}
	int AddField(CString strFieldName);
	int DeleteField(CString strFieldID);
	int ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info);
	int ChangeFieldInfo(FieldInfo& info);
	int GetAllData(FIELD_LIST& data);
	int AddFieldDay(CString strFieldID);
	void Save();
private:
	CMysqlConn m_mysql;
};

#endif // fielddata_h__


