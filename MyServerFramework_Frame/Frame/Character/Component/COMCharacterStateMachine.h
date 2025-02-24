#pragma once

#include "GameComponent.h"
#include "FrameStringDefine.h"
#include "FrameDefine.h"
#include "SafeVector.h"

class MICRO_LEGEND_FRAME_API COMCharacterStateMachine : public GameComponent
{
	BASE(COMCharacterStateMachine, GameComponent);
public:
	static ushort staticType() { return FrameStringDefine::COMCharacterStateMachine; }
	void destroy() override;
	void resetProperty() override;
	bool canAddState(ushort type, StateParam* param) const;
	CharacterState* addState(ushort type, StateParam* param, float stateTime, bool removeByDie);
	// isBreak表示是否是因为与要添加的状态冲突,所以才移除的状态
	bool removeState(CharacterState* state, bool isBreak, bool removeByDie);
	void removeStateType(ushort type, bool isBreak, bool removeByDie);
	// 移除一个状态组中的所有状态
	void removeStateInGroup(int group, bool isBreak, bool removeByDie);
	CharacterState* getFirstState(ushort type) const;
	// 是否拥有该组的任意一个状态
	bool hasStateGroup(int group)const;
	bool hasState(ushort type) const;
	SafeVector<CharacterState*>* getState(ushort type)								{ return mStateTypeList.getPtr(type); }
	const HashMap<ushort, SafeVector<CharacterState*>>& getStateList() const		{ return mStateTypeList; }
protected:
	static void leaveStateInternal(CharacterState* state, bool isBreak, bool willDestroy, bool removeByDie);
	static void destroyState(CharacterState* state, int line);
	static void destroyStateList(const Vector<CharacterState*>& stateList, int line);
	bool canAddState(CharacterState* state, StateParam* param, CharacterState** needRemoveState) const;
	// 是否因为与当前已存在的同类型的互斥而无法添加此状态,返回值表示是否可以添加新的状态,如果可添加,则needRemoveState返回需要被移除的状态
	bool mutexWithExistState(CharacterState* state, CharacterState** needRemoveState) const;
	// 添加了newState以后是否还会保留existState
	static bool allowKeepStateByGroup(ushort newState, ushort existState);
	static bool allowKeepStateByGroupInternal(ushort newState, ushort existState);
	// 当存在existState时,是否允许添加newState
	static bool allowAddStateByGroup(ushort newState, ushort existState);
	static bool allowAddStateByGroupInternal(ushort newState, ushort existState);
	static void initAllowKeepTypeList();
	static void initAllowAddTypeList();
	void registeTick() override { mComponentOwner->registeFrameTick(this, frameTick); }
	void unregisteTick() override { mComponentOwner->unregisteFrameTick(this, frameTick); }
	static void frameTick(GameComponent* component, float elapsedTime) { static_cast<This*>(component)->onFrameTick(elapsedTime); }
private:
	void onFrameTick(float elapsedTime);
protected:
	HashMap<ushort, SafeVector<CharacterState*>> mStateTypeList;	// 以状态类型为索引的状态列表
	SafeVector<CharacterState*> mStateTickList;						// 以状态唯一ID为索引的列表,用来更新遍历,只存储需要调用update的状态
	HashMap<short, short> mGroupStateCountList;						// 每个状态组中的状态数量
	short mStateTickCount = 0;										// mStateTickList的元素个数,单独使用一个变量记录,比mStateTickList.size()访问更快
private:
	static ushort* mAllowAddTypeCountList;							// 以类型为下标,记录此类型有多少个互斥的类型
	static ushort* mAllowKeepTypeCountList;							// 以类型为下标,记录此类型有多少个允许共存的类型
	static HashMap<uint, bool> mAllowAddTypeList;					// allowAddStateByGroupInternal预先计算出来的列表,可以根据类型索引,避免运行时做过多的判断,key是两个类型的字节组合
	static HashMap<uint, bool> mAllowKeepTypeList;					// allowKeepStateByGroupInternal预先计算出来的列表,可以根据类型索引,避免运行时做过多的判断,key是两个类型的字节组合
};