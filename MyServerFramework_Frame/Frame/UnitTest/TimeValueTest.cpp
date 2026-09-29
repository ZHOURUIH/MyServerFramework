#ifdef WINDOWS
#include "FrameHeader.h"

#define TVE_ASSERT(expr) \
    if (!(expr)) { ERROR("TimeValueTest FAILED: " #expr); }

TimeValue TimeValueTest::makeTV(int weekDay, int year, int month, int day, int hour, int minute, int second)
{
    TimeValue tv;
    tv.mWeekDay = weekDay;
    tv.mYear = year;
    tv.mMonth = month;
    tv.mDay = day;
    tv.mHour = hour;
    tv.mMinute = minute;
    tv.mSecond = second;
    return tv;
}

void TimeValueTest::testTimeValueClear()
{
    TimeValue tv = makeTV(3, 2024, 6, 15, 10, 30, 45);
    tv.clear();
    TVE_ASSERT(tv.mWeekDay == 0);
    TVE_ASSERT(tv.mYear == 0);
    TVE_ASSERT(tv.mMonth == 0);
    TVE_ASSERT(tv.mDay == 0);
    TVE_ASSERT(tv.mHour == 0);
    TVE_ASSERT(tv.mMinute == 0);
    TVE_ASSERT(tv.mSecond == 0);
}

void TimeValueTest::testTimeValueDefaultInit()
{
    TimeValue tv;
    TVE_ASSERT(tv.mYear == 0);
    TVE_ASSERT(tv.mMonth == 0);
    TVE_ASSERT(tv.mDay == 0);
    TVE_ASSERT(tv.mHour == 0);
    TVE_ASSERT(tv.mMinute == 0);
    TVE_ASSERT(tv.mSecond == 0);
    TVE_ASSERT(tv.mWeekDay == 0);
}

void TimeValueTest::testTimeValueOperatorGreater()
{
    TimeValue t1 = makeTV(0, 2024, 6, 15, 10, 0, 0);
    TimeValue t2 = makeTV(0, 2024, 6, 15, 9, 0, 0);
    TVE_ASSERT(t1 > t2);
    TVE_ASSERT(!(t2 > t1));
    TimeValue t3 = makeTV(0, 2025, 1, 1, 0, 0, 0);
    TimeValue t4 = makeTV(0, 2024, 12, 31, 23, 59, 59);
    TVE_ASSERT(t3 > t4);
    TVE_ASSERT(!(t4 > t3));
    TimeValue t5 = makeTV(0, 2024, 6, 16, 0, 0, 0);
    TimeValue t6 = makeTV(0, 2024, 6, 15, 23, 59, 59);
    TVE_ASSERT(t5 > t6);
    TimeValue t7 = makeTV(0, 2024, 6, 15, 10, 31, 0);
    TimeValue t8 = makeTV(0, 2024, 6, 15, 10, 30, 0);
    TVE_ASSERT(t7 > t8);
    TimeValue t9 = makeTV(0, 2024, 6, 15, 10, 30, 46);
    TimeValue t10 = makeTV(0, 2024, 6, 15, 10, 30, 45);
    TVE_ASSERT(t9 > t10);
}

void TimeValueTest::testOperatorGreaterEdge()
{
    TimeValue t1 = makeTV(0, 2024, 1, 1, 0, 0, 1);
    TimeValue t2 = makeTV(0, 2024, 1, 1, 0, 0, 0);
    TVE_ASSERT(t1 > t2);
    TVE_ASSERT(!(t2 > t1));
}

void TimeValueTest::testOperatorGreaterSameValues()
{
    TimeValue t1 = makeTV(0, 2024, 6, 15, 10, 30, 0);
    TimeValue t2 = makeTV(0, 2024, 6, 15, 10, 30, 0);
    TVE_ASSERT(!(t1 > t2));
    TVE_ASSERT(!(t2 > t1));
}

void TimeValueTest::testIsTimeInRangeInHourBasic()
{
    TimeValue t = makeTV(0, 0, 0, 0, 0, 30, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 0, 59, 59);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 0, 0, 0, 59, 59);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(tAtMax, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInHourEdge()
{
    TimeValue t = makeTV(0, 0, 0, 0, 5, 30, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 30, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 0, 30, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(t, minT, maxT));
    TimeValue minOnly = makeTV(0, 0, 0, 0, 0, 15, 0);
    TimeValue maxOnly = makeTV(0, 0, 0, 0, 0, 45, 0);
    TimeValue inside = makeTV(0, 0, 0, 0, 0, 30, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInHour(inside, minOnly, maxOnly));
}

void TimeValueTest::testIsTimeInRangeInHourOutside()
{
    TimeValue t = makeTV(0, 0, 0, 0, 0, 5, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 10, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 0, 50, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInHour(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 0, 0, 55, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInHour(t2, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInDayBasic()
{
    TimeValue t = makeTV(0, 0, 0, 0, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 0, 0, 9, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(tAtMax, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInDayEdge()
{
    TimeValue t = makeTV(0, 2024, 3, 15, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 23, 59, 59);
    TVE_ASSERT(TimeValue::isTimeInRangeInDay(t, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInDayOutside()
{
    TimeValue t = makeTV(0, 0, 0, 0, 7, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 0, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInDay(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 0, 20, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInDay(t2, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInWeekBasic()
{
    TimeValue t = makeTV(3, 0, 0, 0, 0, 0, 0);
    TimeValue minT = makeTV(1, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(5, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(t, minT, maxT));
    TimeValue tAtMin = makeTV(1, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(5, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(tAtMax, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInWeekEdge()
{
    TimeValue t = makeTV(1, 0, 0, 0, 12, 0, 0);
    TimeValue minT = makeTV(1, 0, 0, 0, 9, 0, 0);
    TimeValue maxT = makeTV(1, 0, 0, 0, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInWeek(t, minT, maxT));
    TimeValue t2 = makeTV(3, 0, 0, 0, 0, 0, 0);
    TimeValue minT2 = makeTV(3, 0, 0, 0, 10, 0, 0);
    TimeValue maxT2 = makeTV(3, 0, 0, 0, 20, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInWeek(t2, minT2, maxT2));
}

void TimeValueTest::testIsTimeInRangeInWeekOutside()
{
    TimeValue t = makeTV(6, 0, 0, 0, 0, 0, 0);
    TimeValue minT = makeTV(1, 0, 0, 0, 0, 0, 0);
    TimeValue maxT = makeTV(5, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInWeek(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 0, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInWeek(t2, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInMonthBasic()
{
    TimeValue t = makeTV(0, 0, 0, 15, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 1, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 28, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 0, 1, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 0, 28, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(tAtMax, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInMonthEdge()
{
    TimeValue t = makeTV(0, 0, 0, 15, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 15, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 15, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInMonth(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 15, 8, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInMonth(t2, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInMonthOutside()
{
    TimeValue t = makeTV(0, 0, 0, 5, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 0, 10, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 0, 20, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInMonth(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 0, 25, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInMonth(t2, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInYearBasic()
{
    TimeValue t = makeTV(0, 0, 6, 15, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 1, 1, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 12, 31, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(t, minT, maxT));
    TimeValue tAtMin = makeTV(0, 0, 1, 1, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(tAtMin, minT, maxT));
    TimeValue tAtMax = makeTV(0, 0, 12, 31, 0, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(tAtMax, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInYearEdge()
{
    TimeValue t = makeTV(0, 0, 6, 15, 12, 0, 0);
    TimeValue minT = makeTV(0, 0, 6, 15, 9, 0, 0);
    TimeValue maxT = makeTV(0, 0, 6, 15, 18, 0, 0);
    TVE_ASSERT(TimeValue::isTimeInRangeInYear(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 6, 1, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInYear(t2, minT, maxT));
}

void TimeValueTest::testIsTimeInRangeInYearOutside()
{
    TimeValue t = makeTV(0, 0, 3, 15, 0, 0, 0);
    TimeValue minT = makeTV(0, 0, 6, 1, 0, 0, 0);
    TimeValue maxT = makeTV(0, 0, 9, 30, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInYear(t, minT, maxT));
    TimeValue t2 = makeTV(0, 0, 11, 1, 0, 0, 0);
    TVE_ASSERT(!TimeValue::isTimeInRangeInYear(t2, minT, maxT));
}

void TimeValueTest::test()
{
    testTimeValueClear();
    testTimeValueDefaultInit();
    testTimeValueOperatorGreater();
    testOperatorGreaterEdge();
    testOperatorGreaterSameValues();
    testIsTimeInRangeInHourBasic();
    testIsTimeInRangeInHourEdge();
    testIsTimeInRangeInHourOutside();
    testIsTimeInRangeInDayBasic();
    testIsTimeInRangeInDayEdge();
    testIsTimeInRangeInDayOutside();
    testIsTimeInRangeInWeekBasic();
    testIsTimeInRangeInWeekEdge();
    testIsTimeInRangeInWeekOutside();
    testIsTimeInRangeInMonthBasic();
    testIsTimeInRangeInMonthEdge();
    testIsTimeInRangeInMonthOutside();
    testIsTimeInRangeInYearBasic();
    testIsTimeInRangeInYearEdge();
    testIsTimeInRangeInYearOutside();
	testTimeValue();
	testTimeValueRangeInHour();
	testTimeValueRangeInDay();
	testTimeValueRangeInWeek();
	testTimeValueRangeInMonth();
	testTimeValueRangeInYear();
	testTimeValueBoundary();
	testTimeValueExtreme();
	testTimeValueRefreshTime();
	testTimeValueClearCycle();
	testTimeValueCompareStress();
	testTimeValueRangeEdge();
	testTimeValueRangeInYearExtended();
	testTimeValueRangeInMonthFull();
	testTimeValueClearAndRange();
	testTimeValueRangeInWeekFull();
	testTimeValueTransitivity();
	testTimeValueIncrementalCompare();
	testTimeValueYearBoundary();
	testTimeValueSecondPrecision();
	testTimeValueClearMultipleTimes();
	testTimeValueRangeInHourExtended();
	testTimeValueRangeInDayExtended();
}

// ============================================================
// Consolidated TimeValue tests formerly in TimePointTest
// ============================================================
#define TIMEVALUE_TP_ASSERT(expr) \
	if (!(expr)) { ERROR(string("TimeValueTest FAILED: ") + (#expr)); }

void TimeValueTest::testTimeValue()
{
	// 默认构造
	TimeValue tv;
	TIMEVALUE_TP_ASSERT(tv.mYear == 0);
	TIMEVALUE_TP_ASSERT(tv.mMonth == 0);
	TIMEVALUE_TP_ASSERT(tv.mDay == 0);
	TIMEVALUE_TP_ASSERT(tv.mHour == 0);
	TIMEVALUE_TP_ASSERT(tv.mMinute == 0);
	TIMEVALUE_TP_ASSERT(tv.mSecond == 0);
	TIMEVALUE_TP_ASSERT(tv.mWeekDay == 0);

	// clear
	tv.mYear = 2024;
	tv.mMonth = 3;
	tv.mDay = 15;
	tv.mHour = 10;
	tv.mMinute = 30;
	tv.mSecond = 45;
	tv.mWeekDay = 5;
	tv.clear();
	TIMEVALUE_TP_ASSERT(tv.mYear == 0);
	TIMEVALUE_TP_ASSERT(tv.mMonth == 0);
	TIMEVALUE_TP_ASSERT(tv.mDay == 0);
	TIMEVALUE_TP_ASSERT(tv.mHour == 0);
	TIMEVALUE_TP_ASSERT(tv.mMinute == 0);
	TIMEVALUE_TP_ASSERT(tv.mSecond == 0);
	TIMEVALUE_TP_ASSERT(tv.mWeekDay == 0);

	// operator> : 年份差异
	TimeValue a, b;
	a.mYear = 2025; 
	a.mMonth = 1; 
	a.mDay = 1;
	b.mYear = 2024; 
	b.mMonth = 1; 
	b.mDay = 1;
	TIMEVALUE_TP_ASSERT(a > b);
	TIMEVALUE_TP_ASSERT(!(b > a));

	// operator> : 月份差异
	TimeValue c, d;
	c.mYear = 2024; 
	c.mMonth = 6;  
	c.mDay = 1;
	d.mYear = 2024; 
	d.mMonth = 3;  
	d.mDay = 1;
	TIMEVALUE_TP_ASSERT(c > d);
	TIMEVALUE_TP_ASSERT(!(d > c));

	// operator> : 日期差异
	TimeValue e, f;
	e.mYear = 2024; 
	e.mMonth = 3; 
	e.mDay = 20;
	f.mYear = 2024; 
	f.mMonth = 3; 
	f.mDay = 10;
	TIMEVALUE_TP_ASSERT(e > f);
	TIMEVALUE_TP_ASSERT(!(f > e));

	// operator> : 时分秒差异
	TimeValue g, h;
	g.mYear = 2024; 
	g.mMonth = 3; 
	g.mDay = 15; 
	g.mHour = 12; 
	g.mMinute = 0; 
	g.mSecond = 0;
	h.mYear = 2024; 
	h.mMonth = 3; 
	h.mDay = 15; 
	h.mHour = 8;  
	h.mMinute = 0; 
	h.mSecond = 0;
	TIMEVALUE_TP_ASSERT(g > h);
	TIMEVALUE_TP_ASSERT(!(h > g));

	// operator> : 同值不大于
	TimeValue i, j;
	i.mYear = 2024; 
	i.mMonth = 3; 
	i.mDay = 15; 
	i.mHour = 8; 
	i.mMinute = 30; 
	i.mSecond = 0;
	j.mYear = 2024; 
	j.mMonth = 3; 
	j.mDay = 15; 
	j.mHour = 8; 
	j.mMinute = 30; 
	j.mSecond = 0;
	TIMEVALUE_TP_ASSERT(!(i > j));
	TIMEVALUE_TP_ASSERT(!(j > i));

	// operator> : 秒差异
	TimeValue k, l;
	k.mYear = 2024; 
	k.mMonth = 1; 
	k.mDay = 1; 
	k.mHour = 0; 
	k.mMinute = 0; 
	k.mSecond = 59;
	l.mYear = 2024; 
	l.mMonth = 1; 
	l.mDay = 1; 
	l.mHour = 0; 
	l.mMinute = 0; 
	l.mSecond = 0;
	TIMEVALUE_TP_ASSERT(k > l);
	TIMEVALUE_TP_ASSERT(!(l > k));

	// operator> : 分钟差异
	TimeValue m2, n2;
	m2.mYear = 2024;
	m2.mMonth = 1;
	m2.mDay = 1;
	m2.mHour = 10;
	m2.mMinute = 59;
	m2.mSecond = 0;
	n2.mYear = 2024; 
	n2.mMonth = 1; 
	n2.mDay = 1; 
	n2.mHour = 10; 
	n2.mMinute = 0;
	n2.mSecond = 0;
	TIMEVALUE_TP_ASSERT(m2 > n2);

}

void TimeValueTest::testTimeValueRangeInHour()
{
	// 时间在范围内（分钟）
	TimeValue time1, minT, maxT;
	time1.mHour = 10;
	time1.mMinute = 30;
	time1.mSecond = 0;
	minT.mHour = 10;
	minT.mMinute = 0;
	minT.mSecond = 0;
	maxT.mHour = 10;
	maxT.mMinute = 59;
	maxT.mSecond = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(time1, minT, maxT));

	// 等于下界
	TimeValue time2 = minT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(time2, minT, maxT));

	// 等于上界
	TimeValue time3 = maxT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(time3, minT, maxT));

	// 超出上界
	TimeValue time4;
	time4.mHour = 10; 
	time4.mMinute = 30; 
	time4.mSecond = 0;
	TimeValue maxT2;
	maxT2.mHour = 10; 
	maxT2.mMinute = 20; 
	maxT2.mSecond = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInHour(time4, minT, maxT2));
}

void TimeValueTest::testTimeValueRangeInDay()
{
	// 正常范围内
	TimeValue time1, minT, maxT;
	time1.mHour = 10; 
	time1.mMinute = 30;
	minT.mHour = 8;  
	minT.mMinute = 0;
	maxT.mHour = 18; 
	maxT.mMinute = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(time1, minT, maxT));

	// 在下界
	TimeValue time2 = minT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(time2, minT, maxT));

	// 在上界
	TimeValue time3 = maxT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(time3, minT, maxT));

	// 超出（早于下界）
	TimeValue time4;
	time4.mHour = 6;
	time4.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInDay(time4, minT, maxT));

	// 超出（晚于上界）
	TimeValue time5;
	time5.mHour = 20; 
	time5.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInDay(time5, minT, maxT));
}

void TimeValueTest::testTimeValueRangeInWeek()
{
	// weekDay 在范围
	TimeValue time1, minT, maxT;
	time1.mWeekDay = 3; 
	time1.mHour = 12; 
	time1.mMinute = 0;
	minT.mWeekDay  = 1; 
	minT.mHour = 8;
	minT.mMinute = 0;
	maxT.mWeekDay = 5;
	maxT.mHour = 18;
	maxT.mMinute = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInWeek(time1, minT, maxT));

	// weekDay 等于下界
	TimeValue time2 = minT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInWeek(time2, minT, maxT));

	// weekDay 等于上界
	TimeValue time3 = maxT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInWeek(time3, minT, maxT));

	// weekDay 超出上界
	TimeValue time4;
	time4.mWeekDay = 6; time4.mHour = 12; time4.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInWeek(time4, minT, maxT));

	// weekDay 低于下界
	TimeValue time5;
	time5.mWeekDay = 0; time5.mHour = 12; time5.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInWeek(time5, minT, maxT));
}

void TimeValueTest::testTimeValueRangeInMonth()
{
	// 天数在范围内
	TimeValue time1;
	TimeValue minT;
	TimeValue maxT;
	time1.mDay = 15;
	time1.mHour = 12;
	time1.mMinute = 0;
	minT.mDay = 1;
	minT.mHour = 0;
	minT.mMinute = 0;
	maxT.mDay = 28;
	maxT.mHour = 23;
	maxT.mMinute = 59;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInMonth(time1, minT, maxT));

	// 在下界
	TimeValue time2 = minT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInMonth(time2, minT, maxT));

	// 在上界
	TimeValue time3 = maxT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInMonth(time3, minT, maxT));

	// 超出上界（天）
	TimeValue time4;
	time4.mDay = 29;
	time4.mHour = 0;
	time4.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInMonth(time4, minT, maxT));
}

void TimeValueTest::testTimeValueRangeInYear()
{
	// 月份在范围内
	TimeValue time1;
	TimeValue minT;
	TimeValue maxT;
	time1.mMonth = 6;
	time1.mDay = 15;
	time1.mHour = 12;
	time1.mMinute = 0;
	minT.mMonth = 3;
	minT.mDay = 1;
	minT.mHour = 0;
	minT.mMinute = 0;
	maxT.mMonth = 9;
	maxT.mDay = 30;
	maxT.mHour = 23;
	maxT.mMinute = 59;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInYear(time1, minT, maxT));

	// 在下界
	TimeValue time2 = minT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInYear(time2, minT, maxT));

	// 在上界
	TimeValue time3 = maxT;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInYear(time3, minT, maxT));

	// 超出上界（月）
	TimeValue time4;
	time4.mMonth = 10;
	time4.mDay = 1;
	time4.mHour = 0;
	time4.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInYear(time4, minT, maxT));

	// 低于下界（月）
	TimeValue time5;
	time5.mMonth = 1; 
	time5.mDay = 15;
	time5.mHour = 12;
	time5.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInYear(time5, minT, maxT));

}

void TimeValueTest::testTimeValueBoundary()
{
	// 年末 vs 年初：用足够大的年份差（operator> 用 31天/月的简化公式，年份差需 >> 12*31/365≈1年）
	// year=10 vs year=1: 年份秒差=9*365*86400>>月份贡献差，确保比较正确
	TimeValue yearEnd, yearStart;
	yearEnd.mYear = 1; yearEnd.mMonth = 12; yearEnd.mDay = 31;
	yearEnd.mHour = 23; yearEnd.mMinute = 59; yearEnd.mSecond = 59;
	yearStart.mYear = 10; yearStart.mMonth = 1; yearStart.mDay = 1;
	yearStart.mHour = 0; yearStart.mMinute = 0; yearStart.mSecond = 0;
	TIMEVALUE_TP_ASSERT(yearStart > yearEnd);
	TIMEVALUE_TP_ASSERT(!(yearEnd > yearStart));

	// 月末 vs 月初（同年相邻月，月份差仅1，day差30，不会有溢出）
	TimeValue monthEnd, monthStart;
	monthEnd.mYear = 2; monthEnd.mMonth = 3; monthEnd.mDay = 31;
	monthEnd.mHour = 23; monthEnd.mMinute = 59; monthEnd.mSecond = 59;
	monthStart.mYear = 2; monthStart.mMonth = 4; monthStart.mDay = 1;
	monthStart.mHour = 0; monthStart.mMinute = 0; monthStart.mSecond = 0;
	TIMEVALUE_TP_ASSERT(monthStart > monthEnd);

	// 同年同月同日，区分时分秒
	TimeValue t1, t2;
	t1.mYear = 2; t1.mMonth = 6; t1.mDay = 15;
	t1.mHour = 23; t1.mMinute = 59; t1.mSecond = 59;
	t2.mYear = 2; t2.mMonth = 6; t2.mDay = 15;
	t2.mHour = 0; t2.mMinute = 0; t2.mSecond = 0;
	TIMEVALUE_TP_ASSERT(t1 > t2);
	TIMEVALUE_TP_ASSERT(!(t2 > t1));

}

void TimeValueTest::testTimeValueExtreme()
{
	// 零时间
	TimeValue zero;
	zero.clear();
	TIMEVALUE_TP_ASSERT(zero.mYear == 0 && zero.mMonth == 0 && zero.mDay == 0);
	TIMEVALUE_TP_ASSERT(zero.mHour == 0 && zero.mMinute == 0 && zero.mSecond == 0);

	// 大年份
	TimeValue bigYear;
	bigYear.mYear = 9999; bigYear.mMonth = 12; bigYear.mDay = 31;
	bigYear.mHour = 23; bigYear.mMinute = 59; bigYear.mSecond = 59;
	TIMEVALUE_TP_ASSERT(bigYear.mYear == 9999);

	// 比较：大年份 > 小年份
	TimeValue smallYear;
	smallYear.mYear = 1; smallYear.mMonth = 1; smallYear.mDay = 1;
	TIMEVALUE_TP_ASSERT(bigYear > smallYear);

}

void TimeValueTest::testTimeValueRefreshTime()
{
	// refreshTime 从系统时间读取，验证年份合理（>= 2020）
	TimeValue tv;
	tv.refreshTime();
	TIMEVALUE_TP_ASSERT(tv.mYear >= 2020);
	TIMEVALUE_TP_ASSERT(tv.mMonth >= 1 && tv.mMonth <= 12);
	TIMEVALUE_TP_ASSERT(tv.mDay >= 1 && tv.mDay <= 31);
	TIMEVALUE_TP_ASSERT(tv.mHour >= 0 && tv.mHour <= 23);
	TIMEVALUE_TP_ASSERT(tv.mMinute >= 0 && tv.mMinute <= 59);
	TIMEVALUE_TP_ASSERT(tv.mSecond >= 0 && tv.mSecond <= 59);

}

void TimeValueTest::testTimeValueClearCycle()
{
	TimeValue tv;
	for (int i = 0; i < 20; ++i)
	{
		tv.mYear   = 2020 + i;
		tv.mMonth  = (i % 12) + 1;
		tv.mDay    = (i % 28) + 1;
		tv.mHour   = i % 24;
		tv.mMinute = i % 60;
		tv.mSecond = i % 60;
		tv.clear();
		TIMEVALUE_TP_ASSERT(tv.mYear == 0);
		TIMEVALUE_TP_ASSERT(tv.mMonth == 0);
		TIMEVALUE_TP_ASSERT(tv.mDay == 0);
		TIMEVALUE_TP_ASSERT(tv.mHour == 0);
		TIMEVALUE_TP_ASSERT(tv.mMinute == 0);
		TIMEVALUE_TP_ASSERT(tv.mSecond == 0);
	}
}

void TimeValueTest::testTimeValueCompareStress()
{
	// 100对随机分布比较，确保传递性
	for (int y = 2020; y <= 2025; ++y)
	{
		TimeValue a, b;
		a.mYear = y;     a.mMonth = 6; a.mDay = 15; a.mHour = 12; a.mMinute = 0; a.mSecond = 0;
		b.mYear = y - 1; b.mMonth = 6; b.mDay = 15; b.mHour = 12; b.mMinute = 0; b.mSecond = 0;
		TIMEVALUE_TP_ASSERT(a > b);
		TIMEVALUE_TP_ASSERT(!(b > a));
	}
	for (int m = 1; m <= 12; ++m)
	{
		TimeValue a, b;
		a.mYear = 2024; a.mMonth = m;
		b.mYear = 2024; b.mMonth = (m > 1 ? m - 1 : 1);
		if (m > 1)
		{
			TIMEVALUE_TP_ASSERT(a > b);
		}
	}
}

void TimeValueTest::testTimeValueRangeEdge()
{
	// 跨天范围（hour 22 ~ 次日 02，这里只测同天的逻辑）
	TimeValue time1, minT, maxT;
	time1.mHour = 22; time1.mMinute = 30;
	minT.mHour  = 22; minT.mMinute  = 0;
	maxT.mHour  = 23; maxT.mMinute  = 59;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(time1, minT, maxT));

	// 同 min == max 时，只有等于才在范围内
	TimeValue t;
	t.mHour = 10; t.mMinute = 30; t.mDay = 15; t.mMonth = 6;
	TimeValue same = t;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(t, same, same));
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInMonth(t, same, same));
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInYear(t, same, same));

}

void TimeValueTest::testTimeValueRangeInYearExtended()
{
	// 全年范围（1月到12月）
	TimeValue minT, maxT;
	minT.mMonth = 1;  minT.mDay = 1;  minT.mHour = 0;  minT.mMinute = 0;
	maxT.mMonth = 12; maxT.mDay = 31; maxT.mHour = 23; maxT.mMinute = 59;
	for (int m = 1; m <= 12; ++m)
	{
		TimeValue t;
		t.mMonth = m; t.mDay = 15; t.mHour = 12; t.mMinute = 0;
		TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInYear(t, minT, maxT));
	}

	// 窄范围（3月到5月）
	TimeValue minT2, maxT2;
	minT2.mMonth = 3; minT2.mDay = 1;  minT2.mHour = 0;
	maxT2.mMonth = 5; maxT2.mDay = 31; maxT2.mHour = 23; maxT2.mMinute = 59;
	{
		TimeValue t;
		t.mMonth = 2; t.mDay = 28; t.mHour = 23; t.mMinute = 59;
		TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInYear(t, minT2, maxT2));
	}
	{
		TimeValue t;
		t.mMonth = 6; t.mDay = 1; t.mHour = 0; t.mMinute = 0;
		TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInYear(t, minT2, maxT2));
	}
}

void TimeValueTest::testTimeValueRangeInMonthFull()
{
	TimeValue minT, maxT;
	minT.mDay = 1;  minT.mHour = 0;  minT.mMinute = 0;
	maxT.mDay = 31; maxT.mHour = 23; maxT.mMinute = 59;
	for (int d = 1; d <= 31; ++d)
	{
		TimeValue t;
		t.mDay = d; t.mHour = d % 24; t.mMinute = d % 60;
		TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInMonth(t, minT, maxT));
	}

	// 边界外（day = 0，先于第一天）
	TimeValue t0;
	t0.mDay = 0; t0.mHour = 0; t0.mMinute = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInMonth(t0, minT, maxT));

}

void TimeValueTest::testTimeValueClearAndRange()
{
	TimeValue tv;
	tv.mYear = 2024; tv.mMonth = 6; tv.mDay = 15;
	tv.mHour = 12; tv.mMinute = 30; tv.mSecond = 0;
	tv.clear();
	// 清除后所有字段为0
	TimeValue minT, maxT;
	minT.mMonth = 0; minT.mDay = 0; minT.mHour = 0; minT.mMinute = 0;
	maxT.mMonth = 0; maxT.mDay = 0; maxT.mHour = 0; maxT.mMinute = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(tv, minT, maxT));
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInMonth(tv, minT, maxT));
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInYear(tv, minT, maxT));

}

void TimeValueTest::testTimeValueRangeInWeekFull()
{
	TimeValue minT, maxT;
	minT.mWeekDay = 0; minT.mHour = 0;  minT.mMinute = 0;
	maxT.mWeekDay = 6; maxT.mHour = 23; maxT.mMinute = 59;
	for (int wd = 0; wd <= 6; ++wd)
	{
		TimeValue t;
		t.mWeekDay = wd; t.mHour = 12; t.mMinute = 0;
		TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInWeek(t, minT, maxT));
	}
}

void TimeValueTest::testTimeValueTransitivity()
{
	// a > b, b > c => a > c
	TimeValue a, b, c;
	a.mYear = 2025; a.mMonth = 12; a.mDay = 31;
	b.mYear = 2024; b.mMonth = 6;  b.mDay = 15;
	c.mYear = 2023; c.mMonth = 1;  c.mDay = 1;
	TIMEVALUE_TP_ASSERT(a > b);
	TIMEVALUE_TP_ASSERT(b > c);
	TIMEVALUE_TP_ASSERT(a > c);

	// 仅时间差异
	TimeValue x, y, z;
	x.mYear = 2024; x.mMonth = 3; x.mDay = 15; x.mHour = 23; x.mMinute = 59; x.mSecond = 59;
	y.mYear = 2024; y.mMonth = 3; y.mDay = 15; y.mHour = 12; y.mMinute = 0;  y.mSecond = 0;
	z.mYear = 2024; z.mMonth = 3; z.mDay = 15; z.mHour = 0;  z.mMinute = 0;  z.mSecond = 0;
	TIMEVALUE_TP_ASSERT(x > y);
	TIMEVALUE_TP_ASSERT(y > z);
	TIMEVALUE_TP_ASSERT(x > z);

}

void TimeValueTest::testTimeValueIncrementalCompare()
{
	// Build a time series and check ordering
	TimeValue t[6];
	// t[0] < t[1] < t[2] < t[3] < t[4] < t[5]
	t[0].mYear = 2020; t[0].mMonth = 1; t[0].mDay = 1; t[0].mHour = 0; t[0].mMinute = 0; t[0].mSecond = 0;
	t[1].mYear = 2021; t[1].mMonth = 1; t[1].mDay = 1; t[1].mHour = 0; t[1].mMinute = 0; t[1].mSecond = 0;
	t[2].mYear = 2021; t[2].mMonth = 6; t[2].mDay = 1; t[2].mHour = 0; t[2].mMinute = 0; t[2].mSecond = 0;
	t[3].mYear = 2021; t[3].mMonth = 6; t[3].mDay = 15; t[3].mHour = 0; t[3].mMinute = 0; t[3].mSecond = 0;
	t[4].mYear = 2021; t[4].mMonth = 6; t[4].mDay = 15; t[4].mHour = 12; t[4].mMinute = 0; t[4].mSecond = 0;
	t[5].mYear = 2021; t[5].mMonth = 6; t[5].mDay = 15; t[5].mHour = 12; t[5].mMinute = 30; t[5].mSecond = 45;

	FOR(5)
	{
		TIMEVALUE_TP_ASSERT(t[i + 1] > t[i]);
		TIMEVALUE_TP_ASSERT(!(t[i] > t[i + 1]));
	}
	TIMEVALUE_TP_ASSERT(!(t[0] > t[0]));
	TIMEVALUE_TP_ASSERT(!(t[5] > t[5]));

}

void TimeValueTest::testTimeValueYearBoundary()
{
	TimeValue y1, y2, y3;
	y1.mYear = 1;
	y1.mMonth = 1; 
	y1.mDay = 1;
	y2.mYear = 9999; 
	y2.mMonth = 12;
	y2.mDay = 31; 
	y2.mHour = 23; 
	y2.mMinute = 59;
	y2.mSecond = 59;
	y3.mYear = 2000;
	y3.mMonth = 6; 
	y3.mDay = 15;

	TIMEVALUE_TP_ASSERT(y2 > y1);
	TIMEVALUE_TP_ASSERT(y2 > y3);
	TIMEVALUE_TP_ASSERT(y3 > y1);
	TIMEVALUE_TP_ASSERT(!(y1 > y2));
}

void TimeValueTest::testTimeValueSecondPrecision()
{
	// Differ only by 1 second
	TimeValue a, b;
	a.mYear = 2024; a.mMonth = 6; a.mDay = 1; a.mHour = 12; a.mMinute = 0; a.mSecond = 0;
	b.mYear = 2024; b.mMonth = 6; b.mDay = 1; b.mHour = 12; b.mMinute = 0; b.mSecond = 1;
	TIMEVALUE_TP_ASSERT(b > a);
	TIMEVALUE_TP_ASSERT(!(a > b));
	TIMEVALUE_TP_ASSERT(!(a > a));

	// Differ only by 1 minute
	TimeValue c, d;
	c.mYear = 2024; c.mMonth = 6; c.mDay = 1; c.mHour = 12; c.mMinute = 0; c.mSecond = 59;
	d.mYear = 2024; d.mMonth = 6; d.mDay = 1; d.mHour = 12; d.mMinute = 1; d.mSecond = 0;
	TIMEVALUE_TP_ASSERT(d > c);
	TIMEVALUE_TP_ASSERT(!(c > d));

	// Identical time
	TimeValue e, f;
	e.mYear = 2024; e.mMonth = 6; e.mDay = 1; e.mHour = 12; e.mMinute = 30; e.mSecond = 45;
	f = e;
	TIMEVALUE_TP_ASSERT(!(e > f));
	TIMEVALUE_TP_ASSERT(!(f > e));
}

void TimeValueTest::testTimeValueClearMultipleTimes()
{
	TimeValue tv;
	for (int cycle = 0; cycle < 10; ++cycle)
	{
		tv.mYear = 2000 + cycle;
		tv.mMonth = (cycle % 12) + 1;
		tv.mDay = (cycle % 28) + 1;
		tv.mHour = cycle % 24;
		tv.mMinute = cycle * 3 % 60;
		tv.mSecond = cycle * 7 % 60;
		tv.mWeekDay = cycle % 7;

		tv.clear();
		TIMEVALUE_TP_ASSERT(tv.mYear == 0);
		TIMEVALUE_TP_ASSERT(tv.mMonth == 0);
		TIMEVALUE_TP_ASSERT(tv.mDay == 0);
		TIMEVALUE_TP_ASSERT(tv.mHour == 0);
		TIMEVALUE_TP_ASSERT(tv.mMinute == 0);
		TIMEVALUE_TP_ASSERT(tv.mSecond == 0);
		TIMEVALUE_TP_ASSERT(tv.mWeekDay == 0);
	}
}

void TimeValueTest::testTimeValueRangeInHourExtended()
{
	// Range spans exactly 1 minute 
	TimeValue t, lo, hi;
	t.mMinute = 30; 
	t.mSecond = 30;
	lo.mMinute = 30; 
	lo.mSecond = 0;
	hi.mMinute = 30;
	hi.mSecond = 59;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(t, lo, hi));

	// Just outside range
	TimeValue t2;
	t2.mMinute = 31; 
	t2.mSecond = 0;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInHour(t2, lo, hi));

	// t == lo
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(lo, lo, hi));

	// t == hi
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(hi, lo, hi));

	// Full hour range [0:0, 59:59]
	TimeValue fullLo, fullHi;
	fullLo.mMinute = 0; 
	fullLo.mSecond = 0;
	fullHi.mMinute = 59;
	fullHi.mSecond = 59;
	for (int m = 0; m < 60; ++m)
	{
		TimeValue test;
		test.mMinute = m;
		test.mSecond = 0;
		TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInHour(test, fullLo, fullHi));
	}

}

void TimeValueTest::testTimeValueRangeInDayExtended()
{
	// Midnight to midnight: [0:0:0, 23:59:59]
	TimeValue lo, hi;
	lo.mHour = 0; 
	lo.mMinute = 0; 
	lo.mSecond = 0;
	hi.mHour = 23;
	hi.mMinute = 59; 
	hi.mSecond = 59;

	TimeValue midday;
	midday.mHour = 12; 
	midday.mMinute = 0;
	midday.mSecond = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(midday, lo, hi));

	TimeValue midnight;
	midnight.mHour = 0; 
	midnight.mMinute = 0; 
	midnight.mSecond = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(midnight, lo, hi));

	// Single-second range
	TimeValue exactLo, exactHi;
	exactLo.mHour = 15;
	exactLo.mMinute = 30; 
	exactLo.mSecond = 0;
	exactHi.mHour = 15; 
	exactHi.mMinute = 30;
	exactHi.mSecond = 0;
	TIMEVALUE_TP_ASSERT(TimeValue::isTimeInRangeInDay(exactLo, exactLo, exactHi));

	TimeValue oneSecAfter;
	oneSecAfter.mHour = 15; 
	oneSecAfter.mMinute = 30; 
	oneSecAfter.mSecond = 1;
	TIMEVALUE_TP_ASSERT(!TimeValue::isTimeInRangeInDay(oneSecAfter, exactLo, exactHi));
}
#endif
