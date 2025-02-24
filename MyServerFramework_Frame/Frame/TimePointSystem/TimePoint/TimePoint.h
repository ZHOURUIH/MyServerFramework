#pragma once

#include "ClassPooledObject.h"
#include "TimeValue.h"

class MICRO_LEGEND_FRAME_API TimePoint : public ClassPooledObject
{
	BASE(TimePoint, ClassPooledObject);
public:
	virtual void init(){}
	virtual void update(float elapsedTime);
	virtual void lateUpdate(float elapsedTime);
	void resetProperty() override;
	void setCallback(VoidPtrCallback callback, void* userData) 
	{
		mCallback = callback; 
		mUserData = userData;
	}
protected:
	void arrivalTime();
protected:
	VoidPtrCallback mCallback = nullptr;	// 到时间的回调
	void* mUserData = nullptr;
	TimeValue mTime;
	float mArrivalCD = -1.0f;				// 因为时间只能精确到秒,所以需要加一个2秒内只能触发一次mArrivalTime为true的限制
	bool mArrivalTime = false;
};