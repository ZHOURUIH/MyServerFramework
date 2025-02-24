#pragma once

#include "FrameDefine.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 枚举定义
// 角色类型
class CHARACTER_TYPE
{
public:
	static constexpr byte NONE = 0;			// 无效值
	static constexpr byte PLAYER = 1;		// 玩家
	static constexpr byte MONSTER = 2;		// 怪物
	static constexpr byte NPC = 3;			// NPC
	static constexpr byte ENVIRONMENT = 4;	// 环境
	static constexpr byte MAX = 5;			// 最大值,用于获取类型数量
};

// 玩家的超级权限,用于调试
enum class SUPER_PERMISSION : byte
{
	NO,				// 无权限
	GM,				// GM权限,可执行所有命令
	NORMAL_ADMIN,	// 普通管理员权限,可执行部分命令
};

// 角色装备位的类型
enum class EQUIP_POSITION : byte
{
	CLOTH,			// 衣服
	WEAPON,			// 武器
	HELMET,			// 头盔
	NECKLACK,		// 项链
	MEDAL,			// 勋章
	LEFT_BRACELET,	// 左手镯
	RIGHT_BRACELET, // 右手镯
	LEFT_RING,		// 左戒指
	RIGHT_RING,		// 右戒指
	BELT,			// 腰带
	SHOE,           // 鞋子
	DIAMOND,		// 宝石
	WING,			// 翅膀
	FASHION_CLOTH,	// 时装衣服
	FASHION_WEAPON,	// 时装武器
	FASHION_WING,	// 时装翅膀
	MAX,
};

// 装备类型
enum class EQUIP_TYPE : byte
{
	CLOTH,			// 衣服
	WEAPON,			// 武器
	HELMET,			// 头盔
	NECKLACK,		// 项链
	MEDAL,			// 勋章
	BRACELET,		// 手镯
	RING,			// 戒指
	BELT,			// 腰带
	SHOE,           // 鞋子
	DIAMOND,		// 宝石
	WING,			// 翅膀
	FASHION_CLOTH,	// 时装衣服
	FASHION_WEAPON,	// 时装武器
	FASHION_WING,	// 时装翅膀
	MAX,
};

// 玩家移动状态类型,其实是表示一个持续性动作
enum class PLAYER_MOVE_STATE : byte
{
	NONE,			// 无效值
	STAND,			// 站立
	WALK,			// 走路
	RUN,			// 奔跑
	DIE,			// 死亡
	DIG,			// 挖矿
	SEARCH,			// 搜寻
};

// 玩家职业
enum class PLAYER_JOB : byte
{
	NONE,			// 无效值,或者装备上的适用于所有职业
	FIGHTER,		// 战士
	MAGE,			// 法师
	TAOIST,			// 道士
};

// 玩家性别,因为装备上也会有适用性别的属性,所以有无性别的类型
enum class PLAYER_SEX : byte
{
	NO_SEX,			// 无效值
	MALE,			// 男
	FEMALE,			// 女
};

// 枚举值对应Magic表格中的ID
enum class PLAYER_SKILL : byte
{
	NONE = 0,
	BASE_ATTACK = 100,				// 普攻
	// 战士
	GONG_SHA_JIAN_SHU = 1,			// 攻杀剑术
	JI_BEN_JIAN_SHU = 4,			// 基本剑术
	CI_SHA_JIAN_SHU = 12,			// 刺杀剑术
	LIE_HUO_JIAN_FA = 26,			// 烈火剑法
	YE_MAN_CHONG_ZHUANG = 27,		// 野蛮冲撞
	// 法师
	KANG_JU_HUO_HUAN = 8,			// 抗拒火环
	LEI_DIAN_SHU = 11,				// 雷电术
	YOU_HUO_ZHI_GUANG = 20,			// 诱惑之光
	SHUN_XI_YI_DONG = 21,			// 瞬息移动
	HUO_QIANG = 22,					// 火墙
	BAO_LIE_HUO_YAN = 23,			// 爆裂火焰
	DI_YU_LEI_GUANG = 24,			// 地狱雷光
	MO_FA_DUN = 31,					// 魔法盾
	BING_PAO_XIAO = 33,				// 冰咆哮
	// 道士
	ZHI_YU_SHU = 3,					// 治愈术
	JING_SHEN_LI_ZHAN_FA = 5,		// 精神力战法
	SHI_DU_SHU = 7,					// 施毒术
	LING_HUN_HUO_FU = 13,			// 灵魂火符
	FANG_YU_SHU = 15,				// 防御术
	KUN_MO_ZHOU = 16,				// 困魔咒
	ZHAO_HUAN_KU_LOU = 17,			// 召唤骷髅
	ZHAO_HUAN_SHEN_SHOU = 30,		// 召唤神兽
};

// 角色属性的数据类型
enum class PROPERTY_VALUE_TYPE : byte
{
	INT,
	FLOAT,
};

// 角色属性类型
enum class CHARACTER_PROPERTY : char
{
	NONE = -1,					// 无效值
	HP,							// 血量
	MAX_HP,						// 最大血量
	MP,							// 魔法量
	MAX_MP,						// 最大魔法量
	MIN_PHYSICS_ATTACK,			// 最小物理攻击力
	MAX_PHYSICS_ATTACK,			// 最大物理攻击力
	MIN_MAGIC_ATTACK,			// 最小魔法攻击力
	MAX_MAGIC_ATTACK,			// 最大魔法攻击力
	MIN_SORCERY_ATTACK,			// 最小道术攻击力
	MAX_SORCERY_ATTACK,			// 最大道术攻击力
	MIN_PHYSICS_DEFENCE,		// 最小物理防御力
	MAX_PHYSICS_DEFENCE,		// 最大物理防御力
	MIN_MAGIC_DEFENCE,			// 最小魔法防御力
	MAX_MAGIC_DEFENCE,			// 最大魔法防御力
	ATTACK_SPEED,				// 攻击速度
	MOVE_SPEED,					// 移动速度
	HIT_PROBABILITY,			// 命中率
	EVADE_PROBABILITY,			// 闪避率
	CRITICAL_PROBABILITY,		// 暴击率
	CRITICAL_DAMAGE_RATE,		// 暴击伤害
	DECREASE_DAMAGE_RATE,		// 减伤率
	INCREASE_DAMAGE_RATE,		// 增伤率
	CRIME,						// 罪恶值
	LUCKY,						// 幸运值
	HP_RECOVER,					// 血量回复速度
	MP_RECOVER,					// 魔法量回复速度
	PHYSICS_DECREASE,			// 物理免伤率
	MAGIC_DECREASE,				// 魔法免伤率
	SORCERY_DECREASE,			// 道术免伤率
	DIG_MINE_SPEED,				// 挖矿速度
	SKILL_ATTACK_INCREASE,		// 技能攻击力增幅
	ANTI_CRITICAL,				// 暴击抗性
	COUNTER_ATTACK,				// 破招攻击力
	COUNTER_PHYSICS_ATTACK,		// 破招物理攻击力
	COUNTER_MAGIC_ATTACK,		// 破招魔法攻击力
	COUNTER_SORCERY_ATTACK,		// 破招道术攻击力
	PACK_MAX_WEIGHT,			// 背包最大负重
	NO_DEFENCE_PHYSICS_ATTACK,	// 无视防御的物理攻击力
	NO_DEFENCE_MAGIC_ATTACK,	// 无视防御的魔法攻击力
	NO_DEFENCE_SORCERY_ATTACK,	// 无视防御的道术攻击力
	MONSTER_EXP_MULTI,			// 怪物经验倍数
	SKILL_EXP_MULTI,			// 技能经验倍数
	PICK_GOLD_MULTI,			// 拾取金币倍数
	PICK_GOLD_INGOT_MULTI,		// 拾取元宝倍数
	EQUIP_STRENGTH,				// 全身装备强化总值
};

// 物品的总分类
enum class OBJECT_ITEM : byte
{
	NONE,					// 无效类型
	BOX,					// 宝箱
	CONSUMABLE,				// 消耗品
	EQUIP,					// 装备
	SKILL_BOOK,				// 技能书
	MATERIAL,				// 材料
	CARD,					// 怪物卡片
	MAX,					// 最大值
};

// 怪物种族
enum class MONSTER_RACE : byte
{
	NONE,					// 无种族
	UNDEAD,					// 不死族
};

// 一块地砖中的三角形定义
enum class TILE_TRIANGLE : byte
{
	LEFT_TOP,				// 左上角
	RIGHT_TOP,				// 右上角
	RIGHT_BOTTOM,			// 右下角
	LEFT_BOTTOM,			// 左下角
	INNER_LEFT_TOP,			// 中心左上角
	INNER_RIGHT_TOP,		// 中心右上角
	INNER_RIGHT_BOTTOM,		// 中心右下角
	INNER_LEFT_BOTTOM,		// 中心左下角
	MAX,
};

// 伤害类型
enum class DAMAGE_TYPE : byte
{
	PHYSICS,		// 物理伤害
	MAGIC,			// 魔法伤害
	SORCERY,		// 道术伤害
	MAX,			// 最大值,也表示无效值
};

// 血量变化的类型
enum class HP_DELTA : byte
{
	NORMAL,						// 普通伤害
	CRITICAL,					// 暴击普通伤害
	COUNTER_ATTACK,				// 破招伤害
	CRITICAL_COUNTER_ATTACK,	// 暴击破招伤害
	RECOVER,					// 普通回血
	MISS,						// 被闪避
	ADDITIVE,					// 附加伤害
	BEEN_ATTACK,				// 被攻击伤害
	INVINCIBLE,					// 免疫伤害
	COUNTER_INJURY,				// 反弹伤害,只针对产生反弹伤害的来说,反弹出去的伤害类型仍然是普通伤害
	KILL_PERCENT,				// 百分比减血
	SUCK_BLOOD,					// 吸血来回复血量
	BE_MISS,					// 自己闪避了他人的攻击
};

// 物品的品质类型
enum class ITEM_QUALITY : byte
{
	NONE,			// 无效值
	NORMAL,			// 普通物品,白
	HIGH,			// 高级物品,蓝
	PRECIOUS,		// 稀有物品,紫
	BEST,			// 极品物品,粉
	LEGEND,			// 传奇物品,橙
};

// 物品掉落到场景中的方式
enum class DROP_TYPE : byte
{
	DIE_DROP,		// 死亡后掉落物品
	MANUAL_DROP,	// 手动丢弃物品
};

// 请求其他玩家的数据的用途
enum class REQUEST_PLAYER_PURPOSE : byte
{
	NONE,				// 无效值
	SCENE_PLAYER,       // 用于显示场景中的其他玩家
	DATA_VIEW,          // 仅仅获取玩家的信息,用于查看信息
	CONTEXT_MENU,		// 显示菜单
	TEAM_VIEW,			// 用于显示队伍中玩家的信息
	PK_ARENA,			// 用于显示PK竞技场中玩家信息
};

// 怪物强度类型
enum class MONSTER_STRENGTH : byte
{
	NORMAL,				// 普通小怪
	ELITE,				// 精英怪
	BOSS,				// 教主
	SPECIAL,			// 特殊怪物
	MAX,				// 表示无效值
};

// 物品的交易类型
enum class TRADE_TYPE : byte
{
	NONE,			// 无效值
	CAN_TRADE,		// 可交易
	DONT_TRADE,		// 不可交易
};

// 物品的功能类型
enum class ITEM_FUNCTION : byte
{
	NONE,				// 无效值
	MEDICINE,			// 药品
};

// 罪恶等级
enum class CRIME_LEVEL : byte
{
	NONE,			// 白名
	GREY,			// 灰名
	YELLOW,			// 黄名
	RED,			// 红名
};

// 起点和终点类型
enum class START_TARGET_POINT : byte
{
	NONE,						// 代码自定义起点和终点
	SELF_BODY,					// 起点和终点都是自身身体
	SELF_FOOT,					// 起点和终点都是自身脚底
	TARGET_BODY,				// 起点和终点都是目标身体
	TARGET_FOOT,				// 起点和终点都是目标脚底
	SELF_BODY_TO_TARGET_BODY,	// 从自身身体到目标身体
	SELF_BODY_TO_TARGET_FOOT,	// 从自身身体到目标脚底
};

// 子弹独立寻敌模式类型,因为一个技能包含多个子弹,每个子弹都可能会作用于不同的目标
// 所以需要指定子弹单独的目标
enum class BULLET_TARGET_TYPE : byte
{
	NONE,
	SELF,        // 子弹作用于自身
	TARGET,      // 当前技能目标
	RANDOM,      // 周围随机目标
};

// 聊天气泡组类型
enum class BUBBLE_GROUP : byte
{
	RANDOM_ONCE,        // 从组中随机选择一句进行显示
	SEQUENCE,           // 按顺序显示组中所有的气泡
	CONVERSATION,		// NPC之间的对话
};

// 怪物创建时默认的状态类型
enum class MONSTER_DEFAULT_STATE : byte
{
	NONE,
	STAND,		// 站立状态
	UNBORN,		// 未出生状态
};

// 怪物行为树类型
enum class MONSTER_DECISION_TREE : byte
{
	NONE,					// 无效值
	DEFAULT,				// 默认类型
	DA_DAO,					// 大刀护卫
	GONG_JIAN_SHOU,			// 弓箭手
	SHI_REN_HUA,			// 食人花
	XIE_ZI,					// 蝎子,对应有两个技能的怪,每次会随机释放一个技能
	HUAN_YING_ZHI_ZHU,		// 幻影蜘蛛
	BAO_LIE_ZHI_ZHU,		// 爆裂蜘蛛
	PA_XING_JIANG_SHI,		// 爬行僵尸,可复活
	ZU_MA_DIAO_XIANG,		// 祖玛雕像
	SHEN_SHOU,				// 神兽,可用于所有召唤兽
};

