// DlgFieldMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgFieldMgr.h"
#include "DlgAddField.h"
#include "DlgChangeFieldInfo.h"
// CDlgFieldMgr �Ի���

IMPLEMENT_DYNAMIC(CDlgFieldMgr, CDialog)

CDlgFieldMgr::CDlgFieldMgr(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFieldMgr::IDD, pParent)
{

}

CDlgFieldMgr::~CDlgFieldMgr()
{
}

void CDlgFieldMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listField);
}


BEGIN_MESSAGE_MAP(CDlgFieldMgr, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgFieldMgr::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlgFieldMgr::OnBnClickedButton4)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDlgFieldMgr::OnNMDblclkList1)
END_MESSAGE_MAP()


// CDlgFieldMgr ��Ϣ�������

void CDlgFieldMgr::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgAddField dlg;
	dlg.m_strXML = m_strXML;
	dlg.DoModal();
	if (dlg.IsSuccess())
	{
		ShowItemField();
	}
}

void CDlgFieldMgr::OnBnClickedButton4()
{
	POSITION	pos;
	int			iPos;
	pos = m_listField.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox("����ѡ��Ҫɾ���ļ�¼��","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	iPos		    	= m_listField.GetNextSelectedItem(pos);

	CMainDlg* pMainWnd = GETMAINWND;

	CString strMsg;
	CString strID = pMainWnd->m_pageField.m_ayFieldInfo[iPos].m_strFieldID;
	CString strName = pMainWnd->m_pageField.m_ayFieldInfo[iPos].m_strFieldName;
	strMsg.Format("�Ƿ�ɾ������ ID:%s,����:%s?", strID.GetBuffer(), strName.GetBuffer());
	if (MessageBox(strMsg, _T("��ʾ"), MB_YESNO|MB_ICONINFORMATION|MB_TASKMODAL) != IDYES)
	{
		return;
	}
	
	int ret = pMainWnd->m_pageField.m_pFieldData->DeleteField(pMainWnd->m_pageField.m_ayFieldInfo[iPos].m_strFieldID);
	ShowItemField();
}

BOOL CDlgFieldMgr::OnInitDialog()
{
	CMainDlg* pMain = GETMAINWND;
	m_strDate = pMain->m_pageField.GetDate();
	m_nCurSelectIndex = 0;
	m_pFieldData = GetFieldData;

	CDialog::OnInitDialog();
	m_listField.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	int k=0;
	m_listField.InsertColumn(k++, _T("ID"),	LVCFMT_CENTER, 100);
	m_listField.InsertColumn(k++, _T("��������"),	LVCFMT_CENTER, 200);
	ShowItemField();
	return TRUE;
}

void CDlgFieldMgr::ShowItemField()
{
	//�ڴ�ֻ������������
	m_listField.DeleteAllItems();

	CMainDlg* pMainWnd = GETMAINWND;
	pMainWnd->m_pageField.ShowItemField(m_strDate);

	int nFieldCount = pMainWnd->m_pageField.m_ayFieldInfo.GetCount();
	for (int i = 0; i < nFieldCount; i++)
	{
		int j = 0;
		m_listField.InsertItem(i, pMainWnd->m_pageField.m_ayFieldInfo[i].m_strFieldID);
		m_listField.SetItemText(i, ++j, pMainWnd->m_pageField.m_ayFieldInfo[i].m_strFieldName);
	}

	UpdateData(FALSE);
}

void CDlgFieldMgr::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NMITEMACTIVATE *pNotifier=(NMITEMACTIVATE*)pNMHDR;

	CDlgChangeFieldInfo dlg;
	dlg.m_strID = m_listField.GetItemText(pNotifier->iItem, 0);
	dlg.m_strName = m_listField.GetItemText(pNotifier->iItem, 1);
	dlg.DoModal();
	ShowItemField();
}
