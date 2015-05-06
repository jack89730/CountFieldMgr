#ifndef vipdata_h__
#define vipdata_h__

#include "stdafx.h"
#include <vector>
#include "singleton.h"
#include "public.h"
#include "tinyxml.h"
#include "mysqlcon.h"

//#define GetVipData &CTLSingleton<CVipDataXML>::Instance()
#define GetVipData &CTLSingleton<CVipDataMysql>::Instance()

typedef struct _tagVipInfo
{
	CString m_strID;//����ʱӦ���Զ����ɸ��ֶα����ԡ�
	CString m_strName;
	CString m_strSex;
	CString m_strPhone;
	CString m_strMoney;
	CString m_strLevelID;
	BOOL	m_bLock;
	_tagVipInfo()
	{
		m_bLock = FALSE;
		m_strSex = "��";
		m_strMoney = "0";
	}

}VipInfo;

//���г��ص���������
typedef CArray<VipInfo> VIP_LIST;

class CVipData
{
public:
	virtual int AddVip(VipInfo& info) = 0;
	virtual int DeleteVip(CString strFieldID) = 0;
	virtual int ChangeVipInfo(VipInfo& info) = 0;
	virtual int GetAllData(VIP_LIST& data, VIP_LIST& del) = 0;
	virtual void Save() = 0;
};

class CVipDataXML : public CVipData
{
public:
	CVipDataXML():CVipData()
	{
		m_strXML = TL_GetModulePath(NULL) + "vip.xml";
		m_bInit = FALSE;
	}
	int AddVip(VipInfo& info);
	int DeleteVip(CString strVipID);
	int ChangeVipInfo(VipInfo& info);
	int GetAllData(VIP_LIST& data, VIP_LIST& del);
	void Save();
private:
	CString m_strXML;
	TiXmlDocument myDocument;
	BOOL m_bInit;
};


class CVipDataMysql : public CVipData
{
public:
	CVipDataMysql():CVipData()
	{
	}
	int AddVip(VipInfo& info);
	int DeleteVip(CString strVipID);
	int ChangeVipInfo(VipInfo& info);
	int GetAllData(VIP_LIST& data, VIP_LIST& del);
	void Save();
	CMysqlConn m_mysql;
};

#endif // vipdata_h__
