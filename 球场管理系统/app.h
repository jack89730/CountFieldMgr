
// 球场管理系统.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#include "userdata.h"

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "userdata.h"
// CMainApp:
// 有关此类的实现，请参阅 球场管理系统.cpp
//

class CMainApp : public CWinAppEx
{
public:
	CMainApp();

// 重写
	public:
	virtual BOOL InitInstance();

	CString m_strUserLevel;
	CString m_strUserName;
	BOOL IsAdmin(){return m_strUserLevel == USER_LEVEL_ADMIN;};
	CString GetUserName(){return m_strUserName;}
	CString strDeadDate;

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;