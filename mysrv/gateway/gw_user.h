#ifndef  _GW_USER_H_INC
#define  _GW_USER_H_INC

#include <boost/shared_ptr.hpp>
#include <unordered_map>

using namespace std;

class gw_user_t
{
    typedef boost::shared_ptr<gw_user_t> sp_user_t;

public:
    double longitude;
    double latitude;
    uint32_t team;
    bool isleader;
    uint16_t sex;

public:
    gw_user_t();
    double get_dist(sp_user_t user_);
};

typedef boost::shared_ptr<gw_user_t> sp_user_t;

class user_mgr_t
{
    typedef std::unordered_map<uint32_t, sp_user_t> user_hm_t;
public:
    user_mgr_t();
    bool get(const int32_t& uid_, sp_user_t& sp_user_);
    void insert(const int32_t& uid_, const sp_user_t& sp_user_) { m_user_hm.insert(make_pair(uid_, sp_user_)); }
    void erase(const int32_t& uid_) { m_user_hm.erase(uid_); }
    template<class F>
    void foreach(F fun_)
    {
        for(auto it=m_user_hm.begin();it!=m_user_hm.end();++it)
        {
            fun_(it->second);
        }
    }
private:
    user_hm_t m_user_hm;
};
#endif   /* ----- #ifndef _GW_USER_H_INC  ----- */
