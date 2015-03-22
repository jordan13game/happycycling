#ifndef _SU_CMD_DEF_H_
#define _SU_CMD_DEF_H_

#include <boost/shared_ptr.hpp>

#include "../../lib/jpack/jserialize_macro.h"
#include "../../lib/utility/yarray.h"

#define DEC_CMD_RANGE(s, e) static bool cmd_range(uint16_t cmd_){return (s<=cmd_&&cmd_<e);}


//网关消息
struct gw_msg_def
{
    DEC_CMD_RANGE(100, 200)

    struct req_reg_t: public jcmd_t<101>
    {
        string account;
        string password;
        uint16_t sex;
        string nickname;
        string head;
        JSON5(req_reg_t, account, password, sex, nickname, head)
    };

    struct ret_reg_t : public jcmd_t<102>
    {
        uint16_t code;
        JSON1(ret_reg_t, code)
    };

    //请求登陆网关
    struct req_login_t: public jcmd_t<103>
    {
        string account;
        string password;
        JSON2(req_login_t, account, password)
    };

    //返回网关登陆
    struct ret_login_t: public jcmd_t<104>
    {
        //返回码
        uint16_t code;
        uint32_t userid;
        uint64_t session;
        uint16_t sex;
        string nickname;
        string head;
        uint32_t team;
        uint16_t isleader;
        JSON8(ret_login_t, code, userid, session, sex, nickname, head, team, isleader)
    };

    //心跳
    struct req_heartbeat_t : public jcmd_t<105>
    {
        uint64_t session;
        JSON1(req_heartbeat_t, session);
    };
    struct ret_heartbeat_t : public jcmd_t<106>
    {
        uint16_t code;
        JSON1(ret_heartbeat_t, code);
    };

    struct req_update_pos_t : public jcmd_t<107>
    {
        double longitude;
        double latitude;
        JSON2(req_update_pos_t, longitude, latitude);
    };

    struct ret_update_pos_t : public jcmd_t<108>
    {
        uint16_t code;
        JSON1(ret_update_pos_t, code);
    };

    struct req_type_t
    {
        uint16_t type;
        uint32_t para;
        JSON2(req_type_t, type, para);
    };
    struct req_rolelist_t : public jcmd_t<109>
    {
        uint64_t session;
        vector<req_type_t> list;
        JSON2(req_rolelist_t, session, list);
    };

    struct role_t
    {
        uint32_t userid;
        double longitude;
        double latitude;
        string head;
        string nickname;
        uint16_t sex;
        JSON6(role_t, userid, longitude, latitude, head, nickname, sex);
    };

    struct ret_rolelist_t : public jcmd_t<110>
    {
        uint16_t code;
        vector<role_t> list;
        JSON2(ret_rolelist_t, code, list);
    };

    struct req_send_message_t : public jcmd_t<111>
    {
        uint16_t type;
        uint32_t userid;
        uint32_t groupid;
        string datetime;
        string content;
        JSON5(req_send_message_t, type, userid, groupid, datetime, content);
    };

    struct ret_send_message_t : public jcmd_t<112>
    {
        uint16_t code;
        string datetime;
        JSON2(ret_send_message_t, datetime, code);
    };

    struct req_recv_message_t : public jcmd_t<113>
    {
        uint16_t type;
        uint32_t userid;
        uint32_t groupid;
        string datetime;
        string content;
        string nickname;
        JSON6(req_recv_message_t, type, userid, groupid, datetime, content, nickname);
    };

    struct ret_recv_message_t : public jcmd_t<114>
    {
        uint16_t code;
        string datetime;
        JSON2(ret_recv_message_t, datetime, code);
    };

    struct req_userinfo_t : public jcmd_t<115>
    {
        uint32_t userid;
        JSON1(req_userinfo_t, userid);
    };

    struct ret_userinfo_t : public jcmd_t<116>
    {
        uint16_t code;
        uint16_t sex;
        string nickname;
        string head;
        uint32_t team;
        JSON5(ret_userinfo_t, code, sex, nickname, head, team);
    };

    struct req_edit_userinfo_t : public jcmd_t<117>
    {
        string password;
        string nickname;
        string oldpassword;
        JSON3(req_edit_userinfo_t, password, oldpassword, nickname);
    };

    struct ret_edit_userinfo_t : public jcmd_t<118>
    {
        uint16_t code;
        JSON1(ret_edit_userinfo_t, code);
    };

};

struct team_msg_def
{
    DEC_CMD_RANGE(200,300);

    struct req_create_team_t : public jcmd_t<200>
    {
        string teamname;
        string desp;
        JSON2(req_create_team_t, teamname, desp);
    };

