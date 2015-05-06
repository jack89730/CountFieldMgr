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

//创建管理员表，等级1超级管理员 2普通管理员
#define CREATE_USER "create table user\
(\
 id int unsigned auto_increment primary key,\
 name char(20) not null,\
 password char(40) not null,\
 level tinyint not null\
 )"

//添加默认的超级管理员账号
#define INSERT_ADMIN "insert into user values(null, \"admin\", \"admin\", 1)"

//创建会员等级表
#define  CREATE_LEVEL "create table level\
(\
 id int not null primary key,\
 name char(20) not null,\
 high int not null,\
 low int not null\
 )"
#define INSERT_LEVEL "insert into level values(0, \"非会员\", 10,10)"

//创建营业时间表，包含黄金时段区间
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

//创建交易历史表
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

//创建场地表
#define  CREATE_FIELD "create table field\
(\
 id int unsigned auto_increment primary key,\
 name char(40) not null,\
 del bool not null\
 )"

//创建时间表，一天内的半小时为单位的时间
#define  CREATE_TIMEINDEX "create table timeindex\
(\
 id int not null primary key,\
 name char(20) not null\
 )"


//创建预订信息表，没有记录表示没有预订
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

//创建操作历史表
#define  CREATE_OPERATE "create table operate\
(\
id int unsigned auto_increment primary key,\
name char(30) not null,\
addtime timestamp default current_timestamp,\
type char(20) not null,\
info char(200) not null\
)"


#endif // sql_h__
