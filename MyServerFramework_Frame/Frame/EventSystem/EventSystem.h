#pragma once

#include "FrameComponent.h"
#include "SafeVector.h"
#include "HashMap.h"
#include "Vector.h"
#include "EventInfo.h"

// 事件系统,一个逻辑的处理非常分散时,则需要写为事件的方式
class MICRO_LEGEND_FRAME_API EventSystem : public FrameComponent
{
	BASE(EventSystem, FrameComponent);
public:
	void quit() override;
	void update(float elapsedTime) override;
	template<typename T>
	void pushEvent(const T& param, llong characterID = 0)
	{
		pushEventInternal(const_cast<T*>(&param), characterID);
	}
	// 监听指定玩家的该事件
	template<typename EventType, typename ListenerType>
	void listenEvent(const llong characterID, ListenerType* listener)
	{
		const int eventType = (int)typeid(EventType).hash_code();
		EventInfo* info = mEventInfoPool->newClass();
		info->mType = eventType;
		info->mCharacterGUID = characterID;
		info->mEventProcess = EventProcess<EventType, ListenerType>::mProcess;
		info->mListener = listener;
		mListenerEventMap.insertOrGet(listener).push_back(info);
		mCharacterEventList.insertOrGet(characterID).insertOrGet(eventType).push_back(info);
	}
	// 监听所有玩家的该事件
	template<typename EventType, typename ListenerType>
	void listenEvent(ListenerType* listener)
	{
		const int eventType = (int)typeid(EventType).hash_code();
		EventInfo* info = mEventInfoPool->newClass();
		info->mType = eventType;
		info->mCharacterGUID = 0;
		info->mEventProcess = EventProcess<EventType, ListenerType>::mProcess;
		info->mListener = listener;
		mListenerEventMap.insertOrGet(listener).push_back(info);
		mGlobalEventList.insertOrGet(eventType).push_back(info);
	}
	// 注销该监听者的所有消息
	void unlistenEvent(IEventListener* listener);
	// 角色销毁时需要移除此角色的所有事件监听
	void removeCharacterEvent(llong characterID);
protected:
	void pushEventInternal(GameEvent* param, llong characterID);
protected:
	HashMap<llong, HashMap<int, SafeVector<EventInfo*>>> mCharacterEventList;	// 指定角色的事件监听列表
	HashMap<IEventListener*, Vector<EventInfo*>> mListenerEventMap;				// 存储每一个监听者的所有的事件列表
	HashMap<int, SafeVector<EventInfo*>> mGlobalEventList;						// 全局事件监听列表
	Vector<EventInfo*> mTempEventList;											// 用于临时遍历的列表
	bool mNeedCheckEmptyEvent = false;											// 是否需要检测有没有空的角色事件监听列表
};