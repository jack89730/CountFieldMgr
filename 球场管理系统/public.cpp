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
