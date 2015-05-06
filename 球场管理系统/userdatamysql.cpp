#include "userdata.h"

int CUserDataMysql::Add(UserInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	//create table user
	//	(
	//	id int unsigned auto_increment primary key,
	//	name char(20) not null,
	//	pwd char(40) not null,
	//	level tinyint not null,
	//	del bool not null
	//	);

	CString strSQL;
	strSQL.Format("insert into user values(null, '%s', '%s', %s)"
		, info.m_strName.GetBuffer()
		,info.m_strPwd.GetBuffer()
		,info.m_strLevel.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

//用户名不能重复，所以按用户名删除
int CUserDataMysql::Delete(CString strName)
{	
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("delete from user where name='%s'",strName.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res==0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CUserDataMysql::Change(UserInfo& info)
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
	strSQL.Format("update user set password='%s',level=%s where name='%s'"
		,info.m_strPwd.GetBuffer()
		,info.m_strLevel.GetBuffer()
		,info.m_strName.GetBuffer());

	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CUserDataMysql::GetAllData(USER_LIST& data)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	CString strSQL = "select* from user order by id";
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
			UserInfo info;
			info.m_strName = sql_row[1];
			info.m_strPwd = sql_row[2];
			info.m_strLevel = sql_row[3];
			data.Add(info);
		}
	}
	return ERROR_OK;
}

void CUserDataMysql::Save()
{

}