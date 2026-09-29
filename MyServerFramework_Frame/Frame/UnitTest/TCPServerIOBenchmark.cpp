#ifdef WINDOWS
#include "FrameHeader.h"
#include "TCPServerIOBenchmark.h"

namespace TCPServerIOBenchmarkLocal
{
#ifdef LINUX
	static constexpr int CLOCK_ROUNDS = 5;
	static constexpr int CLOCK_COUNT = 1000000;
	static constexpr int IO_ROUNDS = 5;
	static constexpr int IO_ITERATIONS = 2000;
	volatile int gTCPIOBenchmarkSink = 0;
	struct SocketPairSet
	{
		vector<int> mServerFDList;
		vector<int> mPeerFDList;
		~SocketPairSet()
		{
			for (int fd : mServerFDList) { close(fd); }
			for (int fd : mPeerFDList) { close(fd); }
		}
	};
#endif
}

#ifdef LINUX
llong TCPServerIOBenchmark::nowNS()
{
	using namespace TCPServerIOBenchmarkLocal;
		return (llong)std::chrono::duration_cast<std::chrono::nanoseconds>(steady_clock::now().time_since_epoch()).count();
	}
#endif

#ifdef LINUX
double TCPServerIOBenchmark::median(double* samples, const int count)
{
	using namespace TCPServerIOBenchmarkLocal;
		std::sort(samples, samples + count);
		return samples[count / 2];
	}
#endif

#ifdef LINUX
bool TCPServerIOBenchmark::setNonBlocking(const int fd)
{
	using namespace TCPServerIOBenchmarkLocal;
		const int flags = fcntl(fd, F_GETFL, 0);
		return flags >= 0 && fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0;
	}
#endif

#ifdef LINUX
bool TCPServerIOBenchmark::createSocketPairs(TCPServerIOBenchmarkLocal::SocketPairSet& socketSet, const int count)
{
	using namespace TCPServerIOBenchmarkLocal;
		socketSet.mServerFDList.reserve(count);
		socketSet.mPeerFDList.reserve(count);
		for (int i = 0; i < count; ++i)
		{
			int pairFD[2]{ -1, -1 };
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, pairFD) != 0)
			{
				LOG("V53 socketpair failed, Requested:" + IToS(count) + ", Created:" + IToS((int)socketSet.mServerFDList.size()) + ", errno:" + IToS(errno));
				return false;
			}
			if (!setNonBlocking(pairFD[0]) || !setNonBlocking(pairFD[1]))
			{
				close(pairFD[0]);
				close(pairFD[1]);
				LOG("V53 setNonBlocking failed, errno:" + IToS(errno));
				return false;
			}
			socketSet.mServerFDList.push_back(pairFD[0]);
			socketSet.mPeerFDList.push_back(pairFD[1]);
		}
		return true;
	}
#endif

#ifdef LINUX
void TCPServerIOBenchmark::benchmarkClockRead()
{
	using namespace TCPServerIOBenchmarkLocal;
		double samples[CLOCK_ROUNDS];
		for (int round = 0; round < CLOCK_ROUNDS; ++round)
		{
			const llong start = nowNS();
			for (int i = 0; i < CLOCK_COUNT; ++i) { gTCPIOBenchmarkSink += (int)(nowNS() & 1); }
			const llong end = nowNS();
			samples[round] = (double)(end - start) / CLOCK_COUNT;
		}
		double sorted[CLOCK_ROUNDS];
		memcpy(sorted, samples, sizeof(samples));
		const double med = median(sorted, CLOCK_ROUNDS);
		double minValue = samples[0];
		double maxValue = samples[0];
		for (int i = 1; i < CLOCK_ROUNDS; ++i)
		{
			minValue = getMin(minValue, samples[i]);
			maxValue = getMax(maxValue, samples[i]);
		}
		const double spread = med > 0.0 ? (maxValue - minValue) * 100.0 / med : 0.0;
		LOG("ClockRead CountPerRound:" + IToS(CLOCK_COUNT) + ", Rounds:" + IToS(CLOCK_ROUNDS) + ", Median(ns/read):" + FToS((float)med, 2) + ", Min:" + FToS((float)minValue, 2) + ", Max:" + FToS((float)maxValue, 2) + ", Spread:" + FToS((float)spread, 2) + "%");
	}
#endif

