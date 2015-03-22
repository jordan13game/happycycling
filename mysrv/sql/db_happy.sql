set names utf8;

create table if not exists Account(
uid int(10) unsigned NOT NULL comment '角色id',
username varchar(20) character set utf8 not null comment '账号',
password varchar(20) character set utf8 not null comment '密码',
ctime timestamp not null default CURRENT_TIMESTAMP comment 'create time',
nickname varchar(20) character set utf8 not null comment '昵称',
sex tinyint(1) unsigned not null default 0 comment '性别',
head mediumtext not null comment '头像',
primary key(uid),
unique key(username))engine=innodb;

create table if not exists Chat_log(
uid int(10) unsigned NOT NULL comment '角色id',
stime varchar(20) character set utf8 not null comment '发送时间',
isteam tinyint(1) unsigned not null default 0 comment '是否是队伍聊天',
content mediumtext not null comment '内容',
primary key(uid, stime))engine=innodb;

create table if not exists Team(
gid int(10) unsigned NOT NULL comment '队伍id',
uid int(10) unsigned NOT NULL comment '队长id',
teamname varchar(20) character set utf8 not null comment '队伍名称',
desp varchar(1000) not null comment '队伍描述',
primary key(gid))engine=innodb;

create table if not exists Team_User(
uid int(10) unsigned NOT NULL comment '角色id',
gid int(10) unsigned NOT NULL comment '队伍id',
primary key(uid),
key(gid))engine=innodb;

create table if not exists Team_ntinfo(
id int(10) unsigned not null,
uid int(10) unsigned NOT NULL comment '角色id',
fromuid int(10) unsigned not NULL comment '发起角色id',
infotype tinyint(1) unsigned not NULL comment '1加入队伍 2退出队伍 3踢出队伍 4队伍解散 5接受加入 6拒绝加入',
vertify varchar(400) comment '认证信息',
primary key(id),
key(uid, fromuid, infotype))engine=innodb;

create table if not exists Blog(
id int(10) unsigned not null,
uid int(10) unsigned NOT NULL comment '角色id',
ctime timestamp not null default CURRENT_TIMESTAMP comment 'create time',
content mediumtext not null comment '内容',
lng double(10,6) not null comment '经度',
lat double(10,6) not null comment '纬度',
location varchar(100) comment '位置',
primary key(id),
key(uid, ctime))engine=innodb;
