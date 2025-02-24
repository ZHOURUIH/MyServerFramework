#pragma once

#include "MathUtility.h"
#include "ThreadLock.h"

namespace SystemUtility
{
	MICRO_LEGEND_FRAME_API extern llong mIDSeedMain;			// 用于生成唯一ID的种子,仅用于主线程
	MICRO_LEGEND_FRAME_API extern atomic<llong> mIDSeedThread;	// 用于生成唯一ID的种子,用于多线程
	MICRO_LEGEND_FRAME_API extern llong mTimeMS;				// 系统从启动到现在所经过的毫秒,每帧获取一次,避免频繁获取造成性能下降
	MICRO_LEGEND_FRAME_API extern llong mTimeS;					// 从1970年1月1日到现在的秒数,每帧获取一次,避免频繁获取造成性能下降
	MICRO_LEGEND_FRAME_API extern int mMainThread;				// 主线程的线程ID
	//------------------------------------------------------------------------------------------------------------------------------
	MICRO_LEGEND_FRAME_API void stop();
	void sleep(ulong timeMS)
	{
#ifdef WINDOWS
		Sleep(timeMS);
#elif defined LINUX
		usleep(timeMS * 1000);
#endif
	}
	// 获取系统从启动到现在所经过的毫秒,每帧更新一次的
	llong getTimeMS() { return mTimeMS; }
	void setTimeMS(const llong timeMS) { mTimeMS = timeMS; }
	// 获取系统从启动到现在所经过的毫秒,实时的
	llong getRealTimeMS()
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
	// 获取从1970年1月1日到现在的秒数,本地时间,非UTC时间
	llong getTimeSecond() { return mTimeS; }
	void setTimeSecond(llong timeS) { mTimeS = timeS; }
	// 获得今天是周几,0表示周日,1表示周一
	MICRO_LEGEND_FRAME_API int getDayOfWeek();
	// 判断两次时间戳是否是在同一天
	MICRO_LEGEND_FRAME_API bool isSameDay(const time_t& timeStamp0, const time_t& timeStamp1);
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
	int getThreadID()
	{
#ifdef WINDOWS
		return (int)GetCurrentThreadId();
#elif defined LINUX
		return (int)pthread_self();
#endif
	}
	// 获得程序当前所在路径,带文件名
	string getProgramFile()
	{
		MyString<256> name;
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
	bool isMainThread() { return getThreadID() == mMainThread; }
	void setMainThread(const int id) { mMainThread = id; }
	void checkMainThread()
	{
#ifdef WINDOWS
		if (!isMainThread())
		{
			ERROR("只能在主线程调用");
		}
#endif
	}
	llong makeIDMain()
	{
		checkMainThread();
		return ++mIDSeedMain;
	}
	llong makeIDThread() { return ++mIDSeedThread; }
	MICRO_LEGEND_FRAME_API string getStackTrace(int depth);
}

using SystemUtility::getTimeSecond;
using SystemUtility::getTimeMS;
using SystemUtility::getRealTimeMS;
using SystemUtility::checkMainThread;
using SystemUtility::isMainThread;
using SystemUtility::getThreadID;
using SystemUtility::getTime;
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
using SystemUtility::setTimeMS;
using SystemUtility::setTimeSecond;
using SystemUtility::makeIDThread;
using SystemUtility::sleep;