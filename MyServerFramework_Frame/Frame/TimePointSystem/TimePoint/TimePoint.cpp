#include "FrameHeader.h"

void TimePoint::update(float elapsedTime)
{
	tickTimerOnce(mArrivalCD, elapsedTime);
}

void TimePoint::resetProperty()
{
	base::resetProperty();
	mCallback = nullptr;
	mUserData = nullptr;
	mTime.clear();
	mArrivalCD = -1.0f;
	mArrivalTime = false;
}

void TimePoint::lateUpdate(const float elapsedTime)
{
	mArrivalTime = false;
}

void TimePoint::arrivalTime()
{
	if (mArrivalCD > 0.0f)
	{
		return;
	}
	mArrivalTime = true;
	mArrivalCD = 2.0f;
	mCallback(mUserData);
}