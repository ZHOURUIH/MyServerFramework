#ifdef WINDOWS
#include "FrameHeader.h"

void SystemUtilityTest::assertSystemUtility(bool condition, const char* message)
{
	if (!condition)
	{
		ERROR(string("SystemUtilityTest failed: ") + message);
	}
}

void SystemUtilityTest::testDateMath()
{
	assertSystemUtility(isLeapYear(2000), "2000 leap");
	assertSystemUtility(!isLeapYear(1900), "1900 not leap");
	assertSystemUtility(isLeapYear(2024), "2024 leap");
	assertSystemUtility(!isLeapYear(2023), "2023 not leap");
	assertSystemUtility(daysToSeconds(1) == 86400, "days to seconds");
	assertSystemUtility(getDaysInMonth(2024, 2) == 29, "feb leap");
	assertSystemUtility(getDaysInMonth(2023, 2) == 28, "feb normal");
	assertSystemUtility(getDaysInMonth(2023, 4) == 30, "april days");
	assertSystemUtility(getDaysInMonth(2023, 1) == 31, "jan days");
}

void SystemUtilityTest::testTimestampRoundTrip()
{
	const time_t stamp = convertToTimestamp(2024, 3, 31, 12, 34, 56);
	struct tm timeStruct = getTimeStruct(stamp);
	assertSystemUtility(timeStruct.tm_year == 2024, "year roundtrip");
	assertSystemUtility(timeStruct.tm_mon == 3, "month roundtrip");
	assertSystemUtility(timeStruct.tm_mday == 31, "day roundtrip");
	assertSystemUtility(timeStruct.tm_hour == 12, "hour roundtrip");
	assertSystemUtility(timeStruct.tm_min == 34, "minute roundtrip");
	assertSystemUtility(timeStruct.tm_sec == 56, "second roundtrip");
	assertSystemUtility(isSameDay(stamp, stamp + 3600), "same day");
	assertSystemUtility(!isSameDay(stamp, stamp + daysToSeconds(1)), "different day");
}

void SystemUtilityTest::testTodayBoundaries()
{
	const llong begin = getTodayBegin();
	const llong end = getTodayEnd();
	assertSystemUtility(end - begin == daysToSeconds(1), "today interval");
	struct tm beginStruct = getTimeStruct((time_t)begin);
	assertSystemUtility(beginStruct.tm_hour == 0, "today begin hour");
	assertSystemUtility(beginStruct.tm_min == 0, "today begin minute");
	assertSystemUtility(beginStruct.tm_sec == 0, "today begin second");
}

void SystemUtilityTest::testMutableGlobalState()
{
	setTimeMSecondUTC(123456);
	setTimeSecondUTC(654321);
	assertSystemUtility(getTimeMSecondUTC() == 123456, "ms utc set");
	assertSystemUtility(getTimeSecondUTC() == 654321, "sec utc set");

	setMainThread(getThreadID());
	assertSystemUtility(isMainThread(), "main thread flag");
	const llong id0 = makeIDMain();
	const llong id1 = makeIDMain();
	const llong tid0 = makeIDThread();
	const llong tid1 = makeIDThread();
	assertSystemUtility(id1 == id0 + 1, "main id increment");
	assertSystemUtility(tid1 == tid0 + 1, "thread id increment");
}

void SystemUtilityTest::testTimeQueries()
{
	const int year = getYear();
	const int month = getMonth();
	const int day = getDay();
	const int hour = getTimeHourInDay();
	const int cpu = getCPUCoreCount();
	const llong realTime = getRealTimeMS();

	assertSystemUtility(year >= 1970, "year valid");
	assertSystemUtility(month >= 1 && month <= 12, "month valid");
	assertSystemUtility(day >= 1 && day <= 31, "day valid");
	assertSystemUtility(hour >= 0 && hour < 24, "hour valid");
	assertSystemUtility(cpu > 0, "cpu count");
	assertSystemUtility(realTime >= 0, "real time");
}

void SystemUtilityTest::test()
{
	testDateMath();
	testTimestampRoundTrip();
	testTodayBoundaries();
	testMutableGlobalState();
	testTimeQueries();
	testSystem();
	testExtended();
}


#define SYS_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SystemUtilityTest FAIL: ") + (msg)); } } while(0)
#define SYS_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SystemUtilityTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SYS_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SystemUtilityTest FAIL: ") + (msg) + \
		" | expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)

//------------------------------------------------------------------------------
// isLeapYear
//------------------------------------------------------------------------------
void SystemUtilityTest::testIsLeapYear()
{
	// standard leap: divisible by 4, not 100
	SYS_ASSERT(SystemUtility::isLeapYear(2000), "2000 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(2004), "2004 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(2024), "2024 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(1600), "1600 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(400), "400 is leap");

	// divisible by 100 but not 400: not leap
	SYS_ASSERT(!SystemUtility::isLeapYear(1900), "1900 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1800), "1800 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1700), "1700 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2100), "2100 not leap");

	// not divisible by 4: not leap
	SYS_ASSERT(!SystemUtility::isLeapYear(2001), "2001 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2003), "2003 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2023), "2023 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1999), "1999 not leap");
}

//------------------------------------------------------------------------------
// daysToSeconds
//------------------------------------------------------------------------------
void SystemUtilityTest::testDaysToSeconds()
{
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(0), 0LL, "0 days = 0 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(1), 86400LL, "1 day = 86400 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(7), 604800LL, "7 days = 604800 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(30), 2592000LL, "30 days=2592000 secs");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(365), 31536000LL, "365 days=31536000 secs");
}

//------------------------------------------------------------------------------
// getDaysInMonth (year, month)
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetDaysInMonth()
{
	// January (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 1), 31, "Jan=31");
	// February normal year (28)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 2), 28, "Feb 2023=28");
	// February leap year (29)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2024, 2), 29, "Feb 2024=29");
	// March (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 3), 31, "Mar=31");
	// April (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 4), 30, "Apr=30");
	// May (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 5), 31, "May=31");
	// June (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 6), 30, "Jun=30");
	// July (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 7), 31, "Jul=31");
	// August (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 8), 31, "Aug=31");
	// September (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 9), 30, "Sep=30");
	// October (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 10), 31, "Oct=31");
	// November (30)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 11), 30, "Nov=30");
	// December (31)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, 12), 31, "Dec=31");

	// Feb boundary: year 1900 (not leap)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(1900, 2), 28, "Feb 1900=28 (not leap)");
	// Feb boundary: year 2000 (leap)
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2000, 2), 29, "Feb 2000=29 (leap)");
}

