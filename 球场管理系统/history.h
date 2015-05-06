#ifndef viphistory_h__
#define viphistory_h__
#include "stdafx.h"
#include "fastdelegate.h"
#include "mysqlcon.h"

#define GetHistoryData &CTLSingleton<CHisDataMysql>::Instance()

#define HIS_TYPE_BUYFOOD 1
#define HIS_TYPE_FIELD 2
#define HIS_TYPE_CHARGE 3
#define HIS_STR_BUYFOOD "1"
#define HIS_STR_FIELD "2"
#define HIS_STR_CHARGE "3"

typedef struct _tagHisTimeInfo
{
	CString m_strDate;
	CString m_strTime;
}HisTimeInfo;

//1，买东西，2 球场，3 充值
typedef struct _tagHisChargeInfo
{
	HisTimeInfo m_time;
	CString m_strMoney;
	CString m_strVipID;
}HisChargeInfo;

typedef struct _tagHisBuyFoodInfo
{
	HisTimeInfo m_time;
	CString m_strFoodID;//没删除的food用id查询最新的名字
	CString m_strFoodName;//删除了的food，直接显示这个名字
	CString m_strOnePrice;
	CString m_strCount;
	CString m_strVipID;
}HisBuyFoodInfo;

typedef struct _tagHisBuyFieldInfo
{
	HisTimeInfo m_time;
	CString m_strFieldID;
	CString m_strTime;
	CString m_strMoney;
	CString m_strVipID;

}HisBuyFieldInfo;

typedef fastdelegate::FastDelegate2<int, void*, void> HookGetHisData;

class CHisData
{
public:
	virtual int AddCharge(HisChargeInfo& info) = 0;
	virtual int AddBuyFood(HisBuyFoodInfo& info) = 0;
	virtual int AddBuyField(HisBuyFieldInfo& info) = 0;
	virtual void GetAllData(HookGetHisData hookFunc, COleDateTime tStart, COleDateTime tStop) = 0;
};

class TiXmlDocument;
class TiXmlElement;

class CHisDataXML : public CHisData
{
public:
	CHisDataXML(CString strXML):CHisData(){m_strXML = strXML;};
	int AddCharge(HisChargeInfo& info);
	int AddBuyFood(HisBuyFoodInfo& info);
	int AddBuyField(HisBuyFieldInfo& info);
	void GetAllData(HookGetHisData hookFunc, COleDateTime tStart, COleDateTime tStop);
	void CreateXML();
private:
	CString m_strXML;
	int Base(TiXmlDocument& myDocument, TiXmlElement *&pParent, HisTimeInfo& info);
};

class CHisDataMysql : public CHisData
{
public:
	CHisDataMysql():CHisData(){};
	int AddCharge(HisChargeInfo& info);
	int AddBuyFood(HisBuyFoodInfo& info);
	int AddBuyField(HisBuyFieldInfo& info);
	void GetAllData(HookGetHisData hookFunc, COleDateTime tStart, COleDateTime tStop);
private:
	CMysqlConn m_mysql;
};


#endif // viphistory_h__
