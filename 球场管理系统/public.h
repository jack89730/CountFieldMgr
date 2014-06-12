#ifndef public_h__
#define public_h__
#include <vector>
using namespace std;

#define PATHSPLIT 					_T('\\')		//µ¥¸ö×Ö·û
#define PATHSPLITSTRING				_T("\\")		//×Ö·û´®
#define PATHSPLIT_OTHER				_T('/')			//µ¥¸ö×Ö·û
#define PATHSPLITSTRING_OTHER		_T("/")			//×Ö·û´®

#define FIELD_BUSY "Ã¦"
#define FIELD_IDLE "ÏÐ"
#define FIELD_BUSY_VALUE "1"
#define FIELD_IDLE_VALUE "0"

#define IS_FIELD_BUSY(x) (x == FIELD_BUSY)
#define IS_FIELD_IDLE(x) (x == FIELD_IDLE)

#define FIELD_NULL_COLOR RGB(255,255,255)
#define FIELD_BUSY_COLOR RGB(255,0,0)
#define FIELD_IDLE_COLOR RGB(0,255,0)

typedef struct _tagOneTimeInfo
{
	CString m_strVipID;
	BOOL m_bBusy;
	int m_nTimeIndex;
}OneTimeInfo;

typedef struct _tagOneFieldInfo
{
	CString m_strFieldID;
	vector<OneTimeInfo> m_ayTimeInfo;

}OneFieldInfo;

typedef struct _tagVipInfo
{
	CString m_strID;
	CString m_strName;
	CString m_strSex;
	CString m_strPhone;
	CString m_strMoney;

}VipInfo;

typedef struct _tagFoodInfo
{
	CString m_strID;
	CString m_strName;
	CString m_strPrice;

}FoodInfo;

CString TL_GetModulePath(HANDLE hModule);

#endif // public_h__

