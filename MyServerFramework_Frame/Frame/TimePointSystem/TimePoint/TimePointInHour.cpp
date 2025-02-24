#include "FrameHeader.h"

void TimePointInHour::update(const float elapsedTime)
{
	base::update(elapsedTime);
	if (TimeValue::isTimeInRangeInHour(mTime, mTimePointSystem->getLastTime(), mTimePointSystem->getCurTime()))
	{
		arrivalTime();
	}
}