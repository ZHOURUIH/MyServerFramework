#pragma once
#ifdef _MYSQL

#include "Vector.h"
#include "MySQLDataPool.h"
#include "CounterThreadPool.h"

class MySQLDataListScope
{
private:
    Vector<MySQLData*> mList;       // 原始数据
    Counter* mCounter = nullptr;    // 引用计数
public:
    // 默认构造函数
    MySQLDataListScope() {}
    explicit MySQLDataListScope(Vector<MySQLData*>&& list)
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
    MySQLDataListScope(const MySQLDataListScope& other) :
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
    MySQLDataListScope(MySQLDataListScope&& other) noexcept :
        mList(move(other.mList)),
        mCounter(other.mCounter)
    {
        other.mCounter = nullptr;
    }
    // 移动赋值操作符
    MySQLDataListScope& operator=(MySQLDataListScope&& other) noexcept
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
    ~MySQLDataListScope()
    {
        release();
    }
    // 指针访问操作符
    const Vector<MySQLData*>& get() const& { return mList; }
    const Vector<MySQLData*>& get() const&& = delete;
    bool isValid() const { return mList.size() != 0; }
    // 如果已经确认data在外部被销毁了,则可以从当前列表中移除
    void erase(MySQLData* data) { mList.eraseElement(data); }
    void erase(const Vector<MySQLData*>& dataList)
    {
        for (MySQLData* data : dataList)
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