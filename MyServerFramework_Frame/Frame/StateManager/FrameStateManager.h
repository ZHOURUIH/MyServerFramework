#pragma once

#include "FrameComponent.h"
#include "StateGroup.h"

class MICRO_LEGEND_FRAME_API FrameStateManager : public FrameComponent
{
	BASE(FrameStateManager, FrameComponent);
public:
	void quit() override;
	template<typename T, typename TypeCheck = typename IsSubClassOf<StateGroup, T>::mType>
	void registeGroup(const GROUP_MUTEX groupMutex)
	{
		T* group = new T();
		group->setType(T::staticType());
		group->setMutex(groupMutex);
		mGroupStateList.insert(T::staticType(), group);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<StateGroup, T>::mType>
	void assignGroup(const ushort state, const bool mainState)
	{
		mStateGroupList.insertOrGet(state).push_back(T::staticType());
		if (StateGroup* stateGroup = mGroupStateList.tryGet(T::staticType()))
		{
			stateGroup->addState(state, mainState);
		}
	}
	const HashMap<ushort, Vector<short>>& getStateGroupList() const		{ return mStateGroupList; }
	const Vector<short>& getGroupList(ushort state)						{ return mStateGroupList[state]; }
	const HashMap<short, StateGroup*>& getGroupStateList() const		{ return mGroupStateList; }
	StateGroup* getStateGroup(short group) const						{ return mGroupStateList.tryGet(group); }
protected:
	HashMap<ushort, Vector<short>> mStateGroupList;				// 查找状态所在的所有组,key为状态类型,value为该状态所在的所有状态组
	HashMap<short, StateGroup*> mGroupStateList;				// 查找该组中的所有状态
};