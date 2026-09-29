#pragma once

#include "MathUtility.h"
#include "ThreadLock.h"

namespace SystemUtility
{
	MICRO_LEGEND_FRAME_API extern llong mIDSeedMain;			// 用于生成唯一ID的种子,仅用于主线程
	MICRO_LEGEND_FRAME_API extern atomic<llong> mIDSeedThread;	// 用于生成唯一ID的种子,用于多线程
	MICRO_LEGEND_FRAME_API extern llong mTimeMSecondUTC;		// 系统从启动到现在所经过的毫秒,每帧获取一次,避免频繁获取造成性能下降
	MICRO_LEGEND_FRAME_API extern llong mTimeSecondUTC;			// 从1970年1月1日到现在的秒数,每帧获取一次,避免频繁获取造成性能下降
	MICRO_LEGEND_FRAME_API extern int mMainThread;				// 主线程的线程ID
	//------------------------------------------------------------------------------------------------------------------------------
	MICRO_LEGEND_FRAME_API void stop();
	inline void sleep(ulong timeMS)
	{
#ifdef WINDOWS
		Sleep(timeMS);
#elif defined LINUX
		usleep(timeMS * 1000);
#endif
	}
	// 只需要在windows下才会暂停,linux中不需要暂停
	inline void systemPause()
	{
#ifdef WINDOWS
		system("pause");
#endif
	}
	// 获取系统从启动到现在所经过的毫秒,每帧更新一次的
	inline llong getTimeMSecondUTC() { return mTimeMSecondUTC; }
	inline void setTimeMSecondUTC(const llong timeMS) { mTimeMSecondUTC = timeMS; }
	// 获取系统从启动到现在所经过的毫秒,实时的
	inline llong getRealTimeMS()
	{
#ifdef WINDOWS
		return timeGetTime();
#elif defined LINUX
		struct timeval tv;
		if (gettimeofday(&tv, nullptr) != 0)
		{
			ERROR("time get error : " + IToS(errno));
		}
		return tv.tv_sec * 1000 + (llong)(tv.tv_usec * 0.001f);
#endif
	}
	// 是否为闰年
	constexpr bool isLeapYear(int year) { return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); }
	// 获取当前这个月有多少天
	MICRO_LEGEND_FRAME_API int getDaysInMonth();
	// 获取一个月有多少天
	MICRO_LEGEND_FRAME_API int getDaysInMonth(int year, int month);
	// 获取年份,从1970开始
	MICRO_LEGEND_FRAME_API int getYear();
	// 获取月份,从1开始
	MICRO_LEGEND_FRAME_API int getMonth();
	// 获取年月份,从1开始
	MICRO_LEGEND_FRAME_API void getYearMonth(int& year, int& month);
	// 获取天,从1开始
	MICRO_LEGEND_FRAME_API int getDay();
	// 获取当前时间的周几(从1开始),年(从1970开始),月(从1开始),天(从1开始),时(从0开始),分,秒
	MICRO_LEGEND_FRAME_API void getTime(int& weekDay, int& year, int& month, int& day, int& hour, int& minute, int& second);
	// 获取当前时间的年,月,天,时,分,秒
	MICRO_LEGEND_FRAME_API void getTime(int& year, int& month, int& day, int& hour, int& minute, int& second);
	// 获取当前时间的时,分,秒
	MICRO_LEGEND_FRAME_API void getTime(int& hour, int& minute, int& second);
	// 获取当前时间是今天的几点,24小时制
	MICRO_LEGEND_FRAME_API int getTimeHourInDay();
	// 获取当前时间,以字符串形式表示
	MICRO_LEGEND_FRAME_API string getTime(bool timeStamp = false);
	// 获取从1970年1月1日到现在的秒数,本地时间,UTC时间
	inline llong getTimeSecondUTC() { return mTimeSecondUTC; }
	inline void setTimeSecondUTC(llong timeS) { mTimeSecondUTC = timeS; }
	constexpr int daysToSeconds(int days) { return 24 * 60 * 60 * days; }
	// 获取今天24点的时间戳
	MICRO_LEGEND_FRAME_API llong getTodayEnd();
	// 获取今天0点的时间戳
	MICRO_LEGEND_FRAME_API llong getTodayBegin();
	// 获得今天是周几,0表示周日,1表示周一
	MICRO_LEGEND_FRAME_API int getDayOfWeek();
	// 判断两次时间戳是否是在同一天
	MICRO_LEGEND_FRAME_API bool isSameDay(time_t timeStamp0, time_t timeStamp1);
	// 根据时间戳获取时间结构体
	MICRO_LEGEND_FRAME_API struct tm getTimeStruct(time_t timeStamp);
	// 获取指定时间的时间戳
	MICRO_LEGEND_FRAME_API time_t convertToTimestamp(int year, int month, int day, int hour, int minute, int second);
	// 获得cpu核心数
	MICRO_LEGEND_FRAME_API int getCPUCoreCount();
	// 获取外网IP
	MICRO_LEGEND_FRAME_API string getInternetIP(MY_SOCKET socket);
	// 获取内网IP
	MICRO_LEGEND_FRAME_API string getLocalIP(MY_SOCKET socket);
	MICRO_LEGEND_FRAME_API void makeSockAddrByHost(sockaddr_in& addr, const char* host, ushort port);
	MICRO_LEGEND_FRAME_API void makeSockAddrByIP(sockaddr_in& addr, const char* ip, ushort port);
	MICRO_LEGEND_FRAME_API void makeSockAddr(sockaddr_in& addr, ulong hostlong, ushort port);
	MICRO_LEGEND_FRAME_API void print(const char* str, bool nextLine = true);
	MICRO_LEGEND_FRAME_API void input(string& str);
	inline int getThreadID()
	{
#ifdef WINDOWS
		return (int)GetCurrentThreadId();
#elif defined LINUX
		// 使用 syscall(SYS_gettid) 获取内核线程 ID，始终为正整数
		// pthread_self() 返回 pthread_t 是 unsigned long，强转 int 可能变负
		return (int)syscall(SYS_gettid);
#endif
	}
	// 获得程序当前所在路径,带文件名
	inline string getProgramFile()
	{
		MyCharArray<256> name;
#ifdef WINDOWS
		GetModuleFileNameA(nullptr, name.toBuffer(), name.size());
#elif defined LINUX
		const ssize_t ret = readlink("/proc/self/exe", name.toBuffer(), name.size());
		if (ret == -1)
		{
			return "";
		}
#endif
		return name.str();
	}
	// 参数为绝对路径,并且在windows上需要将所有的'/'转换为'\\'
	// linux上只需要填写fullName
	MICRO_LEGEND_FRAME_API bool launchExe(const string& path, const string& fullName);
	// Windows参数为文件名,不带路径,不带后缀
	// linux为绝对路径
	MICRO_LEGEND_FRAME_API bool isExeRunning(const string& fileName);
