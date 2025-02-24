#pragma once

#include "GameMacro.h"
#include "GameClassDeclare.h"
#include "GameEnum.h"
#include "FrameDefine.h"
#include "Vector.h"
#include "HashMap.h"
#include "Set.h"
#include "Array.h"
#include "ArrayList.h"
#include "Map.h"
#include "SafeHashMap.h"
#include "SafeVector.h"

// GameDefine,由于使用很频繁,所以简化为GD
class GD
{
public:
	// IP和端口
	static const string QQ_MAIL_SERVER_IP;
	static constexpr int MAIL_SERVER_PORT = 60668;
	static const string LOGIN_GATE_IP;
	static constexpr int LOGIN_GATE_PORT = 60577;
	// 常量字符串定义
	static const string MONSTER_DROP_PATH;
	static const string DROP_PACKAGE_PATH;
	static const string DROP_LIST_PATH;
	// 常量数字定义
	static constexpr float ATTACK_PLAYER_DURA = 0.05f;					// 攻击玩家时全身装备减少的持久度
	static constexpr float ATTACK_MONSTER_DURA = 0.008f;				// 攻击怪物时全身装备减少的持久度
	static constexpr float ATTACK_DURA_ODDS = 0.2f;						// 攻击时装备持久的减少几率
	static constexpr float CRIME_DECREASE = 0.2f;						// 罪恶值减少的速度,每分钟减少0.2的罪恶值
	static constexpr float TEMPTATION_FAILED_INCREASE = 0.1f;			// 诱惑失败时增加的诱惑抗性百分比
	static constexpr float LIFE_ITEM_HUMAN_LUCKY = 0.1f;				// 拥有本命物品时增加的人品值
	static constexpr float BASE_HUMAN_LUCKY = 0.02f;					// 角色基础人品值
	static constexpr float MIN_ATTACK_INTERVAL = 0.2f;					// 释放攻击的最小间隔
	static constexpr float MONSTER_ACTIVE_AI_CHECK_RANGE = 800.0f;		// 怪物指定范围内有玩家才会开启AI
	static constexpr float MINE_STONE_TIME = 5.0f;						// 挖矿时每次获得矿石的间隔时间
	static constexpr float REPAIR_DECREASE_DURA = 1.0f;					// 普通修理时降低的持久度
	static constexpr float GUILD_DISTRIBUTE_RATE = 0.01f;				// 捐献行会基金时获取的贡献度比例
	static constexpr float MAX_CREATE_GOLD_DECREASE = 0.7f;				// 单个角色每日金币产出达到上限时,再次产出金币时金币的减少量
	static constexpr float DIE_LOGIN_HP_MP = 0.1f;						// 角色死亡后登录时的血量魔法量百分比
	static constexpr float NEWER_DIE_LOGIN_HP_MP = 1.0f;				// 新手角色死亡后登录时的血量魔法量百分比
	static constexpr float KILL_LOSE_LUCKY = 0.5f;						// 击杀白名玩家时降低武器幸运的几率
	static constexpr float KILL_LOSE_DURA = 0.5f;						// 击杀白名玩家时降低装备耐久度的几率
	static constexpr float KILL_LOSE_DURA_VALUE = 0.7f;					// 击杀白名玩家时降低装备耐久度的百分比
	static constexpr float WHITE_HIT_RED_PLAYER_DAMAGE = 1.0f;			// 白名玩家攻击红名玩家时所增加的伤害百分比
	static constexpr float RED_HIT_WHITE_PLAYER_DAMAGE = 0.75f;			// 红名玩家攻击白名玩家时所减少的伤害百分比
	static constexpr float VERIFY_DECREASE_GOLD_PERCENT = 0.7f;			// 游戏验证码的超时后收益减少的百分比
	static constexpr float BOSS_OVER_COUNT_DROP_DECREASE = 0.07f;		// 当每日击杀的野外boss超过最低上限时,每多击杀一个boss,爆率降低的百分比
	static constexpr float BOSS_OVER_COUNT_DAMAGE_DECREASE = 0.07f;		// 当每日击杀的野外boss超过最低上限时,每多击杀一个boss,伤害降低的百分比

