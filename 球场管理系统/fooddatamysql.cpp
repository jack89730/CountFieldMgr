#include "fooddata.h"
#include "operatedata.h"
#include "singleton.h"
#include "app.h"


int CFoodDataMysql::AddFood(FoodInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("insert into food values(null, '%s', %s, %s,false)"
		, info.m_strName.GetBuffer()
		,info.m_strPrice.GetBuffer()
		,info.m_strRemain.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_ADD_FOOD;
	his_op_info.m_strInfo.Format("添加商品:%s", info.m_strName.GetBuffer());

	pOperate->Add(his_op_info);

	return ERROR_OK;
}

int CFoodDataMysql::DeleteFood(CString strID)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("update food set del=true where id=%s",strID.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	return ERROR_OK;
}

int CFoodDataMysql::ChangeFoodInfo(FoodInfo& info)
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
	strSQL.Format("update food set name='%s',price=%s,remain=%s where id=%s"
		, info.m_strName.GetBuffer()
		,info.m_strPrice.GetBuffer()
		,info.m_strRemain.GetBuffer()
		,info.m_strID.GetBuffer());

	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CFoodDataMysql::GetAllData(FOOD_LIST& data)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	CString strSQL = "select* from food where del=false order by id";
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
		ASSERT(nFields == 5);
		while(sql_row = mysql_fetch_row(result))//获取具体的数据
		{
			FoodInfo info;
			info.m_strID = sql_row[0];
			info.m_strName = sql_row[1];
			info.m_strPrice = sql_row[2];
			info.m_strRemain  = sql_row[3];
			data.Add(info);
		}
	}
	return ERROR_OK;
}
void CFoodDataMysql::Save()
{

}