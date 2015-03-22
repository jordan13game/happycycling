#include "gw_team.h"
#include "log.h"
#include "code_def.h"
#include "db_service.h"
#include "dbgl_service.h"
#include "db_ext.h"
#include "db_def.h"
#include "gw_public_service.h"

#define LOG "GW_TEAM"

gw_team_t::gw_team_t()
{
    reg_call(&gw_team_t::on_req_create_team);
    reg_call(&gw_team_t::on_req_add_team);
    reg_call(&gw_team_t::on_ret_nt_add_team);
    reg_call(&gw_team_t::on_ret_nt_add_team_user);
    reg_call(&gw_team_t::on_req_exit_team);
    reg_call(&gw_team_t::on_req_kick_user);
    reg_call(&gw_team_t::on_req_team_member);
    reg_call(&gw_team_t::on_req_team_info);
    reg_call(&gw_team_t::on_req_sos);
    reg_call(&gw_team_t::on_ret_ntinfo);
}

gw_team_t::~gw_team_t()
{
}

void gw_team_t::check_lost(uint32_t userid)
{
    logwarn((LOG, "check_lost begin..."));
    db_service.async_do((uint64_t)userid, [](uint32_t userid)
    {
        sql_result_t res;
        db_Team_User_ext_t::select_uid(userid, res);
        if ( res.affect_row_num() == 0 ) {
            return ;
        }
        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));

        db_Team_ext_t team;
        db_Team_ext_t::select_gid(team_user.gid, res);
        if ( res.affect_row_num() == 0 ) {
            return ;
        }
        team.init(*res.get_row_at(0));

        sp_user_t user;
        if (gw_public_service.m_user_mgr.get(userid, user))
        {
            sp_user_t tuser;
            if(gw_public_service.m_user_mgr.get(team.uid, tuser))
            {
                if ( user->get_dist(tuser) > 1000 ) {
                    team_msg_def::nt_lost_t ret;
                    db_Account_ext_t acc;
                    db_Account_ext_t::select_uid(userid, res);
                    acc.init(*res.get_row_at(0));
                    ret.nickname = std::move(acc.nickname());
                    uint64_t seskey_ = gw_public_service.session.get_seskey(team.uid);
                    gw_public_service.async_unicast_to_client(seskey_, ret);
                }
            }
        }
    }, userid);
}

void gw_team_t::check_nt_info(uint32_t userid)
{
    db_service.async_do([](uint32_t userid)
    {
        sql_result_t res;
        db_Team_ntinfo_ext_t::select_uid(userid, res);
        size_t row = res.affect_row_num();
        logdebug((LOG, "ntinfo %u %u", userid, row));
        db_Team_ntinfo_ext_t info;

        uint64_t seskey_ = gw_public_service.session.get_seskey(userid);
        logdebug((LOG, "ntinfo session: %llu", seskey_));
        for ( size_t i = 0; i < row ; i++ ) {
            info.init(*res.get_row_at(i));

            sql_result_t tmp;
            db_Account_ext_t::select_uid(info.fromuid, tmp);
            db_Account_ext_t acc;
            acc.init(*tmp.get_row_at(0));

            switch ( info.infotype ) {
            case 1:	
            {
                team_msg_def::nt_add_team_t ret;
                ret.id = info.id;
                ret.userid = info.fromuid;
                ret.vertify = std::move(info.vertify());
                ret.head = std::move(acc.head());
                ret.nickname = std::move(acc.nickname());

                gw_public_service.async_unicast_to_client(seskey_, ret);
            }
            break;

            case 2:	

            case 3:	

            case 4:	
            {
                team_msg_def::nt_msg_t ret;
                ret.id = info.id;
                ret.userid = info.fromuid;
                ret.type = info.infotype;
                ret.nickname = std::move(acc.nickname());
                gw_public_service.async_unicast_to_client(seskey_, ret);
            }
            break;

            case 5:
            {
                team_msg_def::nt_add_team_user_t ret;
                ret.id = info.id;
                ret.gid = info.fromuid;
                ret.code = 0;
                gw_public_service.async_unicast_to_client(seskey_, ret);
            }
            break;

            case 6:	
            {
                team_msg_def::nt_add_team_user_t ret;
                ret.id = info.id;
                ret.code = 1;
                gw_public_service.async_unicast_to_client(seskey_, ret);
            }
            break;

            default:	
            break;
            }				/* -----  end switch  ----- */
        }
    }, userid);
}

