#ifdef WINDOWS
#include "FrameHeader.h"

namespace
{
	//static void assertMemoryTracker(bool condition, const char* message)
	//{
	//	if (!condition)
	//	{
	//		ERROR(string("MemoryTrackerTest failed: ") + message);
	//	}
	//}

	//static void testSingletonAndThreadId()
	//{
	//	assertMemoryTracker(MemoryTracker::Get() == nullptr, "initial nullptr");
	//	MemoryTracker::NewInstance();
	//	assertMemoryTracker(MemoryTracker::Get() != nullptr, "instance created");
	//	assertMemoryTracker(MemoryTracker::Get()->IsMainThread(), "main thread");
	//	assertMemoryTracker(MemoryTracker::getThreadID() != 0, "thread id valid");
	//}

	//static void testHashFunction()
	//{
	//	MemoryTracker::NewInstance();
	//	MemoryTracker* tracker = MemoryTracker::Get();
	//	assertMemoryTracker(tracker != nullptr, "tracker exists");

	//	int a = 1;
	//	int b = 2;
	//	int c = 3;

	//	const size_t hashA = tracker->Hash(&a);
	//	const size_t hashB = tracker->Hash(&b);
	//	const size_t hashC = tracker->Hash(&c);

	//	assertMemoryTracker(hashA < (1u << 22), "hash a range");
	//	assertMemoryTracker(hashB < (1u << 22), "hash b range");
	//	assertMemoryTracker(hashC < (1u << 22), "hash c range");
	//	assertMemoryTracker(tracker->Hash(&a) == hashA, "hash stable a");
	//	assertMemoryTracker(tracker->Hash(&b) == hashB, "hash stable b");
	//	assertMemoryTracker(tracker->Hash(&c) == hashC, "hash stable c");
	//}

	//static void testRegisterAllocFreeNoCrash()
	//{
	//	MemoryTracker::NewInstance();
	//	MemoryTracker* tracker = MemoryTracker::Get();
	//	assertMemoryTracker(tracker != nullptr, "tracker exists for alloc");

	//	void* p1 = malloc(16);
	//	void* p2 = malloc(24);
	//	void* p3 = nullptr;

	//	tracker->RegisterAlloc(p1, 16, "MemoryTrackerTest.cpp", 10);
	//	tracker->RegisterAlloc(p2, 24, "MemoryTrackerTest.cpp", 11);
	//	tracker->RegisterAlloc(p3, 32, "MemoryTrackerTest.cpp", 12);
	//	tracker->RegisterFree(p1);
	//	tracker->RegisterFree(p2);
	//	tracker->RegisterFree(p3);
	//	tracker->RegisterFree(reinterpret_cast<void*>(0x1234));

	//	free(p1);
	//	free(p2);

	//	assertMemoryTracker(true, "register alloc/free completed");
	//}
}

void MemoryTrackerTest::test()
{
	//testSingletonAndThreadId();
	//testHashFunction();
	//testRegisterAllocFreeNoCrash();
}
#endif
