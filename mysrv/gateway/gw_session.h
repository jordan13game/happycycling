#ifndef _gw_session_h_
#define _gw_session_h_

#include <unordered_map>

#include "singleton.h"

class gw_session_t
{

    typedef std::unordered_map<uint64_t, int32_t> ses_uid_t;
    typedef std::unordered_map<int32_t, uint64_t> uid_ses_t;
public:
    void add(uint64_t seskey_, int32_t uid_);
    void update(uint64_t seskey_, uint64_t newseskey_);
    int32_t get_uid(uint64_t seskey_); 
    uint64_t get_seskey(int32_t uid_);
    int on_broken(uint64_t seskey_);
    void clear();
    void dump();
private:
    ses_uid_t m_session_hm;
    uid_ses_t m_user_hm;
};


#endif
