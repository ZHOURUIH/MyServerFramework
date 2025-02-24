#include "GameHeader.h"

CharacterGameData::~CharacterGameData()
{
	delete mPercentIncreaseList;
	mPercentIncreaseList = nullptr;
	delete mFixedIncreaseList;
	mFixedIncreaseList = nullptr;
}

void CharacterGameData::resetProperty()
{
	if (mPercentIncreaseList != nullptr)
	{
		FOR_VECTOR(*mPercentIncreaseList)
		{
			(*mPercentIncreaseList)[i] = 0.0f;
		}
	}
	if (mFixedIncreaseList != nullptr)
	{
		FOR_VECTOR(*mFixedIncreaseList)
		{
			(*mFixedIncreaseList)[i] = 0.0f;
		}
	}
	mTargetGUID = 0;
	mTargetTimeStamp = 0;
	mScene = nullptr;
	mMoveVector.clear();
	mTilePosition.clear();
	mSceneBlock = -1;
	mMaxHP = 0;
	mMaxMP = 0;
	mLastHP = 0;
	mHP = 0;
	mMP = 0;
	mMinPhysicsAttack = 0;
	mMaxPhysicsAttack = 0;
	mMinMagicAttack = 0;
	mMaxMagicAttack = 0;
	mMinSorceryAttack = 0;
	mMaxSorceryAttack = 0;
	mMinPhysicsDefence = 0;
	mMaxPhysicsDefence = 0;
	mMinMagicDefence = 0;
	mMaxMagicDefence = 0;
	mHPRecover = 0;
	mMPRecover = 0;
	mAttackSpeed = 0.0f;
	mMoveSpeed = 0.0f;
	mHitProbability = 0.0f;
	mEvadeProbability = 0.0f;
	mCriticalProbability = 0.0f;
	mCriticalDamageRate = 0.0f;
	mDecreaseDamageRate = 0.0f;
	mIncreaseDamageRate = 0.0f;
	mPhysicsDecreaseRate = 0.0f;
	mMagicDecreaseRate = 0.0f;
	mSorceryDecreaseRate = 0.0f;
	mSkillAttackIncrease = 0.0f;
	mSkillCDDecrease = 0.0f;
	mAntiCritical = 0.0f;
	mCounterAttack = 0.0f;
	mCounterPhysicsAttack = 0.0f;
	mCounterMagicAttack = 0.0f;
	mCounterSorceryAttack = 0.0f;
	mNoDefencePhysicsAttack = 0;
	mNoDefenceMagicAttack = 0;
	mNoDefenceSorceryAttack = 0;
	mActionStateID = 0;
	mLevel = 0;
	mLucky = 0;
	mDirection = 0;
	mIsDying = false;
	mPropertyDirty = false;
	mMoveState = PLAYER_MOVE_STATE::NONE;
}

