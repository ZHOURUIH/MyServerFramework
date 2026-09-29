#pragma once

#include <unordered_map>
#include "FrameCallbackClean.h"

using std::pair;
using std::initializer_list;
using std::unordered_map;

template<typename Key, typename Value>
class HashMap
{
public:
	typedef typename unordered_map<Key, Value>::iterator iterator;
	typedef typename unordered_map<Key, Value>::const_iterator const_iterator;
public:
	HashMap()
	{
		mMap.max_load_factor(0.25f);
		mMap.reserve(16);
	}
	explicit HashMap(int capacity)
	{
		mMap.max_load_factor(0.25f);
		mMap.reserve(capacity);
	}
	HashMap(const HashMap<Key, Value>& other):
		mMap(other.mMap)
	{
#ifdef WINDOWS
		if (mMap.size() > 0)
		{
			LOG("HashMap请尝试使用移动构造,减少拷贝构造");
		}
#endif
	}
	HashMap(HashMap<Key, Value>&& other) noexcept :
		mMap(move(other.mMap))
	{}
	explicit HashMap(initializer_list<pair<const Key, Value>> list)
	{
		mMap.insert(list);
	}
	HashMap<Key, Value>& operator=(HashMap<Key, Value>&& other) noexcept
	{
		mMap = move(other.mMap);
		return *this;
	}
	HashMap<Key, Value>& operator=(const HashMap<Key, Value>& other)
	{
		mMap = other.mMap;
#ifdef WINDOWS
		LOG("HashMap请尝试使用移动赋值,减少拷贝赋值");
#endif
		return *this;
	}
	virtual ~HashMap() { mMap.clear(); }
	iterator begin()					{ return mMap.begin(); }
	iterator end()						{ return mMap.end(); }
	const_iterator begin() const		{ return mMap.begin(); }
	const_iterator end() const			{ return mMap.end(); }
	const_iterator cbegin() const		{ return mMap.cbegin(); }
	const_iterator cend() const			{ return mMap.cend(); }
	// 获取列表中指定顺序的值,如果获取失败,则返回设置的defaultValue
	Value getAt(const int index, const Value& defaultValue) const
	{
		int curIndex = 0;
		for (const auto& iter : mMap)
		{
			if (index == curIndex++)
			{
				return iter.second;
			}
		}
		return defaultValue;
	}
	// 尝试获取值的指针,适用于value是非指针类型的列表,由于返回出去的值允许被修改,所以返回值不能添加const,此函数也不是const
	Value* getPtr(const Key& key)
	{
		auto iter = mMap.find(key);
		return iter != mMap.end() ? &(iter->second) : nullptr;
	}
	// 尝试获取值的指针,适用于value是非指针类型的列表,返回出去的值不允许被修改
	const Value* getPtrConst(const Key& key) const
	{
		auto iter = mMap.find(key);
		return iter != mMap.end() ? &(iter->second) : nullptr;
	}
	// 获取值,如果获取失败,则返回设置的defaultValue
	Value get(const Key& key, const Value& defaultValue) const
	{
		auto iter = mMap.find(key);
		return iter != mMap.end() ? iter->second : defaultValue;
	}
	// 获取值,如果获取失败,则返回默认值,适用于value是类对象,可以自动调用构造的
	const Value& get(const Key& key) const
	{
		auto iter = mMap.find(key);
		return iter != mMap.end() ? iter->second : mEmptyValue;
	}
	const_iterator find(const Key& key) const { return mMap.find(key); }
	iterator find(const Key& key) { return mMap.find(key); }
	bool contains(const Key& key) const { return mMap.find(key) != mMap.end(); }
	bool addOrRemove(const Key& key, const Value& value, bool isAdd)
	{
		if (isAdd)
		{
			add(key, value);
		}
		else
		{
			remove(key);
		}
		return isAdd;
	}
	// 尝试修改列表中的值,如果key存在则执行成功,key不存在则执行失败
	bool set(const Key& key, const Value& value)
	{
		auto iter = mMap.find(key);
		const bool result = iter != mMap.end();
		if (result)
		{
			iter->second = value;
		}
		return result;
	}
	// 尝试向列表中插入值或者更新对应键的值,返回值表示是否插入成功
	bool addOrSet(const Key& key, const Value& value)
	{
		// linux不支持try_emplace,所以为了统一,还是先查询再emplace
		auto iter = mMap.find(key);
		const bool insertSuccess = iter == mMap.end();
		if (insertSuccess)
		{
			mMap.emplace(key, value);
		}
		else
		{
			iter->second = value;
		}
		return insertSuccess;
	}
	// 尝试向列表中插入值或者更新对应键的值,返回值表示是否插入成功
	bool addOrSet(const Key& key, Value&& value)
	{
		// linux不支持try_emplace,所以为了统一,还是先查询再emplace
		auto iter = mMap.find(key);
		const bool insertSuccess = iter == mMap.end();
		if (insertSuccess)
		{
			mMap.emplace(key, move(value));
		}
		else
		{
			iter->second = move(value);
		}
		return insertSuccess;
	}
	// 向列表中插入键,插入成功则返回插入值的引用,如果key已经存在,则返回对应的value的引用
	Value& addOrGet(const Key& key)
	{
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			return mMap.emplace(key, mEmptyValue).first->second;
		}
		return iter->second;
	}
	// 向列表中插入键,插入成功则返回插入值的引用,如果key已经存在,则返回对应的value的引用
	Value& addOrGet(const Key& key, const Value& value)
	{
		// linux不支持try_emplace,所以为了统一,还是先查询再emplace
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			return mMap.emplace(key, value).first->second;
		}
		return iter->second;
	}
	// 向列表中插入键,插入成功则返回插入值的引用,如果key已经存在,则返回对应的value的引用
	Value& addOrGet(const Key& key, Value&& value)
	{
		// linux不支持try_emplace,所以为了统一,还是先查询再emplace
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			return mMap.emplace(key, move(value)).first->second;
		}
		return iter->second;
	}
	// 插入key和value,如果key存在则插入失败
	bool add(const Key& key, const Value& value)
	{
		return mMap.emplace(key, value).second;
	}
	// 插入key和value,如果key存在则插入失败
	bool add(const Key& key, Value&& value)
	{
		return mMap.emplace(key, move(value)).second;
	}
	// 插入key,value为默认值,如果key存在则插入失败
	bool add(const Key& key)
	{
		return mMap.emplace(key, mEmptyValue).second;
	}
	// 插入key和value,如果key存在则插入失败,condition为true才会执行插入
	bool addIf(const Key& key, const Value& value, bool condition)
	{
		if (!condition)
		{
			return false;
		}
		return mMap.emplace(key, value).second;
	}
	// 移除迭代器位置的元素,并返回下一个元素的迭代器
	iterator remove(const iterator& iter)
	{
		return mMap.erase(iter);
	}
	// 移除迭代器位置的元素,并返回下一个元素的迭代器
	bool removeOrNext(iterator& iter, bool condition)
	{
		if (condition)
		{
			iter = mMap.erase(iter);
		}
		else
		{
			++iter;
		}
		return condition;
	}
	void remove(const BoolFunctionT2<Key, Value>& condition, const CallbackT<Value>& willRemoveCallback)
	{
		for (auto iter = begin(); iter != end(); )
		{
			if (condition(iter->first, iter->second))
			{
				willRemoveCallback(iter->second);
				iter = remove(iter);
			}
			else
			{
				++iter;
			}
		}
	}
	void removeFirst(const BoolFunctionT2<Key, Value>& condition, const CallbackT<Value>& willRemoveCallback)
	{
		for (auto iter = begin(); iter != end(); ++iter)
		{
			if (condition(iter->first, iter->second))
			{
				willRemoveCallback(iter->second);
				remove(iter);
				break;
			}
		}
	}
	// 将key从列表中移除,如果移除成功,则将value设置为被移除的元素的second,一般用于指针类型
	bool remove(const Key& key, Value& value)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			value = iter->second;
			mMap.erase(iter);
			return true;
		}
		return false;
	}
	// 返回值表示移除成功或失败
	bool remove(const Key& key)
	{
		auto iter = mMap.find(key);
		if (iter != mMap.end())
		{
			mMap.erase(iter);
			return true;
		}
		return false;
	}
	// 返回值表示移除成功或失败
	bool removeIf(const Key& key, bool condition)
	{
		if (!condition)
		{
			return false;
		}
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			return false;
		}
		mMap.erase(iter);
		return true;
	}
	// 将key从列表中移除,如果移除成功,则将value设置为被移除的元素的second,一般用于指针类型
	bool removeIf(const Key& key, Value& value, bool condition)
	{
		if (!condition)
		{
			return false;
		}
		auto iter = mMap.find(key);
		if (iter == mMap.end())
		{
			return false;
		}
		value = iter->second;
		mMap.erase(iter);
		return true;
	}
	// 因为clear本身会执行元素的析构,即使列表为空,也会执行较多的指令,所以先排除空列表的不必要的清空操作
	void clear(bool disposeMemory = false)
	{
		if (mMap.empty())
		{
			return;
		}
		mMap.clear();
		if (disposeMemory)
		{
			dispose();
		}
	}
	void dispose()
	{
		unordered_map<Key, Value> temp;
		mMap.swap(temp);
	}
	bool addRange(const HashMap<Key, Value>& other)
	{
		bool success = true;
		for (const auto& iter : other)
		{
			success = mMap.emplace(iter.first, iter.second).second && success;
		}
		return success;
	}
	// 预留可容纳的元素数量,避免元素持续增加时反复rehash.
	void reserve(const int count)
	{
		if (count > 0)
		{
			mMap.reserve(count);
		}
	}
	int size() const { return (int)mMap.size(); }
	bool isEmpty() const { return (int)mMap.size() == 0; }
	Value& operator[](const Key& k) { return mMap[k]; }
	// 添加克隆函数的目的是为了显式调用拷贝,而非自动调用拷贝,可以避免可以使用移动构造而没有使用的情况
	void cloneTo(HashMap<Key, Value>& target) const
	{
		target.mMap = mMap;
	}
public:
	unordered_map<Key, Value> mMap;
	static const HashMap<Key, Value> mDefaultHashMap;
private:
	static Value mEmptyValue;
};

template<typename Key, typename Value>
Value HashMap<Key, Value>::mEmptyValue;

template<typename Key, typename Value>
const HashMap<Key, Value> HashMap<Key, Value>::mDefaultHashMap;