	// ID类常量
	static constexpr int GOLD_ID = 50059;								// 金币的ID
	static constexpr int GOLD_INGOT_ID = 50060;							// 元宝的ID
	static constexpr int EXP_ID = 50050;								// 经验的ID
	static constexpr int DEFAULT_MALE_MODEL = 1;						// 男角色没有穿衣服时在场景中的模型ID
	static constexpr int DEFAULT_FEMALE_MODEL = 2;						// 女角色没有穿衣服时在场景中的模型ID
	static constexpr int NORMAL_ATTACK_ID = 100;						// 全职业普通攻击的技能ID
	static constexpr int MONSTER_BORN_INVICINBLE_BUFF_DETAIL_ID = 161;	// 怪物出生时的无敌buffID
	static constexpr int DIG_MINE_WEAPON_ID = 30033;					// 挖矿所需要穿戴的武器ID
	static constexpr int WHITE_PLUME_ID = 50079;						// 白色羽毛ID
	static constexpr int GOLDEN_BOX_ID = 10037;							// 赤金宝箱ID
	static constexpr int HUI_CHENG_JUAN = 20018;						// 回城卷的ID
	static constexpr int LUCKY_OIL_ID = 20041;							// 祝福油的表格ID
	static constexpr int WORLD_CHAT_ITEM_ID = 20043;					// 世界发言需要消耗的道具ID
	static constexpr int SPECIAL_CHAT_ITEM_ID = 20044;					// 特殊发言需要消耗的道具ID
	static constexpr int CLEAR_PROFICIENCY_ID = 20049;					// 清除熟练度所需的熟练清洗剂的ID
	static constexpr int MEDAL_56 = 30476;								// 56号荣誉勋章的ID
	static constexpr int SHABAKE_FASHION_MALE = 30427;					// 沙巴克荣耀(男)时装ID
	static constexpr int SHABAKE_FASHION_FEMALE = 30428;				// 沙巴克荣耀(女)时装ID
	static constexpr int GOLD_BAR_ID = 50027;							// 金条的ID
	static constexpr int GOLD_BRICK_ID = 50028;							// 金砖的ID
	static constexpr int GOLD_BOX_ID = 50061;							// 金盒的ID
	static constexpr int STRENGTH_EQUIP_ITEM_ID_0 = 50020;				// 强化装备所需要消耗的道具ID,黑铁原石
	static constexpr int STRENGTH_EQUIP_ITEM_ID_1 = 50062;				// 强化装备所需要消耗的道具ID,黑铁矿石
	static constexpr int GUILD_CREATE_MATERIAL_ID = 50023;				// 创建行会所需材料ID
	static constexpr int EQUIP_STRENGTH_PROTECT_ID = 50064;				// 装备强化保护券的物品ID
	static constexpr int SKILL_EXP_ITEM_0 = 50038;						// 可增加技能经验的物品ID0
	static constexpr int RESET_PRECIOURS_VALUE = 50068;					// 洗炼装备极品属性所需的物品ID
	static constexpr int TRANSFER_PRECIOURS_VALUE = 50069;				// 转换装备极品属性所需的物品ID
	static constexpr int MINE_TIME_ITEM = 50070;						// 兑换挖矿时间所需要的物品ID
	static constexpr int HELL_ITEM_0 = 50071;							// 深渊邀请函ID
	static constexpr int HELL_ITEM_1 = 50072;							// 深渊挑战书ID
	static constexpr int SKILL_STONE_0 = 50073;							// 技能神石
	static constexpr int SKILL_STONE_1 = 50039;							// 技能原石
	static constexpr int NORMAL_OFFER_REWARD_ITEM_ID = 50074;			// 发布普通悬赏令所需要的道具ID
	static constexpr int HIGH_OFFER_REWARD_ITEM_ID = 50075;				// 发布高级悬赏令所需要的道具ID
	static constexpr int AUCTION_DISCOUNT_ITEM_ID = 50076;				// 拍卖行优惠券ID
	static constexpr int CHANGE_NAME_ITEM_ID = 20082;					// 改名卷轴的ID
	static constexpr int PURE_WHITE_PLUME_ID = 50084;					// 纯白羽毛ID
	static constexpr int LIGHT_CORE_ID = 50092;							// 光能核心ID
	static constexpr int LIGHT_CORE_PIECE_ID = 50094;					// 光能核心碎片ID
	static constexpr int MUTATION_POWER = 50095;						// 变异能量的ID
	static constexpr int ITEM_BOX_ID = 50029;							// 宝箱ID
	static constexpr int MEDAL_HEART = 50025;							// 勋章之心的ID
	static constexpr int MEDAL_ELITE = 50089;							// 勋章精华的ID
	static constexpr int HIGH_MEDAL_ELITE = 50090;						// 极品勋章精华的ID
	static constexpr int REVIVE_COIN_ID = 50087;						// 复活币ID
	static constexpr int GREEN_MINE = 50021;							// 绿宝石矿的ID
	static constexpr int PURPLE_MINE = 50022;							// 紫水晶矿的ID
	static constexpr int SHEN_LU_JIE_JING = 50088;						// 神路结晶的ID
	static constexpr int UNKONWN_DARK_PALACE_ID = 280;					// 未知暗殿地图ID
	static constexpr int UNKNOW_DARK_PALACE = 280;						// 未知暗殿的地图ID
	static constexpr int FAIRY_LAND_MAP_4 = 515;						// 幻境4层的地图ID
	static constexpr int NUO_MA_LING_PAI_ID = 50091;					// 诺玛令牌的ID
	static constexpr int TEAM_CALL_ITEM = 50097;						// 组队召唤卷的ID
	static constexpr int GUILD_CALL_ITEM = 50098;						// 行会召唤卷的ID
	static constexpr int DIE_DONT_DROP_ITEM = 50109;					// 死亡防爆券的ID
	static constexpr int NUOMA_MAP_ID = 580;							// 诺玛地图的ID
	static constexpr int TIANMA_MAP_ID = 582;							// 天玛地图的ID
	static constexpr int SHABAK_MAP_ID = 61;							// 沙巴克宫殿地图ID
	static constexpr int FAIRY_LAND_MAP = 512;							// 进入幻境的默认地图ID
	static constexpr int SHENGYU_MAP = 86;								// 有几率传送的圣域地图1ID
	static constexpr int MENG_ZHONG_MAP = 4;							// 盟重的地图ID
	static constexpr int BI_QI_MAP = 1;									// 比奇的地图ID
	static constexpr int DEFAULT_MAP_ID = 1;							// 新建角色初始进入的地图ID,比奇
	static constexpr int RED_DIE_LOGIN_MAP_ID = 1;						// 红名死亡后死亡后重新登录时所在的地图ID,比奇
	static constexpr int NEWER_DIE_LOGIN_MAP_ID = 1;					// 新手死亡后重新登录时所在的地图ID,比奇
	static constexpr int PK_RANK_REWARD_1 = 137;						// 竞技积分排行榜第1的奖励ID
	static constexpr int PK_RANK_REWARD_2 = 138;						// 竞技积分排行榜第2的奖励ID
	static constexpr int PK_RANK_REWARD_3 = 139;						// 竞技积分排行榜第3的奖励ID
	static constexpr int PK_RANK_REWARD_4_10 = 140;						// 竞技积分排行榜第4到第10的奖励ID
	static constexpr int PK_RANK_REWARD_11_20 = 141;					// 竞技积分排行榜第11到第20的奖励ID
	static constexpr int NORMAL_GACHA_ITEM_ID = 50111;					// 普通抽卡所需要的物品ID,碧玉水滴
	static constexpr int HIGH_GACHA_ITEM_ID = 50110;					// 高级抽卡所需要的物品ID,橙光金字塔
	static constexpr int NORMAL_GACHA_POOL_ID = 1;						// 普通抽卡的卡池ID
	static constexpr int HIGH_GACHA_POOL_ID = 2;						// 高级抽卡的卡池ID
	static constexpr int BU_GUI_LU_ID = 23;								// 不归路的副本ID

