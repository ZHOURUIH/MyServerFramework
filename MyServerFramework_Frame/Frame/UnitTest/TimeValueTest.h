#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"
#include "TimeValue.h"

class MICRO_LEGEND_FRAME_API TimeValueTest
{
#ifdef WINDOWS
public:
    static void test();
private:
    static void testTimeValueClear();
    static void testTimeValueOperatorGreater();
    static void testIsTimeInRangeInHourBasic();
    static void testIsTimeInRangeInHourEdge();
    static void testIsTimeInRangeInHourOutside();
    static void testIsTimeInRangeInDayBasic();
    static void testIsTimeInRangeInDayEdge();
    static void testIsTimeInRangeInDayOutside();
    static void testIsTimeInRangeInWeekBasic();
    static void testIsTimeInRangeInWeekEdge();
    static void testIsTimeInRangeInWeekOutside();
    static void testIsTimeInRangeInMonthBasic();
    static void testIsTimeInRangeInMonthEdge();
    static void testIsTimeInRangeInMonthOutside();
    static void testIsTimeInRangeInYearBasic();
    static void testIsTimeInRangeInYearEdge();
    static void testIsTimeInRangeInYearOutside();
    static void testTimeValueDefaultInit();
    static void testOperatorGreaterEdge();
    static void testOperatorGreaterSameValues();
	static TimeValue makeTV(int weekDay, int year, int month, int day, int hour, int minute, int second);
	static void testTimeValue();
	static void testTimeValueRangeInHour();
	static void testTimeValueRangeInDay();
	static void testTimeValueRangeInWeek();
	static void testTimeValueRangeInMonth();
	static void testTimeValueRangeInYear();
	static void testTimeValueBoundary();
	static void testTimeValueExtreme();
	static void testTimeValueRefreshTime();
	static void testTimeValueClearCycle();
	static void testTimeValueCompareStress();
	static void testTimeValueRangeEdge();
	static void testTimeValueRangeInYearExtended();
    static void testTimeValueRangeInMonthFull();
    static void testTimeValueClearAndRange();
    static void testTimeValueRangeInWeekFull();
    static void testTimeValueTransitivity();
    static void testTimeValueIncrementalCompare();
    static void testTimeValueYearBoundary();
    static void testTimeValueSecondPrecision();
    static void testTimeValueClearMultipleTimes();
    static void testTimeValueRangeInHourExtended();
    static void testTimeValueRangeInDayExtended();
#endif
};