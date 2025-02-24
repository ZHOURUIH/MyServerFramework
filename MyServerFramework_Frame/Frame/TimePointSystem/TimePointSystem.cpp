#include "FrameHeader.h"

void TimePointSystem::init()
{
	mCurTime.refreshTime();
	mLastTime.refreshTime();
}

void TimePointSystem::quit()
{
	mTimePointPool->destroyClassList(mTimePointList);
	mTimePointList.clear();
}

void TimePointSystem::update(const float elapsedTime)
{
	mCurTime.refreshTime();
	for (TimePoint* time : mTimePointList)
	{
		time->update(elapsedTime);
	}
	mLastTime = mCurTime;
}

void TimePointSystem::lateUpdate(const float elapsedTime)
{
	for (TimePoint* time : mTimePointList)
	{
		time->lateUpdate(elapsedTime);
	}
}

void TimePointSystem::destroyTimePoint(TimePoint* timePoint)
{
	// 不在列表中,则可能是已经被销毁了
	if (!mTimePointList.eraseElement(timePoint))
	{
		timePoint = nullptr;
		return;
	}
	mTimePointPool->destroyClass(timePoint);
}