	// 玩家等级类常量
	static constexpr int GUILD_SALARY_NEED_LEVEL = 35;					// 领取行会酬劳最低需要角色等级35级
	static constexpr int CHAT_MIN_LEVEL = 25;							// 低于25级无法私聊,无法发起好友请求
	static constexpr int RED_ENVELOPE_MIN_LEVEL = 30;					// 需要30级以上的玩家才能抢或者发红包
	static constexpr int CONDITION_LISTEN_LEVEL = 20;					// 如果低于20级,则会查找所有20级以内的条件监听
	static constexpr int CHARACTER_SELL_MIN_LEVEL = 45;					// 上架账号时角色的最低等级
	static constexpr int NEWER_LEVEL = 28;								// 小于等于28级认为是新手
	static constexpr int PK_LEVEL = 35;									// 大于等于35级才可以PK

	// 时间类常量
	static constexpr int GAMA_VERIFY_TIME = 2 * 60;						// 游戏验证码的超时时间,2分钟,超时未验证成功就会进行收益制裁
	static constexpr int GAMA_VERIFY_PUNISH_TIME = 12 * 60 * 60;		// 游戏验证码的超时后的制裁持续事件,12小时
	static constexpr int FRIEND_REQUEST_TIME = 3 * 24 * 3600;			// 添加离线角色为好友的加好友请求保留时间,单位秒,对方超时未确认,请求就会失效
	static constexpr int ITEM_INCREASE_MINE_TIME = 60;					// 一个挖矿许可证可以兑换的挖矿时间
	static constexpr int MAX_MINE_TIME = 1000000;						// 剩余挖矿时间的最大值,100万秒
	static constexpr int AUCTION_SEARCH_INTERVAL = 500;					// 拍卖品的搜索间隔时间500毫秒
	static constexpr int NORMAL_OFFER_REWARD_TIME = 48 * 3600;			// 普通悬赏令的有效时间,48小时
	static constexpr int HIGH_OFFER_REWARD_TIME = 96 * 3600;			// 高级悬赏令的有效时间,96小时
	static constexpr int FREEZE_TIME = 24 * 60 * 60;					// 交易账号的锁定时间
	static constexpr int SUPPOSE_PRICE_TIME_INTERVAL = 5 * 60 * 1000;	// 期望价格操作的间隔时间,5分钟
	static constexpr int SECONDARY_PASSWORD_TIME_INTERVAL = 5;			// 验证二级密码的间隔时间,5秒
	static constexpr int GUILD_REQUEST_TIME = 3 * 24 * 3600;			// 加入行会请求的保留时间,单位秒,会长超时未确认,请求就会失效
	static constexpr int AUCTION_TIME = 24;								// 拍卖品上架的有效时间,固定为24小时,因为需要在查询的同时就能够直接判断拍卖品是否过期,如果不固定则需要先获取拍卖品的有效期,然后才能判断是否过期
	static constexpr int RED_ENVELOPE_TIME = 3600;						// 红包的有效期,1小时
	static constexpr int VERIFY_EXPIRATION = 300;						// 邮箱验证码的有效时间,秒
	static constexpr int VERIFY_FETCH_INTERVAL = 10;					// 邮箱验证码的获取间隔时间,秒
	static constexpr int RESET_2ND_PASSWORD_INTERVAL = 60 * 60 * 24;	// 重置二级密码的间隔时间,24小时
	static constexpr int PK_ARENA_NOT_ALLOW_MATCH = 15 * 60;			// 在匹配或者竞技场中途退出游戏时被禁止再次匹配的时间,15分钟