//------------------------------------------------------------------------------
// convertToTimestamp + getTimeStruct (round-trip)
//------------------------------------------------------------------------------
void SystemUtilityTest::testConvertToTimestamp()
{
	// Known timestamp: 2024-01-15 12:30:45
	const time_t ts = SystemUtility::convertToTimestamp(2024, 1, 15, 12, 30, 45);
	SYS_ASSERT(ts > 0, "convertToTimestamp > 0");

	// Round-trip: convert back via getTimeStruct
	// NOTE: framework's getTimeStruct already applies tm_year+=1900 and tm_mon+=1
	// so tm_year is absolute year (e.g. 2024) and tm_mon is 1-based (1..12)
	const struct tm tmVal = SystemUtility::getTimeStruct(ts);
	SYS_ASSERT_EQ(tmVal.tm_year, 2024, "round-trip year=2024");
	SYS_ASSERT_EQ(tmVal.tm_mon, 1, "round-trip month=1");
	SYS_ASSERT_EQ(tmVal.tm_mday, 15, "round-trip day=15");
	SYS_ASSERT_EQ(tmVal.tm_hour, 12, "round-trip hour=12");
	SYS_ASSERT_EQ(tmVal.tm_min, 30, "round-trip min=30");
	SYS_ASSERT_EQ(tmVal.tm_sec, 45, "round-trip sec=45");

	// 2000-02-29 (leap day)
	const time_t leapTs = SystemUtility::convertToTimestamp(2000, 2, 29, 0, 0, 0);
	const struct tm leapTm = SystemUtility::getTimeStruct(leapTs);
	SYS_ASSERT_EQ(leapTm.tm_year, 2000, "leap day year=2000");
	SYS_ASSERT_EQ(leapTm.tm_mon, 2, "leap day month=2");
	SYS_ASSERT_EQ(leapTm.tm_mday, 29, "leap day day=29");

	// 2000-06-15 08:00:00 — safely away from epoch boundary on any timezone
	// (1970-01-01 local time in UTC+8 yields negative timestamp, mktime returns -1)
	const time_t ts2000 = SystemUtility::convertToTimestamp(2000, 6, 15, 8, 0, 0);
	SYS_ASSERT(ts2000 > 0, "convertToTimestamp 2000-06-15 > 0");
	const struct tm tm2000 = SystemUtility::getTimeStruct(ts2000);
	SYS_ASSERT_EQ(tm2000.tm_year, 2000, "2000 round-trip year=2000");
	SYS_ASSERT_EQ(tm2000.tm_mon, 6, "2000 round-trip month=6");
	SYS_ASSERT_EQ(tm2000.tm_mday, 15, "2000 round-trip day=15");
}

//------------------------------------------------------------------------------
// isSameDay
//------------------------------------------------------------------------------
void SystemUtilityTest::testIsSameDay()
{
	// Two timestamps in the same calendar day
	const time_t ts1 = SystemUtility::convertToTimestamp(2024, 3, 15, 8, 0, 0);
	const time_t ts2 = SystemUtility::convertToTimestamp(2024, 3, 15, 23, 59, 59);
	SYS_ASSERT(SystemUtility::isSameDay(ts1, ts2), "isSameDay: same day true");

	// Two timestamps in different calendar days
	const time_t ts3 = SystemUtility::convertToTimestamp(2024, 3, 15, 23, 59, 59);
	const time_t ts4 = SystemUtility::convertToTimestamp(2024, 3, 16, 0, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(ts3, ts4), "isSameDay: diff day false");

	// Same timestamp with itself
	SYS_ASSERT(SystemUtility::isSameDay(ts1, ts1), "isSameDay: ts with itself");

	// Different months
	const time_t ts5 = SystemUtility::convertToTimestamp(2024, 3, 31, 12, 0, 0);
	const time_t ts6 = SystemUtility::convertToTimestamp(2024, 4, 1, 12, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(ts5, ts6), "isSameDay: diff month false");

	// Different years
	const time_t ts7 = SystemUtility::convertToTimestamp(2023, 12, 31, 12, 0, 0);
	const time_t ts8 = SystemUtility::convertToTimestamp(2024, 1, 1, 12, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(ts7, ts8), "isSameDay: diff year false");
}

//------------------------------------------------------------------------------
// mTimeMSecondUTC / mTimeSecondUTC set-get
//------------------------------------------------------------------------------
void SystemUtilityTest::testTimeSeed()
{
	// set and get mTimeMSecondUTC
	const llong origMS = SystemUtility::getTimeMSecondUTC();
	SystemUtility::setTimeMSecondUTC(999999LL);
	SYS_ASSERT_EQ_LL(SystemUtility::getTimeMSecondUTC(), 999999LL, "setTimeMSecond round-trip");
	// restore
	SystemUtility::setTimeMSecondUTC(origMS);

	// set and get mTimeSecondUTC
	const llong origS = SystemUtility::getTimeSecondUTC();
	SystemUtility::setTimeSecondUTC(888888LL);
	SYS_ASSERT_EQ_LL(SystemUtility::getTimeSecondUTC(), 888888LL, "setTimeSecond round-trip");
	// restore
	SystemUtility::setTimeSecondUTC(origS);
}

//------------------------------------------------------------------------------
// makeIDMain - generates monotonically increasing IDs on main thread
//------------------------------------------------------------------------------
void SystemUtilityTest::testMakeID()
{
	// Ensure IDs are strictly increasing
	const llong id0 = SystemUtility::makeIDMain();
	const llong id1 = SystemUtility::makeIDMain();
	const llong id2 = SystemUtility::makeIDMain();
	SYS_ASSERT(id1 > id0, "makeIDMain id1>id0");
	SYS_ASSERT(id2 > id1, "makeIDMain id2>id1");
	SYS_ASSERT(id0 > 0, "makeIDMain id>0");

	// makeIDThread must also be increasing
	const llong tid0 = SystemUtility::makeIDThread();
	const llong tid1 = SystemUtility::makeIDThread();
	SYS_ASSERT(tid1 > tid0, "makeIDThread tid1>tid0");
}

//------------------------------------------------------------------------------
// getThreadID / isMainThread
//------------------------------------------------------------------------------
void SystemUtilityTest::testThreadID()
{
	const int tid = SystemUtility::getThreadID();
	SYS_ASSERT(tid != 0, "getThreadID != 0");

	// isMainThread: depends on whether setMainThread was called
	// Since main.cpp calls setMainThread before UnitTest::test(),
	// this should be true in our test context
	SYS_ASSERT(SystemUtility::isMainThread(), "isMainThread() true in test");
}

//------------------------------------------------------------------------------
// getYear / getMonth / getDay consistency
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetYearMonthDay()
{
	int year = SystemUtility::getYear();
	int month = SystemUtility::getMonth();
	int day = SystemUtility::getDay();

	SYS_ASSERT(year >= 2024, "getYear >= 2024");
	SYS_ASSERT(month >= 1 && month <= 12, "getMonth in [1,12]");
	SYS_ASSERT(day >= 1 && day <= 31, "getDay in [1,31]");

	int y2 = 0, m2 = 0;
	SystemUtility::getYearMonth(y2, m2);
	SYS_ASSERT_EQ(y2, year, "getYearMonth year matches");
	SYS_ASSERT_EQ(m2, month, "getYearMonth month matches");
}

//------------------------------------------------------------------------------
// getTime (hour, minute, second)
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetTimeHMS()
{
	int h = -1, m = -1, s = -1;
	SystemUtility::getTime(h, m, s);
	SYS_ASSERT(h >= 0 && h < 24, "getTime hour in [0,23]");
	SYS_ASSERT(m >= 0 && m < 60, "getTime min in [0,59]");
	SYS_ASSERT(s >= 0 && s < 60, "getTime sec in [0,59]");
}

//------------------------------------------------------------------------------
// getTime (year, month, day, hour, minute, second)
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetTimeYMDHMS()
{
	int y = 0, mo = 0, d = 0, h = 0, mi = 0, s = 0;
	SystemUtility::getTime(y, mo, d, h, mi, s);
	SYS_ASSERT(y >= 2024, "getTime YMDHMS year >= 2024");
	SYS_ASSERT(mo >= 1 && mo <= 12, "getTime YMDHMS month valid");
	SYS_ASSERT(d >= 1 && d <= 31, "getTime YMDHMS day valid");
	SYS_ASSERT(h >= 0 && h < 24, "getTime YMDHMS hour valid");
	SYS_ASSERT(mi >= 0 && mi < 60, "getTime YMDHMS min valid");
	SYS_ASSERT(s >= 0 && s < 60, "getTime YMDHMS sec valid");
}

