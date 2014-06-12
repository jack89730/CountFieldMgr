// DlgCancelBook.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgCancelBook.h"


// CDlgCancelBook 对话框

IMPLEMENT_DYNAMIC(CDlgCancelBook, CDialog)

CDlgCancelBook::CDlgCancelBook(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCancelBook::IDD, pParent)
{

}

CDlgCancelBook::~CDlgCancelBook()
{
}

void CDlgCancelBook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCancelBook, CDialog)
END_MESSAGE_MAP()


// CDlgCancelBook 消息处理程序
