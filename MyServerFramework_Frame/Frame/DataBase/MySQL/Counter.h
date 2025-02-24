#pragma once

#include "ClassPooledObject.h"

class Counter : public ClassPooledObject
{
	BASE(Counter, ClassPooledObject);
protected:
    atomic<int> mCount;
public:
    Counter() : mCount(0) {}
    int increase() { return ++mCount; }
    int decrease() { return --mCount; }
    bool isZero() { return mCount == 0; }
    void resetProperty() override
    {
        base::resetProperty();
        mCount = 0;
    }
};