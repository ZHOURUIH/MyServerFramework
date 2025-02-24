#pragma once

#include "GameBase.h"
#include "ThreadLock.h"
#include "MonsterDrop.h"
#include "MapData.h"
#include "PlayerSkill.h"
#include "CharacterMonster.h"
#include "SceneMap.h"
#include "Utility.h"

namespace GameUtility
{
	extern Set<char> mValidAccount;
	extern Set<char> mValidPassword;
	bool qqBindVerifyCheck(CharacterPlayer* player);
	bool isQQBindAndVerify(CharacterPlayer* player);
	bool isValidAccount(const char* account);
	bool isValidPassword(const char* password);
	bool isValid2ndPassword(const char* password);
	bool isValidPlayerName(const string& name);
	void createNewCharacter(MDCharacterBase& baseData, MDCharacterAsset& assetData, llong accountGUID, const string& name, PLAYER_SEX sex, PLAYER_JOB job);
	int generateRepairCost(const ObjectItemEquip* item, float targetDura, bool specialRepair, const CharacterPlayer* player);
	bool isInTerritoryActivity(CharacterPlayer* attacker);
	bool isInShabakeBattle(CharacterPlayer* attacker);
	// 检查是否满足进入副本的条件,playerList是用于返回进入此副本的玩家列表,避免其他地方再次生成这样一个列表
	bool checkEnterSceneCopy(CharacterPlayer* requestPlayer, ArrayList<GD::TEAM_MEMBER, CharacterPlayer*>& playerList, int sceneCopyID, const CharacterNPC* npc, float costMultiple);
	CharacterPlayer* findFollowTarget(CharacterPlayer* follower);
	void playerFollowOther(CharacterPlayer* player, CharacterPlayer* followTarget);
	int generateAdventureMissionRandomCost(int randomCount);
	// 将奖励发放到玩家背包中,发放失败则返回错误码
	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const int rewardID, const EQUIP_CREATE createType);
	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const int rewardID0, const int rewardID1, EQUIP_CREATE createType);
	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const TDReward* reward, EQUIP_CREATE createType);
	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const TDReward* reward0, const TDReward* reward1, EQUIP_CREATE createType);
	void receiveRewardIntoMail(const llong playerGUID, const int rewardID, const string& content, EQUIP_CREATE createType);
	void receiveRewardIntoMail(const llong playerGUID, const TDReward* reward, const string& content, EQUIP_CREATE createType);
	// 将奖励通过邮件发送到玩家邮箱
	void sendItemToMail(llong playerGUID, const string& content, int gold, int goldIngot);
	void sendItemToMail(llong playerGUID, ObjectItem* item0, const string& content, int gold, int goldIngot);
	void sendItemToMail(llong playerGUID, ObjectItem* item0, ObjectItem* item1, const string& content, int gold, int goldIngot);
	template<int Length>
	void collectRewardListItems(int rewardID, ArrayList<Length, int>& objIDList, ArrayList<Length, int>& objCountList, int& gold, int& goldIngot)
	{
		HashMap<int, int> objMap;
		TDReward* data = mSQLiteReward->query(rewardID);
		// 赛季服的奖励倍率
		if (mSeasonSystem->isSeasonServer())
		{
			FOR_VECTOR(data->mObjectList)
			{
				objMap.insertOrGet(data->mObjectList[i]) += data->mCountList[i] + data->mObjectSeasonRatio;
			}
			gold += (int)(data->mGold * data->mGoldSeasonRatio);
			goldIngot += (int)(data->mGoldIngot * data->mGoldIngotSeasonRatio);
		}
		else
		{
			FOR_VECTOR(data->mObjectList)
			{
				objMap.insertOrGet(data->mObjectList[i]) += data->mCountList[i];
			}
			gold += data->mGold;
			goldIngot += data->mGoldIngot;
		}
		for (const auto& item : objMap)
		{
			objIDList.add(item.first);
			objCountList.add(item.second);
		}
	}
	template<int Length0, int Length>
	void collectRewardListItems(const ArrayList<Length0, int>& rewardIDList, ArrayList<Length, int>& objIDList, ArrayList<Length, int>& objCountList, int& gold, int& goldIngot)
	{
		HashMap<int, int> objMap;
		// 赛季服的奖励倍率
		if (mSeasonSystem->isSeasonServer())
		{
			FOR_I(rewardIDList.size())
			{
				TDReward* data = mSQLiteReward->query(rewardIDList[i]);
				FOR_VECTOR_J(data->mObjectList)
				{
					objMap.insertOrGet(data->mObjectList[j]) += data->mCountList[j] * data->mObjectSeasonRatio;
				}
				gold += (int)(data->mGold * data->mGoldSeasonRatio);
				goldIngot += (int)(data->mGoldIngot * data->mGoldIngotSeasonRatio);
			}
		}
		else
		{
			FOR_I(rewardIDList.size())
			{
				TDReward* data = mSQLiteReward->query(rewardIDList[i]);
				FOR_VECTOR_J(data->mObjectList)
				{
					objMap.insertOrGet(data->mObjectList[j]) += data->mCountList[j];
				}
				gold += data->mGold;
				goldIngot += data->mGoldIngot;
			}
		}
		for (const auto& item : objMap)
		{
			objIDList.add(item.first);
			objCountList.add(item.second);
		}
	}
	void receiveItemToBackpackOrMail(CharacterPlayer* player, int objID, int objCount, int gold, int goldIngot, const string& mailContent);
	template<int Length>
	void receiveItemToBackpackOrMail(CharacterPlayer* player, const ArrayList<Length, int>& objIDList, const ArrayList<Length, int>& objCountList, int gold, int goldIngot, const string& mailContent)
	{
		COMPlayerPack* comPack = player->getComPack();
		if (comPack->canGetObjects(objIDList, objCountList) == ERROR_CODE::SUCCESS && 
			comPack->canGetGold(gold) == ERROR_CODE::SUCCESS &&
			comPack->canGetGoldIngot(goldIngot) == ERROR_CODE::SUCCESS)
		{
			FOR_I(objIDList.size())
			{
				ObjectItem* item = CmdObjectItemManagerNewItem::execute(objIDList[i], objCountList[i]);
				CmdCharacterGetObject::execute(player, item, true);
			}
			CmdCharacterAddGold::execute(player, gold);
			CmdCharacterAddGoldIngot::execute(player, goldIngot);
		}
		else
		{
			ArrayList<16, pair<ObjectItem*, int>> itemList;
			FOR_I(objIDList.size())
			{
				ObjectItem* item = CmdObjectItemManagerNewItem::execute(objIDList[i], objCountList[i]);
				itemList.add(make_pair(item, item->getCount()));
				// 当一个邮件装不下时,需要分为多个邮件发送
				if (itemList.size() >= GD::MAIL_ITEM_COUNT)
				{
					const int maxGold = GD::GOLD_INGOT_MAX >> 1;
					int mailGold = clampMax(gold, maxGold);
					int mailGoldIngot = clampMax(goldIngot, maxGold);
					CmdMailManagerSendMail::executeSystemMail(player->getGUID(), mailGold, mailGoldIngot, mailContent, itemList);
					gold -= mailGold;
					goldIngot -= mailGoldIngot;
					itemList.clear();
				}
			}
			if (itemList.size() > 0 || gold > 0 || goldIngot > 0)
			{
				CmdMailManagerSendMail::executeSystemMail(player->getGUID(), gold, goldIngot, mailContent, itemList);
			}
		}
	}
	void initValidAccountPassword();
	template<int Length>
	void fillObjectArray(const TDReward* data, ArrayList<Length, int>& objectIDs, ArrayList<Length, int>& objectCounts, const bool includeGold)
	{
		if (data == nullptr)
		{
			return;
		}
		FOR_VECTOR(data->mObjectList)
		{
			objectIDs.add(data->mObjectList[i]);
			if (mSeasonSystem->isSeasonServer())
			{
				objectCounts.add(data->mCountList[i] * data->mObjectSeasonRatio);
			}
			else
			{
				objectCounts.add(data->mCountList[i]);
			}
		}
		// 把奖励中的金币和经验也当作物品加入到物品列表中
		if (includeGold)
		{
			// 赛季服的奖励倍率
			if (mSeasonSystem->isSeasonServer())
			{
				if (data->mGold > 0)
				{
					objectIDs.add(GD::GOLD_ID);
					objectCounts.add((int)(data->mGold * data->mGoldSeasonRatio));
				}
				if (data->mGoldIngot > 0)
				{
					objectIDs.add(GD::GOLD_INGOT_ID);
					objectCounts.add((int)(data->mGoldIngot * data->mGoldIngotSeasonRatio));
				}
				if (data->mExp > 0)
				{
					objectIDs.add(GD::EXP_ID);
					objectCounts.add((int)(data->mExp * data->mExpSeasonRatio));
				}
			}
			else
			{
				if (data->mGold > 0)
				{
					objectIDs.add(GD::GOLD_ID);
					objectCounts.add(data->mGold);
				}
				if (data->mGoldIngot > 0)
				{
					objectIDs.add(GD::GOLD_INGOT_ID);
					objectCounts.add(data->mGoldIngot);
				}
				if (data->mExp > 0)
				{
					objectIDs.add(GD::EXP_ID);
					objectCounts.add(data->mExp);
				}
			}
		}
	}
	extern Vector<Vector3> mDirectionVector8;
	extern Vector<Vector3> mDirectionVector16;
	extern Vector<float> mDirectionAngle16;
	extern ThreadLock mThreadLock;
	extern llong mMonsterIDStart;// 怪物id从10亿开始,为了与玩家角色区分开来,角色id认为不超过10亿
	extern llong mMonsterIDSeed;

	bool tradeCheck(const CharacterPlayer* player)
	{
#ifndef VIRTUAL_CLIENT_TEST
		return player == nullptr || !player->check(!player->getPlayerData()->mSecondaryPasswordVerify, ERROR_CODE::SECONDARY_PASSWORD_NOT_CONFIRMED);
#endif
		return true;
	}
	CharacterPlayer* toPlayer(ComponentOwner* character) { return static_cast<CharacterPlayer*>(character); }
	const CharacterPlayer* toPlayer(const ComponentOwner* character) { return static_cast<const CharacterPlayer*>(character); }
	CharacterMonster* toMonster(ComponentOwner* character) { return static_cast<CharacterMonster*>(character); }
	const CharacterMonster* toMonster(const ComponentOwner* character) { return static_cast<const CharacterMonster*>(character); }
	CharacterEnvironment* toEnvironment(ComponentOwner* character) { return static_cast<CharacterEnvironment*>(character); }
	const CharacterEnvironment* toEnvironment(const ComponentOwner* character) { return static_cast<const CharacterEnvironment*>(character); }
	CharacterGame* toCharacterGame(ComponentOwner* character) { return static_cast<CharacterGame*>(character); }
	const CharacterGame* toCharacterGame(const ComponentOwner* character) { return static_cast<const CharacterGame*>(character); }
	CharacterNPC* toNPC(ComponentOwner* character) { return static_cast<CharacterNPC*>(character); }
	const CharacterNPC* toNPC(const ComponentOwner* character) { return static_cast<const CharacterNPC*>(character); }
	ObjectItemEquip* toEquip(ObjectItem* item) { return static_cast<ObjectItemEquip*>(item); }
	const ObjectItemEquip* toEquip(const ObjectItem* item) { return static_cast<const ObjectItemEquip*>(item); }
	ObjectItemBox* toBox(ObjectItem* item) { return static_cast<ObjectItemBox*>(item); }
	ObjectItemConsumable* toConsumable(ObjectItem* item) { return static_cast<ObjectItemConsumable*>(item); }
	ObjectItemSkillBook* toSkillBook(ObjectItem* item) { return static_cast<ObjectItemSkillBook*>(item); }
	ObjectItemMaterial* toMaterial(ObjectItem* item) { return static_cast<ObjectItemMaterial*>(item); }
	ObjectItemCard* toCard(ObjectItem* item) { return static_cast<ObjectItemCard*>(item); }
	bool isMonsterGUID(const llong guid) { return guid > mMonsterIDStart; }
	llong generateMonsterGUID() { return ++mMonsterIDSeed; }
	constexpr OBJECT_ITEM objectIDToType(const int objectID) { return (OBJECT_ITEM)(objectID / 10000); }
	CRIME_LEVEL getCrimeLevel(float crime);
	// 获得当前等级需要多少经验升到下一等级
	llong getLevelExp(int level);
	// 获得当前等级的金币上限
	int getLevelGold(int level);
	// 获得当前等级的每日产出金币上限
	int getLevelMaxCreateGold(int level);
	float getAnimationLength(ushort animationID);
	int getEquipModelAnimation(int equipModelID, ANIMATION animation);
	void parseSkillInfo(const string& str, SkillSimpleData& info);
	template<int Length>
	void skillInfoToString(MyString<Length>& charArray, const int skillID, const int level, const int exp, const int hotKey)
	{
		charArray[0] = '\0';
		INT_STR(skillStr, skillID);
		INT_STR(levelStr, level);
		INT_STR(expStr, exp);
		INT_STR(keyStr, hotKey);
		strcat_t(charArray, "{", skillStr.str(), ",", levelStr.str(), ",", expStr.str(), ",", keyStr.str(), "}");
	}
	template<int Length>
	void playerSkillListToString(MyString<Length>& charArray, const HashMap<short, PlayerSkill*>& skillList)
	{
		MyString<32> temp;
		for (const auto& iter : skillList)
		{
			const PlayerSkill* skill = iter.second;
			skillInfoToString(temp, skill->getSkillID(), skill->getLevelOrigin(), skill->getExp(), skill->getHotKey());
			strcat_t(charArray, temp.str(), "|");
		}
	}
	string playerSkillListToString(const HashMap<short, PlayerSkill*>& skillList)
	{
		string str;
		str.reserve(((size_t)skillList.size() * 16));
		MyString<32> temp;
		for (const auto& iter : skillList)
		{
			const PlayerSkill* skill = iter.second;
			skillInfoToString(temp, skill->getSkillID(), skill->getLevelOrigin(), skill->getExp(), skill->getHotKey());
			str += temp.str();
			str += "|";
		}
		return str;
	}
	int generateLifeItem();
	void parseDropPackageFile(const string& fileName, HashMap<string, Vector<MonsterDrop*>>& monsterDropList);
	Vector<MonsterDrop*>* parseDropPackageHeader(const string& headerLine, HashMap<string, Vector<MonsterDrop*>>& monsterDropList);
	Vector<MonsterDrop*>* parseDropHeader(const string& headerLine, HashMap<int, Vector<MonsterDrop*>>& monsterDropList);
	void parseDropFile(const string& fileName, const HashMap<string, Vector<MonsterDrop*>>& dropPackageList, HashMap<int, Vector<MonsterDrop*>>& monsterDropList, Vector<MonsterDrop*>& allDropList);
	MonsterDrop* parseDropLine(const ArrayList<5, string>& elementList, const string& fileName);
	// 计算从centerTile往外扩第step圈的地砖下标列表,不判断地砖是否可行走
	template<int Length>
	void generateAroundTileList(const int centerTile, const int step, ArrayList<Length, int>& tileList, MapData* mapData)
	{
		if (step == 0)
		{
			tileList.add(centerTile);
			return;
		}
		const ushort mapHeight = mapData->mHeight;
		const ushort mapWidth = mapData->mWidth;
		const int centerX = mapData->tileIndexToTileX(centerTile);
		const int centerY = mapData->tileIndexToTileY(centerTile);
		FOR_I(step << 1)
		{
			if (tileList.size() + 4 > (int)Length)
			{
				return;
			}
			// 上
			const int curTopX = centerX - step + i;
			const int curTopY = centerY + step;
			if (curTopX >= 0 && curTopX < mapWidth && curTopY >= 0 && curTopY < mapHeight)
			{
				tileList.add(mapData->tilePosToTileIndex(curTopX, curTopY));
			}
			// 右
			const int curRightX = centerX + step;
			const int curRightY = centerY + step - i;
			if (curRightX >= 0 && curRightX < mapWidth && curRightY >= 0 && curRightY < mapHeight)
			{
				tileList.add(mapData->tilePosToTileIndex(curRightX, curRightY));
			}
			// 下
			const int curBottomX = centerX + step - i;
			const int curBottomY = centerY - step;
			if (curBottomX >= 0 && curBottomX < mapWidth && curBottomY >= 0 && curBottomY < mapHeight)
			{
				tileList.add(mapData->tilePosToTileIndex(curBottomX, curBottomY));
			}
			// 左
			const int curLeftX = centerX - step;
			const int curLeftY = centerY - step + i;
			if (curLeftX >= 0 && curLeftX < mapWidth && curLeftY >= 0 && curLeftY < mapHeight)
			{
				tileList.add(mapData->tilePosToTileIndex(curLeftX, curLeftY));
			}
		}
	}
	// 获得position上可以穿戴的装备类型
	constexpr EQUIP_TYPE getPositionEquipType(EQUIP_POSITION position);
	// 根据equipType自动在playerEquip中查找可以穿戴的位置
	EQUIP_POSITION findEquipPosition(const COMPlayerEquip* playerEquip, EQUIP_TYPE equipType);
	template<int Length>
	void generateDropPosition(const SceneMap* scene, ArrayList<Length, Vector3>& posList, const int needPosCount, const Vector3& centerPos, const int monsterID)
	{
		if (scene == nullptr)
		{
			return;
		}
		MapData* mapData = scene->getMapData();
		const int tileIndex = mapData->pixelPosToTileIndex(centerPos);
		int step = 0;
		ArrayList<128, int> tileList;
		int maxLoop = 10;
		while (posList.size() < needPosCount && --maxLoop > 0)
		{
			// 从中间往外散布
			generateAroundTileList(tileIndex, step, tileList, mapData);
			FOR_I(tileList.size())
			{
				// 如果地砖不属于阻挡区域才会放物品
				if (mapData->mTileUnreachList[tileList[i]] == 0)
				{
					constexpr int halfOffset = GD::DROP_ITEM_POS_OFFSET >> 1;
					const Vector2 randomOffset((float)(randomInt(-halfOffset, halfOffset)), (float)(randomInt(-halfOffset, halfOffset)));
					Vector3 pos = toVec3(mapData->tileIndexToPixelCenterPos(tileList[i]) + randomOffset);
					if (lengthGreater(pos - centerPos, 500))
					{
						ERROR_PROFILE(("掉落物品离中心超过500的距离,怪物:" + IToS(monsterID) +
							",物品数量:" + IToS(needPosCount) +
							",中心位置:" + V3ToS(centerPos) +
							",地砖坐标:" + V2IToS(scene->getMapData()->pixelPosToTilePos(centerPos)) +
							",距离:" + IToS((int)getLength(pos - centerPos)) +
							",地图ID:" + IToS(scene->getMapID())).c_str());
					}
					if (!posList.add(pos))
					{
						break;
					}
				}
			}
			++step;
			// 如果找不到空的地砖,则回到原点继续往外扩散
			if (tileList.size() == 0)
			{
				step = 0;
			}
		}
		// 如果实在找不到位置,可能怪物被卡在一个全是阻挡的区域
		if (posList.size() < needPosCount)
		{
			FOR_I(needPosCount - posList.size())
			{
				posList.add(centerPos);
			}
		}
	}
	void generatePlayerDieDrop(const CharacterPlayer* killer, const CharacterPlayer* diePlayer, ArrayList<GD::DIE_DROP_MAX, pair<ObjectItem*, int>>& dropItemList);
	void dropPackItem(const CharacterPlayer* diePlayer, ArrayList<GD::DIE_DROP_MAX, pair<ObjectItem*, int>>& dropItemList);
	void dropDressItem(const CharacterPlayer* diePlayer, ArrayList<GD::DIE_DROP_MAX, pair<ObjectItem*, int>>& dropItemList);
	int generateSimpleDamage(CharacterGame* attackCharacter, int attack, float attackPercent, int noDefenceAttack, float noDefenceAttackPercent,
		DAMAGE_TYPE damageType, const CharacterGame* target, bool& isCritical, bool& isCounterAttack);
	int generateDamage(const CharacterSkill* skill, const SkillBullet* bullet, const CharacterGame* target, bool& isCritical, bool& isCounterAttack);
	int generateMaxHP(int level, PLAYER_JOB job, int baseMaxHP);
	int generateMaxMP(int level, PLAYER_JOB job, int baseMaxMP);
	bool canDecreaseMonsterDamage(CharacterMonster* monster);
	// 根据已装备的物品和数据库的角色属性,计算角色的指定属性,不含buff
	constexpr int generateSimplePhysicsAttack(const PLAYER_JOB job, const int level, const int baseAttack)
	{
		if (job == PLAYER_JOB::FIGHTER)
		{
			return (int)(level * 2.0f) + baseAttack;
		}
		return baseAttack;
	}
	constexpr int generateSimpleMagicAttack(const PLAYER_JOB job, const int level, const int baseAttack)
	{
		if (job == PLAYER_JOB::MAGE)
		{
			return (int)(level * 2.0f) + baseAttack;
		}
		return baseAttack;
	}
	constexpr int generateSimpleSorceryAttack(const PLAYER_JOB job, const int level, const int baseAttack)
	{
		if (job == PLAYER_JOB::TAOIST)
		{
			return (int)(level * 2.0f) + baseAttack;
		}
		return baseAttack;
	}
	constexpr int generateSimplePhysicsDefence(const int level, const int baseDefence) { return (int)(level * 2.0f) + baseDefence; }
	constexpr int generateSimpleMagicDefence(const int level, const int baseDefence) { return (int)(level * 2.0f) + baseDefence; }
	// 装备强化到targetStrength级时,计算装备总共的强化攻击值
	int generateStrengthAttack(const int targetStrength, int equipLevel);
	int generateStrengthDefence(const int targetStrength, int equipLevel);
	int generateStrengthCostItem(const ObjectItemEquip* equip, int baseCount);
	int generateStrengthCostGold(const ObjectItemEquip* equip, int baseGold);
	constexpr HP_DELTA generateHPDeltaTypeAttacker(HP_DELTA originType, bool isCritical, bool isCounterAttack);
	constexpr HP_DELTA generateHPDeltaTypeHPChanged(HP_DELTA originType);
	void fillEquipList(ArrayList<(int)EQUIP_POSITION::MAX, ObjectItem*>& equipList, const MDCharacterEquip& mysqlData, const HashMap<llong, MDObjectItemEquip*>& equipDataList);
	int vectorToDir8(const Vector3& vec);
	Vector3 dir8ToVector(int dir);
	// source是谁给character添加的此状态,sourceBuff时如果当前buff是由一个buff添加的,则表示该源buff
	// 由于回调函数被调用的时机不确定,所以需要使用sourceAssignID来判断回调时该sourceBuff是否还有效
	CharacterBuff* characterAddBuff(int buffDetailID, float buffTime, CharacterGame* character);
	CharacterBuff* characterAddBuff(int buffDetailID, CharacterGame* character, CharacterGame* source = nullptr);
	CharacterBuff* characterAddBuff(CharacterBuffParam* param, CharacterGame* character, CharacterGame* source = nullptr);
	constexpr TILE_TRIANGLE pixelPosToTriangleIndex(const Vector2& pos);
	constexpr void getTrianglePoints(TILE_TRIANGLE pos, Vector2& point0, Vector2& point1, Vector2& point2);
	// 从curPos向direction方向移动moveLength的距离,如果目标点不可到达,则切换另外一个方向尝试移动
	Vector3 generateMonsterMove(const CharacterMonster* monster, const Vector3& curPos, byte direction, float moveLength, const SceneMap* scene);
	// 计算从curPos移动到moveDest是否需要调整位置,如果遇到阻挡则会调整位置,moveDest会输出调整以后的位置
	// moveable表示是否能移动,返回值为false表示移动目标点是否是经过调整以后的,true表示目标点没有调整,可以直接到达
	bool checkMovable(const Vector3& curPos, Vector3& moveDest, bool& movable, MapData* map);
	void generateRangeBlock(const SceneMap* scene, const Vector3& pos, float radius, ArrayList<64, int>& blockIndexList);

	// 查找角色相关
	void getPlayersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList);
	void getMonstersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList);
	void getSummonsNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList);
	void getNormalMonstersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList);
	void getNormalMonstersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList, float range, int maxCount);
	void getPlayersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList);
	void getMonstersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList);
	void getSummonsNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList);
	void getNormalMonstersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList);
	void getNormalMonstersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList, float range, int maxCount);
	// 一定范围内是否有角色
	// 为了更高的执行效率,单独为这个功能写一个函数
	bool hasPlayerOrSummonInRange(CharacterGame* character, float radius);
	bool hasPlayerOrSummonInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList);
	bool hasPlayerInRange(CharacterGame* character, float radius);
	bool hasPlayerInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList);
	bool hasMonsterInRange(CharacterGame* character, float radius);
	bool hasMonsterInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList);
	bool hasSummonInRange(CharacterGame* character, float radius);
	bool hasSummonInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList);
	bool hasNormalMonsterInRange(CharacterGame* character, float radius);
	bool hasNormalMonsterInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList);
	bool hasPlayerInRangeEx(CharacterGame* character, float radius, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasPlayerInRangeEx(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasMonsterInRangeEx(CharacterGame* character, float radius, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasMonsterInRangeEx(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasSummonInRangeEx(CharacterGame* character, float radius, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasSummonInRangeEx(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasNormalMonsterInRangeEx(CharacterGame* character, float radius, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasNormalMonsterInRangeEx(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	// 一定范围内的角色数量是否超过一定数量
	bool hasPlayerCountInRange(CharacterGame* character, float radius, int minCount);
	bool hasPlayerCountInRange(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList);
	bool hasSummonCountInRange(CharacterGame* character, float radius, int minCount);
	bool hasSummonCountInRange(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList);
	bool hasMonsterCountInRange(CharacterGame* character, float radius, int minCount);
	bool hasMonsterCountInRange(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList);
	bool hasNormalMonsterCountInRange(CharacterGame* character, float radius, int minCount);
	bool hasNormalMonsterCountInRange(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList);
	bool hasPlayerCountInRangeEx(CharacterGame* character, float radius, int minCount, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasPlayerCountInRangeEx(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasSummonCountInRangeEx(CharacterGame* character, float radius, int minCount, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasSummonCountInRangeEx(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasMonsterCountInRangeEx(CharacterGame* character, float radius, int minCount, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasMonsterCountInRangeEx(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasNormalMonsterCountInRangeEx(CharacterGame* character, float radius, int minCount, CheckCharacter check, void* userData, bool includeDead = false);
	bool hasNormalMonsterCountInRangeEx(const CharacterGame* character, float radius, int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, bool includeDead = false);
	// 获取一定范围内的角色数量
	int getPlayerCountInRange(CharacterGame* character, float radius, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getPlayerCountInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getMonsterCountInRange(CharacterGame* character, float radius, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getMonsterCountInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getSummonCountInRange(CharacterGame* character, float radius, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getSummonCountInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getNormalMonsterCountInRange(CharacterGame* character, float radius, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	int getNormalMonsterCountInRange(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, int maxCount, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	template<int Length>
	void getPlayersInRange(const CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getPlayersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}
	template<int Length>
	void getPlayersInRange(CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getPlayersInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getPlayersInRange(character, radius, characterList, blockIndexList, check, userData, includeDead);
		}
	}
	template<int Length>
	void getMonstersInRange(const CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getMonstersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}
	template<int Length>
	void getMonstersInRange(CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		if (character->isMonster())
		{
			getMonstersInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getMonstersInRange(character, radius, characterList, blockIndexList, check, userData, includeDead);
		}
	}
	template<int Length>
	void getSummonsInRange(const CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getSummonsInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}
	template<int Length>
	void getSummonsInRange(CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getSummonsInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getSummonsInRange(character, radius, characterList, blockIndexList, check, userData, includeDead);
		}
	}
	template<int Length>
	void getNormalMonstersInRange(const CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getNormalMonstersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}
	template<int Length>
	void getNormalMonstersInRange(CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		if (character->isMonster())
		{
			getNormalMonstersInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getNormalMonstersInRange(character, radius, characterList, blockIndexList, check, userData, includeDead);
		}
	}
	template<int Length>
	void getPlayerOrSummonsInRange(const CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getPlayerOrSummonsInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}
	template<int Length>
	void getPlayerOrSummonsInRange(CharacterGame* character, const float radius, ArrayList<Length, CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getPlayerOrSummonsInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getPlayerOrSummonsInRange(character, radius, characterList, blockIndexList, check, userData, includeDead);
		}
	}
	void getPlayersInRange(CharacterGame* character, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersInRange(const CharacterGame* character, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getMonstersInRange(CharacterGame* character, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getMonstersInRange(const CharacterGame* character, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getSummonsInRange(CharacterGame* character, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getSummonsInRange(const CharacterGame* character, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getNormalMonstersInRange(CharacterGame* character, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getNormalMonstersInRange(const CharacterGame* character, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersInRangeIncludeSelf(CharacterGame* character, const Vector3& pos, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersInRangeIncludeSelf(const CharacterGame* character, const Vector3& pos, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersInRange(CharacterGame* character, const Vector3& pos, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersInRange(const CharacterGame* character, const Vector3& pos, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersAndSummonsInRange(CharacterGame* character, float radius, Set<CharacterGame*>& characterList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	void getPlayersAndSummonsInRange(const CharacterGame* character, float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool clear = true, bool includeDead = false);
	CharacterGame* getNearestPlayerOrSummon(CharacterGame* character, float radius);
	CharacterGame* getNearestPlayerOrSummonEx(CharacterGame* character, float radius, CheckCharacter check, void* userData);
	CharacterGame* getNearestPlayerOrSummon(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList);
	CharacterGame* getNearestPlayerOrSummonEx(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr);
	CharacterGame* getNearestPlayer(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestPlayer(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestMonster(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestMonster(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestSummon(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestSummon(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestNormalMonster(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearestNormalMonster(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstPlayer(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstPlayer(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstMonster(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstMonster(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstSummon(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstSummon(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstNormalMonster(CharacterGame* character, float radius, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);
	CharacterGame* getNearBlockFirstNormalMonster(const CharacterGame* character, float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check = nullptr, void* userData = nullptr, bool includeDead = false);

	// 在centerPos周围寻找一个可站立的位置,newPos输入一个初始计算出的位置,如果该位置可用,则不会修改此变量
	// 如果该位置不可用,则会在以centerPos为圆心,newPos到centerPos的距离为半径的圆上寻找一个新的可用的位置,设置为newPos
	// 如果周围找不到,则会将newPos设置为centerPos
	void generateAvailablePosition(const SceneMap* scene, const Vector3& centerPos, Vector3& newPos);
	// 解析装备类型掩码
	template<int Length>
	void parseEquipTypeList(ArrayList<Length, EQUIP_TYPE>& equipTypeList, const ushort equipTypeMask)
	{
		FOR_I((int)EQUIP_TYPE::MAX)
		{
			if (hasBit(equipTypeMask, i))
			{
				equipTypeList.add((EQUIP_TYPE)i);
			}
		}
	}
	// 计算一定范围内所包含的所有区块的列表,返回值是列表的长度
	template<int Length>
	void getBlocksInRange(const CharacterGame* character, ArrayList<Length, SceneBlock*>& blockList, const float range, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		const SceneBlock* sceneBlockList = scene->getBlockList();
		FOR_I(blockIndexList.size())
		{
			if (!blockList.add((SceneBlock*)&(sceneBlockList[blockIndexList[i]])))
			{
				ERROR("缓冲区太小");
				return;
			}
		}
	}
	// 计算一定范围内所包含的所有区块的列表,返回值是列表的长度
	template<int Length>
	void getBlocksInRange(CharacterGame* character, ArrayList<Length, SceneBlock*>& blockList, const float range, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		const SceneBlock* sceneBlockList = scene->getBlockList();
		FOR_I(blockIndexList.size())
		{
			if (!blockList.add((SceneBlock*)&(sceneBlockList[blockIndexList[i]])))
			{
				ERROR("缓冲区太小");
				return;
			}
		}
	}
	// 如果character是玩家,则直接返回此玩家,如果是玩家的召唤兽则返回召唤兽所属的玩家,否则返回空
	CharacterPlayer* getRelatedPlayer(CharacterGame* character);
	void removeEnterSceneCopyCost(CharacterPlayer* player, int sceneCopyID, float costMultiple);
	CharacterPlayer* getPlayer(const llong playerGUID);
	CharacterGame* getCharacterGame(const llong characterGUID);
	void sendPacket(PacketTCP* packet, const CharacterPlayer* player0, const CharacterPlayer* player1, const CharacterPlayer* player2);
	void sendPacket(PacketTCP* packet, const CharacterPlayer* player0, const CharacterPlayer* player1);
	void sendPacket(PacketTCP* packet, const CharacterPlayer* player);
	void sendPacket(ushort packetType, const string& name, const CharacterPlayer* player);
	// 如果消息没有任何参数,则推荐使用此函数发送消息,避免不必要的逻辑执行
	template<typename T,
		typename TypeCheck0 = typename IsSubClassOf<PacketTCP, T>::mType,
		typename TypeCheck1 = enable_if_t<!T::hasMember()>>
		void sendPacket(const CharacterPlayer* player)
	{
		if (player == nullptr)
		{
			return;
		}
		sendPacketTypeTCP(T::getStaticType(), T::getStaticPacketName(), player->getClient());
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	void initAngle();
	void getEquipObject(ArrayList<(int)EQUIP_POSITION::MAX, ObjectItem*>& equipList, llong instanceID, const HashMap<llong, MDObjectItemEquip*>& equipDataList);
};

using namespace GameUtility;