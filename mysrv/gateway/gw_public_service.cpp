#include "gw_public_service.h"
#include "log.h"
#include "code_def.h"
#include "seskey_assign.h"
#include "db_service.h"
#include "dbgl_service.h"
#include "db_ext.h"
#include "db_def.h"
#include "remote_info.h"
#include "gw_chat.h"
#include "gw_team.h"
#include <boost/lexical_cast.hpp>
//#include "../common/config.h"


#define LOG "GW_PUBLIC" 
#define MAX_PLAIN_LEN 8192

//#define RECORD_SCENE_MSG

gw_public_handler_t::gw_public_handler_t(io_t& io_):rpc_t(io_)
{
    reg_call(&gw_public_handler_t::on_login);
    reg_call(&gw_public_handler_t::on_req_heartbeat);
//    reg_call(&gw_public_handler_t::on_req_rolelist);
    reg_call(&gw_public_handler_t::on_reg);
    reg_call(&gw_public_handler_t::on_update_pos);
    reg_call(&gw_public_handler_t::on_req_rolelist);
    reg_call(&gw_public_handler_t::on_req_roleinfo);
    reg_call(&gw_public_handler_t::on_req_edit_roleinfo);
    reg_call(&gw_public_handler_t::on_req_send_msg);
    reg_call(&gw_public_handler_t::on_ret_recv_msg);
    reg_call(&gw_public_handler_t::on_req_create_blog);
    reg_call(&gw_public_handler_t::on_req_blog);
}
gw_public_handler_t::~gw_public_handler_t()
{
}
void gw_public_handler_t::on_broken(sp_rpc_conn_t conn_)
{
    logwarn((LOG, "gw_public_handler_t::on_broken..."));
    remote_info_t* info = conn_->get_data<remote_info_t>();
    if (info == NULL)
        return;
    logwarn((LOG, "client broken! seskey:%lu", info->remote_id));

    uint64_t seskey_ = info->remote_id;
    uint32_t userid = gw_public_service.session.get_uid(seskey_);
    logdebug((LOG, "2"));
    gw_public_service.m_user_mgr.erase(userid);
    logdebug((LOG, "3"));
    gw_public_service.session.on_broken(seskey_);
    logdebug((LOG, "1"));
    gw_public_service.del_client(seskey_);
    logwarn((LOG, "gw_public_handler_t::on_broken...ok"));
}
int gw_public_handler_t::on_unknown_msg(uint16_t cmd_, const string& body_, sp_rpc_conn_t conn_)
{
    logwarn((LOG, "gw_public_handler_t::on_unknown_msg..."));
    remote_info_t* info = conn_->get_data<remote_info_t>();
    if (info == NULL)
        return -1;

    if ( gw_team.handle_msg(info->remote_id, cmd_, body_, conn_) == 0 ) {
        return 0;
    }
    gw_public_service.del_client(info->remote_id);
    uint32_t userid = gw_public_service.session.get_uid(info->remote_id);
    gw_public_service.m_user_mgr.erase(userid);
    gw_public_service.session.on_broken(info->remote_id);
    logwarn((LOG, "gw_public_handler_t::on_unknown_msg...ok"));
    return -1;
}
void gw_public_handler_t::on_login(sp_rpc_conn_t conn_, gw_msg_def::req_login_t& jpk_)
{
    logwarn((LOG, "client login... "));

    db_service.async_do([](sp_rpc_conn_t conn_, gw_msg_def::req_login_t& jpk_)
    {
        gw_msg_def::ret_login_t ret;

        sql_result_t res;
        db_Account_ext_t::select_name(jpk_.account, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }

        db_Account_ext_t acc;
        acc.init(*res.get_row_at(0));

        if ( acc.password() != jpk_.password ) {
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }

        ret.head = acc.head();
        ret.sex = acc.sex;
        ret.nickname = acc.nickname();
        ret.userid = acc.uid;
        ret.session = singleton_t<seskey_assign_t>::instance().new_seskey(123);
        
        db_Team_User_ext_t::select_uid(acc.uid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.team = 0;
            ret.isleader = 0;
        } else {
            db_Team_User_ext_t tmp;
            tmp.init(*res.get_row_at(0));
            ret.team = tmp.gid;
            db_Team_ext_t team;
            db_Team_ext_t::select_gid(tmp.gid, res);
            team.init(*res.get_row_at(0));
            if ( team.uid == ret.userid ) {
                ret.isleader = 1;
            } else {
                ret.isleader = 0;
            }

        }

        remote_info_t* remote = new remote_info_t;
        remote->is_client = true;
        remote->remote_id = ret.session;
        remote->verified = true;

        conn_->set_data(remote);

        uint64_t ses = gw_public_service.session.get_seskey(acc.uid);
        if ( ses != 0 ) {
            gw_public_service.del_client(ses);
            gw_public_service.session.update(ses, ret.session);
        }
        else {
            gw_public_service.session.add(ret.session, acc.uid);
            sp_user_t user = boost::shared_ptr<gw_user_t>(new gw_user_t);
            user->sex = acc.sex;
            logdebug((LOG, "add user %u", acc.uid));
            gw_public_service.m_user_mgr.insert(acc.uid, user);
        }

        if ( false == gw_public_service.add_client(ret.session, conn_) ) {
            logwarn((LOG, "client login but online! seskey:%lu", ret.session));
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }

        ret.code = SUCCESS;
        async_call(conn_, ret);
        gw_chat.check_message(ret.userid);
        gw_team.check_nt_info(ret.userid);
    }, conn_, jpk_);

//    //检查seskey
//    uint16_t serid = jpk_.seskey>>48;
//    uint32_t stamp = jpk_.seskey>>16;
//
}
void gw_public_handler_t::on_req_heartbeat(sp_rpc_conn_t conn_, gw_msg_def::req_heartbeat_t& jpk_)
{
    remote_info_t* info = conn_->get_data<remote_info_t>();
    if (info == NULL || info->remote_id != jpk_.session)
    {
        logerror((LOG, "on_nt_heartbeat...unkonw conn! session:%lu", jpk_.session));
        conn_->close(RPC_SHUTDOWN);
        return ;
    }

    gw_msg_def::ret_heartbeat_t ret;
    ret.code = SUCCESS;
    async_call(conn_, ret);
}
void gw_public_handler_t::on_reg(sp_rpc_conn_t conn_, gw_msg_def::req_reg_t& jpk_)
{

    db_service.async_do( [](sp_rpc_conn_t conn_, gw_msg_def::req_reg_t& jpk_)
    {
        gw_msg_def::ret_reg_t ret;
        ret.code = SUCCESS;

        sql_result_t res;

        db_Account_ext_t::select_name(jpk_.account, res);
        if( res.affect_row_num() > 0)
        {
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }

        db_Account_ext_t acc;
        if ((acc.uid = dbid_assign.new_dbid("uid")) == 0)
        {
            logerror((LOG, "on_req_reg, create new uid failed!"));
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }
        acc.username = jpk_.account;
        acc.password = jpk_.password;
        acc.nickname = jpk_.nickname;
        acc.sex = jpk_.sex;
        acc.head = jpk_.head;

        if( acc.insert() )
        {
            logerror((LOG, "on_req_reg, create new role failed!"));
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }
        async_call(conn_, ret);
        
    }, conn_, jpk_);
}
void gw_public_handler_t::on_update_pos(sp_rpc_conn_t conn_, gw_msg_def::req_update_pos_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_update_pos, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }
    
    gw_msg_def::ret_update_pos_t ret;

    sp_user_t user = nullptr;
    uint32_t userid = gw_public_service.session.get_uid(remote->remote_id);
    gw_public_service.m_user_mgr.get(userid, user);
    if(user == nullptr)
    {
        logerror((LOG, "client update pos, has no user %u", userid));
        ret.code = FAILED;
        async_call(conn_, ret);
        return ;
    }
    logwarn((LOG, "client update pos, session=%lu, pos from %f, %f to %f, %f", remote->remote_id, user->longitude, user->latitude, jpk_.longitude, jpk_.latitude));
    user->longitude = jpk_.longitude;
    user->latitude = jpk_.latitude;
    gw_team.check_lost(userid);
    ret.code = SUCCESS;
    async_call(conn_, ret);
}
void gw_public_handler_t::on_req_rolelist(sp_rpc_conn_t conn_, gw_msg_def::req_rolelist_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_rolelist, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }
    
    gw_msg_def::ret_rolelist_t ret;
    ret.code = SUCCESS;
    gw_public_service.get_rolelist(remote, jpk_.list, ret);
}
void gw_public_handler_t::on_req_send_msg(sp_rpc_conn_t conn_, gw_msg_def::req_send_message_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_send_msg, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }

    uint32_t userid = gw_public_service.session.get_uid(remote->remote_id);

    gw_msg_def::ret_send_message_t ret;
    if ( userid != 0 ) {
        ret.code = SUCCESS;
        gw_chat.post(userid, jpk_);
    } else {
        ret.code = FAILED;
    }

    ret.datetime = jpk_.datetime;
    async_call(conn_, ret);

}
void gw_public_handler_t::on_ret_recv_msg(sp_rpc_conn_t conn_, gw_msg_def::ret_recv_message_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_send_msg, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }

    uint32_t userid = gw_public_service.session.get_uid(remote->remote_id);

    db_service.async_do((uint64_t)userid, [](uint32_t userid, gw_msg_def::ret_recv_message_t& jpk_)
    {
        db_Chat_log_ext_t chat;
        chat.uid = userid;
        chat.stime = jpk_.datetime;
        if ( chat.remove() ) {
            logerror((LOG, "fail to del chat_log userid=%u, stime=%s", userid, jpk_.datetime.c_str()));
            return ;
        }
    }, userid, jpk_);
}

