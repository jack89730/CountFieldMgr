#include "history.h"

//create table history
//(
// id int unsigned auto_increment primary key,
// typeid tinyint not null,
// vipid int not null
// fieldid int  null,
// timestr char(20)  null,
// foodname char(40)  null,
// onemoney float not null,
// count int unsigned not null,
// addtime timestamp default current_timestamp
// );

int CHisDataMysql::AddCharge(HisChargeInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("insert into history values(null, 3, %s, 0, 0, 0, %s, 1, null)"
		,info.m_strVipID.GetBuffer()
		,info.m_strMoney.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CHisDataMysql::AddBuyFood(HisBuyFoodInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	if (info.m_strVipID.IsEmpty())
	{
		info.m_strVipID = "null";
	}
	CString strSQL;
	strSQL.Format("insert into history values(null, 1, %s, 0, 0, '%s', %s, %s, null)"
		,info.m_strVipID.GetBuffer()
		,info.m_strFoodName.GetBuffer()
		,info.m_strOnePrice.GetBuffer()
		,info.m_strCount.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CHisDataMysql::AddBuyField(HisBuyFieldInfo& info)
{

	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("insert into history values(null, 2, %s, %s, '%s', 0, %s, 1, null)"
		,info.m_strVipID.GetBuffer()
		,info.m_strFieldID.GetBuffer()
		,info.m_strTime.GetBuffer()
		,info.m_strMoney.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

void CHisDataMysql::GetAllData(HookGetHisData hookFunc, COleDateTime tStart, COleDateTime tStop)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return;
		}
	}

	CString strStart;
	CString strStop;
	strStart.Format("%04d-%02d-%02d %02d:%02d:%02d", tStart.GetYear(), tStart.GetMonth()
		, tStart.GetDay(), 0, 0, 0);

	strStop.Format("%04d-%02d-%02d %02d:%02d:%02d", tStop.GetYear(), tStop.GetMonth()
		, tStop.GetDay(), 59, 59, 59);

	CString strSQL;
	strSQL.Format("select * from history where addtime >= '%s' and addtime <= '%s' order by id"
		, strStart.GetBuffer()
		, strStop.GetBuffer());
	int res = mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return;
	}
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	result = mysql_store_result(m_mysql.GetConnect());//保存查询到的数据到result
	if (result == NULL)
	{
		return;
	}
	int nFields = mysql_num_fields(result);
	ASSERT(nFields == 9);
	while(sql_row = mysql_fetch_row(result))//获取具体的数据
	{
		int nType = atoi(sql_row[1]);
		if (nType == 1)//买东西
		{
			HisBuyFoodInfo info;
			info.m_strVipID = sql_row[2];
			info.m_strFoodName = sql_row[5];
			info.m_strOnePrice = sql_row[6];
			info.m_strCount = sql_row[7];
			CString strTime = sql_row[8];
			int nYear = 0;
			int nMonth = 0;
			int nDay = 0;
			int nHour = 0;
			int nMin = 0;
			int nSec = 0;
			sscanf(strTime.GetBuffer(), "%04d-%02d-%02d %02d:%02d:%02d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);		
			info.m_time.m_strDate.Format("%04d%02d%02d", nYear,nMonth,nDay);
			info.m_time.m_strTime.Format("%02d:%02d:%02d", nHour, nMin, nSec);
			hookFunc(1, &info);
		}
		else if (nType == 2)
		{
			//'1', 'zhangpeng', '男', '13693646751', '100', '0', '0', '1', '2014-07-08 17:04:28

			HisBuyFieldInfo info;

			info.m_strVipID = sql_row[2];
			info.m_strFieldID = sql_row[3];
			info.m_strTime = sql_row[4];
			info.m_strMoney = sql_row[6];
			CString strTime = sql_row[8];
			int nYear = 0;
			int nMonth = 0;
			int nDay = 0;
			int nHour = 0;
			int nMin = 0;
			int nSec = 0;
			sscanf(strTime.GetBuffer(), "%04d-%02d-%02d %02d:%02d:%02d", &nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);		
			info.m_time.m_strDate.Format("%04d%02d%02d", nYear,nMonth,nDay);
			info.m_time.m_strTime.Format("%02d:%02d:%02d", nHour, nMin, nSec);
			hookFunc(2, &info);
		}
		else if (nType == 3)
		{
			HisChargeInfo info;
			info.m_strMoney = sql_row[6];
			info.m_strVipID = sql_row[2];
			CString strTime = sql_row[8];
			int nYear = 0;
			int nMonth = 0;
			int nDay = 0;
			int nHour = 0;
			int nMin = 0;
			int nSec = 0;
			sscanf(strTime.GetBuffer(), "%04d-%02d-%02d %02d:%02d:%02d",
				&nYear, &nMonth, &nDay, &nHour, &nMin, &nSec);		
			info.m_time.m_strDate.Format("%04d%02d%02d", nYear,nMonth,nDay);
			info.m_time.m_strTime.Format("%02d:%02d:%02d", nHour, nMin, nSec);
			hookFunc(3, &info);
		}
	}
}
