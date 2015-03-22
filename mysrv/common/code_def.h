#ifndef _CODE_DEF_H_
#define _CODE_DEF_H_

enum code_type_e
{
    SUCCESS = 0,
    FAILED = 1,

    ERROR_USER_EXCPEL = 10,

    //服务器内部错误码
    ERROR_TEAM_SERVER = 100,
    ERROR_TEAM_EXISTED,
    ERROR_TEAM_NOTEXISTED,            

    //会话码异常
    ERROR_SESKEY_EXCEPTION = 200,
    //当前会话已经失效
    ERROR_SESKEY_TIMEOVER,
    //重复登陆网关
    ERROR_SESKEY_EXISTED,

    //服务器异常
    ERROR_GW_EXCEPTION = 300,
    //更新会话码失败
    ERROR_GW_RESET_SESKEY_FAILED,
    //找不到游戏服务器
    ERROR_GW_NO_SCENE_SERVER,

    //服务器异常
    ERROR_LG_EXCEPTION = 400,
    //非法的mac地址
    ERROR_LG_MAC_ILLEGLE,
    //创建新角色，已经达到最大角色数
    ERROR_LG_ROLE_MAX_NUM,
    //非法的角色表格id
    ERROR_LG_ROLE_RESID,
    //账户不存在
    ERROR_LG_NO_AID,
    //平台验证失败
    ERROR_LG_VERIFY,

    //角色不存在
    ERROR_SC_NO_USER        = 500,
    //游戏服务器异常
    ERROR_SC_EXCEPTION,
    //找不到主城
    ERROR_SC_NO_CITY,
    //主城主城中没有当前用户
    ERROR_SC_NO_CITY_USER,
    //没有体力
    ERROR_SC_NO_POWER,
    //无效的请求
    ERROR_SC_ILLEGLE_REQ,
    //元宝不足
    ERROR_SC_NO_YB,
    //友情点不足
    ERROR_SC_NO_FPOINT,
    //金币不足
    ERROR_SC_NO_GOLD,
    //刷新次数不足
    ERROR_SC_NO_FLUSH,
    //没有当前伙伴
    ERROR_SC_NO_PARTNER  = 510,
    //材料不足
    ERROR_SC_NOT_ENOUGH_MATERIAL,
    //活力不足
    ERROR_SC_NO_ENERGY,
    //战历不足
    ERROR_SC_NO_BATTLEXP,
    //技能不存在
    ERROR_SC_NO_SKILL,
    //时间未到
    ERROR_SC_NOT_ENOUGH_TIME,
    //做某个操作的时候，vip等级不足
    ERROR_SC_NOT_ENOUGH_VIP,
    //伙伴数已达上线
    ERROR_PARTNER_MAX_COUNT,
    //英雄进阶已达最高等级
    ERROR_UPGRADE_MAX_LV,
    //活动已过期
    ERROR_QUEST_OVERDUED,
    //声望不足
    ERROR_SC_NO_HONOR = 520,
    //商城限量商品，个数不足
    ERROR_SHOP_LIMIT_OUT,

    //英雄系统
    //雇佣英雄时碎片不够
    ERROR_HERO_NOT_ENOUGH_CHIP = 550,
    //雇佣英雄时，该英雄已存在
    ERROR_HERO_EXISTED,

    //好友系统
    //添加好友，自己好友已达上限
    ERROR_SC_FRIEND_MAX_SOURCE = 600,
    //添加好友，对方好友已达上限
    ERROR_SC_FRIEND_MAX_TARGET,
    //不能添加自己为好友
    ERROR_SC_FRIEND_NO_SELF,
    //该玩家已经是你的好友
    ERROR_SC_FRIEND_REDU_ADD,
    //好友助战，已经助战过
    ERROR_SC_ROUND_HELPED,
    //好友助战，达到最大助战次数
    ERROR_SC_ROUND_MAX_HELP,
    //好友邀请，无效的邀请码
    ERROR_SC_ERROR_INVCODE,
    //好友邀请，不能邀请自己
    ERROR_SC_NO_INV_SELF,
    
    //新手系统
    //修改名字，名字已存在
    ERROR_SC_NAME_EXISTENCE = 620,

    //训练系统
    //训练场位置未开放
    ERROR_SC_PRACTICE_LOCKED = 700,
    //训练场位置不需要清除cd
    ERROR_SC_PRACTICE_NO_CLEAR,
    //伙伴于主角等级相同，不能训练
    ERROR_SC_PRACTICE_LV_EQUAL,
    //训练场cd
    ERROR_SC_PRACTICE_CD,

