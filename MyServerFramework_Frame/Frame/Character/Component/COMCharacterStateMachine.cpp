#include "FrameHeader.h"

ushort* COMCharacterStateMachine::mAllowAddTypeCountList;
ushort* COMCharacterStateMachine::mAllowKeepTypeCountList;
HashMap<uint, bool> COMCharacterStateMachine::mAllowAddTypeList;
HashMap<uint, bool> COMCharacterStateMachine::mAllowKeepTypeList;

void COMCharacterStateMachine::destroy()
{
	base::destroy();

	// 先退出所有状态
	for (auto& iter : mStateTypeList)
	{
		SAFE_VECTOR_SCOPE(iter.second, readList);
		for (CharacterState* state : readList)
		{
			// 暂时不知道这里为什么会有被销毁的状态
			if (state->isDestroy())
			{
				ERROR_PROFILE(("0Debug:销毁角色时发现存储了已经被销毁的状态:" + string(typeid(*state).name()) + ", 销毁时的行:" + IToS(state->mLine)).c_str());
				continue;
			}
			leaveStateInternal(state, true, true, false);
		}
	}

	// 再销毁所有状态
	for (auto& iter : mStateTypeList)
	{
		SAFE_VECTOR_SCOPE(iter.second, readList);
		for (CharacterState* state : readList)
		{
			// 暂时不知道这里为什么会有被销毁的状态
			if (state->isDestroy())
			{
				ERROR_PROFILE(("1Debug:销毁角色时发现存储了已经被销毁的状态:" + string(typeid(*state).name()) + ", 销毁时的行:" + IToS(state->mLine)).c_str());
				continue;
			}
			destroyState(state, __LINE__);
		}
	}
	mStateTypeList.clear();
	mStateTickList.clear();
	mGroupStateCountList.clear();
}

void COMCharacterStateMachine::destroyState(CharacterState* state, const int line)
{
	state->destroy();
	mStatePool->destroyClass(state);
	state->mLine = line;
}

void COMCharacterStateMachine::destroyStateList(const Vector<CharacterState*>& stateList, const int line)
{
	for (CharacterState* state : stateList)
	{
		state->destroy();
		state->mLine = line;
	}
	mStatePool->destroyClassList(stateList);
}

void COMCharacterStateMachine::onFrameTick(const float elapsedTime)
{
	if (mStateTickCount == 0)
	{
		return;
	}
	SAFE_VECTOR_SCOPE(mStateTickList, stateList);
	for (CharacterState* state : stateList)
	{
		if (state == nullptr)
		{
			continue;
		}
		// 由于在状态的更新过程中可以任意的添加或者移除自己或者其他状态,所以此处存在一个意外情况
		// 比如第一个状态更新中将第三个状态移除了,第三个状态被回收到对象池中,第二个状态在更新中又添加了跟
		// 第三个状态类型相同的状态,所以从对象池中拿到的实际上是上一次已经销毁的状态,导致第三个状态的对象又变成可用的,但是却是跟原来不一样的
		// 这种情况只能继续正常更新,不用再考虑第三个状态是否应该更新
		if (state->isDestroy())
		{
			ERROR_PROFILE((string("2状态已经被销毁:") + typeid(*state).name() + ", 销毁行号:" + IToS(state->mLine)).c_str());
			continue;
		}
		if (state->getCharacter() == nullptr || state->getCharacter()->isDestroy())
		{
			ERROR_PROFILE("3状态所属角色已经被销毁");
			continue;
		}
		if (state->isActive())
		{
			state->update(elapsedTime);
		}
	}
}

bool COMCharacterStateMachine::canAddState(const ushort type, StateParam* param) const
{
	CharacterState* state = mStatePool->newClass(type);
	state->setCharacter(static_cast<Character*>(mComponentOwner));
	const bool result = canAddState(state, param, nullptr);
	destroyState(state, __LINE__);
	return result;
}

