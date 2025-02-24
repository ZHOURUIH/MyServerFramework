#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API TimeValue
{
public:
	void clear();
	void refreshTime() { getTime(mWeekDay, mYear, mMonth, mDay, mHour, mMinute, mSecond); }
	static bool isTimeInRangeInHour(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInDay(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInWeek(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInMonth(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	static bool isTimeInRangeInYear(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime);
	bool operator>(const TimeValue& other) const
	{
		// 转换成秒数来计算
		const long seconds0 = 60 * 60 * 24 * 365 * mYear + 60 * 60 * 24 * 31 * mMonth + 60 * 60 * 24 * mDay + 60 * 60 * mHour + 60 * mMinute + mSecond;
		const long seconds1 = 60 * 60 * 24 * 365 * other.mYear + 60 * 60 * 24 * 31 * other.mMonth + 60 * 60 * 24 * other.mDay + 60 * 60 * other.mHour + 60 * other.mMinute + other.mSecond;
		return seconds0 > seconds1;
	}
public:
	int mWeekDay = 0;
	int mYear = 0;
	int mMonth = 0;
	int mDay = 0;
	int mHour = 0;
	int mMinute = 0;
	int mSecond = 0;
};