#pragma once

#include "GameEnum.h"

// 为了提高属性的访问速度,不通过虚函数进行访问,而是可以直接从基类的数据类中获取
class CharacterGameData
{
public:
	virtual ~CharacterGameData();
	virtual void resetProperty();
	void refreshProperty(CharacterGame* character);
public:
	static constexpr int INSREASE_LIST_SIZE = 128;
	Vector<float>* mPercentIncreaseList = nullptr;	// 各个属性百分比增幅,比如增加20%的物理攻击力等等
	Vector<float>* mFixedIncreaseList = nullptr;	// 各个属性固定增幅,比如增加20点物理攻击力
	llong mTargetGUID = 0;							// 当前目标,因为通知目标被销毁时不一定是全量通知,所以只是记录一个ID,用到时查询,在能确认目标无效时清空,考虑到效率,此ID无效时不一定会进行通知
	llong mTargetTimeStamp = 0;						// 寻找到当前目标的时间戳,用于实现每隔一定间隔重新寻找一次目标,避免某些情况下怪物总是一直打一个人,不打召唤兽
	SceneMap* mScene = nullptr;						// 所在场景
	Vector3 mMoveVector;							// 当前移动方向,带移动速度
	Vector2Int mTilePosition;						// 角色所在的地砖坐标
	int mSceneBlock = -1;							// 角色在场景中所处的区域下标
	int mMaxHP = 0;									// 当前最大HP
	int mMaxMP = 0;									// 当前最大MP
	int mLastHP = 0;								// 上一次的血量
	int mHP = 0;									// 当前HP
	int mMP = 0;									// 当前MP
	int mMinPhysicsAttack = 0;						// 最小物理攻击力
	int mMaxPhysicsAttack = 0;						// 最大物理攻击力
	int mMinMagicAttack = 0;						// 最小魔法攻击力
	int mMaxMagicAttack = 0;						// 最大魔法攻击力
	int mMinSorceryAttack = 0;						// 最小道术攻击力
	int mMaxSorceryAttack = 0;						// 最大道术攻击力
	int mMinPhysicsDefence = 0;						// 最小物理防御力
	int mMaxPhysicsDefence = 0;						// 最大物理防御力
	int mMinMagicDefence = 0;						// 最小魔法防御力
	int mMaxMagicDefence = 0;						// 最大魔法防御力
	short mHPRecover = 0;							// 每秒HP回复量
	short mMPRecover = 0;							// 每秒MP回复量
	float mAttackSpeed = 0.0f;						// 攻击速度
	float mMoveSpeed = 0.0f;						// 移动速度
	float mHitProbability = 0.0f;					// 命中率
	float mEvadeProbability = 0.0f;					// 闪避率
	float mCriticalProbability = 0.0f;				// 暴击率
	float mCriticalDamageRate = 0.0f;				// 暴击伤害
	float mDecreaseDamageRate = 0.0f;				// 减伤率
	float mIncreaseDamageRate = 0.0f;				// 增伤率
	float mPhysicsDecreaseRate = 0.0f;				// 物理伤害减伤率
	float mMagicDecreaseRate = 0.0f;				// 魔法伤害减伤率
	float mSorceryDecreaseRate = 0.0f;				// 道术伤害减伤率
	float mSkillAttackIncrease = 0.0f;				// 技能攻击力增幅,不会影响技能面板属性,只是在计算伤害时对技能攻击力进行增幅,,如果小于0则是降幅,不能低于-1
	float mSkillCDDecrease = 0.0f;					// 技能CD的减少
	float mAntiCritical = 0.0f;						// 暴击抗性
	float mCounterAttack = 0.0f;					// 破招攻击力增加比率
	float mCounterPhysicsAttack = 0.0f;				// 破招物理攻击力增加比率,物理攻击时会与破招攻击力叠加计算
	float mCounterMagicAttack = 0.0f;				// 破招魔法攻击力增加比率,魔法攻击时会与破招攻击力叠加计算
	float mCounterSorceryAttack = 0.0f;				// 破招道术攻击力增加比率,道术攻击时会与破招攻击力叠加计算
	short mNoDefencePhysicsAttack = 0;				// 无视防御的物理攻击力
	short mNoDefenceMagicAttack = 0;				// 无视防御的魔法攻击力
	short mNoDefenceSorceryAttack = 0;				// 无视防御的道术攻击力
	ushort mActionStateID = 0;						// 存储一个当前的行为状态ID,方便获取
	short mLevel = 0;								// 当前等级
	char mLucky = 0;								// 幸运值
	byte mDirection = 0;							// 角色当前朝向
	bool mIsDying = false;							// 是否正在执行死亡的逻辑,避免递归触发死亡
	bool mPropertyDirty = false;					// 属性是否需要重新计算,当子类的tick中将属性重新计算后,子类需要将此标记设置为false
	PLAYER_MOVE_STATE mMoveState = PLAYER_MOVE_STATE::NONE;	// 当前移动状态
};