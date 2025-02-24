#pragma once

#ifdef _MYSQL
#include "MySQLDataPool.h"
#include "MySQLDataScopeT.h"
#include "CounterThreadPool.h"

class MySQLDataScope
{
private:
    MySQLData* mData = nullptr;     // 原始指针
    Counter* mCounter = nullptr;    // 引用计数
public:
    MySQLDataScope() {}
    // 默认构造函数
    explicit MySQLDataScope(MySQLData* ptr) :
        mData(ptr)
    {
        if (mData != nullptr)
        {
            mCounter = mCounterThreadPool->newClass();
            mCounter->increase();
        }
    }
    // 拷贝构造函数
    MySQLDataScope(const MySQLDataScope& other) :
        mData(other.mData),
        mCounter(other.mCounter)
    {
        if (mData == nullptr)
        {
            return;
        }
        mCounter->increase();
    }
    // 移动构造函数
    MySQLDataScope(MySQLDataScope&& other) noexcept :
        mData(other.mData),
        mCounter(other.mCounter)
    {
        other.mData = nullptr;
        other.mCounter = nullptr;
    }
    // 移动赋值操作符
    MySQLDataScope& operator=(MySQLDataScope&& other) noexcept
    {
        if (this != &other)
        {
            // 先减少当前指针的引用计数
            release();

            // 复制新对象的内容
            mData = other.mData;
            mCounter = other.mCounter;
            other.mData = nullptr;
            other.mCounter = nullptr;
        }
        return *this;
    }
    // 析构函数
    ~MySQLDataScope()
    {
        release();
    }
    // 指针访问操作符
    MySQLData* operator->() const& { return mData; }
    MySQLData* operator->() const&& = delete;
    MySQLData* get() const& { return mData; }
    MySQLData* get() const&& = delete;
    bool isValid() const { return mData != nullptr; }
    void release()
    {
        // 先减少当前指针的引用计数
        if (mCounter != nullptr && mCounter->decrease() == 0)
        {
            mMySQLDataPool->destroyClass(mData);
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
                mData = nullptr;
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