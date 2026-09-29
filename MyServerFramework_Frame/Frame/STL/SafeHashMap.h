#pragma once

#include "HashMap.h"
#include "Vector.h"
#include "MapModify.h"

// 支持在遍历列表时对列表进行插入和删除
// 需要搭配SAFE_HASHMAP_SCOPE宏来进行遍历
// SafeHashMap<int, int> list;
// SAFE_HASHMAP_SCOPE(list, readList);
// for (const auto& item : readList)
// {}
template<typename Key, typename Value>
class SafeHashMap
{
public:
	typedef HashMap<Key, Value> List;
	typedef Key KeyType;
	typedef Value ValueType;
public:
	SafeHashMap(int capacity = 8) :
		mUpdateList(capacity),
		mMainList(capacity)
	{}
	virtual ~SafeHashMap() { clear(); }
	const HashMap<Key, Value>& startForeach()
	{
		if (mForeaching)
		{
			ERROR("正在遍历列表,无法再次开始遍历");
		}
		mForeaching = true;
		return mUpdateList;
	}
	void endForeach()													
	{
		mForeaching = false;
		// 在结束遍历时同步一次
		if (!mModifyList.isEmpty())
		{
			sync();
		}
	}
	bool isForeaching() const											{ return mForeaching; }
	// 获取主列表,存储着当前实时的数据列表,所有的删除和新增都会立即更新此列表
	// 当遍历过程中不能确定列表一定不会被修改时,都应该使用startForeach进行遍历
	// 当确定遍历过程中列表不会被修改时,才会使用getMainList进行过遍历
	const HashMap<Key, Value>& getMainList() const						{ return mMainList; }
	// 尝试获取值的指针,适用于value是非指针类型的列表
	Value* getPtr(const Key& key)										{ return mMainList.getPtr(key); }
	const Value* getPtrConst(const Key& key) const						{ return mMainList.getPtrConst(key); }
	// 获取值,如果获取失败,则返回设置的defaultValue
	Value get(const Key& key, const Value& defaultValue) const	{ return mMainList.get(key, defaultValue); }
	const Value& get(const Key& key) const								{ return mMainList.get(key); }
	bool contains(const Key& key) const									{ return mMainList.contains(key); }
	int size() const													{ return mMainList.size(); }
	bool isEmpty() const												{ return mMainList.isEmpty(); }
	bool addIf(const Key& key, const Value& value, bool condition)
	{
		if (condition)
		{
			return add(key, value);
		}
		return false;
	}
	bool add(const Key& key, const Value& value)
	{
		if (!mMainList.add(key, value))
		{
			return false;
		}
		// 只有当正在遍历中对列表进行修改时,才会记录修改操作
		if (mForeaching)
		{
			mModifyList.emplace(key, value);
		}
		// 没有在遍历,则对mUpdateList做与mMainList相同的操作
		else
		{
			mUpdateList.add(key, value);
		}
		return true;
	}
	bool removeIf(const Key& key, bool condition)
	{
		if (condition)
		{
			remove(key);
		}
		return false;
	}
	bool remove(const Key& key)
	{
		if (!mMainList.remove(key))
		{
			return false;
		}
		// 只有当正在遍历中对列表进行修改时,才会记录修改操作
		if (mForeaching)
		{
			mModifyList.emplace(key);
		}
		else
		{
			mUpdateList.remove(key);
		}
		return true;
	}
	bool remove(const Key& key, Value& value)
	{
		if (!mMainList.remove(key, value))
		{
			return false;
		}
		// 只有当正在遍历中对列表进行修改时,才会记录修改操作
		if (mForeaching)
		{
			mModifyList.emplace(key);
		}
		else
		{
			mUpdateList.remove(key);
		}
		return true;
	}
	void reserve(const int capacity)
	{
		mMainList.reserve(capacity);
		mUpdateList.reserve(capacity);
	}
	// Safe类型的容器不能使用tryInsert等返回引用的函数,这样会导致遍历的列表和存储的列表不一致,因为没办法将两个列表的引用返回出去
	// 清空所有数据,不能正在遍历时调用
	void clear()
	{
		if (mForeaching)
		{
			ERROR("正在遍历列表,无法清空列表");
		}
		mMainList.clear();
		mUpdateList.clear();
		mModifyList.clear();
	}
protected:
	void sync()
	{
		const int modifyCount = mModifyList.size();
		const int mainCount = mMainList.size();
		// 主列表为空,则直接清空列表即可
		if (mainCount == 0)
		{
			mUpdateList.clear();
		}
		else
		{
			// 更新操作较少,则遍历更新操作列表进行数据同步
			if (modifyCount < mainCount)
			{
				FOR(modifyCount)
				{
					auto& modifyValue = mModifyList[i];
					mUpdateList.addOrRemove(modifyValue.mKey, modifyValue.mValue, modifyValue.mAdd);
				}
			}
			// 更新操作较多,则直接复制列表
			else
			{
				mMainList.cloneTo(mUpdateList);
			}
		}
		mModifyList.clear();
	}
protected:
	Vector<MapModify<Key, Value>> mModifyList;	// 记录操作的列表
	HashMap<Key, Value> mUpdateList;			// 用于遍历更新的列表
	HashMap<Key, Value> mMainList;				// 用于存储实时数据的列表
	bool mForeaching = false;					// 是否正在遍历中
};