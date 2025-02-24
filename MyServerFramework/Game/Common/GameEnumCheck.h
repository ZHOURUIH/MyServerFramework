#pragma once

#include "GameEnum.h"

// auto generated file
class GameEnumCheck
{
public:
	static constexpr bool checkEnum(const ACHIVEMENT_CATEGORY value)
	{
		switch (value)
		{
		case ACHIVEMENT_CATEGORY::NONE:break;
		case ACHIVEMENT_CATEGORY::PVP:break;
		case ACHIVEMENT_CATEGORY::PVE:break;
		case ACHIVEMENT_CATEGORY::GROWTH:break;
		case ACHIVEMENT_CATEGORY::NORMAL_LIFE:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ACHIVEMENT_TYPE value)
	{
		switch (value)
		{
		case ACHIVEMENT_TYPE::NONE:break;
		case ACHIVEMENT_TYPE::LEVEL:break;
		case ACHIVEMENT_TYPE::KILL_MONSTER:break;
		case ACHIVEMENT_TYPE::KILL_ELITE_MONSTER:break;
		case ACHIVEMENT_TYPE::KILL_BOSS:break;
		case ACHIVEMENT_TYPE::MOVE:break;
		case ACHIVEMENT_TYPE::KILL_PLAYER:break;
		case ACHIVEMENT_TYPE::ONLINE_TIME:break;
		case ACHIVEMENT_TYPE::LOGIN_DAY:break;
		case ACHIVEMENT_TYPE::STRENGTHEN_WEAPON:break;
		case ACHIVEMENT_TYPE::PICK_UP_LUCKY_OIL:break;
		case ACHIVEMENT_TYPE::HIT_BY_MONSTER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ACTION_STATE_TYPE value)
	{
		switch (value)
		{
		case ACTION_STATE_TYPE::NONE:break;
		case ACTION_STATE_TYPE::TRACKING:break;
		case ACTION_STATE_TYPE::TRACKING_DONE:break;
		case ACTION_STATE_TYPE::TRACKING_WAIT:break;
		case ACTION_STATE_TYPE::ATTACK_DONE:break;
		case ACTION_STATE_TYPE::ATTACK_WAIT:break;
		case ACTION_STATE_TYPE::WANDER_WAIT:break;
		case ACTION_STATE_TYPE::WANDER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ANIMATION value)
	{
		switch (value)
		{
		case ANIMATION::NONE:break;
		case ANIMATION::ATTACK:break;
		case ANIMATION::SKILL:break;
		case ANIMATION::RUN:break;
		case ANIMATION::WALK:break;
		case ANIMATION::DIE:break;
		case ANIMATION::HIT:break;
		case ANIMATION::STAND:break;
		case ANIMATION::DIG:break;
		case ANIMATION::SEARCH:break;
		case ANIMATION::JUMP_ATTACK:break;
		case ANIMATION::RUSH_ATTACK:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const AUCTION_SEARCH_CONDITION value)
	{
		switch (value)
		{
		case AUCTION_SEARCH_CONDITION::NAME:break;
		case AUCTION_SEARCH_CONDITION::MIN_LEVEL:break;
		case AUCTION_SEARCH_CONDITION::MAX_LEVEL:break;
		case AUCTION_SEARCH_CONDITION::MIN_STRENGTHEN:break;
		case AUCTION_SEARCH_CONDITION::MAX_STRENGTHEN:break;
		case AUCTION_SEARCH_CONDITION::OBJECT_TYPE:break;
		case AUCTION_SEARCH_CONDITION::JOB:break;
		case AUCTION_SEARCH_CONDITION::EQUIP_TYPE:break;
		case AUCTION_SEARCH_CONDITION::QUALITY:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const BUBBLE_GROUP value)
	{
		switch (value)
		{
		case BUBBLE_GROUP::RANDOM_ONCE:break;
		case BUBBLE_GROUP::SEQUENCE:break;
		case BUBBLE_GROUP::CONVERSATION:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const BULLET_TARGET_TYPE value)
	{
		switch (value)
		{
		case BULLET_TARGET_TYPE::NONE:break;
		case BULLET_TARGET_TYPE::SELF:break;
		case BULLET_TARGET_TYPE::TARGET:break;
		case BULLET_TARGET_TYPE::RANDOM:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CAMP_TYPE value)
	{
		switch (value)
		{
		case CAMP_TYPE::NONE:break;
		case CAMP_TYPE::FRIEND:break;
		case CAMP_TYPE::ENEMY:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CHANGE_NAME_RESULT value)
	{
		switch (value)
		{
		case CHANGE_NAME_RESULT::SUCCESS:break;
		case CHANGE_NAME_RESULT::SAME_MYSELF:break;
		case CHANGE_NAME_RESULT::INVALID_NAME:break;
		case CHANGE_NAME_RESULT::NAME_EXIST:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CHARACTER_BEHAVIOUR value)
	{
		switch (value)
		{
		case CHARACTER_BEHAVIOUR::NONE:break;
		case CHARACTER_BEHAVIOUR::SHEN_FA_LEI:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CHARACTER_PROPERTY value)
	{
		switch (value)
		{
		case CHARACTER_PROPERTY::NONE:break;
		case CHARACTER_PROPERTY::HP:break;
		case CHARACTER_PROPERTY::MAX_HP:break;
		case CHARACTER_PROPERTY::MP:break;
		case CHARACTER_PROPERTY::MAX_MP:break;
		case CHARACTER_PROPERTY::MIN_PHYSICS_ATTACK:break;
		case CHARACTER_PROPERTY::MAX_PHYSICS_ATTACK:break;
		case CHARACTER_PROPERTY::MIN_MAGIC_ATTACK:break;
		case CHARACTER_PROPERTY::MAX_MAGIC_ATTACK:break;
		case CHARACTER_PROPERTY::MIN_SORCERY_ATTACK:break;
		case CHARACTER_PROPERTY::MAX_SORCERY_ATTACK:break;
		case CHARACTER_PROPERTY::MIN_PHYSICS_DEFENCE:break;
		case CHARACTER_PROPERTY::MAX_PHYSICS_DEFENCE:break;
		case CHARACTER_PROPERTY::MIN_MAGIC_DEFENCE:break;
		case CHARACTER_PROPERTY::MAX_MAGIC_DEFENCE:break;
		case CHARACTER_PROPERTY::ATTACK_SPEED:break;
		case CHARACTER_PROPERTY::MOVE_SPEED:break;
		case CHARACTER_PROPERTY::HIT_PROBABILITY:break;
		case CHARACTER_PROPERTY::EVADE_PROBABILITY:break;
		case CHARACTER_PROPERTY::CRITICAL_PROBABILITY:break;
		case CHARACTER_PROPERTY::CRITICAL_DAMAGE_RATE:break;
		case CHARACTER_PROPERTY::DECREASE_DAMAGE_RATE:break;
		case CHARACTER_PROPERTY::INCREASE_DAMAGE_RATE:break;
		case CHARACTER_PROPERTY::CRIME:break;
		case CHARACTER_PROPERTY::LUCKY:break;
		case CHARACTER_PROPERTY::HP_RECOVER:break;
		case CHARACTER_PROPERTY::MP_RECOVER:break;
		case CHARACTER_PROPERTY::PHYSICS_DECREASE:break;
		case CHARACTER_PROPERTY::MAGIC_DECREASE:break;
		case CHARACTER_PROPERTY::SORCERY_DECREASE:break;
		case CHARACTER_PROPERTY::DIG_MINE_SPEED:break;
		case CHARACTER_PROPERTY::SKILL_ATTACK_INCREASE:break;
		case CHARACTER_PROPERTY::ANTI_CRITICAL:break;
		case CHARACTER_PROPERTY::COUNTER_ATTACK:break;
		case CHARACTER_PROPERTY::COUNTER_PHYSICS_ATTACK:break;
		case CHARACTER_PROPERTY::COUNTER_MAGIC_ATTACK:break;
		case CHARACTER_PROPERTY::COUNTER_SORCERY_ATTACK:break;
		case CHARACTER_PROPERTY::PACK_MAX_WEIGHT:break;
		case CHARACTER_PROPERTY::NO_DEFENCE_PHYSICS_ATTACK:break;
		case CHARACTER_PROPERTY::NO_DEFENCE_MAGIC_ATTACK:break;
		case CHARACTER_PROPERTY::NO_DEFENCE_SORCERY_ATTACK:break;
		case CHARACTER_PROPERTY::MONSTER_EXP_MULTI:break;
		case CHARACTER_PROPERTY::SKILL_EXP_MULTI:break;
		case CHARACTER_PROPERTY::PICK_GOLD_MULTI:break;
		case CHARACTER_PROPERTY::PICK_GOLD_INGOT_MULTI:break;
		case CHARACTER_PROPERTY::EQUIP_STRENGTH:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CHARACTER_RANGE value)
	{
		switch (value)
		{
		case CHARACTER_RANGE::NONE:break;
		case CHARACTER_RANGE::PLAYER:break;
		case CHARACTER_RANGE::MONSTER:break;
		case CHARACTER_RANGE::SUMMON:break;
		case CHARACTER_RANGE::NORMAL_MONSTER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CHAT_MESSAGE value)
	{
		switch (value)
		{
		case CHAT_MESSAGE::WORLD:break;
		case CHAT_MESSAGE::LOCAL:break;
		case CHAT_MESSAGE::PRIVATE:break;
		case CHAT_MESSAGE::SYSTEM:break;
		case CHAT_MESSAGE::SPECIAL:break;
		case CHAT_MESSAGE::GUILD:break;
		case CHAT_MESSAGE::TEAM:break;
		case CHAT_MESSAGE::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CLIENT_PLATFORM value)
	{
		switch (value)
		{
		case CLIENT_PLATFORM::NONE:break;
		case CLIENT_PLATFORM::ANDROID:break;
		case CLIENT_PLATFORM::PC:break;
		case CLIENT_PLATFORM::IOS:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CONDITION value)
	{
		switch (value)
		{
		case CONDITION::NONE:break;
		case CONDITION::LEVEL:break;
		case CONDITION::KILL_MONSTER:break;
		case CONDITION::GET_OBJECT:break;
		case CONDITION::DRESS_EQUIP:break;
		case CONDITION::TALK_FINISH:break;
		case CONDITION::ENTER_MAP_AREA:break;
		case CONDITION::FIRE_SKILL:break;
		case CONDITION::DRESS_EQUIP_TYPE:break;
		case CONDITION::LEARN_SKILL:break;
		case CONDITION::STRENGTH_EQUIP:break;
		case CONDITION::DRESS_EQUIP_TYPE_QUALITY:break;
		case CONDITION::PICK_MONSTER_DROP_ITEM:break;
		case CONDITION::KILL_STRENGTH_MONSTER:break;
		case CONDITION::ENTER_SCENE_COPY:break;
		case CONDITION::WIN_HIGH_LEVEL_SINGLE_ARENA:break;
		case CONDITION::WIN_LOW_LEVEL_SINGLE_ARENA:break;
		case CONDITION::TRIGGER_LUCKY_DROP:break;
		case CONDITION::PICK_OVER_COUNT_GOLD:break;
		case CONDITION::PICK_OVER_COUNT_GOLD_INGOT:break;
		case CONDITION::STRENGTH_QUALITY_WEAPON_TO_LEVEL:break;
		case CONDITION::STRENGTH_EQUIP_TO_LEVEL:break;
		case CONDITION::QUALITY_WEAPON_CONSECRATION:break;
		case CONDITION::QUALITY_WEAPON_CONSECRATION_SUCCESS:break;
		case CONDITION::SKILL_USE_WHITE_PLUME:break;
		case CONDITION::MINE_GET_OBJECT:break;
		case CONDITION::KILL_ANY_MONSTER:break;
		case CONDITION::SELL_NPC_SHOP:break;
		case CONDITION::USE_CONSUMABLE:break;
		case CONDITION::TEAM_KILL_ANY_MONSTER:break;
		case CONDITION::DECOMPOSE_GET_WHITE_PLUME:break;
		case CONDITION::ENTER_SHENGYU:break;
		case CONDITION::ENTER_FAIRY_LAND:break;
		case CONDITION::WIN_SINGLE_ARENA:break;
		case CONDITION::BUY_AUCTION:break;
		case CONDITION::BLESS_WEAPON:break;
		case CONDITION::ENTER_ANY_HELL:break;
		case CONDITION::PICK_GOLD:break;
		case CONDITION::PUT_ON_AUCTION:break;
		case CONDITION::ENTER_UNKNOWN_DARK_PLACE:break;
		case CONDITION::REFRESH_EQUIP_PRECIOUS:break;
		case CONDITION::REPAIR_EQUIP:break;
		case CONDITION::NORMAL_GACHA:break;
		case CONDITION::CONTRIBUTE_GOLD:break;
		case CONDITION::BUY_LOTTERY_TICKET:break;
		case CONDITION::DAMAGE_TO_MONSTER:break;
		case CONDITION::PASS_BUGUILU:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CREATE_CHARACTER_RESULT value)
	{
		switch (value)
		{
		case CREATE_CHARACTER_RESULT::SUCCESS:break;
		case CREATE_CHARACTER_RESULT::NAME_DUPLICATE:break;
		case CREATE_CHARACTER_RESULT::NAME_ERROR:break;
		case CREATE_CHARACTER_RESULT::CHARACTER_FULL:break;
		case CREATE_CHARACTER_RESULT::OTHER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const CRIME_LEVEL value)
	{
		switch (value)
		{
		case CRIME_LEVEL::NONE:break;
		case CRIME_LEVEL::GREY:break;
		case CRIME_LEVEL::YELLOW:break;
		case CRIME_LEVEL::RED:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const DAMAGE_TYPE value)
	{
		switch (value)
		{
		case DAMAGE_TYPE::PHYSICS:break;
		case DAMAGE_TYPE::MAGIC:break;
		case DAMAGE_TYPE::SORCERY:break;
		case DAMAGE_TYPE::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const DRESS_OPERATE value)
	{
		switch (value)
		{
		case DRESS_OPERATE::DRESS:break;
		case DRESS_OPERATE::UNDRESS:break;
		case DRESS_OPERATE::REPLACE_POSITION:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const DROP_TYPE value)
	{
		switch (value)
		{
		case DROP_TYPE::DIE_DROP:break;
		case DROP_TYPE::MANUAL_DROP:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const DURA_DELTA_TYPE value)
	{
		switch (value)
		{
		case DURA_DELTA_TYPE::NONE:break;
		case DURA_DELTA_TYPE::REPAIR:break;
		case DURA_DELTA_TYPE::SPECIAL_REPAIR:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ENVIRONMENT_SEARCH value)
	{
		switch (value)
		{
		case ENVIRONMENT_SEARCH::NONE:break;
		case ENVIRONMENT_SEARCH::NEAREST:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const EQUIP_CREATE value)
	{
		switch (value)
		{
		case EQUIP_CREATE::NONE:break;
		case EQUIP_CREATE::MONSTER_DROP:break;
		case EQUIP_CREATE::SYSTEM:break;
		case EQUIP_CREATE::MAKE:break;
		case EQUIP_CREATE::GM_MAKE:break;
		case EQUIP_CREATE::SHOP:break;
		case EQUIP_CREATE::MISSION:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const EQUIP_EXTRA_ATTACK value)
	{
		switch (value)
		{
		case EQUIP_EXTRA_ATTACK::NONE:break;
		case EQUIP_EXTRA_ATTACK::PHYSICS:break;
		case EQUIP_EXTRA_ATTACK::MAGIC:break;
		case EQUIP_EXTRA_ATTACK::SORCERY:break;
		case EQUIP_EXTRA_ATTACK::ALL:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const EQUIP_FLAG value)
	{
		switch (value)
		{
		case EQUIP_FLAG::NONE:break;
		case EQUIP_FLAG::SPECIAL_RING:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const EQUIP_POSITION value)
	{
		switch (value)
		{
		case EQUIP_POSITION::CLOTH:break;
		case EQUIP_POSITION::WEAPON:break;
		case EQUIP_POSITION::HELMET:break;
		case EQUIP_POSITION::NECKLACK:break;
		case EQUIP_POSITION::MEDAL:break;
		case EQUIP_POSITION::LEFT_BRACELET:break;
		case EQUIP_POSITION::RIGHT_BRACELET:break;
		case EQUIP_POSITION::LEFT_RING:break;
		case EQUIP_POSITION::RIGHT_RING:break;
		case EQUIP_POSITION::BELT:break;
		case EQUIP_POSITION::SHOE:break;
		case EQUIP_POSITION::DIAMOND:break;
		case EQUIP_POSITION::WING:break;
		case EQUIP_POSITION::FASHION_CLOTH:break;
		case EQUIP_POSITION::FASHION_WEAPON:break;
		case EQUIP_POSITION::FASHION_WING:break;
		case EQUIP_POSITION::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const EQUIP_TYPE value)
	{
		switch (value)
		{
		case EQUIP_TYPE::CLOTH:break;
		case EQUIP_TYPE::WEAPON:break;
		case EQUIP_TYPE::HELMET:break;
		case EQUIP_TYPE::NECKLACK:break;
		case EQUIP_TYPE::MEDAL:break;
		case EQUIP_TYPE::BRACELET:break;
		case EQUIP_TYPE::RING:break;
		case EQUIP_TYPE::BELT:break;
		case EQUIP_TYPE::SHOE:break;
		case EQUIP_TYPE::DIAMOND:break;
		case EQUIP_TYPE::WING:break;
		case EQUIP_TYPE::FASHION_CLOTH:break;
		case EQUIP_TYPE::FASHION_WEAPON:break;
		case EQUIP_TYPE::FASHION_WING:break;
		case EQUIP_TYPE::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const GAME_VERIFY value)
	{
		switch (value)
		{
		case GAME_VERIFY::NONE:break;
		case GAME_VERIFY::SEARCH_WEAPON:break;
		case GAME_VERIFY::SEARCH_JEWELRY:break;
		case GAME_VERIFY::PUT_ON_AUCTION:break;
		case GAME_VERIFY::LOCAL_CHAT:break;
		case GAME_VERIFY::BUY_GOLD_INGOT_HUICHENGJUAN:break;
		case GAME_VERIFY::DROP_ITEM:break;
		case GAME_VERIFY::FIRE_BASE_ATTACK:break;
		case GAME_VERIFY::NPC_TRANSFER:break;
		case GAME_VERIFY::BUY_NPC_WEAPON:break;
		case GAME_VERIFY::BUY_NPC_CLOTH:break;
		case GAME_VERIFY::BUY_NPC_RING:break;
		case GAME_VERIFY::OPEN_RANK:break;
		case GAME_VERIFY::UNDRESS_WEAPON:break;
		case GAME_VERIFY::UNDRESS_CLOTH:break;
		case GAME_VERIFY::UNDRESS_NECKLACK:break;
		case GAME_VERIFY::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const GOODS_SHOP value)
	{
		switch (value)
		{
		case GOODS_SHOP::NPC:break;
		case GOODS_SHOP::GUILD:break;
		case GOODS_SHOP::MALL:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const GUILD_TITLE value)
	{
		switch (value)
		{
		case GUILD_TITLE::NONE:break;
		case GUILD_TITLE::PRESIDENT:break;
		case GUILD_TITLE::VICE_PRESIDENT:break;
		case GUILD_TITLE::ELDER:break;
		case GUILD_TITLE::ELITE_MEMBER:break;
		case GUILD_TITLE::MEMBER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const HP_DELTA value)
	{
		switch (value)
		{
		case HP_DELTA::NORMAL:break;
		case HP_DELTA::CRITICAL:break;
		case HP_DELTA::COUNTER_ATTACK:break;
		case HP_DELTA::CRITICAL_COUNTER_ATTACK:break;
		case HP_DELTA::RECOVER:break;
		case HP_DELTA::MISS:break;
		case HP_DELTA::ADDITIVE:break;
		case HP_DELTA::BEEN_ATTACK:break;
		case HP_DELTA::INVINCIBLE:break;
		case HP_DELTA::COUNTER_INJURY:break;
		case HP_DELTA::KILL_PERCENT:break;
		case HP_DELTA::SUCK_BLOOD:break;
		case HP_DELTA::BE_MISS:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const INCREASE_CRIME_REASON value)
	{
		switch (value)
		{
		case INCREASE_CRIME_REASON::NONE:break;
		case INCREASE_CRIME_REASON::KILL_PLAYER:break;
		case INCREASE_CRIME_REASON::HIT_NO_CRIME_PLAYER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const INVINCIBLE_TYPE value)
	{
		switch (value)
		{
		case INVINCIBLE_TYPE::NORMAL:break;
		case INVINCIBLE_TYPE::PHYSICS:break;
		case INVINCIBLE_TYPE::MAGIC:break;
		case INVINCIBLE_TYPE::SORCERY:break;
		case INVINCIBLE_TYPE::CRITICAL:break;
		case INVINCIBLE_TYPE::COUNTER_ATTACK:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ITEM_BOX_TYPE value)
	{
		switch (value)
		{
		case ITEM_BOX_TYPE::NONE:break;
		case ITEM_BOX_TYPE::FIXED:break;
		case ITEM_BOX_TYPE::RANDOM:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ITEM_FUNCTION value)
	{
		switch (value)
		{
		case ITEM_FUNCTION::NONE:break;
		case ITEM_FUNCTION::MEDICINE:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const ITEM_QUALITY value)
	{
		switch (value)
		{
		case ITEM_QUALITY::NONE:break;
		case ITEM_QUALITY::NORMAL:break;
		case ITEM_QUALITY::HIGH:break;
		case ITEM_QUALITY::PRECIOUS:break;
		case ITEM_QUALITY::BEST:break;
		case ITEM_QUALITY::LEGEND:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const LEAVE_GUILD_REASON value)
	{
		switch (value)
		{
		case LEAVE_GUILD_REASON::KICK_OUT:break;
		case LEAVE_GUILD_REASON::SELF_QUIT:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const LEAVE_TEAM_REASON value)
	{
		switch (value)
		{
		case LEAVE_TEAM_REASON::KICK_OUT:break;
		case LEAVE_TEAM_REASON::SELF_QUIT:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const LOGIN_RESULT value)
	{
		switch (value)
		{
		case LOGIN_RESULT::SUCCESS:break;
		case LOGIN_RESULT::PASSWORD_ERROR:break;
		case LOGIN_RESULT::OTHER_LOGINED:break;
		case LOGIN_RESULT::FREEZED:break;
		case LOGIN_RESULT::LOGIN_FULLED:break;
		case LOGIN_RESULT::INVALID_INPUT:break;
		case LOGIN_RESULT::IP_HAS_LOGIN:break;
		case LOGIN_RESULT::IP_FULLED:break;
		case LOGIN_RESULT::OPERATE_TOO_OFTEN:break;
		case LOGIN_RESULT::IP_FORBIDDEN:break;
		case LOGIN_RESULT::IN_CLEAR_SEASON:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const LOGIN_TIP value)
	{
		switch (value)
		{
		case LOGIN_TIP::NONE:break;
		case LOGIN_TIP::FRIEND:break;
		case LOGIN_TIP::SHABAK_LORD:break;
		case LOGIN_TIP::GUILD_MEMBER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MAIL_TYPE value)
	{
		switch (value)
		{
		case MAIL_TYPE::NONE:break;
		case MAIL_TYPE::PLAYER:break;
		case MAIL_TYPE::SYSTEM:break;
		case MAIL_TYPE::AUCTION:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MESSAGE_DISPLAY value)
	{
		switch (value)
		{
		case MESSAGE_DISPLAY::NONE:break;
		case MESSAGE_DISPLAY::CHAT:break;
		case MESSAGE_DISPLAY::TIP:break;
		case MESSAGE_DISPLAY::IMPORTANT_TIP:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MISSION_ACCEPT value)
	{
		switch (value)
		{
		case MISSION_ACCEPT::NONE:break;
		case MISSION_ACCEPT::AUTO:break;
		case MISSION_ACCEPT::NPC:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MISSION_TYPE value)
	{
		switch (value)
		{
		case MISSION_TYPE::NONE:break;
		case MISSION_TYPE::MAIN:break;
		case MISSION_TYPE::BRANCH:break;
		case MISSION_TYPE::ADVENTURE:break;
		case MISSION_TYPE::DAILY:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONEY_TYPE value)
	{
		switch (value)
		{
		case MONEY_TYPE::NONE:break;
		case MONEY_TYPE::GOLD:break;
		case MONEY_TYPE::GOLD_INGOT:break;
		case MONEY_TYPE::PK_GOLD:break;
		case MONEY_TYPE::CONTRIBUTION:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_DECISION_TREE value)
	{
		switch (value)
		{
		case MONSTER_DECISION_TREE::NONE:break;
		case MONSTER_DECISION_TREE::DEFAULT:break;
		case MONSTER_DECISION_TREE::DA_DAO:break;
		case MONSTER_DECISION_TREE::GONG_JIAN_SHOU:break;
		case MONSTER_DECISION_TREE::SHI_REN_HUA:break;
		case MONSTER_DECISION_TREE::XIE_ZI:break;
		case MONSTER_DECISION_TREE::HUAN_YING_ZHI_ZHU:break;
		case MONSTER_DECISION_TREE::BAO_LIE_ZHI_ZHU:break;
		case MONSTER_DECISION_TREE::PA_XING_JIANG_SHI:break;
		case MONSTER_DECISION_TREE::ZU_MA_DIAO_XIANG:break;
		case MONSTER_DECISION_TREE::SHEN_SHOU:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_DEFAULT_STATE value)
	{
		switch (value)
		{
		case MONSTER_DEFAULT_STATE::NONE:break;
		case MONSTER_DEFAULT_STATE::STAND:break;
		case MONSTER_DEFAULT_STATE::UNBORN:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_DUTY value)
	{
		switch (value)
		{
		case MONSTER_DUTY::NONE:break;
		case MONSTER_DUTY::ATTACK:break;
		case MONSTER_DUTY::GUARD:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_GENERATE value)
	{
		switch (value)
		{
		case MONSTER_GENERATE::NONE:break;
		case MONSTER_GENERATE::FILL:break;
		case MONSTER_GENERATE::CYCLE:break;
		case MONSTER_GENERATE::WU_JIN_JIAN_TA:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_RACE value)
	{
		switch (value)
		{
		case MONSTER_RACE::NONE:break;
		case MONSTER_RACE::UNDEAD:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_SEARCH value)
	{
		switch (value)
		{
		case MONSTER_SEARCH::NONE:break;
		case MONSTER_SEARCH::NEAREST:break;
		case MONSTER_SEARCH::HIT_SELF_FIRST_RED_NAME:break;
		case MONSTER_SEARCH::RED_NAME:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_SERIES value)
	{
		switch (value)
		{
		case MONSTER_SERIES::NONE:break;
		case MONSTER_SERIES::KULOU:break;
		case MONSTER_SERIES::WOMA:break;
		case MONSTER_SERIES::ZHUDONG:break;
		case MONSTER_SERIES::ZUMA:break;
		case MONSTER_SERIES::FENGMO:break;
		case MONSTER_SERIES::SHIMO:break;
		case MONSTER_SERIES::GUMO:break;
		case MONSTER_SERIES::CHIYUE:break;
		case MONSTER_SERIES::NIUMO:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_STRENGTH value)
	{
		switch (value)
		{
		case MONSTER_STRENGTH::NORMAL:break;
		case MONSTER_STRENGTH::ELITE:break;
		case MONSTER_STRENGTH::BOSS:break;
		case MONSTER_STRENGTH::SPECIAL:break;
		case MONSTER_STRENGTH::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const MONSTER_WAR_TYPE value)
	{
		switch (value)
		{
		case MONSTER_WAR_TYPE::NONE:break;
		case MONSTER_WAR_TYPE::NORMAL:break;
		case MONSTER_WAR_TYPE::HIGH:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const OBJECT_ITEM value)
	{
		switch (value)
		{
		case OBJECT_ITEM::NONE:break;
		case OBJECT_ITEM::BOX:break;
		case OBJECT_ITEM::CONSUMABLE:break;
		case OBJECT_ITEM::EQUIP:break;
		case OBJECT_ITEM::SKILL_BOOK:break;
		case OBJECT_ITEM::MATERIAL:break;
		case OBJECT_ITEM::CARD:break;
		case OBJECT_ITEM::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PK_ARENA value)
	{
		switch (value)
		{
		case PK_ARENA::NONE:break;
		case PK_ARENA::SINGLE:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PK_ARENA_SINGLE_STAGE value)
	{
		switch (value)
		{
		case PK_ARENA_SINGLE_STAGE::NONE:break;
		case PK_ARENA_SINGLE_STAGE::ENTERING:break;
		case PK_ARENA_SINGLE_STAGE::COUNTING:break;
		case PK_ARENA_SINGLE_STAGE::FIGHTING:break;
		case PK_ARENA_SINGLE_STAGE::ENDING:break;
		case PK_ARENA_SINGLE_STAGE::SETTLEMENT:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PK_MATCH_RESULT value)
	{
		switch (value)
		{
		case PK_MATCH_RESULT::NONE:break;
		case PK_MATCH_RESULT::SUCCESS:break;
		case PK_MATCH_RESULT::TIMEOUT:break;
		case PK_MATCH_RESULT::CANCEL:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PK_MODE value)
	{
		switch (value)
		{
		case PK_MODE::PEACE:break;
		case PK_MODE::TEAM:break;
		case PK_MODE::GUILD:break;
		case PK_MODE::KILL_EVIL:break;
		case PK_MODE::KILL_ALL:break;
		case PK_MODE::OFFER_REWARD:break;
		case PK_MODE::BLACK_LIST:break;
		case PK_MODE::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PLAYER_JOB value)
	{
		switch (value)
		{
		case PLAYER_JOB::NONE:break;
		case PLAYER_JOB::FIGHTER:break;
		case PLAYER_JOB::MAGE:break;
		case PLAYER_JOB::TAOIST:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PLAYER_MOVE_STATE value)
	{
		switch (value)
		{
		case PLAYER_MOVE_STATE::NONE:break;
		case PLAYER_MOVE_STATE::STAND:break;
		case PLAYER_MOVE_STATE::WALK:break;
		case PLAYER_MOVE_STATE::RUN:break;
		case PLAYER_MOVE_STATE::DIE:break;
		case PLAYER_MOVE_STATE::DIG:break;
		case PLAYER_MOVE_STATE::SEARCH:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PLAYER_SEARCH_TARGET value)
	{
		switch (value)
		{
		case PLAYER_SEARCH_TARGET::NEAREST:break;
		case PLAYER_SEARCH_TARGET::SELF:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PLAYER_SEX value)
	{
		switch (value)
		{
		case PLAYER_SEX::NO_SEX:break;
		case PLAYER_SEX::MALE:break;
		case PLAYER_SEX::FEMALE:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PLAYER_SKILL value)
	{
		switch (value)
		{
		case PLAYER_SKILL::NONE:break;
		case PLAYER_SKILL::BASE_ATTACK:break;
		case PLAYER_SKILL::GONG_SHA_JIAN_SHU:break;
		case PLAYER_SKILL::JI_BEN_JIAN_SHU:break;
		case PLAYER_SKILL::CI_SHA_JIAN_SHU:break;
		case PLAYER_SKILL::LIE_HUO_JIAN_FA:break;
		case PLAYER_SKILL::YE_MAN_CHONG_ZHUANG:break;
		case PLAYER_SKILL::KANG_JU_HUO_HUAN:break;
		case PLAYER_SKILL::LEI_DIAN_SHU:break;
		case PLAYER_SKILL::YOU_HUO_ZHI_GUANG:break;
		case PLAYER_SKILL::SHUN_XI_YI_DONG:break;
		case PLAYER_SKILL::HUO_QIANG:break;
		case PLAYER_SKILL::BAO_LIE_HUO_YAN:break;
		case PLAYER_SKILL::DI_YU_LEI_GUANG:break;
		case PLAYER_SKILL::MO_FA_DUN:break;
		case PLAYER_SKILL::BING_PAO_XIAO:break;
		case PLAYER_SKILL::ZHI_YU_SHU:break;
		case PLAYER_SKILL::JING_SHEN_LI_ZHAN_FA:break;
		case PLAYER_SKILL::SHI_DU_SHU:break;
		case PLAYER_SKILL::LING_HUN_HUO_FU:break;
		case PLAYER_SKILL::FANG_YU_SHU:break;
		case PLAYER_SKILL::KUN_MO_ZHOU:break;
		case PLAYER_SKILL::ZHAO_HUAN_KU_LOU:break;
		case PLAYER_SKILL::ZHAO_HUAN_SHEN_SHOU:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const PROPERTY_VALUE_TYPE value)
	{
		switch (value)
		{
		case PROPERTY_VALUE_TYPE::INT:break;
		case PROPERTY_VALUE_TYPE::FLOAT:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const QQ_BIND_VERIFY_RESULT value)
	{
		switch (value)
		{
		case QQ_BIND_VERIFY_RESULT::NONE:break;
		case QQ_BIND_VERIFY_RESULT::SUCCESS:break;
		case QQ_BIND_VERIFY_RESULT::FAILED:break;
		case QQ_BIND_VERIFY_RESULT::EXPIRED:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const RANK_TYPE value)
	{
		switch (value)
		{
		case RANK_TYPE::LEVEL:break;
		case RANK_TYPE::KILL_MONSTER:break;
		case RANK_TYPE::KILL_ELITE:break;
		case RANK_TYPE::KILL_BOSS:break;
		case RANK_TYPE::KILL_SPECIAL:break;
		case RANK_TYPE::KILL_PLAYER:break;
		case RANK_TYPE::PICK_GOLD:break;
		case RANK_TYPE::PICK_SPECIAL_EQUIP:break;
		case RANK_TYPE::STRENGTH_VALUE:break;
		case RANK_TYPE::HELL_COUNT:break;
		case RANK_TYPE::PK_SCORE:break;
		case RANK_TYPE::WU_JIN_JIAN_TA:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const RED_ENVELOPE_TYPE value)
	{
		switch (value)
		{
		case RED_ENVELOPE_TYPE::NONE:break;
		case RED_ENVELOPE_TYPE::WORLD:break;
		case RED_ENVELOPE_TYPE::GUILD:break;
		case RED_ENVELOPE_TYPE::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const REGISTE_RESULT value)
	{
		switch (value)
		{
		case REGISTE_RESULT::SUCCESS:break;
		case REGISTE_RESULT::EXIST:break;
		case REGISTE_RESULT::INVALID_ACCOUNT_PASSWORD:break;
		case REGISTE_RESULT::INVALID_PHONE:break;
		case REGISTE_RESULT::INVALID_VERIFY:break;
		case REGISTE_RESULT::REQUESTED:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const RELATIONSHIP_TYPE value)
	{
		switch (value)
		{
		case RELATIONSHIP_TYPE::NOT_EXIST:break;
		case RELATIONSHIP_TYPE::ONE_ACCOUNT:break;
		case RELATIONSHIP_TYPE::HUSBAND:break;
		case RELATIONSHIP_TYPE::WIFE:break;
		case RELATIONSHIP_TYPE::MASTER:break;
		case RELATIONSHIP_TYPE::APPRENTICE:break;
		case RELATIONSHIP_TYPE::FRIEND:break;
		case RELATIONSHIP_TYPE::GUILD_MEMBER:break;
		case RELATIONSHIP_TYPE::STRANGER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const REQUEST_PLAYER_PURPOSE value)
	{
		switch (value)
		{
		case REQUEST_PLAYER_PURPOSE::NONE:break;
		case REQUEST_PLAYER_PURPOSE::SCENE_PLAYER:break;
		case REQUEST_PLAYER_PURPOSE::DATA_VIEW:break;
		case REQUEST_PLAYER_PURPOSE::CONTEXT_MENU:break;
		case REQUEST_PLAYER_PURPOSE::TEAM_VIEW:break;
		case REQUEST_PLAYER_PURPOSE::PK_ARENA:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SCENE_COPY_TYPE value)
	{
		switch (value)
		{
		case SCENE_COPY_TYPE::NONE:break;
		case SCENE_COPY_TYPE::HELL:break;
		case SCENE_COPY_TYPE::BU_GUI_LU:break;
		case SCENE_COPY_TYPE::WU_JIN_JIAN_TA:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SCENE_TRIGGER value)
	{
		switch (value)
		{
		case SCENE_TRIGGER::NONE:break;
		case SCENE_TRIGGER::TRANS_MAP:break;
		case SCENE_TRIGGER::SUMMON_MONSTER:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SCENE_TYPE value)
	{
		switch (value)
		{
		case SCENE_TYPE::NONE:break;
		case SCENE_TYPE::WORLD:break;
		case SCENE_TYPE::SCENE_COPY:break;
		case SCENE_TYPE::ARENA:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SECONDARY_PASSWORD_STATE value)
	{
		switch (value)
		{
		case SECONDARY_PASSWORD_STATE::NO_PASSWORD:break;
		case SECONDARY_PASSWORD_STATE::NOT_CONFIRM:break;
		case SECONDARY_PASSWORD_STATE::CONFIRMED:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SHABAKE_BATTLE_STAGE value)
	{
		switch (value)
		{
		case SHABAKE_BATTLE_STAGE::NONE:break;
		case SHABAKE_BATTLE_STAGE::ENROLL:break;
		case SHABAKE_BATTLE_STAGE::FIGHTING_0:break;
		case SHABAKE_BATTLE_STAGE::FIGHTING_1:break;
		case SHABAKE_BATTLE_STAGE::FIGHTING_2:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const START_TARGET_POINT value)
	{
		switch (value)
		{
		case START_TARGET_POINT::NONE:break;
		case START_TARGET_POINT::SELF_BODY:break;
		case START_TARGET_POINT::SELF_FOOT:break;
		case START_TARGET_POINT::TARGET_BODY:break;
		case START_TARGET_POINT::TARGET_FOOT:break;
		case START_TARGET_POINT::SELF_BODY_TO_TARGET_BODY:break;
		case START_TARGET_POINT::SELF_BODY_TO_TARGET_FOOT:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const STRENGTH_RESULT value)
	{
		switch (value)
		{
		case STRENGTH_RESULT::SUPER_SUCCESS:break;
		case STRENGTH_RESULT::SUCCESS:break;
		case STRENGTH_RESULT::BROKEN:break;
		case STRENGTH_RESULT::DOWN_LEVEL:break;
		case STRENGTH_RESULT::NOTHING:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SUMMON_CONTROL value)
	{
		switch (value)
		{
		case SUMMON_CONTROL::IDLE:break;
		case SUMMON_CONTROL::ATTACK:break;
		case SUMMON_CONTROL::BACK:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const SUPER_PERMISSION value)
	{
		switch (value)
		{
		case SUPER_PERMISSION::NO:break;
		case SUPER_PERMISSION::GM:break;
		case SUPER_PERMISSION::NORMAL_ADMIN:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TERRITORY_ACTIVITY_STAGE value)
	{
		switch (value)
		{
		case TERRITORY_ACTIVITY_STAGE::NONE:break;
		case TERRITORY_ACTIVITY_STAGE::SIGN_UP:break;
		case TERRITORY_ACTIVITY_STAGE::PREPARE:break;
		case TERRITORY_ACTIVITY_STAGE::ACTIVIYING:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TERRITORY_MAP value)
	{
		switch (value)
		{
		case TERRITORY_MAP::NONE:break;
		case TERRITORY_MAP::WOMA:break;
		case TERRITORY_MAP::SHIMU:break;
		case TERRITORY_MAP::ZUMA:break;
		case TERRITORY_MAP::SHIMO:break;
		case TERRITORY_MAP::GUMO:break;
		case TERRITORY_MAP::NIUMO:break;
		case TERRITORY_MAP::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TILE_TRIANGLE value)
	{
		switch (value)
		{
		case TILE_TRIANGLE::LEFT_TOP:break;
		case TILE_TRIANGLE::RIGHT_TOP:break;
		case TILE_TRIANGLE::RIGHT_BOTTOM:break;
		case TILE_TRIANGLE::LEFT_BOTTOM:break;
		case TILE_TRIANGLE::INNER_LEFT_TOP:break;
		case TILE_TRIANGLE::INNER_RIGHT_TOP:break;
		case TILE_TRIANGLE::INNER_RIGHT_BOTTOM:break;
		case TILE_TRIANGLE::INNER_LEFT_BOTTOM:break;
		case TILE_TRIANGLE::MAX:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TRADE_TYPE value)
	{
		switch (value)
		{
		case TRADE_TYPE::NONE:break;
		case TRADE_TYPE::CAN_TRADE:break;
		case TRADE_TYPE::DONT_TRADE:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TRIGGER_BUFF_REMOVE_RULE value)
	{
		switch (value)
		{
		case TRIGGER_BUFF_REMOVE_RULE::AUTO:break;
		case TRIGGER_BUFF_REMOVE_RULE::REMOVE_ADDED_BUFF:break;
		case TRIGGER_BUFF_REMOVE_RULE::DONT_REMOVE_ADDED_BUFF:break;
		default:return false;
		}
		return true;
	}

	static constexpr bool checkEnum(const TRIGGER_SKILL_DAMAGE value)
	{
		switch (value)
		{
		case TRIGGER_SKILL_DAMAGE::NONE:break;
		case TRIGGER_SKILL_DAMAGE::DAMAGE:break;
		case TRIGGER_SKILL_DAMAGE::NON_DAMAGE:break;
		case TRIGGER_SKILL_DAMAGE::ALL_DAMAGE:break;
		default:return false;
		}
		return true;
	}
};