// 角色行为定义
enum class CHARACTER_BEHAVIOUR : byte
{
	NONE,					// 无效值
	SHEN_FA_LEI,			// 环境角色,神罚雷
};

// PK模式
enum class PK_MODE : byte
{
	PEACE,					// 和平模式,己方是自己,敌方是怪物
	TEAM,					// 组队模式,己方是所有队友,敌方是其他玩家及怪物
	GUILD,					// 行会模式,己方是自己行会的所有成员,敌方是行会以外玩家及怪物
	KILL_EVIL,				// 善恶模式,己方是自己,敌方是其他非白名玩家及怪物
	KILL_ALL,				// 全体模式,己方是自己,敌方是其他所有玩家及怪物
	OFFER_REWARD,			// 悬赏模式,己方是自己,敌方是其他所有已接受悬赏的玩家
	BLACK_LIST,				// 黑名单模式,己方是自己,敌方是黑名单中的所有玩家
	MAX,					// 最大值
};

// 怪物的寻敌方式
enum class MONSTER_SEARCH : byte
{
	NONE,
	NEAREST,					// 寻找最近的敌人
	HIT_SELF_FIRST_RED_NAME,	// 优先寻找攻击过自己的,或者红名玩家
	RED_NAME,					// 寻找红名玩家
};

// 怪物职责类型
enum class MONSTER_DUTY : byte
{
	NONE,		// 无效值
	ATTACK,		// 攻击玩家,也就是普通怪物的职责
	GUARD,		// 守卫,大刀,带刀怪物的职责
};

// 子弹生效阵营
enum class CAMP_TYPE : byte
{
	NONE,		// 无效值
	FRIEND,		// 友方
	ENEMY,		// 敌方
};

// 查找角色的范围
enum class CHARACTER_RANGE : byte
{
	NONE,				// 无效值
	PLAYER,				// 寻找玩家
	MONSTER,			// 寻找怪物,包含召唤兽
	SUMMON,				// 寻找召唤兽
	NORMAL_MONSTER,		// 寻找非召唤兽怪物
};

// 伤害免疫类型
enum class INVINCIBLE_TYPE : byte
{
	NORMAL,				// 免疫普通伤害
	PHYSICS,			// 免疫物理伤害
	MAGIC,				// 免疫魔法伤害
	SORCERY,			// 免疫道术伤害
	CRITICAL,			// 免疫暴击伤害
	COUNTER_ATTACK,		// 免疫破招伤害
};

// 装备穿戴操作类型
enum class DRESS_OPERATE : byte
{
	DRESS,				// 穿戴装备
	UNDRESS,			// 卸下装备
	REPLACE_POSITION,	// 在已穿戴装备中调换位置
};

// 环境的寻敌方式
enum class ENVIRONMENT_SEARCH : byte
{
	NONE,				// 无效值
	NEAREST,			// 寻找最近的玩家
};

// 技能目标寻敌模式类型
enum class PLAYER_SEARCH_TARGET : byte
{
	NEAREST,        // 寻找最近的敌人作为目标
	SELF,           // 将自己作为目标
};

// 玩家动作类型
enum class ANIMATION : byte
{
	NONE,           // 无效值
	ATTACK,         // 普攻
	SKILL,          // 技能
	RUN,            // 奔跑
	WALK,           // 行走
	DIE,            // 死亡
	HIT,            // 被击
	STAND,          // 站立
	DIG,            // 挖矿
	SEARCH,         // 搜寻
	JUMP_ATTACK,    // 跳跃攻击
	RUSH_ATTACK,    // 野蛮冲撞
};

// 装备额外攻击力的类型
enum class EQUIP_EXTRA_ATTACK : byte
{
	NONE,			// 没有附加攻击力
	PHYSICS,		// 可附加物理攻击力
	MAGIC,			// 可附加魔法攻击力
	SORCERY,		// 可附加道术攻击力
	ALL,			// 可附加三种攻击力
};

// 怪物系列枚举
enum class MONSTER_SERIES : byte
{
	NONE,		// 无
	KULOU,		// 骷髅洞系列怪物
	WOMA,		// 沃玛系列怪物
	ZHUDONG,	// 猪洞系列怪物
	ZUMA,		// 祖玛系列怪物
	FENGMO,		// 封魔系列怪物
	SHIMO,		// 尸魔系列怪物
	GUMO,		// 骨魔系列怪物
	CHIYUE,		// 赤月系列怪物
	NIUMO,		// 牛魔系列怪物
};

// 宝箱类物品的宝箱类型
enum class ITEM_BOX_TYPE : byte
{
	NONE,		// 无效值
	FIXED,		// 只包含固定物品
	RANDOM,		// 可开出随机物品
};

// 装备标记,表示装备的特别分类吧
enum class EQUIP_FLAG : byte
{
	NONE,			// 无效值
	SPECIAL_RING,	// 特戒
};

// 召唤兽的控制行为
enum class SUMMON_CONTROL : byte
{
	IDLE,				// 进入待机状态
	ATTACK,				// 进入战斗状态
	BACK,				// 回到身边
};

// 客户端设备平台类型
enum class CLIENT_PLATFORM : byte
{
	NONE,			// 无效值
	ANDROID,		// 安卓
	PC,				// windows
	IOS,			// ios
};

// 刷怪区域的类型
enum class MONSTER_GENERATE : byte
{
	NONE,			// 无效值
	FILL,			// 隔一定范围内的随机时间,将怪物补充到一定数量
	CYCLE,			// 隔一定范围内的随机时间,刷新固定数量的怪物,直到达到刷新次数
	WU_JIN_JIAN_TA,	// 无尽尖塔中使用额刷怪区域
};

// 罪恶值增加的原因
enum class INCREASE_CRIME_REASON : byte
{
	NONE,					// 无效值
	KILL_PLAYER,			// 击杀玩家
	HIT_NO_CRIME_PLAYER,	// 攻击没有罪恶值的玩家
};

// 触发类buff中所指定的触发技能命中类型
enum class TRIGGER_SKILL_DAMAGE : byte
{
	NONE,			// 无效值
	DAMAGE,			// 伤害性技能
	NON_DAMAGE,		// 非伤害性技能
	ALL_DAMAGE,		// 所有技能
};

// 场景所属的类型
enum class SCENE_TYPE : byte
{
	NONE,			// 无效值
	WORLD,			// 公共的世界
	SCENE_COPY,		// 副本
	ARENA,			// 竞技场
};

// pk匹配的结果类型
enum class PK_MATCH_RESULT : byte
{
	NONE,			// 无效值
	SUCCESS,		// 匹配成功
	TIMEOUT,		// 超时导致失败
	CANCEL,			// 匹配被取消
};

// PK竞技场类型
enum class PK_ARENA : byte
{
	NONE,			// 无效值
	SINGLE,			// 个人竞技
};

// 单人竞技场的阶段类型
enum class PK_ARENA_SINGLE_STAGE : byte
{
	NONE,			// 无效值
	ENTERING,		// 参与战斗的玩家正在进入竞技场地图
	COUNTING,		// 正在进行倒计时
	FIGHTING,		// 正在战斗中
	ENDING,			// 战斗已结束
	SETTLEMENT,		// 结算中
};

// 货币的类型,也就是可以用来买东西的货币
enum class MONEY_TYPE : byte
{
	NONE,			// 无效值
	GOLD,			// 金币
	GOLD_INGOT,		// 元宝
	PK_GOLD,		// 竞技币
	CONTRIBUTION,	// 行会贡献
};

// 触发类的buff移除附加buff的规则
enum class TRIGGER_BUFF_REMOVE_RULE : byte
{
	AUTO,					// 自动根据触发器所在的角色以及触发的buff附加的角色来判断是否需要移除附加的buff
	REMOVE_ADDED_BUFF,		// 移除触发器buff时会将此触发器所触发的所有buff移除,无论是附加在谁身上
	DONT_REMOVE_ADDED_BUFF,	// 移除触发器时不会移除此触发器所触发的所有buff
};

// 场景触发类型
enum class SCENE_TRIGGER : byte
{
	NONE,				// 无效值
	TRANS_MAP,			// 传送到指定地图
	SUMMON_MONSTER,		// 召唤怪物
};

// 服务器发送到客户端的消息的显示方式
enum class MESSAGE_DISPLAY : byte
{
	NONE,			// 无效值
	CHAT,			// 聊天栏中的信息
	TIP,			// 普通自动消失的提示信息
	IMPORTANT_TIP,	// 重要的自动消失的提示信息
};

// 装备的产出方式
enum class EQUIP_CREATE : byte
{
	NONE,				// 无效值
	MONSTER_DROP,		// 怪物掉落
	SYSTEM,				// 系统产出
	MAKE,				// 合成
	GM_MAKE,			// GM制作
	SHOP,				// 商店购买
	MISSION,			// 任务获取
};

// 行会领地类型
enum class TERRITORY_MAP : byte
{
	NONE,			// 无效值
	WOMA,			// 沃玛寺庙
	SHIMU,			// 石墓
	ZUMA,			// 祖玛寺庙
	SHIMO,			// 尸魔洞
	GUMO,			// 骨魔洞
	NIUMO,			// 牛魔寺庙
	MAX,			// 最大值
};

// 副本的类型
enum class SCENE_COPY_TYPE : byte
{
	NONE,			// 无效值
	HELL,			// 深渊
	BU_GUI_LU,		// 不归路
	WU_JIN_JIAN_TA,	// 无尽尖塔
};

// 强化装备的结果
enum class STRENGTH_RESULT : byte
{
	SUPER_SUCCESS,	// 超级成功,提升2级强化等级
	SUCCESS,		// 成功
	BROKEN,			// 失败并且装备已破碎
	DOWN_LEVEL,		// 失败并且装备的强化等级降低
	NOTHING,		// 失败并且装备的属性未发生改变
};

// 默认怪物行为树中的行为阶段类型
enum class ACTION_STATE_TYPE : byte
{
	NONE,			// 无效值
	TRACKING,		// 正在追踪
	TRACKING_DONE,	// 追踪完成
	TRACKING_WAIT,	// 正在追踪后的等待
	ATTACK_DONE,	// 攻击完成,攻击认为是瞬间的行为,没有持续
	ATTACK_WAIT,	// 攻击后的等待
	WANDER_WAIT,	// 闲逛后的等待
	WANDER,			// 正在闲逛
};

// 聊天类型
enum class CHAT_MESSAGE : byte
{
	WORLD,			// 世界发言,需要消耗道具
	LOCAL,			// 当前地图发言,可自由发言
	PRIVATE,		// 私聊
	SYSTEM,			// 系统消息
	SPECIAL,		// 特殊消息,需要消耗贵重道具
	GUILD,			// 行会聊天
	TEAM,			// 组队聊天
	MAX,
};

// 注册账号结果
enum class REGISTE_RESULT : byte
{
	SUCCESS,					// 成功
	EXIST,						// 账号已存在
	INVALID_ACCOUNT_PASSWORD,	// 账号或密码不合法
	INVALID_PHONE,				// 手机号不合法
	INVALID_VERIFY,				// 验证码不合法(已废弃)
	REQUESTED,					// 已经请求过注册此账号了,不一定创建成功
};

// 持久度恢复类型
enum class DURA_DELTA_TYPE : byte
{
	NONE,				// 正常的设置装备持久度
	REPAIR,				// 普通修理装备
	SPECIAL_REPAIR,		// 特殊修理装备
};

// 任务类型
enum class MISSION_TYPE : byte
{
	NONE,				// 无效值
	MAIN,				// 主线任务
	BRANCH,				// 支线任务
	ADVENTURE,			// 奇遇任务
	DAILY,				// 每日任务
};

// 任务接受类型
enum class MISSION_ACCEPT : byte
{
	NONE,				// 无效值
	AUTO,				// 满足任务接受条件时自动接受任务
	NPC,				// 满足任务接受条件时会在指定的NPC交互界面上显示该任务,玩家可以点击手动接受任务
};

