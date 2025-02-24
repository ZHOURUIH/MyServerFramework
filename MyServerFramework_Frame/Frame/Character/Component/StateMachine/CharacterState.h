#pragma once

#include "ClassPooledObject.h"
#include "FrameDefine.h"
#include "StateParam.h"
#include "StateParamMemberValue.h"

class MICRO_LEGEND_FRAME_API CharacterState : public ClassPooledObject
{
	BASE(CharacterState, ClassPooledObject);
public:
	CharacterState();
	~CharacterState() override;
	// 当前是否可以进入该状态
	virtual bool canEnter(StateParam* param) const { return true; }
	virtual void enter(StateParam* param);
	// 一般在子类的update最后再调用该父类的update,确保在移除状态后不会再执行update
	virtual void update(const float elapsedTime);
	// isBreak表示是否是因为添加了互斥状态而退出的,willDestroy表示是否是销毁而退出的
	virtual void leave(bool isBreak, bool willDestroy, bool removeByDie) {}
	virtual void destroy() {}
	void resetProperty() override;
	virtual void setCharacter(Character* character)	{ mCharacter = character; }
	void setType(ushort type)						{ mType = type; }
	void setActive(bool active)						{ mActive = active; }
	void setStateTime(float time)					{ mStateTime = time; }
	void setOriginStateTime(float time)				{ mOriginStateTime = time; }
	void setLeaving(bool leaving)					{ mLeaving = leaving; }
	void setID(llong id)							{ mID = id; }
	bool isActive() const							{ return mActive; }
	float getStateTime() const						{ return mStateTime; }
	llong getID() const								{ return mID; }
	ushort getType() const							{ return mType; }
	STATE_MUTEX getStateMutex() const				{ return mSuperposition; }
	bool isLeaving() const							{ return mLeaving; }
	bool isBuff() const								{ return mIsBuff; }
	bool isNeedTick() const							{ return mNeedTick || mOriginStateTime >= 0.0f; }
	Character* getCharacter() const					{ return mCharacter; }
	const Vector<StateParamMemberValue>& getParamList() const { return mParamMemberList != nullptr ? *mParamMemberList : Vector<StateParamMemberValue>::mDefaultList; }
	// 计算当前状态的优先级,可能会根据状态的属性进行计算
	virtual int getPriority() const					{ return 0; }
	// 添加即将移除此状态的回调监听
	void addWillRemoveCallback(void* listener, const CharacterStateCallback& callback)
	{
		if (mWillRemoveCallbackList == nullptr)
		{
			mWillRemoveCallbackList = new HashMap<void*, Vector<CharacterStateCallback>>();
		}
		mWillRemoveCallbackList->insertOrGet(listener).push_back(callback); 
	}
	// 移除回调监听,只能移除此监听者的所有监听
	void removeWillRemoveCallback(void* listener) 
	{
		if (mWillRemoveCallbackList != nullptr)
		{
			mWillRemoveCallbackList->erase(listener);
		}
	}
	void callWillRemoveCallback();
protected:
	void removeSelf(bool isBreak = false, bool removeByDie = false);
protected:
	HashMap<void*, Vector<CharacterStateCallback>>* mWillRemoveCallbackList = nullptr;	// 即将销毁此状态时的回调列表,不一定所有状态都需要这个,需要时才创建
	Vector<StateParamMemberValue>* mParamMemberList = nullptr;	// 存储的是添加状态时的参数值的数据备份,仅存储为浮点数或者字符串,不一定所有状态都需要这个,需要时才创建
	Character* mCharacter = nullptr;							// 状态所属角色
	llong mID = 0;												// 状态唯一ID
	float mOriginStateTime = -1.0f;								// 添加状态时设置的状态持续时间,做一个备份,用于判断是否基类需要tick,当基类需要持续一定时长时,需要进行tick,无限时长则不需要tick
	float mStateTime = -1.0f;									// 该状态持续的时间,小于0表示无限制
	ushort mType = 0;											// 状态类型枚举
	bool mActive = true;										// 是否激活,用于表示状态是否正在使用中,退出一个状态时会将其设置为不激活状态,并非只是控制其tick
	bool mLeaving = false;										// 是否正在退出状态
	bool mIsBuff = false;										// 是否为buff
	bool mNeedTick = false;										// 该状态是否需要每帧执行update,在子类构造中设置,子类可以不用考虑CharacterState基类中的update,只关心自己是否需要执行update
	STATE_MUTEX mSuperposition = STATE_MUTEX::REMOVE_OLD;		// 该状态是否允许叠加
public:
	short mLine = 0;											// 销毁此对象的代码所在行号
};