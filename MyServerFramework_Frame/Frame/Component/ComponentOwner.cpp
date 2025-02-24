#include "FrameHeader.h"

ComponentOwner::~ComponentOwner()
{
	if (mFrameTickList != nullptr)
	{
		delete mFrameTickList;
		mFrameTickList = nullptr;
	}
	if (mSecondTickList != nullptr)
	{
		delete mSecondTickList;
		mSecondTickList = nullptr;
	}
	if (mBreableComponentList != nullptr)
	{
		delete mBreableComponentList;
		mBreableComponentList = nullptr;
	}
	if (mComponentArray != nullptr)
	{
		delete mComponentArray;
		mComponentArray = nullptr;
	}
	destroyAllComponents();

	if (mComponentTypeMap != nullptr)
	{
		delete mComponentTypeMap;
		mComponentTypeMap = nullptr;
	}
	if (mComponentTypeList != nullptr)
	{
		delete mComponentTypeList;
		mComponentTypeList = nullptr;
	}
}

void ComponentOwner::update(const float elapsedTime)
{
	// 每帧都更新的组件
	if (mFrameTickList != nullptr)
	{
		SAFE_VECTOR_SCOPE(*mFrameTickList, readList);
		FOR_VECTOR(readList)
		{
			FrameTickCallback tickFunction = readList[i].second;
			// 可能会存在遍历过程中被禁用的组件
			if (tickFunction == nullptr)
			{
				continue;
			}
			tickFunction(readList[i].first, elapsedTime);
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
		SAFE_VECTOR_SCOPE(*mSecondTickList, readList);
		FOR_VECTOR(readList)
		{
			SecondTickCallback tickFunction = readList[i].second;
			// 可能会存在遍历过程中被禁用的组件
			if (tickFunction == nullptr)
			{
				continue;
			}
			tickFunction(readList[i].first);
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
		auto& mainList = mFrameTickList->getMainList();
		FOR_VECTOR(mainList)
		{
			if (mainList[i].first == component)
			{
				mFrameTickList->eraseAt(i);
				break;
			}
		}
	}
	if (mSecondTickList != nullptr)
	{
		auto& mainList = mSecondTickList->getMainList();
		FOR_VECTOR(mainList)
		{
			if (mainList[i].first == component)
			{
				mSecondTickList->eraseAt(i);
				break;
			}
		}
	}
	if (mComponentTypeMap != nullptr)
	{
		mComponentTypeMap->erase(component->getType());
	}
	if (mComponentTypeList != nullptr)
	{
		mComponentTypeList->eraseElement(component);
	}
	if (mBreableComponentList != nullptr)
	{
		mBreableComponentList->eraseElement(component);
	}
	if (mComponentArray != nullptr)
	{
		mComponentArray->eraseElement(component);
	}
}

void ComponentOwner::destroyAllComponents()
{
	if (mFrameTickList != nullptr)
	{
		mFrameTickList->clear();
	}
	if (mSecondTickList != nullptr)
	{
		mSecondTickList->clear();
	}
	if (mComponentTypeMap != nullptr)
	{
		mComponentTypeMap->clear();
	}
	if (mComponentTypeList != nullptr)
	{
		for (GameComponent* com : *mComponentTypeList)
		{
			com->destroy();
		}
		mGameComponentPool->destroyClassList(*mComponentTypeList);
		mComponentTypeList->clear();
	}
	if (mBreableComponentList != nullptr)
	{
		mBreableComponentList->clear();
	}
	if (mComponentArray != nullptr)
	{
		mComponentArray->clear();
	}
}

void ComponentOwner::resetProperty()
{
	base::resetProperty();
	if (mFrameTickList != nullptr)
	{
		mFrameTickList->clear();
	}
	if (mSecondTickList != nullptr)
	{
		mSecondTickList->clear();
	}
	if (mComponentTypeMap != nullptr)
	{
		mComponentTypeMap->clear();
	}
	if (mComponentTypeList != nullptr)
	{
		mComponentTypeList->clear();
	}
	if (mBreableComponentList != nullptr)
	{
		mBreableComponentList->clear();
	}
	if (mComponentArray != nullptr)
	{
		mComponentArray->clear();
	}
	mTickTimer = 1.0f;
}

void ComponentOwner::registeFrameTick(GameComponent* component, FrameTickCallback callback)
{
	if (mFrameTickList == nullptr)
	{
		mFrameTickList = new SafeVector<pair<GameComponent*, FrameTickCallback>>();
	}
	mFrameTickList->push_back(make_pair(component, callback));
	if (dynamic_cast<IComponentBreakable*>(component) != nullptr)
	{
		if (mBreableComponentList == nullptr)
		{
			mBreableComponentList = new Vector<GameComponent*>();
		}
		mBreableComponentList->push_back(component);
	}
}

void ComponentOwner::unregisteFrameTick(GameComponent* component, FrameTickCallback callback)
{
	if (mFrameTickList != nullptr)
	{
		mFrameTickList->eraseElement(make_pair(component, callback));
	}
	if (mBreableComponentList != nullptr && dynamic_cast<IComponentBreakable*>(component) != nullptr)
	{
		mBreableComponentList->eraseElement(component);
	}
}

void ComponentOwner::registeSecondTick(GameComponent* component, SecondTickCallback callback)
{
	if (mSecondTickList == nullptr)
	{
		mSecondTickList = new SafeVector<pair<GameComponent*, SecondTickCallback>>();
	}
	mSecondTickList->push_back(make_pair(component, callback));
	if (dynamic_cast<IComponentBreakable*>(component) != nullptr)
	{
		if (mBreableComponentList == nullptr)
		{
			mBreableComponentList = new Vector<GameComponent*>();
		}
		mBreableComponentList->push_back(component);
	}
}

void ComponentOwner::unregisteSecondTick(GameComponent* component, SecondTickCallback callback)
{
	if (mSecondTickList != nullptr)
	{
		mSecondTickList->eraseElement(make_pair(component, callback));
	}
	if (mBreableComponentList != nullptr && dynamic_cast<IComponentBreakable*>(component) != nullptr)
	{
		mBreableComponentList->eraseElement(component);
	}
}