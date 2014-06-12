// DlgBuyFood.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgBuyFood.h"


// CDlgBuyFood �Ի���

IMPLEMENT_DYNAMIC(CDlgBuyFood, CDialog)

CDlgBuyFood::CDlgBuyFood(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgBuyFood::IDD, pParent)
	, m_nCount(0)
{

}

CDlgBuyFood::~CDlgBuyFood()
{
}

void CDlgBuyFood::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listMenu);
	DDX_Control(pDX, IDC_LIST2, m_listPointMenu);
	DDX_Text(pDX, IDC_EDIT1, m_nCount);
}


BEGIN_MESSAGE_MAP(CDlgBuyFood, CDialog)
	ON_BN_CLICKED(IDC_BTN_BUY, &CDlgBuyFood::OnBnClickedBtnBuy)
END_MESSAGE_MAP()


// CDlgBuyFood ��Ϣ�������

void CDlgBuyFood::OnBnClickedBtnBuy()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	int i;
	i = m_listMenu.GetSelectionMark();  //��ȡ��Ϣλ��

	if (i == -1)  //���δѡ����ʾ
	{
		MessageBox("����ѡ����Ҫ��Ĳˣ�","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}

	CString PointMenuName   = m_listMenu.GetItemText(i, 0);  //��ȡ��i�е�0�е�����
	CString PointMenuPrice  = m_listMenu.GetItemText(i, 1);  //��ȡ��i�е�0�е�����
	CString PointMenuNameSelect;
	int n = m_listPointMenu.GetItemCount();   //��ȡ����Ŀ
	for (i=0; i<n; i++)
	{
		PointMenuNameSelect = m_listPointMenu.GetItemText(i, 0);  //��ֵ���Ա���ʱ����
		if (PointMenuNameSelect == PointMenuName)  //�ж��Ƿ�����
		{
			MessageBox(PointMenuName+"�ò��ܸղ��Ѿ�����ˣ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}

	if(m_nCount<=0)
	{
		MessageBox("��������Ϊ�գ�","��ʾ",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	_ConnectionPtr	m_pConnection;//�������Ӷ���ָ��
	m_pConnection.CreateInstance(__uuidof(Connection));//�������Ӷ���ʵ��

	try                 
	{	
		// �򿪱���Access��Menu.mdb
		m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Menu.mdb","","",adModeUnknown);
	}
	catch(_com_error e)  //��׽�쳣
	{
		AfxMessageBox("���ݿ�����ʧ�ܣ�");
		return;
	}  

	_RecordsetPtr	m_pRecordset;//������¼��ָ��						
	m_pRecordset.CreateInstance(__uuidof(Recordset));//������¼������

	try
	{
		m_pRecordset->Open("SELECT * FROM PointMenu",              // ��ѯMenu���������ֶ�
			m_pConnection.GetInterfacePtr(),				// ��ȡ��ӿ��IDispatchָ��
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}  

	//����¼�¼
	m_pRecordset->AddNew();
	m_pRecordset->PutCollect("Name", _variant_t(PointMenuName));
	m_pRecordset->PutCollect("MenuNum", _variant_t(m_nCount));
	//m_pRecordset->PutCollect("DeskID", _variant_t(DeskID));
	m_pRecordset->PutCollect("MenuPrice", _variant_t(atof(PointMenuPrice)*m_nCount));
	m_pRecordset->Update();  //���µ����ݿ�

	m_pRecordset->Close();//�رռ�¼������
	m_pRecordset = NULL;

	if(m_pConnection->State)
		m_pConnection->Close();//�ر����Ӷ���
	m_pConnection= NULL;

	CString strSQL;
	//strSQL.Format("select * from PointMenu where DeskID Like '%s'",  DeskID);
	//ShowItemPointMenu(strSQL);
	UpdateData(FALSE);
}
