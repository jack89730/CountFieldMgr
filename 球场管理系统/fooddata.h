#ifndef fooddata_h__
#define fooddata_h__

#include "stdafx.h"
#include <vector>

typedef struct _tagFoodInfo
{
	CString m_strID;
	CString m_strName;
	CString m_strPrice;

}FoodInfo;

typedef CArray<FoodInfo> FOOD_LIST;

class CFoodData
{
public:
	virtual int AddFood(FoodInfo& info) = 0;
	virtual int DeleteFood(CString strID) = 0;
	virtual int ChangeFoodInfo(FoodInfo& info) = 0;
	virtual int GetAllData(FOOD_LIST& data) = 0;
};

class CFoodDataXML : public CFoodData
{
	public:
		CFoodDataXML(const CString& strXML)
		{
			m_strXML = strXML;
		}
		int AddFood(FoodInfo& info);
		int DeleteFood(CString strID);
		int ChangeFoodInfo(FoodInfo& info);
		int GetAllData(FOOD_LIST& data);
private:
	CString m_strXML;
};

#endif // fooddata_h__