CharacterState* COMCharacterStateMachine::addState(const ushort type, StateParam* param, const float stateTime, const bool removeByDie)
{
	CharacterState* state = mStatePool->newClass(type);
	state->setCharacter(static_cast<Character*>(mComponentOwner));
	CharacterState* needRemoveState = nullptr;
	if (!canAddState(state, param, &needRemoveState))
	{
		destroyState(state, __LINE__);
		state = nullptr;
		return nullptr;
	}

	// 移除自身不可共存的状态
	if (needRemoveState != nullptr)
	{
		removeState(needRemoveState, true, removeByDie);
	}

	if (mAllowKeepTypeCountList == nullptr)
	{
		initAllowKeepTypeList();
	}

	// 移除其他因为状态组而不能共存的状态
	if (mAllowKeepTypeCountList[state->getType()] > 0)
	{
		// 移除状态组互斥的状态,此处会涉及到列表的删除,所以不能直接使用主列表进行遍历
		for (auto& iterState : mStateTypeList)
		{
			if (iterState.second.size() == 0 || iterState.first == state->getType())
			{
				continue;
			}
			if (!allowKeepStateByGroup(state->getType(), iterState.first))
			{
				removeStateType(iterState.first, true, removeByDie);
			}
		}
	}

	// 暂时不知道这里为什么会已经被销毁了
	if (state->isDestroy())
	{
		ERROR_PROFILE("4Debug:状态在添加之前销毁了自己");
		state = nullptr;
		return state;
	}

	// 进入状态
	state->setStateTime((param != nullptr) ? param->getStateTime() : stateTime);
	state->enter(param);
	// 在enter以后才会记录初始时间,因为有很多状态是在enter的时候计算时间的
	state->setOriginStateTime(state->getStateTime());

	// 如果在添加的过程中就被移除了,则不再继续执行
	if (state->isDestroy())
	{
		ERROR_PROFILE((string("5状态已经被销毁:") + typeid(*state).name()).c_str());
		state = nullptr;
		return state;
	}
	// 如果没有持续时间,则只执行一个enter,不存储起来
	if (isZero(state->getStateTime()))
	{
		leaveStateInternal(state, false, false, false);
		destroyState(state, __LINE__);
		return nullptr;
	}

	// 添加到状态列表
	mStateTypeList.insertOrGet(state->getType()).push_back(state);
	if (state->isNeedTick())
	{
		mStateTickList.push_back(state);
		++mStateTickCount;
	}
	const auto& groupList = mFrameStateManager->getGroupList(type);
	if (groupList.size() > 0)
	{
		FOR_VECTOR(groupList)
		{
			++mGroupStateCountList.insertOrGet(groupList[i]);
		}
	}
	return state;
}

bool COMCharacterStateMachine::removeState(CharacterState* state, const bool isBreak, const bool removeByDie)
{
	if (state == nullptr)
	{
		return false;
	}
	if (state->isDestroy())
	{
		ERROR_PROFILE((string("6状态已经被销毁:") + typeid(*state).name()).c_str());
		return false;
	}
	if (!state->isActive())
	{
		return false;
	}

	leaveStateInternal(state, isBreak, false, removeByDie);

	// 从列表中移除,这里需要保证状态如果是需要执行更新的,就一定在列表中
	// 但是仍然需要正常执行leave,所以不考虑移除失败的情况
	if (state->isNeedTick())
	{
		mStateTickList.eraseElement(state);
		--mStateTickCount;
	}

	if (auto* stateList = mStateTypeList.getPtr(state->getType()))
	{
		stateList->eraseElement(state);
	}

	auto& groupList = mFrameStateManager->getGroupList(state->getType());
	if (groupList.size() > 0)
	{
		FOR_VECTOR(groupList)
		{
			if (short* countPtr = mGroupStateCountList.getPtr(groupList[i]))
			{
				--(*countPtr);
			}
		}
	}

	destroyState(state, __LINE__);
	return true;
}

void COMCharacterStateMachine::leaveStateInternal(CharacterState* state, const bool isBreak, const bool willDestroy, const bool removeByDie)
{
	// 为了避免在一个状态中移除了其他状态,被移除的状态还可能继续更新的问题
	// 因为状态更新列表在遍历过程中是不会改变的,所以只能通过是否激活来准确判断状态是否有效
	state->setActive(false);
	// 通知即将移除此状态
	state->callWillRemoveCallback();
	// 离开状态
	state->setLeaving(true);
	state->leave(isBreak, willDestroy, removeByDie);
	state->setLeaving(false);
}

