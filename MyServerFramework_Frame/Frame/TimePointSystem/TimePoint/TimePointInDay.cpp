#include "FrameHeader.h"

void TimePointInDay::update(const float elapsedTime)
{
	base::update(elapsedTime);
	if (TimeValue::isTimeInRangeInDay(mTime, mTimePointSystem->getLastTime(), mTimePointSystem->getCurTime()))
	{
		arrivalTime();
	}
}