    struct ret_create_team_t : public jcmd_t<201>
    {
        uint16_t code;
        uint32_t gid;
        JSON2(ret_create_team_t, code, gid);
    };

    struct req_edit_team_t : public jcmd_t<202>
    {
        string desp;
        JSON1(req_edit_team_t, desp);
    };

    struct ret_edit_team_t : public jcmd_t<203>
    {
        uint16_t code;
        JSON1(ret_edit_team_t, code);
    };

    struct req_add_team_t : public jcmd_t<204>
    {
        uint32_t gid;
        string vertify;
        JSON2(req_add_team_t, gid, vertify);
    };

    struct ret_add_team_t : public jcmd_t<205>
    {
        uint16_t code;
        JSON1(ret_add_team_t, code);
    };

    struct nt_add_team_t : public jcmd_t<206>
    {
        uint32_t id;
        uint32_t userid;
        string head;
        string vertify;
        string nickname;
        JSON5(nt_add_team_t, id, userid, vertify, head, nickname);
    };

    struct ret_nt_add_team_t : public jcmd_t<207>
    {
        uint32_t id;
        uint16_t code;
        JSON2(ret_nt_add_team_t, id, code);
    };

    struct nt_add_team_user_t : public jcmd_t<208>
    {
        uint32_t id;
        uint32_t gid;
        uint16_t code;
        JSON3(nt_add_team_user_t, id, gid, code);
    };

    struct ret_nt_add_team_user_t : public jcmd_t<209>
    {
        uint32_t id;
        JSON1(ret_nt_add_team_user_t, id);
    };

    struct req_exit_team_t : public jcmd_t<210>
    {
        JSON0(req_exit_team_t);
    };

    struct ret_exit_team_t : public jcmd_t<211>
    {
        uint16_t code;
        JSON1(ret_exit_team_t, code);
    };

    struct req_kick_t : public jcmd_t<212>
    {
        uint32_t userid;
        JSON1(req_kick_t, userid);
    };

    struct ret_kick_t : public jcmd_t<213>
    {
        uint16_t code;
        JSON1(ret_kick_t, code);
    };

    struct req_team_member_t : public jcmd_t<214>
    {
        JSON0(req_team_member_t);
    };

    struct team_member_info
    {
        uint32_t userid;
        uint16_t type;
        double longitude;
        double latitude;
        string head;
        string nickname;
        uint16_t sex;
        JSON7(team_member_info, userid, type, longitude, latitude, head, nickname, sex);
    };

    struct ret_team_member_t : public jcmd_t<215>
    {
        uint16_t code;
        vector<team_member_info> list;
        JSON2(ret_team_member_t, code, list);
    };

    struct req_team_info_t : public jcmd_t<216>
    {
        JSON0(req_team_info_t);
    };

    struct ret_team_info_t : public jcmd_t<217>
    {
        uint16_t code;
        string teamname;
        string desp;
        JSON3(ret_team_info_t, code, teamname, desp);
    };

    struct nt_msg_t : public jcmd_t<218>
    {
        uint32_t id;
        uint32_t userid;
        uint16_t type;
        string nickname;
        JSON4(nt_msg_t, id, userid, type, nickname);
    };

    struct ret_msg_t : public jcmd_t<219>
    {
        uint32_t id;
        JSON1(ret_msg_t, id);
    };

    struct nt_lost_t : public jcmd_t<220>
    {
        string nickname;
        JSON1(nt_lost_t, nickname);
    };

    struct req_sos_t : public jcmd_t<221>
    {
        JSON0(req_sos_t);
    };

    struct nt_sos_t : public jcmd_t<222>
    {
        uint32_t userid;
        JSON1(nt_sos_t, userid);
    };

};

struct blog_msg_def
{
    DEC_CMD_RANGE(300,400);

    struct req_create_blog_t : public jcmd_t<300>
    {
        double longitude;
        double latitude;
        string content;
        string location;
        JSON4(req_create_blog_t, longitude, latitude, content, location);
    };

    struct ret_create_blog_t : public jcmd_t<301>
    {
        uint16_t code;
        JSON1(ret_create_blog_t, code);
    };

    struct req_blog_t : public jcmd_t<302>
    {
        double longitude;
        double latitude;
        uint32_t index;
        double distance;
        JSON4(req_blog_t, longitude, latitude, index, distance);
    };

    struct blog_t
    {
        string longitude;
        string latitude;
        string content;
        string userid;
        string time;
        string head;
        string nickname;
        uint16_t sex;
        string location;
        JSON9(blog_t, longitude, latitude, content, userid, time, head, nickname, sex, location);
    };

    struct ret_blog_t : public jcmd_t<303>
    {
        vector<blog_t> list;
        JSON1(ret_blog_t, list);
    };

};
#endif
