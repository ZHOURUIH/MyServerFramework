#pragma once

#include "IEventListener.h"
#include "FrameClassDeclare.h"
#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API ClassPooledObject : public IEventListener
{
	BASE(ClassPooledObject, IEventListener);
public:
	void markUsable(FrameComponent* classPool, const llong assignID)
	{
		mCreateSourcePool = classPool;
		mAssignID = assignID;
		mDestroy = false;
	}
	bool markDispose(FrameComponent* classPool)
	{
		if (mCreateSourcePool != classPool || mDestroy)
		{
			return false;
		}
		resetProperty();
		return true;
	}
	FrameComponent* getPool() const			{ return mCreateSourcePool; }
	llong getAssignID() const				{ return mAssignID; }
	bool isPendingDestroy() const			{ return mPendingDestroy; }
	bool isDestroy() const					{ return mDestroy; }
	void setPendingDestroy()				{ mPendingDestroy = true; }
	virtual void resetProperty()
	{
		mCreateSourcePool = nullptr;
		mAssignID = 0;
		mPendingDestroy = false;
		mDestroy = true;
	}
protected:
	FrameComponent* mCreateSourcePool = nullptr;// 所属的对象池
	llong mAssignID = 0;						// 被分配时的唯一ID,每次分配都会设置一个新的唯一执行ID
	bool mPendingDestroy = false;				// 是否已经加入销毁队列,部分类会有延迟销毁的操作,需要标记是否即将被销毁,即将被销毁的对象不应该再执行任何逻辑
	bool mDestroy = true;						// 当前对象是否已经被回收
};