void CharacterGameData::refreshProperty(CharacterGame* character)
{
	mPropertyDirty = false;
	int originMaxHP = mMaxHP;
	if (mFixedIncreaseList != nullptr)
	{
		// 先计算固定增幅,为了效率考虑,此处不使用循环
		mMaxHP = clampZero(mMaxHP + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_HP]);
		mMaxMP = clampZero(mMaxMP + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_MP]);
		mMinPhysicsAttack = clampZero(mMinPhysicsAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MIN_PHYSICS_ATTACK]);
		mMaxPhysicsAttack = clampZero(mMaxPhysicsAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_PHYSICS_ATTACK]);
		mMinMagicAttack = clampZero(mMinMagicAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MIN_MAGIC_ATTACK]);
		mMaxMagicAttack = clampZero(mMaxMagicAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_MAGIC_ATTACK]);
		mMinSorceryAttack = clampZero(mMinSorceryAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MIN_SORCERY_ATTACK]);
		mMaxSorceryAttack = clampZero(mMaxSorceryAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_SORCERY_ATTACK]);
		mMinPhysicsDefence = clampZero(mMinPhysicsDefence + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MIN_PHYSICS_DEFENCE]);
		mMaxPhysicsDefence = clampZero(mMaxPhysicsDefence + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_PHYSICS_DEFENCE]);
		mMinMagicDefence = clampZero(mMinMagicDefence + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MIN_MAGIC_DEFENCE]);
		mMaxMagicDefence = clampZero(mMaxMagicDefence + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_MAGIC_DEFENCE]);
		mAttackSpeed = clampZero(mAttackSpeed + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::ATTACK_SPEED]);
		mMoveSpeed = clampZero(mMoveSpeed + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MOVE_SPEED]);
		mHitProbability = clampZero(mHitProbability + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::HIT_PROBABILITY]);
		mEvadeProbability = clampZero(mEvadeProbability + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::EVADE_PROBABILITY]);
		mCriticalProbability = clampZero(mCriticalProbability + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::CRITICAL_PROBABILITY]);
		mCriticalDamageRate = clampZero(mCriticalDamageRate + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::CRITICAL_DAMAGE_RATE]);
		mDecreaseDamageRate = clampZero(mDecreaseDamageRate + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::DECREASE_DAMAGE_RATE]);
		mIncreaseDamageRate = clampZero(mIncreaseDamageRate + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::INCREASE_DAMAGE_RATE]);
		mLucky = clampZero(mLucky + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::LUCKY]);
		mHPRecover = clampZero(mHPRecover + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::HP_RECOVER]);
		mMPRecover = clampZero(mMPRecover + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MP_RECOVER]);
		mPhysicsDecreaseRate = clampZero(mPhysicsDecreaseRate + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::PHYSICS_DECREASE]);
		mMagicDecreaseRate = clampZero(mMagicDecreaseRate + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAGIC_DECREASE]);
		mSorceryDecreaseRate = clampZero(mSorceryDecreaseRate + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::SORCERY_DECREASE]);
		mSkillAttackIncrease = clampZero(mSkillAttackIncrease + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::SKILL_ATTACK_INCREASE]);
		mAntiCritical = clampZero(mAntiCritical + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::ANTI_CRITICAL]);
		mCounterAttack = clampZero(mCounterAttack + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_ATTACK]);
		mCounterPhysicsAttack = clampZero(mCounterPhysicsAttack + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_PHYSICS_ATTACK]);
		mCounterMagicAttack = clampZero(mCounterMagicAttack + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_MAGIC_ATTACK]);
		mCounterSorceryAttack = clampZero(mCounterSorceryAttack + (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_SORCERY_ATTACK]);
		mNoDefencePhysicsAttack = clampZero(mNoDefencePhysicsAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::NO_DEFENCE_PHYSICS_ATTACK]);
		mNoDefenceMagicAttack = clampZero(mNoDefenceMagicAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::NO_DEFENCE_MAGIC_ATTACK]);
		mNoDefenceSorceryAttack = clampZero(mNoDefenceSorceryAttack + (int)(*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::NO_DEFENCE_SORCERY_ATTACK]);
	}

	// 再计算百分比增幅,不能出现负数
	if (mPercentIncreaseList != nullptr)
	{
		mMaxHP = (int)(mMaxHP * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_HP] + 1.0f));
		mMaxMP = (int)(mMaxMP * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_MP] + 1.0f));
		mMinPhysicsAttack = (int)(mMinPhysicsAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MIN_PHYSICS_ATTACK] + 1.0f));
		mMaxPhysicsAttack = (int)(mMaxPhysicsAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_PHYSICS_ATTACK] + 1.0f));
		mMinMagicAttack = (int)(mMinMagicAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MIN_MAGIC_ATTACK] + 1.0f));
		mMaxMagicAttack = (int)(mMaxMagicAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_MAGIC_ATTACK] + 1.0f));
		mMinSorceryAttack = (int)(mMinSorceryAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MIN_SORCERY_ATTACK] + 1.0f));
		mMaxSorceryAttack = (int)(mMaxSorceryAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_SORCERY_ATTACK] + 1.0f));
		mMinPhysicsDefence = (int)(mMinPhysicsDefence * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MIN_PHYSICS_DEFENCE] + 1.0f));
		mMaxPhysicsDefence = (int)(mMaxPhysicsDefence * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_PHYSICS_DEFENCE] + 1.0f));
		mMinMagicDefence = (int)(mMinMagicDefence * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MIN_MAGIC_DEFENCE] + 1.0f));
		mMaxMagicDefence = (int)(mMaxMagicDefence * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_MAGIC_DEFENCE] + 1.0f));
		mAttackSpeed = mAttackSpeed * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::ATTACK_SPEED] + 1.0f);
		mMoveSpeed = mMoveSpeed * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MOVE_SPEED] + 1.0f);
		mHitProbability = mHitProbability * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::HIT_PROBABILITY] + 1.0f);
		mEvadeProbability = mEvadeProbability * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::EVADE_PROBABILITY] + 1.0f);
		mCriticalProbability = mCriticalProbability * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::CRITICAL_PROBABILITY] + 1.0f);
		mCriticalDamageRate = mCriticalDamageRate * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::CRITICAL_DAMAGE_RATE] + 1.0f);
		mDecreaseDamageRate = mDecreaseDamageRate * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::DECREASE_DAMAGE_RATE] + 1.0f);
		mIncreaseDamageRate = mIncreaseDamageRate * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::INCREASE_DAMAGE_RATE] + 1.0f);
		mLucky = (int)(mLucky * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::LUCKY] + 1.0f));
		mHPRecover = (int)(mHPRecover * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::HP_RECOVER] + 1.0f));
		mMPRecover = (int)(mMPRecover * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MP_RECOVER] + 1.0f));
		mPhysicsDecreaseRate = mPhysicsDecreaseRate * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::PHYSICS_DECREASE] + 1.0f);
		mMagicDecreaseRate = mMagicDecreaseRate * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAGIC_DECREASE] + 1.0f);
		mSorceryDecreaseRate = mSorceryDecreaseRate * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::SORCERY_DECREASE] + 1.0f);
		mSkillAttackIncrease = mSkillAttackIncrease * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::SKILL_ATTACK_INCREASE] + 1.0f);
		mAntiCritical = mAntiCritical * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::ANTI_CRITICAL] + 1.0f);
		mCounterAttack = mCounterAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_ATTACK] + 1.0f);
		mCounterPhysicsAttack = mCounterPhysicsAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_PHYSICS_ATTACK] + 1.0f);
		mCounterMagicAttack = mCounterMagicAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_MAGIC_ATTACK] + 1.0f);
		mCounterSorceryAttack = mCounterSorceryAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::COUNTER_SORCERY_ATTACK] + 1.0f);
		mNoDefencePhysicsAttack = (int)(mNoDefencePhysicsAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::NO_DEFENCE_PHYSICS_ATTACK] + 1.0f));
		mNoDefenceMagicAttack = (int)(mNoDefenceMagicAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::NO_DEFENCE_MAGIC_ATTACK] + 1.0f));
		mNoDefenceSorceryAttack = (int)(mNoDefenceSorceryAttack * clampZero((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::NO_DEFENCE_SORCERY_ATTACK] + 1.0f));
	}
	// 当血量上限有变化时,发送事件,仅限玩家
	if (originMaxHP != mMaxHP && character->isPlayer())
	{
		mEventSystem->pushEvent(EventMaxHPChanged(), character->getGUID());
	}
	if (mMaxHP <= 0)
	{
		ERROR_PROFILE(("血量小于0, originMaxHP:" + IToS(originMaxHP) +
			", (*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_HP]:" + FToS((*mFixedIncreaseList)[(int)CHARACTER_PROPERTY::MAX_HP]) +
			", (*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_HP]:" + FToS((*mPercentIncreaseList)[(int)CHARACTER_PROPERTY::MAX_HP])).c_str());
	}
}