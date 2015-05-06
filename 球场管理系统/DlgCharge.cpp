// DlgCharge.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgCharge.h"
#include "MainDlg.h"
#include "history.h"
#include "operatedata.h"
#include "app.h"

// CDlgCharge �Ի���

IMPLEMENT_DYNAMIC(CDlgCharge, CDialog)

CDlgCharge::CDlgCharge(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCharge::IDD, pParent)
	, m_strName(_T(""))
	, m_strID(_T(""))
	, m_nMoney(0)
	, m_nCharge(0)
	, m_nAfterCharge(0)
	, m_nTmpMoney(0)
{

}

CDlgCharge::~CDlgCharge()
{
}

void CDlgCharge::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_strName);
	DDX_Text(pDX, IDC_EDIT4, m_strID);
	DDX_Text(pDX, IDC_EDIT1, m_nMoney);
	DDX_Text(pDX, IDC_EDIT2, m_nCharge);
	DDX_Text(pDX, IDC_EDIT3, m_nAfterCharge);
	DDX_Text(pDX, IDC_EDIT6, m_nTmpMoney);
}


BEGIN_MESSAGE_MAP(CDlgCharge, CDialog)
	ON_BN_CLICKED(IDOK2, &CDlgCharge::OnBnClickedOk2)
	ON_BN_CLICKED(IDOK, &CDlgCharge::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgCharge::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgCharge ��Ϣ�������

void CDlgCharge::OnBnClickedOk2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_strID.IsEmpty())
	{
		MessageBox("��������Ϊ�գ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	if(m_nTmpMoney <= 0)
	{
		MessageBox("��ֵ���������0��", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}


	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	VipInfo* info = pMainWnd->m_pageVip.GetVipInfo(m_strID);
	if (!info)
	{
		MessageBox("��ԱID�����ڣ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return;
	}

	GetDlgItem(IDOK)->EnableWindow(TRUE);

	//������ʾ�������ֵ�����
	m_strName = info->m_strName;
	m_nMoney = atoi(info->m_strMoney.GetBuffer());
	m_nAfterCharge = m_nTmpMoney + m_nMoney;
	m_nCharge = m_nTmpMoney;
	UpdateData(FALSE);

	m_bIsSuccess = TRUE;
}

BOOL CDlgCharge::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_nMoney = 0;
	m_nAfterCharge = 0;
	m_strID;
	m_nCharge = 0;
	m_bIsSuccess = FALSE;
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	return TRUE;
}

void CDlgCharge::OnBnClickedOk()
{
	//Ҫ�˶���Ϣ֮����ܵ����ֵ��ť
	CWnd* p = AfxGetApp()->GetMainWnd();
	CMainDlg* pMainWnd = (CMainDlg*)p;
	const VipInfo* info = pMainWnd->m_pageVip.GetVipInfo(m_strID);
	if (!info)
	{
		MessageBox("��ԱID�����ڣ�", "��ʾ", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}
	VipInfo infoTmp = *info;
	infoTmp.m_strMoney.Format("%d", m_nAfterCharge);
	pMainWnd->m_pageVip.ChangeVipInfo(infoTmp);//ʹ��*info �����ڲ��Լ���ֵ���Լ���������

	HisChargeInfo chargeinfo;
	CTime nowTime(time(NULL));
	chargeinfo.m_time.m_strDate.Format("%04d%02d%02d", nowTime.GetYear(), nowTime.GetMonth(), nowTime.GetDay());
	chargeinfo.m_time.m_strTime.Format("%02d:%02d:%02d", nowTime.GetHour(), nowTime.GetMinute(), nowTime.GetSecond());
	chargeinfo.m_strMoney.Format("%d", m_nCharge);
	chargeinfo.m_strVipID = infoTmp.m_strID;

	//д�����Ѽ�¼
	CHisData* pVipHisData = NULL;
	if (DATA_FROM_TYPE == DATA_FROM_XML)
	{
		pVipHisData = new CHisDataXML(TL_GetModulePath(NULL) + 
			"viphistory" + PATHSPLITSTRING + m_strID + ".xml");
	}
	else if (DATA_FROM_TYPE == DATA_FROM_MYSQL)
	{
		pVipHisData = GetHistoryData;
	}
	else
	{
		pVipHisData = NULL;
		ASSERT(FALSE && "������Դ��֧�֣�");
		return;
	}

	pVipHisData->AddCharge(chargeinfo);

	if (DATA_FROM_TYPE == DATA_FROM_XML)
	{
		delete pVipHisData;
	}

	CHisOperateData* pOperate = GetOperateData;
	HisOperateInfo his_op_info;
	his_op_info.m_strName = GETUSERNAME;
	his_op_info.m_strType = OPERATE_TYPE_CHARGE_VIP;
	his_op_info.m_strInfo.Format("ID:%s, ��ֵ���:%s"
		, chargeinfo.m_strVipID.GetBuffer()
		, chargeinfo.m_strMoney.GetBuffer());

	pOperate->Add(his_op_info);

	OnOK();
}

void CDlgCharge::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}