void gw_public_handler_t::on_req_roleinfo(sp_rpc_conn_t conn_, gw_msg_def::req_userinfo_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_roleinfo, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }


    db_service.async_do([](sp_rpc_conn_t conn_, gw_msg_def::req_userinfo_t& jpk_)
    {
        gw_msg_def::ret_userinfo_t ret;

        sql_result_t res;
        db_Account_ext_t::select_uid(jpk_.userid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = FAILED;
            async_call(conn_, ret);
            return ;
        }

        db_Account_ext_t acc;
        acc.init(*res.get_row_at(0));

        ret.head = acc.head();
        ret.sex = acc.sex;
        ret.nickname = acc.nickname();
        
        db_Team_User_ext_t::select_uid(acc.uid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.team = 0;
        } else {
            db_Team_User_ext_t tmp;
            tmp.init(*res.get_row_at(0));
            ret.team = tmp.gid;
        }

        ret.code = SUCCESS;
        async_call(conn_, ret);
    }, conn_, jpk_);
}

void gw_public_handler_t::on_req_edit_roleinfo(sp_rpc_conn_t conn_, gw_msg_def::req_edit_userinfo_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_edit_roleinfo, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }

    uint32_t userid = gw_public_service.session.get_uid(remote->remote_id);

    db_service.async_do(remote->remote_id, [](sp_rpc_conn_t conn_, gw_msg_def::req_edit_userinfo_t& jpk_, uint32_t userid)
    {
        gw_msg_def::ret_edit_userinfo_t ret;

        sql_result_t res;

        db_Account_ext_t::select_uid(userid, res);
        if ( res.affect_row_num() == 0 ) {
            ret.code = FAILED;
            async_call(conn_, ret);
            return;
        }

        db_Account_ext_t acc;
        acc.init(*res.get_row_at(0));
        
        if ( acc.password() != jpk_.oldpassword ) {
            ret.code = FAILED;
            async_call(conn_, ret);
            return;
        }
        acc.password = jpk_.password;
        acc.nickname = jpk_.nickname;

        if ( acc.update() ) {
            ret.code = FAILED;
            async_call(conn_, ret);
            return;
        }

        ret.code = SUCCESS;
        async_call(conn_, ret);
    }, conn_, jpk_, userid);
}
void gw_public_handler_t::on_req_create_blog(sp_rpc_conn_t conn_, blog_msg_def::req_create_blog_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_create_blog, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }


    uint32_t userid = gw_public_service.session.get_uid(remote->remote_id);

    if ( userid == 0 ) {
        return;
    }
    db_service.async_do(remote->remote_id, [](sp_rpc_conn_t conn_, blog_msg_def::req_create_blog_t& jpk_, uint32_t userid)
    {
        blog_msg_def::ret_create_blog_t ret;

        db_Blog_ext_t blog;

        if ( (blog.id = dbid_assign.new_dbid("bid")) == 0 ) {
            logerror((LOG, "on_req_create_blog, new id failed"));
            ret.code = FAILED;
            async_call(conn_, ret);
            return;
        }

        blog.content = jpk_.content;
        blog.lng = jpk_.longitude;
        blog.lat = jpk_.latitude;
        blog.uid = userid;
        blog.location = jpk_.location;
        
        if ( blog.insert() ) {
            logerror((LOG, "on_req_create_blog, new id failed"));
            ret.code = FAILED;
            async_call(conn_, ret);
            return;
        }
        ret.code = SUCCESS;
        async_call(conn_, ret);
    }, conn_, jpk_, userid);

}

