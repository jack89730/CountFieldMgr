// DlgBuyFood.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgBuyFood.h"


// CDlgBuyFood 对话框

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


// CDlgBuyFood 消息处理程序

void CDlgBuyFood::OnBnClickedBtnBuy()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int i;
	i = m_listMenu.GetSelectionMark();  //获取信息位置

	if (i == -1)  //如果未选则提示
	{
		MessageBox("请先选择您要点的菜！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return ;
	}

	CString PointMenuName   = m_listMenu.GetItemText(i, 0);  //获取第i行第0列的名称
	CString PointMenuPrice  = m_listMenu.GetItemText(i, 1);  //获取第i行第0列的名称
	CString PointMenuNameSelect;
	int n = m_listPointMenu.GetItemCount();   //获取行数目
	for (i=0; i<n; i++)
	{
		PointMenuNameSelect = m_listPointMenu.GetItemText(i, 0);  //附值给对比临时变量
		if (PointMenuNameSelect == PointMenuName)  //判断是否已有
		{
			MessageBox(PointMenuName+"该菜能刚才已经点过了！", "提示", MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
			return;
		}
	}

	if(m_nCount<=0)
	{
		MessageBox("数量不能为空！","提示",MB_OK|MB_ICONINFORMATION|MB_TASKMODAL);
		return;
	}

	_ConnectionPtr	m_pConnection;//创建连接对象指针
	m_pConnection.CreateInstance(__uuidof(Connection));//创建连接对象实例

	try                 
	{	
		// 打开本地Access库Menu.mdb
		m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Menu.mdb","","",adModeUnknown);
	}
	catch(_com_error e)  //捕捉异常
	{
		AfxMessageBox("数据库连接失败！");
		return;
	}  

	_RecordsetPtr	m_pRecordset;//创建记录集指针						
	m_pRecordset.CreateInstance(__uuidof(Recordset));//创建记录集对象

	try
	{
		m_pRecordset->Open("SELECT * FROM PointMenu",              // 查询Menu表中所有字段
			m_pConnection.GetInterfacePtr(),				// 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}  

	//添加新记录
	m_pRecordset->AddNew();
	m_pRecordset->PutCollect("Name", _variant_t(PointMenuName));
	m_pRecordset->PutCollect("MenuNum", _variant_t(m_nCount));
	//m_pRecordset->PutCollect("DeskID", _variant_t(DeskID));
	m_pRecordset->PutCollect("MenuPrice", _variant_t(atof(PointMenuPrice)*m_nCount));
	m_pRecordset->Update();  //更新到数据库

	m_pRecordset->Close();//关闭记录集对象
	m_pRecordset = NULL;

	if(m_pConnection->State)
		m_pConnection->Close();//关闭连接对象
	m_pConnection= NULL;

	CString strSQL;
	//strSQL.Format("select * from PointMenu where DeskID Like '%s'",  DeskID);
	//ShowItemPointMenu(strSQL);
	UpdateData(FALSE);
}
