#pragma once

#include "TimePoint.h"

// 每天的指定时,分
class MICRO_LEGEND_FRAME_API TimePointInDay : public TimePoint
{
	BASE(TimePointInDay, TimePoint);
public:
	void update(float elapsedTime) override;
	void setParam(const int hour, const int minute)
	{
		mTime.mHour = hour;
		mTime.mMinute = minute;
		mTime.mSecond = 0;
	}
	void resetProperty() override
	{
		base::resetProperty();
	}
protected:
};