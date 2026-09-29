#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

namespace TCPServerIOBenchmarkLocal 
{
	struct SocketPairSet; 
}

class MICRO_LEGEND_FRAME_API TCPServerIOBenchmark
{
#ifdef WINDOWS
public:
	static void test();
private:
#ifdef LINUX
	static llong nowNS();
	static double median(double* samples, const int count);
	static bool setNonBlocking(const int fd);
	static bool createSocketPairs(TCPServerIOBenchmarkLocal::SocketPairSet& socketSet, const int count);
	static void benchmarkClockRead();
	static bool verifyNonBlockingBackpressure();
	static bool verifyLevelTriggerUnreadPersistence();
	static bool prepareReadableData(const TCPServerIOBenchmarkLocal::SocketPairSet& socketSet, const int readyCount);
	static int createRecvEpoll(const TCPServerIOBenchmarkLocal::SocketPairSet& socketSet);
	static double benchmarkProductionEpollWait(const int clientCount, const int readyCount, bool& result);
	static void runLinuxEpollProductionVerification();
#endif
#endif
};
