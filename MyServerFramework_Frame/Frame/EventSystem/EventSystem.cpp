#include "FrameHeader.h"

void EventSystem::quit()
{
	for (auto& item : mListenerEventMap)
	{
		UN_CLASS(item.second);
	}
	mListenerEventMap.clear();
	mCharacterEventList.clear();
	mGlobalEventList.clear();
	mTempEventList.clear();
}

void EventSystem::update(float elapsedTime)
{
	if (!mNeedCheckEmptyEvent)
	{
		return;
	}
	Vector<int> removeEvent;
	Vector<llong> removeCharacterEvent;
	for (auto& itemCharacter : mCharacterEventList)
	{
		auto& characterListenerList = itemCharacter.second;
		if (characterListenerList.isEmpty())
		{
			continue;
		}
		removeEvent.clear();
		for (const auto& itemEvent : characterListenerList)
		{
			if (itemEvent.second.isEmpty())
			{
				removeEvent.add(itemEvent.first);
			}
		}
		if (!removeEvent.isEmpty())
		{
			for (const int eventType : removeEvent)
			{
				characterListenerList.remove(eventType);
			}
			if (characterListenerList.isEmpty())
			{
				removeCharacterEvent.add(itemCharacter.first);
			}
		}
	}
	if (!removeCharacterEvent.isEmpty())
	{
		for (const llong characterGUID : removeCharacterEvent)
		{
			mCharacterEventList.remove(characterGUID);
		}
	}
	mNeedCheckEmptyEvent = false;
}

void EventSystem::pushEventInternal(GameEvent* param, const Character* character)
{
	const llong characterID = character != nullptr ? character->getGUID() : 0;
	const int eventType = param->mType;
	param->mCharacterGUID = characterID;

	if (mDispatchDepth >= MAX_DEPTH)
	{
		ERROR("事件嵌套太深");
		return;
	}
	++mDispatchDepth;
	// 如果是玩家局部的事件,则只发送给玩家的事件监听
	FOR_ONCE
	{
		if (characterID == 0)
		{
			break;
		}
		auto* characterListenerList = mCharacterEventList.getPtr(characterID);
		if (characterListenerList == nullptr)
		{
			break;
		}
		if (characterListenerList->isEmpty())
		{
			mCharacterEventList.remove(characterID);
			break;
		}
		auto* listenerList = characterListenerList->getPtr(eventType);
		if (listenerList == nullptr)
		{
			break;
		}
		if (listenerList->isEmpty())
		{
			characterListenerList->remove(eventType);
			break;
		}
		// 事件处理过程中可能会再次发送事件
		// 在遍历过程中还可能会出现会将后续需要遍历的事件销毁的情况,所以分发时需要确定事件未销毁且还在分发列表中
		// 但是由于在此循环中判断是否存在于列表会非常耗时,就考虑是否已经销毁就可以了
		// 这里只能使用固定的遍历长度,否则可能会遍历到新加入的事件,导致意外情况
		SAFE_VECTOR_0_SCOPE(*listenerList);
		FOR(listenerList->size())
		{
			EventInfo* info = listenerList->get(i);
			if (info == nullptr)
			{
				continue;
			}
			if (info->isDestroy())
			{
				ERROR_PROFILE("0事件信息已经被销毁");
				continue;
			}
			info->mCallback(param, info->mListener);
		}
	}

	// 全局事件监听
	if (!mGlobalEventList.isEmpty())
	{
		auto* globalListenerList = mGlobalEventList.getPtr(eventType);
		if (globalListenerList != nullptr && !globalListenerList->isEmpty())
		{
			SAFE_VECTOR_0_SCOPE(*globalListenerList);
			FOR(globalListenerList->size())
			{
				EventInfo* info = globalListenerList->get(i);
				if (info == nullptr)
				{
					continue;
				}
				if (info->isDestroy())
				{
					ERROR_PROFILE("2事件信息已经被销毁");
					continue;
				}
				info->mCallback(param, info->mListener);
			}
		}
	}
	--mDispatchDepth;
}

void EventSystem::unlistenEvent(IEventListener* listener)
{
	auto* eventList = mListenerEventMap.getPtr(listener);
	if (eventList == nullptr)
	{
		return;
	}
	for (EventInfo* info : *eventList)
	{
		// 玩家局部事件监听列表中移除
		FOR_ONCE
		{
			if (info->mCharacterGUID == 0)
			{
				break;
			}
			auto* characterList = mCharacterEventList.getPtr(info->mCharacterGUID);
			if (characterList == nullptr)
			{
				break;
			}
			auto* curEventList = characterList->getPtr(info->mType);
			if (curEventList == nullptr)
			{
				break;
			}
			curEventList->remove(info);
			if (curEventList->isEmpty())
			{
				if (!curEventList->isForeaching())
				{
					characterList->remove(info->mType);
					if (characterList->isEmpty())
					{
						mCharacterEventList.remove(info->mCharacterGUID);
					}
				}
				else
				{
					mNeedCheckEmptyEvent = true;
				}
			}
		}
		// 全局事件监听列表中移除
		if (auto* globalEventList = mGlobalEventList.getPtr(info->mType))
		{
			globalEventList->remove(info);
			if (globalEventList->isEmpty())
			{
				mGlobalEventList.remove(info->mType);
			}
		}
	}
	UN_CLASS(*eventList);
	mListenerEventMap.remove(listener);
}

void EventSystem::removeCharacterEvent(const llong characterID)
{
	auto* characterList = mCharacterEventList.getPtr(characterID);
	if (characterList == nullptr)
	{
		return;
	}
	// 此时可能还在遍历某个事件的列表,所以只能先清空
	for (auto iter = characterList->begin(); iter != characterList->end(); )
	{
		// 从以监听者为索引的列表中移除此角色的所有事件
		for (EventInfo* item : iter->second.getMainList())
		{
			if (item == nullptr)
			{
				continue;
			}
			if (item->isDestroy())
			{
				ERROR_PROFILE("3事件信息已经被销毁");
				continue;
			}
			auto* infoList = mListenerEventMap.getPtr(item->mListener);
			if (infoList != nullptr)
			{
				infoList->remove(item);
			}
		}
		UN_CLASS(iter->second);
		// 如果没有在遍历这个列表,就可以直接移除
		characterList->removeOrNext(iter, !iter->second.isForeaching());
	}
	if (characterList->isEmpty())
	{
		mCharacterEventList.remove(characterID);
	}
	else
	{
		mNeedCheckEmptyEvent = true;
	}
}

void EventSystem::listenerEventInternal(EventCallback callback, const Character* character, int eventType, IEventListener* listener)
{
	EventInfo* info = CLASS<EventInfo>();
	info->mType = eventType;
	info->mCharacterGUID = character != nullptr ? character->getGUID() : 0;
	info->mCallback = callback;
	info->mListener = listener;
	mListenerEventMap.addOrGet(listener).add(info);
	if (character != nullptr)
	{
		mCharacterEventList.addOrGet(character->getGUID()).addOrGet(eventType).add(info);
	}
	else
	{
		mGlobalEventList.addOrGet(eventType).add(info);
	}
}