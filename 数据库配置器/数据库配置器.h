
// ���ݿ�������.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// C���ݿ�������App:
// �йش����ʵ�֣������ ���ݿ�������.cpp
//

class C���ݿ�������App : public CWinAppEx
{
public:
	C���ݿ�������App();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern C���ݿ�������App theApp;