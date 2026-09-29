#pragma once

#include "SafeFastDeepVector.h"

// 需要通过SAFE_VECTOR_0_SCOPE宏来使用
template<typename T>
class SafeFastDeepVectorScope
{
protected:
	SafeFastDeepVector<T>* mList = nullptr;
public:
	SafeFastDeepVectorScope(SafeFastDeepVector<T>& list)
	{
		mList = &list;
		mList->startForeach();
	}
	~SafeFastDeepVectorScope()
	{
		mList->endForeach();
	}
};