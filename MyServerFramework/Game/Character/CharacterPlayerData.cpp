#include "GameHeader.h"

CharacterPlayerData::~CharacterPlayerData()
{
	if (mUnlockMap != nullptr)
	{
		delete mUnlockMap;
		mUnlockMap = nullptr;
	}
}

void CharacterPlayerData::resetProperty()
{
	base::resetProperty();
	if (mUnlockMap != nullptr)
	{
		mUnlockMap->clear();
	}
	// 构造时赋值,不重置
	//mPlayer = nullptr;
	mClient = nullptr;
	mPKModeInstance = nullptr;
	mSecondaryPassword.clear();
	mPropertyDirtyFlag = 0;
	mLoginTimeInWeek = 0;
	mLastLogout = 0;
	mCreateTime = 0;
	mLastLogin = 0;
	mLoginTime = 0;
	mLastSearchAuctionTime = 0;
	mLastSecondaryPasswordTime = 0;
	mLastReceiveGuildSalaryTime = 0;
	mPayExpiration = 0;
	mExp = 0;
	mMaxExp = 0;
	mLastWriteExp = 0;
	mHumanLucky = 0.0f;
	mLifeItemHumanLucky = 0.0f;
	mExtraHumanLucky = 0.0f;
	mActivityHumanLucky = 0.0f;
	mBossKillingHumanLucky = 0.0f;
	mRealtimeSpeed = 0.0f;
	mDigMineSpeed = 0.0f;
	mMultiMonsterExp = 1.0f;
	mMultiSkillExp = 1.0f;
	mMultiPickGold = 1.0f;
	mMultiPickGoldIngot = 1.0f;
	mSellGoldDecrease = 0.0f;
	mMineTime = 0.0f;
	mCrime = 0.0f;
	mAccountGUID = 0;
	mQuickSellCount = 0;
	mQuickDecomposeCount = 0;
	mAuctionDiscount = 0;
	mBossDropProtectCount = 0;
	mExchangeID = 0;
	mMaxGold = 0;
	mMaxCreateGold = 0;
	mLifeItem = 0;
	mPKArenaScore = 0;
	mDefaultMapID = 0;
	mNotAllowUseConsumable = 0;
	mInverseHPMedicine = 0;
	mMoveDirection = 0;
	mSuperPermission = SUPER_PERMISSION::NO;
	mSex = PLAYER_SEX::MALE;
	mJob = PLAYER_JOB::FIGHTER;
	mPKMode = PK_MODE::MAX;
	mSecondaryPasswordVerify = false;
	mEnterMapFinish = false;
	mLoginFinish = false;
	mPackChanged = false;
	mPackOnlyPropertyChanged = false;
}

