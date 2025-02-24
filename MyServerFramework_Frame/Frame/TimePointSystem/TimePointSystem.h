#pragma once

#include "FrameComponent.h"
#include "TimeValue.h"
#include "TimePointPool.h"
#include "TimePoint.h"

class MICRO_LEGEND_FRAME_API TimePointSystem : public FrameComponent
{
	BASE(TimePointSystem, FrameComponent);
public:
	void init() override;
	void quit() override;
	template<typename T, typename TypeCheck = typename IsSubClassOf<TimePoint, T>::mType>
	T* createTimePoint()
	{
		T* timePoint = mTimePointPool->newClass<T>();
		timePoint->init();
		mTimePointList.push_back(timePoint);
		return timePoint;
	}
	void destroyTimePoint(TimePoint* timePoint);
	void update(float elapsedTime) override;
	void lateUpdate(float elapsedTime) override;
	const TimeValue& getCurTime() const { return mCurTime; }
	const TimeValue& getLastTime() const { return mLastTime; }
protected:
	Vector<TimePoint*> mTimePointList;
	TimeValue mCurTime;
	TimeValue mLastTime;
};