// 用于玩家的条件类型
enum class CONDITION : byte
{
	NONE,
	LEVEL = 1,									// 需要等级大于等于一定值
	KILL_MONSTER = 2,							// 击杀一定数量的指定怪物
	GET_OBJECT = 3,								// 获取指定物品
	DRESS_EQUIP = 4,							// 穿戴指定ID的装备
	TALK_FINISH = 5,							// 完成对话
	ENTER_MAP_AREA = 6,							// 进入指定地图的指定区域范围
	FIRE_SKILL = 7,								// 释放指定技能
	DRESS_EQUIP_TYPE = 8,						// 穿戴指定类型的装备
	LEARN_SKILL = 9,							// 学习技能
	STRENGTH_EQUIP = 10,						// 强化装备
	DRESS_EQUIP_TYPE_QUALITY = 11,				// 穿戴一定品质以上的指定类型的装备
	PICK_MONSTER_DROP_ITEM = 12,				// 拾取指定的怪物掉落的物品
	KILL_STRENGTH_MONSTER = 13,					// 击杀一定数量的指定强度的怪物
	ENTER_SCENE_COPY = 14,						// 挑战一定次数的副本
	WIN_HIGH_LEVEL_SINGLE_ARENA = 15,			// 竞技场单人PK中战胜比自己等级高的玩家一定次数
	WIN_LOW_LEVEL_SINGLE_ARENA = 16,			// 竞技场单人PK中战胜比自己等级低的玩家一定次数
	TRIGGER_LUCKY_DROP = 17,					// 触发一定次数的幸运掉落
	PICK_OVER_COUNT_GOLD = 18,					// 拾取指定数量以上的金币一定次数
	PICK_OVER_COUNT_GOLD_INGOT = 19,			// 拾取指定数量以上的元宝一定次数
	STRENGTH_QUALITY_WEAPON_TO_LEVEL = 20,		// 强化指定次数的一定品级的武器到指定强化等级
	STRENGTH_EQUIP_TO_LEVEL = 21,				// 强化指定次数的装备到指定强化等级
	QUALITY_WEAPON_CONSECRATION = 22,			// 开光一定次数的指定品级的武器
	QUALITY_WEAPON_CONSECRATION_SUCCESS = 23,	// 开光成功一定次数的指定品级的武器
	SKILL_USE_WHITE_PLUME = 24,					// 使用技能消耗一定数量的白色羽毛
	MINE_GET_OBJECT = 25,						// 挖矿获得一定数量的指定物品
	KILL_ANY_MONSTER = 26,						// 击杀任意怪物 
	SELL_NPC_SHOP = 27,							// 出售装备到商店 
	USE_CONSUMABLE = 28,						// 使用指定消耗品 
	TEAM_KILL_ANY_MONSTER = 29,					// 组队击杀任意怪物 
	DECOMPOSE_GET_WHITE_PLUME = 30,				// 分解装备获得指定数量的白色羽毛 
	ENTER_SHENGYU = 31,							// 进入圣域 
	ENTER_FAIRY_LAND = 32,						// 进入幻境 
	WIN_SINGLE_ARENA = 33,						// 竞技场单人PK中获胜一定次数 
	BUY_AUCTION = 34,							// 在拍卖行中购买物品 
	BLESS_WEAPON = 35,							// 祝福武器 
	ENTER_ANY_HELL = 36,						// 挑战任意深渊 
	PICK_GOLD = 37,								// 拾取金币 
	PUT_ON_AUCTION = 38,						// 拍卖行上架物品 
	ENTER_UNKNOWN_DARK_PLACE = 39,				// 进入未知暗殿 
	REFRESH_EQUIP_PRECIOUS = 40,				// 洗炼装备的极品属性 
	REPAIR_EQUIP = 41,							// 修理装备 
	NORMAL_GACHA = 42,							// 普通抽卡 
	CONTRIBUTE_GOLD = 43,						// 捐献行会基金 
	BUY_LOTTERY_TICKET = 44,					// 购买彩票 
	DAMAGE_TO_MONSTER = 45,						// 对指定怪物累计造成一定伤害 
	PASS_BUGUILU = 46,							// 通关不归路 
};

// 邮件类型
enum class MAIL_TYPE : byte
{
	NONE,				// 无效值
	PLAYER,				// 玩家发送的邮件
	SYSTEM,				// 系统发送的邮件
	AUCTION,			// 拍卖行邮件
};

// 表示一个玩家与当前玩家的关系
enum class RELATIONSHIP_TYPE : byte
{
	NOT_EXIST,			// 无效玩家
	ONE_ACCOUNT,		// 玩家与当前玩家是同账号的
	HUSBAND,            // 玩家是当前玩家的丈夫
	WIFE,               // 玩家是当前玩家的妻子
	MASTER,				// 玩家是当前玩家的师父
	APPRENTICE,			// 玩家是当前玩家的徒弟
	FRIEND,             // 玩家是当前玩家的好友
	GUILD_MEMBER,		// 玩家是当前玩家的共同工会成员
	STRANGER,           // 玩家为陌生人
};

// 拍卖品搜索类型
enum class AUCTION_SEARCH_CONDITION : byte
{
	NAME = 0,				// 根据包含名字搜索
	MIN_LEVEL = 1,			// 限制最低等级
	MAX_LEVEL = 2,			// 限制最高等级
	MIN_STRENGTHEN = 3,		// 限制最低强化等级
	MAX_STRENGTHEN = 4,		// 限制最高强化等级
	OBJECT_TYPE = 5,		// 限制物品类型
	JOB = 6,				// 限制适用职业
	EQUIP_TYPE = 7,			// 限制装备类型
	QUALITY = 8,			// 限制物品品级
};

// 玩家上线时的广播提示类型
enum class LOGIN_TIP : byte
{
	NONE,					// 无效值
	FRIEND,					// 好友上线提示
	SHABAK_LORD,			// 沙巴克城主上线提示
	GUILD_MEMBER,			// 行会成员上线
};

// 玩家离开队伍的原因
enum class LEAVE_TEAM_REASON : byte
{
	KICK_OUT,			// 被踢出
	SELF_QUIT,			// 自己退出
};

// 行会的职位
enum class GUILD_TITLE : byte
{
	NONE,				// 无效值
	PRESIDENT,			// 会长
	VICE_PRESIDENT,		// 副会长
	ELDER,				// 长老
	ELITE_MEMBER,		// 精英成员
	MEMBER,				// 普通成员
};

// 玩家退出行会的原因
enum class LEAVE_GUILD_REASON : byte
{
	KICK_OUT,		// 被踢出
	SELF_QUIT,		// 自己退出
};

// 商品所属的商店类型
enum class GOODS_SHOP : byte
{
	NPC,			// NPC商店
	GUILD,			// 行会商店
	MALL,			// 商城
};

// 成就种类,大的种类
enum class ACHIVEMENT_CATEGORY : byte
{
	NONE,			// 无效值
	PVP,			// 杀戮人生
	PVE,			// 浴血历练
	GROWTH,			// 艰难成长
	NORMAL_LIFE,	// 日常生活
};

// 成就类型,同一个成就种类下有多个不同成就类型
enum class ACHIVEMENT_TYPE : byte
{
	NONE,
	LEVEL,					// 等级
	KILL_MONSTER,			// 击杀怪物数量
	KILL_ELITE_MONSTER,		// 击杀精英怪
	KILL_BOSS,				// 击杀BOSS
	MOVE,					// 移动
	KILL_PLAYER,			// 击杀玩家
	ONLINE_TIME,			// 累计在线
	LOGIN_DAY,				// 登录天数
	STRENGTHEN_WEAPON,		// 强化武器
	PICK_UP_LUCKY_OIL,		// 拾取祝福油
	HIT_BY_MONSTER,			// 被怪物击中
};

// 排行榜类型
enum class RANK_TYPE : byte
{
	LEVEL,				// 等级
	KILL_MONSTER,		// 击杀怪物
	KILL_ELITE,			// 击杀精英怪
	KILL_BOSS,			// 击杀教主
	KILL_SPECIAL,		// 击杀特殊怪
	KILL_PLAYER,		// 击杀玩家
	PICK_GOLD,			// 拾取金币
	PICK_SPECIAL_EQUIP,	// 拾取极品装备,粉色及以上装备
	STRENGTH_VALUE,		// 全身装备强化总值
	HELL_COUNT,			// 进入深渊的次数
	PK_SCORE,			// 竞技积分
	WU_JIN_JIAN_TA,		// 无尽尖塔通关层数
};

// 登录结果类型
enum class LOGIN_RESULT : byte
{
	SUCCESS,			// 登录成功
	PASSWORD_ERROR,		// 账号或密码错误
	OTHER_LOGINED,		// 已在其他地方登录,暂时不能顶账号,后续可以添加一个确认框确认是否要顶账号
	FREEZED,			// 账号已被冻结
	LOGIN_FULLED,		// 服务器在线人数已满
	INVALID_INPUT,		// 非法的账号密码
	IP_HAS_LOGIN,		// 此IP已经登录了账号,已废弃
	IP_FULLED,			// 此IP累计登录账号达到上限,已废弃
	OPERATE_TOO_OFTEN,	// 操作太频繁
	IP_FORBIDDEN,		// 此IP已禁止登录游戏
	IN_CLEAR_SEASON,	// 正在清理赛季数据中,暂时无法登录
};

// 创建角色的结果类型
enum class CREATE_CHARACTER_RESULT : byte
{
	SUCCESS,			// 成功
	NAME_DUPLICATE,		// 名字重复
	NAME_ERROR,			// 名字不合规
	CHARACTER_FULL,		// 角色数量已满
	OTHER,				// 其他未知错误,比如数据库插入失败
};

// 二级密码的状态
enum class SECONDARY_PASSWORD_STATE : byte
{
	NO_PASSWORD,		// 未创建
	NOT_CONFIRM,		// 未验证
	CONFIRMED,			// 已验证
};

// 行会领地争夺活动的阶段枚举
enum class TERRITORY_ACTIVITY_STAGE : byte
{
	NONE,			// 无效值
	SIGN_UP,		// 报名阶段
	PREPARE,		// 准备阶段
	ACTIVIYING,		// 进行中阶段
};

// 改名的结果
enum class CHANGE_NAME_RESULT : byte
{
	SUCCESS,		// 成功
	SAME_MYSELF,	// 与自己的名字一样
	INVALID_NAME,	// 名字包含无效字符
	NAME_EXIST,		// 名字已存在
};

// 怪物攻城的规模类型
enum class MONSTER_WAR_TYPE : byte
{
	NONE,			// 无效值
	NORMAL,			// 普通攻城
	HIGH,			// 高级攻城
};

// 绑定QQ的验证码验证结果
enum class QQ_BIND_VERIFY_RESULT : byte
{
	NONE,			// 无效值
	SUCCESS,		// 验证成功
	FAILED,			// 验证失败
	EXPIRED,		// 验证码已过期
};

// 沙巴克攻城战的阶段
enum class SHABAKE_BATTLE_STAGE : byte
{
	NONE,			// 无效值
	ENROLL,			// 报名阶段,持续10分钟
	FIGHTING_0,		// 开始争夺的第一阶段,每秒钟每人可获得1点积分,持续30分钟
	FIGHTING_1,		// 开始争夺的第一阶段,每秒钟每人可获得2点积分,持续20分钟
	FIGHTING_2,		// 开始争夺的第一阶段,每秒钟每人可获得5点积分,持续10分钟
};

// 红包的类型
enum class RED_ENVELOPE_TYPE : byte
{
	NONE,			// 无效值
	WORLD,			// 世界红包
	GUILD,			// 行会红包
	MAX,			// 最大值,用于判断范围是否正常
};

// 游戏验证码类型
enum class GAME_VERIFY : byte
{
	NONE,							// 无效值
	SEARCH_WEAPON,					// 在拍卖行输入武器名搜索任意一个武器
	SEARCH_JEWELRY,					// 在拍卖行输入首饰名搜索任意一个首饰
	PUT_ON_AUCTION,					// 上架一个拍卖品
	LOCAL_CHAT,						// 在聊天栏发送一条普通聊天信息
	BUY_GOLD_INGOT_HUICHENGJUAN,	// 在元宝商城购买一个回城卷
	DROP_ITEM,						// 丢弃任意一个物品
	FIRE_BASE_ATTACK,				// 释放一次普攻
	NPC_TRANSFER,					// 进行一次NPC传送
	BUY_NPC_WEAPON,					// 购买一个NPC商店的武器
	BUY_NPC_CLOTH,					// 购买一个NPC商店的衣服
	BUY_NPC_RING,					// 购买一个NPC商店的戒指
	OPEN_RANK,						// 查看一次排行榜
	UNDRESS_WEAPON,					// 将武器卸下
	UNDRESS_CLOTH,					// 将衣服卸下
	UNDRESS_NECKLACK,				// 将项链卸下
	MAX,
};

