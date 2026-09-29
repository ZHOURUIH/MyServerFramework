#pragma once
#include "CommandReceiver.h"
#include "GameComponent.h"
#include "IComponentInterface.h"
#include "FrameBase.h"
#include "GameComponentPool.h"

template<typename T>struct ComponentIndex { static constexpr int mIndex = -1; };

class MICRO_LEGEND_FRAME_API ComponentOwner : public CommandReceiver
{
	BASE(ComponentOwner, CommandReceiver);
public:
	~ComponentOwner() override;
	void destroy() override { destroyAllComponents(); }
	virtual void update(float elapsedTime);
	virtual void lateUpdate(float elapsedTime);
	void destroyComponent(GameComponent* component);
	template<typename T, typename TypeCheck = typename IsSubClassOf<GameComponent, T>::mType>
	void addComponent(T*& com, bool active = true)
	{
		com = addComponent<T>(active);
	}
	// 根据类型添加一个组件,组件类型需要定义staticType的静态函数,来获取此类组件的类型
	template<typename T, typename TypeCheck = typename IsSubClassOf<GameComponent, T>::mType>
	T* addComponent(const bool active = true)
	{
		// 有固定下标的组件直接走下标快速路径
		if constexpr (ComponentIndex<T>::mIndex >= 0)
		{
			return addComponentAtIndex<T>(active);
		}
		if (mComponentTypeList != nullptr)
		{
			FOR(mComponentTypeList->size())
			{
				GameComponent* com = (*mComponentTypeList)[i];
				if (com->getType() == T::staticType())
				{
					com->setActive(active);
					return static_cast<T*>(com);
				}
			}
		}
		else
		{
			mComponentTypeList = new Vector<GameComponent*>();
		}
		// 创建组件并且设置拥有者,然后初始化
		GameComponent* component = mGameComponentPool->newClass(T::staticType());
		component->init(this);
		// 将组件加入列表
		mComponentTypeList->add(component);
		// 如果active为true,则强制刷新,否则就不刷新
		component->setActive(active, true);
		component->setDefaultActive(active);
		return static_cast<T*>(component);
	}
	// 创建组件并且将组件加入到指定下标,该组件需要有ComponentIndex<T>的模板类定义,且指定下标为正数
	template<typename T,
		typename TypeCheck0 = typename IsSubClassOf<GameComponent, T>::mType,
		typename TypeCheck1 = typename IsPositive<ComponentIndex<T>::mIndex>::mType>
	void addComponentAtIndex(T*& com, const bool active = true)
	{
		com = addComponentAtIndex<T>(active);
	}
	// 固定下标组件只保存到固定数组和组件列表,不再创建和维护类型Map
	template<typename T,
		typename TypeCheck0 = typename IsSubClassOf<GameComponent, T>::mType,
		typename TypeCheck1 = typename IsPositive<ComponentIndex<T>::mIndex>::mType>
	T* addComponentAtIndex(const bool active = true)
	{
		constexpr int index = ComponentIndex<T>::mIndex;
		static_assert(index >= 0 && index < COMPONENT_ARRAY_COUNT, "ComponentIndex超出ComponentOwner固定数组范围");
		GameComponent* curComponent = mComponentArray[index];
		if (curComponent != nullptr)
		{
			curComponent->setActive(active);
			return static_cast<T*>(curComponent);
		}
		if (mComponentTypeList == nullptr)
		{
			mComponentTypeList = new Vector<GameComponent*>();
		}
		GameComponent* component = mGameComponentPool->newClass(T::staticType());
		component->init(this);
		mComponentTypeList->add(component);
		mComponentArray[index] = component;
		component->setActive(active, true);
		component->setDefaultActive(active);
		return static_cast<T*>(component);
	}
	// 获取指定自定义下标的组件
	template<typename T,
		typename TypeCheck0 = typename IsSubClassOf<GameComponent, T>::mType,
		typename TypeCheck1 = typename IsPositive<ComponentIndex<T>::mIndex>::mType>
	T* getComponentAtIndex() const
	{
		constexpr int index = ComponentIndex<T>::mIndex;
		static_assert(index >= 0 && index < COMPONENT_ARRAY_COUNT, "ComponentIndex超出ComponentOwner固定数组范围");
		return static_cast<T*>(mComponentArray[index]);
	}
	// 组件类型需要定义staticType的静态函数,固定下标组件直接从数组获取
	template<typename T, typename TypeCheck = typename IsSubClassOf<GameComponent, T>::mType>
	T* getComponent(const bool needActive = false) const
	{
		GameComponent* component = nullptr;
		if constexpr (ComponentIndex<T>::mIndex >= 0)
		{
			constexpr int index = ComponentIndex<T>::mIndex;
			static_assert(index < COMPONENT_ARRAY_COUNT, "ComponentIndex超出ComponentOwner固定数组范围");
			component = mComponentArray[index];
		}
		else
		{
			if (mComponentTypeList != nullptr)
			{
				FOR(mComponentTypeList->size())
				{
					GameComponent* com = (*mComponentTypeList)[i];
					if (com->getType() == T::staticType())
					{
						component = com;
						break;
					}
				}
			}
		}
		if (component == nullptr || (needActive && !component->isActive()))
		{
			return nullptr;
		}
		return static_cast<T*>(component);
	}
	const Vector<GameComponent*>& getComponentList() const
	{
		return mComponentTypeList != nullptr ? *mComponentTypeList : Vector<GameComponent*>::mDefaultList;
	}
	// 按类型中断其他组件,T是中断类型,比如移动,旋转,缩放,ExceptType是需要排除的类型,一般就是此类型导致其他组件需要中断
	template<typename T,
		typename ExceptType,
		typename TypeCheck0 = typename IsSubClassOf<IComponentBreakable, T>::mType,
		typename TypeCheck1 = typename IsSubClassOf<GameComponent, ExceptType>::mType,
		typename TypeCheck2 = typename IsSubClassOf<T, ExceptType>::mType>
	void breakComponent() const
	{
		if (mBreakableComponentList == nullptr)
		{
			return;
		}
		for (GameComponent* component : *mBreakableComponentList)
		{
			if (component->isActive() &&
				dynamic_cast<T*>(component) != nullptr &&
				component->getType() != ExceptType::staticType())
			{
				dynamic_cast<T*>(component)->notifyBreak();
				component->setActive(false);
			}
		}
	}
	void resetProperty() override;
	void setFrameTick(GameComponent* component, FloatCallback callback);
	void clearFrameTick(GameComponent* component);
	void setLateFrameTick(GameComponent* component, FloatCallback callback);
	void clearLateFrameTick(GameComponent* component);
	void setSecondTick(GameComponent* component, VoidCallback callback);
	void clearSecondTick(GameComponent* component);
protected:
	void destroyAllComponents();
	void addBreakableComponent(GameComponent* component);
	void removeBreakableComponent(GameComponent* component);
	void clearComponentArray();
protected:
	// 每帧都tick的组件列表,没有顺序,只有注册了Tick的组件才能执行更新
	SafeHashMap<GameComponent*, FloatCallback>* mFrameTickList = nullptr;
	// 每帧在lateUpdate中执行的组件列表
	SafeHashMap<GameComponent*, FloatCallback>* mLateFrameTickList = nullptr;
	// 每秒tick一次的组件列表,没有顺序,只有注册了Tick的组件才能执行更新
	SafeHashMap<GameComponent*, VoidCallback>* mSecondTickList = nullptr;
	// 所有组件列表,用于统一销毁以及需要遍历全部组件的逻辑
	Vector<GameComponent*>* mComponentTypeList = nullptr;
	// 可被其他组件中断的组件列表,因为只有少量类型的组件可被中断,所以为了减少被中断时的遍历次数,只在特定的列表中遍历,而且只有可tick的组件才能被中断
	Vector<GameComponent*>* mBreakableComponentList = nullptr;
	// 可直接通过下标访问的固定组件数组,预留到16
	static constexpr int COMPONENT_ARRAY_COUNT = 16;
	GameComponent* mComponentArray[COMPONENT_ARRAY_COUNT]{};
	// 每秒tick的计时器
	float mTickTimer = 1.0f;
};