    //符文系统
    //无效的符文
    ERROR_RUNE_NO_RESID     = 800,
    //当前符文已经最大等级
    ERROR_RUNE_MAX_LV,
    //符文背包空
    ERROR_RUNE_BAG_EMPTY,
    //符文背包满
    ERROR_RUNE_BAG_FULL,
    //符文碎片不足
    ERROR_RUNE_NO_CHIP,
    //符文位置未解锁
    ERROR_RUNE_POS_LOCK,
    //当前角色等级不足
    ERROR_RUNE_LESS_GRADE,
    //玩家身上已有相同属性的符文
    ERROR_RUNE_DOUBLE_ATTR,
    //已招募五个怪
    ERROR_RUNE_FIVE_BOSS,
    //解雇英雄时，符文背包已满
    ERROR_FIRE_RUNE_BAG_FULL,

    //背包满
    ERROR_BAG_FULL          = 900,
    //没有当前道具
    ERROR_BAG_NO_ITEM,

    //没有当前装备
    ERROR_EQ_NO_EQ          = 1000,
    //背包中不存在
    ERROR_EQ_IN_BAG, 
    //等级不足
    ERROR_EQ_LOW_LV,
    //无效的部位
    ERROR_EQ_ILLEGLE_PART,
    //无效的职业
    ERROR_EQ_ILLEGLE_JOB,
    //装备升级，金币不足
    ERROR_EQ_LESS_MONEY,
    //无效的装备合成
    ERROR_EQ_ILLEGLE_COMPOSE,
    //缺少材料
    ERROR_EQ_LESS_MATERIAL,
    //等级不足
    ERROR_EQ_LESS_GRADE,
    //已经使用的宝石槽
    ERROR_EQ_USED_GEM_SLOT,
    //无效的宝石槽
    ERROR_EQ_ILLEGLE_GEM_SLOT,
    //无效的宝石属性
    ERROR_EQ_ILLEGLE_GEM_ATTR,
    //没有宝石
    ERROR_EQ_NO_GEM,
    //镶嵌失败
    ERROR_EQ_ILLEGLE_INLAY,
    //卸下宝石水晶不足
    ERROR_UNLAY_LESS_RMB,
    //卸下宝石道具不足
    ERROR_UNLAY_LESS_ITEM,
    //脱下未穿上的装备
    ERROR_EQ_TAKEOFF_UNWEARED,
    //已经装备
    ERROR_EQ_WEAR_EQUIPED,

    //关卡
    //进关卡条件不足
    ERROR_ROUND_NO_PERMIT   = 1100,
    //挂机异常
    ERROR_ROUND_NO_HALT, 

    //技能
    //升级技能时，技能已达最大等级
    ERROR_SKILL_MAX_LEVEL   = 1200,
    //升阶技能时，技能未达最大等级
    ERROR_SKILL_NO_MAX_LEVEL,
    //技能升阶材料不足
    ERROR_SKILL_NOT_ENOUGH_DRAWING,

    //潜力
    //升级潜能时，潜能已达当前星级的上限
    ERROR_SC_POTENTIAL_CAP  = 1300,
    //升级潜能时，等级未达25级
    ERROR_SC_NOT_ENOUGH_GRADE,

    //BOSS
    //世界boss时，请求进入战斗，但是boss已死
    ERROR_SC_BOSS_DEAD      = 1400,

    //宝石
    //没有宝石表
    ERROR_GEM_NO_REPO       = 1500,
    //当前宝石不存在
    ERROR_GEM_NO_ITEM,
    //材料不足
    ERROR_GEM_LESS_MATERIAL,
    //保护石不足
    ERROR_GEM_LESS_SAFESTONE,
    //元宝不足
    ERROR_GEM_LESS_RMB,

    //竞技场
    //购买超出挑战上限
    ERROR_ARENA_BUY_MAX_FIGHTCOUNT = 1600,
    //挑战次数不足
    ERROR_ARENA_NO_FIGHT_COUNT,
    //无效的挑战目标
    ERROR_ARENA_ILLEGLE_TARGET,
    //竞技场奖励不存在
    ERROR_ARENA_NO_REWARD,

    //任务
    //任务已经存在
    ERROR_TASK_HAS_EXISTED = 1700,
    //任务达到上限
    ERROR_TASK_FULL,
    //任务无效接取
    ERROR_TASK_REQ_INVALID,
    //任务不存在
    ERROR_TASK_NOT_EXISTED,
    //任务已经完成
    ERROR_TASK_NOT_FINISHED,

