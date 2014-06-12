#ifndef xmlbase_h__
#define xmlbase_h__

#include "stdafx.h"

class CXMLBase
{
	int Open();
	int DeleteAll();
	int DeleteRecord();//id
	int UpdateRecord();//id,key=value
};

BOOL EditFieldStatus(CString strPath, CString strFieldID, CString strVipID, int nTimeIndex, CString strValue);

#endif // xmlbase_h__
