#ifndef access_h__
#define access_h__

int ExcuteSql(CString strDb, CString strSQL);

int CreateDB(CString strDb);

#define  OPEN_DB_STRING "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=database.mdb"
#endif // access_h__

