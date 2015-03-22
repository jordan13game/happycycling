#include <math.h>

#include "gw_user.h"

gw_user_t::gw_user_t():
    longitude(0),
    latitude(0),
    team(0),
    isleader(false),
    sex(0)
{
}

double gw_user_t::get_dist( sp_user_t user_)
{
#define rad(x) ((x) * 3.1415926 / 180.0)
    double radLat1 = rad(latitude);
    double radLat2 = rad(user_->latitude);
    double a = radLat1 - radLat2;
    double b = rad(longitude) - rad(user_->longitude);

    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1) * cos(radLat2) * pow(sin(b/2),2)));
    s = s * 63781370;
    return s;
}

////////////////////////////////////////////////////
user_mgr_t::user_mgr_t()
{
}
bool user_mgr_t::get(const int32_t& uid_, sp_user_t& sp_user_)
{
    auto it = m_user_hm.find(uid_);
    if (it != m_user_hm.end())
    {
        sp_user_ = it->second;
        return true;
    }
    return false;
}