	// 其他常量
	static constexpr llong RUNTIME_ITEM_ID_START = 100000000LL * 10000;	// 运行时的物品的实例ID起始,为了尽可能与数据库中的物品ID重合,所以设置比较大,1万亿
	static constexpr llong GOLD_INGOT_MAX = 999999999;					// 金币的最大数量,也是元宝,竞技币的数量上限
	static constexpr llong GUILD_GOLD = 10000000000LL;					// 行会基金上限,100亿
	static constexpr int ODDS_SCALE = 10000;							// 几率缩放,所有的几率填写的单位都是万分比
	static constexpr int MONSTER_ODDS_SCALE = 1000000;					// 怪物掉落物品的几率缩放
	static constexpr int CHARACTER_COUNT = 2;							// 每个账号最大的角色数量
	static constexpr int MONSTER_BATCH = 128;							// 获得怪物列表时,一个包中包含的怪物信息最大数量
	static constexpr int NPC_BATCH = 128;								// 获得NPC列表时,一个包中包含的NPC信息最大数量
	static constexpr int CHAR_PACK_GRID = 160;							// 角色最大的背包格子数量,角色当前可用数量小于等于该值
	static constexpr int MAX_WAREHOUSE_SIZE = 160;						// 仓库最大可使用的格子数量
	static constexpr int PACK_DEFAULT_WEIGHT = 5000;					// 背包的初始负重上限
	static constexpr int WAREHOUSE_MAX_GOLD = 10000000;					// 仓库初始最大可存入金币数量
	static constexpr int QUICK_GRID = 6;								// 快捷栏数量
	static constexpr int OBJECT_BATCH = 128;							// 单次请求物品属性的最大数量,当需要请求的数量超过该值时,需要分包请求
	static constexpr int DROP_ITEM_BATCH = 128;							// 一个消息包中可包含的掉落物品信息的数量
	static constexpr int SKILL_INFO_BATCH = 16;							// 一个消息包中可包含的技能信息的数量
	static constexpr int WEAPON_LUCKY = 7;								// 武器的最大幸运值
	static constexpr int STATE_PARAM_COUNT = 13;						// 状态参数的最大数量
	static constexpr int DIRECTION_COUNT_8 = 8;							// 角色方向的数量
	static constexpr int SKILL_COUNT = 32;								// 一个角色最大可学习的技能数量
	static constexpr int MYSQL_SKILL_COUNT = 32;						// 数据库中存储的技能数量上限
	static constexpr int SKILL_LEVEL = 4;								// 技能最大等级
	static constexpr int DROP_COUNT = 256;								// 怪物死亡时掉落物品的最大数量
	static constexpr int MAX_LUCKY = 9;									// 角色最大幸运值
	static constexpr int MIN_LUCKY = -9;								// 角色最低幸运值
	static constexpr int TILE_WIDTH = 48;								// 一块地砖的宽
	static constexpr int TILE_HEIGHT = 32;								// 一块地砖的高
	static constexpr int BLOCK_SIZE = 16;								// 将场景分割为16*16的块,单位为地砖
	static constexpr int SCENE_ITEM_EXIST_TIME = 60 * 1000;				// 场景物品的存在时间,超过时间后将会被销毁,单位毫秒
	static constexpr int SCENE_SKILL_COUNT = 32;						// 向客户端发送当前场景的持续技能信息时一个包中最大的技能数量
	static constexpr int DIE_DROP_PACK_ODDS = 500;						// 被玩家击杀后掉落背包物品的几率,实际几率需要除以ODDS_SCALE
	static constexpr int DIE_DROP_DRESS_ODDS = 300;						// 被玩家击杀后掉落已穿戴物品的几率,实际几率需要除以ODDS_SCALE
	static constexpr int DIE_DROP_PACK_COUNT = 3;						// 被玩家击杀后掉落背包物品可能出现的情况的数量
	static constexpr int DIE_DROP_DRESS_COUNT = 2;						// 被玩家击杀后掉落已穿戴物品可能出现的情况的数量
	static constexpr int DIE_DROP_MAX = 4;								// 死亡后最多的掉落物品数量
	static constexpr int RED_NAME_CRIME = 30;							// 红名的罪恶值
	static constexpr int YELLOW_NAME_CRIME = 20;						// 黄名的罪恶值
	static constexpr int KILL_CRIME = 20;								// 击杀一名玩家增加20点罪恶值
	static constexpr int ATTACK_CRIME = 2;								// 攻击时增加2点罪恶值
	static constexpr int SUMMON_DISTANCE = 500;							// 召唤兽离主人的最远距离,超过此距离时会瞬移到主人旁边
	static constexpr int SUMMON_FLASH_MIN_OFFSET = 30;					// 召唤兽朝主人瞬移后到主人的最近距离
	static constexpr int SUMMON_FLASH_MAX_OFFSET = 70;					// 召唤兽朝主人瞬移后到主人的最远距离
	static constexpr int EQUIP_STAR = 5;								// 装备的最高星级
	static constexpr int DROP_ITEM_POS_OFFSET = 10;						// 掉落到场景中的物品的位置随机偏移范围,单位为像素
	static constexpr int ENVIRONMENT_BATCH = 64;						// 向客户端发送环境角色列表时的一个包的最大数量
	static constexpr int MONSTER_STAND_RADIUS = 20;						// 怪物站立时占据的范围半径,暂时所有怪物都固定为20个像素
	static constexpr int ITEM_IN_BOX = 16;								// 宝箱中最多包含16种道具
	static constexpr int SKILL_BUFF = 16;								// 一个技能所包含的buff数量,最多16个
#ifndef VIRTUAL_CLIENT_TEST
	static constexpr int ACCOUNT_LOGIN_COUNT = 600;						// 最多允许600个账号登录服务器,登录满后无法再登录服务器
#else
	static constexpr int ACCOUNT_LOGIN_COUNT = 6000;					// 虚拟客户端测试时最多允许6000个账号登录服务器,登录满后无法再登录服务器
#endif
	static constexpr int STATE_COUNT = 64;								// 一个角色(怪物或者玩家等所有可战斗的角色)身上最多的状态数量
	static constexpr int SKILL_DISPLAY_COUNT = 12;						// 主界面的技能栏格子数量,也是技能快捷键数量
	static constexpr int SCENE_TRANSFER_COUNT = 64;						// 每个场景最多有64个传送点
	static constexpr int OTHER_PLAYER_BATCH = 16;						// 获取其他玩家列表时,每个包最多包含16个玩家的信息
	static constexpr int UNKONWN_DARK_PALACE_GOLD = 500000;				// 进入未知暗殿需要消耗的金币数量
	static constexpr int UNKONWN_DARK_PALACE_GOLDINGOT = 100;			// 进入未知暗殿需要消耗的元宝数量
	static constexpr int CHAR_PACK_GRID_DEFAULT = 40;					// 角色初始默认的背包格子数量
	static constexpr int WAREHOUSE_SIZE_DEFAULT = 40;					// 角色初始默认的仓库格子数量
	static constexpr int NAME_LENGTH = 8 * 4;							// 名字最大的字节数
	static constexpr int DEBUG_SYSTEM_CMD = 2;							// 调试系统的共享内存命令ID
	static constexpr int REPAIR_DECREASE_DURA_ODDS = 3000;				// 普通修理时降低持久度的几率,实际使用时需要除以
	static constexpr int CHAT_LENGTH = 64 * 4;							// 单条聊天消息的最大字节数
	static constexpr int NPC_TRANSFER_SHENGYU_COST = 100000;			// NPC传送到圣域需要的金币数量
	static constexpr int ACCEPT_MISSION_COUNT = 32;						// 接受任务的最大数量
	static constexpr int MAX_MISSION = 8192;							// 最大的任务ID,必须是4的整数倍
	static constexpr int REWARD_COUNT = 8;								// 任务奖励最大的物品种类数量,包括可选物品列表
	static constexpr int BROADCAST_STRENGTH = 12;						// 强化成功后当强化值大于等于此值时会通知全服所有玩家
	static constexpr int BROADCAST_CONSECRATION = 6;					// 开光成功后当开光等级大于等于此值时会通知全服所有玩家
	static constexpr int BROADCAST_COMPOSITE_STAR = 3;					// 当合成成功大于等于3星的装备时会通知全服所有玩家
	static constexpr int BROADCAST_WEAPON_LUCKY = 5;					// 当使用祝福油将武器成功增加到大于等于5幸运值时会通知全服所有玩家
	static constexpr int NPC_SHOP_GOODS_COUNT = 12;						// NPC商店中一页的物品数量
	static constexpr int REPAIR_COUNT = 16;								// 一次最大的修理装备个数
	static constexpr int SHOW_MAIL_COUNT = 30;							// 最多显示前30条邮件
	static constexpr int MAIL_ITEM_COUNT = 10;							// 邮件中附件的最大数量
	static constexpr int MAIL_MAX_COUNT = 128;							// 角色邮箱中最大可存放的邮件数量,超出数量时将无法接收邮件
	static constexpr int PLAYER_AUCTION_COUNT = 32;						// 每个玩家最多可上架的拍卖品数量
	static constexpr int AUCTION_PAGE_SIZE = 10;						// 拍卖行一页可显示的物品数量
	static constexpr int FRIEND_COUNT = 100;							// 玩家好友数量上限
	static constexpr int BLACK_COUNT = 100;								// 玩家黑名单数量上限
	static constexpr int FRIEND_HELLO_LENGTH = 30 * 4;					// 添加好友的打招呼请求内容最大字节长度,一个字符最多占4个字节
	static constexpr int TEAM_MEMBER = 6;								// 一个队伍最多有6个人
	static constexpr int GUILD_MEMBER = 200;							// 行会最多的成员数量
	static constexpr int GUILD_LIST_BATCH = 128;						// 行会列表的一个消息包中所包含的最大行会数量
	static constexpr int GUILD_DISTRIBUTE = 10000000;					// 一次捐献最多可捐献的金币数
	static constexpr int PLAYER_CONTRIBUTION = 1000000000;				// 玩家行会贡献度的上限,10亿
	static constexpr int GUILD_MEMBER_BATCH = 128;						// 获取行会成员列表时一个消息中包含的成员
	static constexpr int GUILD_REQUEST = 32;							// 行会请求堆积的最大数量,达到上限时,将不能再请求加入行会
	static constexpr int GUILD_SHOP_GOODS_COUNT = 12;					// 行会商店中一页的物品数量
	static constexpr int EXCHANGE_ITEM_COUNT = 10;						// 交易物品类型的最大数量
	static constexpr int ACHIVEMENT_COUNT = 128;						// 一个消息包中包含的成就信息的最大数量
	static constexpr int RANK_PAGE = 10;								// 排行榜一页显示的玩家数量
	static constexpr int RANK_PLAYER_COUNT = 100;						// 排行榜只显示前100名玩家
	static constexpr int EQUIP_COLLECTION_ID_BLOCK = 10000;				// 装备收藏表中每个角色可用的ID段长度
	static constexpr int MIN_KILL_PROFICIENCY = 1;						// 击杀怪物获得的装备熟练度下限
	static constexpr int MAX_KILL_PROFICIENCY = 3;						// 击杀怪物获得的装备熟练度上限
	static constexpr int SHOPPING_MALL_GOODS_COUNT = 24;				// 商城中一页的物品数量
	static constexpr int DEFAULT_POSITION_X = 30876;					// 新建角色初始的像素坐标X,银杏村庄
	static constexpr int DEFAULT_POSITION_Y = 2662;						// 新建角色初始的像素坐标Y,银杏村庄
	static constexpr int RED_DIE_LOGIN_MIN_X = 284;						// 红名死亡后重新登录时的地砖下标X最小范围
	static constexpr int RED_DIE_LOGIN_MAX_X = 290;						// 红名死亡后重新登录时的地砖下标X最大范围
	static constexpr int RED_DIE_LOGIN_MIN_Y = 616;						// 红名死亡后重新登录时的地砖下标Y最小范围
	static constexpr int RED_DIE_LOGIN_MAX_Y = 622;						// 红名死亡后重新登录时的地砖下标Y最大范围
	static constexpr int NEWER_DIE_LOGIN_MIN_X = 640;					// 新手死亡后重新登录时的地砖下标X最小范围
	static constexpr int NEWER_DIE_LOGIN_MAX_X = 648;					// 新手死亡后重新登录时的地砖下标X最大范围
	static constexpr int NEWER_DIE_LOGIN_MIN_Y = 617;					// 新手死亡后重新登录时的地砖下标Y最小范围
	static constexpr int NEWER_DIE_LOGIN_MAX_Y = 620;					// 新手死亡后重新登录时的地砖下标Y最大范围
	static constexpr int EXPAND_BACK_PACK_GOLDINGOT = 500;				// 扩容一次背包所需要消耗的元宝数量
	static constexpr int EXPAND_BACK_PACK_SIZE = 8;						// 扩容一次背包所增加的格子数量
	static constexpr int EXPAND_WAREHOUSE_GOLDINGOT = 500;				// 扩容一次仓库所需要消耗的元宝数量
	static constexpr int EXPAND_WAREHOUSE_SIZE = 8;						// 扩容一次仓库所增加的格子数量
	static constexpr int DECOMPOSE_COUNT = 4;							// 分解物品可获得的物品种类的最大数量
	static constexpr int DECOMPOSE_GOLD = 10000000;						// 分解可获得的最大金币数量,1000万
	static constexpr int ENTER_FAIRY_LAND_GOLD = 500000;				// 进入幻境所需要消耗的金币
	static constexpr int STRENGTHEN_RECORD = 15;						// 强化大于等于多少时就将玩家的名字记录到装备上
	static constexpr int QUICK_SALE_GOLDINGOT_PRICE = 1;				// 购买一次快速出售所需要的元宝数量
	static constexpr int QUICK_DECOMPOSE_GOLDINGOT_PRICE = 2;			// 购买一次快速分解所需要的元宝数量
	static constexpr int QUICK_SALE_COUNT_MAX = 99999;					// 快速出售剩余次数的上限
	static constexpr int QUICK_DECOMPOSE_COUNT_MAX = 99999;				// 快速分解剩余次数的上限
	static constexpr int SECONDARY_LENGTH = 8;							// 二级密码固定8位
	static constexpr int OFFER_REWARD_ACCEPT_COUNT = 5;					// 最多只能同时接5个悬赏令
	static constexpr int OFFER_REWARD_PUTON_COUNT = 10;					// 最多只能同时发布10个悬赏令
	static constexpr int OFFER_REWARD_PAGE = 13;						// 一页最多显示13个悬赏令
	static constexpr int NORMAL_OFFER_REWARD_GOLD = 500000;				// 发布普通悬赏令所需要的金币数量
	static constexpr int HIGH_OFFER_REWARD_GOLD = 5000000;				// 发布高级悬赏令所需要的金币数量
	static constexpr int SKILL_STONE_TO_HELL_ITEM = 15;					// 1个技能神石可以兑换15个深渊邀请函
	static constexpr int AUCTION_DISCOUNT_COUNT = 5;					// 1个拍卖行优惠券可兑换5次拍卖行优惠次数
	static constexpr int AUCTION_DISCOUNT_MAX = 999999;					// 最大的拍卖行优惠次数
	static constexpr int BOSS_DROP_PROTECT_COUNT_MAX = 999999;			// 最大的boss掉落物品保护次数
	static constexpr int DEBUG_INFO_LENGTH = 512;						// 调试信息的最大长度
	static constexpr int EQUIP_COLLECTION_BATCH = 128;					// 向客户端发送装备收藏列表时单个消息包的信息数量
	static constexpr int EQUIP_COUNT_PER_PAGE = 10;						// 向客户端发送全服装备掉落数量时的最大长度
	static constexpr int SERVER_HELP_LENGTH = 4096;						// 向客户端发送的GM命令列表信息的最大长度
	static constexpr int MESSAGE_LENGTH = 256;							// 服务器发送的字符串消息的最大字节数
	static constexpr int TERRITORY_SIGN_UP_GUILD_COUNT = 10;			// 每个地图最多允许10个行会进行报名争夺
	static constexpr int TERRITORY_SIGN_UP_GOLD = 20000000;				// 报名行会领地争夺所需要的行会基金数量,2000万
	static constexpr int SHABAKE_BATTLE_SIGN_UP_GOLD = 30000000;		// 报名沙巴克攻城所需要的行会基金数量,3000万
	static constexpr int SHOW_TERRITORY_SCORE_COUNT = 30;				// 向客户端返回领地争夺活动当前行会成员的积分列表的最大显示数量
	static constexpr int WHITE_PLUME_EXCHANGE_COUNT = 1000;				// 1000个白色羽毛可兑换1个纯白羽毛
	static constexpr int LIGHT_CORE_EXCHANGE_COUNT = 10;				// 10个光能核心碎片可兑换1个光能核心
	static constexpr int BOSS_DROP_PROTECT_PRICE = 50000;				// boss掉落保护的购买价格,5万金币一次
	static constexpr int RED_ENVELOPE_PAGE_SIZE = 10;					// 一页红包的数量
	static constexpr int MAX_ENVELOPE_PACKET_COUNT = 100;				// 红包的最大个数
	static constexpr int MIN_ENVELOPE_PACKET_COUNT = 5;					// 红包的最小个数
	static constexpr int ACCOUNT_DESCRIPTION_LENGTH = 512 * 4;			// 账号出售描述信息的最大字节数量
	static constexpr int TREASUREHOUSE_PAGE = 6;						// 一页账号出售信息的数量
	static constexpr int ACCOUNT_SELL_MIN_PRICE = 30;					// 账号出售的最低价格
	static constexpr int WANT_BUY_PAGE = 10;							// 意向玩家列表每页的数量
	static constexpr int NUO_MA_UNLOCK_ITEM_COUNT = 200;				// 解锁诺玛地图需要消耗200个神路结晶
	static constexpr int NUO_MA_UNLOCK_GOLD = 20000000;					// 解锁诺玛地图需要消耗2000万金币
	static constexpr int NUO_MA_ENTER_GOLD = 100000;					// 进入诺玛地图需要消耗10万金币
	static constexpr int TIAN_MA_UNLOCK_ITEM_COUNT = 150;				// 解锁天玛地图需要消耗150个诺玛令牌
	static constexpr int IP_REGISTE_ACCOUNT_COUNT = 3;					// 同一个ip最多允许注册3个账号
	static constexpr int OBJECT_LABEL_LENGTH = 32;						// 物品名称的最大长度,字节数
	static constexpr int LOTTERY_TICKET_NUMBER = 3;						// 彩票数字的个数
	static constexpr int LOTTERY_TICKET_MONEY_MIN = 100000;				// 彩票下注最低金额,10万
	static constexpr int LOTTERY_TICKET_MONEY_MAX = 1000000;			// 彩票下注最高金额,100万
	static constexpr int DAY_BOSS_LIMIT = 30;							// 每日的野外boss击杀上限为30个,超过上限后对boss伤害和爆率都会降低
	static constexpr int ADVECNTURE_RANDOM_COUNT = 4;					// 每次只随机出4个奇遇任务
	static constexpr int POWER_PER_MUTATION_POWER = 10;					// 一个变异能量可以补充的能量
	static constexpr int NORMAL_GACHA_ITEM_COUNT = 10;					// 普通抽卡所需要的物品数量
	static constexpr int HIGH_GACHA_ITEM_COUNT = 10;					// 高级抽卡所需要的物品数量

