set names utf8;

create table if not exists uid(
value int(10) unsigned not null default '0',
primary key(value))engine=innodb;
insert into uid values(1000);

create table if not exists gid(
value int(10) unsigned not null default '0',
primary key(value))engine=innodb;
insert into gid values(1000);

create table if not exists gnid(
value int(10) unsigned not null default '0',
primary key(value))engine=innodb;
insert into gnid values(1000);

create table if not exists bid(
value int(10) unsigned not null default '0',
primary key(value))engine=innodb;
insert into bid values(1000);
