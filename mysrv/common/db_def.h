#ifndef _db_def_h_
#define _db_def_h_
#include <stdint.h>
#include <sstream>
using namespace std;
#include "sql_result.h"
#include "db_helper.h"
#include "db_service.h"
#include "ystring.h"
struct db_Team_User_t {
uint32_t gid;
uint32_t uid;
static const char* tablename(){ return "Team_User"; }
int init(sql_result_row_t& row_);
static int select_gid(const uint32_t& gid_, sql_result_t &res_);
static int select_uid(const uint32_t& uid_, sql_result_t &res_);
static int sync_select_gid(const uint32_t& gid_, sql_result_t &res_);
static int sync_select_uid(const uint32_t& uid_, sql_result_t &res_);
int update();
int sync_update();
int remove();
int insert();
string gen_insert_sql();
int sync_insert();
};
struct db_Chat_log_t {
ystring_t<20> stime;
uint32_t uid;
ystring_t<600000> content;
uint16_t isteam;
static const char* tablename(){ return "Chat_log"; }
int init(sql_result_row_t& row_);
static int select_uid_team(const uint32_t& uid_, const uint16_t& isteam_, sql_result_t &res_);
static int select_uid(const uint32_t& uid_, sql_result_t &res_);
static int sync_select_uid_team(const uint32_t& uid_, const uint16_t& isteam_, sql_result_t &res_);
static int sync_select_uid(const uint32_t& uid_, sql_result_t &res_);
int update();
int sync_update();
int remove();
int insert();
string gen_insert_sql();
int sync_insert();
};
struct db_Blog_t {
uint32_t uid;
ystring_t<100> location;
uint32_t id;
float lat;
ystring_t<600000> content;
float lng;
static const char* tablename(){ return "Blog"; }
int init(sql_result_row_t& row_);
static int select_id(const uint32_t& id_, sql_result_t &res_);
static int select_uid(const uint32_t& uid_, sql_result_t &res_);
static int sync_select_id(const uint32_t& id_, sql_result_t &res_);
static int sync_select_uid(const uint32_t& uid_, sql_result_t &res_);
int update();
int sync_update();
int remove();
int insert();
string gen_insert_sql();
int sync_insert();
};
struct db_Team_t {
ystring_t<20> teamname;
uint32_t uid;
ystring_t<1000> desp;
uint32_t gid;
static const char* tablename(){ return "Team"; }
int init(sql_result_row_t& row_);
static int select_gid(const uint32_t& gid_, sql_result_t &res_);
static int sync_select_gid(const uint32_t& gid_, sql_result_t &res_);
int update();
int sync_update();
int remove();
int insert();
string gen_insert_sql();
int sync_insert();
};
struct db_Team_ntinfo_t {
uint32_t uid;
uint32_t id;
ystring_t<400> vertify;
uint32_t fromuid;
uint16_t infotype;
static const char* tablename(){ return "Team_ntinfo"; }
int init(sql_result_row_t& row_);
static int select_id(const uint32_t& id_, sql_result_t &res_);
static int select_uid(const uint32_t& uid_, sql_result_t &res_);
static int sync_select_id(const uint32_t& id_, sql_result_t &res_);
static int sync_select_uid(const uint32_t& uid_, sql_result_t &res_);
int update();
int sync_update();
int remove();
int insert();
string gen_insert_sql();
int sync_insert();
};
struct db_Account_t {
ystring_t<20> username;
uint32_t uid;
uint16_t sex;
ystring_t<600000> head;
ystring_t<20> nickname;
ystring_t<20> password;
static const char* tablename(){ return "Account"; }
int init(sql_result_row_t& row_);
static int select_uid(const uint32_t& uid_, sql_result_t &res_);
static int select_name(const ystring_t<20>& username_, sql_result_t &res_);
static int sync_select_uid(const uint32_t& uid_, sql_result_t &res_);
static int sync_select_name(const ystring_t<20>& username_, sql_result_t &res_);
int update();
int sync_update();
int remove();
int insert();
string gen_insert_sql();
int sync_insert();
};
#endif