void CharacterPlayerData::refreshPlayerProperty()
{
	// 计算等级的基础属性
	COMPlayerBaseProperty* baseProperty = mPlayer->getComBaseProperty();
	mMaxHP = generateMaxHP(mLevel, mJob, baseProperty->mBaseMaxHP);
	mMaxMP = generateMaxMP(mLevel, mJob, baseProperty->mBaseMaxMP);
	mMinMagicAttack = generateSimpleMagicAttack(mJob, mLevel, baseProperty->mBaseMagicAttack);
	mMaxMagicAttack = mMinMagicAttack;
	mMinPhysicsAttack = generateSimplePhysicsAttack(mJob, mLevel, baseProperty->mBasePhysicsAttack);
	mMaxPhysicsAttack = mMinPhysicsAttack;
	mMinSorceryAttack = generateSimpleSorceryAttack(mJob, mLevel, baseProperty->mBaseSorceryAttack);
	mMaxSorceryAttack = mMinSorceryAttack;
	mMinMagicDefence = generateSimpleMagicDefence(mLevel, baseProperty->mBaseMagicDefence);
	mMaxMagicDefence = mMinMagicDefence;
	mMinPhysicsDefence = generateSimplePhysicsDefence(mLevel, baseProperty->mBasePhysicsDefence);
	mMaxPhysicsDefence = mMinPhysicsDefence;
	mHPRecover = 0;
	mMPRecover = 0;
	mAttackSpeed = 1.0f + baseProperty->mBaseAttackSpeed;
	mMoveSpeed = 1.0f + baseProperty->mBaseMoveSpeed;
	mHitProbability = baseProperty->mBaseHitProbability;
	mEvadeProbability = baseProperty->mBaseEvadeProbability;
	mCriticalProbability = baseProperty->mBaseCriticalProbability;
	mCriticalDamageRate = baseProperty->mBaseCriticalDamageRate;
	mAntiCritical = baseProperty->mBaseAntiCritical;
	mLucky = 0;
	mDecreaseDamageRate = 0.0f;
	mIncreaseDamageRate = 0.0f;
	mPhysicsDecreaseRate = 0.0f;
	mMagicDecreaseRate = 0.0f;
	mSorceryDecreaseRate = 0.0f;
	mSkillAttackIncrease = 0.0f;
	mCounterAttack = 0.0f;
	mCounterPhysicsAttack = 0.0f;
	mCounterMagicAttack = 0.0f;
	mCounterSorceryAttack = 0.0f;
	mNoDefencePhysicsAttack = 0;
	mNoDefenceMagicAttack = 0;
	mNoDefenceSorceryAttack = 0;
	mMultiMonsterExp = 1.0f;
	mMultiSkillExp = 1.0f;
	mMultiPickGold = 1.0f;
	mMultiPickGoldIngot = 1.0f;
	mDigMineSpeed = 0.0f;
	int packMaxWeight = GD::PACK_DEFAULT_WEIGHT;
	refreshHumanLucky();

	// 计算装备属性
	const COMPlayerEquip* playerEquip = mPlayer->getComEquip();
	for (const auto& iter : playerEquip->getEquipList())
	{
		const ObjectItemEquipData* data = iter.second->getEquipData();
		// 持久为0或者被禁用的装备将不会计算属性
		if ((isZero(data->mDura) && data->mDuraMax > 0.0f) || !playerEquip->isEquipEnable(data->mEquipTableData->mEquipType))
		{
			continue;
		}
		// 基础属性
		mMaxHP += data->mMaxHP;
		mMaxMP += data->mMaxMP;
		mMinPhysicsAttack += data->mMinPhysicsAttack;
		mMaxPhysicsAttack += data->mMaxPhysicsAttack;
		mMinMagicAttack += data->mMinMagicAttack;
		mMaxMagicAttack += data->mMaxMagicAttack;
		mMinSorceryAttack += data->mMinSorceryAttack;
		mMaxSorceryAttack += data->mMaxSorceryAttack;
		mMinPhysicsDefence += data->mMinPhysicsDefence;
		mMaxPhysicsDefence += data->mMaxPhysicsDefence;
		mMinMagicDefence += data->mMinMagicDefence;
		mMaxMagicDefence += data->mMaxMagicDefence;
		mHPRecover += data->mHPRecover;
		mMPRecover += data->mMPRecover;
		mAttackSpeed += data->mAttackSpeed;
		mMoveSpeed += data->mMoveSpeed;
		mHitProbability += data->mHitProbability;
		mEvadeProbability += data->mEvadeProbability;
		mCriticalProbability += data->mCriticalProbability;
		mCriticalDamageRate += data->mCriticalDamageRate;
		mIncreaseDamageRate += data->mIncreaseDamage;
		mDecreaseDamageRate += data->mDecreaseDamage;
		mAntiCritical += data->mAntiCritical;
		mPhysicsDecreaseRate += data->mPhysicsDecreaseRate;
		mMagicDecreaseRate += data->mMagicDecreaseRate;
		mSorceryDecreaseRate += data->mSorceryDecreaseRate;
		mDigMineSpeed += data->mDigMineSpeed;
		mSkillAttackIncrease += data->mSkillAttackIncrease;
		mCounterAttack += data->mCounterAttack;
		mCounterPhysicsAttack += data->mCounterPhysicsAttack;
		mCounterMagicAttack += data->mCounterMagicAttack;
		mCounterSorceryAttack += data->mCounterSorceryAttack;
		mNoDefencePhysicsAttack += data->mNoDefencePhysicsAttack;
		mNoDefenceMagicAttack += data->mNoDefenceMagicAttack;
		mNoDefenceSorceryAttack += data->mNoDefenceSorceryAttack;
		mMultiMonsterExp += data->mMonsterExpMulti;
		mMultiSkillExp += data->mSkillExpMulti;
		mMultiPickGold += data->mGoldMulti;
		mMultiPickGoldIngot += data->mGoldIngotMulti;
		mLucky += data->mLucky;
		packMaxWeight += data->mPackMaxWeight;
	}

	// 套装的基础属性
	for (const int suitID : playerEquip->getSuitIDList())
	{
		const EquipSuit* suitData = mObjectItemManager->getEquipSuit(suitID);
		const TDEquipSuit* suitSQLiteData = suitData->getSuitData();
		mMaxHP += suitSQLiteData->mMaxHP;
		mMaxMP += suitSQLiteData->mMaxMP;
		mMinPhysicsAttack += suitSQLiteData->mPhysicsAttack;
		mMaxPhysicsAttack += suitSQLiteData->mPhysicsAttack;
		mMinMagicAttack += suitSQLiteData->mMagicAttack;
		mMaxMagicAttack += suitSQLiteData->mMagicAttack;
		mMinSorceryAttack += suitSQLiteData->mSorceryAttack;
		mMaxSorceryAttack += suitSQLiteData->mSorceryAttack;
		mMinPhysicsDefence += suitSQLiteData->mPhysicsDefence;
		mMaxPhysicsDefence += suitSQLiteData->mPhysicsDefence;
		mMinMagicDefence += suitSQLiteData->mMagicDefence;
		mMaxMagicDefence += suitSQLiteData->mMagicDefence;
		mHPRecover += suitSQLiteData->mHPRecover;
		mMPRecover += suitSQLiteData->mMPRecover;
		mAttackSpeed += suitSQLiteData->mAttackSpeed;
		mMoveSpeed += suitSQLiteData->mMoveSpeed;
		mHitProbability += suitSQLiteData->mHitProbability;
		mEvadeProbability += suitSQLiteData->mEvadeProbability;
		mCriticalProbability += suitSQLiteData->mCriticalProbability;
		mCriticalDamageRate += suitSQLiteData->mCriticalDamageRate;
		mAntiCritical += suitSQLiteData->mAntiCritical;
		mPhysicsDecreaseRate += suitSQLiteData->mPhysicsDecrease;
		mMagicDecreaseRate += suitSQLiteData->mMagicDecrease;
		mSorceryDecreaseRate += suitSQLiteData->mSorceryDecrease;
		mSkillAttackIncrease += suitSQLiteData->mSkillAttackIncrease;
	}

	refreshProperty(mPlayer);
	// 先计算固定增幅
	if (mFixedIncreaseList != nullptr)
	{
		mDigMineSpeed += (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::DIG_MINE_SPEED];
		packMaxWeight += (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::PACK_MAX_WEIGHT];
		mMultiMonsterExp += (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MONSTER_EXP_MULTI];
		mMultiSkillExp += (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::SKILL_EXP_MULTI];
		mMultiPickGold += (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::PICK_GOLD_MULTI];
		mMultiPickGoldIngot += (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::PICK_GOLD_INGOT_MULTI];
	}

	// 再计算百分比增幅,不能出现负数
	if (mPercentIncreaseList != nullptr)
	{
		mDigMineSpeed *= clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::DIG_MINE_SPEED] + 1.0f);
		packMaxWeight = (int)(packMaxWeight * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::PACK_MAX_WEIGHT] + 1.0f));
		mMultiMonsterExp *= clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MONSTER_EXP_MULTI] + 1.0f);
		mMultiSkillExp *= clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::SKILL_EXP_MULTI] + 1.0f);
		mMultiPickGold *= clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::PICK_GOLD_MULTI] + 1.0f);
		mMultiPickGoldIngot *= clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::PICK_GOLD_INGOT_MULTI] + 1.0f);
	}

	// 限定属性范围
	clampRef(mHP, 0, mMaxHP);
	clampRef(mMP, 0, mMaxMP);
	clampRef(mLucky, (char)GD::MIN_LUCKY, (char)GD::MAX_LUCKY);
	clampMinRef(mDigMineSpeed);
	clampMinRef(packMaxWeight);
	clampMinRef(mAttackSpeed, 0.01f);
	clampMinRef(mMoveSpeed, 0.01f);
	clampMinRef(mMultiMonsterExp);
	clampMinRef(mMultiSkillExp);
	clampMinRef(mMultiPickGold);
	clampMinRef(mMultiPickGoldIngot);

	mPlayer->getComPack()->setMaxWeight((float)packMaxWeight);
}

inline void CharacterPlayerData::setPKMode(const PK_MODE mode)
{
	mPKMode = mode;
	mPKModeInstance = mPKModeManager->getPKMode(mode);
}