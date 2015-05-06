#include "perioddata.h"
#include "operatedata.h"
#include "MainDlg.h"
#include "app.h"

int CPeriodDataMysql::Set(const PeriodInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	//create table period
	//	(
	//	init bool not null,
	//	startmin int not null,
	//	halfcount int not null,
	//	goldstart int not null,
	//	goldstop int not null,
	//	weekend bool not null
	//	);

	CString strSQL;
	strSQL.Format("update period set init=%d,startmin=%d,halfcount=%d,goldstart=%d,goldstop=%d,weekend=%d where id=1"
		,info.m_bInit, info.m_nStartMin, info.m_nHalfCount, info.m_nGoldStart, info.m_nGoldStop, info.m_bWeekend);
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res==0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CHANGE_GOLD_TIME;


	CMainDlg* pMainWnd = GETMAINWND;
	CString strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[info.m_nGoldStart];
	CString strStart = strTmp.Left(strTmp.Find('-'));

	strTmp = pMainWnd->m_pageField.m_ayTimeByIndex[info.m_nGoldStop - 1];
	CString strStop =strTmp.Right(strTmp.Find('-'));

	if (info.m_bWeekend)
	{
		strTmp = "是";
	}
	else
	{
		strTmp = "否";
	}

	his_op_info.m_strInfo.Format("黄金时段:%s-%s,周末为黄金:%s", strStart, strStop, strTmp.GetBuffer());

	pOperate->Add(his_op_info);

	return ERROR_OK;
}

int CPeriodDataMysql::Read(PeriodInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	CString strSQL = "select* from period";
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
		ASSERT(nFields == 7);
		sql_row = mysql_fetch_row(result);//获取具体的数据
		if (sql_row)
		{
			info.m_bInit = atoi(sql_row[1]);
			info.m_nStartMin = atoi(sql_row[2]);
			info.m_nHalfCount = atoi(sql_row[3]);
			info.m_nGoldStart = atoi(sql_row[4]);
			info.m_nGoldStop = atoi(sql_row[5]);
			info.m_bWeekend = atoi(sql_row[6]);
		}
		
	}
	return ERROR_OK;
}

void CPeriodDataMysql::Save()
{

}