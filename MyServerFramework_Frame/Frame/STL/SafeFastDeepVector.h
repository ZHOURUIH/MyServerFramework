#pragma once

#include "Vector.h"

// 可在遍历中再次开始遍历,删除,增加的列表,并且遍历时只能通过下标访问,且循环次数不能动态改变
// 不过由于删除不是立即删除的,所以在部分情况下使用时需要注意
// 而且会将默认值认为是被删除的值,比如nullptr,0等
// 需要搭配SAFE_VECTOR_0_SCOPE宏来进行遍历
// SafeFastDeepVector<int> list;
// SAFE_VECTOR_0_SCOPE(list);
// FOR(list.size())
// {}
template<typename T>
class SafeFastDeepVector
{
public:
    typedef T ValueType;
public:
    void startForeach()
    {
        ++mForeachDepth;
    }
    void endForeach()
    {
        if (--mForeachDepth == 0 && mNeedCompact)
        {
            compact();
        }
    }
    bool isEmpty() const { return mMainList.isEmpty(); }
    bool isForeaching() const { return mForeachDepth > 0; }
    const Vector<T>& getMainList() const { return mMainList; }
    int size() const { return mMainList.size(); }
    // 根据下标获取元素,如果下标不合法则返回默认值
    decltype(auto) get(const int index) const
    {
        return mMainList.get(index);
    }
    void add(const T& value)
    {
        mMainList.add(value);
    }
    bool remove(const T& value)
    {
        int index = mMainList.findIndex(value);
        if (index < 0)
        {
            return false;
        }
        if (mForeachDepth > 0)
        {
            mMainList[index] = T();   // 标记删除（例如 nullptr）
            mNeedCompact = true;
        }
        else
        {
            mMainList.removeAt(index);
        }
        return true;
    }
    void clear()
    {
        if (mForeachDepth > 0)
        {
            for (auto& v : mMainList)
            {
                v = T();
            }
            mNeedCompact = true;
        }
        else
        {
            mMainList.clear();
        }
    }
protected:
    void compact()
    {
        int write = 0;
        FOR_VECTOR(mMainList)
        {
            if (mMainList[i] != T())
            {
                mMainList[write++] = mMainList[i];
            }
        }
        mMainList.resize(write);
        mNeedCompact = false;
    }
protected:
    Vector<T> mMainList;
    int mForeachDepth = 0;
    bool mNeedCompact = false;
};