void COMCharacterStateMachine::removeStateType(const ushort type, const bool isBreak, const bool removeByDie)
{
	auto* stateList = mStateTypeList.getPtr(type);
	if (stateList == nullptr)
	{
		return;
	}
	// 复制一份列表出来,实际上只需要移动即可,同时也会清空原列表
	Vector<CharacterState*> tempList;
	stateList->moveMainListTo(tempList);
	stateList->clear();
	int removeCount = 0;
	for (CharacterState* state : tempList)
	{
		if (state == nullptr)
		{
			continue;
		}
		if (state->isDestroy())
		{
			ERROR_PROFILE((string("7状态已经被销毁:") + typeid(*state).name()).c_str());
			continue;
		}
		if (!state->isActive())
		{
			continue;
		}
		// 从列表中移除,这里需要保证状态如果是需要执行更新的,就一定在列表中
		// 但是仍然需要正常执行leave,所以不考虑移除失败的情况
		if (state->isNeedTick())
		{
			mStateTickList.eraseElement(state);
			--mStateTickCount;
		}

		leaveStateInternal(state, isBreak, false, removeByDie);
		++removeCount;
	}
	auto& groupList = mFrameStateManager->getGroupList(type);
	if (groupList.size() > 0)
	{
		FOR_VECTOR(groupList)
		{
			if (short* countPtr = mGroupStateCountList.getPtr(groupList[i]))
			{
				(*countPtr) -= removeCount;
			}
		}
	}
	destroyStateList(tempList, __LINE__);
}

void COMCharacterStateMachine::removeStateInGroup(const int group, const bool isBreak, const bool removeByDie)
{
	if (group == 0)
	{
		return;
	}
	auto& stateGroupList = mFrameStateManager->getStateGroupList();
	for (auto& iter : mStateTypeList)
	{
		auto* ptr = stateGroupList.getPtrConst(iter.first);
		if (ptr == nullptr || !ptr->contains(group))
		{
			continue;
		}
		removeStateType(iter.first, isBreak, removeByDie);
	}
}

CharacterState* COMCharacterStateMachine::getFirstState(const ushort type) const
{
	if (mStateTypeList.size() == 0)
	{
		return nullptr;
	}
	const auto* iter = mStateTypeList.getPtrConst(type);
	if (iter == nullptr)
	{
		return nullptr;
	}
	auto& itemList = iter->getMainList();
	return itemList.size() > 0 ? itemList[0] : nullptr;
}

bool COMCharacterStateMachine::hasStateGroup(const int group) const
{
	const short* countPtr = mGroupStateCountList.getPtrConst(group);
	return countPtr != nullptr && *countPtr > 0;
}

bool COMCharacterStateMachine::hasState(const ushort type) const
{
	const auto* iter = mStateTypeList.getPtrConst(type);
	return iter != nullptr && iter->size() > 0;
}

bool COMCharacterStateMachine::canAddState(CharacterState* state, StateParam* param, CharacterState** needRemoveState) const
{
	// 检查自身限制条件
	if (!state->canEnter(param) ||
		!mutexWithExistState(state, needRemoveState))
	{
		return false;
	}

	if (mAllowAddTypeCountList == nullptr)
	{
		initAllowAddTypeList();
	}

	// 如果此状态不会跟任何状态互斥,则不需要再判断
	if (mAllowAddTypeCountList[state->getType()] > 0)
	{
		// 检查是否有跟要添加的状态互斥且不能移除的状态
		// 因为此处在遍历过程中不会对列表进行任何的修改,所以可以直接访问主列表
		for (const auto& iterState : mStateTypeList)
		{
			if (iterState.second.size() == 0 || iterState.first == state->getType())
			{
				continue;
			}
			if (!allowAddStateByGroup(state->getType(), iterState.first))
			{
				return false;
			}
		}
	}
	return true;
}

