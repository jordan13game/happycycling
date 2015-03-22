#include "config.h"
#include "remote_info.h"

conf_def::log_t             g_log;
conf_def::comlog_t          g_comlog;
conf_def::heart_beat_t      g_heart_beat; 
conf_def::perf_t            g_perf;
conf_def::local_cache_t     g_local_cache;
conf_def::gateway_map_t     g_gateway_map;
conf_def::route_t           g_route;
conf_def::res_t             g_res;
conf_def::sqldb_t           g_sqldb;
conf_def::global_sqldb_t    g_global_sqldb;

config_t::config_t():
log(g_log),
comlog(g_comlog),
heart_beat(g_heart_beat), 
perf(g_perf),
local_cache(g_local_cache),
gateway_map(g_gateway_map),
res(g_res),
sqldb(g_sqldb),
global_sqldb(g_global_sqldb),
m_hid(0),
m_phpstart(false),
m_stoptm(0)
{
}

void config_t::init(const char* path_, int ft_/*=ft_lua*/)
{
    m_path = path_;

    switch(ft_)
    {
    case ft_lua:
    {
        boost::shared_ptr<ys_lua_t> lua_ptr(new ys_lua_t());
        lua_ptr->load_file(path_);

        log << lua_ptr->call<string>("get_log");
        heart_beat << lua_ptr->call<string>("get_heart_beat");
        gateway_map << lua_ptr->call<string> ("get_gateway");
        sqldb << lua_ptr->call<string>("get_sqldb");
        global_sqldb << lua_ptr->call<string>("get_global_sqldb");
    }
    break;
    case ft_json:
    {
        FILE* file = fopen(path_, "rb");
        if (file == NULL)
            throw std::runtime_error("open config file failed!");

        string json;
        char buf[256] = {0};
        int readlen = 0;
        while((readlen = fread(buf, 1, sizeof(buf), file)))
        {
            json.append(buf, readlen);
        }
        if (readlen == -1)
        {
            throw std::runtime_error("read config file failed!");
        }
        fclose(file);


        (*this) << json;
    }
    break;
    }
}

void config_t::hotload(const char* path_)
{
    try
    {
        boost::shared_ptr<ys_lua_t> lua_ptr(new ys_lua_t());
        lua_ptr->load_file(path_);
        gateway_map << lua_ptr->call<string> ("get_gateway");
        local_cache << lua_ptr->call<string>("get_cache");
    }
    catch(std::exception& ex_)
    {
//        logerror(("HOTLOAD", "hotload error!, exception<%s>", ex_.what()));
    }
}



void config_t::reset(int ft_/*=ft_lua*/)
{
    init(m_path.c_str(), ft_);
}

