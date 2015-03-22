#ifndef _gw_public_service_h_
#define _gw_public_service_h_

#include "../common/service_base.h"
#include "../../lib/utility/singleton.h"
#include "../common/msg_def.h"
#include "../common/remote_info.h"
#include "gw_session.h"
#include "gw_user.h"

class gw_public_handler_t : public rpc_t 
{
    typedef boost::shared_ptr<rpc_connecter_t>          sp_rpc_conn_t;
public:
    gw_public_handler_t(io_t& io_);
    ~gw_public_handler_t();
protected:
    //bool on_decrypt(sp_rpc_conn_t conn_, uint16_t cmd_, uint16_t res_, string& body_); 
private:
    //收到未绑定的消息
    int on_unknown_msg(uint16_t cmd_, const string& body_, sp_rpc_conn_t conn_);
    //客户端断开
    void on_broken(sp_rpc_conn_t conn_);
    //客户端请求登陆网关
    void on_login(sp_rpc_conn_t conn_, gw_msg_def::req_login_t& jpk_);
    //客户端发来的心跳消息
    void on_req_heartbeat(sp_rpc_conn_t conn_, gw_msg_def::req_heartbeat_t& jpk_);
    void on_reg(sp_rpc_conn_t conn_, gw_msg_def::req_reg_t& jpk_);
    void on_update_pos(sp_rpc_conn_t conn_, gw_msg_def::req_update_pos_t& jpk_);
    void on_req_rolelist(sp_rpc_conn_t conn_, gw_msg_def::req_rolelist_t& jpk_);
    void on_req_edit_roleinfo(sp_rpc_conn_t conn_, gw_msg_def::req_edit_userinfo_t& jpk_);
    void on_req_roleinfo(sp_rpc_conn_t conn_, gw_msg_def::req_userinfo_t& jpk_);
    void on_req_send_msg(sp_rpc_conn_t conn_, gw_msg_def::req_send_message_t& jpk_);
    void on_ret_recv_msg(sp_rpc_conn_t conn_, gw_msg_def::ret_recv_message_t& jpk_);
    void on_req_create_blog(sp_rpc_conn_t conn_, blog_msg_def::req_create_blog_t& jpk_);
    void on_req_blog(sp_rpc_conn_t conn_, blog_msg_def::req_blog_t& jpk_);
};


class gw_public_service_t : public service_base_t<gw_public_handler_t>
{
public:
    //[seskey][sid]
    typedef hash_map<uint64_t, uint32_t> trans_hm_t;
    //[uid][seskey]
    typedef hash_map<int32_t, uint64_t> user_ses_hm_t;
public:
    gw_public_service_t();

    void start(uint16_t serid_, string ip_, string port_);

    void get_rolelist(remote_info_t* remote, vector<gw_msg_def::req_type_t> &list, gw_msg_def::ret_rolelist_t& jpk_);
    //增删查，转发表
    //用来保存当前seskey对应的转发服务器id
    //在断线重连的情况下，可以根据seskey找到转发，重新设置链接信息

    //保存有效的seskey
    void add_valid_seskey(uint64_t seskey_);

    gw_session_t session;
    user_mgr_t          m_user_mgr;
private:
    bool                m_started;
    trans_hm_t          m_trans_hm;
    user_ses_hm_t       m_user_ses_hm;
};

#define gw_public_service (singleton_t<gw_public_service_t>::instance())

#endif