// 错误码
class ERROR_CODE
{
public:
	static constexpr ushort SUCCESS = 0;										// 成功
	static constexpr ushort SKILL_NOT_COOL_DOWN = 1;							// 技能未冷却
	static constexpr ushort MP_NOT_ENOUGH = 2;									// MP不足
	static constexpr ushort OBJECT_NOT_ENOUGH = 3;								// 物品不足
	static constexpr ushort GOLD_OVER_MAX = 4;									// 金币超出上限
	static constexpr ushort WEIGHT_OVER_MAX = 5;								// 负重超出上限
	static constexpr ushort NO_PACK_GRID = 6;									// 背包格子不足
	static constexpr ushort OBJECT_NOT_EXIST = 7;								// 物品不存在
	static constexpr ushort GOLD_NOT_ENOUGH = 8;								// 金币不足
	static constexpr ushort REPAIR_DONE = 9;									// 修理完成
	static constexpr ushort UNKNOWN_EQUIP_TYPE = 10;							// 未知类型的装备
	static constexpr ushort FIND_NO_EQUIP = 11;									// 身上找不到指定的装备
	static constexpr ushort GOLD_COUNT_INVALID = 12;							// 金币数量错误,无法获取
	static constexpr ushort EQUIP_NOT_EXIST = 13;								// 装备不存在
	static constexpr ushort OBJECT_GUID_ERROR = 14;								// 物品唯一ID错误
	static constexpr ushort SEX_NOT_MATCH = 15;									// 性别不符
	static constexpr ushort SKILL_HAS_LEARNED = 16;								// 该技能已学习
	static constexpr ushort OBJECT_NOT_OWNED = 17;								// 未拥有该物品
	static constexpr ushort LEVEL_NOT_ENOUGH = 18;								// 等级不足
	static constexpr ushort JOB_NOT_MATCH = 19;									// 职业不符
	static constexpr ushort OBJECT_NOT_COOL_DOWN = 20;							// 物品未冷却
	static constexpr ushort REMOVE_PACK_ITEM_FAILED = 21;						// 移除背包物品失败
	static constexpr ushort NO_WAREHOUSE_GRID = 22;								// 仓库格子不足
	static constexpr ushort OBJECT_NOT_IN_WAREHOUSE = 23;						// 仓库中没有该物品
	static constexpr ushort WAREHOUSE_GOLD_NOT_ENOUGH = 24;						// 仓库中金币不足
	static constexpr ushort WAREHOUSE_GOLD_OVER_MAX = 25;						// 仓库金币超出上限
	static constexpr ushort CHARACTER_NOT_EXIST = 26;							// 玩家不存在
	static constexpr ushort ITEM_CAN_NOT_TRADE = 27;							// 物品无法交易
	static constexpr ushort MAP_NOT_EXIST = 28;									// 地图不存在
	static constexpr ushort GOLD_INGOT_OVER_MAX = 29;							// 元宝超出上限
	static constexpr ushort GOLD_INGOT_NOT_ENOUGH = 30;							// 元宝不足
	static constexpr ushort GOLD_INGOT_COUNT_INVALID = 31;						// 元宝数量错误,无法获取
	static constexpr ushort SUMMON_MAX = 32;									// 召唤数量已达上限
	static constexpr ushort DURA_NOT_ENOUGH = 33;								// 装备耐久度不足,无法装备
	static constexpr ushort DRESS_POSITION = 34;								// 装备类型与装备位不符
	static constexpr ushort OVER_MAX_CARRY_COUNT = 35;							// 超出携带数量上限
	static constexpr ushort SKILL_NOT_EXIST = 36;								// 技能不存在
	static constexpr ushort CAN_NOT_USE_ITEM = 37;								// 当前无法使用此物品
	static constexpr ushort BACK_PACK_GRID_OVER_MAX = 38;						// 角色背包格子数量已经达到上限
	static constexpr ushort CHARACTER_IN_GAME = 39;								// 角色正在游戏中
	static constexpr ushort WAREHOUSE_GRID_OVER_MAX = 40;						// 仓库格子数量已经达到上限
	static constexpr ushort SCENE_COPY_NOT_VALID = 41;							// 此副本不存在
	static constexpr ushort UNKNOWN_ERROR = 42;									// 未知错误
	static constexpr ushort YOUHUO_MAX_COUNT = 43;								// 诱惑数量已经达到上限
	static constexpr ushort OPERATION_TOO_OFTEN = 44;							// 操作太频繁
	static constexpr ushort ATTACK_SPEED_TOO_FAST = 45;							// 攻速太快,请确保客户端未使用非法软件,否则有封号风险
	static constexpr ushort MOVE_SPEED_TOO_FAST = 46;							// 移速太快,请确保客户端未使用非法软件,否则有封号风险
	static constexpr ushort TRANSFER_POINT_ERROR = 47;							// 传送点检测失败
	static constexpr ushort EQUIP_LOCKED = 48;									// 装备已锁定
	static constexpr ushort EQUIP_CANNOT_LOCKED_AGAIN = 49;						// 装备不能重复锁定
	static constexpr ushort EQUIP_NOT_LOCKED = 50;								// 装备未锁定,无法解锁
	static constexpr ushort PK_GOLD_OVER_MAX = 53;								// 竞技币超出上限
	static constexpr ushort PK_GOLD_COUNT_INVALID = 54;							// 竞技币数量无效
	static constexpr ushort ARENA_CAN_NOT_SWITCH_PK_MODE = 55;					// 竞技场内无法切换PK模式
	static constexpr ushort PLAYER_OFFLINE = 61;								// 玩家不在线
	static constexpr ushort OBJECT_NOT_IN_PACK = 62;							// 物品不在背包中
	static constexpr ushort MOVE_SPEED_ERROR = 63;								// 移动速度错误
	static constexpr ushort NOT_ALLOW_FIRE_SKILL = 64;							// 当前不允许释放技能,处在技能制裁中
	static constexpr ushort NOT_ALLOW_STORE_WAREHOUSE = 65;						// 此物品不允许存入仓库
	static constexpr ushort BUY_GOODS_NOT_EXIST = 10001;						// 购买的商品不存在
	static constexpr ushort PICK_OBJECT_NOT_EXIST = 10002;						// 拾取的物品不存在
	static constexpr ushort PICK_OBJECT_NOT_IN_SCENE = 10003;					// 拾取的物品不在场景中
	static constexpr ushort SELL_OBJECT_NOT_EXIST = 10004;						// 出售的物品不存在
	static constexpr ushort SELECT_CHARACTER_INVALID = 10005;					// 当前选择角色无效
	static constexpr ushort INVALID_MAKE_ITEM = 10006;							// 制作的物品无效
	static constexpr ushort MAKE_MATERIAL_NOT_ENOUGH = 10007;					// 制作物品的材料不足
	static constexpr ushort MISSION_HAS_RECEIVED = 10008;						// 任务已经接受,不能重复接受
	static constexpr ushort MISSION_HAS_COMPLETED = 10009;						// 任务已经完成过,不能再接受
	static constexpr ushort NOT_SATISFIED_MISSION_CONDITION = 10010;			// 不满足任务接受条件
	static constexpr ushort MISSION_NOT_EXIST = 10011;							// 任务不存在
	static constexpr ushort MISSION_NOT_FINISH = 10012;							// 任务完成条件不足,无法提交任务
	static constexpr ushort MISSION_REMOVE_FAILED = 10013;						// 任务移除失败
	static constexpr ushort MISSION_REMOVE_OBJECT_FAILED = 10014;				// 任务完成后需要移除的任务物品不足
	static constexpr ushort NOT_FINISH_PRE_MISSION = 10015;						// 未完成前置任务
	static constexpr ushort MISSION_OVER_COUNT = 10016;							// 接受任务数量已达到上限
	static constexpr ushort MAIL_NOT_EXIST = 10017;								// 邮件不存在
	static constexpr ushort MAIL_NOT_THIS_PLAYER = 10018;						// 邮件不是此玩家的
	static constexpr ushort MAIL_RECEIVER_NOT_EXIST = 10019;					// 收件人不存在
	static constexpr ushort MAIL_RECEIVER_MAILBOX_FULL = 10020;					// 收件人邮箱已满
	static constexpr ushort MAIL_DELETE_ITEM_NOT_ALLOW = 10021;					// 无法删除带附件的邮件
	static constexpr ushort MAIL_CAN_NOT_SEND_TO_SELF = 10022;					// 不能给自己发送邮件
	static constexpr ushort MAIL_ITEM_NOT_EXIST = 10023;						// 邮件附件不存在
	static constexpr ushort MAIL_ITEM_COUNT_ERROR = 10024;						// 邮件附件物品的数量超出物品自身数量
	static constexpr ushort AUCTION_COUNT_ERROR = 10025;						// 拍卖的物品数量错误
	static constexpr ushort AUCTION_PRICE_ERROR = 10026;						// 拍卖品价格错误
	static constexpr ushort AUCTION_NOT_EXIST = 10027;							// 拍卖品不存在
	static constexpr ushort AUCTION_BUY_COUNT_ERROR = 10028;					// 拍卖购买超出物品数量
	static constexpr ushort AUCTION_SEARCH_PARAM_ERROR = 10029;					// 拍卖品搜索参数错误
	static constexpr ushort FRIEND_EXIST = 10030;								// 好友已经在好友列表
	static constexpr ushort FRIEND_NOT_EXIST = 10031;							// 对方不是自己的好友
	static constexpr ushort NO_CHAT_OFFLINE_CHARACTER = 10032;					// 玩家不在线,不能发送私聊
	static constexpr ushort NO_TEAM_INVITE_OFFLINE_CHARACTER = 10033;			// 玩家不在线,不能邀请组队
	static constexpr ushort START_TEAM_INVITE_CHARACTER_OFFLINE = 10034;		// 发起组队邀请的玩家不在线
	static constexpr ushort TEAM_FULL = 10035;									// 队伍已满,无法加入
	static constexpr ushort HAS_TEAM_NOT_JOIN_OTHER_TEAM = 10036;				// 玩家已经有队伍了,不能再加入其他队伍
	static constexpr ushort TEAM_HAS_MEMBER = 10037;							// 队伍中已经有该玩家,不能重复加入
	static constexpr ushort TEAM_NOT_EXIST = 10038;								// 队伍不存在
	static constexpr ushort NO_PRIVATE_CHAT_WITH_SELF = 10039;					// 不能发送私聊给自己
	static constexpr ushort CAN_NOT_CREATE_GUILD_WITH_GUILD = 10040;			// 已经加入行会了,不能创建行会
	static constexpr ushort CREATE_GUILD_MATERIAL_NOT_ENOUGH = 10041;			// 创建行会所需物品不足
	static constexpr ushort CREATE_GUILD_GOLD_NOT_ENOUGH = 10042;				// 创建行会所需金币不足
	static constexpr ushort GUILD_NAME_EXIST = 10043;							// 已经有相同名字的行会了
	static constexpr ushort PLAYER_NOT_THIS_GUILD = 10044;						// 玩家不属于该行会
	static constexpr ushort GUILD_NOT_EXIST = 10045;							// 行会不存在
	static constexpr ushort ONLY_PRESIDENT_DISMISS_GUILD = 10046;				// 只有会长才能解散行会
	static constexpr ushort ONLY_PRESIDENT_ANNOUNCEMENT = 10047;				// 只有会长才能修改公告
	static constexpr ushort GUILD_GOLD_OVER_MAX = 10048;						// 行会基金将超出上限,上限为10亿
	static constexpr ushort GUILD_CONTRIBUTION_OVER_MAX = 10049;				// 行会贡献度将超出上限
	static constexpr ushort PLAYER_HAS_GUILD = 10050;							// 玩家已经有行会了
	static constexpr ushort NO_INVITE_GUILD_OFFLINE_CHARACTER = 10051;			// 玩家不在线,不能邀请加入行会
	static constexpr ushort GUILD_MEMBER_FULL = 10052;							// 行会人数已满,无法加入
	static constexpr ushort GUILD_REQUEST_TOO_MUCH = 10053;						// 当前请求人数过多,会长已经处理不过来了
	static constexpr ushort GUILD_REQUEST_DONT_AGAIN = 10054;					// 不能重复申请加入同一行会
	static constexpr ushort VICE_PRESIDENT_ONLY_ONE = 10055;					// 只能有一个副会长
	static constexpr ushort ONLY_PRESIDENT_CAN = 10056;							// 只有会长才能任命其他人为会长
	static constexpr ushort NO_EXCHANGE_PLAYER_OFFLINE = 10057;					// 玩家不在线,不能交易
	static constexpr ushort PLAYER_IN_EXCHANGE = 10058;							// 玩家正在交易,不能进行其他交易
	static constexpr ushort EXCHANGE_NOT_EXIST = 10059;							// 交易不存在
	static constexpr ushort PLAYER_PACK_NOT_ENOUGH = 10060;						// 对方背包容量不足
	static constexpr ushort PLAYER_OVER_WEIGHT = 10061;							// 对方负重已超出上限
	static constexpr ushort PLAYER_GOLD_OVER_MAX = 10062;						// 对方已超出金币上限
	static constexpr ushort NO_WORLD_CHAT_ITEM = 10063;							// 缺少传音筒(小),无法进行世界发言
	static constexpr ushort NO_SPECIAL_CHAT_ITEM = 10064;						// 缺少传音筒(大),无法进行特殊发言
	static constexpr ushort ACHIVEMENT_NOT_EXIST = 10065;						// 成就不存在
	static constexpr ushort ACHIVEMENT_HAS_RECEIVED = 10066;					// 已经领取过了成就奖励
	static constexpr ushort ONLY_EQUIP_CAN_STRENGTH = 10067;					// 只有装备可以强化
	static constexpr ushort EQUIP_IN_PACK_TO_STRENGTH = 10068;					// 只有背包中或已穿戴的装备才能强化
	static constexpr ushort NOT_ALLOW_DELETE_CHARACTER = 10069;					// 不允许删除角色
	static constexpr ushort COMPOSITE_SAME_STAR_EQUIP_ONLY = 10070;				// 只能使用相同星级的装备进行合成
	static constexpr ushort COMPOSITE_STAR_EQUIP_STAR_ERROR = 10071;			// 无法使用零星和五星装备进行合成
	static constexpr ushort MATERIAL_NOT_ENOUGH = 10072;						// 材料不足
	static constexpr ushort SALER_OFFLINE = 10073;								// 卖家不在线,无法请求购买
	static constexpr ushort SUPPOSED_PRICE_ERROR = 10074;						// 期望价格错误
	static constexpr ushort SUPPOSED_PRICE_REFUSED = 10075;						// 卖家拒绝了期望价格
	static constexpr ushort REPAIR_ONLY_EQUIP = 10076;							// 只能修复装备
	static constexpr ushort REPAIR_EQUIP_NOT_EXIST = 10077;						// 要修复的装备不存在
	static constexpr ushort BUY_COUNT_NOT_ALLOWED = 10078;						// 剩余可购买数量不足
	static constexpr ushort NOT_CHANGE_WEAPON_DIGGING = 10079;					// 正在挖矿时不能切换武器
	static constexpr ushort NO_CHAT_TO_MYSELF = 10080;							// 不能给自己发送私聊
	static constexpr ushort SKILL_EXP_FULL = 10081;								// 技能经验已满
	static constexpr ushort EQUIP_CANNOT_DECOMPOSE = 10082;						// 此装备无法分解
	static constexpr ushort MINE_TIME_OVER_MAX = 10083;							// 可挖矿时间将超过上限
	static constexpr ushort NEED_WEAPON_TO_DIG_MINE = 10084;					// 需要装备鹤嘴锄才能进行挖矿
	static constexpr ushort HAVE_NO_MINE_TIME = 10085;							// 当前没有可挖矿时间
	static constexpr ushort QUICK_SELL_COUNT_OVER_MAX = 10086;					// 快速出售的剩余次数超过了上限
	static constexpr ushort QUICK_SELL_COUNT_NOT_ENOUGH = 10087;				// 快速出售的剩余次数不足
	static constexpr ushort FRIEND_FULL = 10088;								// 好友已满,添加失败
	static constexpr ushort CANNOT_ADD_SELF_FRIEND = 10089;						// 不能添加自己为好友
	static constexpr ushort EXCHANGED_LOCKED = 10090;							// 交易已锁定,无法再修改
	static constexpr ushort SUPPOSE_PRICE_TOO_OFTEN = 10091;					// 期望价格操作太频繁
	static constexpr ushort MAP_CAN_NOT_DIG_MINE = 10092;						// 当前地图不允许挖矿
	static constexpr ushort SECONDARY_PASSWORD_EXIST = 10093;					// 二级密码已经创建,无法重复创建
	static constexpr ushort SECONDARY_PASSWORD_INVALID = 10094;					// 二级密码不合法
	static constexpr ushort SECONDARY_PASSWORD_NOT_EXIST = 10095;				// 二级密码不存在
	static constexpr ushort SECONDARY_PASSWORD_HAS_CONFIRMED = 10096;			// 二级密码已验证
	static constexpr ushort SECONDARY_PASSWORD_NOT_CONFIRMED = 10097;			// 二级密码未验证,请打开主菜单中的二级密码页面进行验证
	static constexpr ushort OFFER_REWARD_PUTON_FULL = 10098;					// 悬赏令发布数量已达到上限
	static constexpr ushort OFFER_REWARD_NOT_EXIST = 10099;						// 悬赏令不存在或已到期
	static constexpr ushort OFFER_REWARD_ACCEPT_FULL = 10100;					// 悬赏令接受数量已达到上限
	static constexpr ushort OFFER_REWARD_ACCEPT_OTHER = 10101;					// 悬赏令已经被其他玩家接受
	static constexpr ushort OFFER_REWARD_ACCEPT_SAME_TARGET = 10102;			// 不能接受通缉同一个玩家的悬赏令
	static constexpr ushort OFFER_REWARD_PUT_ON_SAME_TARGET = 10103;			// 不能对同一个玩家的发布重复悬赏令
	static constexpr ushort OFFER_REWARD_ACCEPT_SELF_PUTON = 10104;				// 不能接受自己发布的悬赏令
	static constexpr ushort OFFER_REWARD_ACCEPT_TARGET_SELF = 10105;			// 不能接受对通缉自己的悬赏令
	static constexpr ushort OFFER_REWARD_PUTON_TARGET_SELF = 10106;				// 不能发布对通缉自己的悬赏令
	static constexpr ushort AUCTION_DISCOUNT_OVER_MAX = 10107;					// 拍卖行优惠次数超过上限
	static constexpr ushort ITEM_TOO_FAR = 10108;								// 距离物品太远
	static constexpr ushort QQ_NOT_BIND = 10109;								// 未绑定QQ
	static constexpr ushort ONLY_PRESIDENT_LEVEL_UP_GUILD = 10110;				// 只有会长或者副会长可以升级行会
	static constexpr ushort GUILD_EXP_NOT_ENOUGH = 10111;						// 行会经验不足
	static constexpr ushort GUILD_GOLD_NOT_ENOUGH = 10112;						// 行会基金不足
	static constexpr ushort GUILD_LEVEL_MAX = 10113;							// 行会等级已经达到上限
	static constexpr ushort QQ_HAS_BIND = 10114;								// QQ已经绑定,不允许修改
	static constexpr ushort ONLY_PRESIDENT_CAN_SIGN_UP = 10115;					// 只有会长或者副会长可以报名活动
	static constexpr ushort ACTIVITY_SIGNED_UP = 10116;							// 已经报名过活动了
	static constexpr ushort ACTIVITY_TERRITORY_GUILD_FULL = 10117;				// 此领地的报名行会数量已满
	static constexpr ushort TERRITORY_CAN_NOT_SIGN_UP = 10118;					// 此领地不可报名
	static constexpr ushort DISABLE_TALK = 10119;								// 你已被禁言
	static constexpr ushort ONLY_PRESIDENT_CAN_UPGRADE = 10120;					// 仅会长或者副会长可以操作升级
	static constexpr ushort TERRITORY_LEVEL_OVER_MAX = 10121;					// 行会领地等级已经达到上限
	static constexpr ushort CARD_EQUIP_TYPE_ERROR = 10122;						// 卡片无法附加在此类型的装备上
	static constexpr ushort RECEIVE_FAILED = 10123;								// 领取失败
	static constexpr ushort QUICK_DECOMPOSE_COUNT_OVER_MAX = 10124;				// 快速分解的剩余次数超过了上限
	static constexpr ushort QUICK_DECOMPOSE_COUNT_NOT_ENOUGH = 10125;			// 快速分解的剩余次数不足
	static constexpr ushort OTHER_SECONDARY_PASSWORD_NOT_CONFIRMED = 10126;		// 对方二级密码未验证
	static constexpr ushort OTHER_QQ_NOT_BIND = 10127;							// 对方QQ未绑定
	static constexpr ushort EQUIP_CAN_NOT_STRENGTH = 10128;						// 此装备无法强化
	static constexpr ushort ONLY_WEAPON_CAN_BLESS = 10129;						// 只有武器可以祝福
	static constexpr ushort OTHER_FRIEND_FULL = 10130;							// 对方好友已满,添加失败
	static constexpr ushort NOT_ENROLL_STAGE = 10131;							// 当前不在报名阶段,无法报名
	static constexpr ushort SHABAKE_BATTLE_CAN_TRANSFER = 10132;				// 仅沙巴克攻城期间可以传送
	static constexpr ushort BOSS_DROP_PROTECT_COUNT_OVER_MAX = 10133;			// boss掉落物品保护的剩余次数超过了上限
	static constexpr ushort QQMAIL_HAS_PROTECTED = 10134;						// 此QQ邮箱已经开启保护,无法绑定
	static constexpr ushort SCENE_COPY_NEED_NO_TEAM = 10135;					// 当前副本不能组队进入
	static constexpr ushort SCENE_COPY_NEED_TEAM = 10136;						// 当前副本需要组队进入
	static constexpr ushort ONLY_TEAM_LEADER_CAN_REQUEST_ENTER = 10137;			// 只允许队长请求进入
	static constexpr ushort TEAM_MEMBER_LEVEL_NOT_ENOUGH = 10138;				// 队伍中有玩家等级不足
	static constexpr ushort TEAM_MEMBER_GOLD_NOT_ENOUGH = 10139;				// 队伍中有玩家金币不足
	static constexpr ushort TEAM_MEMBER_GOLD_INGOT_NOT_ENOUGH = 10140;			// 队伍中有玩家元宝不足
	static constexpr ushort TEAM_MEMBER_MATERIAL_NOT_ENOUGH = 10141;			// 队伍中有玩家材料不足
	static constexpr ushort TEAM_MEMBER_NOT_NEAR = 10142;						// 队伍中有玩家不在附近
	static constexpr ushort CAN_NOT_LEAVE_TEAM_IN_SCENE_COPY = 10143;			// 副本中不允许退出队伍
	static constexpr ushort CAN_NOT_KICK_TEAM_IN_SCENE_COPY = 10144;			// 副本中不允许将玩家踢出队伍
	static constexpr ushort SCENE_COPY_PLAYER_COUNT_ERROR = 10145;				// 进入副本的玩家数量错误
	static constexpr ushort CAN_NOT_JOIN_TEAM_IN_SCENE_COPY = 10146;			// 自己或者有队员在副本中,无法加入队伍
	static constexpr ushort ONLY_LEADER_FINISH_SCENE_COPY = 10147;				// 只有队长可以选择完成副本
	static constexpr ushort SCENE_COPY_NOT_FINISH = 10148;						// 当前副本未完成
	static constexpr ushort QQ_NOT_VERIFY = 10149;								// QQ未验证
	static constexpr ushort SCENE_COPY_NOT_ALLOW_TRADE = 10150;					// 副本中不允许交易
	static constexpr ushort CAN_NOT_MATCH_PK = 10151;							// 当前不允许进行匹配
	static constexpr ushort CAN_NOT_MATCH_PK_IN_TEAM = 10152;					// 组队中不允许进行匹配
	static constexpr ushort MATCH_COST_NOT_ENOUGH = 10153;						// 匹配所需物品不足
	static constexpr ushort ARENA_CAN_NOT_CHAT = 10154;							// 竞技场内无法聊天
	static constexpr ushort ONLY_WORLD_CAN_EXCHANGE = 10155;					// 副本或竞技场内无法交易
	static constexpr ushort ARENA_CAN_NOT_MAIL = 10156;							// 竞技场内无法发送邮件
	static constexpr ushort ARENA_CAN_NOT_TEAM = 10157;							// 竞技场内无法组队
	static constexpr ushort ARENA_CAN_NOT_ADD_FRIEND = 10158;					// 竞技场内无法添加好友
	static constexpr ushort ARENA_CAN_NOT_JOIN_GUILD = 10159;					// 竞技场内无法加入行会
	static constexpr ushort PK_GOLD_NOT_ENOUGH = 10160;							// 竞技币不足
	static constexpr ushort RED_ENVELOPE_AVERAGE_GOLD_NOT_ENOUGH = 10161;			// 平均每个金币红包的金额不能低于1万金币
	static constexpr ushort RED_ENVELOPE_AVERAGE_GOLD_INGOT_NOT_ENOUGH = 10162;	// 平均每个元宝红包的金额不能低于10元宝
	static constexpr ushort RED_ENVELOPE_NOT_EXIST = 10163;						// 红包不存在
	static constexpr ushort RED_ENVELOPE_EMPTY = 10164;							// 红包已被抢完
	static constexpr ushort RED_ENVELOPE_PACKET_TOO_MANY = 10165;				// 红包数量太多
	static constexpr ushort RED_ENVELOPE_HAS_GRAB = 10166;						// 已经抢过这个红包了
	static constexpr ushort ACCOUNT_NOT_EXIST = 10167;							// 账号不存在
	static constexpr ushort RED_ENVELOPE_PACKET_TOO_FEW = 10168;				// 红包数量太少
	static constexpr ushort ACCOUNT_PRICE_TOO_LOW = 10169;						// 账号价格太低
	static constexpr ushort ACCOUNT_HAS_IN_SELL = 10170;						// 账号已经正在出售中
	static constexpr ushort ACCOUNT_NOT_IN_SELL = 10171;						// 账号没有在上架
	static constexpr ushort HAS_WANT_BUY = 10172;								// 已经请求想要购买,不能重复请求
	static constexpr ushort NOT_IN_WANT_BUY_LIST = 10173;						// 玩家不在请求购买列表中
	static constexpr ushort ACCOUNT_SELL_HAS_DEAL = 10174;						// 此账号已经选择了同意成交的买家
	static constexpr ushort NOT_ACCOUNT_SELL_DEAL_PLAYER = 10175;				// 你不是卖家同意成交的玩家
	static constexpr ushort CANNOT_BUY_SAME_ACCOUNT = 10176;					// 当前角色属于此账号,无法购买
	static constexpr ushort ACCOUNT_FREEZED_SELL = 10177;						// 账号已被封禁,无法交易
	static constexpr ushort ACCOUNT_INFO_HAS_DONE = 10178;						// 账号交易信息已经填写过了,不能再次填写
	static constexpr ushort ACCOUNT_INFO_FILL_INVALID = 10179;					// 账号交易信息填写格式错误
	static constexpr ushort SELF_ACCOUNT_IN_SELL = 10180;						// 自己的账号正在上架交易中,不能购买其他账号
	static constexpr ushort SELF_HAS_WANT_BUY_ACCOUNT = 10181;					// 自己正在请求购买其他账号中,不能上架自己的账号
	static constexpr ushort YOU_DONT_HAVE_GUILD = 10182;						// 你当前没有行会
	static constexpr ushort YOU_DONT_HAVE_TEAM = 10183;							// 你当前没有队伍
	static constexpr ushort MAP_HAS_UNLOCKED = 10184;							// 地图已经解锁过了
	static constexpr ushort MAP_IS_LOCKED = 10185;								// 地图未解锁
	static constexpr ushort PLAYER_NOT_ALLOW_FRIEND_REQUEST = 10186;			// 对方不允许添加好友
	static constexpr ushort PLAYER_NOT_ALLOW_GUILD_REQUEST = 10187;				// 对方不允许邀请行会
	static constexpr ushort PLAYER_NOT_ALLOW_TEAM_REQUEST = 10188;				// 对方不允许邀请组队
	static constexpr ushort PLAYER_NOT_ALLOW_TRADE_REQUEST = 10189;				// 对方不允许邀请交易
	static constexpr ushort BASE_ANTI_CRITICAL_OVER_MAX = 10192;				// 基础抗暴率已达到上限
	static constexpr ushort BASE_CRITICAL_PROBABILITY_OVER_MAX = 10193;			// 基础暴击率已达到上限
	static constexpr ushort BASE_CRITICAL_DAMAGE_OVER_MAX = 10194;				// 基础暴击伤害已达到上限
	static constexpr ushort BASE_MAX_HP_OVER_MAX = 10195;						// 基础血量上限已达到上限
	static constexpr ushort BASE_MAX_MP_OVER_MAX = 10196;						// 基础魔法量上限已达到上限
	static constexpr ushort BASE_PHYSICS_ATTACK_OVER_MAX = 10197;				// 基础物理攻击力上限已达到上限
	static constexpr ushort BASE_MAGIC_ATTACK_OVER_MAX = 10198;					// 基础魔法攻击力上限已达到上限
	static constexpr ushort BASE_SORECRY_ATTACK_OVER_MAX = 10199;				// 基础道术攻击力上限已达到上限
	static constexpr ushort BASE_PHYSICS_DEFENCE_OVER_MAX = 10200;				// 基础物理防御力上限已达到上限
	static constexpr ushort BASE_MAGIC_DEFENCE_OVER_MAX = 10201;				// 基础魔法防御力上限已达到上限
	static constexpr ushort BASE_ATTACK_SPEED_OVER_MAX = 10202;					// 基础攻击速度上限已达到上限
	static constexpr ushort BASE_MOVE_SPEED_OVER_MAX = 10203;					// 基础移动速度上限已达到上限
	static constexpr ushort BASE_HIT_PROBABILITY_OVER_MAX = 10204;				// 基础命中率上限已达到上限
	static constexpr ushort BASE_EVADE_PROBABILITY_OVER_MAX = 10205;			// 基础闪避率上限已达到上限
	static constexpr ushort IN_OTHER_BLACK_LIST = 10206;						// 你在对方的黑名单中
	static constexpr ushort PLAYER_IN_BLACK_LIST = 10207;						// 对方在你的黑名单中
	static constexpr ushort FORCE_PULL_OFF_RECENT = 10208;						// 被强制下架的账号一定时间内无法上架
	static constexpr ushort NO_GUILD_CANNOT_DISPATCH = 10209;					// 没有加入任何行会不允许发送行会红包
	static constexpr ushort MEMBER_COUNT_NOT_ENOUGH_DISPATCH = 10210;			// 行会成员低于50人时不允许发送行会元宝红包
	static constexpr ushort PLAYER_NOT_ALLOW_RECEIVE_MAIL = 10211;				// 对方设置了不接收玩家邮件
	static constexpr ushort PLAYER_NOT_ALLOW_PRIVATE_CHAT = 10212;				// 对方设置了不允许私聊
	static constexpr ushort RESET_PASSWORD_TOO_OFTEN = 10213;					// 修改密码太频繁
	static constexpr ushort RESET_PASSWORD_FAILED = 10214;						// 修改密码信息填写错误
	static constexpr ushort EQUIP_QUALITY_NOT_ENOUGH = 10215;					// 装备品质太低
	static constexpr ushort VERIFY_ERROR = 10216;								// 验证码错误
	static constexpr ushort VERIFY_EXPIRATION = 10217;							// 验证码已失效
	static constexpr ushort VERIFY_FETCH_TOO_OFTEN = 10218;						// 获取验证码太频繁,请稍后再试
	static constexpr ushort RESET_2ND_PASSWORD_TOO_OFTEN = 10219;				// 重置二级密码太频繁
	static constexpr ushort PK_SCORE_NOT_ENOUGH = 10220;						// PK分不足
	static constexpr ushort NOT_SAME_TEAM = 10221;								// 玩家不在同一队伍
	static constexpr ushort PLAYER_REFUSE_CALL = 10222;							// 对方拒绝了传送请求
	static constexpr ushort CALL_ONLY_IN_WORLD = 10223;							// 只能在世界中才能召唤
	static constexpr ushort CALL_OTHER_ONLY_IN_WORLD = 10224;					// 只能召唤在世界中的队友
	static constexpr ushort PLAYER_NOT_IN_WORLD = 10225;						// 玩家不在世界中
	static constexpr ushort NOT_SAME_GUILD = 10226;								// 玩家不在同一行会
	static constexpr ushort CALL_NOT_IN_DIG_MINE = 10227;						// 对方正在挖矿,无法召唤
	static constexpr ushort SELF_OR_MEMBER_MAP_LEVEL_NOT_ENOUGH = 10228;		// 自己或者队友的通关等级不足
	static constexpr ushort ADVENTURE_MISSION_LEVEL_NOT_ENOUGH = 10229;			// 需要30级才能解锁刷新奇遇任务
	static constexpr ushort WU_JIN_JIAN_TA_NOT_FINISH = 10230;					// 无尽尖塔当前层未通关
};

