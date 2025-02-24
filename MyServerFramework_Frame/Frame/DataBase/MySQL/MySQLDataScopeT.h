#pragma once

#ifdef _MYSQL
#include "MySQLDataPool.h"
#include "CounterThreadPool.h"

template <typename T>
class MySQLDataScopeT
{
private:
    T* mData = nullptr;             // 原始指针
    Counter* mCounter = nullptr;    // 引用计数
public:
    // 默认构造函数
    MySQLDataScopeT() {}
    explicit MySQLDataScopeT(T* ptr) :
        mData(ptr)
    {
        if (mData != nullptr)
        {
            mCounter = mCounterThreadPool->newClass();
            mCounter->increase();
        }
    }
    // 拷贝构造函数
    MySQLDataScopeT(const MySQLDataScopeT& other) :
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
    MySQLDataScopeT(MySQLDataScopeT&& other) noexcept :
        mData(other.mData),
        mCounter(other.mCounter)
    {
        other.mData = nullptr;
        other.mCounter = nullptr;
    }
    // 移动赋值操作符
    MySQLDataScopeT& operator=(MySQLDataScopeT&& other) noexcept
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
    ~MySQLDataScopeT()
    {
        release();
    }
    // 指针访问操作符,只允许左值调用,右值不能调用,因为容易产生析构后才返回的问题
    T* operator->() const& { return mData; }
    T* operator->() const&& = delete;
    // 指针访问操作符,只允许左值调用,右值不能调用,因为容易产生析构后才返回的问题
    T* get() const& { return mData; }
    T* get() const&& = delete;
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