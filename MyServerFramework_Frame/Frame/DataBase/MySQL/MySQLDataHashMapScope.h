#pragma once
#ifdef _MYSQL

#include "HashMap.h"
#include "MySQLDataPool.h"
#include "CounterThreadPool.h"

class MySQLDataHashMapScope
{
private:
    HashMap<llong, MySQLData*> mList;       // 原始数据
    Counter* mCounter = nullptr;            // 引用计数
public:
    // 默认构造函数
    MySQLDataHashMapScope() {}
    explicit MySQLDataHashMapScope(HashMap<llong, MySQLData*>&& list)
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
    MySQLDataHashMapScope(const MySQLDataHashMapScope& other) :
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
    MySQLDataHashMapScope(MySQLDataHashMapScope&& other) noexcept :
        mList(move(other.mList)),
        mCounter(other.mCounter)
    {
        other.mCounter = nullptr;
    }
    // 移动赋值操作符
    MySQLDataHashMapScope& operator=(MySQLDataHashMapScope&& other) noexcept
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
    ~MySQLDataHashMapScope()
    {
        release();
    }
    const HashMap<llong, MySQLData*>& get() const& { return mList; }
    const HashMap<llong, MySQLData*>& get() const&& = delete;
    bool isValid() const { return mList.size() != 0; }
    // 如果已经确认data在外部被销毁了,则可以从当前列表中移除
    void erase(MySQLData* data) { mList.erase(data->mID); }
    void erase(const Vector<MySQLData*>& dataList)
    {
        for (MySQLData* data : dataList)
        {
            mList.erase(data->mID);
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