// 对应状态表中的状态类型ID
class CHARACTER_STATE
{
public:
	static constexpr ushort NONE = 0;
	// Buff
	static constexpr ushort BuffRecoverHPCycle = 1;								// 持续回血
	static constexpr ushort BuffRecoverMPCycle = 2;								// 持续回蓝
	static constexpr ushort BuffRecoverHPOnce = 3;								// 单次回血
	static constexpr ushort BuffRecoverMPOnce = 4;								// 单次回蓝
	static constexpr ushort BuffPhysicsAttackUp = 5;							// 提升物理攻击力
	static constexpr ushort BuffMagicAttackUp = 6;								// 提升魔法攻击力
	static constexpr ushort BuffSorceryAttackUp = 7;							// 提升道术攻击力
	static constexpr ushort BuffMoveSpeedUp = 14;								// 提升移动速度
	static constexpr ushort BuffAttackSpeedUp = 15;								// 提升攻击速度
	static constexpr ushort BuffMaxHPUp = 16;									// 提升血量上限
	static constexpr ushort BuffMaxMPUp = 17;									// 提升魔法量上限
	static constexpr ushort BuffTransToMainCity = 27;							// 传送到主城
	static constexpr ushort BuffTransRandom = 28;								// 传送到当前地图随机位置
	static constexpr ushort BuffTransToMap = 31;								// 传送到指定地图
	static constexpr ushort BuffMultipleMonsterExp = 32;						// 获得多倍杀怪经验
	static constexpr ushort BuffPhysicsAttackDown = 33;							// 降低物理攻击力
	static constexpr ushort BuffSorceryAttackDown = 35;							// 降低道术攻击力
	static constexpr ushort BuffMagicAttackDown = 37;							// 降低魔法攻击力
	static constexpr ushort BuffPhysicsDefenceUp = 39;							// 提升物理防御力
	static constexpr ushort BuffMagicDefenceUp = 40;							// 提升魔法防御力
	static constexpr ushort BuffPhysicsDefenceDown = 43;						// 降低物理防御力
	static constexpr ushort BuffMagicDefenceDown = 44;							// 降低魔法防御力
	static constexpr ushort BuffMoveSpeedDown = 47;								// 降低移动速度
	static constexpr ushort BuffAttackSpeedDown = 48;							// 降低攻击速度
	static constexpr ushort BuffMaxHPDown = 49;									// 降低血量上限
	static constexpr ushort BuffMaxMPDown = 50;									// 降低魔法量上限
	static constexpr ushort BuffHitProbabilityUp = 57;							// 提升命中率
	static constexpr ushort BuffEvadeProbabilityUp = 58;						// 提升闪避率
	static constexpr ushort BuffCriticalProbabilityUp = 59;						// 提升暴击率
	static constexpr ushort BuffCriticalDamageRateUp = 60;						// 提升暴击伤害
	static constexpr ushort BuffHitProbabilityDown = 61;						// 降低命中率
	static constexpr ushort BuffEvadeProbabilityDown = 62;						// 降低闪避率
	static constexpr ushort BuffCriticalProbabilityDown = 63;					// 降低暴击率
	static constexpr ushort BuffCriticalDamageRateDown = 64;					// 降低暴击伤害
	static constexpr ushort BuffRecoverHPCycleMax = 65;							// 持续回复血量
	static constexpr ushort BuffRecoverMPCycleMax = 66;							// 持续回复魔法量
	static constexpr ushort BuffMoFaDun = 67;									// 魔法盾
	static constexpr ushort BuffStrickBack = 68;								// 击退
	static constexpr ushort BuffTransForward = 69;								// 向前瞬移一定距离
	static constexpr ushort BuffInvisible = 70;									// 隐身
	static constexpr ushort BuffPoisoningGreen = 71;							// 绿毒
	static constexpr ushort BuffPoisoningRed = 72;								// 红毒
	static constexpr ushort BuffDamageCycle = 73;								// 持续伤害
	static constexpr ushort BuffStandFixed = 74;								// 束缚状态,不可改变位置
	static constexpr ushort BuffKunMoZhou = 75;									// 困魔咒束缚,不可改变位置
	static constexpr ushort BuffXinLingQiShi = 76;								// 心灵启示
	static constexpr ushort BuffShengYanShu = 77;								// 圣言术                  
	static constexpr ushort BuffAdditiveDamage = 79;							// 附加伤害
	static constexpr ushort BuffMoFaDunTimeUp = 80;								// 提升魔法盾持续时间
	static constexpr ushort BuffSummonMonster = 81;								// 召唤怪物
	static constexpr ushort BuffInvincible = 82;								// 无敌
	static constexpr ushort BuffResetSkillCD = 84;								// 重置技能CD
	static constexpr ushort BuffSuckBlood = 86;									// 攻击时吸血
	static constexpr ushort BuffShenShouAttackWaitDown = 87;					// 减少神兽的攻击间隔
	static constexpr ushort BuffShenShouAttackSpeedUp = 88;						// 增加神兽的攻击速度
	static constexpr ushort BuffKillPercentHP = 89;								// 百分比减血
	static constexpr ushort BuffTriggerAttack = 90;								// 攻击时触发
	static constexpr ushort BuffMultipleDamageImmediately = 91;					// 立即多倍伤害,只在添加buff时生效一次
	static constexpr ushort BuffTriggerWillHit = 92;							// 即将命中时触发
	static constexpr ushort BuffTriggerCriticalAttack = 93;						// 暴击时触发
	static constexpr ushort BuffDistanceIncreaseDamage = 94;					// 伤害随距离增加
	static constexpr ushort BuffCounterInjury = 95;								// 反伤
	static constexpr ushort BuffCounterInjuryPhysics = 96;						// 反弹物理伤害
	static constexpr ushort BuffCounterInjuryMagic = 97;						// 反弹魔法伤害
	static constexpr ushort BuffCounterInjurySorcery = 98;						// 反弹道术伤害
	static constexpr ushort BuffCounterInjuryFar = 99;							// 反弹远程伤害
	static constexpr ushort BuffCounterInjuryNear = 100;						// 反弹近程伤害
	static constexpr ushort BuffCounterInjuryCritical = 101;					// 反弹暴击伤害
	static constexpr ushort BuffHPRecoverUp = 102;								// 血量回复速度增加
	static constexpr ushort BuffMPRecoverUp = 103;								// 魔法量回复速度增加
	static constexpr ushort BuffClampMaxDamage = 104;							// 将每次受到的伤害限定不超过一个最大值
	static constexpr ushort BuffTriggerHPUnderPercent = 105;					// 血量低于一定百分比时触发
	static constexpr ushort BuffTriggerRangePlayerCount0 = 106;					// 范围内玩家数量超过一定值时触发
	static constexpr ushort BuffTriggerBeMissHit = 108;							// 攻击被闪避时触发
	static constexpr ushort BuffLowHPIncreaseAttack = 109;						// 血量越低增加的攻击力越高
	static constexpr ushort BuffTriggerHPOverPercent = 110;						// 血量高于一定百分比时触发
	static constexpr ushort BuffInvincibleNonCritical = 111;					// 免疫除暴击以外的伤害
	static constexpr ushort BuffRemoveBuff = 112;								// 移除其他buff
	static constexpr ushort BuffInvinciblePhysicsDamage = 113;					// 免疫物理伤害
	static constexpr ushort BuffInvincibleMagicDamage = 114;					// 免疫魔法伤害
	static constexpr ushort BuffInvincibleSorceryDamage = 115;					// 免疫道术伤害
	static constexpr ushort BuffDamageRangeCycle = 116;							// 范围内可攻击角色持续掉血
	static constexpr ushort BuffTriggerContinueBeCriticalHit = 117;				// 连续受到一定次数的暴击时触发
	static constexpr ushort BuffTalkState = 118;								// 说话
	static constexpr ushort BuffTriggerGetBuff = 119;							// 获得指定buff时触发
	static constexpr ushort BuffInvincibleCritical = 120;						// 免疫暴击伤害
	static constexpr ushort BuffTriggerInvinciblePhysicsDamage = 121;			// 累计免疫物理伤害一定次数时触发
	static constexpr ushort BuffTriggerInvincibleMagicDamage = 122;				// 累计免疫物理伤害一定次数时触发
	static constexpr ushort BuffTriggerInvincibleSorceryDamage = 123;			// 累计免疫物理伤害一定次数时触发
	static constexpr ushort BuffTriggerInvincibleCriticalDamage = 124;			// 累计免疫物理伤害一定次数时触发
	static constexpr ushort BuffTriggerInvincibleNonCriticalDamage = 125;		// 累计免疫物理伤害一定次数时触发
	static constexpr ushort BuffTriggerRemoveBuff = 126;						// 移除指定buff时触发
	static constexpr ushort BuffTriggerBeTotalCriticalHit = 127;				// 累计被暴击一定次数时触发
	static constexpr ushort BuffTriggerTime = 128;								// 状态正常结束时触发
	static constexpr ushort BuffTriggerDie = 129;								// 死亡时触发
	static constexpr ushort BuffTriggerRangePlayerCount1 = 130;					// 仅当范围内玩家超过一定数量时触发一次,且不会移除
	static constexpr ushort BuffDestroySelf = 131;								// 角色销毁
	static constexpr ushort BuffSkillAttackUp = 132;							// 提升指定技能攻击力
	static constexpr ushort BuffSkillAttackDown = 133;							// 降低指定技能攻击力
	static constexpr ushort BuffMultipleSkillExp = 134;							// 获得多倍的技能经验
	static constexpr ushort BuffAllowDetectPlayerLocation = 135;				// 允许探测其他玩家的位置
	static constexpr ushort BuffAllowTransferToPosition = 136;					// 传送到指定位置
	static constexpr ushort BuffRevive = 137;									// 复活
	static constexpr ushort BuffParalysis = 138;								// 麻痹
	static constexpr ushort BuffPackMaxWeightUp = 139;							// 背包负重上限增加
	static constexpr ushort BuffProtectHPWithMP = 140;							// 护身效果,使用MP抵扣一定百分比的HP
	static constexpr ushort BuffProvocation = 141;								// 挑衅,减少攻击间隔,降低防御力
	static constexpr ushort BuffAntiCriticalUp = 142;							// 提升暴击抗性
	static constexpr ushort BuffAntiCriticalDown = 143;							// 降低暴击抗性
	static constexpr ushort BuffAllSkillAttackUp = 144;							// 提升技能攻击力
	static constexpr ushort BuffAllSkillAttackDown = 145;						// 降低技能攻击力
	static constexpr ushort BuffRangePlayerSkillAttackUp = 146;					// 范围内玩家技能攻击力提升
	static constexpr ushort BuffRangePlayerPhysicsAttackUp = 147;				// 范围内玩家物理攻击力提升
	static constexpr ushort BuffRangePlayerMagicAttackUp = 148;					// 范围内玩家魔法攻击力提升
	static constexpr ushort BuffRangePlayerSorceryAttackUp = 149;				// 范围内玩家道术攻击力提升
	static constexpr ushort BuffRangePlayerPhysicsDefenceUp = 150;				// 范围内玩家物理防御力提升
	static constexpr ushort BuffRangePlayerMagicDefenceUp = 151;				// 范围内玩家魔法防御力提升
	static constexpr ushort BuffRangePlayerCriticalUp = 152;					// 范围内玩家暴击率提升
	static constexpr ushort BuffRangePlayerCriticalDamageUp = 153;				// 范围内玩家暴击伤害提升
	static constexpr ushort BuffRangePlayerHitProbabilityUp = 154;				// 范围内玩家命中率提升
	static constexpr ushort BuffRangePlayerEvadeProbabilityUp = 155;			// 范围内玩家闪避率提升
	static constexpr ushort BuffRangePlayerAntiCriticalUp = 156;				// 范围内玩家暴击抗性提升
	static constexpr ushort BuffRangeEnemyPhysicsAttackDown = 157;				// 范围内敌方玩家或怪物物理攻击力降低
	static constexpr ushort BuffRangeEnemyMagicAttackDown = 158;				// 范围内敌方玩家或怪物魔法攻击力降低
	static constexpr ushort BuffRangeEnemySorceryAttackDown = 159;				// 范围内敌方玩家或怪物道术攻击力降低
	static constexpr ushort BuffRangeEnemyPhysicsDefenceDown = 160;				// 范围内敌方玩家或怪物物理防御力降低
	static constexpr ushort BuffRangeEnemyMagicDefenceDown = 161;				// 范围内敌方玩家或怪物魔法防御力降低
	static constexpr ushort BuffHuoQiangTimeUp = 162;							// 增加火墙持续时间
	static constexpr ushort BuffSkillCDDown = 163;								// 减少技能CD百分比
	static constexpr ushort BuffZhaoHuanShenShouMaxCountUp = 164;				// 召唤神兽召唤数量上限增加
	static constexpr ushort BuffOpenClearEquipProficiency = 165;				// 打开清零装备熟练度界面
	static constexpr ushort BuffOpenRepairEquip = 166;							// 打开修复装备界面
	static constexpr ushort BuffTriggerRun = 167;								// 奔跑时触发其他buff
	static constexpr ushort BuffStrengthIncreaseSkillAttack = 168;				// 强化等级越高,增加的技能攻击力越高
	static constexpr ushort BuffStrengthIncreasePhysicsAttack = 169;			// 强化等级越高,增加的物理攻击力越高
	static constexpr ushort BuffStrengthIncreaseMagicAttack = 170;				// 强化等级越高,增加的魔法攻击力越高
	static constexpr ushort BuffStrengthIncreaseSorceryAttack = 171;			// 强化等级越高,增加的道术攻击力越高
	static constexpr ushort BuffCounterAttackUp = 172;							// 破招攻击力提升
	static constexpr ushort BuffCounterPhysicsAttackUp = 173;					// 破招物理攻击力提升
	static constexpr ushort BuffCounterMagicAttackUp = 174;						// 破招魔法攻击力提升
	static constexpr ushort BuffCounterSorceryAttackUp = 175;					// 破招道术攻击力提升
	static constexpr ushort BuffSkillLevelUp = 176;								// 技能等级提升
	static constexpr ushort BuffTriggerBeHit = 177;								// 被攻击时触发
	static constexpr ushort BuffGongShaJianShu = 178;							// 攻杀剑术的效果,增加伤害
	static constexpr ushort BuffTriggerDirectly = 179;							// 直接触发其他buff
	static constexpr ushort BuffPlayerLevelIncreaseMaxHP = 182;					// 角色等级越高,增加的血量越多
	static constexpr ushort BuffPlayerLevelIncreaseMaxMP = 183;					// 角色等级越高,增加的魔法量越多
	static constexpr ushort BuffPlayerLevelIncreasePhysicsAttack = 184;			// 角色等级越高,增加的物理攻击力越多
	static constexpr ushort BuffPlayerLevelIncreaseMagicAttack = 185;			// 角色等级越高,增加的魔法攻击力越多
	static constexpr ushort BuffPlayerLevelIncreaseSorceryAttack = 186;			// 角色等级越高,增加的道术攻击力越多
	static constexpr ushort BuffPlayerLevelIncreasePhysicsDefence = 187;		// 角色等级越高,增加的物理防御力越多
	static constexpr ushort BuffPlayerLevelIncreaseMagicDefence = 188;			// 角色等级越高,增加的魔法防御力越多
	static constexpr ushort BuffTalkConversation = 189;							// 角色之间的对话
	static constexpr ushort BuffTriggerFireSkill = 191;							// 释放技能时触发
	static constexpr ushort BuffPoisoningBlue = 192;							// 蓝毒
	static constexpr ushort BuffBreakArmor = 193;								// 破甲,使当前装备的衣服无效
	static constexpr ushort BuffBreakWeapon = 194;								// 破刃,使当前装备的武器无效
	static constexpr ushort BuffKillRandomPercentMaxHP = 195;					// 在一定百分比范围内随机按照血量上限减血
	static constexpr ushort BuffDamageFixedCycle = 196;							// 持续的固定伤害
	static constexpr ushort BuffDamageFixedRangeCycle = 197;					// 范围内持续固定伤害
	static constexpr ushort BuffTransRandomMap = 198;							// 传送到指定范围的随机地图
	static constexpr ushort BuffCheckInMap = 199;								// 检查是否在指定地图中
	static constexpr ushort BuffTriggerKillMonster = 200;						// 击杀怪物时触发
	static constexpr ushort BuffBaoLieHuoYanTimeUp = 201;						// 爆裂火焰持续时间增加
	static constexpr ushort BuffRedPoisonIncreaseDamageOnce = 202;				// 攻击中红毒状态下的目标时伤害增加
	static constexpr ushort BuffTimeToDecreaseGold = 203;						// 每隔一定时间减少一定金币
	static constexpr ushort BuffLowGoldToBack = 204;							// 金币低于一定值时回城
	static constexpr ushort BuffTriggerLeaveFairyLand = 205;					// 离开幻境地图时触发
	static constexpr ushort BuffTriggerEnterFairyLand = 206;					// 进入幻境地图时触发,需要主动调用
	static constexpr ushort BuffFlashToPlayerNear = 207;						// 瞬移到最近的玩家身边
	static constexpr ushort BuffCounterInjuryCounterAttack = 208;				// 反弹破招伤害
	static constexpr ushort BuffMonsterSeriesIncreaseDamage = 209;				// 对指定系列的怪物增伤
	static constexpr ushort BuffDecreaseMPOnce = 210;							// 单次消耗MP
	static constexpr ushort BuffZhaoHuanKuLouMaxCountUp = 211;					// 增加召唤骷髅数量上限
	static constexpr ushort BuffTriggerBeAddPoisonRed = 212;					// 中红毒时触发
	static constexpr ushort BuffTriggerBeAddPoisonGreen = 213;					// 中绿毒时触发
	static constexpr ushort BuffTriggerBeAddPoisonBlue = 214;					// 中蓝毒时触发
	static constexpr ushort BuffShenShouRecoverHPUp = 215;						// 神兽回血速度增加
	static constexpr ushort BuffRangeEnemyFixedDamageOnce = 216;				// 范围内单次固定伤害,直接设置血量
	static constexpr ushort BuffDecreaseDamageUp = 217;							// 减伤率增加
	static constexpr ushort BuffTriggerMissOtherHit = 218;						// 闪避其他人的攻击时触发
	static constexpr ushort BuffPoisonRedAttackUp = 219;						// 增加施毒术红的伤害
	static constexpr ushort BuffPoisonRedTimeDown = 220;						// 减少施毒术红的持续时间
	static constexpr ushort BuffTriggerBeMagicHit = 221;						// 被魔法攻击时触发
	static constexpr ushort BuffTriggerRunStart = 222;							// 开始奔跑时触发
	static constexpr ushort BuffHuoQiangIntervalTimeDown = 223;					// 减少火墙间隔时间
	static constexpr ushort BuffHuoQiangTimeDown = 224;							// 减少火墙持续时间
	static constexpr ushort BuffZhaoHuanShenShouInheritAttackUp = 225;			// 增加神兽的继承攻击力比率
	static constexpr ushort BuffZhaoHuanShenShouInheritDefenceUp = 226;			// 增加神兽的继承防御力比率
	static constexpr ushort BuffIncreaseDamageUp = 227;							// 增伤率提升
	static constexpr ushort BuffTriggerBeenHitOverMaxHPPercent = 228;			// 受到的伤害超过血量上限一定百分比时触发
	static constexpr ushort BuffTriggerSummonMonster = 229;						// 召唤怪物时触发
	static constexpr ushort BuffIncreaseDamageByMonsterStrength = 230;			// 对一定强度以上的怪物的攻击力增加
	static constexpr ushort BuffZhaoHuanShenShouInheritCriticalProbabilityUp = 231;// 增加神兽的继承暴击率比率
	static constexpr ushort BuffTriggerSpecificSkillAttack = 232;				// 指定技能命中时触发
	static constexpr ushort BuffDecreaseDamageDown = 233;						// 降低减伤率
	static constexpr ushort BuffTriggerFireSkillTimes = 234;					// 释放一定次数的技能后触发
	static constexpr ushort BuffMultipleDamageOnce = 235;						// 将伤害变为多倍,只生效一次
	static constexpr ushort BuffTransferDamageToSummonImmediately = 236;		// 立即将部分伤害转移到召唤身上
	static constexpr ushort BuffRangeEnemyAttackOnce = 237;						// 范围内单次攻击伤害,根据伤害公式计算伤害
	static constexpr ushort BuffStrengthIncreaseDamageUp = 238;					// 装备强化等级超过一定时增加增伤率
	static constexpr ushort BuffTriggerMissOtherHitContinue = 239;				// 连续闪避攻击时触发
	static constexpr ushort BuffRemoveCrime = 240;								// 减少罪恶值
	static constexpr ushort BuffSkillFireIncrease = 241;						// 增加触发此buff的技能的攻击力
	static constexpr ushort BuffMultiplePickGold = 242;							// 拾取金币增加
	static constexpr ushort BuffMultiplePickGoldIngot = 243;					// 拾取元宝增加
	static constexpr ushort BuffTriggerWillHitLowHP = 244;						// 攻击血量低于一定百分比的角色时触发
	static constexpr ushort BuffTriggerBeHitByMonster = 245;					// 被怪物攻击时触发
	static constexpr ushort BuffTriggerAttackPlayer = 246;						// 攻击玩家时触发
	static constexpr ushort BuffTriggerAttackInTimeSpan = 247;					// 在指定时间段攻击时触发
	static constexpr ushort BuffStrengthMaxHPUp = 248;							// 强化等级超过一定时增加HP上限
	static constexpr ushort BuffStrengthMaxMPUp = 249;							// 强化等级超过一定时增加MP上限
	static constexpr ushort BuffStrengthMagicAttackUp = 250;					// 强化等级超过一定时增加魔法攻击力
	static constexpr ushort BuffStrengthPhysicsDefenceUp = 251;					// 强化等级超过一定时增加物理防御力
	static constexpr ushort BuffStrengthMagicDefenceUp = 252;					// 强化等级超过一定时增加魔法防御力
	static constexpr ushort BuffAddAllSummonBuff = 253;							// 给所有召唤兽附加buff
	static constexpr ushort BuffAddRandomSummonBuff = 254;						// 给随机一个召唤兽附加buff
	static constexpr ushort BuffShenShouDecreaseDamageUp = 255;					// 增加神兽的减伤率
	static constexpr ushort BuffOpenChangeName = 256;							// 打开改名界面
	static constexpr ushort BuffAddLowestHPSummonBuff = 257;					// 给血量百分比最低的召唤兽附加buff
	static constexpr ushort BuffRangePlayerCountMakeMagicDefenceDown = 258;		// 周围一定范围内玩家数量越多,物理防御力越低
	static constexpr ushort BuffRangePlayerCountMakePhysicsDefenceDown = 259;	// 周围一定范围内玩家数量越多,魔法防御力越低
	static constexpr ushort BuffRangePlayerCountMakeDamageDecreaseDown = 260;	// 周围一定范围内玩家数量越多,减伤率越低
	static constexpr ushort BuffMagicDecreaseUp = 261;							// 魔法减伤率增加
	static constexpr ushort BuffPhysicsDecreaseUp = 262;						// 物理减伤率增加
	static constexpr ushort BuffSorceryDecreaseUp = 263;						// 道术减伤率增加
	static constexpr ushort BuffTriggerTimeCycle = 264;							// 每隔一定时间触发一次
	static constexpr ushort BuffRangePlayerKillHPOnce = 265;					// 周围一定范围内的玩家血量减少
	static constexpr ushort BuffNoDefencePhysicsAttackUp = 266;					// 无视防御的物理攻击力提升
	static constexpr ushort BuffNoDefenceMagicAttackUp = 267;					// 无视防御的魔法攻击力提升
	static constexpr ushort BuffNoDefenceSorceryAttackUp = 268;					// 无视防御的道术攻击力提升
	static constexpr ushort BuffOpenBlessEquip = 269;							// 打开祝福装备界面
	static constexpr ushort BuffTriggerWillDie = 270;							// 即将死亡时触发
	static constexpr ushort BuffInvinciblePhysicsDamageSingle = 271;			// 单次免疫物理伤害
	static constexpr ushort BuffInvincibleMagicDamageSingle = 272;				// 单次免疫魔法伤害
	static constexpr ushort BuffInvincibleSorceryDamageSingle = 273;			// 单次免疫道术伤害
	static constexpr ushort BuffInvincibleCounterAttackSingle = 274;			// 单次免疫破招伤害
	static constexpr ushort BuffInvincibleCriticalSingle = 275;					// 单次免疫暴击伤害
	static constexpr ushort BuffBreakShield = 276;								// 破盾
	static constexpr ushort BuffSuckMP = 277;									// 吸蓝
	static constexpr ushort BuffTriggerCounterAttack = 278;						// 破招时触发
	static constexpr ushort BuffAllSummonBuffContinous = 279;					// 持续给所有召唤兽附加buff
	static constexpr ushort BuffInvincibleOverHPPercentDamage = 280;			// 免疫超过一定血量百分比的伤害
	static constexpr ushort BuffTriggerWillBeHit = 281;							// 即将被攻击时触发
	static constexpr ushort BuffDetectInvisible = 282;							// 可发现隐身的玩家
	static constexpr ushort BuffTriggerSummonDie = 283;							// 召唤兽死亡时触发
	static constexpr ushort BuffTriggerRangeEnemy = 284;						// 直接触发一次范围内敌人附加指定buff
	static constexpr ushort BuffTriggerRangeFriend = 285;						// 直接触发一次范围内友方附加指定buff
	static constexpr ushort BuffMoreMPMorePhysicsAttack = 286;					// MP越高增加的物理攻击力越多
	static constexpr ushort BuffMoreMPMoreMagicAttack = 287;					// MP越高增加的魔法攻击力越多
	static constexpr ushort BuffMoreMPMoreSorceryAttack = 288;					// MP越高增加的道术攻击力越多
	static constexpr ushort BuffLessHPMoreNoDefencePhysicsAttack = 289;			// HP越低增加的无视防御物理攻击力越多
	static constexpr ushort BuffLessHPMoreNoDefenceMagicAttack = 290;			// HP越低增加的无视防御魔法攻击力越多
	static constexpr ushort BuffLessHPMoreNoDefenceSorceryAttack = 291;			// HP越低增加的无视防御道术攻击力越多
	static constexpr ushort BuffTriggerSpecificSkillAttackPlayer = 292;			// 指定技能命中玩家时触发
	static constexpr ushort BuffNotAllowUseConsumable = 293;					// 禁止使用消耗品
	static constexpr ushort BuffConsecrationEffect = 294;						// 武器开光触发效果
	static constexpr ushort BuffWeaponPhysicsAttackUp = 295;					// 增加武器基础物理攻击力的百分比
	static constexpr ushort BuffWeaponMagicAttackUp = 296;						// 增加武器基础魔法攻击力的百分比
	static constexpr ushort BuffWeaponSorceryAttackUp = 297;					// 增加武器基础道术攻击力的百分比
	static constexpr ushort BuffReviveTrigger = 298;							// 专用于触发复活效果的触发器
	static constexpr ushort BuffEquipStrengthIncreaseAttack = 299;				// 装备强化等级超过一定时增加攻击力,且强化越高增加越多
	static constexpr ushort BuffHighPlayerLevelDecreaseAttack = 300;			// 角色等级超过一定时,等级越高,增加的攻击力越少
	static constexpr ushort BuffTriggerWillHitHasTypeBuff = 301;				// 即将命中拥有指定buff的敌人时触发
	static constexpr ushort BuffRecoverHPOnceByBeenDamage = 302;				// 立即恢复与受到的伤害相同的血量
	static constexpr ushort BuffRangeSummonCountIncreaseDefense = 303;			// 范围内召唤兽数量越多,增加的防御力越多
	static constexpr ushort BuffRangeFriendPlayerCountIncreaseAttack = 304;		// 一定范围内友方玩家数量越多,增加的攻击力越多
	static constexpr ushort BuffRangeEnemyPlayerCountDecreaseDamageUp = 305;	// 一定范围内敌方玩家数量越多,增加的减伤率越多
	static constexpr ushort BuffLessHPMoreDecreaseDamage = 306;					// 血量低于一定百分比时血量越低增加的减伤率越多
	static constexpr ushort BuffLessHPMoreIncreaseDamage = 307;					// 血量低于一定百分比时血量越低增加的增伤率越多
	static constexpr ushort BuffLessHPMoreSkillAttack = 308;					// 血量低于一定百分比时血量越低增加的技能攻击力越多
	static constexpr ushort BuffLessHPMoreCriticalDamage = 309;					// 血量低于一定百分比时血量越低增加的暴击伤害越多
	static constexpr ushort BuffLessHPMoreEvade = 310;							// 血量低于一定百分比时血量越低增加的闪避率越多
	static constexpr ushort BuffAllAttackUp = 311;								// 提升所有攻击力
	static constexpr ushort BuffAllDefenseUp = 312;								// 提升所有防御力
	static constexpr ushort BuffDecreaseDamageShield = 313;						// 减伤盾
	static constexpr ushort BuffTriggerBeCriticalHit = 314;						// 被暴击时触发
	static constexpr ushort BuffInvincibleDamageOnce = 315;						// 单次免疫伤害
	static constexpr ushort BuffEquipStrengthIncreaseCounterAttack = 316;		// 装备强化等级超过一定时增加破招攻击力
	static constexpr ushort BuffTriggerBeCounterAttack = 317;					// 被破招时触发
	static constexpr ushort BuffSpecificMonsterStrengthIncreaseDamage = 318;	// 对指定强度的怪物的伤害增加
	static constexpr ushort BuffTriggerBePhysicsHit = 319;						// 被物理攻击时触发
	static constexpr ushort BuffTriggerBeSorceryHit = 320;						// 被道术攻击时触发
	static constexpr ushort BuffTriggerAttackInSomeWeekDay = 321;				// 在一周的周几攻击时触发
	static constexpr ushort BuffIncreaseDamageOnce = 322;						// 单次增加伤害
	static constexpr ushort BuffIncreaseDamageOnceFixed = 323;					// 单次增加固定伤害
	static constexpr ushort BuffIncreaseDamageToMonsterWar = 324;				// 对攻城怪物的伤害增加
	static constexpr ushort BuffIncreaseDamageInArena = 325;					// 竞技场中伤害增加
	static constexpr ushort BuffIncreaseEvadeInArena = 326;						// 竞技场中闪避率增加
	static constexpr ushort BuffIncreaseMoveSpeedInArena = 327;					// 竞技场中移动速度增加
	static constexpr ushort BuffTriggerBeHitBySpecificSkill = 334;				// 被指定技能攻击时触发
	static constexpr ushort BuffHitBySummonMakeSummonDie = 335;					// 被召唤兽攻击时使召唤兽立即死亡
	static constexpr ushort BuffRecoverHPWhenCounterInjuryFromPlayer = 336;		// 被玩家反伤时可回复反伤值一定百分比的血量
	static constexpr ushort BuffIncreasePickGoldInSpecificMap = 338;			// 在指定地图中拾取金币加成
	static constexpr ushort BuffTriggerBeHitByPlayer = 339;						// 被玩家攻击时触发
	static constexpr ushort BuffTriggerWillBeHitByPlayerHigherLevel = 340;		// 即将被等级比自己高的玩家攻击时触发
	static constexpr ushort BuffDecreaseDamageOnce = 341;						// 单次减少伤害
	static constexpr ushort BuffTriggerHPOverPercent1 = 342;					// 血量高于一定百分比时触发buff,低于百分比时移除buff
	static constexpr ushort BuffTriggerHPUnderPercent1 = 343;					// 血量低于一定百分比时触发buff,高于百分比时移除buff
	static constexpr ushort BuffCreateMonster = 344;							// 在当前位置刷新一个怪物出来
	static constexpr ushort BuffInverseRecoverHP = 345;							// 使用药剂回血的效果变为掉血
	static constexpr ushort BuffLingHunHuoFuToTrack = 346;						// 使灵魂火符变为追踪效果
	static constexpr ushort BuffBeenTakeDura = 347;								// 被吸取身上装备的耐久度
	static constexpr ushort BuffEmptyPackGridIncreaseAttack = 348;				// 背包每空出一个格子增加一定的攻击力
	static constexpr ushort BuffSpecificMonsterStrengthDecreaseDamage = 349;	// 对指定强度的怪物的伤害减少
	static constexpr ushort BuffBeenStealGold = 350;							// 被窃取金币
	static constexpr ushort BuffTriggerSwitchWeapon = 351;						// 切换武器时触发
	static constexpr ushort BuffRecoverHPOnceByAttackDamage = 352;				// 回复与攻击伤害一定百分比的血量
	static constexpr ushort BuffIncreaseAttackByEquip = 353;					// 根据穿戴指定的装备数量增加攻击力
	static constexpr ushort BuffTriggerAttackCrime = 354;						// 命中有罪恶的玩家时触发
	static constexpr ushort BuffMaxHPAddDecreaseDamage = 355;					// 根据当前最大血量增加减伤率
	static constexpr ushort BuffTriggerStand = 356;								// 站立一定时间时触发,移动时移除
	static constexpr ushort BuffAllSkillCDDown = 357;							// 所有技能冷却减少
	static constexpr ushort BuffTriggerBuffToAllSummon = 358;					// 给所有召唤兽附加buff
	static constexpr ushort BuffSpecificMonsterStrengthIncreaseDamage1 = 359;   // 对指定强度的怪物的伤害增加,且随装备强化等级提升
	static constexpr ushort BuffTriggerOverCountHitMonster = 360;				// 累计对指定强度的怪物造成一定次数伤害时触发
	static constexpr ushort BuffNextSkillAttackUp = 361;						// 下一次释放的技能攻击力增加
	static constexpr ushort BuffAllDefenseDown = 362;                           // 降低所有防御力
	static constexpr ushort BuffAllAttackDown = 363;							// 降低所有攻击力
	static constexpr ushort BuffTriggerAttackWhileLowHP = 364;					// 自身血量低于一定百分比时命中敌人时触发
	static constexpr ushort BuffTriggerSummonBeHit = 365;						// 召唤兽被攻击时触发
	static constexpr ushort BuffTriggerKillMonsterBuffToRangeMonster = 366;		// 击杀怪物时给死亡怪物附近所有怪物附加buff
	static constexpr ushort BuffIncreaseDamageInSceneCopy = 367;				// 对副本中的怪物伤害增加
	static constexpr ushort BuffOnlinePlayerGetGold = 368;						// 全服所有在线玩家获得随机金币
	static constexpr ushort BuffForeverMaxHPUp = 369;							// 永久提升血量上限
	static constexpr ushort BuffForeverMaxMPUp = 370;							// 永久提升魔法量上限
	static constexpr ushort BuffForeverPhysicsAttackUp = 371;					// 永久提升物理攻击
	static constexpr ushort BuffForeverMagicAttackUp = 372;						// 永久提升魔法攻击
	static constexpr ushort BuffForeverSorceryAttackUp = 373;					// 永久提升道术攻击
	static constexpr ushort BuffForeverAttackSpeedUp = 374;						// 永久提升攻击速度
	static constexpr ushort BuffForeverMoveSpeedUp = 375;						// 永久提升移动速度
	static constexpr ushort BuffForeverPhysicsDefenceUp = 379;					// 永久提升物理防御
	static constexpr ushort BuffForeverMagicDefenceUp = 380;					// 永久提升魔法防御
	static constexpr ushort BuffForeverHitProbabilityUp = 381;					// 永久提升命中率
	static constexpr ushort BuffForeverEvadeProbabilityUp = 382;				// 永久提升闪避率
	static constexpr ushort BuffForeverCriticalProbabilityUp = 383;				// 永久提升暴击率
	static constexpr ushort BuffForeverCriticalDamageUp = 384;					// 永久提升暴击伤害
	static constexpr ushort BuffForeverAntiCriticalUp = 385;					// 永久提升抗暴率
	static constexpr ushort BuffRepairWeapon = 376;								// 修复武器
	static constexpr ushort BuffRepairAllEquip = 377;							// 修复全身装备
	static constexpr ushort BuffTransToShabak = 378;							// 传送到沙巴克
	static constexpr ushort BuffIncreaseDamageToOtherGuildPlayer = 386;			// 攻击不同行会的玩家时伤害增加
	static constexpr ushort BuffTeamIncreaseDamage = 387;						// 组队时伤害增加
	static constexpr ushort BuffTeamIncreaseCritical = 388;						// 组队时暴击率增加
	static constexpr ushort BuffTeamIncreaseExp = 389;							// 组队时经验增加
	static constexpr ushort BuffTeamIncreaseDamageUp = 390;						// 组队时增伤率增加
	static constexpr ushort BuffTeamDecreaseDamageUp = 391;						// 组队时减伤率增加
	static constexpr ushort BuffTriggerTeamPlayerDie = 392;						// 范围内有队友死亡时触发
	static constexpr ushort BuffIncreaseCriticalWhenTeamHasOtherJobPlayer = 393;		// 队伍中有不同职业的玩家时增加暴击率
	static constexpr ushort BuffIncreaseSkillAttackWhenTeamHasSameGuildPlayer = 394;	// 队伍中有相同行会的玩家时技能攻击力增加

	// Action
	static constexpr ushort StateActionAttack = 10001;
	static constexpr ushort StateActionUnborn = 10002;
	static constexpr ushort StateActionBorn = 10003;
	static constexpr ushort StateActionDestroy = 10004;
	static constexpr ushort StateActionDie = 10005;
	static constexpr ushort StateActionHit = 10006;
	static constexpr ushort StateActionSearch = 10007;
	static constexpr ushort StateActionStand = 10008;
	static constexpr ushort StateActionReborn = 10009;
	static constexpr ushort StateActionWalk = 10010;
	static constexpr ushort StateActionRun = 10011;
	static constexpr ushort StateActionDig = 10012;
	// Behaviour
	static constexpr ushort StateBehaviourMove = 10013;
	static constexpr ushort StateBehaviourFlash = 10014;
};