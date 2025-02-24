#include "FrameHeader.h"

void TimePointInMonth::update(const float elapsedTime)
{
	base::update(elapsedTime);
	if (TimeValue::isTimeInRangeInMonth(mTime, mTimePointSystem->getLastTime(), mTimePointSystem->getCurTime()))
	{
		arrivalTime();
	}
}