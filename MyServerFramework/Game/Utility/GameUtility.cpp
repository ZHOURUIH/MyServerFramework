#include "GameHeader.h"

namespace GameUtility
{
	Set<char> mValidAccount;
	Set<char> mValidPassword;
	
	bool qqBindVerifyCheck(CharacterPlayer* player)
	{
		return !player->check(!isQQBindAndVerify(player), ERROR_CODE::QQ_NOT_BIND);
	}

	inline bool isQQBindAndVerify(CharacterPlayer* player)
	{
		COMPlayerGamePlay* com = player->getComGamePlay();
		return com->isQQBind() && com->isQQVerify();
	}

	bool isValidAccount(const char* account)
	{
		if (mValidAccount.size() == 0)
		{
			initValidAccountPassword();
		}
		const int length = strlength(account);
		if (length < mSQLiteGlobal->getAccountMinLength() || length >= mSQLiteGlobal->getAccountLength())
		{
			return false;
		}
		FOR_I(length)
		{
			if (!mValidAccount.contains(account[i]))
			{
				return false;
			}
		}
		return true;
	}

	bool isValidPassword(const char* password)
	{
		if (mValidPassword.size() == 0)
		{
			initValidAccountPassword();
		}
		const int length = strlength(password);
		if (length < mSQLiteGlobal->getPasswordMinLength() || length >= mSQLiteGlobal->getPasswordLength())
		{
			return false;
		}
		FOR_I(length)
		{
			if (!mValidPassword.contains(password[i]))
			{
				return false;
			}
		}
		return true;
	}

	inline bool isValid2ndPassword(const char* password)
	{
		const int length = strlength(password);
		return length == GD::SECONDARY_LENGTH && isNumber(password, length);
	}

	inline bool isValidPlayerName(const string& name)
	{
		// 名字不能有空格,也不能包含GM,名字不能太长
		return !hasInvisibleChar(name.c_str()) && !findString(name, "GM") && name.length() <= GD::NAME_LENGTH;
	}

	void createNewCharacter(MDCharacterBase& baseData, MDCharacterAsset& assetData, const llong accountGUID, const string& name, const PLAYER_SEX sex, const PLAYER_JOB job)
	{
		baseData.mAccountGUID = (int)accountGUID;
		baseData.mName = name;
		baseData.mLevel = 1;
		baseData.mSex = (int)sex;
		baseData.mJob = (int)job;
		baseData.mLastMapID = GD::DEFAULT_MAP_ID;
		baseData.mLastPosX = GD::DEFAULT_POSITION_X;
		baseData.mLastPosY = GD::DEFAULT_POSITION_Y;
		baseData.mLastHP = generateMaxHP(baseData.mLevel, job, 0);
		baseData.mLastMP = generateMaxMP(baseData.mLevel, job, 0);
		baseData.mPKMode = (byte)PK_MODE::PEACE;
		// 新建角色默认学习普攻
		MyString<32> tempSkillStr;
		skillInfoToString(tempSkillStr, GD::NORMAL_ATTACK_ID, 1, 0, 0);
		baseData.mSkill = tempSkillStr.str();
		baseData.mSuperPermission = (int)SUPER_PERMISSION::NO;	// 创建的角色没有任何权限,要打开权限需要在数据库中操作
		baseData.mCreateTime = getTimeSecond();
		ArrayList<GD::CHAR_PACK_GRID, llong> tempPack;
		// 创建默认获得的物品
		constexpr int defaultCount = 5;
		const int defaultObjectID[defaultCount]{ 30001, sex == PLAYER_SEX::MALE ? 30038 : 30050, 30077, 30106, 30145 };
		FOR_I(defaultCount)
		{
			ObjectItem* item = CmdObjectItemManagerNewItem::execute(defaultObjectID[i]);
			tempPack.add(item->getInstanceID());
			// 新建角色的物品需要写入数据库
			MySQLUtility::insertObjectItem(item);
		}
		assetData.mPack = LLsToS(tempPack);
		assetData.mQuickPack = "";
		assetData.mPackSize = GD::CHAR_PACK_GRID_DEFAULT;
		assetData.mWarehouseSize = GD::WAREHOUSE_SIZE_DEFAULT;
		assetData.mWarehouseMaxGold = GD::WAREHOUSE_MAX_GOLD;
		// 创建角色时随机选择一个物品作为角色的本命物品
		assetData.mLifeItem = generateLifeItem();
	}

	int generateRepairCost(const ObjectItemEquip* item, const float targetDura, const bool specialRepair, const CharacterPlayer* player)
	{
		const ObjectItemEquipData* equipData = item->getEquipData();
		const int level = clampMin((int)item->getLevel(), 4);
		int cost = (int)((targetDura - equipData->mDura) * 15 * (level * 0.2f) * (int)item->getQuality() * 1.2f);
		if (specialRepair)
		{
			cost = (int)(cost * mSQLiteGlobal->getSpecialRepairCost());
		}
		if (player != nullptr && getCrimeLevel(player->getPlayerData()->mCrime) >= CRIME_LEVEL::RED)
		{
			cost = (int)(cost * mSQLiteGlobal->getRedPlayerRepairCost());
		}
		return cost;
	}

	bool isInTerritoryActivity(CharacterPlayer* attacker)
	{
		if (mActivityTerritorySystem->getStage() != TERRITORY_ACTIVITY_STAGE::ACTIVIYING)
		{
			return false;
		}
		const SceneMap* scene = attacker->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		const Guild* attackerGuild = attacker != nullptr ? attacker->getComGuild()->getGuild() : nullptr;
		if (attackerGuild == nullptr)
		{
			return false;
		}
		const TERRITORY_MAP curSceneTerritoryType = scene->getSceneSQLiteData()->mTerritory;
		if (curSceneTerritoryType == TERRITORY_MAP::NONE)
		{
			return false;
		}
		return mActivityTerritorySystem->getJoinActivityTerritory(attackerGuild->getID()) == curSceneTerritoryType;
	}

	bool isInShabakeBattle(CharacterPlayer* attacker)
	{
		if (mShabakeBattleSystem->getStage() < SHABAKE_BATTLE_STAGE::FIGHTING_0 ||
			mShabakeBattleSystem->getStage() > SHABAKE_BATTLE_STAGE::FIGHTING_2)
		{
			return false;
		}
		const Guild* attackerGuild = attacker != nullptr ? attacker->getComGuild()->getGuild() : nullptr;
		if (attackerGuild == nullptr)
		{
			return false;
		}
		if (!mShabakeBattleSystem->isGuildSigned(attackerGuild))
		{
			return false;
		}
		const SceneMap* scene = attacker->getScene();
		// 只要在比奇或者沙巴克宫殿中就算是在沙巴克攻城中
		return scene != nullptr && (scene->getMapID() == GD::BI_QI_MAP || scene->getMapID() == GD::SHABAK_MAP_ID);
	}

	bool checkEnterSceneCopy(CharacterPlayer* requestPlayer, ArrayList<GD::TEAM_MEMBER, CharacterPlayer*>& playerList, const int sceneCopyID, const CharacterNPC* npc, const float costMultiple)
	{
		const TDSceneCopy* sceneCopyData = mSQLiteSceneCopy->querySceneCopy(sceneCopyID);
		if (requestPlayer->check(sceneCopyData == nullptr, ERROR_CODE::SCENE_COPY_NOT_VALID))
		{
			return false;
		}

		// 先判断副本的人数需求,再决定是否可以由此玩家请求进入
		// 副本表格的人数填写错误,则不允许进入,副本的人数上限与组队的人数上限一致,表格中填写的值不能超过这个上限
		if (sceneCopyData->mMinPlayerCount == 0 ||
			sceneCopyData->mMaxPlayerCount == 0 ||
			sceneCopyData->mMinPlayerCount > GD::TEAM_MEMBER ||
			sceneCopyData->mMaxPlayerCount > GD::TEAM_MEMBER ||
			sceneCopyData->mMinPlayerCount > sceneCopyData->mMaxPlayerCount)
		{
			SCErrorCode::send(requestPlayer, ERROR_CODE::SCENE_COPY_NOT_VALID);
			return false;
		}

		playerList.clear();
		const TeamInfo* team = requestPlayer->getComTeam()->getTeam();
		// 单人副本则不需要组队
		if (sceneCopyData->mMinPlayerCount == 1 && sceneCopyData->mMaxPlayerCount == 1)
		{
			if (requestPlayer->check(team != nullptr, ERROR_CODE::SCENE_COPY_NEED_NO_TEAM))
			{
				return false;
			}
			playerList.add(requestPlayer);
		}
		// 多人副本需要组队进入
		else
		{
			// 允许单人进入
			if (sceneCopyData->mMinPlayerCount == 1)
			{
				if (team == nullptr)
				{
					playerList.add(requestPlayer);
				}
				else
				{
					// 只能由队长请求进入
					if (requestPlayer->check(team->getLeader() != requestPlayer->getGUID(), ERROR_CODE::ONLY_TEAM_LEADER_CAN_REQUEST_ENTER))
					{
						return false;
					}
					for (const llong memberID : team->getMemberList())
					{
						playerList.addNotEqual(getPlayer(memberID), nullptr);
					}
				}
			}
			// 只能组队进入
			else
			{
				// 只能由队长请求进入
				if (requestPlayer->check(team == nullptr, ERROR_CODE::SCENE_COPY_NEED_TEAM) ||
					requestPlayer->check(team->getLeader() != requestPlayer->getGUID(), ERROR_CODE::ONLY_TEAM_LEADER_CAN_REQUEST_ENTER))
				{
					return false;
				}
				for (const llong memberID : team->getMemberList())
				{
					playerList.addNotEqual(getPlayer(memberID), nullptr);
				}
			}
		}
		const int playerCount = playerList.size();
		// 人数不能超过副本限定的范围
		if (playerCount == 0 ||
			requestPlayer->check(playerCount < sceneCopyData->mMinPlayerCount, ERROR_CODE::SCENE_COPY_PLAYER_COUNT_ERROR) ||
			requestPlayer->check(playerCount > sceneCopyData->mMaxPlayerCount, ERROR_CODE::SCENE_COPY_PLAYER_COUNT_ERROR))
		{
			return false;
		}
		FOR_I(playerCount)
		{
			const CharacterPlayer* player = playerList[i];
			// 是否在NPC的一定范围内
			if (npc != nullptr)
			{
				if (requestPlayer->check(player->getScene() != npc->getScene(), ERROR_CODE::TEAM_MEMBER_NOT_NEAR) ||
					requestPlayer->check(playerCount > 1 && lengthGreater(player->getPosition() - npc->getPosition(), 700.0f), ERROR_CODE::TEAM_MEMBER_NOT_NEAR))
				{
					return false;
				}
			}
			// 检查等级和物品是否满足
			const COMPlayerPack* playerPack = player->getComPack();
			if (requestPlayer->check(player->getPlayerData()->mLevel < sceneCopyData->mNeedLevel, ERROR_CODE::TEAM_MEMBER_LEVEL_NOT_ENOUGH) ||
				requestPlayer->check(playerPack->getGold() < (int)(sceneCopyData->mCostGold * costMultiple), ERROR_CODE::TEAM_MEMBER_GOLD_NOT_ENOUGH) ||
				requestPlayer->check(playerPack->getGoldIngot() < (int)(sceneCopyData->mCostGoldIngot * costMultiple), ERROR_CODE::TEAM_MEMBER_GOLD_INGOT_NOT_ENOUGH))
			{
				return false;
			}
			FOR_VECTOR_J(sceneCopyData->mCostItem)
			{
				if (playerPack->getItemCount(sceneCopyData->mCostItem[j]) < (int)(sceneCopyData->mCostItemCount[j] * costMultiple))
				{
					SCErrorCode::send(requestPlayer, ERROR_CODE::TEAM_MEMBER_MATERIAL_NOT_ENOUGH);
					return false;
				}
			}
		}
		return true;
	}

	CharacterPlayer* findFollowTarget(CharacterPlayer* follower)
	{
		const TeamInfo* team = follower->getComTeam()->getTeam();
		if (team == nullptr)
		{
			return nullptr;
		}
		for (const llong memberID : team->getMemberList())
		{
			CharacterPlayer* member = getPlayer(memberID);
			if (member == nullptr)
			{
				continue;
			}
			if (member->isDestroy())
			{
				ERROR_PROFILE("0玩家已经被销毁");
				continue;
			}
			if (!member->isDead())
			{
				return member;
			}
		}
		return nullptr;
	}

