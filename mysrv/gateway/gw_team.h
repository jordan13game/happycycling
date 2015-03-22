#ifndef  _GW_TEAM_H_INC
#define  _GW_TEAM_H_INC

#include <boost/shared_ptr.hpp>

#include "singleton.h"
#include "msg_def.h"
#include "msg_dispatcher.h"

class gw_team_t : public msg_dispatcher_t<gw_team_t>
{
public:
    gw_team_t();
    ~gw_team_t();

    void check_lost(uint32_t userid);
    void check_nt_info(uint32_t userid);

    void nt_info(uint32_t userid, uint16_t type, uint32_t fromuid);
private:

private:
    void on_req_create_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_create_team_t& jpk_);
    void on_req_edit_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_edit_team_t& jpk_);
    void on_req_add_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_add_team_t& jpk_);
    void on_ret_ntinfo(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::ret_msg_t& jpk_);
    void on_ret_nt_add_team_user(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::ret_nt_add_team_user_t& jpk_);
    void on_ret_nt_add_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::ret_nt_add_team_t& jpk_);
    void on_req_exit_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_exit_team_t& jpk_);
    void on_req_kick_user(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_kick_t& jpk_);
    void on_req_team_member(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_team_member_t& jpk_);
    void on_req_team_info(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_team_info_t& jpk_);
    void on_req_sos(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_sos_t& jpk_);
};

#define gw_team singleton_t<gw_team_t>::instance()

#endif   /* ----- #ifndef _GW_TEAM_H_INC  ----- */