bool COMCharacterStateMachine::mutexWithExistState(CharacterState* state, CharacterState** needRemoveState) const
{
	const STATE_MUTEX operate = state->getStateMutex();
	// 不允许添加新状态
	if (operate == STATE_MUTEX::CAN_NOT_ADD_NEW)
	{
		return getFirstState(state->getType()) == nullptr;
	}
	// 相同的状态可以共存
	if (operate == STATE_MUTEX::COEXIST)
	{
		return true;
	}
	if (needRemoveState != nullptr)
	{
		// 移除自身不可共存的状态
		CharacterState* existState = getFirstState(state->getType());
		if (existState == nullptr)
		{
			return true;
		}
		if (existState == state)
		{
			ERROR_PROFILE("检查互斥状态时在列表中查询到当前状态");
		}

		// 移除旧状态
		if (operate == STATE_MUTEX::REMOVE_OLD)
		{
			*needRemoveState = existState;
			return true;
		}
		// 只保留优先级最高的状态
		if (operate == STATE_MUTEX::KEEP_HIGH_PRIORITY)
		{
			// 移除优先级低的旧状态,因为每次都只会保留优先级最高的状态,所以同时最多只会存在一个状态
			if (existState->getPriority() < state->getPriority())
			{
				*needRemoveState = existState;
				return true;
			}
			return false;
		}
	}
	else
	{
		// 移除旧状态
		if (operate == STATE_MUTEX::REMOVE_OLD)
		{
			return true;
		}
		// 只保留优先级最高的状态
		if (operate == STATE_MUTEX::KEEP_HIGH_PRIORITY)
		{
			// 移除自身不可共存的状态
			CharacterState* existState = getFirstState(state->getType());
			if (existState == nullptr)
			{
				return true;
			}
			if (existState == state)
			{
				ERROR_PROFILE("检查互斥状态时在列表中查询到当前状态");
			}
			// 移除优先级低的旧状态,因为每次都只会保留优先级最高的状态,所以同时最多只会存在一个状态
			return existState->getPriority() < state->getPriority();
		}
	}
	return true;
}

bool COMCharacterStateMachine::allowKeepStateByGroup(const ushort newState, const ushort existState)
{
	if (mAllowKeepTypeList.size() == 0)
	{
		initAllowKeepTypeList();
	}
	return mAllowKeepTypeList.tryGet((newState << 16) + existState, true);
}

// 添加了newState以后是否还会保留existState
bool COMCharacterStateMachine::allowKeepStateByGroupInternal(const ushort newState, const ushort existState)
{
	// 此处不考虑同类型状态,因为在这之前已经判断过了
	if (newState == existState)
	{
		return true;
	}
	auto& newGroup = mFrameStateManager->getGroupList(newState);
	auto& existGroup = mFrameStateManager->getGroupList(existState);
	const int countNew = newGroup.size();
	const int countExist = existGroup.size();
	// 有一个状态不属于任何状态组时两个状态是可以共存的
	if (countNew == 0 || countExist == 0)
	{
		return true;
	}
	FOR_I(countNew)
	{
		FOR_J(countExist)
		{
			if (newGroup[i] != existGroup[j])
			{
				continue;
			}
			const StateGroup* group = mFrameStateManager->getStateGroup(newGroup[i]);
			const GROUP_MUTEX operate = group->getMutex();
			switch (operate)
			{
			// 状态可共存
			case GROUP_MUTEX::COEXIST:break;
			// 仅与主状态互斥,添加主状态时移除其他所有状态
			case GROUP_MUTEX::MUTEX_WITH_MAIN: 
			{
				// newState为主状态时不能保留其他状态
				if (group->isMainState(newState))
				{
					return false;
				}
			}
			break;
			// 仅与主状态互斥,添加主状态时移除其他所有状态
			case GROUP_MUTEX::MUTEX_WITH_MAIN_ONLY:
			{
				// newState为主状态时不能保留其他状态
				if (group->isMainState(newState))
				{
					return false;
				}
			}
			break;
			// 不可添加新状态
			case GROUP_MUTEX::NO_NEW:return false;
			// 添加新状态时移除所有旧状态
			case GROUP_MUTEX::REMOVE_OTHERS:return false;
			// 主状态反向互斥,添加其他状态时,立即将主状态移除
			case GROUP_MUTEX::MUTEX_INVERSE_MAIN:
			{
				// 存在主状态,则将主状态移除
				if (group->isMainState(existState))
				{
					return false;
				}
			}
			break;
			}
		}
	}
	return true;
}

bool COMCharacterStateMachine::allowAddStateByGroup(const ushort newState, const ushort existState)
{
	if (mAllowAddTypeList.size() == 0)
	{
		initAllowAddTypeList();
	}
	return mAllowAddTypeList.tryGet((newState << 16) + existState, true);
}

