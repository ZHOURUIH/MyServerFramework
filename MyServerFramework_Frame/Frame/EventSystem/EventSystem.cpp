#include "FrameHeader.h"

void EventSystem::quit()
{
	for (const auto& item : mListenerEventMap)
	{
		mEventInfoPool->destroyClassList(item.second);
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
	for (auto& itemChracter : mCharacterEventList)
	{
		auto& characterListenerList = itemChracter.second;
		if (characterListenerList.size() == 0)
		{
			continue;
		}
		removeEvent.clear();
		for (const auto& itemEvent : characterListenerList)
		{
			if (itemEvent.second.size() == 0)
			{
				removeEvent.push_back(itemEvent.first);
			}
		}
		if (removeEvent.size() > 0)
		{
			for (const int eventType : removeEvent)
			{
				characterListenerList.erase(eventType);
			}
			if (characterListenerList.size() == 0)
			{
				removeCharacterEvent.push_back(itemChracter.first);
			}
		}
	}
	if (removeCharacterEvent.size() > 0)
	{
		for (const llong characterGUID : removeCharacterEvent)
		{
			mCharacterEventList.erase(characterGUID);
		}
	}
	mNeedCheckEmptyEvent = false;
}

void EventSystem::pushEventInternal(GameEvent* param, const llong characterID)
{
	const int eventType = param->mType;
	param->mCharacterGUID = characterID;

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
		if (characterListenerList->size() == 0)
		{
			mCharacterEventList.erase(characterID);
			break;
		}
		auto* listenerList = characterListenerList->getPtr(eventType);
		if (listenerList == nullptr)
		{
			break;
		}
		if (listenerList->size() == 0)
		{
			characterListenerList->erase(eventType);
			break;
		}
		// 事件处理过程中可能会再次发送事件,所以如果没有正在遍历,则开始正常遍历
		// 在遍历过程中还可能会出现会将后续需要遍历的事件销毁的情况,所以分发时需要确定事件未销毁且还在分发列表中
		// 但是由于在此循环中判断是否存在于列表会非常耗时,就考虑是否已经销毁就可以了
		if (!listenerList->isForeaching())
		{
			SAFE_VECTOR_SCOPE(*listenerList, readList);
			for (EventInfo* info : readList)
			{
				if (info == nullptr)
				{
					ERROR_PROFILE("0事件信息已经被销毁, nullptr");
					continue;
				}
				if (info->isDestroy())
				{
					ERROR_PROFILE("0事件信息已经被销毁");
					continue;
				}
				info->mEventProcess->onEventInternal(param, info->mListener);
			}
		}
		// 如果正在遍历过程中,则使用临时列表复制一份
		else
		{
			listenerList->getMainList().clone(mTempEventList);
			for (EventInfo* info : mTempEventList)
			{
				if (info == nullptr)
				{
					ERROR_PROFILE("1事件信息已经被销毁, nullptr");
					continue;
				}
				if (info->isDestroy())
				{
					ERROR_PROFILE("1事件信息已经被销毁");
					continue;
				}
				info->mEventProcess->onEventInternal(param, info->mListener);
			}
		}
	}

	// 全局事件监听
	if (mGlobalEventList.size() > 0)
	{
		auto* globalListenerList = mGlobalEventList.getPtr(eventType);
		if (globalListenerList != nullptr && globalListenerList->size() > 0)
		{
			SAFE_VECTOR_SCOPE(*globalListenerList, readList);
			for (EventInfo* info : readList)
			{
				if (info == nullptr)
				{
					ERROR_PROFILE("2事件信息已经被销毁, nullptr");
					continue;
				}
				if (info->isDestroy())
				{
					ERROR_PROFILE("2事件信息已经被销毁");
					continue;
				}
				info->mEventProcess->onEventInternal(param, info->mListener);
			}
		}
	}
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
			curEventList->eraseElement(info);
			if (curEventList->size() == 0)
			{
				if (!curEventList->isForeaching())
				{
					characterList->erase(info->mType);
					if (characterList->size() == 0)
					{
						mCharacterEventList.erase(info->mCharacterGUID);
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
			globalEventList->eraseElement(info);
			if (globalEventList->size() == 0)
			{
				mGlobalEventList.erase(info->mType);
			}
		}
	}
	mEventInfoPool->destroyClassList(*eventList);
	mListenerEventMap.erase(listener);
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
			auto* infoList = mListenerEventMap.getPtr(item->mListener);
			if (infoList != nullptr)
			{
				infoList->eraseElement(item);
			}
		}
		mEventInfoPool->destroyClassList(iter->second.getMainList());
		iter->second.clear();
		// 如果没有在遍历这个列表,就可以直接移除
		if (!iter->second.isForeaching())
		{
			iter = characterList->erase(iter);
		}
		else
		{
			++iter;
		}
	}
	if (characterList->size() == 0)
	{
		mCharacterEventList.erase(characterID);
	}
	else
	{
		mNeedCheckEmptyEvent = true;
	}
}