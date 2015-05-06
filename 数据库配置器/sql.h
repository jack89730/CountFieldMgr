#ifndef sql_h__
#define sql_h__

#define CREATE_VIP "create table vip\
	(\
		id int unsigned auto_increment primary key,\
		name char(20) not null,\
		sex char(2) not null,\
		phone char(11) not null,\
		money float not null,\
		level tinyint not null,\
		locked bool not null,\
		del bool not null,\
		addtime timestamp default current_timestamp\
		)"


#define CREATE_FOOD "create table food\
(\
 id int unsigned not null auto_increment primary key,\
 name char(20) not null,\
 price float not null,\
 remain int not null,\
 del bool not null\
 )"

//��������Ա���ȼ�1��������Ա 2��ͨ����Ա
#define CREATE_USER "create table user\
(\
 id int unsigned auto_increment primary key,\
 name char(20) not null,\
 password char(40) not null,\
 level tinyint not null\
 )"

//���Ĭ�ϵĳ�������Ա�˺�
#define INSERT_ADMIN "insert into user values(null, \"admin\", \"admin\", 1)"

//������Ա�ȼ���
#define  CREATE_LEVEL "create table level\
(\
 id int not null primary key,\
 name char(20) not null,\
 high int not null,\
 low int not null\
 )"
#define INSERT_LEVEL "insert into level values(0, \"�ǻ�Ա\", 10,10)"

//����Ӫҵʱ��������ƽ�ʱ������
#define CREATE_PERIOD "create table period\
(\
 id int unsigned auto_increment primary key,\
 init bool not null,\
 startmin int not null,\
 halfcount int not null,\
 goldstart int not null,\
 goldstop int not null,\
 weekend bool not null\
 );"

#define INSERT_PERIOD "insert into period values(null,false,0,0,0,0,false)"

//����������ʷ��
#define CREATE_HISTORY "create table history\
(\
 id int unsigned auto_increment primary key,\
 typeid tinyint not null,\
 vipid char(20) null,\
 fieldid int  null,\
 timestr char(20)  null,\
 foodname char(40)  null,\
 onemoney float not null,\
 count int unsigned not null,\
 addtime timestamp default current_timestamp\
 )"

//�������ر�
#define  CREATE_FIELD "create table field\
(\
 id int unsigned auto_increment primary key,\
 name char(40) not null,\
 del bool not null\
 )"

//����ʱ���һ���ڵİ�СʱΪ��λ��ʱ��
#define  CREATE_TIMEINDEX "create table timeindex\
(\
 id int not null primary key,\
 name char(20) not null\
 )"


//����Ԥ����Ϣ��û�м�¼��ʾû��Ԥ��
#define  CREATE_BOOKINFO "create table bookinfo\
(\
 id int unsigned auto_increment primary key,\
 bookstatus tinyint not null,\
 fieldid int unsigned not null,\
 vipid int not null,\
 timeindex int unsigned not null,\
 autoprice bool not null,\
 price float null,\
 date char(10) not null,\
 boxkey char(10) null\
 )"

//����������ʷ��
#define  CREATE_OPERATE "create table operate\
(\
id int unsigned auto_increment primary key,\
name char(30) not null,\
addtime timestamp default current_timestamp,\
type char(20) not null,\
info char(200) not null\
)"


#endif // sql_h__
