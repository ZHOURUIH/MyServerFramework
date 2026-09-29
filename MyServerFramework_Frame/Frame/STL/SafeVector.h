#pragma once

#include "Vector.h"
#include "ValueModify.h"

// 支持在遍历列表时对列表进行插入和删除,但是不能嵌套遍历
// 需要搭配SAFE_VECTOR_SCOPE宏来进行遍历
// SafeVector<int> list;
// SAFE_VECTOR_SCOPE(list, readList);
// for (const int item : readList)
// {}
template<typename T>
class SafeVector
{
public:
	typedef Vector<T> List;
	typedef T ValueType;
public:
	virtual ~SafeVector() { clear(); }
	const Vector<T>& startForeach()
	{
		if (mForeaching)
		{
			ERROR("正在遍历列表,无法再次开始遍历");
		}
		mForeaching = true;
		mModified = false;
		return mUpdateList;
	}
	void endForeach()
	{
		mForeaching = false;
		if (mHasResetCount > 0)
		{
			mUpdateList.clearDefaultElement(mHasResetCount);
			mHasResetCount = 0;
		}
		// 在结束遍历时同步一次
		if (mModified)
		{
			sync();
		}
	}
	bool isForeaching() const				{ return mForeaching; }
	// 获取主列表,存储着当前实时的数据列表,所有的删除和新增都会立即更新此列表
	// 当遍历过程中不能确定列表一定不会被修改时,都应该使用startForeach进行遍历
	// 当确定遍历过程中列表不会被修改时,才会使用getMainList进行过遍历
	const Vector<T>& getMainList() const	{ return mMainList; }
	void moveMainListTo(Vector<T>& target)
	{
		// 只有当正在遍历中对列表进行修改时,才会记录修改操作
		if (mForeaching)
		{
			for (const T& item : mMainList)
			{
				mModifyList.emplace(item, false);
			}
		}
		else
		{
			mUpdateList.clear();
		}
		target = move(mMainList);
		mModified = true;
	}
	int size() const						{ return mMainList.size(); }
	bool isEmpty() const					{ return mMainList.isEmpty(); }
	T* data() const							{ return mMainList.data(); }
	bool contains(const T& value) const		{ return mMainList.contains(value); }
	void add(const T& value)
	{
		mMainList.add(value);
		// 只有当正在遍历中对列表进行修改时,才会记录修改操作
		if (mForeaching)
		{
			mModifyList.emplace(value, true);
		}
		else
		{
			mUpdateList.add(value);
		}
		mModified = true;
	}
	bool removeAt(const int index)
	{
		if (index < 0 || index >= mMainList.size())
		{
			return false;
		}
		// 只有当正在遍历中对列表进行修改时,才会记录修改操作
		if (mForeaching)
		{
			mModifyList.emplace(mMainList[index], false);
		}
		else
		{
			mUpdateList.remove(mMainList[index]);
		}
		mMainList.removeAt(index);
		mModified = true;
		return true;
	}
	// 在遍历中移除元素有两种选择
	// 一种是直接移除,但是重置之后这一帧仍然可能会对移除的元素进行遍历,因为不一定移除的是当前元素之前还是之后的
	// 另外一种是重置元素为默认值,比如设置为空指针,由于不会修改列表,所以可以立即执行,重置之后这一帧就不会再去遍历
	// 不过在重置时,结束遍历时需要将列表中所有已经重置的元素进行清除,确保列表中不会有多余的元素
	// 而且重置的效率实际上比较低,因为每一次的遍历结束后都要再次遍历一次,相当于遍历了两次,但是运行结果是最准确的
	bool remove(const T& value)
	{
		if (!mMainList.remove(value))
		{
			return false;
		}
		// 正在遍历中,则只是重置元素
		if (mForeaching)
		{
			// 无论当前mUpdateList中有没有这个元素,都必须记录删除操作
			// mModifyList描述的是本轮对实时列表发生的完整修改
			mModifyList.emplace(value, false);
			// 如果元素原本就在当前遍历列表中,则立即重置,避免本轮继续遍历它
			if (mUpdateList.resetElement(value))
			{
				++mHasResetCount;
			}
		}
		// 不在遍历中,则可以直接移除
		else
		{
			mUpdateList.remove(value);
		}
		mModified = true;
		return true;
	}
	// 清空所有数据
	void clear()
	{
		if (mForeaching)
		{
			for (const T& item : mMainList)
			{
				mModifyList.emplace(item, false);
			}
		}
		else
		{
			mModifyList.clear();
			mUpdateList.clear();
		}
		mMainList.clear();
		mModified = true;
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
					mUpdateList.addOrRemove(modifyValue.mValue, modifyValue.mAdd);
				}
			}
			// 更新操作较多,则直接复制列表
			else
			{
				mMainList.cloneTo(mUpdateList);
			}
		}
		mModifyList.clear();
		mModified = false;
	}
public:
	static const SafeVector<T> mDefaultList;
protected:
	Vector<ValueModify<T>> mModifyList;	// 记录操作的列表
	Vector<T> mUpdateList;				// 用于遍历更新的列表
	Vector<T> mMainList;				// 用于存储实时数据的列表
	bool mForeaching = false;			// 是否正在遍历中
	bool mModified = false;				// 是否有被修改,如果被修改,则在结束遍历时需要同步
	int mHasResetCount = 0;				// 在遍历中被重置的元素个数
private:
	static const T mDefaultValue;
};

template<typename T>
const T SafeVector<T>::mDefaultValue = T();

template<typename T>
const SafeVector<T> SafeVector<T>::mDefaultList;