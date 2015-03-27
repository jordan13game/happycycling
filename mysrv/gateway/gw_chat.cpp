#include "gw_chat.h"
#include "db_service.h"
#include "db_ext.h"
#include "gw_public_service.h"
#include "gw_team.h"
#define LOG "GW_CHAT"

void gw_chat_t::post(uint32_t userid, gw_msg_def::req_send_message_t& jpk_)
{
    logwarn((LOG, "post message from %u", userid));
    gw_msg_def::req_recv_message_t msg;
    msg.userid = userid;
    msg.content = jpk_.content;
    msg.datetime = jpk_.datetime;
    msg.type = jpk_.type;
    msg.groupid = jpk_.groupid;
    if ( jpk_.groupid !=0 ) {
        db_service.async_do((uint64_t)userid, [](uint32_t userid, gw_msg_def::req_recv_message_t& msg)
        {
            sql_result_t res;
            db_Team_User_ext_t::select_uid(userid, res);

            if ( res.affect_row_num() == 0 ) {
                return ;
            }
            db_Team_User_ext_t team_user;
            team_user.init(*res.get_row_at(0));

            uint32_t gid = team_user.gid;

            db_Team_User_ext_t::select_gid(gid, res);

            db_Chat_log_ext_t chat;
            chat.stime = msg.datetime;
            chat.isteam = 1;
            msg.datetime = std::move(chat.stime());
            sql_result_t t;
            db_Account_ext_t acc;
            db_Account_ext_t::select_uid(msg.userid, t);
            acc.init(*t.get_row_at(0));
            msg.nickname = std::move(acc.nickname());
            msg.head = std::move(acc.head());

            string tmp;
            msg >> tmp;
            chat.content = std::move(tmp);
            
            size_t row = res.affect_row_num();

            for ( size_t i = 0 ; i < row ; i++ ) {
                team_user.init(*res.get_row_at(i));
                chat.uid = team_user.uid;
                if ( chat.insert() ) {
                    logerror((LOG, "insert chat failed! uid:%u ", team_user.uid));
                    return ;
                }
                logtrace((LOG, "insert chat end..."));
                uint64_t session = gw_public_service.session.get_seskey(team_user.uid);
                gw_public_service.async_unicast_to_client(session, msg);
            }
        }, jpk_.userid, msg);
        
    } else {
        db_service.async_do((uint64_t)userid, [](uint32_t userid, gw_msg_def::req_recv_message_t& msg)
        {
            db_Chat_log_ext_t chat;
            chat.uid = userid;
            chat.stime = msg.datetime;
            chat.isteam = 0;
            msg.datetime = std::move(chat.stime());
            sql_result_t t;
            db_Account_ext_t acc;
            db_Account_ext_t::select_uid(msg.userid, t);
            acc.init(*t.get_row_at(0));
            msg.nickname = std::move(acc.nickname());
            msg.head = std::move(acc.head());

            string tmp;
            msg >> tmp;
            chat.content = std::move(tmp);
            if ( chat.insert() ) {
                logerror((LOG, "insert chat failed! uid:%u ", userid));
                return ;
            }
            logtrace((LOG, "insert chat end..."));
            
            uint64_t session = gw_public_service.session.get_seskey(userid);
            if(session != 0)
            {
                gw_public_service.async_unicast_to_client(session, msg);
            }
        }, jpk_.userid, msg);
    }

}

void gw_chat_t::check_message(uint32_t userid)
{
    logtrace((LOG, "check_message userid:%u", userid));
    db_service.async_do((uint64_t)userid, [](uint32_t userid)
    {
        sql_result_t res;
        db_Chat_log_ext_t::select_uid(userid, res);

        size_t row = res.affect_row_num();

        for ( size_t i = 0; i < row ; i++ ) {
            db_Chat_log_ext_t chat;
            chat.init(*res.get_row_at(i));
            uint64_t session = gw_public_service.session.get_seskey(userid);
            string msg = std::move(chat.content());
            if(session != 0)
            {
                gw_public_service.async_unicast_to_client(session, gw_msg_def::req_recv_message_t::cmd(), msg);
            }
        }
    }, userid);
}
