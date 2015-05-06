#include "stdafx.h"
#include "public.h"

CString TL_GetModulePath(HANDLE hModule)
{
	TCHAR szPath[_MAX_PATH];
	szPath[0] ='\0';
	GetModuleFileName((HMODULE)hModule, szPath, sizeof(szPath));
	int i = 0;
	for (i = _tcslen(szPath) - 1; i >= 0 && szPath[i] != PATHSPLIT; i--);
	i++;
	szPath[i] = '\0';
	return szPath;
}

BOOL IsWeekend(CString strDate)
{
	//CTime( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
	//	int nDST = -1 );
	int nDate = atoi(strDate.GetBuffer());
	int nYear = nDate/10000;
	int nMonth = nDate%10000/100;
	int nDay = nDate%10000%100;
	CTime nowTime(nYear, nMonth, nDay, 0, 0, 0);
	int nDOW = nowTime.GetDayOfWeek();
	BOOL bHgih = FALSE;
	if (nDOW == 5 || nDOW == 6)
	{
		return TRUE;
	}
	return FALSE;
}