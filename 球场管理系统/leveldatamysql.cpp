#include "leveldata.h"
#include "operatedata.h"
#include "app.h"

int CLevelDataMysql::AddPrice(OnePriceInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

// 	create table level
// 		(
// 		id int unsigned not null auto_increment primary key,
// 		name char(20) not null,
// 		high int not null,
// 		low int not null
// 		);

	int high = atoi(info.m_strPriceHigh);
	int low = atoi(info.m_strPriceLow);
	CString strSQL;
	strSQL.Format("insert into level values(%s, '%s', %d, %d)"
		,info.m_strID.GetBuffer()
		,info.m_strName.GetBuffer(),high,low);
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_ADD_VIP_LEVEL;
	his_op_info.m_strInfo.Format("数值:%s,名称:%s,黄金时段:%s元,非黄金时段:%s元"
		, info.m_strID.GetBuffer()
		, info.m_strName.GetBuffer()
		, info.m_strPriceHigh.GetBuffer()
		, info.m_strPriceLow.GetBuffer());

	pOperate->Add(his_op_info);


	return ERROR_OK;
}

int CLevelDataMysql::DeletePrice(CString strID)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("delete from level where id='%s'",strID.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res==0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CLevelDataMysql::ChangePriceInfo(OnePriceInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	//update 表名称 set 列名称=新值 where 更新条件;
	strSQL.Format("update level set name='%s',high=%d,low=%d where id=%s"
		,info.m_strName.GetBuffer()
		,atoi(info.m_strPriceHigh)
		,atoi(info.m_strPriceLow)
		,info.m_strID.GetBuffer());

	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CLevelDataMysql::GetAllData(PRICE_LIST& data)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	CString strSQL = "select* from level order by id";
	int res = mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	result = mysql_store_result(m_mysql.GetConnect());//保存查询到的数据到result
	if(result)
	{
		int nFields = mysql_num_fields(result);
		ASSERT(nFields == 4);
		while(sql_row = mysql_fetch_row(result))//获取具体的数据
		{
			OnePriceInfo info;
			info.m_strID = sql_row[0];
			info.m_strName = sql_row[1];
			info.m_strPriceHigh = sql_row[2];
			info.m_strPriceLow = sql_row[3];
			data.Add(info);
		}
	}
	return ERROR_OK;
}

void CLevelDataMysql::Save()
{

}