#pragma once

#include "TimePoint.h"

// 每周的指定周几,时,分
class MICRO_LEGEND_FRAME_API TimePointInWeek : public TimePoint
{
	BASE(TimePointInWeek, TimePoint);
public:
	void update(float elapsedTime) override;
	void setParam(const int weekDay, const int hour, const int minute)
	{
		mTime.mWeekDay = weekDay;
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