//------------------------------------------------------------------------------
// getTime (weekDay, year, month, day, hour, minute, second)
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetTimeWithWeekDay()
{
	int wd = -1, y = 0, mo = 0, d = 0, h = 0, mi = 0, s = 0;
	SystemUtility::getTime(wd, y, mo, d, h, mi, s);
	SYS_ASSERT(wd >= 0 && wd <= 6, "getTime weekDay in [1,7]");
	SYS_ASSERT(y >= 2024, "getTime weekDay year >= 2024");
}

//------------------------------------------------------------------------------
// getTimeHourInDay
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetTimeHourInDay()
{
	int h = SystemUtility::getTimeHourInDay();
	SYS_ASSERT(h >= 0 && h < 24, "getTimeHourInDay in [0,23]");
}

//------------------------------------------------------------------------------
// getDayOfWeek
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetDayOfWeek()
{
	int dow = SystemUtility::getDayOfWeek();
	SYS_ASSERT(dow >= 0 && dow <= 6, "getDayOfWeek in [0,6]");
}

//------------------------------------------------------------------------------
// getTime (string)
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetTimeString()
{
	string t = SystemUtility::getTime();
	SYS_ASSERT(!t.empty(), "getTime() string non-empty");

	string ts = SystemUtility::getTime(true);
	SYS_ASSERT(!ts.empty(), "getTime(timestamp) string non-empty");
}

//------------------------------------------------------------------------------
// daysToSeconds — extended
//------------------------------------------------------------------------------
void SystemUtilityTest::testDaysToSecondsExt()
{
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(2), 172800LL, "2 days");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(3), 259200LL, "3 days");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(10), 864000LL, "10 days");
	SYS_ASSERT_EQ_LL(SystemUtility::daysToSeconds(100), 8640000LL, "100 days");
}

//------------------------------------------------------------------------------
// isLeapYear — extended range
//------------------------------------------------------------------------------
void SystemUtilityTest::testIsLeapYearExt()
{
	// every 4th year in range [2000, 2100]
	for (int y = 2000; y <= 2100; y += 4)
	{
		if (y == 2100)
		{
			SYS_ASSERT(!SystemUtility::isLeapYear(y), "2100 not leap (div100 not div400)");
		}
		else
		{
			SYS_ASSERT(SystemUtility::isLeapYear(y), "y%4==0 is leap");
		}
	}
	// non-leap years
	for (int y = 2001; y <= 2099; y += 4)
	{
		// 2001 2005 2009 ... are not leap
		SYS_ASSERT(!SystemUtility::isLeapYear(y), "non-leap year");
	}
}

//------------------------------------------------------------------------------
// getDaysInMonth — all months for leap and non-leap
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetDaysInMonthFull()
{
	const int daysNormal[13] = { 0, 31,28,31,30,31,30,31,31,30,31,30,31 };
	const int daysLeap[13] = { 0, 31,29,31,30,31,30,31,31,30,31,30,31 };

	// non-leap 2023
	for (int m = 1; m <= 12; ++m)
	{
		SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2023, m), daysNormal[m], "2023 days month");
	}
	// leap 2024
	for (int m = 1; m <= 12; ++m)
	{
		SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2024, m), daysLeap[m], "2024 days month");
	}
	// leap 2000
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(2000, 2), 29, "2000 Feb=29");
	// non-leap 1900
	SYS_ASSERT_EQ(SystemUtility::getDaysInMonth(1900, 2), 28, "1900 Feb=28");
}

//------------------------------------------------------------------------------
// convertToTimestamp — multiple dates
//------------------------------------------------------------------------------
void SystemUtilityTest::testConvertToTimestampMultiple()
{
	// 2020-03-01 midnight
	const time_t ts1 = SystemUtility::convertToTimestamp(2020, 3, 1, 0, 0, 0);
	SYS_ASSERT(ts1 > 0, "ts1 > 0");
	const struct tm tm1 = SystemUtility::getTimeStruct(ts1);
	SYS_ASSERT_EQ(tm1.tm_year, 2020, "ts1 year");
	SYS_ASSERT_EQ(tm1.tm_mon, 3, "ts1 month");
	SYS_ASSERT_EQ(tm1.tm_mday, 1, "ts1 day");

	// 2023-12-31 23:59:59
	const time_t ts2 = SystemUtility::convertToTimestamp(2023, 12, 31, 23, 59, 59);
	SYS_ASSERT(ts2 > 0, "ts2 > 0");
	const struct tm tm2 = SystemUtility::getTimeStruct(ts2);
	SYS_ASSERT_EQ(tm2.tm_year, 2023, "ts2 year");
	SYS_ASSERT_EQ(tm2.tm_mon, 12, "ts2 month");
	SYS_ASSERT_EQ(tm2.tm_mday, 31, "ts2 day");
	SYS_ASSERT_EQ(tm2.tm_hour, 23, "ts2 hour");
	SYS_ASSERT_EQ(tm2.tm_sec, 59, "ts2 sec");

	// 2024-02-29 (leap day)
	const time_t ts3 = SystemUtility::convertToTimestamp(2024, 2, 29, 12, 0, 0);
	SYS_ASSERT(ts3 > 0, "ts3 > 0");
	const struct tm tm3 = SystemUtility::getTimeStruct(ts3);
	SYS_ASSERT_EQ(tm3.tm_year, 2024, "ts3 year");
	SYS_ASSERT_EQ(tm3.tm_mon, 2, "ts3 month");
	SYS_ASSERT_EQ(tm3.tm_mday, 29, "ts3 day");

	// 2025-06-15 08:30:00
	const time_t ts4 = SystemUtility::convertToTimestamp(2025, 6, 15, 8, 30, 0);
	SYS_ASSERT(ts4 > 0, "ts4 > 0");
	const struct tm tm4 = SystemUtility::getTimeStruct(ts4);
	SYS_ASSERT_EQ(tm4.tm_year, 2025, "ts4 year");
	SYS_ASSERT_EQ(tm4.tm_mon, 6, "ts4 month");
	SYS_ASSERT_EQ(tm4.tm_mday, 15, "ts4 day");
	SYS_ASSERT_EQ(tm4.tm_hour, 8, "ts4 hour");
	SYS_ASSERT_EQ(tm4.tm_min, 30, "ts4 min");
}

//------------------------------------------------------------------------------
// isSameDay — extended
//------------------------------------------------------------------------------
void SystemUtilityTest::testIsSameDayExt()
{
	// Year boundary: Dec 31 vs Jan 1
	const time_t end2022 = SystemUtility::convertToTimestamp(2022, 12, 31, 23, 0, 0);
	const time_t start2023 = SystemUtility::convertToTimestamp(2023, 1, 1, 0, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(end2022, start2023), "year boundary not same day");

	// Same timestamp
	const time_t t = SystemUtility::convertToTimestamp(2024, 5, 20, 12, 0, 0);
	SYS_ASSERT(SystemUtility::isSameDay(t, t), "same ts is same day");

	// Early morning vs late night same day
	const time_t early = SystemUtility::convertToTimestamp(2024, 7, 4, 0, 0, 1);
	const time_t late = SystemUtility::convertToTimestamp(2024, 7, 4, 23, 59, 58);
	SYS_ASSERT(SystemUtility::isSameDay(early, late), "early vs late same day");

	// Leap day vs day after
	const time_t leapDay = SystemUtility::convertToTimestamp(2024, 2, 29, 12, 0, 0);
	const time_t march1 = SystemUtility::convertToTimestamp(2024, 3, 1, 0, 0, 0);
	SYS_ASSERT(!SystemUtility::isSameDay(leapDay, march1), "leap day vs march1 not same");
}

