#include "FrameHeader.h"

void TimeValue::clear()
{
	mWeekDay = 0;
	mYear = 0;
	mMonth = 0;
	mDay = 0;
	mHour = 0;
	mMinute = 0;
	mSecond = 0;
}

bool TimeValue::isTimeInRangeInHour(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime)
{
	// 如果minTime大于maxTime,则认为时间无效
	if (minTime > maxTime)
	{
		return false;
	}
	const int timeSecond = time.mMinute * 60 + time.mSecond;
	const int minTimeSecond = minTime.mMinute * 60 + minTime.mSecond;
	const int maxTimeSecond = maxTime.mMinute * 60 + maxTime.mSecond;
	// 时间范围都在同一小时
	if (minTimeSecond <= maxTimeSecond)
	{
		return timeSecond >= minTimeSecond && timeSecond <= maxTimeSecond;
	}
	// 时间范围不在同一小时,中间跨过了0分
	else
	{
		return timeSecond >= minTimeSecond || timeSecond <= maxTimeSecond;
	}
}

bool TimeValue::isTimeInRangeInDay(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime)
{
	// 如果minTime大于maxTime,则认为时间无效
	if (minTime > maxTime)
	{
		return false;
	}
	const int timeSecond = time.mHour * 3600 + time.mMinute * 60 + time.mSecond;
	const int minTimeSecond = minTime.mHour * 3600 + minTime.mMinute * 60 + minTime.mSecond;
	const int maxTimeSecond = maxTime.mHour * 3600 + maxTime.mMinute * 60 + maxTime.mSecond;
	// 时间范围都在同一天
	if (minTimeSecond <= maxTimeSecond)
	{
		return timeSecond >= minTimeSecond && timeSecond <= maxTimeSecond;
	}
	// 时间范围不在同一天,中间跨过了0点
	else
	{
		return timeSecond >= minTimeSecond || timeSecond <= maxTimeSecond;
	}
}

bool TimeValue::isTimeInRangeInWeek(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime)
{
	// 如果minTime大于maxTime,则认为时间无效
	if (minTime > maxTime)
	{
		return false;
	}
	const int timeSecond = time.mWeekDay * 3600 * 24 + time.mHour * 3600 + time.mMinute * 60 + time.mSecond;
	const int minTimeSecond = minTime.mWeekDay * 3600 * 24 + minTime.mHour * 3600 + minTime.mMinute * 60 + minTime.mSecond;
	const int maxTimeSecond = maxTime.mWeekDay * 3600 * 24 + maxTime.mHour * 3600 + maxTime.mMinute * 60 + maxTime.mSecond;
	// 时间范围都在同一周
	if (minTimeSecond <= maxTimeSecond)
	{
		return timeSecond >= minTimeSecond && timeSecond <= maxTimeSecond;
	}
	// 时间范围不在同一周,中间跨过了周日0点
	else
	{
		return timeSecond >= minTimeSecond || timeSecond <= maxTimeSecond;
	}
}

bool TimeValue::isTimeInRangeInMonth(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime)
{
	// 如果minTime大于maxTime,则认为时间无效
	if (minTime > maxTime)
	{
		return false;
	}
	const int timeSecond = time.mDay * 3600 * 24 + time.mHour * 3600 + time.mMinute * 60 + time.mSecond;
	const int minTimeSecond = minTime.mDay * 3600 * 24 + minTime.mHour * 3600 + minTime.mMinute * 60 + minTime.mSecond;
	const int maxTimeSecond = maxTime.mDay * 3600 * 24 + maxTime.mHour * 3600 + maxTime.mMinute * 60 + maxTime.mSecond;
	// 时间范围都在同一月
	if (minTimeSecond <= maxTimeSecond)
	{
		return timeSecond >= minTimeSecond && timeSecond <= maxTimeSecond;
	}
	// 时间范围不在同一月,中间跨过了1号0点
	else
	{
		return timeSecond >= minTimeSecond || timeSecond <= maxTimeSecond;
	}
}

bool TimeValue::isTimeInRangeInYear(const TimeValue& time, const TimeValue& minTime, const TimeValue& maxTime)
{
	// 如果minTime大于maxTime,则认为时间无效
	if (minTime > maxTime)
	{
		return false;
	}
	const int timeSecond = time.mMonth * 3600 * 24 * 31 + time.mDay * 3600 * 24 + time.mHour * 3600 + time.mMinute * 60 + time.mSecond;
	const int minTimeSecond = minTime.mMonth * 3600 * 24 * 31 + minTime.mDay * 3600 * 24 + minTime.mHour * 3600 + minTime.mMinute * 60 + minTime.mSecond;
	const int maxTimeSecond = maxTime.mMonth * 3600 * 24 * 31 + maxTime.mDay * 3600 * 24 + maxTime.mHour * 3600 + maxTime.mMinute * 60 + maxTime.mSecond;
	// 时间范围都在同一年
	if (minTimeSecond <= maxTimeSecond)
	{
		return timeSecond >= minTimeSecond && timeSecond <= maxTimeSecond;
	}
	// 时间范围不在同一年,中间跨过了1月1号0点
	else
	{
		return timeSecond >= minTimeSecond || timeSecond <= maxTimeSecond;
	}
}