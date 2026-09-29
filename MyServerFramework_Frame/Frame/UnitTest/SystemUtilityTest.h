#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API SystemUtilityTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void assertSystemUtility(bool condition, const char* message);
	static void testDateMath();
	static void testTimestampRoundTrip();
	static void testTodayBoundaries();
	static void testMutableGlobalState();
	static void testTimeQueries();
	static void testSystem();
	static void testIsLeapYear();
	static void testDaysToSeconds();
	static void testGetDaysInMonth();
	static void testConvertToTimestamp();
	static void testIsSameDay();
	static void testTimeSeed();
	static void testMakeID();
	static void testThreadID();
	static void testGetYearMonthDay();
	static void testGetTimeHMS();
	static void testGetTimeYMDHMS();
	static void testGetTimeWithWeekDay();
	static void testGetTimeHourInDay();
	static void testGetDayOfWeek();
	static void testGetTimeString();
	static void testDaysToSecondsExt();
	static void testIsLeapYearExt();
	static void testGetDaysInMonthFull();
	static void testConvertToTimestampMultiple();
	static void testIsSameDayExt();
	static void testTodayBeginEnd2();
	static void testGetRealTimeMS();
	static void testMakeIDMainBulk();
	static void testMakeIDThreadBulk();
	static void testGetPID();
	static void testGetTimeStructConsistency();
	static void testIsSameDayBatch();
	static void testIsLeapYearBoundary();
	static void testGetProgramFile();
	static void testGetCPUCoreCount();
	static void testExtended();
	static void testTodayBeginEnd1();
	static void testGetDayOfWeek2();
	static void testGetTimeHourInDay2();
	static void testGetTimeFullWithWeekDay();
	static void testGetTimeSixParams();
	static void testGetTimeString2();
	static void testGetCPUCoreCount2();
	static void testGetPID2();
	static void testGetRealTimeMS2();
	static void testSleep();
	static void testGetProgramFile2();
	static void testMakeIDThreadExtended();
	static void testDaysToSecondsExtended();
	static void testIsLeapYearExtended();
	static void testConvertToTimestampExtended();
	static void testGetTimeStruct();
	static void testIsSameDayExtended();
	static void testGetDaysInMonthCurrent();
	static void testTimeUTCSeed();
	static void testMainThreadExtended();
	static void testTimePipeline();
	static void testMakeIDStress();
#endif
};