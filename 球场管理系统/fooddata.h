#ifndef fooddata_h__
#define fooddata_h__

#include "stdafx.h"
#include <vector>
#include "public.h"
#include "tinyxml.h"
#include "mysqlcon.h"

//#define GetFoodData &CTLSingleton<CFoodDataXML>::Instance()
#define GetFoodData &CTLSingleton<CFoodDataMysql>::Instance()

typedef struct _tagFoodInfo
{
	CString m_strID;
	CString m_strName;
	CString m_strPrice;
	CString m_strRemain;

}FoodInfo;

typedef CArray<FoodInfo> FOOD_LIST;

class CFoodData
{
public:
	virtual int AddFood(FoodInfo& info) = 0;
	virtual int DeleteFood(CString strID) = 0;
	virtual int ChangeFoodInfo(FoodInfo& info) = 0;
	virtual int GetAllData(FOOD_LIST& data) = 0;
	virtual void Save() = 0;
};

class CFoodDataXML : public CFoodData
{
	public:
		CFoodDataXML()
		{
			m_strXML = TL_GetModulePath(NULL) + "food.xml";
			m_bInit = FALSE;
		}
		int AddFood(FoodInfo& info);
		int DeleteFood(CString strID);
		int ChangeFoodInfo(FoodInfo& info);
		int GetAllData(FOOD_LIST& data);
		void Save();
private:
	TiXmlDocument myDocument;
	CString m_strXML;
	BOOL m_bInit;
};

class CFoodDataMysql : public CFoodData
{
public:
	CFoodDataMysql()
	{
	}
	int AddFood(FoodInfo& info);
	int DeleteFood(CString strID);
	int ChangeFoodInfo(FoodInfo& info);
	int GetAllData(FOOD_LIST& data);
	void Save();
private:
	CMysqlConn m_mysql;
};

#endif // fooddata_h__