	// 数组或列表类
	static Array<DIE_DROP_PACK_COUNT, int> DIE_DROP_PACK_COUNT_LIST;	// 玩家死亡后背包物品掉落数量列表
	static Array<DIE_DROP_DRESS_COUNT, int> DIE_DROP_DRESS_COUNT_LIST;	// 玩家死亡后已穿戴装备掉落数量列表
	static Array<DIE_DROP_PACK_COUNT, float> DIE_DROP_PACK_ODDS_LIST;	// 玩家死亡后背包物品掉落几率列表
	static Array<DIE_DROP_DRESS_COUNT, float> DIE_DROP_DRESS_ODDS_LIST;	// 玩家死亡后已穿戴装备掉落几率列表
	static Array<10, int> WEAPON_CONSECRATION_BUFF;						// 武器开光的buffID列表,开光等级固定10级
	static HashMap<CHARACTER_PROPERTY, PROPERTY_VALUE_TYPE> PROPERTY_VALUE_TYPE_MAP;	// 角色属性的数据类型
	static HashMap<int, int> DIG_MINE_ODDS;								// 挖矿几率列表,first是该几率的个数,second是权重
	static HashMap<int, int> MINE_ID_ODDS;								// 挖矿获得的物品几率列表,first是该几率的物品ID,second是权重
	static HashMap<float, int> NPC_SELL_RATIO_ODDS;						// NPC出售物品倍率的随机几率,first是出售倍率,second是权重
	static Array<TEAM_MEMBER, float> TEAM_EXP_RATIO;					// 组队时各个人数对应的经验分配比例
	static HashMap<int, int> BLESS_EQUIP_ODDS;							// 祝福装备的几率列表,first表示祝福值,second是几率
	static HashMap<int, float> COMPOSITE_STAR_EQUIP_ODDS;				// 星级装备的合成成功率
	static HashMap<int, int> COMPOSITE_STAR_EQUIP_GOLD;					// 星级装备合成所需金币数量
	static HashMap<int, int> EQUIP_EXTRA_ATTACK_ODDS;					// 装备附加随机攻击力时的每个阶段的几率
	static HashMap<int, int> ENTER_FAIRY_LAND_ITEM;						// 进入幻境所需的物品以及数量
	static HashMap<MONSTER_STRENGTH, Vector2Int> MONSTER_PROFICIENCY;	// 击杀不同强度类型的怪获得的熟练度随机范围
	static HashMap<int, int> SPECIAL_RING_LIST;							// 特戒列表,以及合成几率
	static HashMap<int, float> COMPOSITE_SPECIAL_RING;					// 各个星级随机合成时出特戒的几率
	static HashMap<int, int> GUILD_ITEM_CONTRIBUTE;						// 行会捐赠物品ID和可获得的贡献值
	static HashMap<int, int> GUILD_ITEM_EXP;							// 行会捐赠物品ID和可获得的行会经验
	static Vector<int> HEI_AN_DA_DIAN_PK_SCORE;							// 进入各个层的黑暗大殿所需的pk分
	static Vector<int> HEI_AN_DA_DIAN_MAP_ID;							// 黑暗大殿地图ID
	static Vector<int> WU_JIN_JIAN_TA_ITEM;								// 无尽尖塔扫荡所需材料ID
};