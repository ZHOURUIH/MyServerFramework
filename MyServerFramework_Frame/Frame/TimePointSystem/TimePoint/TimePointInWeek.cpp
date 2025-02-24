#include "FrameHeader.h"

void TimePointInWeek::update(const float elapsedTime)
{
	base::update(elapsedTime);
	if (TimeValue::isTimeInRangeInWeek(mTime, mTimePointSystem->getLastTime(), mTimePointSystem->getCurTime()))
	{
		arrivalTime();
	}
}