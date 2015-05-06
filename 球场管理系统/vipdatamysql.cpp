#include "vipdata.h"
#include "operatedata.h"
#include "app.h"

int CVipDataMysql::AddVip(VipInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("insert into vip values(null,'%s','%s','%s',%s,%s,false,false,null)"
		, info.m_strName.GetBuffer()
		,info.m_strSex.GetBuffer()
		,info.m_strPhone.GetBuffer()
		,info.m_strMoney.GetBuffer()
		,info.m_strLevelID.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_ADD_VIP;
	his_op_info.m_strInfo.Format("姓名:%s, 电话:%s,等级:%s"
		, info.m_strName.GetBuffer()
		, info.m_strPhone.GetBuffer()
		, info.m_strLevelID.GetBuffer());

	pOperate->Add(his_op_info);

	return ERROR_OK;
}

int CVipDataMysql::DeleteVip(CString strVipID)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	CString strSQL;
	strSQL.Format("update vip set del=true where id=%s", strVipID.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CVipDataMysql::ChangeVipInfo(VipInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strLock;
	if (info.m_bLock)
	{
		strLock="true";
	}
	else
	{
		strLock="false";
	}
	CString strSQL;

	//update 表名称 set 列名称=新值 where 更新条件;
	strSQL.Format("update vip set name='%s',sex='%s',phone='%s',money=%s,level=%s,locked=%s where id=%s"
		, info.m_strName.GetBuffer()
		,info.m_strSex.GetBuffer()
		,info.m_strPhone.GetBuffer()
		,info.m_strMoney.GetBuffer()
		,info.m_strLevelID.GetBuffer()
		,strLock.GetBuffer()
		,info.m_strID.GetBuffer());

	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

int CVipDataMysql::GetAllData(VIP_LIST& data, VIP_LIST& del)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	CString strSQL = "select* from vip order by id";
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
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
		ASSERT(nFields == 9);
		while(sql_row = mysql_fetch_row(result))//获取具体的数据
		{
			VipInfo info;
			info.m_strID = sql_row[0];
			info.m_strName = sql_row[1];
			info.m_strSex = sql_row[2];
			info.m_strPhone  = sql_row[3];
			info.m_strMoney = sql_row[4];
			info.m_strLevelID = sql_row[5];
			info.m_bLock = atoi(sql_row[6]);
			if (atoi(sql_row[7]))
			{
				del.Add(info);
			}
			else
			{
				data.Add(info);
			}
			
		}
	}
	return ERROR_OK;
}

void CVipDataMysql::Save()
{

}
