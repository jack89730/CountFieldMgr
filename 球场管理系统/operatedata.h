#ifndef operatedata_h__
#define operatedata_h__

#include "stdafx.h"
#include "mysqlcon.h"
#include "fastdelegate.h"
#define GetOperateData &CTLSingleton<CHisOperateDataMysql>::Instance()

//只对影响数据库的操作进行历史记录

//用户类
#define OPERATE_TYPE_LOGIN "登录"
#define OPERATE_TYPE_ADD_USER "添加用户"
#define OPERATE_TYPE_DEL_USER "删除用户"
#define OPERATE_TYPE_CHANGE_PWD "修改密码"
//场地类
#define OPERATE_TYPE_BOOK_FIELD "预定场地"
#define OPERATE_TYPE_CANCEL_FIELD "取消预定场地"
#define OPERATE_TYPE_PAY_FIELD "场地结账"
#define OPERATE_TYPE_ADD_FIELD "添加场地"
#define OPERATE_TYPE_DEL_FIELD "删除场地"
#define OPERATE_TYPE_CHANGE_FIELD_NAME "修改场地名称"
#define OPERATE_TYPE_SWAP_BOOK_INFO "交换预定信息"
#define OPERATE_TYPE_RENT_BOX "租用柜子"
//商品类
#define OPERATE_TYPE_BUYFOOD "购买商品"
#define OPERATE_TYPE_ADD_FOOD "添加商品"
#define OPERATE_TYPE_DEL_FOOD "删除商品"
#define OPERATE_TYPE_CHANGE_FOOD_INFO "修改商品信息"
#define OPERATE_TYPE_STOCK_FOOD "商品进货"
//会员类
#define OPERATE_TYPE_ADD_VIP "添加会员"
#define OPERATE_TYPE_DEL_VIP "删除会员"
#define OPERATE_TYPE_CHARGE_VIP "会员充值"
#define OPERATE_TYPE_LOCK_VIP "冻结会员"
#define OPERATE_TYPE_UNLOCK_VIP "解冻会员"
#define OPERATE_TYPE_ADD_VIP_LEVEL "添加会员等级"
#define OPERATE_TYPE_DEL_VIP_LEVEL "删除会员等级"
#define OPERATE_TYPE_CHANGE_VIP_LEVEL "修改会员等级"

//系统管理类
#define OPERATE_TYPE_CHANGE_GOLD_TIME "修改黄金时段"

//创建操作历史表
#define  CREATE_OPERATE "create table operate\
(\
id int unsigned auto_increment primary key,\
name char(30) not null,\
addtime timestamp default current_timestamp\
type char(20) not null,\
info char(200) not null\
)"

typedef struct _tagHisOperateInfo
{
	CString m_strName;//用户名
	CString m_addTime;//添加时不用，取出时使用
	CString m_strType;
	CString m_strInfo;
}HisOperateInfo;

typedef fastdelegate::FastDelegate1<HisOperateInfo*, void> HookGetOperateData;

class CHisOperateData
{
public:
	virtual int Add(HisOperateInfo& info) = 0;
	virtual void GetAllData(HookGetOperateData hookFunc, COleDateTime tStart, COleDateTime tStop, CString strUser) = 0;
};

//select * from history where addtime > '2014-08-12 20:57:17';
class CHisOperateDataMysql : public CHisOperateData
{
public:
	virtual int Add(HisOperateInfo& info);
	virtual void GetAllData(HookGetOperateData hookFunc, COleDateTime tStart, COleDateTime tStop, CString strUser);
private:
	CMysqlConn m_mysql;
};

#endif // operatedata_h__