#ifdef LINUX
bool TCPServerIOBenchmark::verifyNonBlockingBackpressure()
{
	using namespace TCPServerIOBenchmarkLocal;
		int pairFD[2]{ -1, -1 };
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, pairFD) != 0) { return false; }
		const bool nonBlocking = setNonBlocking(pairFD[0]) && setNonBlocking(pairFD[1]);
		int sendBufferSize = 4096;
		setsockopt(pairFD[0], SOL_SOCKET, SO_SNDBUF, &sendBufferSize, sizeof(sendBufferSize));
		char buffer[4096]{};
		bool reachedEAGAIN = false;
		if (nonBlocking)
		{
			for (int i = 0; i < 4096; ++i)
			{
				const int ret = send(pairFD[0], buffer, sizeof(buffer), MSG_NOSIGNAL);
				if (ret > 0) { continue; }
				if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) { reachedEAGAIN = true; break; }
				if (ret < 0 && errno == EINTR) { --i; continue; }
				break;
			}
		}
		while (recv(pairFD[1], buffer, sizeof(buffer), 0) > 0) {}
		const char one = 1;
		const bool sendAfterDrain = send(pairFD[0], &one, 1, MSG_NOSIGNAL) == 1;
		close(pairFD[0]);
		close(pairFD[1]);
		return nonBlocking && reachedEAGAIN && sendAfterDrain;
	}
#endif

#ifdef LINUX
bool TCPServerIOBenchmark::verifyLevelTriggerUnreadPersistence()
{
	using namespace TCPServerIOBenchmarkLocal;
		int pairFD[2]{ -1, -1 };
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, pairFD) != 0) { return false; }
		if (!setNonBlocking(pairFD[0]) || !setNonBlocking(pairFD[1]))
		{
			close(pairFD[0]);
			close(pairFD[1]);
			return false;
		}
		const int epollFD = epoll_create1(EPOLL_CLOEXEC);
		if (epollFD < 0)
		{
			close(pairFD[0]);
			close(pairFD[1]);
			return false;
		}
		epoll_event event{};
		event.events = EPOLLIN | EPOLLRDHUP;
		event.data.fd = pairFD[0];
		bool result = epoll_ctl(epollFD, EPOLL_CTL_ADD, pairFD[0], &event) == 0;
		char data[64]{};
		if (result) { result = send(pairFD[1], data, sizeof(data), MSG_NOSIGNAL) == (int)sizeof(data); }
		epoll_event readyEvent{};
		if (result) { result = epoll_wait(epollFD, &readyEvent, 1, 0) == 1; }
		char one = 0;
		if (result) { result = recv(pairFD[0], &one, 1, 0) == 1; }
		if (result) { result = epoll_wait(epollFD, &readyEvent, 1, 0) == 1; }
		while (recv(pairFD[0], data, sizeof(data), 0) > 0) {}
		if (result) { result = epoll_wait(epollFD, &readyEvent, 1, 0) == 0; }
		if (result) { result = epoll_ctl(epollFD, EPOLL_CTL_DEL, pairFD[0], nullptr) == 0; }
		close(epollFD);
		close(pairFD[0]);
		close(pairFD[1]);
		return result;
	}
#endif

#ifdef LINUX
bool TCPServerIOBenchmark::prepareReadableData(const TCPServerIOBenchmarkLocal::SocketPairSet& socketSet, const int readyCount)
{
	using namespace TCPServerIOBenchmarkLocal;
		const char value = 1;
		for (int i = 0; i < readyCount; ++i)
		{
			if (send(socketSet.mPeerFDList[i], &value, 1, MSG_NOSIGNAL) != 1) { return false; }
		}
		return true;
	}
#endif

#ifdef LINUX
int TCPServerIOBenchmark::createRecvEpoll(const TCPServerIOBenchmarkLocal::SocketPairSet& socketSet)
{
	using namespace TCPServerIOBenchmarkLocal;
		const int epollFD = epoll_create1(EPOLL_CLOEXEC);
		if (epollFD < 0) { return -1; }
		for (int fd : socketSet.mServerFDList)
		{
			epoll_event event{};
			event.events = EPOLLIN | EPOLLRDHUP;
			event.data.fd = fd;
			if (epoll_ctl(epollFD, EPOLL_CTL_ADD, fd, &event) != 0)
			{
				close(epollFD);
				return -1;
			}
		}
		return epollFD;
	}
#endif

