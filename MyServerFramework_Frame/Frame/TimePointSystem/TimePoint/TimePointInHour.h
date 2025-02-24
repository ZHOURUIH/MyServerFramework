#pragma once

#include "TimePoint.h"

// 每小时的指定分
class MICRO_LEGEND_FRAME_API TimePointInHour : public TimePoint
{
	BASE(TimePointInHour, TimePoint);
public:
	void update(float elapsedTime) override;
	void setParam(const int minute)
	{
		mTime.mMinute = minute;
		mTime.mSecond = 0;
	}
	void resetProperty() override
	{
		base::resetProperty();
	}
protected:
};