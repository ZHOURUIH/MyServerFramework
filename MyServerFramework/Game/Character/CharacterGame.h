#pragma once

#include "Character.h"
#include "GameCallback.h"
#include "CharacterGameData.h"
#include "GameEnum.h"

// 不一定所有组件都要单独定义一个下标,部分频繁获取的需要定义,提高获取效率
// 公共组件
template<>struct ComponentIndex<COMCharacterSummon>				{ static constexpr int mIndex = 1; };
// 玩家组件
template<>struct ComponentIndex<COMPlayerSkill>					{ static constexpr int mIndex = 2; };
template<>struct ComponentIndex<COMPlayerPack>					{ static constexpr int mIndex = 3; };
template<>struct ComponentIndex<COMPlayerEquip>					{ static constexpr int mIndex = 4; };
template<>struct ComponentIndex<COMPlayerBaseProperty>			{ static constexpr int mIndex = 5; };
template<>struct ComponentIndex<COMPlayerAddupData>				{ static constexpr int mIndex = 6; };
// 怪物组件
template<>struct ComponentIndex<COMMonsterSkill>				{ static constexpr int mIndex = 2; };

class CharacterGame : public Character
{
	BASE(CharacterGame, Character);
public:
	~CharacterGame() override;
	void initComponents() override;
	virtual CharacterSkill* getSkill(int skillID) = 0;
	void resetProperty() override;
	void addCheckBeHit(CanBeHitCallback callback, void* userData)	
	{
		if (mCanBeHitCallbackList == nullptr)
		{
			mCanBeHitCallbackList = new HashMap<void*, CanBeHitCallback>();
		}
		mCanBeHitCallbackList->insert(userData, callback); 
	}
	void removeCheckBeHit(void* userData)							
	{
		if (mCanBeHitCallbackList == nullptr)
		{
			return;
		}
		mCanBeHitCallbackList->erase(userData); 
	}
	bool canBeHit(const CharacterGame* attacker, const CharacterSkill* skill, const SkillBullet* bullet) const;
	CharacterGameData* getCharacterData() const						{ return mData; }
	bool isDead() const												{ return mData->mHP <= 0; }
	Vector2Int getTilePosition() const								{ return mData->mTilePosition; }
	SceneMap* getScene() const										{ return mData->mScene; }
	COMCharacterStateMachine* getStateMachine() const				{ return mStateMachine; }
	COMCharacterHPRecover* getComHPRecover() const					{ return mComHPRecover; }
	COMCharacterMPRecover* getComMPRecover() const					{ return mComMPRecover; }
	COMCharacterSummon* getComSummon(bool createIfNull);
	bool isInMultiPlayerSceneCopy() const;
	SceneCopy* getSceneCopy() const;
	SCENE_TYPE getSceneType() const;
	bool isNPC() const												{ return mCharacterType == CHARACTER_TYPE::NPC; }
	bool isMonster() const											{ return mCharacterType == CHARACTER_TYPE::MONSTER; }
	bool isPlayer() const											{ return mCharacterType == CHARACTER_TYPE::PLAYER; }
	bool isEnvironment() const										{ return mCharacterType == CHARACTER_TYPE::ENVIRONMENT; }
	virtual bool isSummon() const									{ return false; }
	virtual void addPropertyValue(CHARACTER_PROPERTY type, float value);
	virtual void addPropertyPercent(CHARACTER_PROPERTY type, float percent);
	void setPosition(const Vector3& pos) override;
protected:
	HashMap<void*, CanBeHitCallback>* mCanBeHitCallbackList = nullptr;	// 是否可以被攻击的回调注册列表,被攻击时会调用此列表的函数,判断是否有不允许被攻击的情况
	COMCharacterStateMachine* mStateMachine = nullptr;					// 状态机
	COMCharacterHPRecover* mComHPRecover = nullptr;						// 角色HP回复组件
	COMCharacterMPRecover* mComMPRecover = nullptr;						// 角色MP回复组件
	COMCharacterSummon* mComSummon = nullptr;							// 召唤兽组件
	CharacterGameData* mData = nullptr;									// 角色的数据,在子类构造中赋值
};