#include "FrameHeader.h"

void TimeTask::destroy()
{
	mTimePointSystem->destroyTimePoint(mTimePoint);
}

void TimeTask::resetProperty()
{
	base::resetProperty();
	mTimePoint = nullptr;
}