#ifdef WINDOWS
	MICRO_LEGEND_FRAME_API void getAllProcess(Set<string>& processList);
#elif defined LINUX
	MICRO_LEGEND_FRAME_API void getPidFromStr(const char* str, char* pid, int size);
	MICRO_LEGEND_FRAME_API string getExePath(const string& pid);
	MICRO_LEGEND_FRAME_API void memoryUsage(llong& vmUsageKB, llong& residentSetSizeKB);
#endif
	// fillPath为绝对路径,带文件名
	MICRO_LEGEND_FRAME_API void launchExeInScreen(const string& screenName, const string& fullPath);
	MICRO_LEGEND_FRAME_API int getPID();
	inline bool isMainThread() { return getThreadID() == mMainThread; }
	inline void setMainThread(const int id) { mMainThread = id; }
	inline void checkMainThread()
	{
#ifdef WINDOWS
		if (!isMainThread())
		{
			ERROR("只能在主线程调用");
		}
#endif
	}
	inline llong makeIDMain()
	{
		checkMainThread();
		return ++mIDSeedMain;
	}
	inline llong makeIDThread() { return ++mIDSeedThread; }
	MICRO_LEGEND_FRAME_API string getStackTrace(int depth);
}

using SystemUtility::getTimeSecondUTC;
using SystemUtility::getTimeMSecondUTC;
using SystemUtility::getRealTimeMS;
using SystemUtility::checkMainThread;
using SystemUtility::isMainThread;
using SystemUtility::getThreadID;
using SystemUtility::getTime;
using SystemUtility::getYear;
using SystemUtility::getMonth;
using SystemUtility::getYearMonth;
using SystemUtility::getDay;
using SystemUtility::makeIDMain;
using SystemUtility::getDayOfWeek;
using SystemUtility::print;
using SystemUtility::getLocalIP;
using SystemUtility::getStackTrace;
using SystemUtility::isSameDay;
using SystemUtility::getTimeHourInDay;
using SystemUtility::makeSockAddr;
using SystemUtility::getPID;
using SystemUtility::setMainThread;
using SystemUtility::setTimeMSecondUTC;
using SystemUtility::setTimeSecondUTC;
using SystemUtility::makeIDThread;
using SystemUtility::systemPause;
using SystemUtility::getTimeStruct;
using SystemUtility::convertToTimestamp;
using SystemUtility::daysToSeconds;
using SystemUtility::getTodayEnd;
using SystemUtility::getTodayBegin;
using SystemUtility::getDaysInMonth;
using SystemUtility::isLeapYear;
using SystemUtility::getCPUCoreCount;
using SystemUtility::getProgramFile;