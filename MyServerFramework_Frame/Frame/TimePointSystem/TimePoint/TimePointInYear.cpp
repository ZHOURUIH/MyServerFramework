#include "FrameHeader.h"

void TimePointInYear::update(const float elapsedTime)
{
	base::update(elapsedTime);
	if (TimeValue::isTimeInRangeInYear(mTime, mTimePointSystem->getLastTime(), mTimePointSystem->getCurTime()))
	{
		arrivalTime();
	}
}