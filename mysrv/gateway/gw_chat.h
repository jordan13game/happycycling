#ifndef  _GW_CHAT_H__INC
#define  _GW_CHAT_H__INC

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "../../lib/utility/singleton.h"
#include "../../lib/rpc/rpc_connecter.h"
#include "../common/msg_def.h"

class gw_chat_t:private boost::noncopyable
{
public:
    typedef shared_ptr<rpc_connecter_t> sp_rpc_conn_t;
public:
    void post(uint32_t userid, gw_msg_def::req_send_message_t& jpk_);
    void check_message(uint32_t userid);
};


#define gw_chat singleton_t<gw_chat_t>::instance()

#endif   /* ----- #ifndef _GW_CHAT_H__INC  ----- */
