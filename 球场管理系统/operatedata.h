#ifndef operatedata_h__
#define operatedata_h__

#include "stdafx.h"
#include "mysqlcon.h"
#include "fastdelegate.h"
#define GetOperateData &CTLSingleton<CHisOperateDataMysql>::Instance()

//ֻ��Ӱ�����ݿ�Ĳ���������ʷ��¼

//�û���
#define OPERATE_TYPE_LOGIN "��¼"
#define OPERATE_TYPE_ADD_USER "����û�"
#define OPERATE_TYPE_DEL_USER "ɾ���û�"
#define OPERATE_TYPE_CHANGE_PWD "�޸�����"
//������
#define OPERATE_TYPE_BOOK_FIELD "Ԥ������"
#define OPERATE_TYPE_CANCEL_FIELD "ȡ��Ԥ������"
#define OPERATE_TYPE_PAY_FIELD "���ؽ���"
#define OPERATE_TYPE_ADD_FIELD "��ӳ���"
#define OPERATE_TYPE_DEL_FIELD "ɾ������"
#define OPERATE_TYPE_CHANGE_FIELD_NAME "�޸ĳ�������"
#define OPERATE_TYPE_SWAP_BOOK_INFO "����Ԥ����Ϣ"
#define OPERATE_TYPE_RENT_BOX "���ù���"
//��Ʒ��
#define OPERATE_TYPE_BUYFOOD "������Ʒ"
#define OPERATE_TYPE_ADD_FOOD "�����Ʒ"
#define OPERATE_TYPE_DEL_FOOD "ɾ����Ʒ"
#define OPERATE_TYPE_CHANGE_FOOD_INFO "�޸���Ʒ��Ϣ"
#define OPERATE_TYPE_STOCK_FOOD "��Ʒ����"
//��Ա��
#define OPERATE_TYPE_ADD_VIP "��ӻ�Ա"
#define OPERATE_TYPE_DEL_VIP "ɾ����Ա"
#define OPERATE_TYPE_CHARGE_VIP "��Ա��ֵ"
#define OPERATE_TYPE_LOCK_VIP "�����Ա"
#define OPERATE_TYPE_UNLOCK_VIP "�ⶳ��Ա"
#define OPERATE_TYPE_ADD_VIP_LEVEL "��ӻ�Ա�ȼ�"
#define OPERATE_TYPE_DEL_VIP_LEVEL "ɾ����Ա�ȼ�"
#define OPERATE_TYPE_CHANGE_VIP_LEVEL "�޸Ļ�Ա�ȼ�"

//ϵͳ������
#define OPERATE_TYPE_CHANGE_GOLD_TIME "�޸Ļƽ�ʱ��"

//����������ʷ��
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
	CString m_strName;//�û���
	CString m_addTime;//���ʱ���ã�ȡ��ʱʹ��
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
