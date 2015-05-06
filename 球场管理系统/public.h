#ifndef public_h__
#define public_h__
#include <vector>
using namespace std;

class CMainApp;

#define END_DATE "20151231"
#define TIME_COUNT 34
#define TIME_BEGIN 7
#define INDEX_TO_CLOCK(X) (X/2+TIME_BEGIN)

#define VIP_TMP_NAME "临时"
#define VIP_LEVEL_0 "0"//非会员
#

#define DATA_FROM_TYPE 2//底层数据操作版本

#define DATA_FROM_XML 1  //目前不支持
#define DATA_FROM_MYSQL 2

#define ERROR_OK 0
#define ERROR_NAME_EXIST -1
#define ERROR_OPEN_FILE -2
#define ERROR_OPEN_DB -3
#define ERROR_EXCUTE_SQL -4
#define ERROR_UNKNOW_ERROR -100

#define GETMAINWND 	((CMainDlg*)(AfxGetApp()->GetMainWnd()))
#define ISADMIN ((CMainApp*)AfxGetApp())->IsAdmin()
#define GETUSERNAME ((CMainApp*)AfxGetApp())->GetUserName()

#define PATHSPLIT 					_T('\\')		//单个字符
#define PATHSPLITSTRING				_T("\\")		//字符串
#define PATHSPLIT_OTHER				_T('/')			//单个字符
#define PATHSPLITSTRING_OTHER		_T("/")			//字符串

#define FIELD_BUSY_DATA 1
#define FIELD_PAYED_DATA 2
#define FIELD_IDLE_DATA 0

#define FIELD_BUSY "忙"
#define FIELD_IDLE "闲"
#define FIELD_BUSY_VALUE "1"
#define FIELD_IDLE_VALUE "0"

#define IS_FIELD_BUSY(x) (x == FIELD_BUSY)
#define IS_FIELD_IDLE(x) (x == FIELD_IDLE)

#define FIELD_NULL_COLOR RGB(255,255,255)
#define FIELD_BUSY_COLOR RGB(255,0,0)
#define FIELD_IDLE_COLOR RGB(0,255,0)


CString TL_GetModulePath(HANDLE hModule);
BOOL IsWeekend(CString strDate);

#endif // public_h__