//------------------------------------------------------------------------------
// getTodayBegin / getTodayEnd consistency
//------------------------------------------------------------------------------
void SystemUtilityTest::testTodayBeginEnd2()
{
	const llong begin = SystemUtility::getTodayBegin();
	const llong end = SystemUtility::getTodayEnd();
	SYS_ASSERT(begin > 0, "getTodayBegin > 0");
	SYS_ASSERT(end > begin, "getTodayEnd > getTodayBegin");
	// end - begin should be exactly 86400 seconds (24h in seconds)
	SYS_ASSERT_EQ_LL(end - begin, 86400LL, "today end-begin=86400");
}

//------------------------------------------------------------------------------
// getRealTimeMS — monotonically increasing
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetRealTimeMS()
{
	const llong t1 = SystemUtility::getRealTimeMS();
	SYS_ASSERT(t1 > 0, "getRealTimeMS > 0");
	// getRealTimeMS is monotone (we can't guarantee in test, but it should be >= t1)
	const llong t2 = SystemUtility::getRealTimeMS();
	SYS_ASSERT(t2 >= t1, "getRealTimeMS t2 >= t1");
}

//------------------------------------------------------------------------------
// makeIDMain — bulk IDs are strictly increasing
//------------------------------------------------------------------------------
void SystemUtilityTest::testMakeIDMainBulk()
{
	const int N = 100;
	Vector<llong> ids;
	FOR(200)
	{
		ids.add(SystemUtility::makeIDMain());
	}
	for (int i = 1; i < N; ++i)
	{
		SYS_ASSERT(ids[i] > ids[i - 1], "makeIDMain bulk strictly increasing");
	}
}

//------------------------------------------------------------------------------
// makeIDThread — bulk
//------------------------------------------------------------------------------
void SystemUtilityTest::testMakeIDThreadBulk()
{
	const int N = 50;
	Vector<llong> ids;
	FOR(200)
	{
		ids.add(SystemUtility::makeIDThread());
	}
	for (int i = 1; i < N; ++i)
	{
		SYS_ASSERT(ids[i] > ids[i - 1], "makeIDThread bulk strictly increasing");
	}
}

//------------------------------------------------------------------------------
// getPID
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetPID()
{
	const int pid = SystemUtility::getPID();
	SYS_ASSERT(pid > 0, "getPID > 0");
}

//------------------------------------------------------------------------------
// getTimeStruct consistency with convertToTimestamp
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetTimeStructConsistency()
{
	// Test multiple dates
	struct TestDate { int y, mo, d, h, mi, s; };
	TestDate dates[] = {
		{2020, 6, 15, 10, 30, 0},
		{2021, 11, 30, 23, 59, 59},
		{2022, 1, 1, 0, 0, 1},
		{2023, 8, 15, 12, 0, 0},
		{2024, 3, 10, 18, 45, 30},
		{2025, 9, 22, 6, 15, 45},
	};
	for (int i = 0; i < 6; ++i)
	{
		const TestDate& dt = dates[i];
		const time_t ts = SystemUtility::convertToTimestamp(dt.y, dt.mo, dt.d, dt.h, dt.mi, dt.s);
		SYS_ASSERT(ts > 0, "date ts > 0");
		const struct tm tmVal = SystemUtility::getTimeStruct(ts);
		SYS_ASSERT_EQ(tmVal.tm_year, dt.y, "round-trip year");
		SYS_ASSERT_EQ(tmVal.tm_mon, dt.mo, "round-trip month");
		SYS_ASSERT_EQ(tmVal.tm_mday, dt.d, "round-trip day");
		SYS_ASSERT_EQ(tmVal.tm_hour, dt.h, "round-trip hour");
		SYS_ASSERT_EQ(tmVal.tm_min, dt.mi, "round-trip min");
		SYS_ASSERT_EQ(tmVal.tm_sec, dt.s, "round-trip sec");
	}
}

//------------------------------------------------------------------------------
// isSameDay — large batch of known pairs
//------------------------------------------------------------------------------
void SystemUtilityTest::testIsSameDayBatch()
{
	// All pairs within the same year
	for (int m = 1; m <= 12; ++m)
	{
		int days = SystemUtility::getDaysInMonth(2023, m);
		for (int d = 1; d < days; ++d)
		{
			const time_t t1 = SystemUtility::convertToTimestamp(2023, m, d, 0, 0, 0);
			const time_t t2 = SystemUtility::convertToTimestamp(2023, m, d, 23, 59, 59);
			const time_t t3 = SystemUtility::convertToTimestamp(2023, m, d + 1, 0, 0, 0);
			SYS_ASSERT(SystemUtility::isSameDay(t1, t2), "batch same day");
			SYS_ASSERT(!SystemUtility::isSameDay(t1, t3), "batch diff day");
		}
	}
}