void gw_team_t::nt_info(uint32_t userid, uint16_t type, uint32_t fromuid)
{
    db_service.async_do((uint64_t)userid, [](uint32_t userid, uint16_t type, uint32_t fromuid)
    {
        db_Team_ntinfo_ext_t ntinfo;

        if ( (ntinfo.id = dbid_assign.new_dbid("gnid")) == 0 ) {
            logerror((LOG, "nt_info new gnid failed"));
            return;
        }
        ntinfo.uid = userid;
        ntinfo.infotype = type;
        ntinfo.fromuid = fromuid;
        
        if ( ntinfo.insert() ) {
            logerror((LOG, "nt_info insert failed"));
            return;
        }

        sql_result_t tmp;
        db_Account_ext_t::select_uid(fromuid, tmp);
        db_Account_ext_t acc;
        acc.init(*tmp.get_row_at(0));

        team_msg_def::nt_msg_t ret;
        ret.type = type;
        ret.userid = fromuid;
        ret.id = ntinfo.id;
        ret.nickname = std::move(acc.nickname());
        gw_public_service.async_unicast_to_client(gw_public_service.session.get_seskey(userid), ret);
    }, userid, type, fromuid);
}

void gw_team_t::on_ret_ntinfo(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::ret_msg_t& jpk_)
{   
    db_service.async_do(seskey_, [](uint64_t seskey_, team_msg_def::ret_msg_t& jpk_)
    {
        sql_result_t res;
        db_Team_ntinfo_ext_t nt;
        db_Team_ntinfo_ext_t::select_id(jpk_.id, res);
        if(res.affect_row_num() == 0) return ;
        nt.init(*res.get_row_at(0));
        nt.remove();
    }, seskey_, jpk_);
}
void gw_team_t::on_req_create_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_create_team_t& jpk_)
{
    logwarn((LOG, "create team begin..."));
    db_service.async_do((uint64_t)seskey_, [](uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_create_team_t& jpk_)
    {
        team_msg_def::ret_create_team_t ret;
        sql_result_t res;

        uint32_t userid = gw_public_service.session.get_uid(seskey_);

        if ( userid == 0 ) {
            logerror((LOG, "unknown user"));
            ret.code = FAILED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        db_Team_User_ext_t::select_uid(userid, res);
        if( res.affect_row_num() > 0 )
        {
            logerror((LOG, "already have team"));
            ret.code = ERROR_TEAM_EXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ext_t team;
        if ( (team.gid = dbid_assign.new_dbid("gid")) == 0 ) {
            logerror((LOG, "on_req_create_team new gid failed!"));
            ret.code = FAILED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        team.teamname = jpk_.teamname;
        team.desp = jpk_.desp;
        team.uid = userid;

        db_Team_User_ext_t team_user;
        team_user.uid = userid;
        team_user.gid = team.gid;
        if ( team.insert() || team_user.insert() ) {
            logerror((LOG, "on_req_create_team create new team failed!"));
            ret.code = FAILED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }
        ret.gid = team.gid;
        ret.code = SUCCESS;
        gw_public_service.async_unicast_to_client(seskey_, ret);

    }, seskey_, conn_, jpk_ );
}

void gw_team_t::on_req_add_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_add_team_t& jpk_)
{
    db_service.async_do(seskey_, [](uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_add_team_t& jpk_)
    {
        team_msg_def::ret_add_team_t ret;
        sql_result_t res;

        uint32_t userid = gw_public_service.session.get_uid(seskey_);

        if ( userid == 0 ) {
            logerror((LOG, "unknown user"));
            ret.code = FAILED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_User_ext_t::select_uid(userid, res);
        if( res.affect_row_num() > 0 )
        {
            logerror((LOG, "already have team"));
            ret.code = ERROR_TEAM_EXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ntinfo_ext_t ntinfo;
        if ( (ntinfo.id = dbid_assign.new_dbid("gnid")) == 0 ) {
            logerror((LOG, "on_add_team new gnid failed!"));
            ret.code = FAILED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ext_t::select_gid(jpk_.gid, res);

        if( res.affect_row_num() == 0 )
        {
            logerror((LOG, "don't have team gid = %u", jpk_.gid));
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ext_t team;
        team.init(*res.get_row_at(0));

        ntinfo.uid = team.uid;
        ntinfo.fromuid = userid;
        ntinfo.infotype = 1;
        ntinfo.vertify = jpk_.vertify;

        if ( ntinfo.insert() ) {
            logerror((LOG, "on_add_team create new ntinfo failed!"));
            ret.code = FAILED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        ret.code = SUCCESS;
        gw_public_service.async_call(conn_, ret);

        team_msg_def::nt_add_team_t nt;
        nt.id = ntinfo.id;
        nt.userid = userid;
        nt.vertify = jpk_.vertify;
        db_Account_ext_t::select_uid(userid, res);
        db_Account_ext_t acc;
        acc.init(*res.get_row_at(0));
        nt.head = std::move(acc.head());
        nt.nickname = std::move(acc.nickname());

        gw_public_service.async_unicast_to_client(gw_public_service.session.get_seskey(team.uid), nt);

    }, seskey_, conn_, jpk_);
}

void gw_team_t::on_ret_nt_add_team_user(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::ret_nt_add_team_user_t& jpk_)
{
    db_service.async_do(seskey_, [](team_msg_def::ret_nt_add_team_user_t& jpk_)
    {
        db_Team_ntinfo_ext_t nt;
        nt.id = jpk_.id;
        if (nt.remove())
        {
            logerror((LOG, "on_ret_nt_add_team_user remove failed"));
        }
    }, jpk_);
}

void gw_team_t::on_ret_nt_add_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::ret_nt_add_team_t& jpk_)
{
    db_service.async_do(seskey_, [](team_msg_def::ret_nt_add_team_t& jpk_)
    {
        db_Team_ntinfo_ext_t ntinfo;
        sql_result_t res;

        db_Team_ntinfo_ext_t::select_id(jpk_.id, res);
        if( res.affect_row_num() == 0)
            return;

        ntinfo.init(*res.get_row_at(0));

        uint32_t userid = ntinfo.fromuid;

        db_Team_User_ext_t::select_uid(ntinfo.uid, res);
        if( res.affect_row_num() == 0)
            return;

        if ( ntinfo.remove() ) {
            logerror((LOG, "on_nt_add_team remove ntinfo failed! id=%u", jpk_.id));
        }
        db_Team_User_ext_t tmp;
        tmp.init(*res.get_row_at(0));

        if ((ntinfo.id = dbid_assign.new_dbid("gnid")) == 0)
            return;
        team_msg_def::nt_add_team_user_t ret;
        ret.id = ntinfo.id;
        ret.code = jpk_.code;
        ret.gid = 0;
        if ( jpk_.code == 0 ) {
            ntinfo.uid = userid;
            ntinfo.infotype = 5;
            ntinfo.fromuid = tmp.gid;
            ret.gid = tmp.gid;
            db_Team_User_ext_t team_user;
            team_user.gid = tmp.gid;
            team_user.uid = userid;
            if( team_user.insert() ) return;
        } else {
            ntinfo.uid = userid;
            ntinfo.infotype = 6;
        }
        ntinfo.insert();
        gw_public_service.async_unicast_to_client(gw_public_service.session.get_seskey(userid), ret);
    }, jpk_);
}

void gw_team_t::on_req_kick_user(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_kick_t& jpk_)
{
    db_service.async_do( seskey_, [](uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_kick_t& jpk_)
    {
        sql_result_t res;
        team_msg_def::ret_kick_t ret;
        uint32_t userid = gw_public_service.session.get_uid(seskey_);

        db_Team_User_ext_t::select_uid(userid, res);

        if(res.affect_row_num() == 0)
        {
            logerror((LOG, "on_req_kick_user have no team!"));
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));

        db_Team_ext_t::select_gid(team_user.gid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ext_t team;
        team.init(*res.get_row_at(0));

        if ( team.uid != userid ) {
            ret.code = FAILED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }
        db_Team_User_ext_t::select_uid(jpk_.userid, res);

        if(res.affect_row_num() == 0)
        {
            logerror((LOG, "on_req_kick_user have no user!"));
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        team_user.init(*res.get_row_at(0));
        if ( team_user.remove() ) {
            logerror((LOG, "on_req_kick_user remove failed"));
            ret.code = FAILED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }
        ret.code = SUCCESS;
        gw_public_service.async_call(conn_, ret);
    }, seskey_, conn_, jpk_);

}
void gw_team_t::on_req_exit_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_exit_team_t& jpk_)
{
    db_service.async_do( seskey_, [](uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_exit_team_t& jpk_)
    {
        sql_result_t res;
        team_msg_def::ret_exit_team_t ret;
        uint32_t userid = gw_public_service.session.get_uid(seskey_);

        db_Team_User_ext_t::select_uid(userid, res);

        if(res.affect_row_num() == 0)
        {
            logerror((LOG, "on_exit_team have no team!"));
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));

        if( team_user.remove() )
        {
            logerror((LOG, "on_exit_team del failed!"));
            ret.code = FAILED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        db_Team_ext_t::select_gid(team_user.gid, res);
        db_Team_ext_t team;
        team.init(*res.get_row_at(0));

        if ( team.uid == userid ) {
            team.remove();
        }
        db_Team_User_ext_t::select_gid(team.gid, res);
        size_t row = res.affect_row_num();

        for ( size_t i = 0 ; i < row ; i++ ) {
            team_user.init(*res.get_row_at(i));
            gw_team.nt_info(team_user.uid, 2, userid);
            if( team.uid == userid)
            {
                team_user.remove();
                gw_team.nt_info(team_user.uid, 4, userid);
            }
        }
        ret.code = SUCCESS;
        gw_public_service.async_call(conn_, ret);
    }, seskey_, conn_, jpk_);
}

void gw_team_t::on_req_team_member(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_team_member_t& jpk_)
{
    db_service.async_do([](uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_team_member_t& jpk_)
    {
        uint32_t userid = gw_public_service.session.get_uid(seskey_);
        team_msg_def::ret_team_member_t ret;

        sql_result_t res;
        db_Team_User_ext_t::select_uid(userid, res);

        if(res.affect_row_num() == 0)
        {
            logerror((LOG, "on_req_team_member user has no team!"));
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_call(conn_, ret);
            return ;
        }

        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));
        db_Team_ext_t::select_gid(team_user.gid, res);
        db_Team_ext_t team;
        team.init(*res.get_row_at(0));
        uint32_t leaderuid = team.uid;
        db_Team_User_ext_t::select_gid(team_user.gid, res);

        ret.code = SUCCESS;
        size_t row = res.affect_row_num();
        
        for ( size_t i = 0; i < row ; i++) {
            team_user.init(*res.get_row_at(i));
            sp_user_t user;
            if ( gw_public_service.m_user_mgr.get(team_user.uid, user) ) {
                team_msg_def::team_member_info info;
                info.userid = team_user.uid;
                info.latitude = user->latitude;
                info.longitude = user->longitude;
                info.type = (team_user.uid == leaderuid)?1:0;
                sql_result_t tmp;
                db_Account_ext_t::select_uid(info.userid, tmp);

                if ( tmp.affect_row_num() == 0 ) {
                    continue;
                }
                db_Account_ext_t acc;
                acc.init(*tmp.get_row_at(0));
                info.head = std::move(acc.head());
                info.nickname = std::move(acc.nickname());
                info.sex = acc.sex;

                logdebug((LOG, "req_team_member_t %lf %lf", info.latitude, info.longitude));
                ret.list.push_back(std::move(info));
            }
        }

        gw_public_service.async_unicast_to_client(seskey_, ret);
    }, seskey_, conn_, jpk_);
}

void gw_team_t::on_req_sos(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_sos_t& jpk_)
{
    db_service.async_do([](uint64_t seskey_)
    {
        uint32_t userid = gw_public_service.session.get_uid(seskey_);
        sql_result_t res;
        db_Team_User_ext_t::select_uid(userid, res);

        if(res.affect_row_num() == 0)return;

        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));

        uint32_t gid = team_user.gid;

        db_Team_User_ext_t::select_gid(gid, res);

        size_t row = res.affect_row_num();

        team_msg_def::nt_sos_t ret;
        ret.userid = userid;

        for ( size_t i = 0; i < row ; i++ ) {
            team_user.init(*res.get_row_at(i));
            gw_public_service.async_unicast_to_client(gw_public_service.session.get_seskey(team_user.uid), ret);
        }

    }, seskey_);
}

void gw_team_t::on_req_team_info(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_team_info_t& jpk_)
{
    db_service.async_do([](uint64_t seskey_)
    {
        sql_result_t res;
        team_msg_def::ret_team_info_t ret;

        uint32_t userid = gw_public_service.session.get_uid(seskey_);
        db_Team_User_ext_t::select_uid(userid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));

        db_Team_ext_t::select_gid(team_user.gid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ext_t team;
        team.init(*res.get_row_at(0));
        ret.desp = std::move(team.desp());
        ret.teamname = std::move(team.teamname());
        ret.code = SUCCESS;
        gw_public_service.async_unicast_to_client(seskey_, ret);
    }, seskey_);
}

void gw_team_t::on_req_edit_team(uint64_t seskey_, sp_rpc_conn_t conn_, team_msg_def::req_edit_team_t& jpk_)
{
    db_service.async_do(seskey_, [](uint64_t seskey_, team_msg_def::req_edit_team_t& jpk_)
    {
        sql_result_t res;
        team_msg_def::ret_team_info_t ret;

        uint32_t userid = gw_public_service.session.get_uid(seskey_);
        db_Team_User_ext_t::select_uid(userid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_User_ext_t team_user;
        team_user.init(*res.get_row_at(0));

        db_Team_ext_t::select_gid(team_user.gid, res);

        if ( res.affect_row_num() == 0 ) {
            ret.code = ERROR_TEAM_NOTEXISTED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }

        db_Team_ext_t team;
        team.init(*res.get_row_at(0));
        team.desp = std::move(jpk_.desp);

        if ( team.update() ) {
            ret.code = FAILED;
            gw_public_service.async_unicast_to_client(seskey_, ret);
            return ;
        }
        ret.code = SUCCESS;
        gw_public_service.async_unicast_to_client(seskey_, ret);
    }, seskey_, jpk_);
}
