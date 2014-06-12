#include "stdafx.h"
#include "access.h"

int ExcuteSql(CString strDb, CString strSQL)
{
	_ConnectionPtr	m_pConnection;	//建立连接对象指针
	m_pConnection.CreateInstance(__uuidof(Connection));//建立连接对象实例

	try                 
	{	
		// 打开本地Access库AddressBook.mdb
		CString strTmp = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + strDb;
		m_pConnection->Open(strTmp.GetBuffer(0),"","",adModeUnknown);
	}
	catch(_com_error e)
	{
		return -1;
	}  

	_variant_t m_pRecordset;

	try
	{
		//使用由本地游标库提供的客户端游标
		m_pConnection->CursorLocation = adUseClient;
		m_pConnection->Execute(_bstr_t(strSQL), &m_pRecordset, adExecuteNoRecords);
	}
	catch(_com_error *e)
	{
		e;
		return -1;	
	}


	if(m_pConnection->State)
		m_pConnection->Close();
	m_pConnection= NULL;  

	return 0;
}

int CreateDB(CString strDb)
{
	return 0;
}