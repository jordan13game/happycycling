#ifndef _db_ext_h_
#define _db_ext_h_
#include "db_def.h"
#include "db_smart_up.h"
#include <boost/noncopyable.hpp>
class db_Team_User_ext_t: public db_Team_User_t, db_smart_up_t, boost::noncopyable
{
public:
db_Team_User_ext_t()
{
REG_MEM(uint32_t,gid)
}
DEC_SET(uint32_t,gid)
string get_up_sql()
{
if (!has_changed()) return "";

boost::format sqlfmt=boost::format("UPDATE `Team_User` SET %%1%% WHERE  `uid` = %u")% uid;
return gen_up_sql(sqlfmt.str());
}
db_Team_User_t& data()
{
return *((db_Team_User_t*)(this));
}
};
class db_Chat_log_ext_t: public db_Chat_log_t, db_smart_up_t, boost::noncopyable
{
public:
db_Chat_log_ext_t()
{
REG_MEM(ystring_t<600000>,content)
REG_MEM(uint16_t,isteam)
}
DEC_SET(ystring_t<600000>,content)
DEC_SET(uint16_t,isteam)
string get_up_sql()
{
if (!has_changed()) return "";

boost::format sqlfmt=boost::format("UPDATE `Chat_log` SET %%1%% WHERE  `uid` = %u AND  `stime` = '%s'")% uid% stime.c_str();
return gen_up_sql(sqlfmt.str());
}
db_Chat_log_t& data()
{
return *((db_Chat_log_t*)(this));
}
};
class db_Blog_ext_t: public db_Blog_t, db_smart_up_t, boost::noncopyable
{
public:
db_Blog_ext_t()
{
REG_MEM(uint32_t,uid)
REG_MEM(ystring_t<100>,location)
REG_MEM(float,lat)
REG_MEM(ystring_t<600000>,content)
REG_MEM(float,lng)
}
DEC_SET(uint32_t,uid)
DEC_SET(ystring_t<100>,location)
DEC_SET(float,lat)
DEC_SET(ystring_t<600000>,content)
DEC_SET(float,lng)
string get_up_sql()
{
if (!has_changed()) return "";

boost::format sqlfmt=boost::format("UPDATE `Blog` SET %%1%% WHERE  `id` = %u")% id;
return gen_up_sql(sqlfmt.str());
}
db_Blog_t& data()
{
return *((db_Blog_t*)(this));
}
};
class db_Team_ext_t: public db_Team_t, db_smart_up_t, boost::noncopyable
{
public:
db_Team_ext_t()
{
REG_MEM(ystring_t<20>,teamname)
REG_MEM(uint32_t,uid)
REG_MEM(ystring_t<1000>,desp)
}
DEC_SET(ystring_t<20>,teamname)
DEC_SET(uint32_t,uid)
DEC_SET(ystring_t<1000>,desp)
string get_up_sql()
{
if (!has_changed()) return "";

boost::format sqlfmt=boost::format("UPDATE `Team` SET %%1%% WHERE  `gid` = %u")% gid;
return gen_up_sql(sqlfmt.str());
}
db_Team_t& data()
{
return *((db_Team_t*)(this));
}
};
class db_Team_ntinfo_ext_t: public db_Team_ntinfo_t, db_smart_up_t, boost::noncopyable
{
public:
db_Team_ntinfo_ext_t()
{
REG_MEM(uint32_t,uid)
REG_MEM(ystring_t<400>,vertify)
REG_MEM(uint32_t,fromuid)
REG_MEM(uint16_t,infotype)
}
DEC_SET(uint32_t,uid)
DEC_SET(ystring_t<400>,vertify)
DEC_SET(uint32_t,fromuid)
DEC_SET(uint16_t,infotype)
string get_up_sql()
{
if (!has_changed()) return "";

boost::format sqlfmt=boost::format("UPDATE `Team_ntinfo` SET %%1%% WHERE  `id` = %u")% id;
return gen_up_sql(sqlfmt.str());
}
db_Team_ntinfo_t& data()
{
return *((db_Team_ntinfo_t*)(this));
}
};
class db_Account_ext_t: public db_Account_t, db_smart_up_t, boost::noncopyable
{
public:
db_Account_ext_t()
{
REG_MEM(uint16_t,sex)
REG_MEM(ystring_t<600000>,head)
REG_MEM(ystring_t<20>,nickname)
REG_MEM(ystring_t<20>,password)
}
DEC_SET(uint16_t,sex)
DEC_SET(ystring_t<600000>,head)
DEC_SET(ystring_t<20>,nickname)
DEC_SET(ystring_t<20>,password)
string get_up_sql()
{
if (!has_changed()) return "";

boost::format sqlfmt=boost::format("UPDATE `Account` SET %%1%% WHERE  `uid` = %u")% uid;
return gen_up_sql(sqlfmt.str());
}
db_Account_t& data()
{
return *((db_Account_t*)(this));
}
};
#endif
