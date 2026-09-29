#pragma once

#include <unordered_set>
#include "Array.h"
#include "ArrayList.h"
#include "Vector.h"

using std::unordered_set;

template<typename T>
class Set
{
public:
	typedef typename unordered_set<T>::iterator iterator;
	typedef typename unordered_set<T>::const_iterator const_iterator;
public:
	virtual ~Set() { mSet.clear(); }
	Set() = default;
	Set(const Set<T>& other) :
		mSet(other.mSet)
	{
#ifdef WINDOWS
		if (mSet.size() > 0)
		{
			LOG("Set请尝试使用移动构造,减少拷贝构造");
		}
#endif
	}
	Set(Set<T>&& other) noexcept :
		mSet(move(other.mSet))
	{}
	Set<T>& operator=(Set<T>&& other) noexcept
	{
		mSet = move(other.mSet);
		return *this;
	}
	Set<T>& operator=(const Set<T>& other)
	{
		mSet = other.mSet;
#ifdef WINDOWS
		LOG("Set请尝试使用移动赋值,减少拷贝赋值");
#endif
		return *this;
	}
	const_iterator find(const T& elem) const	{ return mSet.find(elem); }
	iterator begin()							{ return mSet.begin(); }
	iterator end()								{ return mSet.end(); }
	const_iterator begin() const				{ return mSet.begin(); }
	const_iterator end() const					{ return mSet.end(); }
	const_iterator cbegin() const				{ return mSet.cbegin(); }
	const_iterator cend() const					{ return mSet.cend(); }
	// 返回第一个元素,并且将该元素移除
	T popFirst()
	{
		auto iter = mSet.begin();
		const T value = *iter;
		mSet.erase(iter);
		return value;
	}
	template<int Length>
	void setData(const Array<Length, T>& values, const int count)
	{
		FOR(count)
		{
			mSet.insert(values[i]);
		}
	}
	template<int Length>
	void setData(const ArrayList<Length, T>& values)
	{
		for (const T& item : values)
		{
			mSet.insert(item);
		}
	}
	void setData(const T* values, const int count)
	{
		FOR(count)
		{
			mSet.insert(values[i]);
		}
	}
	void setData(const Vector<T>& values)
	{
		for (const auto& value : values)
		{
			mSet.insert(value);
		}
	}
	bool addOrRemove(const T& elem, bool isAdd)
	{
		if (isAdd)
		{
			add(elem);
		}
		else
		{
			remove(elem);
		}
		return isAdd;
	}
	bool add(const T& elem)
	{
		return mSet.insert(elem).second;
	}
	bool addIf(const T& elem, bool condition)
	{
		if (!condition)
		{
			return false;
		}
		return mSet.insert(elem).second;
	}
	// 因为要兼容T类型的派生类,所以定义了一个新的类型
	template<typename TElement>
	void addRange(const Set<TElement>& other)
	{
		if (other.isEmpty())
		{
			return;
		}
		for (const TElement& iter : other)
		{
			mSet.insert(iter);
		}
	}
	// 因为要兼容T类型的派生类,所以定义了一个新的类型
	template<typename TElement>
	void addRange(const Vector<TElement>& other)
	{
		if (other.isEmpty())
		{
			return;
		}
		for (const TElement& iter : other)
		{
			mSet.insert(iter);
		}
	}
	void remove(const const_iterator& iter)
	{
		mSet.erase(iter);
	}
	bool remove(const T& value)
	{
		iterator iter = mSet.find(value);
		if (iter != mSet.end())
		{
			mSet.erase(iter);
			return true;
		}
		return false;
	}
	bool contains(const T& value) const { return mSet.find(value) != mSet.end(); }
	void clear()
	{
		if (mSet.empty())
		{
			return;
		}
		mSet.clear();
	}
	int size() const { return (int)mSet.size(); }
	bool isEmpty() const { return (int)mSet.size() == 0; }
	// 添加克隆函数的目的是为了显式调用拷贝,而非自动调用拷贝,可以避免可以使用移动构造而没有使用的情况
	void cloneTo(Set<T>& target) const
	{
		target.mSet = mSet;
	}
public:
	unordered_set<T> mSet;
private:
	static const Set<T> mDefaultList;
};

template<typename T>
const Set<T> Set<T>::mDefaultList;