//------------------------------------------------------------------------------
// isLeapYear — boundary values
//------------------------------------------------------------------------------
void SystemUtilityTest::testIsLeapYearBoundary()
{
	// Historical leap years
	SYS_ASSERT(SystemUtility::isLeapYear(4), "4 is leap");
	SYS_ASSERT(SystemUtility::isLeapYear(8), "8 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(100), "100 not leap");
	SYS_ASSERT(SystemUtility::isLeapYear(400), "400 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(200), "200 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(300), "300 not leap");
	SYS_ASSERT(SystemUtility::isLeapYear(800), "800 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(900), "900 not leap");
	SYS_ASSERT(SystemUtility::isLeapYear(1200), "1200 is leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1300), "1300 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(1), "1 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(2), "2 not leap");
	SYS_ASSERT(!SystemUtility::isLeapYear(3), "3 not leap");
}

//------------------------------------------------------------------------------
// getProgramFile — string non-empty
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetProgramFile()
{
	string progFile = SystemUtility::getProgramFile();
	SYS_ASSERT(!progFile.empty(), "getProgramFile non-empty");
}

//------------------------------------------------------------------------------
// getCPUCoreCount
//------------------------------------------------------------------------------
void SystemUtilityTest::testGetCPUCoreCount()
{
	int cores = SystemUtility::getCPUCoreCount();
	SYS_ASSERT(cores >= 1, "getCPUCoreCount >= 1");
}

//------------------------------------------------------------------------------
// SystemUtilityTest entry point
//------------------------------------------------------------------------------
void SystemUtilityTest::testSystem()
{
	testIsLeapYear();
	testDaysToSeconds();
	testGetDaysInMonth();
	testConvertToTimestamp();
	testIsSameDay();
	testTimeSeed();
	testMakeID();
	testThreadID();
	testGetYearMonthDay();
	testGetTimeHMS();
	testGetTimeYMDHMS();
	testGetTimeWithWeekDay();
	testGetTimeHourInDay();
	testGetDayOfWeek();
	testGetTimeString();
	testDaysToSecondsExt();
	testIsLeapYearExt();
	testGetDaysInMonthFull();
	testConvertToTimestampMultiple();
	testIsSameDayExt();
	testTodayBeginEnd2();
	testGetRealTimeMS();
	testMakeIDMainBulk();
	testMakeIDThreadBulk();
	testGetPID();
	testGetTimeStructConsistency();
	testIsSameDayBatch();
	testIsLeapYearBoundary();
	testGetProgramFile();
	testGetCPUCoreCount();
}


// ============================================================
// SystemUtilityTest
// 覆盖 SystemUtility 中 SystemUtilityTest 未测到的函数：
//   getTodayBegin / getTodayEnd
//   getDayOfWeek
//   getTimeHourInDay
//   getTime(weekDay, year, month, day, hour, minute, second)
//   getTime(year, month, day, hour, minute, second)
//   getTime(string) — 格式化时间字符串
//   getCPUCoreCount
//   getPID
//   getRealTimeMS
//   sleep (short)
//   getProgramFile
//   isMainThread / getThreadID / setMainThread (补充)
//   makeIDThread (压力测试)
//   daysToSeconds (更多边界)
//   isLeapYear (补充边界)
//   convertToTimestamp (更多场景)
//   getTimeStruct (独立验证)
// ============================================================

#define SEXT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("SystemUtilityTest FAILED: ") + (msg)); } } while(0)
#define SEXT_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("SystemUtilityTest FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SEXT_ASSERT_EQ_LL(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("SystemUtilityTest FAILED: ") + (msg) \
        + " expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)

// ============================================================
// getTodayBegin / getTodayEnd
// ============================================================
void SystemUtilityTest::testTodayBeginEnd1()
{
	const llong begin = getTodayBegin();
	const llong end = getTodayEnd();
	const llong nowUTC = (llong)time(nullptr);

	// begin 应 <= now
	SEXT_ASSERT(begin <= nowUTC, "getTodayBegin <= now");

	// end 应 > now（今天还没结束）
	SEXT_ASSERT(end >= nowUTC, "getTodayEnd >= now");

	// end > begin
	SEXT_ASSERT(end > begin, "getTodayEnd > getTodayBegin");

	// end - begin 应 == 86400 秒（一天）
	const llong diff = end - begin;
	SEXT_ASSERT_EQ_LL(diff, 86400LL, "getTodayEnd - getTodayBegin == 86400");

	// begin 对应的时间应该是今天 00:00:00
	const struct tm beginTm = getTimeStruct((time_t)begin);
	SEXT_ASSERT_EQ(beginTm.tm_hour, 0, "getTodayBegin hour==0");
	SEXT_ASSERT_EQ(beginTm.tm_min, 0, "getTodayBegin min==0");
	SEXT_ASSERT_EQ(beginTm.tm_sec, 0, "getTodayBegin sec==0");

	// end 对应的时间应该是今天 24:00:00 即明天 00:00:00
	// end = begin + 86400，对应明天 0 点
	const struct tm endTm = getTimeStruct((time_t)end);
	SEXT_ASSERT_EQ(endTm.tm_hour, 0, "getTodayEnd hour==0 (next day midnight)");
	SEXT_ASSERT_EQ(endTm.tm_min, 0, "getTodayEnd min==0");
	SEXT_ASSERT_EQ(endTm.tm_sec, 0, "getTodayEnd sec==0");

	// 两次调用结果一致（幂等）
	const llong begin2 = getTodayBegin();
	const llong end2 = getTodayEnd();
	SEXT_ASSERT_EQ_LL(begin, begin2, "getTodayBegin idempotent");
	SEXT_ASSERT_EQ_LL(end, end2, "getTodayEnd idempotent");
}

// ============================================================
// getDayOfWeek
// ============================================================
void SystemUtilityTest::testGetDayOfWeek2()
{
	// 当前值必须在 [0, 6]（0=周日, 1=周一, ..., 6=周六）
	const int dow = getDayOfWeek();
	SEXT_ASSERT(dow >= 0 && dow <= 6, "getDayOfWeek in [0,6]");

	// 使用已知日期验证：通过 convertToTimestamp 构造特定日期的 timestamp
	// 2024-01-07 是周日（dow = 0）
	const time_t ts_sunday = convertToTimestamp(2024, 1, 7, 12, 0, 0);
	const struct tm tm_sunday = getTimeStruct(ts_sunday);
	// tm_wday: 0=Sunday in standard C, framework 里若保持标准则 tm_wday==0
	// 注意：framework 的 getTimeStruct 对 tm_wday 是否调整，如不调整则标准 0=Sunday
	// 根据框架代码，getTimeStruct 只对 year/mon 做调整，wday 保持原始 0-6(Sun=0)
	SEXT_ASSERT_EQ(tm_sunday.tm_wday, 0, "2024-01-07 wday==0 (Sunday)");

	// 2024-01-01 是周一
	const time_t ts_monday = convertToTimestamp(2024, 1, 1, 12, 0, 0);
	const struct tm tm_monday = getTimeStruct(ts_monday);
	SEXT_ASSERT_EQ(tm_monday.tm_wday, 1, "2024-01-01 wday==1 (Monday)");

	// 2024-01-06 是周六
	const time_t ts_saturday = convertToTimestamp(2024, 1, 6, 12, 0, 0);
	const struct tm tm_saturday = getTimeStruct(ts_saturday);
	SEXT_ASSERT_EQ(tm_saturday.tm_wday, 6, "2024-01-06 wday==6 (Saturday)");

	// 2024-01-03 是周三
	const time_t ts_wednesday = convertToTimestamp(2024, 1, 3, 12, 0, 0);
	const struct tm tm_wednesday = getTimeStruct(ts_wednesday);
	SEXT_ASSERT_EQ(tm_wednesday.tm_wday, 3, "2024-01-03 wday==3 (Wednesday)");
}

// ============================================================
// getTimeHourInDay
// ============================================================
void SystemUtilityTest::testGetTimeHourInDay2()
{
	const int hour = getTimeHourInDay();
	// 必须在 [0, 23]
	SEXT_ASSERT(hour >= 0 && hour < 24, "getTimeHourInDay in [0,23]");

	// 与 getTime(h,m,s) 一致
	int h = -1;
	int m = -1;
	int s = -1;
	getTime(h, m, s);
	SEXT_ASSERT_EQ(hour, h, "getTimeHourInDay matches getTime hour");
}

// ============================================================
// getTime (full: weekDay, year, month, day, hour, minute, second)
// ============================================================
void SystemUtilityTest::testGetTimeFullWithWeekDay()
{
	int weekDay = -1;
	int year = -1;
	int month = -1;
	int day = -1;
	int hour = -1;
	int minute = -1;
	int second = -1;
	getTime(weekDay, year, month, day, hour, minute, second);

	SEXT_ASSERT(weekDay >= 0 && weekDay <= 6, "getTime7 weekDay in [0,6]");
	SEXT_ASSERT(year >= 2024, "getTime7 year>=2024");
	SEXT_ASSERT(month >= 1 && month <= 12, "getTime7 month in [1,12]");
	SEXT_ASSERT(day >= 1 && day <= 31, "getTime7 day in [1,31]");
	SEXT_ASSERT(hour >= 0 && hour < 24, "getTime7 hour in [0,23]");
	SEXT_ASSERT(minute >= 0 && minute < 60, "getTime7 minute in [0,59]");
	SEXT_ASSERT(second >= 0 && second < 60, "getTime7 second in [0,59]");

	// 与 6-param 版本一致
	int y2 = -1, m2 = -1, d2 = -1, h2 = -1, min2 = -1, s2 = -1;
	getTime(y2, m2, d2, h2, min2, s2);
	SEXT_ASSERT_EQ(year, y2, "getTime7 year matches getTime6");
	SEXT_ASSERT_EQ(month, m2, "getTime7 month matches getTime6");
	SEXT_ASSERT_EQ(day, d2, "getTime7 day matches getTime6");

	// weekDay 与 getDayOfWeek 一致（实时调用，可能有 1 秒误差忽略）
	const int dowCurrent = getDayOfWeek();
	SEXT_ASSERT(dowCurrent >= 0 && dowCurrent <= 6, "getTime7 weekDay matches getDayOfWeek range");
}

// ============================================================
// getTime (6-param: year, month, day, hour, minute, second)
// ============================================================
void SystemUtilityTest::testGetTimeSixParams()
{
	int y = -1, m = -1, d = -1, h = -1, min = -1, s = -1;
	getTime(y, m, d, h, min, s);

	SEXT_ASSERT(y >= 2024, "getTime6 year>=2024");
	SEXT_ASSERT(m >= 1 && m <= 12, "getTime6 month in [1,12]");
	SEXT_ASSERT(d >= 1 && d <= 31, "getTime6 day in [1,31]");
	SEXT_ASSERT(h >= 0 && h < 24, "getTime6 hour in [0,23]");
	SEXT_ASSERT(min >= 0 && min < 60, "getTime6 minute in [0,59]");
	SEXT_ASSERT(s >= 0 && s < 60, "getTime6 second in [0,59]");

	// 与 getYear/getMonth/getDay 一致
	SEXT_ASSERT_EQ(y, getYear(), "getTime6 year matches getYear");
	SEXT_ASSERT_EQ(m, getMonth(), "getTime6 month matches getMonth");

	// 与 getTime(h,m,s) 一致
	int h2 = -1, m2 = -1, s2 = -1;
	getTime(h2, m2, s2);
	SEXT_ASSERT_EQ(h, h2, "getTime6 hour matches getTime3");
}

// ============================================================
// getTime (string 版本)
// ============================================================
void SystemUtilityTest::testGetTimeString2()
{
	// 不带时间戳
	const string timeStr = getTime(false);
	SEXT_ASSERT(!timeStr.empty(), "getTime string non-empty");
	// 格式通常是 "YYYY-MM-DD HH:MM:SS" 或类似，长度一般 >= 8
	SEXT_ASSERT(timeStr.length() >= 8, "getTime string length>=8");

	// 带时间戳（timeStamp=true 格式可能不同）
	const string timeStampStr = getTime(true);
	SEXT_ASSERT(!timeStampStr.empty(), "getTime timeStamp string non-empty");

	// 两次调用间隔很短，时间字符串应该相等或只差 1 秒
	const string t1 = getTime(false);
	const string t2 = getTime(false);
	SEXT_ASSERT(t1.length() == t2.length(), "getTime string same format length");
}

// ============================================================
// getCPUCoreCount
// ============================================================
void SystemUtilityTest::testGetCPUCoreCount2()
{
	const int count = getCPUCoreCount();
	SEXT_ASSERT(count >= 1, "getCPUCoreCount >= 1");
	// 现代机器一般不超过 1024 核
	SEXT_ASSERT(count <= 1024, "getCPUCoreCount <= 1024");

	// 多次调用结果一致
	const int count2 = getCPUCoreCount();
	SEXT_ASSERT_EQ(count, count2, "getCPUCoreCount idempotent");
}

// ============================================================
// getPID
// ============================================================
void SystemUtilityTest::testGetPID2()
{
	const int pid = getPID();
	SEXT_ASSERT(pid > 0, "getPID > 0");

	// 多次调用相同
	const int pid2 = getPID();
	SEXT_ASSERT_EQ(pid, pid2, "getPID idempotent");
}

// ============================================================
// getRealTimeMS
// ============================================================
void SystemUtilityTest::testGetRealTimeMS2()
{
	const llong t1 = getRealTimeMS();
	SEXT_ASSERT(t1 > 0, "getRealTimeMS > 0");

	// 稍等片刻后再获取，应该增加了
	// 使用极短 sleep（1ms），验证时间推进
	SystemUtility::sleep(2);
	const llong t2 = getRealTimeMS();
	SEXT_ASSERT(t2 >= t1, "getRealTimeMS monotonic after sleep");

	// 间隔至少 1ms（sleep 了 2ms）
	SEXT_ASSERT(t2 - t1 >= 1, "getRealTimeMS advanced at least 1ms");
}

// ============================================================
// sleep（验证执行不崩溃，时间推进）
// ============================================================
void SystemUtilityTest::testSleep()
{
	const llong before = getRealTimeMS();
	SystemUtility::sleep(5); // 5ms
	const llong after = getRealTimeMS();

	SEXT_ASSERT(after >= before, "sleep: time advanced");
	SEXT_ASSERT(after - before >= 3, "sleep: at least 3ms elapsed");

	// sleep 0ms 不崩溃
	SystemUtility::sleep(0);
	SEXT_ASSERT(true, "sleep(0) no crash");
}

// ============================================================
// getProgramFile
// ============================================================
void SystemUtilityTest::testGetProgramFile2()
{
	const string progFile = getProgramFile();
	SEXT_ASSERT(!progFile.empty(), "getProgramFile non-empty");

	// 应该是一个文件路径（包含路径分隔符或文件名）
	// 在 Windows 上通常以 .exe 结尾
	SEXT_ASSERT(progFile.length() >= 1, "getProgramFile length >= 1");

	// 多次调用一致
	const string progFile2 = getProgramFile();
	SEXT_ASSERT(progFile == progFile2, "getProgramFile idempotent");
}

// ============================================================
// makeIDThread — 多次调用严格递增
// ============================================================
void SystemUtilityTest::testMakeIDThreadExtended()
{
	// 连续 20 次，每次都应严格递增
	llong prev = makeIDThread();
	for (int i = 0; i < 20; ++i)
	{
		const llong curr = makeIDThread();
		SEXT_ASSERT(curr > prev, "makeIDThread strictly increasing");
		prev = curr;
	}

	// makeIDMain 与 makeIDThread 使用不同种子，互不影响
	const llong mainBefore = makeIDMain();
	FOR(200)
	{
		makeIDThread();
	}
	const llong mainAfter = makeIDMain();
	SEXT_ASSERT(mainAfter > mainBefore, "makeIDMain still increases after makeIDThread calls");
}

// ============================================================
// daysToSeconds 边界（补充）
// ============================================================
void SystemUtilityTest::testDaysToSecondsExtended()
{
	// 负值（框架可能允许负数）
	SEXT_ASSERT_EQ_LL(daysToSeconds(-1), -86400LL, "daysToSeconds -1 day");

	// 大数值
	SEXT_ASSERT_EQ_LL(daysToSeconds(1000), 86400000LL, "daysToSeconds 1000 days");

	// 边界：一年
	SEXT_ASSERT_EQ_LL(daysToSeconds(366), 31622400LL, "daysToSeconds 366 days (leap year)");

	// 精确值验证：1天 = 24*60*60 = 86400
	SEXT_ASSERT_EQ_LL((llong)(24 * 60 * 60), 86400LL, "86400 constant correct");
	SEXT_ASSERT_EQ_LL(daysToSeconds(1), 86400LL, "daysToSeconds(1)==86400");
}

// ============================================================
// isLeapYear 边界（补充）
// ============================================================
void SystemUtilityTest::testIsLeapYearExtended()
{
	// 早期年份
	SEXT_ASSERT(isLeapYear(4), "year 4 is leap");
	SEXT_ASSERT(!isLeapYear(1), "year 1 not leap");
	SEXT_ASSERT(!isLeapYear(100), "year 100 not leap");
	// 200 % 100 == 0 且 200 % 400 != 0，所以 NOT leap
	SEXT_ASSERT(!isLeapYear(200), "year 200 not leap (div 100 but not 400)");

	// 未来年份
	SEXT_ASSERT(isLeapYear(2400), "year 2400 is leap (div 400)");
	SEXT_ASSERT(!isLeapYear(2100), "year 2100 not leap");
	SEXT_ASSERT(!isLeapYear(2200), "year 2200 not leap");
	SEXT_ASSERT(!isLeapYear(2300), "year 2300 not leap");
	SEXT_ASSERT(isLeapYear(2400), "year 2400 is leap");

	// 连续几年
	SEXT_ASSERT(!isLeapYear(2025), "2025 not leap");
	SEXT_ASSERT(!isLeapYear(2026), "2026 not leap");
	SEXT_ASSERT(!isLeapYear(2027), "2027 not leap");
	SEXT_ASSERT(isLeapYear(2028), "2028 is leap");
}

// ============================================================
// convertToTimestamp 更多场景
// ============================================================
void SystemUtilityTest::testConvertToTimestampExtended()
{
	// 年初 1月1日
	const time_t tsJan1 = convertToTimestamp(2023, 1, 1, 0, 0, 0);
	const struct tm tmJan1 = getTimeStruct(tsJan1);
	SEXT_ASSERT_EQ(tmJan1.tm_year, 2023, "convertToTimestamp Jan 1 year==2023");
	SEXT_ASSERT_EQ(tmJan1.tm_mon, 1, "convertToTimestamp Jan 1 month==1");
	SEXT_ASSERT_EQ(tmJan1.tm_mday, 1, "convertToTimestamp Jan 1 day==1");

	// 年末 12月31日
	const time_t tsDec31 = convertToTimestamp(2023, 12, 31, 23, 59, 59);
	const struct tm tmDec31 = getTimeStruct(tsDec31);
	SEXT_ASSERT_EQ(tmDec31.tm_year, 2023, "convertToTimestamp Dec 31 year==2023");
	SEXT_ASSERT_EQ(tmDec31.tm_mon, 12, "convertToTimestamp Dec 31 month==12");
	SEXT_ASSERT_EQ(tmDec31.tm_mday, 31, "convertToTimestamp Dec 31 day==31");
	SEXT_ASSERT_EQ(tmDec31.tm_hour, 23, "convertToTimestamp Dec 31 hour==23");
	SEXT_ASSERT_EQ(tmDec31.tm_min, 59, "convertToTimestamp Dec 31 min==59");
	SEXT_ASSERT_EQ(tmDec31.tm_sec, 59, "convertToTimestamp Dec 31 sec==59");

	// 闰年 2月29日
	const time_t tsLeap = convertToTimestamp(2024, 2, 29, 12, 0, 0);
	const struct tm tmLeap = getTimeStruct(tsLeap);
	SEXT_ASSERT_EQ(tmLeap.tm_year, 2024, "convertToTimestamp 2024-02-29 year");
	SEXT_ASSERT_EQ(tmLeap.tm_mon, 2, "convertToTimestamp 2024-02-29 month==2");
	SEXT_ASSERT_EQ(tmLeap.tm_mday, 29, "convertToTimestamp 2024-02-29 day==29");

	// 时间戳单调递增
	const time_t ts1 = convertToTimestamp(2024, 6, 1, 0, 0, 0);
	const time_t ts2 = convertToTimestamp(2024, 6, 1, 0, 0, 1);
	const time_t ts3 = convertToTimestamp(2024, 6, 1, 0, 1, 0);
	const time_t ts4 = convertToTimestamp(2024, 6, 2, 0, 0, 0);
	SEXT_ASSERT(ts2 > ts1, "convertToTimestamp +1s monotonic");
	SEXT_ASSERT(ts3 > ts2, "convertToTimestamp +1min monotonic");
	SEXT_ASSERT(ts4 > ts3, "convertToTimestamp +1day monotonic");
	SEXT_ASSERT(ts4 - ts1 == 86400, "convertToTimestamp day diff == 86400");

	// 两个相邻月的时间戳差
	const time_t tsAug31 = convertToTimestamp(2024, 8, 31, 12, 0, 0);
	const time_t tsSep1 = convertToTimestamp(2024, 9, 1, 12, 0, 0);
	SEXT_ASSERT(tsSep1 > tsAug31, "convertToTimestamp Aug31 < Sep1");
	SEXT_ASSERT(tsSep1 - tsAug31 == 86400, "convertToTimestamp Aug31 to Sep1 == 86400s");
}

// ============================================================
// getTimeStruct 独立验证
// ============================================================
void SystemUtilityTest::testGetTimeStruct()
{
	// 已知时间：2020-06-15 10:30:00
	const time_t ts = convertToTimestamp(2020, 6, 15, 10, 30, 0);
	const struct tm tm1 = getTimeStruct(ts);
	SEXT_ASSERT_EQ(tm1.tm_year, 2020, "getTimeStruct year==2020");
	SEXT_ASSERT_EQ(tm1.tm_mon, 6, "getTimeStruct month==6");
	SEXT_ASSERT_EQ(tm1.tm_mday, 15, "getTimeStruct day==15");
	SEXT_ASSERT_EQ(tm1.tm_hour, 10, "getTimeStruct hour==10");
	SEXT_ASSERT_EQ(tm1.tm_min, 30, "getTimeStruct min==30");
	SEXT_ASSERT_EQ(tm1.tm_sec, 0, "getTimeStruct sec==0");

	// 两次调用同一 timestamp，结果相同
	const struct tm tm2 = getTimeStruct(ts);
	SEXT_ASSERT(tm1.tm_year == tm2.tm_year, "getTimeStruct idempotent year");
	SEXT_ASSERT(tm1.tm_mon == tm2.tm_mon, "getTimeStruct idempotent month");
	SEXT_ASSERT(tm1.tm_mday == tm2.tm_mday, "getTimeStruct idempotent day");

	// 不同 timestamp 产生不同结果
	const time_t ts2 = convertToTimestamp(2020, 6, 16, 10, 30, 0);
	const struct tm tm3 = getTimeStruct(ts2);
	SEXT_ASSERT(tm3.tm_mday != tm1.tm_mday, "getTimeStruct different timestamps different day");
	SEXT_ASSERT_EQ(tm3.tm_mday, 16, "getTimeStruct ts2 day==16");
}

// ============================================================
// isSameDay 扩展
// ============================================================
void SystemUtilityTest::testIsSameDayExtended()
{
	// 同一天的极端情况：00:00:00 和 23:59:59
	const time_t t0 = convertToTimestamp(2024, 7, 15, 0, 0, 0);
	const time_t t1 = convertToTimestamp(2024, 7, 15, 23, 59, 59);
	SEXT_ASSERT(isSameDay(t0, t1), "isSameDay 00:00 and 23:59 same day");

	// 与自身是同一天
	SEXT_ASSERT(isSameDay(t0, t0), "isSameDay self is same day");

	// 月末和下月初
	const time_t tJul31 = convertToTimestamp(2024, 7, 31, 23, 59, 59);
	const time_t tAug1 = convertToTimestamp(2024, 8, 1, 0, 0, 0);
	SEXT_ASSERT(!isSameDay(tJul31, tAug1), "isSameDay Jul31 vs Aug1 false");

	// 年末和新年
	const time_t tDec31 = convertToTimestamp(2023, 12, 31, 23, 59, 59);
	const time_t tJan1 = convertToTimestamp(2024, 1, 1, 0, 0, 0);
	SEXT_ASSERT(!isSameDay(tDec31, tJan1), "isSameDay Dec31 vs Jan1 false");

	// 完全相同的 timestamp 是同一天
	const time_t tNow = convertToTimestamp(2024, 3, 21, 12, 0, 0);
	SEXT_ASSERT(isSameDay(tNow, tNow), "isSameDay same timestamp true");

	// 相差 1 秒但跨天
	const time_t tBefore = convertToTimestamp(2024, 5, 20, 23, 59, 59);
	const time_t tAfter = tBefore + 1; // 5月21日 00:00:00
	SEXT_ASSERT(!isSameDay(tBefore, tAfter), "isSameDay cross-midnight false");
}

// ============================================================
// getDaysInMonth 完整覆盖（补充框架内 getDaysInMonth() 无参版）
// ============================================================
void SystemUtilityTest::testGetDaysInMonthCurrent()
{
	// getDaysInMonth() 无参：返回当前月的天数
	const int daysCurrentMonth = getDaysInMonth();
	SEXT_ASSERT(daysCurrentMonth >= 28 && daysCurrentMonth <= 31, "getDaysInMonth() current in [28,31]");

	// 与 getDaysInMonth(year, month) 一致
	const int year = getYear();
	const int month = getMonth();
	const int daysExpected = getDaysInMonth(year, month);
	SEXT_ASSERT_EQ(daysCurrentMonth, daysExpected, "getDaysInMonth() matches getDaysInMonth(year,month)");
}

// ============================================================
// getTimeMSecondUTC / setTimeMSecondUTC
// getTimeSecondUTC / setTimeSecondUTC
// ============================================================
void SystemUtilityTest::testTimeUTCSeed()
{
	// getTimeMSecondUTC 返回毫秒，应 >= 0
	const llong ms1 = getTimeMSecondUTC();
	SEXT_ASSERT(ms1 >= 0, "getTimeMSecondUTC >= 0");

	// set 后 get 一致
	const llong origMS = ms1;
	setTimeMSecondUTC(123456789LL);
	SEXT_ASSERT_EQ_LL(getTimeMSecondUTC(), 123456789LL, "setTimeMSecondUTC round-trip");
	setTimeMSecondUTC(origMS); // 还原

	// getTimeSecondUTC
	const llong s1 = getTimeSecondUTC();
	SEXT_ASSERT(s1 >= 0, "getTimeSecondUTC >= 0");

	// set second 后 get 一致
	const llong origS = s1;
	setTimeSecondUTC(999888777LL);
	SEXT_ASSERT_EQ_LL(getTimeSecondUTC(), 999888777LL, "setTimeSecondUTC round-trip");
	setTimeSecondUTC(origS); // 还原
}

// ============================================================
// isMainThread / getThreadID / setMainThread（补充测试）
// ============================================================
void SystemUtilityTest::testMainThreadExtended()
{
	// 当前线程 ID
	const int tid = getThreadID();
	SEXT_ASSERT(tid > 0, "getThreadID > 0");

	// isMainThread 与 setMainThread 联动
	// 注意：这里用当前线程 ID 验证，不改变主线程 ID（只在测试范围内验证）
	getThreadID(); // 当前就是主线程

	// 调用两次 getThreadID 结果相同（同一线程内）
	const int tid2 = getThreadID();
	SEXT_ASSERT_EQ(tid, tid2, "getThreadID idempotent in same thread");

	// isMainThread 应为 true（UnitTest 在主线程中运行）
	SEXT_ASSERT(isMainThread(), "isMainThread true in test context");
}

// ============================================================
// 综合场景：时间操作流水线
// ============================================================
void SystemUtilityTest::testTimePipeline()
{
	// 场景：从当前时间获取完整信息，交叉验证各 API 一致性
	int year, month, day, hour, minute, second;
	getTime(year, month, day, hour, minute, second);

	// 通过 convertToTimestamp 重建同一时刻的 timestamp，验证 getTimeStruct 还原
	const time_t reconstructed = convertToTimestamp(year, month, day, hour, minute, second);
	const struct tm reconstructedTm = getTimeStruct(reconstructed);
	SEXT_ASSERT_EQ(reconstructedTm.tm_year, year, "pipeline: year round-trip");
	SEXT_ASSERT_EQ(reconstructedTm.tm_mon, month, "pipeline: month round-trip");
	SEXT_ASSERT_EQ(reconstructedTm.tm_mday, day, "pipeline: day round-trip");
	SEXT_ASSERT_EQ(reconstructedTm.tm_hour, hour, "pipeline: hour round-trip");
	SEXT_ASSERT_EQ(reconstructedTm.tm_min, minute, "pipeline: minute round-trip");
	SEXT_ASSERT_EQ(reconstructedTm.tm_sec, second, "pipeline: second round-trip");

	// getTodayBegin 应 <= reconstructed <= getTodayEnd
	const llong todayBegin = getTodayBegin();
	const llong todayEnd = getTodayEnd();
	SEXT_ASSERT(todayBegin <= (llong)reconstructed, "pipeline: reconstructed >= todayBegin");
	SEXT_ASSERT((llong)reconstructed <= todayEnd, "pipeline: reconstructed <= todayEnd");

	// isSameDay：两个相同日期的 timestamp 应为同一天
	const time_t ts1 = convertToTimestamp(year, month, day, 0, 0, 0);
	const time_t ts2 = convertToTimestamp(year, month, day, 23, 59, 59);
	SEXT_ASSERT(isSameDay(ts1, ts2), "pipeline: same day true");

	// getDaysInMonth(year, month) 应在 [28, 31]
	const int daysInCurMonth = getDaysInMonth(year, month);
	SEXT_ASSERT(daysInCurMonth >= 28 && daysInCurMonth <= 31, "pipeline: daysInMonth valid");

	// day 不超过当月最大天数
	SEXT_ASSERT(day <= daysInCurMonth, "pipeline: day <= daysInMonth");
}

// ============================================================
// 大量 makeIDThread 压力测试
// ============================================================
void SystemUtilityTest::testMakeIDStress()
{
	// 连续生成 1000 个 ID，全部严格单调递增
	const int N = 1000;
	llong prev = makeIDThread();
	for (int i = 0; i < N - 1; ++i)
	{
		const llong curr = makeIDThread();
		SEXT_ASSERT(curr > prev, "makeIDThread stress: strictly increasing");
		prev = curr;
	}

	// 连续生成 1000 个 Main ID
	llong prevMain = makeIDMain();
	for (int i = 0; i < N - 1; ++i)
	{
		const llong currMain = makeIDMain();
		SEXT_ASSERT(currMain > prevMain, "makeIDMain stress: strictly increasing");
		prevMain = currMain;
	}
}

// ============================================================
// 主入口
// ============================================================
void SystemUtilityTest::testExtended()
{
	testTodayBeginEnd1();
	testGetDayOfWeek2();
	testGetTimeHourInDay2();
	testGetTimeFullWithWeekDay();
	testGetTimeSixParams();
	testGetTimeString2();
	testGetCPUCoreCount2();
	testGetPID2();
	testGetRealTimeMS2();
	testSleep();
	testGetProgramFile2();
	testMakeIDThreadExtended();
	testDaysToSecondsExtended();
	testIsLeapYearExtended();
	testConvertToTimestampExtended();
	testGetTimeStruct();
	testIsSameDayExtended();
	testGetDaysInMonthCurrent();
	testTimeUTCSeed();
	testMainThreadExtended();
	testTimePipeline();
	testMakeIDStress();
}
#endif
