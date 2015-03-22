//#include "tbb_mem.h"
#include "signal_service.h"
#include "io_service_pool.h"
#include "log.h"
//#include "performance_service.h"
#include "arg_helper.h"
#include "remote_info.h"
#include "config.h"
#include "db_service.h"
#include "dbgl_service.h"

#include "gw_public_service.h"
//#include "gw_private_service.h"
//#include "gw_rt_client.h"

#define LOG "GW_SERVER"

class runner_t
{
public:
    static int start(int argc, char* argv[]) 
    {
        arg_helper_t arg_helper(argc, argv);
        if(arg_helper.is_enable_option("-v"))
        {
            printf("version:%s\n", VER_GATEWAY);
            exit(0);
        }

        bool dm = false;
        if(arg_helper.is_enable_option("-d"))
        {
            dm = true;
            if (dm)
            {
                daemon(1, 1);
            }
        }

        string serid = arg_helper.get_option_value("-id");
        if (serid == "")
        {
            printf("gateway [-id] [-conf]!");
            exit(-1);
        }

        config.serid = std::atoi(serid.c_str());

        int ft = ft_lua;
        string path = arg_helper.get_option_value("-conf");
        if (path == "")
        {
            ft = ft_json;
            path = arg_helper.get_option_value("-jconf");
            if (path == "")
            {
                printf("login [-id] [-conf/-jconf]!");
                exit(-1);
            }
        }

        try
        {
            config.init(path.c_str(), ft);
        }
        catch(std::exception& ex_)
        {
            printf("config exception<%s>!", ex_.what());
            exit(-1);
        }

        string logname = "gateway";
        logname += serid;
        init_log(config.log.path, logname, config.log.print_file, dm?0:config.log.print_screen, config.log.level, config.log.modules);

        auto it = config.gateway_map.contain.find(serid);
        if (it == config.gateway_map.contain.end())
        {
            //logerror((LOG, "no gateway config! serid:%s", serid.c_str()));
            fprintf(stderr, "no gateway config! serid:%s", serid.c_str());
            exit(-1);
        }
        const conf_def::gateway_t& gw = it->second;


        //log_screen_open(!dm);


        if (config.heart_beat.timedout_flag > 0)
        {
            heart_beat_setting_t hb;
            hb.timeout_flag = 1;
            hb.timeout = config.heart_beat.timedout;
            hb.max_limit_flag = config.heart_beat.max_limit_flag; 
            hb.max_limit = config.heart_beat.max_limit;
            rpc_conn_monitor_t::start_monitor(&hb);
        }
 
        conf_def::sqldb_t sqldb = config.sqldb;
        if (db_service.start(sqldb.ip, sqldb.port, sqldb.user, sqldb.pwd, sqldb.name, 4))
        {
            logerror((LOG, "connect sql[%s:%s,%s]:failed!", sqldb.ip.c_str(), sqldb.port.c_str(), sqldb.name.c_str()));
        }

        conf_def::global_sqldb_t &gldb = config.global_sqldb;
        if (dbgl_service.start(gldb.ip, gldb.port, gldb.user, gldb.pwd, gldb.name, 1))
        {
            logerror((LOG, "connect global sql[%s:%s,%s]:failed!", gldb.ip.c_str(), gldb.port.c_str(), gldb.name.c_str()));
            return -1;
        }

        logtrace((LOG, "runner_t:start..."));

        io_pool.start(gw.thread_num);
        //开启网关服务
        uint16_t nserid = atoi(serid.c_str());
        gw_public_service.start(nserid, gw.listen_ip, gw.public_port);

        logdebug((LOG, "%s:%s", gw.listen_ip.c_str(), gw.public_port.c_str()));

        signal_service.start();

        if (config.heart_beat.timedout_flag > 0)
        {
            rpc_conn_monitor_t::stop_monitor();
        }


        gw_public_service.close_wait();


        logtrace((LOG,"runner_t:stop"));
        return 0;
    }
};

int main(int argc, char* argv[])
{
    return runner_t::start(argc, argv);
}