void gw_public_handler_t::on_req_blog(sp_rpc_conn_t conn_, blog_msg_def::req_blog_t& jpk_)
{
    remote_info_t* remote = conn_->get_data<remote_info_t>();
    if (remote == NULL || remote->verified == false)
    {
        logerror((LOG, "on_req_blog, conn is not verified!"));
        conn_->close(RPC_SHUTDOWN);
        return;
    }


    db_service.async_do([](sp_rpc_conn_t conn_, blog_msg_def::req_blog_t& jpk_)
    {
        blog_msg_def::ret_blog_t ret;

        sql_result_t res;
        db_Blog_ext_t blog;

        boost::format sqlfmt=boost::format("select t2.* from (SELECT `uid` , `ctime` , `content` , `lng` , `lat` , `location` ,  ROUND(6378.138*2*ASIN(SQRT(POW(SIN((%f*PI()/180-lat*PI()/180)/2),2)+COS(%f*PI()/180)*COS(lat*PI()/180)*POW(SIN((%f*PI()/180-lng*PI()/180)/2),2)))*1000) AS juli FROM `Blog`)  t2 WHERE juli < %f order by juli limit %u,10 ")% jpk_.latitude % jpk_.latitude % jpk_.longitude % jpk_.distance % jpk_.index;

        db_service.async_select(sqlfmt.str(), res);

        size_t row = res.affect_row_num();

        for ( size_t i = 0; i < row ; i++) {
            sql_result_row_t* r = res.get_row_at(i);
            blog_msg_def::blog_t b;
            b.userid = r->get_value_at(0);
            b.latitude = r->get_value_at(4);
            b.longitude = r->get_value_at(3);
            b.content = std::move(r->get_value_at(2));
            b.time = r->get_value_at(1);
            b.location = r->get_value_at(5);
            sql_result_t tmp;
            db_Account_ext_t::select_uid(boost::lexical_cast<uint32_t>(b.userid), tmp);

            if ( tmp.affect_row_num() == 0 ) {
                continue;
            }
            db_Account_ext_t acc;
            acc.init(*tmp.get_row_at(0));
            b.head = std::move(acc.head());
            b.nickname = std::move(acc.nickname());
            b.sex = acc.sex;

            ret.list.push_back(std::move(b));
        }

        async_call(conn_, ret);
    }, conn_, jpk_);


}
//===============================================
gw_public_service_t::gw_public_service_t():
    m_started(false)
{
}
void gw_public_service_t::start(uint16_t serid_, string ip_, string port_)
{
    logwarn((LOG, "start gateway public service..."));
    if (m_started)
        return;
    m_started = true;

    listen(ip_, port_, true);

    logwarn((LOG, "start gateway public service ok! serid:%u", serid_));
}
void gw_public_service_t::add_valid_seskey(uint64_t seskey_)
{
    sp_rpc_conn_t conn = get_client(seskey_);
    if (conn != NULL)
    {
        remote_info_t* info = conn->get_data<remote_info_t>();
        if (info != NULL)
            info->verified = true;
    }
}
void gw_public_service_t::get_rolelist(remote_info_t* remote, vector<gw_msg_def::req_type_t> &list, gw_msg_def::ret_rolelist_t& jpk_)
{
    sp_user_t user;
    m_user_mgr.get(session.get_uid(remote->remote_id), user);

    if ( user == nullptr ) {
        return ;
    }
    db_service.async_do([](uint64_t seskey, vector<gw_msg_def::req_type_t> &list, gw_msg_def::ret_rolelist_t &jpk_, sp_user_t user)
    {
        gw_public_service.foreach_client([&](sp_rpc_conn_t conn_){
            remote_info_t* info = conn_->get_data<remote_info_t>();
            if(info != NULL && info->verified )
            {
                uint32_t userid = gw_public_service.session.get_uid(info->remote_id);
                sp_user_t tuser;
                if (gw_public_service.m_user_mgr.get(userid, tuser))
                {
                    bool flag = true;
                    for(auto type : list)
                    {
                        if ( type.type == 1 && user->get_dist(tuser) > type.para ) {
                            flag = false;
                            break;
                        }
                        else if ( type.type == 2 && tuser->sex != type.para)
                        {
                            flag = false;
                            break;
                        }
                        
                    }
                    gw_msg_def::role_t role;
                    sql_result_t res;
                    if ( flag ) {
                        role.userid = userid;
                        role.longitude = tuser->longitude;
                        role.latitude = tuser->latitude;
                        db_Account_ext_t::select_uid(userid, res);

                        if ( res.affect_row_num() == 0 ) {
                            return;
                        }
                        db_Account_ext_t acc;
                        acc.init(*res.get_row_at(0));
                        role.head = std::move(acc.head());
                        role.nickname = std::move(acc.nickname());
                        role.sex = acc.sex;

                        jpk_.list.push_back(std::move(role));
                    }
                }
            }
        });
        gw_public_service.async_unicast_to_client(seskey, jpk_);
    }, remote->remote_id, list, jpk_, user);
}
