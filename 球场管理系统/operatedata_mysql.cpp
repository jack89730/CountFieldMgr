#include "operatedata.h"

//select * from history where addtime > '2014-08-12 20:57:17';
int CHisOperateDataMysql::Add(HisOperateInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("insert into operate values(null, '%s', null, '%s', '%s')"
		,info.m_strName.GetBuffer()
		,info.m_strType.GetBuffer()
		,info.m_strInfo.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}

void CHisOperateDataMysql::GetAllData(HookGetOperateData hookFunc, COleDateTime tStart, COleDateTime tStop, CString strUser)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return;
		}
	}
	//select * from history where addtime >= '2014-08-13 10:33:00' and addtime <= '2014-08-13 10:33:04';
	CString strStart;
	CString strStop;
	strStart.Format("%04d-%02d-%02d %02d:%02d:%02d", tStart.GetYear(), tStart.GetMonth()
		, tStart.GetDay(), 0, 0, 0);

	strStop.Format("%04d-%02d-%02d %02d:%02d:%02d", tStop.GetYear(), tStop.GetMonth()
		, tStop.GetDay(), 59, 59, 59);

	CString strSQL;
	if (strUser.IsEmpty())
	{
		strSQL.Format("select * from operate where addtime >= '%s' and addtime <= '%s' order by id", strStart.GetBuffer(), strStop.GetBuffer());
	}
	else
	{
		strSQL.Format("select * from operate where addtime >= '%s' and addtime <= '%s' and name='%s' order by id", strStart.GetBuffer(), strStop.GetBuffer(), strUser.GetBuffer());
	}
	
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
	ASSERT(nFields == 5);
	while(sql_row = mysql_fetch_row(result))//获取具体的数据
	{
			HisOperateInfo info;
			info.m_strName = sql_row[1];
			info.m_addTime = sql_row[2];
			info.m_strType = sql_row[3];
			info.m_strInfo = sql_row[4];
			hookFunc(&info);
	}
}
