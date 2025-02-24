#pragma once
#ifdef _MYSQL

#include "Vector.h"
#include "MySQLDataPool.h"
#include "CounterThreadPool.h"

template<typename T>
class MySQLDataListScopeT
{
private:
    Vector<T*> mList;               // 原始数据
    Counter* mCounter = nullptr;    // 引用计数
public:
    // 默认构造函数
    MySQLDataListScopeT(){}
    explicit MySQLDataListScopeT(Vector<MySQLData*>&& list)
    {
        if (list.size() == 0)
        {
            return;
        }
        mList.reserve(list.size());
        for (MySQLData* data : list)
        {
            mList.push_back(static_cast<T*>(data));
        }
        mCounter = mCounterThreadPool->newClass();
        mCounter->increase();
    }
    explicit MySQLDataListScopeT(Vector<T*>&& list)
    {
        if (list.size() == 0)
        {
            return;
        }
        mList = move(list);
        mCounter = mCounterThreadPool->newClass();
        mCounter->increase();
    }
    // 拷贝构造函数
    MySQLDataListScopeT(const MySQLDataListScopeT& other) :
        mCounter(other.mCounter)
    {
        if (other.mList.size() == 0)
        {
            return;
        }
        other.mList.clone(mList);
        mCounter->increase();
    }
    // 移动构造函数
    MySQLDataListScopeT(MySQLDataListScopeT&& other) noexcept :
        mList(move(other.mList)),
        mCounter(other.mCounter)
    {
        other.mCounter = nullptr;
    }
    // 移动赋值操作符
    MySQLDataListScopeT& operator=(MySQLDataListScopeT&& other) noexcept
    {
        if (this != &other)
        {
            // 先减少当前指针的引用计数
            release();

            // 复制新对象的内容
            mList = move(other.mList);
            mCounter = other.mCounter;
            other.mCounter = nullptr;
        }
        return *this;
    }
    // 析构函数
    ~MySQLDataListScopeT()
    {
        release();
    }
    // 指针访问操作符,只允许左值调用,右值不能调用,因为容易产生析构后才返回的问题
    const Vector<T*>& get() const& { return mList; }
    const Vector<T*>& get() const&& = delete;
    bool isValid() const { return mList.size() != 0; }
    // 如果已经确认data在外部被销毁了,则可以从当前列表中移除
    void erase(T* data) { mList.eraseElement(data); }
    void erase(const Vector<T*>& dataList)
    {
        for (T* data : dataList)
        {
            mList.eraseElement(data);
        }
    }
    void release()
    {
        // 先减少当前指针的引用计数
        if (mCounter != nullptr && mCounter->decrease() == 0)
        {
            mMySQLDataPool->destroyClassList(mList);
            mCounterThreadPool->destroyClass(mCounter);
            mCounter = nullptr;
        }
    }
    void releaseNoDestroy()
    {
        // 先减少当前指针的引用计数
        if (mCounter != nullptr)
        {
            if (mCounter->decrease() == 0)
            {
                mList.clear();
                mCounterThreadPool->destroyClass(mCounter);
                mCounter = nullptr;
            }
            else
            {
                ERROR("计数不为0");
            }
        }
    }
};

#endif