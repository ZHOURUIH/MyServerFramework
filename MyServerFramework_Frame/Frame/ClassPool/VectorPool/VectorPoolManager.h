#pragma once

#include "VectorPool.h"
#include "VectorPoolThread.h"
#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API VectorPoolManager : public FrameComponent
{
	BASE(VectorPoolManager, FrameComponent);
public:
	~VectorPoolManager() override;
	template<typename T>
	static VectorPoolThread<T>* getPoolThread()
	{
		return static_cast<VectorPoolThread<T>*>(mVectorPoolThreadList.tryGet((llong)typeid(T).hash_code()));
	}
	template<typename T>
	void registeVectorPool()
	{
		if (!mVectorPoolThreadList.insert((llong)typeid(T).hash_code(), new VectorPoolThread<T>()))
		{
			ERROR("注册列表对象池失败");
		}
	}
protected:
	static HashMap<llong, VectorPoolBase*> mVectorPoolThreadList;
};