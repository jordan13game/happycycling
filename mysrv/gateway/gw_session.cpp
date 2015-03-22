#include "gw_session.h"
#include "log.h"
#include "msg_def.h"
#include "remote_info.h"

using namespace std;

#define LOG "GW_SESSION"

void gw_session_t::add(uint64_t seskey_, int32_t uid_)
{
    m_session_hm[seskey_] = uid_;
    m_user_hm[uid_] = seskey_;
}
void gw_session_t::update(uint64_t seskey_, uint64_t newseskey_)
{
    auto it = m_session_hm.find(seskey_);
    if (it != m_session_hm.end())
    {
        m_session_hm.insert(make_pair(newseskey_, it->second));
        auto it2 = m_user_hm.find(it->second);
        it2->second = newseskey_;
        m_session_hm.erase(it);
    }
}
int32_t gw_session_t::get_uid(uint64_t seskey_)
{
    auto it = m_session_hm.find(seskey_);
    if (it != m_session_hm.end())
        return it->second;
    return 0;
}
uint64_t gw_session_t::get_seskey(int32_t uid_)
{
    auto it = m_user_hm.find(uid_);
    if (it != m_user_hm.end())
        return it->second;
    return 0;
}
void gw_session_t::clear()
{
    m_session_hm.clear();
    m_user_hm.clear();
}
void gw_session_t::dump()
{
    for(auto it=m_session_hm.begin(); it!=m_session_hm.end(); it++)
    {
        logwarn((LOG, "ses:%lu,uid:%d", it->first, it->second));
    }
}
int gw_session_t::on_broken(uint64_t seskey_)
{
    auto it = m_session_hm.find(seskey_);
    if (it != m_session_hm.end())
    {
        m_user_hm.erase(it->second);
        m_session_hm.erase(it);
        return 0;
    }
    return -1;
}
