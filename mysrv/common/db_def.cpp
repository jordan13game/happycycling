#include "db_def.h"
#include "db_service.h"
#include <boost/format.hpp>
int db_Team_User_t::init(sql_result_row_t& row_){
    if (row_.empty())
    {
        return -1;
    }
    size_t i=0;gid=(int)std::atoi(row_[i++].c_str());
uid=(int)std::atoi(row_[i++].c_str());
return 0;
}
int db_Team_User_t::select_gid(const uint32_t& gid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `gid` , `uid` FROM `Team_User` WHERE  `gid` = %u")% gid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_User_t::select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `gid` , `uid` FROM `Team_User` WHERE  `uid` = %u")% uid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_User_t::sync_select_gid(const uint32_t& gid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `gid` , `uid` FROM `Team_User` WHERE  `gid` = %u")% gid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_User_t::sync_select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `gid` , `uid` FROM `Team_User` WHERE  `uid` = %u")% uid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_User_t::update(){

boost::format sqlfmt=boost::format("UPDATE `Team_User` SET  `gid` = %u WHERE  `uid` = %u")% gid% uid;
return db_service.async_execute(sqlfmt.str());
}
int db_Team_User_t::sync_update(){

boost::format sqlfmt=boost::format("UPDATE `Team_User` SET  `gid` = %u WHERE  `uid` = %u")% gid% uid;
return db_service.sync_execute(sqlfmt.str());
}
int db_Team_User_t::remove(){

boost::format sqlfmt=boost::format("DELETE FROM `Team_User` WHERE  `uid` = %u")% uid;
return db_service.async_execute(sqlfmt.str());
}
string db_Team_User_t::gen_insert_sql(){

boost::format sqlfmt=boost::format("INSERT INTO `Team_User` ( `gid`  ,  `uid` ) VALUES ( %u , %u )" )% gid% uid;
return sqlfmt.str();
}
int db_Team_User_t::insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Team_User` ( `gid`  ,  `uid` ) VALUES ( %u , %u )" )% gid% uid;
return db_service.async_execute(sqlfmt.str());
}
int db_Team_User_t::sync_insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Team_User` ( `gid`  ,  `uid` ) VALUES ( %u , %u )" )% gid% uid;
return db_service.sync_execute(sqlfmt.str());
}
int db_Chat_log_t::init(sql_result_row_t& row_){
    if (row_.empty())
    {
        return -1;
    }
    size_t i=0;stime = row_[i++];
uid=(int)std::atoi(row_[i++].c_str());
content = row_[i++];
isteam=(int)std::atoi(row_[i++].c_str());
return 0;
}
int db_Chat_log_t::select_uid_team(const uint32_t& uid_, const uint16_t& isteam_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `stime` , `uid` , `content` , `isteam` FROM `Chat_log` WHERE  `uid` = %u AND  `isteam` = %d")% uid_% isteam_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Chat_log_t::select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `stime` , `uid` , `content` , `isteam` FROM `Chat_log` WHERE  `uid` = %u")% uid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Chat_log_t::sync_select_uid_team(const uint32_t& uid_, const uint16_t& isteam_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `stime` , `uid` , `content` , `isteam` FROM `Chat_log` WHERE  `uid` = %u AND  `isteam` = %d")% uid_% isteam_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Chat_log_t::sync_select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `stime` , `uid` , `content` , `isteam` FROM `Chat_log` WHERE  `uid` = %u")% uid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Chat_log_t::update(){

boost::format sqlfmt=boost::format("UPDATE `Chat_log` SET  `content` = '%s' ,  `isteam` = %d WHERE  `uid` = %u AND  `stime` = '%s'")% content.c_str()% isteam% uid% stime.c_str();
return db_service.async_execute(sqlfmt.str());
}
int db_Chat_log_t::sync_update(){

boost::format sqlfmt=boost::format("UPDATE `Chat_log` SET  `content` = '%s' ,  `isteam` = %d WHERE  `uid` = %u AND  `stime` = '%s'")% content.c_str()% isteam% uid% stime.c_str();
return db_service.sync_execute(sqlfmt.str());
}
int db_Chat_log_t::remove(){

boost::format sqlfmt=boost::format("DELETE FROM `Chat_log` WHERE  `uid` = %u AND  `stime` = '%s'")% uid% stime.c_str();
return db_service.async_execute(sqlfmt.str());
}
string db_Chat_log_t::gen_insert_sql(){

boost::format sqlfmt=boost::format("INSERT INTO `Chat_log` ( `stime`  ,  `uid`  ,  `content`  ,  `isteam` ) VALUES ( '%s' , %u , '%s' , %d )" )% stime.c_str()% uid% content.c_str()% isteam;
return sqlfmt.str();
}
int db_Chat_log_t::insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Chat_log` ( `stime`  ,  `uid`  ,  `content`  ,  `isteam` ) VALUES ( '%s' , %u , '%s' , %d )" )% stime.c_str()% uid% content.c_str()% isteam;
return db_service.async_execute(sqlfmt.str());
}
int db_Chat_log_t::sync_insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Chat_log` ( `stime`  ,  `uid`  ,  `content`  ,  `isteam` ) VALUES ( '%s' , %u , '%s' , %d )" )% stime.c_str()% uid% content.c_str()% isteam;
return db_service.sync_execute(sqlfmt.str());
}
int db_Blog_t::init(sql_result_row_t& row_){
    if (row_.empty())
    {
        return -1;
    }
    size_t i=0;uid=(int)std::atoi(row_[i++].c_str());
location = row_[i++];
id=(int)std::atoi(row_[i++].c_str());
lat=(int)std::atoi(row_[i++].c_str());
content = row_[i++];
lng=(int)std::atoi(row_[i++].c_str());
return 0;
}
int db_Blog_t::select_id(const uint32_t& id_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `location` , `id` , `lat` , `content` , `lng` FROM `Blog` WHERE  `id` = %u")% id_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Blog_t::select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `location` , `id` , `lat` , `content` , `lng` FROM `Blog` WHERE  `uid` = %u")% uid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Blog_t::sync_select_id(const uint32_t& id_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `location` , `id` , `lat` , `content` , `lng` FROM `Blog` WHERE  `id` = %u")% id_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Blog_t::sync_select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `location` , `id` , `lat` , `content` , `lng` FROM `Blog` WHERE  `uid` = %u")% uid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Blog_t::update(){

boost::format sqlfmt=boost::format("UPDATE `Blog` SET  `uid` = %u ,  `location` = '%s' ,  `lat` = %f ,  `content` = '%s' ,  `lng` = %f WHERE  `id` = %u")% uid% location.c_str()% lat% content.c_str()% lng% id;
return db_service.async_execute(sqlfmt.str());
}
int db_Blog_t::sync_update(){

boost::format sqlfmt=boost::format("UPDATE `Blog` SET  `uid` = %u ,  `location` = '%s' ,  `lat` = %f ,  `content` = '%s' ,  `lng` = %f WHERE  `id` = %u")% uid% location.c_str()% lat% content.c_str()% lng% id;
return db_service.sync_execute(sqlfmt.str());
}
int db_Blog_t::remove(){

boost::format sqlfmt=boost::format("DELETE FROM `Blog` WHERE  `id` = %u")% id;
return db_service.async_execute(sqlfmt.str());
}
string db_Blog_t::gen_insert_sql(){

boost::format sqlfmt=boost::format("INSERT INTO `Blog` ( `uid`  ,  `location`  ,  `id`  ,  `lat`  ,  `content`  ,  `lng` ) VALUES ( %u , '%s' , %u , %f , '%s' , %f )" )% uid% location.c_str()% id% lat% content.c_str()% lng;
return sqlfmt.str();
}
int db_Blog_t::insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Blog` ( `uid`  ,  `location`  ,  `id`  ,  `lat`  ,  `content`  ,  `lng` ) VALUES ( %u , '%s' , %u , %f , '%s' , %f )" )% uid% location.c_str()% id% lat% content.c_str()% lng;
return db_service.async_execute(sqlfmt.str());
}
int db_Blog_t::sync_insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Blog` ( `uid`  ,  `location`  ,  `id`  ,  `lat`  ,  `content`  ,  `lng` ) VALUES ( %u , '%s' , %u , %f , '%s' , %f )" )% uid% location.c_str()% id% lat% content.c_str()% lng;
return db_service.sync_execute(sqlfmt.str());
}
int db_Team_t::init(sql_result_row_t& row_){
    if (row_.empty())
    {
        return -1;
    }
    size_t i=0;teamname = row_[i++];
uid=(int)std::atoi(row_[i++].c_str());
desp = row_[i++];
gid=(int)std::atoi(row_[i++].c_str());
return 0;
}
int db_Team_t::select_gid(const uint32_t& gid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `teamname` , `uid` , `desp` , `gid` FROM `Team` WHERE  `gid` = %u")% gid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_t::sync_select_gid(const uint32_t& gid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `teamname` , `uid` , `desp` , `gid` FROM `Team` WHERE  `gid` = %u")% gid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_t::update(){

boost::format sqlfmt=boost::format("UPDATE `Team` SET  `teamname` = '%s' ,  `uid` = %u ,  `desp` = '%s' WHERE  `gid` = %u")% teamname.c_str()% uid% desp.c_str()% gid;
return db_service.async_execute(sqlfmt.str());
}
int db_Team_t::sync_update(){

boost::format sqlfmt=boost::format("UPDATE `Team` SET  `teamname` = '%s' ,  `uid` = %u ,  `desp` = '%s' WHERE  `gid` = %u")% teamname.c_str()% uid% desp.c_str()% gid;
return db_service.sync_execute(sqlfmt.str());
}
int db_Team_t::remove(){

boost::format sqlfmt=boost::format("DELETE FROM `Team` WHERE  `gid` = %u")% gid;
return db_service.async_execute(sqlfmt.str());
}
string db_Team_t::gen_insert_sql(){

boost::format sqlfmt=boost::format("INSERT INTO `Team` ( `teamname`  ,  `uid`  ,  `desp`  ,  `gid` ) VALUES ( '%s' , %u , '%s' , %u )" )% teamname.c_str()% uid% desp.c_str()% gid;
return sqlfmt.str();
}
int db_Team_t::insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Team` ( `teamname`  ,  `uid`  ,  `desp`  ,  `gid` ) VALUES ( '%s' , %u , '%s' , %u )" )% teamname.c_str()% uid% desp.c_str()% gid;
return db_service.async_execute(sqlfmt.str());
}
int db_Team_t::sync_insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Team` ( `teamname`  ,  `uid`  ,  `desp`  ,  `gid` ) VALUES ( '%s' , %u , '%s' , %u )" )% teamname.c_str()% uid% desp.c_str()% gid;
return db_service.sync_execute(sqlfmt.str());
}
int db_Team_ntinfo_t::init(sql_result_row_t& row_){
    if (row_.empty())
    {
        return -1;
    }
    size_t i=0;uid=(int)std::atoi(row_[i++].c_str());
id=(int)std::atoi(row_[i++].c_str());
vertify = row_[i++];
fromuid=(int)std::atoi(row_[i++].c_str());
infotype=(int)std::atoi(row_[i++].c_str());
return 0;
}
int db_Team_ntinfo_t::select_id(const uint32_t& id_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `id` , `vertify` , `fromuid` , `infotype` FROM `Team_ntinfo` WHERE  `id` = %u")% id_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_ntinfo_t::select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `id` , `vertify` , `fromuid` , `infotype` FROM `Team_ntinfo` WHERE  `uid` = %u")% uid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_ntinfo_t::sync_select_id(const uint32_t& id_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `id` , `vertify` , `fromuid` , `infotype` FROM `Team_ntinfo` WHERE  `id` = %u")% id_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_ntinfo_t::sync_select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `uid` , `id` , `vertify` , `fromuid` , `infotype` FROM `Team_ntinfo` WHERE  `uid` = %u")% uid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Team_ntinfo_t::update(){

boost::format sqlfmt=boost::format("UPDATE `Team_ntinfo` SET  `uid` = %u ,  `vertify` = '%s' ,  `fromuid` = %u ,  `infotype` = %d WHERE  `id` = %u")% uid% vertify.c_str()% fromuid% infotype% id;
return db_service.async_execute(sqlfmt.str());
}
int db_Team_ntinfo_t::sync_update(){

boost::format sqlfmt=boost::format("UPDATE `Team_ntinfo` SET  `uid` = %u ,  `vertify` = '%s' ,  `fromuid` = %u ,  `infotype` = %d WHERE  `id` = %u")% uid% vertify.c_str()% fromuid% infotype% id;
return db_service.sync_execute(sqlfmt.str());
}
int db_Team_ntinfo_t::remove(){

boost::format sqlfmt=boost::format("DELETE FROM `Team_ntinfo` WHERE  `id` = %u")% id;
return db_service.async_execute(sqlfmt.str());
}
string db_Team_ntinfo_t::gen_insert_sql(){

boost::format sqlfmt=boost::format("INSERT INTO `Team_ntinfo` ( `uid`  ,  `id`  ,  `vertify`  ,  `fromuid`  ,  `infotype` ) VALUES ( %u , %u , '%s' , %u , %d )" )% uid% id% vertify.c_str()% fromuid% infotype;
return sqlfmt.str();
}
int db_Team_ntinfo_t::insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Team_ntinfo` ( `uid`  ,  `id`  ,  `vertify`  ,  `fromuid`  ,  `infotype` ) VALUES ( %u , %u , '%s' , %u , %d )" )% uid% id% vertify.c_str()% fromuid% infotype;
return db_service.async_execute(sqlfmt.str());
}
int db_Team_ntinfo_t::sync_insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Team_ntinfo` ( `uid`  ,  `id`  ,  `vertify`  ,  `fromuid`  ,  `infotype` ) VALUES ( %u , %u , '%s' , %u , %d )" )% uid% id% vertify.c_str()% fromuid% infotype;
return db_service.sync_execute(sqlfmt.str());
}
int db_Account_t::init(sql_result_row_t& row_){
    if (row_.empty())
    {
        return -1;
    }
    size_t i=0;username = row_[i++];
uid=(int)std::atoi(row_[i++].c_str());
sex=(int)std::atoi(row_[i++].c_str());
head = row_[i++];
nickname = row_[i++];
password = row_[i++];
return 0;
}
int db_Account_t::select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `username` , `uid` , `sex` , `head` , `nickname` , `password` FROM `Account` WHERE  `uid` = %u")% uid_;
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Account_t::select_name(const ystring_t<20>& username_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `username` , `uid` , `sex` , `head` , `nickname` , `password` FROM `Account` WHERE  `username` = '%s'")% username_.c_str();
db_service.async_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Account_t::sync_select_uid(const uint32_t& uid_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `username` , `uid` , `sex` , `head` , `nickname` , `password` FROM `Account` WHERE  `uid` = %u")% uid_;
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Account_t::sync_select_name(const ystring_t<20>& username_, sql_result_t &res_){

boost::format sqlfmt=boost::format("SELECT `username` , `uid` , `sex` , `head` , `nickname` , `password` FROM `Account` WHERE  `username` = '%s'")% username_.c_str();
db_service.sync_select(sqlfmt.str(), res_);
        sql_result_row_t* row = res_.get_row_at(0); 
        if (row != NULL)
        {
            return 0;
        }
        return -1;
}
int db_Account_t::update(){

boost::format sqlfmt=boost::format("UPDATE `Account` SET  `sex` = %d ,  `head` = '%s' ,  `nickname` = '%s' ,  `password` = '%s' WHERE  `uid` = %u")% sex% head.c_str()% nickname.c_str()% password.c_str()% uid;
return db_service.async_execute(sqlfmt.str());
}
int db_Account_t::sync_update(){

boost::format sqlfmt=boost::format("UPDATE `Account` SET  `sex` = %d ,  `head` = '%s' ,  `nickname` = '%s' ,  `password` = '%s' WHERE  `uid` = %u")% sex% head.c_str()% nickname.c_str()% password.c_str()% uid;
return db_service.sync_execute(sqlfmt.str());
}
int db_Account_t::remove(){

boost::format sqlfmt=boost::format("DELETE FROM `Account` WHERE  `uid` = %u")% uid;
return db_service.async_execute(sqlfmt.str());
}
string db_Account_t::gen_insert_sql(){

boost::format sqlfmt=boost::format("INSERT INTO `Account` ( `username`  ,  `uid`  ,  `sex`  ,  `head`  ,  `nickname`  ,  `password` ) VALUES ( '%s' , %u , %d , '%s' , '%s' , '%s' )" )% username.c_str()% uid% sex% head.c_str()% nickname.c_str()% password.c_str();
return sqlfmt.str();
}
int db_Account_t::insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Account` ( `username`  ,  `uid`  ,  `sex`  ,  `head`  ,  `nickname`  ,  `password` ) VALUES ( '%s' , %u , %d , '%s' , '%s' , '%s' )" )% username.c_str()% uid% sex% head.c_str()% nickname.c_str()% password.c_str();
return db_service.async_execute(sqlfmt.str());
}
int db_Account_t::sync_insert(){

boost::format sqlfmt=boost::format("INSERT INTO `Account` ( `username`  ,  `uid`  ,  `sex`  ,  `head`  ,  `nickname`  ,  `password` ) VALUES ( '%s' , %u , %d , '%s' , '%s' , '%s' )" )% username.c_str()% uid% sex% head.c_str()% nickname.c_str()% password.c_str();
return db_service.sync_execute(sqlfmt.str());
}
