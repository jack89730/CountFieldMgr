#ifndef mysqlcon_h__
#define mysqlcon_h__

#include "stdafx.h"
#include <mysql.h>
#include "public.h"

class CMysqlConn
{
public:
	CMysqlConn()
	{
		m_strConfigPath  = TL_GetModulePath(NULL) + "database.cfg";
		m_bInit = FALSE;
	}
	~CMysqlConn()
	{
		if (m_bInit)
		{
			mysql_close(&m_Conn);
		}
	}
	BOOL IsConnect();
	int Connect();
	MYSQL* GetConnect(){return &m_Conn;};

private:
	MYSQL m_Conn;
	CString m_strConfigPath;
	BOOL m_bInit;
	CString m_strHost;
	CString m_strPort;
	CString m_strPwd;
	CString m_strUser;
	CString m_strDBName;
};

#endif // mysqlcon_h__
