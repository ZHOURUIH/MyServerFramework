#pragma once

#include "TimePoint.h"

// 每月的指定日,时,分
class MICRO_LEGEND_FRAME_API TimePointInYear : public TimePoint
{
	BASE(TimePointInYear, TimePoint);
public:
	void update(float elapsedTime) override;
	void setParam(const int month, const int day, const int hour, const int minute, const int second)
	{
		mTime.mMonth = month;
		mTime.mDay = day;
		mTime.mHour = hour;
		mTime.mMinute = minute;
		mTime.mSecond = second;
	}
	void resetProperty() override
	{
		base::resetProperty();
	}
protected:
};