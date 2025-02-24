#pragma once

#include "FrameComponent.h"

template<class BaseType, typename KeyType>
class FactoryBase
{
public:
	virtual ~FactoryBase() = default;
	virtual BaseType* create() = 0;
public:
	KeyType mType = (KeyType)0;
};

template<class ClassType, class BaseType, typename KeyType>
class FactoryInstance : public FactoryBase<BaseType, KeyType>
{
public:
	BaseType* create() override { return new ClassType(); }
};

template<class BaseType, typename KeyType>
class FactoryManager : public FrameComponent
{
	BASE(FactoryManager, FrameComponent);
public:
	void quit() override
	{
		for (const auto& iter : mFactoryList)
		{
			delete iter.second;
		}
		mFactoryList.clear();
	}
	FactoryBase<BaseType, KeyType>* getFactory(const KeyType type) const { return mFactoryList.tryGet(type); }
	const HashMap<KeyType, FactoryBase<BaseType, KeyType>*>& getFactoryList() const { return mFactoryList; }
	int getFactoryCount() const { return mFactoryList.size(); }
	bool isFactory() const override { return true; }
protected:
	template<class ClassType, typename TypeCheck = typename IsSubClassOf<BaseType, ClassType>::mType>
	void addFactoryInstance(const KeyType type)
	{
		auto factory = new FactoryInstance<ClassType, BaseType, KeyType>();
		factory->mType = type;
		mFactoryList.insert(factory->mType, factory);
	}
protected:
	HashMap<KeyType, FactoryBase<BaseType, KeyType>*> mFactoryList;
};