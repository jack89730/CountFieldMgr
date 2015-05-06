#include "fielddata.h"
#include "perioddata.h"
#include "operatedata.h"
#include "app.h"

int CFieldDataMysql::AddField(CString strFieldName)
{
	CPeriodData* pPeriod = GetPeriodData;
	PeriodInfo periodInfo;
	pPeriod->Read(periodInfo);
	if (!periodInfo.m_bInit)
	{
		ASSERT(FALSE && "��û��ʼ��������ӳ���");
		return ERROR_UNKNOW_ERROR;
	}

	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("insert into field values(null,'%s',false)"
		, strFieldName.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res == 0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_ADD_FIELD;
	his_op_info.m_strInfo.Format("��������:%s", strFieldName.GetBuffer());

	pOperate->Add(his_op_info);

	return ERROR_OK;
}
int CFieldDataMysql::DeleteField(CString strFieldID)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("update field set del=true where id=%s", strFieldID.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res == 0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_DEL_FIELD;
	his_op_info.m_strInfo.Format("����ID:%s", strFieldID.GetBuffer());

	pOperate->Add(his_op_info);
	return ERROR_OK;
}

int CFieldDataMysql::ChangeFieldBookInfo(CString strFieldID, OneTimeBookInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	{
		CString strSql;
		strSql.Format("select * from bookinfo where fieldid=%s and timeindex=%d and date=%s"
			,strFieldID.GetBuffer()
			,info.m_nTimeIndex
			,m_strDate.GetBuffer());
		int res=mysql_query(m_mysql.GetConnect(),strSql.GetBuffer());
		if (res != 0)
		{
			return ERROR_EXCUTE_SQL;
		}
		MYSQL_RES *result;
		MYSQL_ROW sql_row;
		result = mysql_store_result(m_mysql.GetConnect());//�����ѯ�������ݵ�result
		int RecordCount = 0;
		if (result)
		{
			int nFields = mysql_num_fields(result);
			while(sql_row = mysql_fetch_row(result))//��ȡ���������
			{
					RecordCount++;
			}
		}
		if(RecordCount == 0)
		{
			//���һ����¼
			CString strSQL;
			strSQL.Format("insert into bookinfo values(null,0,%s,0,%d,1,0,%s,null)"
				,strFieldID.GetBuffer()
				,info.m_nTimeIndex
				, m_strDate.GetBuffer());
			int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
			ASSERT(res == 0);
		}
		
	}
	CString strSQL;
	strSQL.Format("update bookinfo set bookstatus=%d,vipid=%s,autoprice=%d,price=%s,boxkey='%s' where fieldid=%s and timeindex=%d and date=%s"
		,info.m_bBusy
		,info.m_strVipID.GetBuffer()
		,info.m_bAutoPrice
		,info.m_strPrice.GetBuffer()
		,info.m_strBoxKey.GetBuffer()
		,strFieldID.GetBuffer()
		,info.m_nTimeIndex
		,m_strDate.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res == 0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	return ERROR_OK;
}
int CFieldDataMysql::ChangeFieldInfo(FieldInfo& info)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CString strSQL;
	strSQL.Format("update field set name='%s' where id=%s"
		,info.m_strFieldName.GetBuffer(),info.m_strFieldID.GetBuffer());
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res == 0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CHANGE_FIELD_NAME;
	his_op_info.m_strInfo.Format("����ID:%s, ������:%s", info.m_strFieldID.GetBuffer(), info.m_strFieldName.GetBuffer());

	pOperate->Add(his_op_info);


	return ERROR_OK;
}
int CFieldDataMysql::GetAllData(FIELD_LIST& data)
{
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}
	//��ȡ��field
	CString strSQL = "select* from field where del=false order by id";
	int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
	ASSERT(res == 0);
	if (res != 0)
	{
		return ERROR_EXCUTE_SQL;
	}
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	result = mysql_store_result(m_mysql.GetConnect());//�����ѯ�������ݵ�result
	if(result)
	{
		int nFields = mysql_num_fields(result);
		while(sql_row = mysql_fetch_row(result))//��ȡ���������
		{
			OneFieldBookInfo info;
			info.m_strFieldID = sql_row[0];
			info.m_strFieldName = sql_row[1];
			data.Add(info);
		}
	}

	//�������ҵ�Ԥ����Ϣ

	CPeriodData* pPeriod = GetPeriodData;
	PeriodInfo periodInfo;
	pPeriod->Read(periodInfo);

	int nField = data.GetSize();
	for (int i = 0; i < nField; i++)
	{
		std::vector<OneTimeBookInfo>& ayTimeInfo = data[i].m_ayTimeInfo;
		//��������ʱ��Σ��յ����ݡ�
		for (int index = 0; index < periodInfo.m_nHalfCount; index++)
		{
			OneTimeBookInfo info;
			info.m_bBusy = 0;
			info.m_strVipID = "0";
			info.m_nTimeIndex = index;
			info.m_bAutoPrice = TRUE;
			info.m_strPrice = "0";
			ayTimeInfo.push_back(info);
		}

		CString strSQL;
		strSQL.Format("select* from bookinfo where fieldid=%s and date=%s order by timeindex", data[i].m_strFieldID.GetBuffer(),m_strDate.GetBuffer());
		int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
		ASSERT(res == 0);
		if (res != 0)
		{
			return ERROR_EXCUTE_SQL;
		}
		MYSQL_RES *result;
		MYSQL_ROW sql_row;
		result = mysql_store_result(m_mysql.GetConnect());//�����ѯ�������ݵ�result
		if(result)
		{
			int nFields = mysql_num_fields(result);
			while(sql_row = mysql_fetch_row(result))//��ȡ���������
			{
				int nTimeIndex = atoi(sql_row[4]);
				OneTimeBookInfo& info = ayTimeInfo[nTimeIndex];
				info.m_bBusy = atoi(sql_row[1]);
				info.m_strVipID = sql_row[3];
				info.m_bAutoPrice = atoi(sql_row[5]);
				info.m_strPrice = sql_row[6];
				info.m_strBoxKey = sql_row[8];
			}
		}

	}
	return ERROR_OK;
}
int CFieldDataMysql::AddFieldDay(CString strFieldID)
{
	DWORD nbegin = GetTickCount();
	//��ӵ�ǰ��
	if (!m_mysql.IsConnect())
	{
		if (m_mysql.Connect() != ERROR_OK)
		{
			return ERROR_OPEN_DB;
		}
	}

	CPeriodData* pPeriod = GetPeriodData;
	PeriodInfo periodInfo;
	pPeriod->Read(periodInfo);

//#����Ԥ����Ϣ��û�м�¼��ʾû��Ԥ��
//	create table bookinfo
//		(
//		id int unsigned auto_increment primary key,
//		bookstatus tinyint not null,#1Ԥ����2����
//		fieldid int unsigned not null,
//		vipid int unsigned not null,
//		timeindex int unsigned not null,
//		aotoprice bool not null,
//		price float null,
//		date char(10) not null
//		);

	for (int i = 0; i < periodInfo.m_nHalfCount; i++)
	{
		CString strSQL;
		strSQL.Format("insert into bookinfo values(null,0,%s,0,%d,1,0,%s,null)"
			,strFieldID.GetBuffer()
			,i
			, m_strDate.GetBuffer());
		int res=mysql_query(m_mysql.GetConnect(),strSQL.GetBuffer());
		ASSERT(res == 0);
	}
	
	DWORD nend = GetTickCount();

	TRACE("AddFieldDay cost %d", nend - nbegin);
	return ERROR_OK;
}
void CFieldDataMysql::Save()
{

}