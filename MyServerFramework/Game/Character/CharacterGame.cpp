#include "GameHeader.h"

CharacterGame::~CharacterGame()
{
	if (mCanBeHitCallbackList != nullptr)
	{
		delete mCanBeHitCallbackList;
		mCanBeHitCallbackList = nullptr;
	}
}

void CharacterGame::initComponents()
{
	base::initComponents();
	mStateMachine = addComponentAtIndex<COMCharacterStateMachine>();
}

bool CharacterGame::canBeHit(const CharacterGame* attacker, const CharacterSkill* skill, const SkillBullet* bullet) const
{
	if (mCanBeHitCallbackList == nullptr || mCanBeHitCallbackList->size() == 0)
	{
		return true;
	}
	for (const auto& iter : *mCanBeHitCallbackList)
	{
		if (!iter.second(attacker, skill, bullet, iter.first))
		{
			return false;
		}
	}
	return true;
}

void CharacterGame::resetProperty()
{
	base::resetProperty();
	if (mCanBeHitCallbackList != nullptr)
	{
		mCanBeHitCallbackList->clear();
	}
	mStateMachine = nullptr;
	mComHPRecover = nullptr;
	mComMPRecover = nullptr;
	mComSummon = nullptr;
	// mData在子类构造中赋值,不重置
	//mData = nullptr;
}

inline COMCharacterSummon* CharacterGame::getComSummon(bool createIfNull)
{
	if (mComSummon == nullptr && createIfNull)
	{
		mComSummon = addComponentAtIndex<COMCharacterSummon>();
	}
	return mComSummon;
}

inline bool CharacterGame::isInMultiPlayerSceneCopy() const
{
	return mData->mScene != nullptr && mData->mScene->getSceneCopy() != nullptr && mData->mScene->getSceneCopy()->isMultiPlayerSceneCopy();
}

inline SceneCopy* CharacterGame::getSceneCopy() const
{
	return mData->mScene != nullptr ? mData->mScene->getSceneCopy() : nullptr;
}

inline SCENE_TYPE CharacterGame::getSceneType() const
{
	return mData->mScene != nullptr ? mData->mScene->getSceneType() : SCENE_TYPE::NONE;
}

void CharacterGame::addPropertyValue(const CHARACTER_PROPERTY type, const float value)
{
	if (isZero(value))
	{
		return;
	}
	if (mData->mFixedIncreaseList == nullptr)
	{
		mData->mFixedIncreaseList = new Vector<float>();
		mData->mFixedIncreaseList->resize(CharacterGameData::INSREASE_LIST_SIZE);
	}
	(*mData->mFixedIncreaseList)[(int)type] += value;
	mData->mPropertyDirty = true;
}

void CharacterGame::addPropertyPercent(const CHARACTER_PROPERTY type, const float percent)
{
	if (isZero(percent))
	{
		return;
	}
	if (mData->mPercentIncreaseList == nullptr)
	{
		mData->mPercentIncreaseList = new Vector<float>();
		mData->mPercentIncreaseList->resize(CharacterGameData::INSREASE_LIST_SIZE);
	}
	(*mData->mPercentIncreaseList)[(int)type] += percent;
	mData->mPropertyDirty = true;
}

void CharacterGame::setPosition(const Vector3& pos)
{
	base::setPosition(pos);
	if (mData->mScene != nullptr)
	{
		mData->mTilePosition = mData->mScene->getMapData()->pixelPosToTilePos(pos);
	}
}