	void playerFollowOther(CharacterPlayer* player, CharacterPlayer* followTarget)
	{
		COMPlayerGhostFollow* comGhostFollow = player->getComGhostFollow();
		comGhostFollow->setActive(followTarget != nullptr);
		const CharacterPlayer* lastFollowTarget = comGhostFollow->getFollowTarget();
		if (lastFollowTarget != followTarget)
		{
			comGhostFollow->setFollowTarget(followTarget);
			if (lastFollowTarget != nullptr)
			{
				lastFollowTarget->getComGhostFollow()->removeFollowSelf(player);
			}
			if (followTarget != nullptr)
			{
				followTarget->getComGhostFollow()->addFollowSelf(player);
			}
		}
		SCGhostFollowPlayer::send(player, followTarget != nullptr ? followTarget->getGUID() : 0);
	}

	// 计算随机一次奇遇任务列表需要消耗的金币数量,randomCount为已经随机的次数
	int generateAdventureMissionRandomCost(int randomCount)
	{
		// 初始10万每次,每随机2次增加10万金币的消耗
		return 100000 + randomCount / 2 * 100000;
	}

	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const int rewardID, const EQUIP_CREATE createType)
	{
		const TDReward* data = rewardID > 0 ? mSQLiteReward->queryReward(rewardID) : nullptr;
		return receiveRewardIntoBackpack(player, data, createType);
	}

	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const TDReward* reward, const EQUIP_CREATE createType)
	{
		if (player == nullptr)
		{
			return ERROR_CODE::PLAYER_OFFLINE;
		}

		if (reward == nullptr)
		{
			return ERROR_CODE::SUCCESS;
		}

		ArrayList<GD::REWARD_COUNT, int> rewardIDs;
		ArrayList<GD::REWARD_COUNT, int> rewardCounts;
		fillObjectArray(reward, rewardIDs, rewardCounts, true);
		const ushort errorCode = player->getComPack()->canGetObjects(rewardIDs, rewardCounts);
		if (errorCode != ERROR_CODE::SUCCESS)
		{
			return errorCode;
		}

		// 玩家获得奖励物品
		FOR_I(rewardIDs.size())
		{
			// 创建奖励物品,将物品给玩家
			ObjectItem* item = CmdObjectItemManagerNewItem::execute(rewardIDs[i], rewardCounts[i], 0, createType);
			CmdCharacterGetObject::execute(player, item, true);
		}
		return ERROR_CODE::SUCCESS;
	}

	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const int rewardID0, const int rewardID1, const EQUIP_CREATE createType)
	{
		const TDReward* data0 = rewardID0 > 0 ? mSQLiteReward->queryReward(rewardID0) : nullptr;
		const TDReward* data1 = rewardID1 > 0 ? mSQLiteReward->queryReward(rewardID1) : nullptr;
		return receiveRewardIntoBackpack(player, data0, data1, createType);
	}

	ushort receiveRewardIntoBackpack(CharacterPlayer* player, const TDReward* reward0, const TDReward* reward1, const EQUIP_CREATE createType)
	{
		if (player == nullptr)
		{
			return ERROR_CODE::PLAYER_OFFLINE;
		}

		// 计算出最终获得的物品ID列表
		ArrayList<GD::REWARD_COUNT * 4, int> rewardIDs;
		ArrayList<GD::REWARD_COUNT * 4, int> rewardCounts;
		fillObjectArray(reward0, rewardIDs, rewardCounts, true);
		fillObjectArray(reward1, rewardIDs, rewardCounts, true);
		if (rewardIDs.size() == 0)
		{
			return ERROR_CODE::SUCCESS;
		}

		// 检查背包能否容下奖励物品
		const ushort errorCode = player->getComPack()->canGetObjects(rewardIDs, rewardCounts);
		if (errorCode != ERROR_CODE::SUCCESS)
		{
			return errorCode;
		}

		// 玩家获得奖励物品
		FOR_I(rewardIDs.size())
		{
			// 创建奖励物品,将物品给玩家
			ObjectItem* item = CmdObjectItemManagerNewItem::execute(rewardIDs[i], rewardCounts[i], 0, createType);
			CmdCharacterGetObject::execute(player, item, true);
		}
		return ERROR_CODE::SUCCESS;
	}

	void receiveRewardIntoMail(const llong playerGUID, const int rewardID, const string& content, const EQUIP_CREATE createType)
	{
		const TDReward* data = rewardID > 0 ? mSQLiteReward->queryReward(rewardID) : nullptr;
		receiveRewardIntoMail(playerGUID, data, content, createType);
	}

	void receiveRewardIntoMail(const llong playerGUID, const TDReward* reward, const string& content, const EQUIP_CREATE createType)
	{
		if (reward == nullptr)
		{
			return;
		}
		const auto& objIDList = reward->mObjectList;
		ArrayList<16, ObjectItem*> allItemList;
		ArrayList<16, pair<ObjectItem*, int>> itemPairList;
		if (mSeasonSystem->isSeasonServer())
		{
			FOR_VECTOR(objIDList)
			{
				allItemList.add(CmdObjectItemManagerNewItem::execute(objIDList[i], reward->mCountList[i] * reward->mObjectSeasonRatio));
			}
			FOR_I(allItemList.size())
			{
				itemPairList.add(make_pair(allItemList[i], allItemList[i]->getCount()));
			}
			CmdMailManagerSendMail::executeSystemMail(playerGUID, (int)(reward->mGold * reward->mGoldSeasonRatio), (int)(reward->mGoldIngot* reward->mGoldIngotSeasonRatio), content, itemPairList);
		}
		else
		{
			FOR_VECTOR(objIDList)
			{
				allItemList.add(CmdObjectItemManagerNewItem::execute(objIDList[i], reward->mCountList[i]));
			}
			FOR_I(allItemList.size())
			{
				itemPairList.add(make_pair(allItemList[i], allItemList[i]->getCount()));
			}
			CmdMailManagerSendMail::executeSystemMail(playerGUID, reward->mGold, reward->mGoldIngot, content, itemPairList);
		}
	}

	void sendItemToMail(const llong playerGUID, const string& content, const int gold, const int goldIngot)
	{
		CmdMailManagerSendMail::executeSystemMail(playerGUID, gold, goldIngot, content);
	}

	void sendItemToMail(const llong playerGUID, ObjectItem* item0, const string& content, const int gold, const int goldIngot)
	{
		if (item0 != nullptr)
		{
			ArrayList<16, pair<ObjectItem*, int>> itemList;
			itemList.add(make_pair(item0, item0->getCount()));
			CmdMailManagerSendMail::executeSystemMail(playerGUID, gold, goldIngot, content, itemList);
		}
		else
		{
			CmdMailManagerSendMail::executeSystemMail(playerGUID, gold, goldIngot, content);
		}
	}

	void sendItemToMail(const llong playerGUID, ObjectItem* item0, ObjectItem* item1, const string& content, const int gold, const int goldIngot)
	{
		ArrayList<16, pair<ObjectItem*, int>> itemList;
		if (item0 != nullptr)
		{
			itemList.add(make_pair(item0, item0->getCount()));
		}
		if (item1 != nullptr)
		{
			itemList.add(make_pair(item1, item1->getCount()));
		}
		CmdMailManagerSendMail::executeSystemMail(playerGUID, gold, goldIngot, content, itemList);
	}

	void receiveItemToBackpackOrMail(CharacterPlayer* player, const int objID, const int objCount, const int gold, const int goldIngot, const string& mailContent)
	{
		COMPlayerPack* comPack = player->getComPack();
		if (comPack->canGetObject(objID, objCount) == ERROR_CODE::SUCCESS &&
			comPack->canGetGold(gold) == ERROR_CODE::SUCCESS &&
			comPack->canGetGoldIngot(goldIngot) == ERROR_CODE::SUCCESS)
		{
			ObjectItem* item = CmdObjectItemManagerNewItem::execute(objID, objCount);
			CmdCharacterGetObject::execute(player, item, true);
			CmdCharacterAddGold::execute(player, gold);
			CmdCharacterAddGoldIngot::execute(player, goldIngot);
		}
		else
		{
			ArrayList<16, pair<ObjectItem*, int>> itemList;
			ObjectItem* item = CmdObjectItemManagerNewItem::execute(objID, objCount);
			itemList.add(make_pair(item, item->getCount()));
			CmdMailManagerSendMail::executeSystemMail(player->getGUID(), gold, goldIngot, mailContent, itemList);
		}
	}

	void initValidAccountPassword()
	{
		FOR_I('9' - '0' + 1)
		{
			mValidAccount.insert('0' + i);
			mValidPassword.insert('0' + i);
		}
		FOR_I('z' - 'a' + 1)
		{
			mValidAccount.insert('a' + i);
			mValidPassword.insert('a' + i);
		}
		FOR_I('Z' - 'A' + 1)
		{
			mValidAccount.insert('A' + i);
			mValidPassword.insert('A' + i);
		}
		mValidPassword.insert('+');
		mValidPassword.insert('-');
		mValidPassword.insert('*');
		mValidPassword.insert('/');
		mValidPassword.insert('=');
		mValidPassword.insert('_');
		mValidPassword.insert('`');
		mValidPassword.insert('!');
		mValidPassword.insert('@');
		mValidPassword.insert('#');
		mValidPassword.insert('$');
		mValidPassword.insert('%');
		mValidPassword.insert('^');
		mValidPassword.insert('&');
		mValidPassword.insert('(');
		mValidPassword.insert(')');
		mValidPassword.insert('[');
		mValidPassword.insert(']');
		mValidPassword.insert('{');
		mValidPassword.insert('}');
		mValidPassword.insert(';');
		mValidPassword.insert(':');
		mValidPassword.insert('\'');
		mValidPassword.insert('"');
		mValidPassword.insert('<');
		mValidPassword.insert('>');
		mValidPassword.insert(',');
		mValidPassword.insert('.');
		mValidPassword.insert('?');
		mValidPassword.insert('|');
	}
	Vector<Vector3> mDirectionVector8;
	Vector<Vector3> mDirectionVector16;
	Vector<float> mDirectionAngle16;
	ThreadLock mThreadLock;
	llong mMonsterIDStart = 1000000000;
	llong mMonsterIDSeed = mMonsterIDStart;
	OnTradableCheck mOnTradableCheck;

	llong getLevelExp(const int level)
	{
		if (level == 0 || level > mSQLitePlayerLevel->getMaxLevel())
		{
			return 0;
		}
		// 此处ID与等级相同
		const TDPlayerLevel* playerLevelData = mSQLitePlayerLevel->queryPlayerLevel(level);
		return playerLevelData != nullptr ? playerLevelData->mExp : 0;
	}

	int getLevelGold(const int level)
	{
		if (level == 0 || level > mSQLitePlayerLevel->getMaxLevel())
		{
			return 0;
		}
		// 此处ID与等级相同
		const TDPlayerLevel* playerLevelData = mSQLitePlayerLevel->queryPlayerLevel(level);
		return playerLevelData != nullptr ? playerLevelData->mMaxGold : 0;
	}

	int getLevelMaxCreateGold(const int level)
	{
		if (level == 0 || level > mSQLitePlayerLevel->getMaxLevel())
		{
			return 0;
		}
		// 此处ID与等级相同
		const TDPlayerLevel* playerLevelData = mSQLitePlayerLevel->queryPlayerLevel(level);
		const float goldRatio = mSeasonSystem->isSeasonServer() ? mSQLiteGlobal->getSeasonDailyGoldRatio() : 1.0f;
		return playerLevelData != nullptr ? (int)(playerLevelData->mMaxCreateGold * goldRatio) : 0;
	}

	// 获取动作长度,不考虑实时设置的播放速度,只考虑表格中动作设置的速度
	float getAnimationLength(const ushort animationID)
	{
		const TDAnimation* animationData = mSQLiteAnimation->queryAnimation(animationID);
		if (animationData == nullptr || isZero(animationData->mAnimationSpeed))
		{
			return 0.0f;
		}
		const TDImagePositionAnimation* animPosData = mSQLiteImagePositionAnimation->query(animationData->mAnimationPosition[0]);
		return animPosData->mPosX.size() * speedToInterval(animationData->mAnimationSpeed);
	}

	// 获得装备模型中指定的动作数据
	int getEquipModelAnimation(const int equipModelID, const ANIMATION animation)
	{
		if (equipModelID <= 0)
		{
			return 0;
		}
		const TDModelEquip* modelData = mSQLiteModelEquip->queryModelEquip(equipModelID);
		if (modelData == nullptr)
		{
			ERROR("找不到装备ID:" + IToS(equipModelID));
			return 0;
		}
		switch (animation)
		{
		case ANIMATION::NONE:			return 0;
		case ANIMATION::ATTACK:			return modelData->mAttackAnimation;
		case ANIMATION::SKILL:			return modelData->mSkillAnimation;
		case ANIMATION::RUN:			return modelData->mRunAnimation;
		case ANIMATION::WALK:			return modelData->mWalkAnimation;
		case ANIMATION::DIE:			return modelData->mDieAnimation;
		case ANIMATION::HIT:			return modelData->mHitAnimation;
		case ANIMATION::STAND:			return modelData->mStandAnimation;
		case ANIMATION::DIG:			return modelData->mDigAnimation;
		case ANIMATION::SEARCH:			return modelData->mSearchAnimation;
		case ANIMATION::JUMP_ATTACK:	return modelData->mJumpAttackAnimation;
		case ANIMATION::RUSH_ATTACK:	return modelData->mRushAttackAnimation;
		}
		return 0;
	}

	void parseSkillInfo(const string& str, SkillSimpleData& info)
	{
		info.mSkill = 0;
		info.mLevel = 0;
		info.mExp = 0;
		info.mKey = 0;
		if (str.length() == 0 || str == "0")
		{
			return;
		}
		ArrayList<4, string> paramList;
		if (!splitFull(str, ",", paramList))
		{
			return;
		}
		info.mSkill = SToI(paramList[0]);
		info.mLevel = SToI(paramList[1]);
		info.mExp = SToI(paramList[2]);
		info.mKey = SToI(paramList[3]);
	}

	int generateLifeItem()
	{
		auto& allItems = mSQLiteItem->queryAll();
		Vector<int> tempItemIDList(allItems.size());
		for (const auto& iterItems : allItems)
		{
			// 排除金币,经验,元宝
			if (iterItems.first == GD::GOLD_ID ||
				iterItems.first == GD::EXP_ID ||
				iterItems.first == GD::GOLD_INGOT_ID)
			{
				continue;
			}
			// 排除翅膀
			if (objectIDToType(iterItems.second->mID) == OBJECT_ITEM::EQUIP)
			{
				const TDItemEquip* sqliteEquip = mSQLiteItemEquip->queryEquip(iterItems.first);
				if (sqliteEquip == nullptr || sqliteEquip->mEquipType == EQUIP_TYPE::WING)
				{
					continue;
				}
			}
			tempItemIDList.push_back(iterItems.first);
		}
		return tempItemIDList[randomInt(0, tempItemIDList.size() - 1)];
	}

	void parseDropPackageFile(const string& fileName, HashMap<string, Vector<MonsterDrop*>>& monsterDropList)
	{
		Vector<MonsterDrop*>* curDropList = nullptr;
		Vector<string> lineList(32);
		openTxtFileLines(fileName, lineList, false);
		FOR_VECTOR(lineList)
		{
			string& line = lineList[i];
			// 将\t替换为空格,去除\r
			replaceAll(line, "\t", " ");
			removeAll(line, '\r');
			// 此包里没有物品
			if (line == "{}")
			{
				parseDropPackageHeader(lineList[i - 1], monsterDropList);
				continue;
			}
			// 开始解析一个包的信息
			if (line == "{")
			{
				curDropList = parseDropPackageHeader(lineList[i - 1], monsterDropList);
				continue;
			}
			// 包信息结束
			if (line == "}")
			{
				curDropList = nullptr;
				continue;
			}
			if (curDropList != nullptr && line.length() > 0 && (int)line.find_first_not_of(' ') != FrameDefine::NOT_FIND)
			{
				ArrayList<5, string> lineElement;
				split(line, " ", lineElement);
				MonsterDrop* dropInfo = parseDropLine(lineElement, fileName);
				if (dropInfo == nullptr)
				{
					ERROR("物品掉落参数错误:" + UTF8ToANSIAuto(line) + ", 文件名:" + fileName);
					continue;
				}
				curDropList->push_back(dropInfo);
			}
		}
	}

	void parseDropFile(const string& fileName, const HashMap<string, Vector<MonsterDrop*>>& dropPackageList, HashMap<int, Vector<MonsterDrop*>>& monsterDropList, Vector<MonsterDrop*>& allDropList)
	{
		Vector<MonsterDrop*>* curDropList = nullptr;
		Vector<string> lineList(128);
		openTxtFileLines(fileName, lineList, false);
		FOR_VECTOR(lineList)
		{
			string& line = lineList[i];
			// 将\t替换为空格,去除\r
			replaceAll(line, '\t', ' ');
			removeAll(line, '\r');
			// 怪物没有可掉落物品
			if (line == "{}")
			{
				parseDropHeader(lineList[i - 1], monsterDropList);
				continue;
			}

			// 开始解析一个怪物的掉落信息
			if (line[0] == '{')
			{
				curDropList = parseDropHeader(lineList[i - 1], monsterDropList);
				continue;
			}
			// 怪物掉落信息结束
			else if (line[0] == '}')
			{
				curDropList = nullptr;
				continue;
			}

			if (curDropList != nullptr && line.length() > 0 && (int)line.find_first_not_of(' ') != FrameDefine::NOT_FIND)
			{
				ArrayList<5, string> lineElement;
				split(line, " ", lineElement);
				// 掉落包
				if (lineElement[0] == "package")
				{
					const auto* packagePtr = dropPackageList.getPtrConst(lineElement[1]);
					if (packagePtr == nullptr)
					{
						ERROR("物品掉落参数错误,找不到对应的掉落包:" + UTF8ToANSIAuto(line) + ", 文件名:" + fileName + ", 行号:" + IToS(i));
						continue;
					}
					curDropList->addRange(*packagePtr);
				}
				// 带额外几率调整的掉落包
				else if (lineElement[1] == "package")
				{
					const auto* packagePtr = dropPackageList.getPtrConst(lineElement[2]);
					if (packagePtr == nullptr)
					{
						ERROR("物品掉落参数错误,找不到对应的掉落包:" + UTF8ToANSIAuto(line) + ", 文件名:" + fileName + ", 行号:" + IToS(i));
						continue;
					}
					ArrayList<2, string> probabilityString;
					if (!splitFull(lineElement[0], "/", probabilityString))
					{
						ERROR("掉落包引用格式错误, line:" + UTF8ToANSIAuto(line) + ", 文件名:" + fileName + ", 行号:" + IToS(i));
						continue;
					}
					// 将物品的掉落几率,因为很多珍稀道具的掉落几率较低,所以使用更大的几率范围来表示
					const float scale = divide(SToI(probabilityString[0]), SToI(probabilityString[1]));
					for (const MonsterDrop* data : *packagePtr)
					{
						MonsterDrop* newDrop = new MonsterDrop();
						*newDrop = *data;
						newDrop->mProbability = ceiling(newDrop->mProbability * scale);
						curDropList->push_back(newDrop);
						// 因为是额外创建出来的新的掉落信息对象,也需要加入列表
						allDropList.push_back(newDrop);
					}
				}
				// 正常的掉落物品
				else
				{
					MonsterDrop* dropInfo = parseDropLine(lineElement, fileName);
					if (dropInfo == nullptr)
					{
						ERROR("物品掉落参数错误:" + UTF8ToANSIAuto(line) + ", 文件名:" + fileName + ", 行号:" + IToS(i));
						continue;
					}
					curDropList->push_back(dropInfo);
					// 此处只将非掉落包中的物品掉落信息加入列表,掉落包中的已经加过了
					allDropList.push_back(dropInfo);
				}
			}
		}
	}

	Vector<MonsterDrop*>* parseDropPackageHeader(const string& headerLine, HashMap<string, Vector<MonsterDrop*>>& monsterDropList)
	{
		if (monsterDropList.contains(headerLine))
		{
			ERROR("怪物掉落包文件中名字重复:" + UTF8ToANSIAuto(headerLine));
			return nullptr;
		}
		monsterDropList.insert(headerLine);
		return &monsterDropList[headerLine];
	}

	Vector<MonsterDrop*>* parseDropHeader(const string& headerLine, HashMap<int, Vector<MonsterDrop*>>& monsterDropList)
	{
		ArrayList<2, string> headerInfoList;
		Vector<MonsterDrop*>* result = nullptr;
		FOR_ONCE
		{
			if (!splitFull(headerLine, "_", headerInfoList))
			{
				ERROR("掉落文件头解析错误: " + UTF8ToANSIAuto(headerLine));
				break;
			}
			const int curMonsterID = SToI(headerInfoList[1]);
			if (monsterDropList.contains(curMonsterID))
			{
				ERROR("怪物掉落文件ID重复,怪物ID:" + headerInfoList[1]);
				break;
			}
			TDMonster* monsterData = mSQLiteMonster->queryMonster(curMonsterID);
			if (monsterData == nullptr)
			{
				ERROR("怪物不存在,怪物ID:" + headerInfoList[1]);
				break;
			}
			// 检查名字是否与怪物实际名字相同
			if (monsterData->mName != headerInfoList[0])
			{
				ERROR("怪物掉落文件头与怪物名字不对应, 掉落文件头:" + UTF8ToANSIAuto(headerLine) + ", 怪物名:" + monsterData->mName);
				break;
			}

			monsterDropList.insert(curMonsterID);
			result = &monsterDropList[curMonsterID];
		}
		return result;
	}

	MonsterDrop* parseDropLine(const ArrayList<5, string>& elementList, const string& fileName)
	{
		if (elementList.size() < 3 || elementList.size() > 5)
		{
			return nullptr;
		}
		ArrayList<2, string> probabilityString;
		if (!splitFull(elementList[0], "/", probabilityString))
		{
			return nullptr;
		}
		MonsterDrop* drop = new MonsterDrop();
		// 将物品的掉落几率,因为很多珍稀道具的掉落几率较低,所以使用更大的几率范围来表示
		drop->mProbability = ceiling(divide((double)SToI(probabilityString[0]), (double)SToI(probabilityString[1])) * GD::MONSTER_ODDS_SCALE);
		drop->mItemID = SToI(elementList[1]);
		// 查找物品属性,判断名字是否一致
		TDItem* itemData = mSQLiteItem->queryItem(drop->mItemID);
		if (itemData == nullptr)
		{
			ERROR("掉落的物品不存在, ID:" + elementList[1]);
			return nullptr;
		}
		const string& realName = itemData->mName;
		if (elementList[2] != realName)
		{
			ERROR("掉落配置文件中的物品名字与实际物品名字不对应,name:" + UTF8ToANSIAuto(elementList[2]) + ",HEX:" + bytesToHexString((byte*)elementList[2].c_str(), (int)elementList[2].length()) +
				", 物品实际名字:" + UTF8ToANSIAuto(realName) + ",HEX:" + bytesToHexString((byte*)realName.c_str(), (int)realName.length()) + ",配置文件:" + fileName);
			delete drop;
			return nullptr;
		}
		// 有3个参数,则物品只掉落1个
		if (elementList.size() == 3)
		{
			drop->mMinCount = 1;
			drop->mMaxCount = 1;
		}
		// 有4个参数,则第四个参数为物品的掉落数量
		else if (elementList.size() == 4)
		{
			drop->mMinCount = SToI(elementList[3]);
			drop->mMaxCount = SToI(elementList[3]);
		}
		// 有5个参数,则第四,五个参数为物品的掉落数量下限和上限
		else if (elementList.size() == 5)
		{
			drop->mMinCount = SToI(elementList[3]);
			drop->mMaxCount = SToI(elementList[4]);
		}
		return drop;
	}

	constexpr EQUIP_TYPE getPositionEquipType(const EQUIP_POSITION position)
	{
		switch (position)
		{
		case EQUIP_POSITION::CLOTH: 			return EQUIP_TYPE::CLOTH;
		case EQUIP_POSITION::WEAPON: 			return EQUIP_TYPE::WEAPON;
		case EQUIP_POSITION::HELMET: 			return EQUIP_TYPE::HELMET;
		case EQUIP_POSITION::NECKLACK: 			return EQUIP_TYPE::NECKLACK;
		case EQUIP_POSITION::MEDAL: 			return EQUIP_TYPE::MEDAL;
		case EQUIP_POSITION::LEFT_BRACELET: 	return EQUIP_TYPE::BRACELET;
		case EQUIP_POSITION::RIGHT_BRACELET: 	return EQUIP_TYPE::BRACELET;
		case EQUIP_POSITION::LEFT_RING: 		return EQUIP_TYPE::RING;
		case EQUIP_POSITION::RIGHT_RING: 		return EQUIP_TYPE::RING;
		case EQUIP_POSITION::BELT: 				return EQUIP_TYPE::BELT;
		case EQUIP_POSITION::SHOE: 				return EQUIP_TYPE::SHOE;
		case EQUIP_POSITION::DIAMOND: 			return EQUIP_TYPE::DIAMOND;
		case EQUIP_POSITION::WING: 				return EQUIP_TYPE::WING;
		case EQUIP_POSITION::FASHION_CLOTH: 	return EQUIP_TYPE::FASHION_CLOTH;
		case EQUIP_POSITION::FASHION_WEAPON: 	return EQUIP_TYPE::FASHION_WEAPON;
		case EQUIP_POSITION::FASHION_WING: 		return EQUIP_TYPE::FASHION_WING;
		case EQUIP_POSITION::MAX:				return EQUIP_TYPE::MAX;
		}
		return EQUIP_TYPE::MAX;
	}

	EQUIP_POSITION findEquipPosition(const COMPlayerEquip* playerEquip, const EQUIP_TYPE equipType)
	{
		switch (equipType)
		{
		case EQUIP_TYPE::WEAPON: 		return EQUIP_POSITION::WEAPON;
		case EQUIP_TYPE::CLOTH: 		return EQUIP_POSITION::CLOTH;
		case EQUIP_TYPE::HELMET: 		return EQUIP_POSITION::HELMET;
		case EQUIP_TYPE::NECKLACK: 		return EQUIP_POSITION::NECKLACK;
		case EQUIP_TYPE::MEDAL: 		return EQUIP_POSITION::MEDAL;
		case EQUIP_TYPE::BRACELET:
		{
			// 只有当左边手镯戴了,且右边为空时才会戴右边,否则戴左边
			if (playerEquip->getLeftBracelet() != nullptr && playerEquip->getRightBracelet() == nullptr)
			{
				return EQUIP_POSITION::RIGHT_BRACELET;
			}
			else
			{
				return EQUIP_POSITION::LEFT_BRACELET;
			}
		}
		case EQUIP_TYPE::RING:
		{
			// 只有当左边戒指戴了,且右边为空时才会戴右边,否则戴左边
			if (playerEquip->getLeftRing() != nullptr && playerEquip->getRightRing() == nullptr)
			{
				return EQUIP_POSITION::RIGHT_RING;
			}
			else
			{
				return EQUIP_POSITION::LEFT_RING;
			}
		}
		case EQUIP_TYPE::BELT: 			return EQUIP_POSITION::BELT;
		case EQUIP_TYPE::SHOE: 			return EQUIP_POSITION::SHOE;
		case EQUIP_TYPE::DIAMOND: 		return EQUIP_POSITION::DIAMOND;
		case EQUIP_TYPE::WING: 			return EQUIP_POSITION::WING;
		case EQUIP_TYPE::FASHION_CLOTH: return EQUIP_POSITION::FASHION_CLOTH;
		case EQUIP_TYPE::FASHION_WEAPON:return EQUIP_POSITION::FASHION_WEAPON;
		case EQUIP_TYPE::FASHION_WING: 	return EQUIP_POSITION::FASHION_WING;
		case EQUIP_TYPE::MAX: break;
		}
		return EQUIP_POSITION::MAX;
	}

	void generatePlayerDieDrop(const CharacterPlayer* killer, const CharacterPlayer* diePlayer, ArrayList<GD::DIE_DROP_MAX, pair<ObjectItem*, int>>& dropItemList)
	{
		// 被红名击杀时不会掉落物品
		const CharacterPlayerData* killerData = killer->getPlayerData();
		const CharacterPlayerData* dieData = diePlayer->getPlayerData();
		const CRIME_LEVEL killerCrimeLevel = getCrimeLevel(killerData->mCrime);
		if (killerCrimeLevel >= CRIME_LEVEL::RED)
		{
			return;
		}
		// 死亡玩家为红名和黄名时掉落几率增加
		const CRIME_LEVEL crimeLevel = getCrimeLevel(dieData->mCrime);
		int dropPackOdds = GD::DIE_DROP_PACK_ODDS;
		int dropDressOdds = GD::DIE_DROP_DRESS_ODDS;
		if (crimeLevel == CRIME_LEVEL::RED)
		{
			dropPackOdds *= 12;
			dropDressOdds *= 12;
		}
		else if (crimeLevel == CRIME_LEVEL::YELLOW)
		{
			dropPackOdds *= 4;
			dropDressOdds *= 4;
		}
		// 攻击者为黄名或者灰名时,降低掉落几率
		if (killerCrimeLevel == CRIME_LEVEL::YELLOW)
		{
			dropPackOdds = (int)(dropPackOdds * 0.3f);
			dropDressOdds = (int)(dropDressOdds * 0.3f);
		}
		else if (killerCrimeLevel == CRIME_LEVEL::GREY)
		{
			dropPackOdds = (int)(dropPackOdds * 0.6f);
			dropDressOdds = (int)(dropDressOdds * 0.6f);
		}
		// 击杀者的等级高于死亡玩家5级以上时,降低掉落物品的几率
		if (killerData->mLevel > dieData->mLevel + 5)
		{
			const float decrease = clampMax((killerData->mLevel - dieData->mLevel - 5) * 0.1f, 1.0f);
			dropPackOdds = (int)(dropPackOdds * (1.0f - decrease));
			dropDressOdds = (int)(dropDressOdds * (1.0f - decrease));
		}
		// 有几率掉落背包
		if (randomHit(dropPackOdds, GD::ODDS_SCALE))
		{
			dropPackItem(diePlayer, dropItemList);
		}
		// 有几率掉落身上穿戴的物品
		if (randomHit(dropDressOdds, GD::ODDS_SCALE))
		{
			dropDressItem(diePlayer, dropItemList);
		}
	}

	void dropPackItem(const CharacterPlayer* diePlayer, ArrayList<GD::DIE_DROP_MAX, pair<ObjectItem*, int>>& dropItemList)
	{
		// 随机获得本次掉落会掉几个物品
		ArrayList<GD::CHAR_PACK_GRID + GD::QUICK_GRID, llong> tempInstanceList;
		const COMPlayerPack* playerPack = diePlayer->getComPack();
		auto& packItemList = playerPack->getItemList();
		playerPack->getItemIDList(tempInstanceList);
		playerPack->getQuickItemIDList(tempInstanceList);
		const int countIndex = randomHit(GD::DIE_DROP_PACK_ODDS_LIST);
		int allObjectCount = tempInstanceList.size();
		FOR_I(clampMax(GD::DIE_DROP_PACK_COUNT_LIST[countIndex], dropItemList.maxSize() - dropItemList.size()))
		{
			if (allObjectCount == 0)
			{
				break;
			}
			const int dropIndex = randomInt(0, allObjectCount - 1);
			ObjectItem* item = packItemList.tryGet(tempInstanceList[dropIndex]);
			if (item == nullptr)
			{
				continue;
			}
			// 时装衣服和时装武器不会掉落
			if (item->getType() == OBJECT_ITEM::EQUIP)
			{
				const EQUIP_TYPE equipType = toEquip(item)->getEquipType();
				if (equipType == EQUIP_TYPE::FASHION_CLOTH ||
					equipType == EQUIP_TYPE::FASHION_WEAPON ||
					equipType == EQUIP_TYPE::FASHION_WING)
				{
					continue;
				}
			}
			// 随机掉落数量
			int itemDropCount = 1;
			if (item->getCount() > 1)
			{
				itemDropCount = randomInt(1, item->getCount());
			}
			dropItemList.add(make_pair(item, itemDropCount));
			MathUtility::swap(tempInstanceList[dropIndex], tempInstanceList[--allObjectCount]);
		}
	}

	void dropDressItem(const CharacterPlayer* diePlayer, ArrayList<GD::DIE_DROP_MAX, pair<ObjectItem*, int>>& dropItemList)
	{
		const int countIndex = randomHit(GD::DIE_DROP_DRESS_ODDS_LIST);
		ArrayList<(int)EQUIP_POSITION::MAX, llong> tempInstanceList;
		const COMPlayerEquip* playerEquip = diePlayer->getComEquip();
		auto& equipList = playerEquip->getEquipList();
		playerEquip->getEquipIDList(tempInstanceList);
		int allObjectCount = tempInstanceList.size();
		FOR_I(GD::DIE_DROP_DRESS_COUNT_LIST[countIndex])
		{
			if (allObjectCount == 0)
			{
				break;
			}
			const int dropIndex = randomInt(0, allObjectCount - 1);
			ObjectItemEquip* equip = equipList.tryGet(tempInstanceList[dropIndex]);
			if (equip == nullptr)
			{
				continue;
			}
			// 时装衣服和时装武器不会掉落
			const EQUIP_TYPE equipType = equip->getEquipType();
			if (equipType == EQUIP_TYPE::FASHION_CLOTH ||
				equipType == EQUIP_TYPE::FASHION_WEAPON ||
				equipType == EQUIP_TYPE::FASHION_WING)
			{
				continue;
			}
			if (!dropItemList.add(make_pair(equip, 1)))
			{
				break;
			}
			MathUtility::swap(tempInstanceList[dropIndex], tempInstanceList[--allObjectCount]);
		}
	}

	int generateSimpleDamage(CharacterGame* attackCharacter, const int attack, const float attackPercent, const int noDefenceAttack,
		const float noDefenceAttackPercent, const DAMAGE_TYPE damageType, const CharacterGame* target, bool& isCritical, bool& isCounterAttack)
	{
		if (attack == 0 && isZero(attackPercent) && noDefenceAttack == 0 && isZero(noDefenceAttackPercent))
		{
			return 0;
		}

		int minAttack = 0;
		int maxAttack = 0;
		int characterNoDefenceAttack = 0;
		int defence = 0;
		const CharacterGameData* attackerProperty = attackCharacter->getCharacterData();
		const CharacterGameData* targetProperty = target->getCharacterData();
		// 如果被攻击的角色有攻击状态,则认为是破招攻击
		isCounterAttack = target->getCharacterData()->mActionStateID == CHARACTER_STATE::StateActionAttack;
		float counterAttack = attackerProperty->mCounterAttack;
		// 获取攻击力和防御力,攻击力不能为负数,防御力可以为负数
		switch (damageType)
		{
		case DAMAGE_TYPE::PHYSICS:
		{
			minAttack = clampZero(attackerProperty->mMinPhysicsAttack);
			maxAttack = clampZero(attackerProperty->mMaxPhysicsAttack);
			characterNoDefenceAttack = attackerProperty->mNoDefencePhysicsAttack;
			defence = randomInt(targetProperty->mMinPhysicsDefence, targetProperty->mMaxPhysicsDefence);
			counterAttack += attackerProperty->mCounterPhysicsAttack;
		}
		break;
		case DAMAGE_TYPE::MAGIC:
		{
			minAttack = clampZero(attackerProperty->mMinMagicAttack);
			maxAttack = clampZero(attackerProperty->mMaxMagicAttack);
			characterNoDefenceAttack = attackerProperty->mNoDefenceMagicAttack;
			defence = randomInt(targetProperty->mMinMagicDefence, targetProperty->mMaxMagicDefence);
			counterAttack += attackerProperty->mCounterMagicAttack;
		}
		break;
		case DAMAGE_TYPE::SORCERY:
		{
			minAttack = clampZero(attackerProperty->mMinSorceryAttack);
			maxAttack = clampZero(attackerProperty->mMaxSorceryAttack);
			characterNoDefenceAttack = attackerProperty->mNoDefenceSorceryAttack;
			// 道术攻击仍然使用的是魔法防御
			defence = randomInt(targetProperty->mMinMagicDefence, targetProperty->mMaxMagicDefence);
			counterAttack += attackerProperty->mCounterSorceryAttack;
		}
		break;
		case DAMAGE_TYPE::MAX:break;
		}
		if (!isCounterAttack)
		{
			counterAttack = 0.0f;
		}

		// 根据幸运值计算随机的角色攻击力,实际上幸运值只会影响攻击下限
		const CharacterPlayer* relatedPlayer = getRelatedPlayer(attackCharacter);
		const int lucky = relatedPlayer != nullptr ? relatedPlayer->getCharacterData()->mLucky : attackerProperty->mLucky;
		const int realMinAttack = lerp(minAttack, maxAttack, lucky * (1.0f / GD::MAX_LUCKY));
		const int characterAttack = randomInt(realMinAttack, maxAttack);
		// 计算实际攻击力
		const int realAttack = attack + 10 + (int)(characterAttack * attackPercent);
		// 根据攻击力防御力计算初始伤害,伤害最低为1
		int damage = clampMin((int)((realAttack * (1.0f + counterAttack) - defence * 1.4f) * 0.8f), 1);

		// 无视防御的攻击伤害
		const int realNoDefenceAttack = noDefenceAttack + (int)(characterNoDefenceAttack * noDefenceAttackPercent);
		damage += (int)(realNoDefenceAttack * (1.0f + counterAttack) * 0.8f);

		// 暴击,计算时暴击率和抗暴率都不能小于0
		float criticalProbability = clampZero(attackerProperty->mCriticalProbability);
		isCritical = randomHit(criticalProbability - clampZero(targetProperty->mAntiCritical));
		if (isCritical)
		{
			// 暴击伤害不能小于0
			damage += (int)(damage * clampZero(attackerProperty->mCriticalDamageRate));
		}

		// 增伤减伤先计算完,最后再乘以伤害值
		float increaseDamageRate = attackerProperty->mIncreaseDamageRate - targetProperty->mDecreaseDamageRate;
		switch (damageType)
		{
		case DAMAGE_TYPE::PHYSICS:	increaseDamageRate -= targetProperty->mPhysicsDecreaseRate; break;
		case DAMAGE_TYPE::MAGIC:	increaseDamageRate -= targetProperty->mMagicDecreaseRate; break;
		case DAMAGE_TYPE::SORCERY:	increaseDamageRate -= targetProperty->mSorceryDecreaseRate; break;
		case DAMAGE_TYPE::MAX:		break;
		}
		// 当最终增伤或者减伤超过0.5时,会逐渐降低增长速率,使之无限接近1,但不会超过1
		const int sign = increaseDamageRate >= 0 ? 1 : -1;
		float finalIncreaseDamageRate = abs(increaseDamageRate);
		if (finalIncreaseDamageRate > 0.5f)
		{
			finalIncreaseDamageRate = finalIncreaseDamageRate / (finalIncreaseDamageRate + 0.5f);
		}
		// 伤害最低为1
		damage = clampMin(damage + (int)(damage * finalIncreaseDamageRate * sign), 1);

		// 最终伤害值需要减少30%
		damage = (int)(damage * 0.7f);

		// 如果是怪物对玩家的伤害,则需要增加150%伤害,一方面是为了补偿上一步减少的30%的伤害,另外也是为了增大一点怪物的难度,但是对召唤兽的伤害不增加
		if (attackCharacter->isMonster() && !attackCharacter->isSummon() && target->isPlayer())
		{
			damage = (int)(damage * 2.5f);
		}
		// 如果是玩家对玩家的伤害,或者玩家对召唤兽的伤害,则伤害减少60%
		else if (attackCharacter->isPlayer() && (target->isPlayer() || target->isSummon()))
		{
			damage = (int)(damage * 0.4f);
		}

		// 确认伤害最低为1
		return clampMin(damage, 1);
	}

	int generateDamage(const CharacterSkill* skill, const SkillBullet* bullet, const CharacterGame* target, bool& isCritical, bool& isCounterAttack)
	{
		CharacterGame* character = skill->getCharacter();
		const float skillAttackIncrease = character->getCharacterData()->mSkillAttackIncrease + 1.0f;
		const int attack = (int)(bullet->getAttack() * skillAttackIncrease);
		const float attackPercent = bullet->getAttackPercent() * skillAttackIncrease;
		const int noDefenceAttack = (int)(bullet->getNoDefenceAttack() * skillAttackIncrease);
		const float noDefenceAttackPercent = bullet->getNoDefenceAttackPercent() * skillAttackIncrease;
		return generateSimpleDamage(character, attack, attackPercent, noDefenceAttack, noDefenceAttackPercent, skill->getDamageType(), target, isCritical, isCounterAttack);
	}

	int generateMaxHP(const int level, const PLAYER_JOB job, const int baseMaxHP)
	{
		switch (job)
		{
		case PLAYER_JOB::FIGHTER:	return (int)(level * 20.0f * 12.5f) + baseMaxHP + 1200;
		case PLAYER_JOB::MAGE:		return (int)(level * 20.0f * 5.0f) + baseMaxHP + 200;
		case PLAYER_JOB::TAOIST:	return (int)(level * 20.0f * 8.5f) + baseMaxHP + 630;
		case PLAYER_JOB::NONE:		return 0;
		}
		return baseMaxHP;
	}

	int generateMaxMP(const int level, const PLAYER_JOB job, const int baseMaxMP)
	{
		switch (job)
		{
		case PLAYER_JOB::FIGHTER:	return (int)(level * 15.0f * 1.0f) + baseMaxMP;
		case PLAYER_JOB::MAGE:		return (int)(level * 15.0f * 3.3f) + baseMaxMP;
		case PLAYER_JOB::TAOIST:	return (int)(level * 15.0f * 1.9f) + baseMaxMP;
		case PLAYER_JOB::NONE:		return 0;
		}
		return baseMaxMP;
	}

	bool canDecreaseMonsterDamage(CharacterMonster* monster)
	{
		const CharacterMonsterData* monsterData = monster->getMonsterData();
		return monsterData->mMonsterSQLite->mStrengthType == MONSTER_STRENGTH::BOSS &&
			monster->getSceneType() == SCENE_TYPE::WORLD &&
			monsterData->mGenerateRegion != nullptr &&
			!monsterData->mGenerateRegion->isMonsterWar();
	}

	int generateStrengthAttack(const int targetStrength, const int equipLevel)
	{
		// 强化等级低于10级时,每升1级,增加3点攻击
		// 10到15级,每级增加5点攻击
		// 15到20级,每级增加8点攻击
		// 20级以上,每级增加12点攻击
		constexpr int INCREASE_0 = 3;
		constexpr int INCREASE_1 = 5;
		constexpr int INCREASE_2 = 8;
		int strengthAttack;
		if (targetStrength > 20)
		{
			constexpr int INCREASE_3 = 12;
			strengthAttack = INCREASE_0 * 10 + INCREASE_1 * 5 + INCREASE_2 * 5 + INCREASE_3 * (targetStrength - 20);
		}
		else if (targetStrength > 15)
		{
			strengthAttack = INCREASE_0 * 10 + INCREASE_1 * 5 + INCREASE_2 * (targetStrength - 15);
		}
		else if (targetStrength > 10)
		{
			strengthAttack = INCREASE_0 * 10 + INCREASE_1 * (targetStrength - 10);
		}
		else
		{
			strengthAttack = INCREASE_0 * targetStrength;
		}
		// 装备等级越高,强化增加的属性越高,但是等级越高,等级带来的增长比例也就越小
		return (int)(strengthAttack * clampMin(2 - divide(20, equipLevel), 1.0f));
	}

	int generateStrengthDefence(const int targetStrength, const int equipLevel)
	{
		// 强化等级低于10级时,每升1级,增加3点防御
		// 10到15级,每级增加6点防御
		// 15到20级,每级增加10点防御
		// 20级以上,每级增加15点防御
		constexpr int INCREASE_0 = 3;
		constexpr int INCREASE_1 = 6;
		constexpr int INCREASE_2 = 10;
		int strengthDefence;
		if (targetStrength > 20)
		{
			constexpr int INCREASE_3 = 15;
			strengthDefence = INCREASE_0 * 10 + INCREASE_1 * 5 + INCREASE_2 * 5 + INCREASE_3 * (targetStrength - 20);
		}
		else if (targetStrength > 15)
		{
			strengthDefence = INCREASE_0 * 10 + INCREASE_1 * 5 + INCREASE_2 * (targetStrength - 15);
		}
		else if (targetStrength > 10)
		{
			strengthDefence = INCREASE_0 * 10 + INCREASE_1 * (targetStrength - 10);
		}
		else
		{
			strengthDefence = INCREASE_0 * targetStrength;
		}
		// 装备等级越高,强化增加的属性越高,但是等级越高,等级带来的增长比例也就越小
		return (int)(strengthDefence * clampMin(2 - divide(25, equipLevel), 1.0f));
	}

	int generateStrengthCostItem(const ObjectItemEquip* equip, const int baseCount)
	{
		// 等级越高,品质越高,消耗数量越多
		const float factor0 = clampZero((equip->getLevel() - 15) * (1.0f / 8.0f));
		const int factor1 = clampZero((int)equip->getQuality() - 2);
		// 不能超过黑铁的携带上限
		return clampMax((int)(((factor0 + factor1) * 0.5f + 1) * baseCount), 9999);
	}

	int generateStrengthCostGold(const ObjectItemEquip* equip, const int baseGold)
	{
		const float factor0 = clampZero((equip->getLevel() - 15) * (1.0f / 8.0f));
		const int factor1 = clampZero((int)equip->getQuality() - 2);
		return (int)(((factor0 + factor1) * 0.5f + 1) * baseGold);
	}

	constexpr HP_DELTA generateHPDeltaTypeAttacker(const HP_DELTA originType, const bool isCritical, const bool isCounterAttack)
	{
		// 当攻击者的攻击产生反弹时,在攻击者看来是反弹的效果
		// 当攻击者的攻击产生免疫时,在攻击者看来是免疫的效果
		// 当攻击者的攻击是附加伤害时,在攻击者看来是附加伤害的效果
		// 当攻击者的攻击是百分比减血时,在攻击者看来是百分比减血的效果
		if (originType == HP_DELTA::COUNTER_INJURY ||
			originType == HP_DELTA::INVINCIBLE ||
			originType == HP_DELTA::ADDITIVE ||
			originType == HP_DELTA::KILL_PERCENT ||
			originType == HP_DELTA::SUCK_BLOOD)
		{
			return originType;
		}

		if (isCritical && isCounterAttack)
		{
			return HP_DELTA::CRITICAL_COUNTER_ATTACK;
		}
		else if (isCritical)
		{
			return HP_DELTA::CRITICAL;
		}
		else if (isCounterAttack)
		{
			return HP_DELTA::COUNTER_ATTACK;
		}
		return HP_DELTA::NORMAL;
	}

	constexpr HP_DELTA generateHPDeltaTypeHPChanged(const HP_DELTA originType)
	{
		// 对于被攻击者来说,除了免疫和反弹会特殊显示,其余的都是普通被击伤害
		if (originType == HP_DELTA::COUNTER_INJURY ||
			originType == HP_DELTA::INVINCIBLE ||
			originType == HP_DELTA::ADDITIVE ||
			originType == HP_DELTA::KILL_PERCENT ||
			originType == HP_DELTA::SUCK_BLOOD)
		{
			return originType;
		}
		return HP_DELTA::BEEN_ATTACK;
	}

	void fillEquipList(ArrayList<(int)EQUIP_POSITION::MAX, ObjectItem*>& equipList, const MDCharacterEquip& mysqlData, const HashMap<llong, MDObjectItemEquip*>& equipDataList)
	{
		getEquipObject(equipList, mysqlData.mCloth, equipDataList);
		getEquipObject(equipList, mysqlData.mWeapon, equipDataList);
		getEquipObject(equipList, mysqlData.mHelmet, equipDataList);
		getEquipObject(equipList, mysqlData.mNecklack, equipDataList);
		getEquipObject(equipList, mysqlData.mMedal, equipDataList);
		getEquipObject(equipList, mysqlData.mLeftBracelet, equipDataList);
		getEquipObject(equipList, mysqlData.mRightBracelet, equipDataList);
		getEquipObject(equipList, mysqlData.mLeftRing, equipDataList);
		getEquipObject(equipList, mysqlData.mRightRing, equipDataList);
		getEquipObject(equipList, mysqlData.mBelt, equipDataList);
		getEquipObject(equipList, mysqlData.mShoe, equipDataList);
		getEquipObject(equipList, mysqlData.mDiamond, equipDataList);
		getEquipObject(equipList, mysqlData.mFashionCloth, equipDataList);
		getEquipObject(equipList, mysqlData.mFashionWeapon, equipDataList);
		getEquipObject(equipList, mysqlData.mFashionWing, equipDataList);
	}

	int vectorToDir8(const Vector3& vec)
	{
		if (mDirectionVector8.size() == 0)
		{
			initAngle();
		}
		float angle = getAngleFromVector2ToVector2(Vector2::UP, Vector2(vec.x, vec.y));
		clampRadian360(angle);
		// 找到位于哪两个角度之间
		if (angle <= mDirectionAngle16[1] || angle > mDirectionAngle16[mDirectionAngle16.size() - 1])
		{
			return 0;
		}
		FOR_I(8)
		{
			if (i > 0 && angle > mDirectionAngle16[(i << 1) - 1] && angle <= mDirectionAngle16[(i << 1) + 1])
			{
				return i;
			}
		}
		ERROR("角度计算错误");
		return 0;
	}

	Vector3 dir8ToVector(int dir)
	{
		if (mDirectionVector8.size() == 0)
		{
			initAngle();
		}
		clampRef(dir, 0, GD::DIRECTION_COUNT_8 - 1);
		return mDirectionVector8[dir];
	}

	CharacterBuff* characterAddBuff(const int buffDetailID, const float buffTime, CharacterGame* character)
	{
		CharacterBuffParam* param = mStateManager->createParamByDetailID(buffDetailID);
		param->mStateTime = buffTime;
		CharacterBuff* state = characterAddBuff(param, character);
		// create出来的参数对象需要被销毁
		mStateParamPool->destroyClass(param);
		return state;
	}

	CharacterBuff* characterAddBuff(const int buffDetailID, CharacterGame* character, CharacterGame* source)
	{
		if (buffDetailID == 0)
		{
			return nullptr;
		}
		CharacterBuffParam* param = mStateManager->getStateOriginParam(buffDetailID);
		return static_cast<CharacterBuff*>(CmdCharacterAddState::execute(character, source, param->mBuffType, param));
	}

	CharacterBuff* characterAddBuff(CharacterBuffParam* param, CharacterGame* character, CharacterGame* source)
	{
		return static_cast<CharacterBuff*>(CmdCharacterAddState::execute(character, source, param->mBuffType, param));
	}

	// 以地砖左下角为原点的像素坐标所处的三角形下标
	constexpr TILE_TRIANGLE pixelPosToTriangleIndex(const Vector2& pos)
	{
		if (pos.x < 0.0f || pos.x > GD::TILE_WIDTH || pos.y < 0.0f || pos.y > GD::TILE_HEIGHT)
		{
			return TILE_TRIANGLE::MAX;
		}
		// 对角线斜率
		constexpr float k = (float)GD::TILE_HEIGHT / GD::TILE_WIDTH;
		// 位于左半部分
		if (pos.x <= GD::TILE_WIDTH >> 1)
		{
			// 位于左下部分小矩形中
			if (pos.y <= GD::TILE_HEIGHT >> 1)
			{
				// 相对于小矩形的右下角的相对坐标
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(divide(pos.y, pos.x - (GD::TILE_WIDTH >> 1))) > k)
				{
					return TILE_TRIANGLE::INNER_LEFT_BOTTOM;
				}
				else
				{
					return TILE_TRIANGLE::LEFT_BOTTOM;
				}
			}
			// 位于左上部分
			else
			{
				// 相对于地砖中左上角小矩形的左下角的相对坐标
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(divide(pos.y - (GD::TILE_HEIGHT >> 1), pos.x)) > k)
				{
					return TILE_TRIANGLE::LEFT_TOP;
				}
				else
				{
					return TILE_TRIANGLE::INNER_LEFT_TOP;
				}
			}
		}
		// 位于右半部分
		else
		{
			// 位于右下部分
			if (pos.y <= GD::TILE_HEIGHT >> 1)
			{
				// 相对于地砖中右下角小矩形的左下角的相对坐标
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(divide(pos.y, pos.x - (GD::TILE_WIDTH >> 1))) > k)
				{
					return TILE_TRIANGLE::INNER_RIGHT_BOTTOM;
				}
				else
				{
					return TILE_TRIANGLE::RIGHT_BOTTOM;
				}
			}
			// 位于右上部分
			else
			{
				// 相对于地砖中右上角小矩形的左下角的相对坐标
				// 根据相对坐标的斜率判断属于哪个三角形
				if (abs(divide(pos.y - (GD::TILE_HEIGHT >> 1), pos.x - GD::TILE_WIDTH)) > k)
				{
					return TILE_TRIANGLE::RIGHT_TOP;
				}
				else
				{
					return TILE_TRIANGLE::INNER_RIGHT_TOP;
				}
			}
		}
	}

	// 获取指定位置上的三角形三个顶点的坐标,坐标是相对于所属地砖左下角
	constexpr void getTrianglePoints(TILE_TRIANGLE pos, Vector2& point0, Vector2& point1, Vector2& point2)
	{
		switch (pos)
		{
		case TILE_TRIANGLE::LEFT_TOP:
		{
			point0 = Vector2(0.0f, GD::TILE_HEIGHT);
			point1 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT);
			point2 = Vector2(0.0f, GD::TILE_HEIGHT >> 1);
		}
		break;
		case TILE_TRIANGLE::RIGHT_TOP:
		{
			point0 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT);
			point1 = Vector2(GD::TILE_WIDTH, GD::TILE_HEIGHT);
			point2 = Vector2(GD::TILE_WIDTH, GD::TILE_HEIGHT >> 1);
		}
		break;
		case TILE_TRIANGLE::RIGHT_BOTTOM:
		{
			point0 = Vector2(GD::TILE_WIDTH, GD::TILE_HEIGHT >> 1);
			point1 = Vector2(GD::TILE_WIDTH, 0.0f);
			point2 = Vector2(GD::TILE_WIDTH >> 1, 0.0f);
		}
		break;
		case TILE_TRIANGLE::LEFT_BOTTOM:
		{
			point0 = Vector2(GD::TILE_WIDTH >> 1, 0.0f);
			point1 = Vector2(0.0f, 0.0f);
			point2 = Vector2(0.0f, GD::TILE_HEIGHT >> 1);
		}
		break;
		case TILE_TRIANGLE::INNER_LEFT_TOP:
		{
			point0 = Vector2(0.0f, GD::TILE_HEIGHT >> 1);
			point1 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT);
			point2 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT >> 1);
		}
		break;
		case TILE_TRIANGLE::INNER_RIGHT_TOP:
		{
			point0 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT >> 1);
			point1 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT);
			point2 = Vector2(GD::TILE_WIDTH, GD::TILE_HEIGHT >> 1);
		}
		break;
		case TILE_TRIANGLE::INNER_RIGHT_BOTTOM:
		{
			point0 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT >> 1);
			point1 = Vector2(GD::TILE_WIDTH, GD::TILE_HEIGHT >> 1);
			point2 = Vector2(GD::TILE_WIDTH >> 1, 0.0f);
		}
		break;
		case TILE_TRIANGLE::INNER_LEFT_BOTTOM:
		{
			point0 = Vector2(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT >> 1);
			point1 = Vector2(GD::TILE_WIDTH >> 1, 0.0f);
			point2 = Vector2(0.0f, GD::TILE_HEIGHT >> 1);
		}
		break;
		case TILE_TRIANGLE::MAX:break;
		}
	}

	Vector3 generateMonsterMove(const CharacterMonster* monster, const Vector3& curPos, const byte direction, const float moveLength, const SceneMap* scene)
	{
		MapData* map = scene->getMapData();
		int findTimes = 0;
		int sign = -1;
		// 最多只查找8次
		while (findTimes++ < 8)
		{
			const int curDirection = (direction + sign * (findTimes >> 1) + GD::DIRECTION_COUNT_8) % GD::DIRECTION_COUNT_8;
			Vector3 destPos = curPos + dir8ToVector(curDirection) * moveLength;
			bool movable = true;
			const bool directReach = checkMovable(curPos, destPos, movable, map);
			bool posOverlap = false;
			if (directReach)
			{
				const int blockIndex = map->pixelPosToSceneBlock(destPos);
				const SceneBlock* block = scene->getBlock(blockIndex);
				posOverlap = block != nullptr && block->isMonsterPosOverlap(destPos, monster);
			}
			// 如果目标点可直接到达,且没有与其他怪物的位置重叠,则允许移动,否则换一个方向
			if (movable && directReach && !posOverlap)
			{
				return destPos;
			}
			sign *= -1;
		}
		return curPos;
	}

	bool checkMovable(const Vector3& curPos, Vector3& moveDest, bool& movable, MapData* map)
	{
		if (!map->isPositionInMap(moveDest))
		{
			movable = false;
			return false;
		}

		const Vector3 supposeDelta = moveDest - curPos;
		const Vector3 moveDir = normalize(supposeDelta);
		const Vector3 supposeDest = moveDest;
		const float supposeMoveLength = getLength(supposeDelta);
		UnreachIntersect result;
		constexpr float radius = 1.0f;
		// 再向移动方向延伸一段距离作为检测点,避免误差
		Vector3 checkPos = supposeDest + moveDir * (radius + supposeMoveLength);
		// 将目标点都限定在地图范围内
		map->clampPosition(checkPos);
		if (map->checkReachable(curPos, checkPos, result))
		{
			moveDest = supposeDest;
			movable = true;
			return true;
		}

		float angle = getAngleBetweenVector(Vector2(moveDir.x, moveDir.y), result.mLinePoint1 - result.mLinePoint0);
		if (angle > HALF_PI_RADIAN)
		{
			angle = PI_RADIAN - angle;
		}
		// 如果圆与墙相切时,圆心到交点的距离
		const float minDis = isEqual(angle, HALF_PI_RADIAN) ? radius : divide(radius, sin(angle));
		// 当角色的圆与墙相切时,角色的位置
		const Vector3 delta = Vector3(result.mPoint.x, result.mPoint.y, 0.0f) - curPos;
		const float realMoveLength = getLength(delta) - minDis;
		// 离交点足够近了,已经无法再向目标方向移动了
		movable = realMoveLength > 0.0f;
		if (!movable)
		{
			moveDest = curPos;
			return false;
		}
		// 当移动距离大于等于需要移动的距离时才认为是可以直接达到,否则就是需要经过调整
		const bool moveRet = realMoveLength >= supposeMoveLength;
		moveDest = curPos + moveDir * clampMax(realMoveLength, supposeMoveLength);
		return moveRet;
	}

	void getPlayersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		getPlayersNearBlock(scene, character->getPosition(), characterList);
	}

	void getMonstersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		getMonstersNearBlock(scene, character->getPosition(), characterList);
	}

	void getSummonsNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		getSummonsNearBlock(scene, character->getPosition(), characterList);
	}

	void getNormalMonstersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		getNormalMonstersNearBlock(scene, character->getPosition(), characterList);
	}

	void getNormalMonstersNearBlock(const CharacterGame* character, Set<CharacterGame*>& characterList, const float range, const int maxCount)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		getNormalMonstersNearBlock(scene, character->getPosition(), characterList, range, maxCount);
	}

	void getPlayersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList)
	{
		const ArrayList<9, int>& blockIndexList = scene->getMapData()->getAroundBlockList(pos);
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历周围区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			const SceneBlock& block = blockList[blockIndexList[i]];
			if (block.getPlayerCount() > 0)
			{
				characterList.insert(block.getPlayerList());
			}
		}
	}

	void getMonstersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList)
	{
		const ArrayList<9, int>& blockIndexList = scene->getMapData()->getAroundBlockList(pos);
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历周围区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			characterList.insert(blockList[blockIndexList[i]].getMonsterList().getMainList());
			characterList.insert(blockList[blockIndexList[i]].getSummonList().getMainList());
		}
	}

	void getSummonsNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList)
	{
		const ArrayList<9, int>& blockIndexList = scene->getMapData()->getAroundBlockList(pos);
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历周围区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			characterList.insert(blockList[blockIndexList[i]].getSummonList().getMainList());
		}
	}

	void getNormalMonstersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList)
	{
		const ArrayList<9, int>& blockIndexList = scene->getMapData()->getAroundBlockList(pos);
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历周围区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			characterList.insert(blockList[blockIndexList[i]].getMonsterList().getMainList());
		}
	}

	void getNormalMonstersNearBlock(const SceneMap* scene, const Vector3& pos, Set<CharacterGame*>& characterList, const float range, const int maxCount)
	{
		const ArrayList<9, int>& blockIndexList = scene->getMapData()->getAroundBlockList(pos);
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历周围区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			const auto& monsterList = blockList[blockIndexList[i]].getMonsterList().getMainList();
			if (monsterList.size() == 0)
			{
				continue;
			}
			for (CharacterMonster* monster : monsterList)
			{
				if (!lengthLess(pos - monster->getPosition(), range))
				{
					continue;
				}
				characterList.insert(monster);
				if (characterList.size() >= maxCount)
				{
					return;
				}
			}
		}
	}

	bool hasPlayerOrSummonInRange(CharacterGame* character, const float radius)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasPlayerOrSummonInRange(character, radius, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasPlayerOrSummonInRange(character, radius, blockIndexList);
		}
	}

	bool hasPlayerOrSummonInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasPlayerOrSummonInRange(pos, radius, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasPlayerInRange(CharacterGame* character, const float radius)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasPlayerInRange(character, radius, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasPlayerInRange(character, radius, blockIndexList);
		}
	}

	bool hasPlayerInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasPlayerInRange(pos, radius, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasMonsterInRange(CharacterGame* character, const float radius)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasMonsterInRange(character, radius, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasMonsterInRange(character, radius, blockIndexList);
		}
	}

	bool hasMonsterInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasMonsterInRange(pos, radius, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasSummonInRange(CharacterGame* character, const float radius)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasSummonInRange(character, radius, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasSummonInRange(character, radius, blockIndexList);
		}
	}

	bool hasSummonInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasSummonInRange(pos, radius, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasNormalMonsterInRange(CharacterGame* character, const float radius)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasNormalMonsterInRange(character, radius, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasNormalMonsterInRange(character, radius, blockIndexList);
		}
	}

	bool hasNormalMonsterInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasNormalMonsterInRange(pos, radius, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasPlayerInRangeEx(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasPlayerInRangeEx(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasPlayerInRangeEx(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasPlayerInRangeEx(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasPlayerInRangeEx(pos, radius, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasMonsterInRangeEx(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasMonsterInRangeEx(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasMonsterInRangeEx(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasMonsterInRangeEx(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasMonsterInRangeEx(pos, radius, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasSummonInRangeEx(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasSummonInRangeEx(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasSummonInRangeEx(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasSummonInRangeEx(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasSummonInRangeEx(pos, radius, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasNormalMonsterInRangeEx(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasNormalMonsterInRangeEx(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasNormalMonsterInRangeEx(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasNormalMonsterInRangeEx(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasNormalMonsterInRangeEx(pos, radius, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasPlayerCountInRange(CharacterGame* character, const float radius, const int minCount)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() < minCount)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasPlayerCountInRange(character, radius, minCount, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasPlayerCountInRange(character, radius, minCount, blockIndexList);
		}
	}

	bool hasPlayerCountInRange(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() < minCount)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasPlayerCountInRange(pos, radius, minCount, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasMonsterCountInRange(CharacterGame* character, const float radius, const int minCount)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasMonsterCountInRange(character, radius, minCount, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasMonsterCountInRange(character, radius, minCount, blockIndexList);
		}
	}

	bool hasMonsterCountInRange(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasMonsterCountInRange(pos, radius, minCount, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasSummonCountInRange(CharacterGame* character, const float radius, const int minCount)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasSummonCountInRange(character, radius, minCount, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasSummonCountInRange(character, radius, minCount, blockIndexList);
		}
	}

	bool hasSummonCountInRange(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasSummonCountInRange(pos, radius, minCount, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasNormalMonsterCountInRange(CharacterGame* character, const float radius, const int minCount)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasNormalMonsterCountInRange(character, radius, minCount, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasNormalMonsterCountInRange(character, radius, minCount, blockIndexList);
		}
	}

	bool hasNormalMonsterCountInRange(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasNormalMonsterCountInRange(pos, radius, minCount, character))
			{
				return true;
			}
		}
		return false;
	}

	bool hasPlayerCountInRangeEx(CharacterGame* character, const float radius, const int minCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() < minCount)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasPlayerCountInRangeEx(character, radius, minCount, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasPlayerCountInRangeEx(character, radius, minCount, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasPlayerCountInRangeEx(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() < minCount)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasPlayerCountInRangeEx(pos, radius, minCount, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasMonsterCountInRangeEx(CharacterGame* character, const float radius, const int minCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasMonsterCountInRangeEx(character, radius, minCount, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasMonsterCountInRangeEx(character, radius, minCount, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasMonsterCountInRangeEx(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasMonsterCountInRangeEx(pos, radius, minCount, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasSummonCountInRangeEx(CharacterGame* character, const float radius, const int minCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasSummonCountInRangeEx(character, radius, minCount, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasSummonCountInRangeEx(character, radius, minCount, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasSummonCountInRangeEx(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasSummonCountInRangeEx(pos, radius, minCount, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	bool hasNormalMonsterCountInRangeEx(CharacterGame* character, const float radius, const int minCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}
		if (character->isMonster())
		{
			return hasNormalMonsterCountInRangeEx(character, radius, minCount, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return hasNormalMonsterCountInRangeEx(character, radius, minCount, blockIndexList, check, userData, includeDead);
		}
	}

	bool hasNormalMonsterCountInRangeEx(const CharacterGame* character, const float radius, const int minCount, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return false;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (blockList[blockIndexList[i]].hasNormalMonsterCountInRangeEx(pos, radius, minCount, character, check, userData, includeDead))
			{
				return true;
			}
		}
		return false;
	}

	int getPlayerCountInRange(CharacterGame* character, const float radius, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return 0;
		}
		if (character->isMonster())
		{
			return getPlayerCountInRange(character, radius, toMonster(character)->getNearBlockList(radius), maxCount, check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getPlayerCountInRange(character, radius, blockIndexList, maxCount, check, userData, includeDead);
		}
	}

	int getPlayerCountInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return 0;
		}

		int count = 0;
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			count = blockList[blockIndexList[i]].getPlayerCountInRange(pos, radius, count, maxCount, character, check, userData, includeDead);
		}
		return count;
	}

	int getMonsterCountInRange(CharacterGame* character, const float radius, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return 0;
		}
		if (character->isMonster())
		{
			return getMonsterCountInRange(character, radius, toMonster(character)->getNearBlockList(radius), maxCount, check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getMonsterCountInRange(character, radius, blockIndexList, maxCount, check, userData, includeDead);
		}
	}

	int getMonsterCountInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return 0;
		}

		int count = 0;
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			count = blockList[blockIndexList[i]].getMonsterCountInRange(pos, radius, count, maxCount, character, check, userData, includeDead);
		}
		return count;
	}

	int getSummonCountInRange(CharacterGame* character, const float radius, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return 0;
		}
		if (character->isMonster())
		{
			return getSummonCountInRange(character, radius, toMonster(character)->getNearBlockList(radius), maxCount, check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getSummonCountInRange(character, radius, blockIndexList, maxCount, check, userData, includeDead);
		}
	}

	int getSummonCountInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return 0;
		}

		int count = 0;
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			count = blockList[blockIndexList[i]].getSummonCountInRange(pos, radius, count, maxCount, character, check, userData, includeDead);
		}
		return count;
	}

	int getNormalMonsterCountInRange(CharacterGame* character, const float radius, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return 0;
		}
		if (character->isMonster())
		{
			return getNormalMonsterCountInRange(character, radius, toMonster(character)->getNearBlockList(radius), maxCount, check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNormalMonsterCountInRange(character, radius, blockIndexList, maxCount, check, userData, includeDead);
		}
	}

	int getNormalMonsterCountInRange(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, const int maxCount, CheckCharacter check, void* userData, const bool includeDead)
	{
		SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return 0;
		}

		int count = 0;
		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			count = blockList[blockIndexList[i]].getNormalMonsterCountInRange(pos, radius, count, maxCount, character, check, userData, includeDead);
		}
		return count;
	}

	void getPlayersInRange(CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getPlayersInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getPlayersInRange(character, radius, characterList, blockIndexList, check, userData, clear, includeDead);
		}
	}

	void getPlayersInRange(const CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getPlayersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}

	void getMonstersInRange(CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		if (character->isMonster())
		{
			getMonstersInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getMonstersInRange(character, radius, characterList, blockIndexList, check, userData, clear, includeDead);
		}
	}

	void getMonstersInRange(const CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getMonstersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}

	void getSummonsInRange(CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getSummonsInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getSummonsInRange(character, radius, characterList, blockIndexList, check, userData, clear, includeDead);
		}
	}

	void getSummonsInRange(const CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getSummonsInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}

	void getNormalMonstersInRange(CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}
		if (character->isMonster())
		{
			getNormalMonstersInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getNormalMonstersInRange(character, radius, characterList, blockIndexList, check, userData, clear, includeDead);
		}
	}

	void getNormalMonstersInRange(const CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getNormalMonstersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}

	void getPlayersInRangeIncludeSelf(CharacterGame* character, const Vector3& pos, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getPlayersInRangeIncludeSelf(character, pos, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, pos, radius, blockIndexList);
			getPlayersInRangeIncludeSelf(character, pos, radius, characterList, blockIndexList, check, userData, clear, includeDead);
		}
	}

	void getPlayersInRangeIncludeSelf(const CharacterGame* character, const Vector3& pos, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}

		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getPlayersInRange(pos, radius, characterList, nullptr, check, userData, includeDead);
		}
	}

	void getPlayersInRange(CharacterGame* character, const Vector3& pos, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getPlayersInRange(character, pos, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, pos, radius, blockIndexList);
			getPlayersInRange(character, pos, radius, characterList, blockIndexList, check, userData, clear);
		}
	}

	void getPlayersInRange(const CharacterGame* character, const Vector3& pos, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getPlayersInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}

	void getPlayersAndSummonsInRange(CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}
		if (character->isMonster())
		{
			getPlayersAndSummonsInRange(character, radius, characterList, toMonster(character)->getNearBlockList(radius), check, userData, clear, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			getPlayersAndSummonsInRange(character, radius, characterList, blockIndexList, check, userData, clear, includeDead);
		}
	}

	void getPlayersAndSummonsInRange(const CharacterGame* character, const float radius, Set<CharacterGame*>& characterList, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool clear, const bool includeDead)
	{
		if (clear)
		{
			characterList.clear();
		}
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			blockList[blockIndexList[i]].getPlayersInRange(pos, radius, characterList, character, check, userData, includeDead);
			blockList[blockIndexList[i]].getSummonsInRange(pos, radius, characterList, character, check, userData, includeDead);
		}
	}

	CharacterGame* getNearestPlayerOrSummon(CharacterGame* character, const float radius)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearestPlayerOrSummon(character, radius, toMonster(character)->getNearBlockList(radius));
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearestPlayerOrSummon(character, radius, blockIndexList);
		}
	}

	CharacterGame* getNearestPlayerOrSummonEx(CharacterGame* character, const float radius, CheckCharacter check, void* userData)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearestPlayerOrSummonEx(character, radius, toMonster(character)->getNearBlockList(radius), check, userData);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearestPlayerOrSummonEx(character, radius, blockIndexList, check, userData);
		}
	}

	CharacterGame* getNearestPlayerOrSummon(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		float minDistance = 9999.0f * 9999.0f;
		CharacterGame* nearestCharacter = nullptr;
		FOR_I(blockIndexList.size())
		{
			float curSquaredDis = 0.0f;
			CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearestPlayerOrSummon(pos, radius, &curSquaredDis, character);
			if (curCharacter != nullptr && curSquaredDis < minDistance)
			{
				minDistance = curSquaredDis;
				nearestCharacter = curCharacter;
			}
		}
		return nearestCharacter;
	}

	CharacterGame* getNearestPlayerOrSummonEx(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		float minDistance = 9999.0f * 9999.0f;
		CharacterGame* nearestCharacter = nullptr;
		FOR_I(blockIndexList.size())
		{
			float curSquaredDis = 0.0f;
			CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearestPlayerOrSummonEx(pos, radius, &curSquaredDis, character, check, userData, false);
			if (curCharacter != nullptr && curSquaredDis < minDistance)
			{
				minDistance = curSquaredDis;
				nearestCharacter = curCharacter;
			}
		}
		return nearestCharacter;
	}

	CharacterGame* getNearestPlayer(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearestPlayer(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearestPlayer(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearestPlayer(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		float minDistance = 9999.0f * 9999.0f;
		CharacterGame* nearestCharacter = nullptr;
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			float curSquaredDis = 0.0f;
			CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearestPlayer(pos, radius, &curSquaredDis, character, check, userData, includeDead);
			if (curCharacter != nullptr && curSquaredDis < minDistance)
			{
				minDistance = curSquaredDis;
				nearestCharacter = curCharacter;
			}
		}
		return nearestCharacter;
	}

	CharacterGame* getNearestMonster(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearestMonster(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearestMonster(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearestMonster(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		float minDistance = 9999.0f * 9999.0f;
		CharacterGame* nearestCharacter = nullptr;
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			float curSquaredDis = 0.0f;
			CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearestMonster(pos, radius, &curSquaredDis, character, check, userData, includeDead);
			if (curCharacter != nullptr && curSquaredDis < minDistance)
			{
				minDistance = curSquaredDis;
				nearestCharacter = curCharacter;
			}
		}
		return nearestCharacter;
	}

	CharacterGame* getNearestSummon(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearestSummon(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearestSummon(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearestSummon(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		float minDistance = 9999.0f * 9999.0f;
		CharacterGame* nearestCharacter = nullptr;
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			float curSquaredDis = 0.0f;
			CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearestSummon(pos, radius, &curSquaredDis, character, check, userData, includeDead);
			if (curCharacter != nullptr && curSquaredDis < minDistance)
			{
				minDistance = curSquaredDis;
				nearestCharacter = curCharacter;
			}
		}
		return nearestCharacter;
	}

	CharacterGame* getNearestNormalMonster(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearestNormalMonster(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearestNormalMonster(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearestNormalMonster(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		float minDistance = 9999.0f * 9999.0f;
		CharacterGame* nearestCharacter = nullptr;
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			float curSquaredDis = 0.0f;
			CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearestNormalMonster(pos, radius, &curSquaredDis, character, check, userData, includeDead);
			if (curCharacter != nullptr && curSquaredDis < minDistance)
			{
				minDistance = curSquaredDis;
				nearestCharacter = curCharacter;
			}
		}
		return nearestCharacter;
	}

	CharacterGame* getNearBlockFirstPlayer(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearBlockFirstPlayer(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearBlockFirstPlayer(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearBlockFirstPlayer(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearFirstPlayer(pos, radius, character, check, userData, includeDead))
			{
				return curCharacter;
			}
		}
		return nullptr;
	}

	CharacterGame* getNearBlockFirstMonster(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearBlockFirstMonster(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearBlockFirstMonster(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearBlockFirstMonster(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearFirstMonster(pos, radius, character, check, userData, includeDead))
			{
				return curCharacter;
			}
		}
		return nullptr;
	}

	CharacterGame* getNearBlockFirstSummon(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearBlockFirstSummon(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearBlockFirstSummon(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearBlockFirstSummon(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr || scene->getPlayerList().size() == 0)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearFirstSummon(pos, radius, character, check, userData, includeDead))
			{
				return curCharacter;
			}
		}
		return nullptr;
	}

	CharacterGame* getNearBlockFirstNormalMonster(CharacterGame* character, const float radius, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}
		if (character->isMonster())
		{
			return getNearBlockFirstNormalMonster(character, radius, toMonster(character)->getNearBlockList(radius), check, userData, includeDead);
		}
		else
		{
			ArrayList<64, int> blockIndexList;
			generateRangeBlock(scene, character->getPosition(), radius, blockIndexList);
			return getNearBlockFirstNormalMonster(character, radius, blockIndexList, check, userData, includeDead);
		}
	}

	CharacterGame* getNearBlockFirstNormalMonster(const CharacterGame* character, const float radius, const ArrayList<64, int>& blockIndexList, CheckCharacter check, void* userData, const bool includeDead)
	{
		const SceneMap* scene = character->getScene();
		if (scene == nullptr)
		{
			return nullptr;
		}

		const Vector3& pos = character->getPosition();
		const SceneBlock* blockList = scene->getBlockList();
		// 遍历形所覆盖的所有区域,判断是否有玩家在指定范围内
		FOR_I(blockIndexList.size())
		{
			if (CharacterGame* curCharacter = blockList[blockIndexList[i]].getNearFirstNormalMonster(pos, radius, character, check, userData, includeDead))
			{
				return curCharacter;
			}
		}
		return nullptr;
	}

	void generateAvailablePosition(const SceneMap* scene, const Vector3& centerPos, Vector3& newPos)
	{
		const MapData* mapData = scene->getMapData();
		// 查看newPos是否可用
		if (mapData->checkPositionValid(newPos))
		{
			newPos = centerPos;
			return;
		}
		// 每次旋转45度,newPos已经检测过了,所以只需要旋转7次
		Vector3 relative = newPos - centerPos;
		FOR_I(7)
		{
			Vector3 newRelative = rotateVector3AroundZ(relative, (i + 1) * toRadian(45.0f));
			// 找到一个可用位置,则直接返回
			if (mapData->checkPositionValid(newRelative + centerPos))
			{
				newPos = newRelative + centerPos;
				return;
			}
		}
		// 找完一圈都找不到位置,则返回圆心位置
		newPos = centerPos;
	}

	// 计算一个正方形所占据的场景分块的坐标列表,最多允许半径覆盖8*8个区块范围
	void generateRangeBlock(const SceneMap* scene, const Vector3& pos, const float radius, ArrayList<64, int>& blockIndexList)
	{
		const MapData* mapData = scene->getMapData();
		// 坐标系Y轴是向下为正
		Vector3 topPixel(pos.x, pos.y + radius, 0.0f);
		Vector3 bottomPixel(pos.x, pos.y - radius, 0.0f);
		Vector3 leftPixel(pos.x - radius, pos.y, 0.0f);
		Vector3 rightPixel(pos.x + radius, pos.y, 0.0f);
		mapData->clampPosition(topPixel);
		mapData->clampPosition(bottomPixel);
		mapData->clampPosition(leftPixel);
		mapData->clampPosition(rightPixel);
		const int topTileY = mapData->pixelPosYToTilePosY(topPixel.y);
		const int topBlockY = (int)(topTileY * (1.0f / GD::BLOCK_SIZE));
		const int bottomTileY = mapData->pixelPosYToTilePosY(bottomPixel.y);
		const int bottomBlockY = (int)(bottomTileY * (1.0f / GD::BLOCK_SIZE));
		const int leftTileX = mapData->pixelPosXToTilePosX(leftPixel.x);
		const int leftBlockX = (int)(leftTileX * (1.0f / GD::BLOCK_SIZE));
		const int rightTileX = mapData->pixelPosXToTilePosX(rightPixel.x);
		const int rightBlockX = (int)(rightTileX * (1.0f / GD::BLOCK_SIZE));

		for (int i = leftBlockX; i <= rightBlockX; ++i)
		{
			for (int j = topBlockY; j <= bottomBlockY; ++j)
			{
				if (!blockIndexList.add(mapData->blockXYToBlockIndex(i, j)))
				{
					ERROR_PROFILE(("范围半径太大,radius:" + FToS(radius) + ", pos:" + V3ToS(pos) + ", mapID:" + IToS(scene->getMapID())).c_str());
					break;
				}
			}
		}
	}

	CharacterPlayer* getRelatedPlayer(CharacterGame* character)
	{
		if (character == nullptr)
		{
			return nullptr;
		}
		CharacterPlayer* player = nullptr;
		if (character->isPlayer())
		{
			player = toPlayer(character);
		}
		else if (character->isMonster())
		{
			CharacterGame* master = toMonster(character)->getMonsterData()->mMaster;
			if (master != nullptr && master->isPlayer())
			{
				player = toPlayer(master);
			}
		}
		return player;
	}

	CRIME_LEVEL getCrimeLevel(const float crime)
	{
		if (crime >= GD::RED_NAME_CRIME)
		{
			return CRIME_LEVEL::RED;
		}
		if (crime >= GD::YELLOW_NAME_CRIME)
		{
			return CRIME_LEVEL::YELLOW;
		}
		if (crime > 0.0f)
		{
			return CRIME_LEVEL::GREY;
		}
		return CRIME_LEVEL::NONE;
	}

	void removeEnterSceneCopyCost(CharacterPlayer* player, const int sceneCopyID, const float costMultiple)
	{
		if (player == nullptr)
		{
			return;
		}
		const TDSceneCopy* sceneCopyData = mSQLiteSceneCopy->querySceneCopy(sceneCopyID);
		if (sceneCopyData == nullptr)
		{
			return;
		}
		// 金币
		CmdCharacterAddGold::execute(player, -(int)(sceneCopyData->mCostGold * costMultiple));
		// 元宝
		CmdCharacterAddGoldIngot::execute(player, -(int)(sceneCopyData->mCostGoldIngot * costMultiple));
		// 材料
		const auto& costItemList = sceneCopyData->mCostItem;
		FOR_VECTOR(costItemList)
		{
			CmdCharacterRemoveObjectType::execute(player, costItemList[i], (int)(sceneCopyData->mCostItemCount[i] * costMultiple));
		}
	}

	CharacterPlayer* getPlayer(const llong playerGUID)
	{
		Character* character = mCharacterManager->getCharacter(playerGUID);
		if (character == nullptr || character->getType() != CHARACTER_TYPE::PLAYER)
		{
			return nullptr;
		}
		CharacterGame* characterGame = toCharacterGame(character);
		if (characterGame->isDestroy())
		{
			ERROR_PROFILE("0角色已经被销毁了");
			return nullptr;
		}
		if (!characterGame->isPlayer())
		{
			return nullptr;
		}
		return toPlayer(characterGame);
	}

	CharacterGame* getCharacterGame(const llong characterGUID)
	{
		return toCharacterGame(mCharacterManager->getCharacter(characterGUID));
	}

	void sendPacket(PacketTCP* packet, const CharacterPlayer* player0, const CharacterPlayer* player1, const CharacterPlayer* player2)
	{
		if (player0 == nullptr && player1 == nullptr && player2 == nullptr)
		{
			return;
		}
		mTCPServerSystem->writePacket(packet);
		if (player0 != nullptr)
		{
			player0->getClient()->sendPacket(packet);
		}
		if (player1 != nullptr)
		{
			player1->getClient()->sendPacket(packet);
		}
		if (player2 != nullptr)
		{
			player2->getClient()->sendPacket(packet);
		}
	}

	void sendPacket(PacketTCP* packet, const CharacterPlayer* player0, const CharacterPlayer* player1)
	{
		if (player0 == nullptr && player1 == nullptr)
		{
			return;
		}
		mTCPServerSystem->writePacket(packet);
		if (player0 != nullptr)
		{
			player0->getClient()->sendPacket(packet);
		}
		if (player1 != nullptr)
		{
			player1->getClient()->sendPacket(packet);
		}
	}

	void sendPacket(PacketTCP* packet, const CharacterPlayer* player)
	{
		if (player == nullptr)
		{
			return;
		}
		mTCPServerSystem->writePacket(packet);
		player->getClient()->sendPacket(packet);
	}

	void sendPacket(const ushort packetType, const string& name, const CharacterPlayer* player)
	{
		if (player == nullptr)
		{
			return;
		}
		player->getClient()->sendPacket(packetType, name);
	}

	//-------------------------------------------------------------------------------------------------------------------------------
	void getEquipObject(ArrayList<(int)EQUIP_POSITION::MAX, ObjectItem*>& equipList, const llong instanceID, const HashMap<llong, MDObjectItemEquip*>& equipDataList)
	{
		if (instanceID == 0)
		{
			return;
		}
		// 先尝试查找,找不到再根据数据库的数据创建
		ObjectItem* item = mObjectItemManager->getObject(instanceID);
		if (item == nullptr)
		{
			const MDObjectItemEquip* equipData = equipDataList.tryGet(instanceID);
			if (equipData == nullptr)
			{
				ERROR("找不到装备数据");
				return;
			}
			item = mObjectItemManager->createItem(equipData, equipData->mObjectID);
		}
		equipList.add(item);
	}

	void initAngle()
	{
		if (mDirectionVector8.size() == 0)
		{
			mDirectionVector8.reserve(8);
			mDirectionVector8.push_back(normalize(Vector3(0.0f, GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(GD::TILE_WIDTH, GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(GD::TILE_WIDTH, 0.0f, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(GD::TILE_WIDTH, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(0.0f, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(-GD::TILE_WIDTH, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(-GD::TILE_WIDTH, 0.0f, 0.0f)));
			mDirectionVector8.push_back(normalize(Vector3(-GD::TILE_WIDTH, GD::TILE_HEIGHT, 0.0f)));
		}
		if (mDirectionVector16.size() == 0)
		{
			mDirectionVector16.reserve(16);
			mDirectionVector16.push_back(normalize(Vector3(0.0f, GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH, GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH, GD::TILE_HEIGHT >> 1, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH, 0.0f, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH, -(GD::TILE_HEIGHT >> 1), 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(GD::TILE_WIDTH >> 1, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(0.0f, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH >> 1, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH, -GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH, -(GD::TILE_HEIGHT >> 1), 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH, 0.0f, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH, GD::TILE_HEIGHT >> 1, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH, GD::TILE_HEIGHT, 0.0f)));
			mDirectionVector16.push_back(normalize(Vector3(-GD::TILE_WIDTH >> 1, GD::TILE_HEIGHT, 0.0f)));
		}
		if (mDirectionAngle16.size() == 0)
		{
			mDirectionAngle16.reserve(16);
			for (const Vector3& vec : mDirectionVector16)
			{
				float angle = getAngleFromVector2ToVector2(Vector2::UP, Vector2(vec.x, vec.y));
				clampRadian360(angle);
				mDirectionAngle16.push_back(angle);
			}
		}
	}
}