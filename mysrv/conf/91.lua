out_ip = '192.168.177.128'
inner_ip = '192.168.177.128'

port_map=
{
route = {out=51100,inner=51200},
gw = {out=12345,inner=12345},
inv = {inner=53000},
}

log = {
    level = 6,
    path = '/tmp/log/',
    print_file = 1,
    print_screen = 1,
    modules = {
        --'REPO',
        'WRITE_STATE',
        'DB_MYSQL',
        'DB_SERVICE',
        --'HEART_BEAT',
        "RPC_ACCEPTOR",
        "RPC_CONNECTER",
        "RPC_CCP",
        "RPC",
        "RPC_CLIENT",
        "RPC_SERVER",
        "RPC_CONN_MGR",
        'SERVICE_BASE',
        --"LG_SERVER",
        --"LG_SERVICE",
        --'LG_LOGIC',
        --'GEN_NAME',
        'GW_SERVER',
        'GW_PUBLIC',
        'GW_SESSION',
        'GW_CHAT',
        'GW_TEAM',
        'PF_SERVICE',
        'MSG_DISPATCH',
    }
}

function get_log()
    --print(cjson.encode(log))
    return cjson.encode(log)
end

comlog = {
    level = 3,
    path = '/tmp/godssenki/comlog/',
    print_file = 0,
    print_screen = 1,
    max_module = 64,
    modules = {
        'COM_PERF',
    }
}

function get_comlog()
    --print(cjson.encode(comlog))
    return cjson.encode(comlog)
end

heart_beat = {
    --是否启用心跳
    timedout_flag = 1,
    --心跳过期时间
    timedout = 3600*5,
    --是否启用最大心跳容器上限
    max_limit_flag = 0,
    --最大上限
    max_limit = 50000,
}

function get_heart_beat()
    --print(cjson.encode(heart_beat))
    return cjson.encode(heart_beat)
end

--性能监控
perf = {
    flag = 1,
    interval = 300,
}

function get_perf()
    --print(cjson.encode(perf))
    return cjson.encode(perf)
end

--本地缓存
cache = {
    max = 0,
}
function get_cache()
    return cjson.encode(cache)
end

--资源文件
res = 
{
lua_path = '/tmp/godssenki/script/',
repo_path = '/tmp/godssenki/repo/',
rank_path = '/tmp/godssenki/rank/91/',
bt_record_path = '/tmp/godssenki/bt_record/91/',
}

function get_res()
    return cjson.encode(res)
end

global_sqldb = {
    ip = '127.0.0.1',
    port='3306',
    user='root',
    pwd ='123456',
    name='global',
}
function get_global_sqldb()
    return cjson.encode(global_sqldb)
end

--游戏数据库表
sqldb_map = {
    ip = '127.0.0.1',
    port='3306',
    user='root',
    pwd ='123456',
    name='happy',
}

function get_sqldb()  
    --print(cjson.encode(wrap))
    return cjson.encode(sqldb_map)
end


gateway_map = {}
gateway_map['1'] = {
    private_ip = inner_ip,
    private_port = tostring(port_map.gw.inner),
    public_ip = out_ip,
    public_port = tostring(port_map.gw.out),
    listen_ip = inner_ip,
    thread_num = 1,
}

function get_gateway()
    local wrap = {}
    wrap.contain = gateway_map 
    --print(cjson.encode(wrap))
    return cjson.encode(wrap)
end

login_map = {}
login_map['1'] = {
    sqldb= {
        id = '1',
        conn_num = 1,
    },
    thread_num = 1,
}

function get_login()
    local wrap = {}
    wrap.contain = login_map
    --print(cjson.encode(wrap))
    return cjson.encode(wrap)
end

route = {}
route.private_ip = inner_ip
route.public_port = tostring(port_map.route.out)
route.public_ip = out_ip
route.private_port = tostring(port_map.route.inner)
route.thread_num = 10

function get_route()
    return cjson.encode(route)
end

gslist = 
{
    { hostnum = 1, name = '众神之巅', serid = 1, recom=true},
}

function get_gslist()
    local wrap = {}
    wrap.contain = gslist 
    return cjson.encode(wrap)
end
