#pragma once

#include "CommandReceiver.h"
#include "GameComponent.h"
#include "IComponentInterface.h"
#include "SafeVector.h"
#include "FrameBase.h"
#include "GameComponentPool.h"

template<typename T>struct ComponentIndex { static constexpr int mIndex = -1; };

class MICRO_LEGEND_FRAME_API ComponentOwner : public CommandReceiver
{
	BASE(ComponentOwner, CommandReceiver);
public:
	~ComponentOwner() override;
	virtual void destroy() { destroyAllComponents(); }
	virtual void update(float elapsedTime);
	void destroyComponent(GameComponent* component);
	// 根据类型添加一个组件,组件类型需要定义staticType的静态函数,来获取此类组件的类型
	template<typename T, typename TypeCheck = typename IsSubClassOf<GameComponent, T>::mType>
	T* addComponent(const bool active = true)
	{
		// 不能创建重名的组件
		if (mComponentTypeMap != nullptr)
		{
			GameComponent* curComponent = mComponentTypeMap->tryGet(T::staticType());
			if (curComponent != nullptr)
			{
				curComponent->setActive(active);
				return static_cast<T*>(curComponent);
			}
		}
		else
		{
			mComponentTypeMap = new HashMap<ushort, GameComponent*>();
		}
		if (mComponentTypeList == nullptr)
		{
			mComponentTypeList = new Vector<GameComponent*>();
		}
		// 创建组件并且设置拥有者,然后初始化
		GameComponent* component = mGameComponentPool->newClass(T::staticType());
		component->init(this);
		// 将组件加入列表
		mComponentTypeMap->insert(component->getType(), component);
		mComponentTypeList->push_back(component);
		// 如果active为true,则强制刷新,否则就不刷新
		component->setActive(active, true);
		component->setDefaultActive(active);
		return static_cast<T*>(component);
	}
	// 创建组件并且将组件加入到指定下标,该组件需要有ComponentIndex<T>的模板类定义,且指定下标为正数
	template<typename T,
		typename TypeCheck0 = typename IsSubClassOf<GameComponent, T>::mType,
		typename TypeCheck1 = typename IsPositive<ComponentIndex<T>::mIndex>::mType>
	T* addComponentAtIndex(const bool active = true)
	{
		constexpr int index = ComponentIndex<T>::mIndex;
		T* component = addComponent<T>(active);
		if (mComponentArray == nullptr)
		{
			mComponentArray = new Vector<GameComponent*>();
		}
		if (mComponentArray->size() <= index)
		{
			mComponentArray->resize(index + 1);
		}
		if ((*mComponentArray)[index] != nullptr)
		{
			ERROR("重复下标的组件:" + IToS(index) + ", 组件:" + typeid(T).name());
		}
		(*mComponentArray)[index] = component;
		return component;
	}
	// 获取指定自定义下标的组件,该组件需要有ComponentIndex<T>的模板类定义,且指定下标为正数
	template<typename T,
		typename TypeCheck0 = typename IsSubClassOf<GameComponent, T>::mType,
		typename TypeCheck1 = typename IsPositive<ComponentIndex<T>::mIndex>::mType>
	T* getComponentAtIndex() const
	{
		if (mComponentArray == nullptr)
		{
			return nullptr;
		}
		constexpr int index = ComponentIndex<T>::mIndex;
		if (index >= mComponentArray->size())
		{
			return nullptr;
		}
		return static_cast<T*>((*mComponentArray)[index]);
	}
	// 组件类型需要定义staticType的静态函数,来获取此类组件的类型
	template<typename T, typename TypeCheck = typename IsSubClassOf<GameComponent, T>::mType>
	T* getComponent(const bool needActive = false) const
	{
		if (mComponentTypeMap == nullptr || mComponentTypeMap->size() == 0)
		{
			return nullptr;
		}
		GameComponent* component = mComponentTypeMap->tryGet(T::staticType());
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
	const HashMap<ushort, GameComponent*>& getComponentMap() const
	{
		return mComponentTypeMap != nullptr ? *mComponentTypeMap : HashMap<ushort, GameComponent*>::mDefaultHashMap;
	}
	// 按类型中断其他组件,T是中断类型,比如移动,旋转,缩放,ExcecptType是需要排除的类型,一般就是此类型导致其他组件需要中断
	template<typename T,
		typename ExceptType,
		typename TypeCheck0 = typename IsSubClassOf<IComponentBreakable, T>::mType,
		typename TypeCheck1 = typename IsSubClassOf<GameComponent, ExceptType>::mType,
		typename TypeCheck2 = typename IsSubClassOf<T, ExceptType>::mType>
	void breakComponent() const
	{
		if (mBreableComponentList == nullptr)
		{
			return;
		}
		for (GameComponent* component : *mBreableComponentList)
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
	void registeFrameTick(GameComponent* component, FrameTickCallback callback);
	void unregisteFrameTick(GameComponent* component, FrameTickCallback callback);
	void registeSecondTick(GameComponent* component, SecondTickCallback callback);
	void unregisteSecondTick(GameComponent* component, SecondTickCallback callback);
protected:
	void destroyAllComponents();
protected:
	// 每帧都tick的组件列表,没有顺序,只有注册了Tick的组件才能执行更新
	SafeVector<pair<GameComponent*, FrameTickCallback>>* mFrameTickList = nullptr;
	// 每秒tick一次的组件列表,没有顺序,只有注册了Tick的组件才能执行更新
	SafeVector<pair<GameComponent*, SecondTickCallback>>* mSecondTickList = nullptr;
	// 组件类型列表,first是组件的类型名,可通过名字进行查找
	HashMap<ushort, GameComponent*>* mComponentTypeMap = nullptr;
	Vector<GameComponent*>* mComponentTypeList = nullptr;
	// 可被其他组件中断的组件列表,因为只有少量类型的组件可被中断,所以为了减少被中断时的遍历次数,只在特定的列表中遍历,而且只有可tick的组件才能被中断
	Vector<GameComponent*>* mBreableComponentList = nullptr;
	// 可直接通过下标访问的组件列表,用于快速获得组件
	Vector<GameComponent*>* mComponentArray = nullptr;
	// 每秒tick的计时器
	float mTickTimer = 1.0f;
};