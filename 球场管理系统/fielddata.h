#ifndef fielddata_h__
#define fielddata_h__
#include "stdafx.h"
#include <vector>

//һ��ʱ��ε�����
typedef struct _tagOneTimeInfo
{
	CString m_strVipID;
	BOOL m_bBusy;
	int m_nTimeIndex;
}OneTimeInfo;

//һ����������ʱ������
typedef struct _tagOneFieldInfo
{
	CString m_strFieldID;
	std::vector<OneTimeInfo> m_ayTimeInfo;

}OneFieldInfo;

//���г��ص���������
typedef CArray<OneFieldInfo> FIELD_LIST;

class CFieldData
{
public:
	CFieldData(int nTimeCount):m_nTimeCount(nTimeCount){};
	virtual int AddField(CString strFieldID) = 0;
	virtual int DeleteField(CString strFieldID) = 0;
	virtual int ChangeFieldInfo(CString strFieldID, OneTimeInfo& info) = 0;
	virtual int GetAllData(FIELD_LIST& data) = 0;
	virtual int AddFieldDay(CString strFieldID) = 0;

	void SetCurrentDate(CString strDate){m_strDate = strDate;};
	CString GetCurrentDate(){return m_strDate;};

protected:
	CString m_strDate;
	int m_nTimeCount;
};

class CFieldDataXML : public CFieldData
{
public:
	CFieldDataXML(const CString& strXML, int nTimeCount):CFieldData(nTimeCount)
	{
		m_strXML = strXML;
	}
	int AddField(CString strFieldID);
	int DeleteField(CString strFieldID);
	int ChangeFieldInfo(CString strFieldID, OneTimeInfo& info);
	int GetAllData(FIELD_LIST& data);
	int AddFieldDay(CString strFieldID);
private:
	CString m_strXML;
};

#endif // fielddata_h__


