#include "FrameHeader.h"

ComponentOwner::~ComponentOwner()
{
	DELETE(mFrameTickList);
	DELETE(mLateFrameTickList);
	DELETE(mSecondTickList);
	DELETE(mBreakableComponentList);
	destroyAllComponents();
	DELETE(mComponentTypeList);
}

void ComponentOwner::update(const float elapsedTime)
{
	// 每帧都更新的组件
	if (mFrameTickList != nullptr && !mFrameTickList->isEmpty())
	{
		SAFE_HASHMAP_SCOPE(*mFrameTickList, readList);
		for (const auto& item : readList)
		{
			FloatCallback tickFunction = item.second;
			// 可能会存在遍历过程中被禁用的组件
			if (tickFunction == nullptr)
			{
				continue;
			}
			tickFunction(elapsedTime);
			// 对象即将被销毁时,不应该再执行任何逻辑
			if (isPendingDestroy())
			{
				return;
			}
			// 如果当前对象已经被销毁了则不能再执行后续的组件更新,不过正常不应该在Tick中销毁对象
			if (isDestroy())
			{
				ERROR_PROFILE((string("0当前对象已经被销毁:") + typeid(*this).name()).c_str());
				mFrameTickList->clear();
				return;
			}
		}
	}

	// 每秒更新的逻辑跟上面每帧更新的完全一样
	if (mSecondTickList != nullptr && tickTimerLoop(mTickTimer, elapsedTime, 1.0f))
	{
		SAFE_HASHMAP_SCOPE(*mSecondTickList, readList);
		for (const auto& item : readList)
		{
			VoidCallback tickFunction = item.second;
			// 可能会存在遍历过程中被禁用的组件
			if (tickFunction == nullptr)
			{
				continue;
			}
			tickFunction();
			// 对象即将被销毁时,不应该再执行任何逻辑
			if (isPendingDestroy())
			{
				return;
			}
			// 如果当前对象已经被销毁了则不能再执行后续的组件更新,不过正常不应该在Tick中销毁对象
			if (isDestroy())
			{
				ERROR_PROFILE((string("0当前对象已经被销毁:") + typeid(*this).name()).c_str());
				mSecondTickList->clear();
				return;
			}
		}
	}
}

void ComponentOwner::lateUpdate(float elapsedTime)
{
	// 每帧都更新的组件
	if (mLateFrameTickList != nullptr)
	{
		SAFE_HASHMAP_SCOPE(*mLateFrameTickList, readList);
		for (const auto& item : readList)
		{
			FloatCallback tickFunction = item.second;
			// 可能会存在遍历过程中被禁用的组件
			if (tickFunction == nullptr)
			{
				continue;
			}
			tickFunction(elapsedTime);
			// 对象即将被销毁时,不应该再执行任何逻辑
			if (isPendingDestroy())
			{
				return;
			}
			// 如果当前对象已经被销毁了则不能再执行后续的组件更新,不过正常不应该在Tick中销毁对象
			if (isDestroy())
			{
				ERROR_PROFILE((string("0当前对象已经被销毁:") + typeid(*this).name()).c_str());
				mLateFrameTickList->clear();
				return;
			}
		}
	}
}

void ComponentOwner::destroyComponent(GameComponent* component)
{
	if (component == nullptr || component->isDestroy())
	{
		return;
	}
	// 禁用组件,这样可以使组件的tick函数从下面的列表中移除,避免正在遍历时访问到了已经被销毁的组件tick
	component->setActive(false);
	if (mFrameTickList != nullptr)
	{
		mFrameTickList->remove(component);
	}
	if (mLateFrameTickList != nullptr)
	{
		mLateFrameTickList->remove(component);
	}
	if (mSecondTickList != nullptr)
	{
		mSecondTickList->remove(component);
	}
	if (mComponentTypeList != nullptr)
	{
		mComponentTypeList->remove(component);
	}
	removeBreakableComponent(component);
	for (int i = 0; i < COMPONENT_ARRAY_COUNT; ++i)
	{
		if (mComponentArray[i] == component)
		{
			mComponentArray[i] = nullptr;
			break;
		}
	}
}

void ComponentOwner::destroyAllComponents()
{
	if (mFrameTickList != nullptr)
	{
		mFrameTickList->clear();
	}
	if (mLateFrameTickList != nullptr)
	{
		mLateFrameTickList->clear();
	}
	if (mSecondTickList != nullptr)
	{
		mSecondTickList->clear();
	}
	if (mComponentTypeList != nullptr)
	{
		mGameComponentPool->destroyClassList(*mComponentTypeList);
	}
	if (mBreakableComponentList != nullptr)
	{
		mBreakableComponentList->clear();
	}
	clearComponentArray();
}

void ComponentOwner::resetProperty()
{
	base::resetProperty();
	if (mFrameTickList != nullptr)
	{
		mFrameTickList->clear();
	}
	if (mLateFrameTickList != nullptr)
	{
		mLateFrameTickList->clear();
	}
	if (mSecondTickList != nullptr)
	{
		mSecondTickList->clear();
	}
	if (mComponentTypeList != nullptr)
	{
		mComponentTypeList->clear();
	}
	if (mBreakableComponentList != nullptr)
	{
		mBreakableComponentList->clear();
	}
	clearComponentArray();
	mTickTimer = 1.0f;
}

void ComponentOwner::clearComponentArray()
{
	for (int i = 0; i < COMPONENT_ARRAY_COUNT; ++i)
	{
		mComponentArray[i] = nullptr;
	}
}

void ComponentOwner::setFrameTick(GameComponent* component, FloatCallback callback)
{
	if (mFrameTickList == nullptr)
	{
		mFrameTickList = new SafeHashMap<GameComponent*, FloatCallback>();
	}
	mFrameTickList->add(component, callback);
	addBreakableComponent(component);
}

void ComponentOwner::clearFrameTick(GameComponent* component)
{
	if (mFrameTickList == nullptr || !mFrameTickList->remove(component))
	{
		return;
	}
	removeBreakableComponent(component);
}

void ComponentOwner::setLateFrameTick(GameComponent* component, FloatCallback callback)
{
	if (mLateFrameTickList == nullptr)
	{
		mLateFrameTickList = new SafeHashMap<GameComponent*, FloatCallback>();
	}
	mLateFrameTickList->add(component, callback);
	addBreakableComponent(component);
}

void ComponentOwner::clearLateFrameTick(GameComponent* component)
{
	if (mLateFrameTickList == nullptr || !mLateFrameTickList->remove(component))
	{
		return;
	}
	removeBreakableComponent(component);
}

void ComponentOwner::setSecondTick(GameComponent* component, VoidCallback callback)
{
	if (mSecondTickList == nullptr)
	{
		mSecondTickList = new SafeHashMap<GameComponent*, VoidCallback>();
	}
	mSecondTickList->add(component, callback);
	addBreakableComponent(component);
}

void ComponentOwner::clearSecondTick(GameComponent* component)
{
	if (mSecondTickList == nullptr || !mSecondTickList->remove(component))
	{
		return;
	}
	removeBreakableComponent(component);
}

void ComponentOwner::addBreakableComponent(GameComponent* component)
{
	if (dynamic_cast<IComponentBreakable*>(component) == nullptr)
	{
		return;
	}
	if (mBreakableComponentList == nullptr)
	{
		mBreakableComponentList = new Vector<GameComponent*>();
	}
	mBreakableComponentList->add(component);
}

void ComponentOwner::removeBreakableComponent(GameComponent* component)
{
	if (mBreakableComponentList != nullptr)
	{
		mBreakableComponentList->remove(component);
	}
}