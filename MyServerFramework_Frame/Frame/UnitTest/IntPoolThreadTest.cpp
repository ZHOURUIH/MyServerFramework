#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// IntPoolThreadTest
// Testing IntPoolThread (a specialized PodPoolThread<int>)
// IntPoolThread inherits PodPoolThread<int> with no additions
// API (inherited): initDefault, newPod, destroyPod, clearPool, quit
// ============================================================

#define IPT_ASSERT(expr) \
	if (!(expr)) { ERROR(string("IntPoolThreadTest FAIL: ") + #expr); }

void IntPoolThreadTest::test()
{
	testNewIntNotNull();
	testDestroyIntNullAfter();
	testDestroyNullSafe();
	testReuseAfterDestroy();
	testInitDefault();
	testInitDefaultExhaust();
	testClearPool();
	testWriteRead();
	testWriteReadMax();
	testWriteReadMin();
	testBatchAllocFree();
	testManyRounds();
	testPointerUniqueness();
	testLargePool();
	testInitDefaultMultipleCalls();
	testClearAfterInit();
	testAlternateAllocFree();
	testValuePersistBetweenAllocs();
	testZeroValueWrite();
	testNegativeValueWrite();
}

void IntPoolThreadTest::testNewIntNotNull()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testDestroyIntNullAfter()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	IPT_ASSERT(p == nullptr);
}

void IntPoolThreadTest::testDestroyNullSafe()
{
	IntPoolThread pool;
	int* p = nullptr;
	pool.destroyPod(p);
	IPT_ASSERT(p == nullptr);
}

void IntPoolThreadTest::testReuseAfterDestroy()
{
	IntPoolThread pool;
	int* first = pool.newPod();
	void* firstAddr = first;
	*first = 12345;
	pool.destroyPod(first);
	IPT_ASSERT(first == nullptr);
	int* second = pool.newPod();
	IPT_ASSERT(second != nullptr);
	IPT_ASSERT(second == firstAddr);
	pool.destroyPod(second);
}

void IntPoolThreadTest::testInitDefault()
{
	IntPoolThread pool;
	pool.initDefault(8);
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testInitDefaultExhaust()
{
	IntPoolThread pool;
	pool.initDefault(3);
	int* a = pool.newPod();
	int* b = pool.newPod();
	int* c = pool.newPod();
	// Exhaust initial pool
	int* d = pool.newPod(); // allocates new
	IPT_ASSERT(a != nullptr);
	IPT_ASSERT(b != nullptr);
	IPT_ASSERT(c != nullptr);
	IPT_ASSERT(d != nullptr);
	pool.destroyPod(a);
	pool.destroyPod(b);
	pool.destroyPod(c);
	pool.destroyPod(d);
}

void IntPoolThreadTest::testClearPool()
{
	IntPoolThread pool;
	pool.initDefault(5);
	pool.clearPool();
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testWriteRead()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	*p = 42;
	IPT_ASSERT(*p == 42);
	*p = 1000;
	IPT_ASSERT(*p == 1000);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testWriteReadMax()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	*p = 2147483647;
	IPT_ASSERT(*p == 2147483647);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testWriteReadMin()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	*p = -2147483647;
	IPT_ASSERT(*p == -2147483647);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testBatchAllocFree()
{
	IntPoolThread pool;
	const int BATCH = 20;
	int* arr[BATCH];
	for (int i = 0; i < BATCH; ++i)
	{
		arr[i] = pool.newPod();
		IPT_ASSERT(arr[i] != nullptr);
		*arr[i] = i * 3;
	}
	for (int i = 0; i < BATCH; ++i)
	{
		IPT_ASSERT(*arr[i] == i * 3);
	}
	for (int i = 0; i < BATCH; ++i)
	{
		pool.destroyPod(arr[i]);
		IPT_ASSERT(arr[i] == nullptr);
	}
}

void IntPoolThreadTest::testManyRounds()
{
	IntPoolThread pool;
	const int ROUNDS = 30;
	const int PER_ROUND = 4;
	for (int r = 0; r < ROUNDS; ++r)
	{
		int* arr[PER_ROUND];
		for (int i = 0; i < PER_ROUND; ++i)
		{
			arr[i] = pool.newPod();
			IPT_ASSERT(arr[i] != nullptr);
			*arr[i] = r * 100 + i;
		}
		for (int i = 0; i < PER_ROUND; ++i)
		{
			IPT_ASSERT(*arr[i] == r * 100 + i);
			pool.destroyPod(arr[i]);
		}
	}
}

void IntPoolThreadTest::testPointerUniqueness()
{
	IntPoolThread pool;
	const int N = 15;
	int* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		IPT_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
		for (int j = i + 1; j < N; ++j)
		{
			IPT_ASSERT(arr[i] != arr[j]);
		}
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyPod(arr[i]);
	}
}

void IntPoolThreadTest::testLargePool()
{
	IntPoolThread pool;
	const int LARGE = 100;
	int* arr[LARGE];
	for (int i = 0; i < LARGE; ++i)
	{
		arr[i] = pool.newPod();
		IPT_ASSERT(arr[i] != nullptr);
		*arr[i] = i * 7;
	}
	for (int i = 0; i < LARGE; ++i)
	{
		IPT_ASSERT(*arr[i] == i * 7);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	// Reallocate and check reuse
	for (int i = 0; i < LARGE; ++i)
	{
		arr[i] = pool.newPod();
		IPT_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		pool.destroyPod(arr[i]);
	}
}

void IntPoolThreadTest::testInitDefaultMultipleCalls()
{
	IntPoolThread pool;
	pool.initDefault(4);
	pool.initDefault(4);
	pool.initDefault(2);
	// 10 items pre-cached
	int* arr[12];
	for (int i = 0; i < 12; ++i)
	{
		arr[i] = pool.newPod();
		IPT_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < 12; ++i)
	{
		pool.destroyPod(arr[i]);
	}
}

void IntPoolThreadTest::testClearAfterInit()
{
	IntPoolThread pool;
	pool.initDefault(10);
	pool.clearPool();
	// Should still work after clear
	for (int i = 0; i < 5; ++i)
	{
		int* p = pool.newPod();
		IPT_ASSERT(p != nullptr);
		*p = i;
		IPT_ASSERT(*p == i);
		pool.destroyPod(p);
	}
}

void IntPoolThreadTest::testAlternateAllocFree()
{
	IntPoolThread pool;
	for (int i = 0; i < 25; ++i)
	{
		int* p = pool.newPod();
		IPT_ASSERT(p != nullptr);
		*p = i * 11;
		IPT_ASSERT(*p == i * 11);
		pool.destroyPod(p);
		IPT_ASSERT(p == nullptr);
	}
}

void IntPoolThreadTest::testValuePersistBetweenAllocs()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	*p = 9876;
	pool.destroyPod(p);
	// Get the same object back (reuse)
	int* q = pool.newPod();
	IPT_ASSERT(q != nullptr);
	// Value is 9876 (not reset by POD pool)
	// Just verify we can read/write
	*q = 1111;
	IPT_ASSERT(*q == 1111);
	pool.destroyPod(q);
}

void IntPoolThreadTest::testZeroValueWrite()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	*p = 0;
	IPT_ASSERT(*p == 0);
	pool.destroyPod(p);
}

void IntPoolThreadTest::testNegativeValueWrite()
{
	IntPoolThread pool;
	int* p = pool.newPod();
	IPT_ASSERT(p != nullptr);
	*p = -99999;
	IPT_ASSERT(*p == -99999);
	*p = -1;
	IPT_ASSERT(*p == -1);
	pool.destroyPod(p);
}
#endif
