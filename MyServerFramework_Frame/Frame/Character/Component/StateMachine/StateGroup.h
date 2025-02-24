#pragma once

#include "FrameDefine.h"
#include "Set.h"

// 状态组,状态组可以指定同一组中状态的互斥类型
class MICRO_LEGEND_FRAME_API StateGroup
{
public:
	virtual ~StateGroup() = default;
	void setType(ushort type)					{ mType = type; }
	void setMutex(GROUP_MUTEX mutexOperation)	{ mMutex = mutexOperation; }
	ushort getType() const						{ return mType; }
	GROUP_MUTEX getMutex() const				{ return mMutex;	}
	bool hasState(ushort state) const			{ return mStateList.contains(state); }
	bool isMainState(ushort state) const		{ return mMainStateList.contains(state); }
	void addState(ushort state, bool mainState)
	{
		mStateList.insert(state);
		if (mainState)
		{
			mMainStateList.insert(state);
		}
	}
public:
	Set<ushort> mMainStateList;					// 状态组中的主状态类型
	Set<ushort> mStateList;						// 状态组中的所有状态
	ushort mType = 0;							// 状态组类型
	GROUP_MUTEX mMutex = GROUP_MUTEX::COEXIST;	// 同一状态组中的状态互斥选项
};