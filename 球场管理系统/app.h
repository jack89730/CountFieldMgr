
// �򳡹���ϵͳ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#include "userdata.h"

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "userdata.h"
// CMainApp:
// �йش����ʵ�֣������ �򳡹���ϵͳ.cpp
//

class CMainApp : public CWinAppEx
{
public:
	CMainApp();

// ��д
	public:
	virtual BOOL InitInstance();

	CString m_strUserLevel;
	CString m_strUserName;
	BOOL IsAdmin(){return m_strUserLevel == USER_LEVEL_ADMIN;};
	CString GetUserName(){return m_strUserName;}
	CString strDeadDate;

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;