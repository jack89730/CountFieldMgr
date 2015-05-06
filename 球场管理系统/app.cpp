
// �򳡹���ϵͳ.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "app.h"
#include "MainDlg.h"
#include "DlgLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainApp

BEGIN_MESSAGE_MAP(CMainApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMainApp ����

CMainApp::CMainApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMainApp ����

CMainApp theApp;


// CMainApp ��ʼ��

BOOL CMainApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	strDeadDate = END_DATE;
	CTime nowTime(time(NULL));
	CString strDate;
	strDate.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
	if (strDate > strDeadDate)
	{
		AfxMessageBox("����ѹ��ڣ�����ʹ�ã�");
		return FALSE;
	}
	CDlgLogin login;
	login.DoModal();
	if (!login.m_bSuccess)
	{
		return FALSE;
	}

	CMainDlg dlg;
	m_pMainWnd = &dlg;

	m_strUserName = login.m_strUserName;
	m_strUserLevel = login.m_strLevel;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	AfxOleInit();

	return FALSE;
}
