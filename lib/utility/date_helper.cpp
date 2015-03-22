#include "date_helper.h"

#include <string.h>
#include <sys/time.h>
#include <boost/format.hpp>

#define SECONDS_DAY 86400
#define SECONDS_8_HOUR 28800

int date_helper_t::offday(uint32_t tv_sec_)
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return ( ( (tm.tv_sec+SECONDS_8_HOUR)/SECONDS_DAY )-( (tv_sec_+SECONDS_8_HOUR)/SECONDS_DAY ) );
}


int date_helper_t::offday(uint32_t tv_sec_b, uint32_t tv_sec_e)
{
    return ( ( (tv_sec_e+SECONDS_8_HOUR)/SECONDS_DAY )-( (tv_sec_b+SECONDS_8_HOUR)/SECONDS_DAY ) );
}
int date_helper_t::offsec(uint32_t tv_sec)
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return ( (tm.tv_sec - tv_sec));
}
uint32_t date_helper_t::cur_sec()
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return tm.tv_sec;
}
uint32_t date_helper_t::cur_local_sec()
{
    return cur_sec()+SECONDS_8_HOUR;
}
int date_helper_t::offmonth(uint32_t stamp_)
{
	time_t timep = time_t(stamp_);
	struct tm tmp;
    localtime_r(&timep,&tmp);

    int cur_y = cur_year();
    int cur_m = cur_month();
    int last_y = (tmp.tm_year + 1900);
    int last_m = (tmp.tm_mon + 1);
    return cur_y*12+cur_m-last_y*12-last_m;
}

string date_helper_t::str_date(uint32_t tv_)
{
    struct tm tmp;
	localtime_r((const time_t*)&tv_,&tmp);
    
    char buf[128];
    sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",tmp.tm_year + 1900,tmp.tm_mon + 1,tmp.tm_mday,tmp.tm_hour,tmp.tm_min,tmp.tm_sec);
    string str(buf);
    return str;
}

string date_helper_t::str_date()
{
	time_t timep = time(NULL);
    struct tm tmp;
	localtime_r(&timep,&tmp);
    
    char buf[128];
    sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",tmp.tm_year + 1900,tmp.tm_mon + 1,tmp.tm_mday,tmp.tm_hour,tmp.tm_min,tmp.tm_sec);
    string str(buf);
    return str;

}

string date_helper_t::str_date_only()
{
	time_t timep = time(NULL);
    struct tm tmp;
	localtime_r(&timep,&tmp);
    
    char buf[128];
    sprintf(buf,"%04d%02d%02d",tmp.tm_year + 1900,tmp.tm_mon + 1,tmp.tm_mday);
    string str(buf);
    return str;
}

string date_helper_t::str_date_mysql(uint32_t time_)
{
    time_t timep;

    if( 0==time_ )
	    timep = time(NULL);
    else
        timep = time_t(time_);

    struct tm tmp;
	localtime_r(&timep,&tmp);

    char buf[128];
    sprintf(buf,"%04d-%02d-%02d %02d:%02d:%02d",tmp.tm_year + 1900,tmp.tm_mon + 1,tmp.tm_mday,tmp.tm_hour,tmp.tm_min,tmp.tm_sec);
    string str(buf);
    return str;
}
string date_helper_t::str_dayofweek(int day_)
{
    day_ %= 7;

    switch(day_)
    {
    case 0:
        return "日";
    case 1:
        return "一"; 
    case 2:
        return "二"; 
    case 3:
        return "三"; 
    case 4:
        return "四"; 
    case 5:
        return "五"; 
    case 6:
        return "六"; 
    }
    return "";
}
int date_helper_t::cur_dayofweek()
{
	time_t timep = time(NULL);
	struct tm *tmp = localtime(&timep);
    char buf[8];
    strftime(buf, sizeof(buf), "%w", tmp);
    return atoi(buf);
}
int date_helper_t::cur_month()
{
	time_t timep = time(NULL);
	struct tm *tmp = localtime(&timep);
    char buf[8];
    strftime(buf, sizeof(buf), "%m", tmp);
    return atoi(buf);
}
int date_helper_t::cur_year()
{
	time_t timep = time(NULL);
	struct tm *tmp = localtime(&timep);
    char buf[8];
    strftime(buf, sizeof(buf), "%Y", tmp);
    return atoi(buf);
}
int date_helper_t::cur_dayofmonth()
{
	time_t timep = time(NULL);
	struct tm *tmp = localtime(&timep);
    char buf[8];
    strftime(buf, sizeof(buf), "%d", tmp);
    return atoi(buf);
}
int date_helper_t::same_month(uint32_t tv_sec)
{
    if( (cur_sec()-tv_sec) > 50*3600*24 )
        return 0;

    char buf1[8],buf2[8];

    time_t timep = time(NULL);
	struct tm *tmp = localtime(&timep);
    strftime(buf1, sizeof(buf1), "%b", tmp);

    timep = (time_t)tv_sec;
    tmp = localtime(&timep);
    strftime(buf2, sizeof(buf2), "%b", tmp);

    return !(strcmp(buf1,buf2));
}
uint32_t date_helper_t::secoffday()
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    return (tm.tv_sec+SECONDS_8_HOUR)%SECONDS_DAY;
}
uint32_t date_helper_t::secoffday(uint32_t sec_)
{
    return (sec_+SECONDS_8_HOUR)%SECONDS_DAY;
}
int date_helper_t::get_day_sec(int day_)
{
    return SECONDS_DAY * day_;
}
uint32_t date_helper_t::countdown(uint32_t start_stamp_, uint32_t offday_)
{
    //当前调整开始时间戳到0时,并调整到+8时区
    start_stamp_ = start_stamp_ - start_stamp_ % SECONDS_DAY - SECONDS_8_HOUR;
    //计算剩余时间
    return (SECONDS_DAY * offday_ - offsec(start_stamp_) % (SECONDS_DAY * offday_)); 
}
int date_helper_t::sec_2_tomorrow(int32_t cur_)
{

    if( 0==cur_ )
        return SECONDS_DAY - ( cur_sec() + SECONDS_8_HOUR )%SECONDS_DAY;
    else
        return SECONDS_DAY - ( cur_ + SECONDS_8_HOUR )%SECONDS_DAY;
}
//获得今日凌晨十二点的时间戳
int date_helper_t::cur_0_stmp()
{
    int32_t cur = cur_sec();
    return (cur - (cur + SECONDS_8_HOUR)%SECONDS_DAY);
}
uint32_t date_helper_t::trans_unixstamp(uint32_t date_)
{
    time_t t;
    struct tm tmp;
    tmp.tm_year = date_ / 10000 - 1900;
    tmp.tm_mon = date_ % 10000 / 100 - 1;
    tmp.tm_mday = date_ % 100;
    tmp.tm_hour = 0; 
    tmp.tm_min = 0; 
    tmp.tm_sec = 0;
    t = mktime(&tmp);
    return t;
}