    //日常任务
    //日常任务已经达到当天最大次数
    ERROR_DTASK_MAX_DO = 1750,

    //公会
    ERROR_GANG_HAS_FULL = 1800,
    //会员不存在
    ERROR_GANG_NO_USER,
    //错误的职位
    ERROR_GANG_FLAG,
    //已经是官员
    ERROR_GANG_HAS_ADM,
    //超出官员上限
    ERROR_GANG_MAX_ADM,
    //用户申请不存在
    ERROR_GANG_NO_ADDREQ,
    //错误的操作目标
    ERROR_GANG_OP_TARGET,
    //操作权限不足
    ERROR_GANG_UNABLE_OP,
    //公会等级不足
    ERROR_GANG_LEVEL,
    //公会名已经存在
    ERROR_GANG_NAME_EXIST,
    //用户创建公会等级不足
    ERROR_GANG_CREATE_LESS_LEVEL,
    //用户创建公会金币不足
    ERROR_GANG_CREATE_LESS_GOLD,
    //捐献金币不足
    ERROR_GANG_PAY_LESS_GOLD,
    //无效的捐献
    ERROR_GANG_PAY,
    //关闭公会失败
    ERROR_GANG_CLOSE,
    //公会解锁技能等级不足
    ERROR_GANG_UNLOCK_SKL_LESS_LEVEL,
    //公会贡献值不足
    ERROR_GANG_LESS_GM,
    //用户学习公会技能等级不足
    ERROR_GANG_LEARN_GSKL_LESS_LEVEL,
    //公会不存在
    ERROR_GANG_NO_GANG,
    //献祭次数不足
    ERROR_GANG_LESS_PRAY_NUM,
    //申请公会被拒绝
    ERROR_GANG_REFUSE_ADDREQ,

    //商城
    //商城的货物id不存在
    ERROR_NO_SHOPID = 1900,
    //出售不存在的装备
    ERROR_SELL_NO_EQUIP,
    //不能出售已经装备的装备
    ERROR_SELL_EQUIPED,
    //出售不存在的道具
    ERROR_SELL_NO_ITEM,

    //DBMID
    ERROR_DB_NO_ACC = 2000,
    ERROR_DB_NO_USER,

    //巨龙宝库
    ERROR_TREASURE_EMPTY_SLOT = 2100,  //该蹲位为空
    ERROR_TREASURE_ROB_ALL,            //该蹲位的打劫次数已满
    ERROR_TREASURE_SLOT_ROBED,         //你已经打劫过该蹲位
    ERROR_TREASURE_HAS_SLOT,           //你已经有蹲位
    ERROR_TREASURE_FULL_SLOT,          //该蹲位不为空
    ERROR_TREASURE_NO_SECONDS,         //你今天的蹲位时间已满
    ERROR_TREASURE_SLOT_CHANGED,       //该蹲位上的人已变化
    ERROR_TREASURE_NO_ROB,             //你今天的打劫次数已满
    ERROR_TREASURE_SLOT_GROWING,       //收割坑位时，尚未成熟
    ERROR_TREASURE_NOT_OPEN,           //不在活动时间段

    //序列号
    ERROR_CDKEY_NOT_EXISTED = 2130,
    ERROR_CDKEY_USED,
    ERROR_CDKEY_INPUTED,

    //乱斗场
    ERROR_SCUFFLE_NOT_OPEN = 2140,    //不在报名时间段
    ERROR_SCUFFLE_REGISTED,           //已经报名
    ERROR_SCUFFLE_NO_REGISTED,        //没有报名,无权进入
    ERROR_SCUFFLE_NO_ENTRANCE,        //不在进入时间段
    ERROR_SCUFFLE_EXIT,               //玩家已经退出该场景
    ERROR_SCUFFLE_TARGET_BATTLE,      //乱斗场挑战时，对方正在战斗中
    ERROR_SCUFFLE_FULL,               //报名人数已满

    //杀星
    ERROR_CITYBOSS_KILLED = 2200,
    ERROR_CITYBOSS_FIGHTMAX = 2201,
    ERROR_CITYBOSS_FIGHTING = 2202,
    ERROR_CITYBOSS_NONE = 2204,

    //关卡星级,星级不足
    ERROR_STARREWARD_NOENOUGH = 2300,

    //DB
    ERROR_DB_EXCEPTION = 3000,
};

#endif
