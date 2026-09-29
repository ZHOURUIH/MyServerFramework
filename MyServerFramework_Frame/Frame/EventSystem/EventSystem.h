#pragma once

#include "FrameComponent.h"
#include "FrameCallback.h"

// 事件系统,一个逻辑的处理非常分散时,则需要写为事件的方式
class MICRO_LEGEND_FRAME_API EventSystem : public FrameComponent
{
	BASE(EventSystem, FrameComponent);
public:
	void quit() override;
	void update(float elapsedTime) override;
	template<typename EventType, typename TypeCheck = typename IsSubClassOf<GameEvent, EventType>::mType>
	void pushEvent(const EventType& param, const Character* character = nullptr)
	{
		pushEventInternal(const_cast<EventType*>(&param), character);
	}
	// 监听指定玩家的该事件,ListenerType需要定义有函数void onEvent(EventType* event)
	template<typename EventType, typename ListenerType, typename TypeCheck0 = typename IsSubClassOf<GameEvent, EventType>::mType, typename TypeCheck1 = typename IsSubClassOf<IEventListener, ListenerType>::mType>
	void listenEvent(const Character* character, ListenerType* listener)
	{
		const int eventType = (int)typeid(EventType).hash_code();
		listenerEventInternal(&EventProcess<EventType, ListenerType>::call, character, eventType, listener);
	}
	// 监听所有玩家的该事件,ListenerType需要定义有函数void onEvent(EventType* event)
	template<typename EventType, typename ListenerType, typename TypeCheck0 = typename IsSubClassOf<GameEvent, EventType>::mType, typename TypeCheck1 = typename IsSubClassOf<IEventListener, ListenerType>::mType>
	void listenEvent(ListenerType* listener)
	{
		const int eventType = (int)typeid(EventType).hash_code();
		listenerEventInternal(&EventProcess<EventType, ListenerType>::call, nullptr, eventType, listener);
	}
	// 注销该监听者的所有消息
	void unlistenEvent(IEventListener* listener);
	// 角色销毁时需要移除此角色的所有事件监听
	void removeCharacterEvent(llong characterID);
protected:
	void pushEventInternal(GameEvent* param, const Character* character);
	void listenerEventInternal(EventCallback callback, const Character* character, int eventType, IEventListener* listener);
protected:
	HashMap<llong, HashMap<int, SafeFastDeepVector<EventInfo*>>> mCharacterEventList;	// 指定角色的事件监听列表
	HashMap<IEventListener*, Vector<EventInfo*>> mListenerEventMap;				// 存储每一个监听者的所有的事件列表
	HashMap<int, SafeFastDeepVector<EventInfo*>> mGlobalEventList;						// 全局事件监听列表
	Vector<EventInfo*> mTempEventList;											// 用于临时遍历的列表
	int mDispatchDepth = 0;														// 当前事件嵌套的深度,用于判断事件的嵌套次数是否过多或者循环嵌套的问题
	const int MAX_DEPTH = 20;													// 事件嵌套的最大深度
	bool mNeedCheckEmptyEvent = false;											// 是否需要检测有没有空的角色事件监听列表
};