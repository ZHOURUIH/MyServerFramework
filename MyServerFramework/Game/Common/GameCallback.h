#pragma once

#include "GameClassDeclare.h"
#include "GameDefine.h"

// 此头文件不能被GameDefine.h包含,所以只能在需要的地方手动包含
// 下面两种定义方式没啥区别,用起来一样的,只不过第一种不支持传递lambda表达式,占的内存少一点
typedef void (*MySQLCreateCharacterCallback)(bool result, int clientGUID);
typedef void (*MySQLDeleteCharacterCallback)(bool result, int clientGUID);
typedef bool (*CheckCharacter)(CharacterGame* character, void* userData);
typedef void (*AddStateCallback)(CharacterState* state, void* userData);
typedef void (*RemoveStateCallback)(CharacterState* state, void* userData);
typedef void (*AddBuffCallback)(bool result, CharacterBuffTrigger* buffTrigger, llong buffTriggerAssignID);
typedef void (*CmdParseFunction)(const ArrayList<4, string>& params, CharacterPlayer* player);
typedef bool (*CanBeHitCallback)(const CharacterGame* attacker, const CharacterSkill* skill, const SkillBullet* bullet, void* userData);
typedef void (*PlayerRangeCallback)(CharacterGame* character, void* userData);
typedef void (*BuffTriggerCallback)(CharacterGame* character, CharacterBuffTrigger* buffTrigger);
typedef bool (*BuffTriggerCheck)(CharacterGame* character, const CharacterBuffTrigger* buffTrigger);
typedef void (*ConditionCallback)(Condition* condition, void* userData);
typedef void (*MySQLRegisterAccountCallback)(REGISTE_RESULT result, int clientGUID);

typedef function<void(MySQLData* data, int objectID)> QueryItemDataCallback;
typedef function<void(const Vector<pair<MySQLDataScope, int>>& dataList)> QueryItemDataListCallback;
typedef function<void(ParamOutQueryCharacterFull* fullData)> QueryCharacterFullCallback;
typedef function<void(bool result)> DeleteCharacterCallback;
typedef function<void(CharacterNPC* npc)> OnCreateNPC;
typedef function<void(CharacterMonster* monster)> OnCreateMonster;
typedef function<void(CharacterEnvironment* environment)> OnCreateEnvironment;
typedef function<void(ObjectItemEquip* equip)> OnEquipEvent;
typedef function<void(CharacterPlayer* player)> OnPlayerEvent;
typedef function<void(CharacterPlayer* player, CharacterGame* attacker, CharacterPlayer* attackerPlayer, int deltaHP)> OnPlayerHPEvent;
typedef function<void(CharacterMonster* monster, CharacterGame* attacker, CharacterPlayer* attackerPlayer, int deltaHP)> OnMonsterHPEvent;
typedef function<bool(CharacterPlayer* player)> OnPlayerBoolEvent;
typedef function<void(CharacterPlayer* player, int& gold)> OnPickGoldEvent;
typedef function<void(CharacterPlayer* player, ObjectItem* item)> OnPickItemEvent;
typedef function<void(CharacterPlayer* killerPlayer, CharacterPlayer* diePlayer, CharacterGame* killer)> OnPlayerKillEvent;
typedef function<void(CharacterPlayer* killerPlayer, CharacterMonster* dieMonster, CharacterGame* killer)> OnKillMonsterEvent;
typedef function<void(CharacterNPC* npc)> OnNPCEvent;
typedef function<void(const HashMap<llong, CharacterNPC*>& npcList, const CharacterPlayer* player)> OnNPCListEvent;
typedef function<void(CharacterPlayer* player, bool resetHPMP, bool backToDefaultMap)> OnPlayerDieProcessEvent;
typedef function<void(CharacterGame* beHit, CharacterGame* attacker, CharacterPlayer* attackerPlayer, int& deltaHP)> OnBeenHitEvent;
typedef function<void(const HashMap<llong, CharacterPlayer*>& playerList, const CharacterPlayer* player)> OnOtherPlayerListEvent;
typedef function<void(const CharacterPlayer* player, ObjectItem* item)> OnPlayerObjectItemEvent;
typedef function<bool(const CharacterPlayer* player, ObjectItemEquip* equip, EQUIP_TYPE equipType, EQUIP_POSITION equipPosition)> OnPlayerReplaceEquipEvent;
typedef function<void(const CharacterPlayer* player, EQUIP_POSITION position)> OnPlayerEquipChangeEvent;
typedef function<bool(const CharacterPlayer* player, EQUIP_POSITION position)> OnPlayerEquipTakeOffEvent;
typedef function<bool(const CharacterPlayer* player)> OnTradableCheck;
typedef function<void(llong, int)> OnWriteMySQLInt;
typedef function<void(llong, float)> OnWriteMySQLFloat;
typedef function<void(llong, string&&)> OnWriteMySQLString;
typedef function<void(llong, llong)> OnWriteMySQLLLong;
typedef function<void(llong, EQUIP_POSITION, llong)> OnWriteMySQLEquipPosition;
typedef function<void(ObjectItem*)> ObjectItemFunction;
typedef function<void(ObjectItemEquip*)> ObjectItemEquipFunction;
typedef function<void(ObjectItemConsumable*)> ObjectItemConsumableFunction;
typedef function<void(llong, OBJECT_ITEM)> OnDeleteObjectItem;
typedef function<llong()> OnGenerateGuid;
typedef function<void(CharacterGame* character, CharacterBuffTrigger* buffTrigger, CharacterBuffParam* param)> BuffTriggerWillAddBuff;
typedef function<void(const string& param)> BuffParamCallback;
typedef function<void(CHANGE_NAME_RESULT result)> ChangeNameCallback;
typedef function<void(ParamOutSearchAuction* auctionData)> QueryAuctionCallback;
typedef function<void(ParamOutQueryOfferRewardList* data)> QueryOfferRewardListCallback;
typedef function<void(ParamOutQueryCharacterList* data)> QueryAccountCharacterListCallback;
typedef function<void(ParamOutQueryCharacterBaseAssetList* data)> QueryCharacterBaseAssetListCallback;
typedef function<void(ParamOutQueryBaseAssetOther* data)> QueryOtherBaseAssetCallback;

template<typename T>
using QueryRankDataCallbackT = function<void(const Vector<T*>& dataList, const HashMap<llong, MDCharacterBase*>& baseList, const HashMap<llong, MDCharacterAsset*>& assetList)>;