// 当存在existState时,是否允许添加newState
bool COMCharacterStateMachine::allowAddStateByGroupInternal(const ushort newState, const ushort existState)
{
	// 此处不考虑同类型状态,因为在这之前已经判断过了
	if (newState == existState)
	{
		return true;
	}
	// 任意一个状态没有所属组,则不在同一组
	auto& newGroup = mFrameStateManager->getGroupList(newState);
	auto& existGroup = mFrameStateManager->getGroupList(existState);
	const int countNew = newGroup.size();
	const int countExist = existGroup.size();
	// 有一个状态不属于任何状态组时两个状态是可以共存的
	if (countNew == 0 || countExist == 0)
	{
		return true;
	}
	FOR_I(countNew)
	{
		FOR_J(countExist)
		{
			// 属于同一状态组,并且该状态组中的所有状态都不能共存,而且不允许添加跟当前状态互斥的状态,则不允许添加该状态
			if (newGroup[i] != existGroup[j])
			{
				continue;
			}
			const StateGroup* group = mFrameStateManager->getStateGroup(newGroup[i]);
			const GROUP_MUTEX operate = group->getMutex();
			switch (operate)
			{
			// 状态可共存
			case GROUP_MUTEX::COEXIST:break;
			// 仅与主状态互斥,有主状态时不可添加其他状态,没有主状态时可任意添加其他状态
			case GROUP_MUTEX::MUTEX_WITH_MAIN:
			{
				// 有主状态时不可添加其他状态
				if (group->isMainState(existState))
				{
					return false;
				}
			}
			break;
			// 仅与主状态互斥,无论是否有主状态都可以添加其他状态
			case GROUP_MUTEX::MUTEX_WITH_MAIN_ONLY:break;
			// 不可添加新状态
			case GROUP_MUTEX::NO_NEW:return false;
			// 添加新状态时移除所有旧状态
			case GROUP_MUTEX::REMOVE_OTHERS:break;
			// 主状态反向互斥,有其他状态时,不允许添加主状态
			case GROUP_MUTEX::MUTEX_INVERSE_MAIN:
			{
				if (group->isMainState(newState))
				{
					return false;
				}
			}
			break;
			}
		}
	}
	return true;
}

void COMCharacterStateMachine::resetProperty()
{
	base::resetProperty();
	mStateTypeList.clear();
	mStateTickList.clear();
	mGroupStateCountList.clear();
	mStateTickCount = 0;
}

void COMCharacterStateMachine::initAllowKeepTypeList()
{
	const auto& factoryList = mCharacterStateFactoryManager->getFactoryList();
	ushort maxType = 0;
	for (const auto& item0 : factoryList)
	{
		clampMinRef(maxType, item0.first);
	}
	mAllowKeepTypeCountList = new ushort[maxType + 1];
	memset(mAllowKeepTypeCountList, 0, sizeof(ushort) * (maxType + 1));

	// 由于绝大部分都是true,所以只记录不允许的情况
	for (const auto& item0 : factoryList)
	{
		for (const auto& item1 : factoryList)
		{
			if (!allowKeepStateByGroupInternal(item0.first, item1.first))
			{
				mAllowKeepTypeList.insert((item0.first << 16) + item1.first, false);
				++mAllowKeepTypeCountList[item0.first];
			}
		}
	}
}

void COMCharacterStateMachine::initAllowAddTypeList()
{
	const auto& factoryList = mCharacterStateFactoryManager->getFactoryList();
	ushort maxType = 0;
	for (const auto& item0 : factoryList)
	{
		clampMinRef(maxType, item0.first);
	}
	mAllowAddTypeCountList = new ushort[maxType + 1];
	memset(mAllowAddTypeCountList, 0, sizeof(ushort) * (maxType + 1));

	// 由于绝大部分都是true,所以只记录不允许的情况
	for (const auto& item0 : factoryList)
	{
		for (const auto& item1 : factoryList)
		{
			if (!allowAddStateByGroupInternal(item0.first, item1.first))
			{
				mAllowAddTypeList.insert((item0.first << 16) + item1.first, false);
				++mAllowAddTypeCountList[item0.first];
			}
		}
	}
}