#ifdef LINUX
double TCPServerIOBenchmark::benchmarkProductionEpollWait(const int clientCount, const int readyCount, bool& result)
{
	using namespace TCPServerIOBenchmarkLocal;
		result = false;
		TCPServerIOBenchmarkLocal::SocketPairSet socketSet;
		if (!createSocketPairs(socketSet, clientCount)) { return -1.0; }
		const int epollFD = createRecvEpoll(socketSet);
		if (epollFD < 0 || !prepareReadableData(socketSet, readyCount))
		{
			if (epollFD >= 0) { close(epollFD); }
			return -1.0;
		}
		vector<epoll_event> eventList(clientCount);
		double samples[IO_ROUNDS];
		int observedReady = 0;
		for (int round = 0; round < IO_ROUNDS; ++round)
		{
			llong readySum = 0;
			const llong start = nowNS();
			for (int iteration = 0; iteration < IO_ITERATIONS; ++iteration)
			{
				const int ready = epoll_wait(epollFD, eventList.data(), clientCount, 0);
				if (ready < 0)
				{
					close(epollFD);
					return -1.0;
				}
				readySum += ready;
				for (int i = 0; i < ready; ++i) { gTCPIOBenchmarkSink += eventList[i].data.fd & 1; }
			}
			const llong end = nowNS();
			observedReady = (int)(readySum / IO_ITERATIONS);
			samples[round] = (double)(end - start) / IO_ITERATIONS;
		}
		char buffer[64];
		for (int i = 0; i < readyCount; ++i) { while (recv(socketSet.mServerFDList[i], buffer, sizeof(buffer), 0) > 0) {} }
		const int afterDrain = epoll_wait(epollFD, eventList.data(), clientCount, 0);
		close(epollFD);
		result = observedReady == readyCount && afterDrain == 0;
		return median(samples, IO_ROUNDS);
	}
#endif

#ifdef LINUX
void TCPServerIOBenchmark::runLinuxEpollProductionVerification()
{
	using namespace TCPServerIOBenchmarkLocal;
		LOG("================ TCPServer IO Benchmark V53 Linux epoll Production Verification Start ================");
		LOG("V53 production: Linux accepted TCP sockets are non-blocking; RecvThread uses level-trigger EPOLLIN and keeps the original one-recv-per-ready-client pass; SendThread directly non-blocking sends and retries EAGAIN on the next 10ms pass. Windows remains the original select implementation.");
		rlimit fileLimit{};
		if (getrlimit(RLIMIT_NOFILE, &fileLimit) == 0)
		{
			LOG("RLIMIT_NOFILE Soft:" + ULLToS((ullong)fileLimit.rlim_cur) + ", Hard:" + ULLToS((ullong)fileLimit.rlim_max));
		}
		benchmarkClockRead();
		LOG("LevelTrigger unread-data + epoll ADD/DEL lifecycle:" + string(verifyLevelTriggerUnreadPersistence() ? "PASS" : "FAIL"));
		LOG("NonBlocking send EAGAIN/backpressure semantics:" + string(verifyNonBlockingBackpressure() ? "PASS" : "FAIL"));
		const int clientCountList[]{ 1000, 5000 };
		for (int clientCount : clientCountList)
		{
			const int readyCountList[]{ 0, 1, getMax(1, clientCount / 100) };
			for (int readyCount : readyCountList)
			{
				bool result = false;
				const double medianNS = benchmarkProductionEpollWait(clientCount, readyCount, result);
				if (medianNS < 0.0)
				{
					LOG("EpollProduction ClientCount:" + IToS(clientCount) + ", Ready:" + IToS(readyCount) + ", SKIP: socket/epoll setup failed under current RLIMIT_NOFILE.");
					break;
				}
				LOG("EpollProduction ClientCount:" + IToS(clientCount) + ", Ready:" + IToS(readyCount) + ", Median(ns/pass):" + FToS((float)medianNS, 2) + ", ReadyCount/DrainReset:" + (result ? "PASS" : "FAIL"));
			}
		}
		LOG("V52 historical select/epoll A/B is retired after production landing. No EPOLLET, EPOLLONESHOT, EPOLLOUT state machine, worker pool or lock-free queue is used.");
		LOG("================ TCPServer IO Benchmark V53 Linux epoll Production Verification End ================");
	}
#endif

void TCPServerIOBenchmark::test()
{
#ifdef LINUX
	runLinuxEpollProductionVerification();
#endif
}
#endif
