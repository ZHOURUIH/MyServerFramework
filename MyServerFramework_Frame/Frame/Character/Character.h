#pragma once

#include "MovableObject.h"

// 不一定所有组件都要单独定义一个下标,部分频繁获取的需要定义,提高获取效率
template<>struct ComponentIndex<COMCharacterStateMachine> { static constexpr int mIndex = 0; };
template<>struct ComponentIndex<COMMonsterDecisionTree> { static constexpr int mIndex = 3; };

class MICRO_LEGEND_FRAME_API Character : public MovableObject
{
	BASE(Character, MovableObject);
public:
	void setGUID(const llong guid)	{ mGUID = guid; }
	void setType(const byte type)	{ mCharacterType = type; }
	byte getType() const			{ return mCharacterType; }
	llong getGUID() const			{ return mGUID; }
	void resetProperty() override;
protected:
	llong mGUID = 0;			// 角色ID
	byte mCharacterType = 0;	// 角色类型,0表示无效值
};