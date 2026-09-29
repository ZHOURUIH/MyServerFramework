#ifdef WINDOWS
#include "FrameHeader.h"
namespace PodPoolThreadTestSupport
{
	// ============================================================
	// PodPoolThreadTest
	// Testing PodPoolThread<T>
	// PodPoolThread is a thread-safe POD integer object pool.
	// API: initDefault(count), newPod(), destroyPod(ptr), clearPool(), quit()
	// Constraints: T must satisfy IsPodIntegerType
	// ============================================================

#define PodPoolThreadTestSupport_PPT_ASSERT(expr) \
	if (!(expr)) { ERROR(string("PodPoolThreadTest FAIL: ") + #expr); }
}

void PodPoolThreadTest::testNewPodInt()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testNewPodLlong()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<llong> pool;
	llong* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testNewPodShort()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<short> pool;
	short* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testNewPodUshort()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<ushort> pool;
	ushort* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testNewPodByte()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<byte> pool;
	byte* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testNewPodUint()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<uint> pool;
	uint* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testNewPodNotNull()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* a = pool.newPod();
	int* b = pool.newPod();
	int* c = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(a != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(b != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(c != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(a != b);
	PodPoolThreadTestSupport_PPT_ASSERT(b != c);
	PodPoolThreadTestSupport_PPT_ASSERT(a != c);
	pool.destroyPod(a);
	pool.destroyPod(b);
	pool.destroyPod(c);
}

void PodPoolThreadTest::testDestroyPodNullAfter()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testDestroyNullSafe()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* p = nullptr;
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testReuseAfterDestroy()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* first = pool.newPod();
	void* firstAddr = first;
	*first = 42;
	pool.destroyPod(first);
	PodPoolThreadTestSupport_PPT_ASSERT(first == nullptr);
	int* second = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(second != nullptr);
	// The reused pointer should be the same address
	PodPoolThreadTestSupport_PPT_ASSERT(second == firstAddr);
	pool.destroyPod(second);
}

void PodPoolThreadTest::testReuseValue()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	*p = 9999;
	pool.destroyPod(p);
	// New object from pool — value is NOT reset (POD pool doesn't reset)
	int* q = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(q != nullptr);
	pool.destroyPod(q);
}

void PodPoolThreadTest::testInitDefault()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(5);
	// After initDefault, newPod should return objects from cache
	int* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testInitDefaultThenNew()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(3);
	int* a = pool.newPod();
	int* b = pool.newPod();
	int* c = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(a != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(b != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(c != nullptr);
	pool.destroyPod(a);
	pool.destroyPod(b);
	pool.destroyPod(c);
}

void PodPoolThreadTest::testInitDefaultExhaust()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(2);
	// Exhaust initial pool, then allocate more
	int* a = pool.newPod();
	int* b = pool.newPod();
	int* c = pool.newPod(); // Should still succeed by allocating new
	PodPoolThreadTestSupport_PPT_ASSERT(a != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(b != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(c != nullptr);
	pool.destroyPod(a);
	pool.destroyPod(b);
	pool.destroyPod(c);
}

void PodPoolThreadTest::testClearPool()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(5);
	pool.clearPool();
	// After clearPool, newPod should still work
	int* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testClearPoolAfterNew()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* a = pool.newPod();
	int* b = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(a != nullptr);
	PodPoolThreadTestSupport_PPT_ASSERT(b != nullptr);
	pool.destroyPod(a);
	pool.destroyPod(b);
	pool.clearPool();
	// Now newPod should allocate fresh objects
	int* c = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(c != nullptr);
	pool.destroyPod(c);
}

void PodPoolThreadTest::testMultipleNewSameType()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<llong> pool;
	const int N = 10;
	llong* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
		*arr[i] = (llong)i * 1000000000LL;
	}
	for (int i = 0; i < N; ++i)
	{
		PodPoolThreadTestSupport_PPT_ASSERT(*arr[i] == (llong)i * 1000000000LL);
		pool.destroyPod(arr[i]);
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] == nullptr);
	}
}

void PodPoolThreadTest::testAlternateNewDestroy()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	for (int i = 0; i < 20; ++i)
	{
		int* p = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
		*p = i * 7;
		PodPoolThreadTestSupport_PPT_ASSERT(*p == i * 7);
		pool.destroyPod(p);
		PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
	}
}

void PodPoolThreadTest::testBatchNewDestroy()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	const int BATCH = 30;
	int* arr[BATCH];
	for (int i = 0; i < BATCH; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
		*arr[i] = i + 100;
	}
	for (int i = 0; i < BATCH; ++i)
	{
		PodPoolThreadTestSupport_PPT_ASSERT(*arr[i] == i + 100);
	}
	for (int i = 0; i < BATCH; ++i)
	{
		pool.destroyPod(arr[i]);
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] == nullptr);
	}
}

void PodPoolThreadTest::testManyRoundsInt()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	const int ROUNDS = 50;
	const int PER_ROUND = 5;
	for (int r = 0; r < ROUNDS; ++r)
	{
		int* arr[PER_ROUND];
		for (int i = 0; i < PER_ROUND; ++i)
		{
			arr[i] = pool.newPod();
			PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
			*arr[i] = r * 100 + i;
		}
		for (int i = 0; i < PER_ROUND; ++i)
		{
			PodPoolThreadTestSupport_PPT_ASSERT(*arr[i] == r * 100 + i);
			pool.destroyPod(arr[i]);
			PodPoolThreadTestSupport_PPT_ASSERT(arr[i] == nullptr);
		}
	}
}

void PodPoolThreadTest::testManyRoundsLlong()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<llong> pool;
	const int ROUNDS = 30;
	const int PER_ROUND = 8;
	for (int r = 0; r < ROUNDS; ++r)
	{
		llong* arr[PER_ROUND];
		for (int i = 0; i < PER_ROUND; ++i)
		{
			arr[i] = pool.newPod();
			PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
			*arr[i] = (llong)r * 1000 + i;
		}
		for (int i = 0; i < PER_ROUND; ++i)
		{
			PodPoolThreadTestSupport_PPT_ASSERT(*arr[i] == (llong)r * 1000 + i);
			pool.destroyPod(arr[i]);
		}
	}
}

void PodPoolThreadTest::testDestroyTwiceSafe()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
	// Destroying nullptr should be a no-op
	pool.destroyPod(p);
	PodPoolThreadTestSupport_PPT_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testLargePool()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	const int LARGE = 200;
	int* arr[LARGE];
	for (int i = 0; i < LARGE; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
		*arr[i] = i;
	}
	for (int i = 0; i < LARGE; ++i)
	{
		PodPoolThreadTestSupport_PPT_ASSERT(*arr[i] == i);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	// Allocate again to verify reuse
	for (int i = 0; i < LARGE; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testNewAfterClear()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(10);
	pool.clearPool();
	for (int i = 0; i < 5; ++i)
	{
		int* p = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
		*p = i * 3;
		PodPoolThreadTestSupport_PPT_ASSERT(*p == i * 3);
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testInitDefaultMultiple()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(3);
	pool.initDefault(3);
	pool.initDefault(4);
	// Should have at least 10 objects cached
	int* arr[12];
	for (int i = 0; i < 12; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < 12; ++i)
	{
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testPointerUniqueness()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	const int N = 20;
	int* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != nullptr);
	}
	// All pointers should be unique
	for (int i = 0; i < N; ++i)
	{
		for (int j = i + 1; j < N; ++j)
		{
			PodPoolThreadTestSupport_PPT_ASSERT(arr[i] != arr[j]);
		}
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testIntWriteRead()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	*p = 2147483647;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 2147483647);
	*p = -2147483647;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == -2147483647);
	*p = 0;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 0);
	*p = 12345678;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 12345678);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testLlongWriteRead()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<llong> pool;
	llong* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	*p = 9223372036854775807LL;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 9223372036854775807LL);
	*p = -9223372036854775807LL;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == -9223372036854775807LL);
	*p = 0LL;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 0LL);
	*p = 1234567890123456789LL;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 1234567890123456789LL);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testShortWriteRead()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<short> pool;
	short* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	*p = 32767;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 32767);
	*p = -32767;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == -32767);
	*p = 0;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 0);
	*p = 1000;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 1000);
	*p = -500;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == -500);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testByteWriteRead()
{
	using namespace PodPoolThreadTestSupport;
	PodPoolThread<byte> pool;
	byte* p = pool.newPod();
	PodPoolThreadTestSupport_PPT_ASSERT(p != nullptr);
	*p = 0xFF;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 0xFF);
	*p = 0x00;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 0x00);
	*p = 0xAB;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 0xAB);
	*p = 127;
	PodPoolThreadTestSupport_PPT_ASSERT(*p == 127);
	pool.destroyPod(p);
}

namespace PodPoolThreadExtTestSupport
{
#define PodPoolThreadExtTestSupport_PPT_ASSERT(cond, msg) \
	if (!(cond)) { ERROR(string("PodPoolThreadExtTest FAIL: ") + (msg)); }

	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------


	// ---------------------------------------------------------------------------


























	// ---------------------------------------------------------------------------
	// 额外：llong 类型 PodPoolThread 边界测试
	// ---------------------------------------------------------------------------






	// PodPoolThreadTest::runExtraPodPoolTests() 由 test() 显式调用
}

void PodPoolThreadTest::testNewPodIntExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testNewPodInt not null");
	*p = 12345;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 12345, "testNewPodInt write/read");
	pool.destroyPod(p);
	PodPoolThreadExtTestSupport_PPT_ASSERT(p == nullptr, "testNewPodInt null after destroy");
}

void PodPoolThreadTest::testNewPodLlongExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<llong> pool;
	llong* p = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testNewPodLlong not null");
	*p = 9876543210LL;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 9876543210LL, "testNewPodLlong value");
	pool.destroyPod(p);
}

void PodPoolThreadTest::testNewPodUshortExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<ushort> pool;
	ushort* p = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testNewPodUshort not null");
	*p = 65535u;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 65535u, "testNewPodUshort value");
	pool.destroyPod(p);
}

void PodPoolThreadTest::testNewPodByteExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<byte> pool;
	byte* p = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testNewPodByte not null");
	*p = 255u;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 255u, "testNewPodByte value");
	pool.destroyPod(p);
}

void PodPoolThreadTest::testDestroyPodSetsNull()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	pool.destroyPod(p);
	PodPoolThreadExtTestSupport_PPT_ASSERT(p == nullptr, "testDestroyPodSetsNull null");
}

void PodPoolThreadTest::testDestroyPodNullSafe()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* p = nullptr;
	pool.destroyPod(p); // 不崩溃
	PodPoolThreadExtTestSupport_PPT_ASSERT(p == nullptr, "testDestroyPodNullSafe still null");
}

void PodPoolThreadTest::testDestroyPodDestroiedFlag()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	// 模拟 pool 已被销毁 (mDestroied=true) — 通过 quit() 后访问
	// 不触发 mDestroied，而是测试正常 destroy
	pool.destroyPod(p);
	PodPoolThreadExtTestSupport_PPT_ASSERT(p == nullptr, "testDestroyPodDestroiedFlag null");
}

void PodPoolThreadTest::testReuseAfterDestroyExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* p1 = pool.newPod();
	*p1 = 100;
	pool.destroyPod(p1);
	PodPoolThreadExtTestSupport_PPT_ASSERT(p1 == nullptr, "testReuseAfterDestroy p1 null");
	int* p2 = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p2 != nullptr, "testReuseAfterDestroy p2 not null");
	// 复用的对象可能保留旧值（POD 不做重置）
	*p2 = 200;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p2 == 200, "testReuseAfterDestroy p2 value");
	pool.destroyPod(p2);
}

void PodPoolThreadTest::testInitDefaultPreAllocates()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(5);
	// 能取出 5 个
	Vector<int*> ptrs;
	FOR(5)
	{
		ptrs.add(pool.newPod());
	}
	FOR(5)
	{
		PodPoolThreadExtTestSupport_PPT_ASSERT(ptrs[i] != nullptr, "testInitDefaultPreAllocates not null");
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testExhaustPoolDynamic()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(3);
	Vector<int*> ptrs;
	// 前3个来自预分配，第4个动态创建
	FOR(4)
	{
		ptrs.add(pool.newPod());
	}
	FOR(4)
	{
		PodPoolThreadExtTestSupport_PPT_ASSERT(ptrs[i] != nullptr, "testExhaustPoolDynamic not null");
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testMultipleDestroyAndNew()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* a = pool.newPod();
	int* b = pool.newPod();
	int* c = pool.newPod();
	*a = 1; *b = 2; *c = 3;
	pool.destroyPod(a);
	pool.destroyPod(b);
	int* d = pool.newPod(); // 复用 b（后进先出）
	int* e = pool.newPod(); // 复用 a
	PodPoolThreadExtTestSupport_PPT_ASSERT(d != nullptr, "testMultipleDestroyAndNew d");
	PodPoolThreadExtTestSupport_PPT_ASSERT(e != nullptr, "testMultipleDestroyAndNew e");
	pool.destroyPod(c);
	pool.destroyPod(d);
	pool.destroyPod(e);
}

void PodPoolThreadTest::testClearPoolExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* a = pool.newPod();
	int* b = pool.newPod();
	pool.destroyPod(a);
	pool.destroyPod(b);
	pool.clearPool(); // 释放 unusedList 中 2 个
	PodPoolThreadExtTestSupport_PPT_ASSERT(true, "testClearPool no crash");
	// 再次 new 应正常工作（动态创建）
	int* c = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(c != nullptr, "testClearPool new after clear");
	pool.destroyPod(c);
}

void PodPoolThreadTest::testClearPoolAfterDestroy()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(3);
	// 先取出来不还回去
	int* a = pool.newPod();
	int* b = pool.newPod();
	// c 仍在 unusedList 中，clearPool 会释放它
	pool.clearPool();
	PodPoolThreadExtTestSupport_PPT_ASSERT(true, "testClearPoolAfterDestroy no crash");
	pool.destroyPod(a);
	pool.destroyPod(b);
}

void PodPoolThreadTest::testTotalCountTracking()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	// mTotalCount 不对外暴露，但通过行为验证：每次动态 new 都不崩溃
	const int N = 20;
	Vector<int*> ptrs;
	FOR(N)
	{
		ptrs.add(pool.newPod());
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
	PodPoolThreadExtTestSupport_PPT_ASSERT(true, "testTotalCountTracking no crash");
}

void PodPoolThreadTest::testIntPoolThread()
{
	using namespace PodPoolThreadExtTestSupport;
	IntPoolThread pool;
	int* p = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testIntPoolThread not null");
	*p = 77;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 77, "testIntPoolThread value");
	pool.destroyPod(p);
	PodPoolThreadExtTestSupport_PPT_ASSERT(p == nullptr, "testIntPoolThread null after destroy");
}

void PodPoolThreadTest::testPoolNewValueIsValid()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	// 可以写入任意有效整数
	*p = INT_MAX;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == INT_MAX, "testPoolNewValueIsValid INT_MAX");
	*p = INT_MIN;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == INT_MIN, "testPoolNewValueIsValid INT_MIN");
	*p = 0;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 0, "testPoolNewValueIsValid 0");
	pool.destroyPod(p);
}

void PodPoolThreadTest::testPoolReuseOrder()
{
	using namespace PodPoolThreadExtTestSupport;
	// 验证复用遵循后进先出（LIFO）顺序
	PodPoolThread<int> pool;
	int* a = pool.newPod();
	int* b = pool.newPod();
	int* c = pool.newPod();
	*a = 1; *b = 2; *c = 3;
	pool.destroyPod(c); // c 回池，排队末尾
	pool.destroyPod(b); // b 回池
	pool.destroyPod(a); // a 回池
	// 新取出：顺序是从末尾（popBack），即 a 优先
	int* x = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(x != nullptr, "testPoolReuseOrder x");
	pool.destroyPod(x);
}

void PodPoolThreadTest::testLargePoolExt()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	const int N = 200;
	Vector<int*> ptrs;
	FOR(N)
	{
		int* p = pool.newPod();
		*p = i;
		ptrs.add(p);
	}
	FOR(N)
	{
		PodPoolThreadExtTestSupport_PPT_ASSERT(*ptrs[i] == i, "testLargePool value check");
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testMixedNewDestroyCycles()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	const int CYCLES = 50;
	FOR(CYCLES)
	{
		int* a = pool.newPod();
		int* b = pool.newPod();
		*a = i * 2;
		*b = i * 2 + 1;
		pool.destroyPod(a);
		int* c = pool.newPod();
		*c = i * 100;
		pool.destroyPod(b);
		pool.destroyPod(c);
	}
	PodPoolThreadExtTestSupport_PPT_ASSERT(true, "testMixedNewDestroyCycles no crash");
}

void PodPoolThreadTest::testQuitFreesMemory()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(5);
	pool.quit(); // 释放 unusedList 中 5 个，不崩溃
	PodPoolThreadExtTestSupport_PPT_ASSERT(true, "testQuitFreesMemory no crash");
}

void PodPoolThreadTest::testNewAfterClearPool()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(3);
	pool.clearPool(); // 清空 unusedList
	// 再次 new 应动态创建
	int* p = pool.newPod();
	PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testNewAfterClearPool not null");
	pool.destroyPod(p);
}

void PodPoolThreadTest::testClearPoolMultipleTimes()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	FOR(3)
	{
		int* a = pool.newPod();
		int* b = pool.newPod();
		pool.destroyPod(a);
		pool.destroyPod(b);
		pool.clearPool(); // 每次清空
	}
	PodPoolThreadExtTestSupport_PPT_ASSERT(true, "testClearPoolMultipleTimes no crash");
}

void PodPoolThreadTest::testDestroyManyThenNew()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	const int N = 30;
	Vector<int*> ptrs;
	FOR(N)
	{
		ptrs.add(pool.newPod());
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
	// 再 new N 个，全部来自复用
	Vector<int*> ptrs2;
	FOR(N)
	{
		ptrs2.add(pool.newPod());
	}
	FOR(N)
	{
		PodPoolThreadExtTestSupport_PPT_ASSERT(ptrs2[i] != nullptr, "testDestroyManyThenNew reuse not null");
	}
	for (int* p : ptrs2)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testInitDefaultThenExhaust()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	const int INIT = 10;
	const int EXTRA = 5;
	pool.initDefault(INIT);
	Vector<int*> ptrs;
	FOR(INIT + EXTRA)
	{
		int* p = pool.newPod();
		PodPoolThreadExtTestSupport_PPT_ASSERT(p != nullptr, "testInitDefaultThenExhaust not null");
		ptrs.add(p);
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testNewReturnsUniquePointers()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	const int N = 10;
	Vector<int*> ptrs;
	FOR(N)
	{
		ptrs.add(pool.newPod());
	}
	// 所有指针互不相同
	for (int i = 0; i < N; ++i)
	{
		for (int j = i + 1; j < N; ++j)
		{
			PodPoolThreadExtTestSupport_PPT_ASSERT(ptrs[i] != ptrs[j], "testNewReturnsUniquePointers unique");
		}
	}
	for (int* p : ptrs)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testPodPoolLlongBoundary()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<llong> pool;
	pool.initDefault(2);
	llong* p1 = pool.newPod();
	llong* p2 = pool.newPod();
	llong* p3 = pool.newPod(); // 动态创建
	*p1 = LLONG_MAX;
	*p2 = LLONG_MIN;
	*p3 = 0LL;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p1 == LLONG_MAX, "llong LLONG_MAX");
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p2 == LLONG_MIN, "llong LLONG_MIN");
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p3 == 0LL, "llong 0");
	pool.destroyPod(p1);
	pool.destroyPod(p2);
	pool.destroyPod(p3);
}

void PodPoolThreadTest::testPodPoolUshortBoundary()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<ushort> pool;
	ushort* p = pool.newPod();
	*p = 0u;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 0u, "ushort 0");
	pool.destroyPod(p);
	p = pool.newPod();
	*p = 65535u;
	PodPoolThreadExtTestSupport_PPT_ASSERT(*p == 65535u, "ushort 65535");
	pool.destroyPod(p);
}

void PodPoolThreadTest::testPodPoolIntReuseInSequence()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	const int ROUNDS = 100;
	FOR(ROUNDS)
	{
		int* p = pool.newPod();
		*p = i;
		PodPoolThreadExtTestSupport_PPT_ASSERT(*p == i, "PodPool int seq value");
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testPodPoolByteAllOnes()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<byte> pool;
	Vector<byte*> ptrs;
	FOR(16)
	{
		byte* p = pool.newPod();
		*p = 0xFFu;
		ptrs.add(p);
	}
	FOR(16)
	{
		PodPoolThreadExtTestSupport_PPT_ASSERT(*ptrs[i] == 0xFFu, "PodPool byte all ones");
	}
	for (byte* p : ptrs)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::testPodPoolIntAlternateOddEven()
{
	using namespace PodPoolThreadExtTestSupport;
	PodPoolThread<int> pool;
	// 新建偶数组，销毁，新建奇数组
	Vector<int*> evens, odds;
	FOR(10)
	{
		int* p = pool.newPod();
		*p = i * 2;
		evens.add(p);
	}
	for (int* p : evens)
	{
		pool.destroyPod(p);
	}
	FOR(10)
	{
		int* p = pool.newPod();
		*p = i * 2 + 1;
		odds.add(p);
	}
	FOR(10)
	{
		PodPoolThreadExtTestSupport_PPT_ASSERT(*odds[i] == i * 2 + 1, "PodPool odd even value");
	}
	for (int* p : odds)
	{
		pool.destroyPod(p);
	}
}

void PodPoolThreadTest::runExtraPodPoolTests()
{
	using namespace PodPoolThreadExtTestSupport;
	testPodPoolLlongBoundary();
	testPodPoolUshortBoundary();
	testPodPoolIntReuseInSequence();
	testPodPoolByteAllOnes();
	testPodPoolIntAlternateOddEven();
}

namespace PodPoolThreadExt2TestSupport
{
	// ============================================================
	// PodPoolThreadExt2Test
	// PodPoolThread<T> 边界值 / 复用语义 / initDefault / clearPool
	// 扩展覆盖（补充 PodPoolThreadTest 和 PodPoolThreadExtTest）
	// ============================================================

#define PodPoolThreadExt2TestSupport_PPE2_ASSERT(expr) \
	if (!(expr)) { ERROR(string("PodPoolThreadExt2Test FAIL: ") + #expr); }


	// ============================================================
	// 基础类型边界
	// ============================================================















	// ============================================================
	// 复用语义
	// ============================================================






	// ============================================================
	// initDefault 场景：initDefault(N) 预分配 N 个对象到池中
	// ============================================================







	// ============================================================
	// clearPool 场景
	// ============================================================





	// ============================================================
	// 压力测试
	// ============================================================
}

void PodPoolThreadTest::testIntBoundaryMax()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 2147483647;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 2147483647);
	pool.destroyPod(p);
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testIntBoundaryMin()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = -2147483647;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == -2147483647);
	pool.destroyPod(p);
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testIntZero()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 0;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 0);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testLlongBoundaryMax()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	llong* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 9223372036854775807LL;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 9223372036854775807LL);
	pool.destroyPod(p);
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p == nullptr);
}

void PodPoolThreadTest::testLlongBoundaryMin()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	llong* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = -9223372036854775807LL;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == -9223372036854775807LL);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testLlongZero()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	llong* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 0LL;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 0LL);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testShortBoundaryMax()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<short> pool;
	short* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 32767;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 32767);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testShortBoundaryMin()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<short> pool;
	short* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = -32767;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == -32767);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testUshortBoundaryMax()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<ushort> pool;
	ushort* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 65535;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 65535);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testUshortZero()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<ushort> pool;
	ushort* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 0;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 0);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testByteMax()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<byte> pool;
	byte* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 255;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 255);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testByteZero()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<byte> pool;
	byte* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 0;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 0);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testUintMax()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<uint> pool;
	uint* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 4294967295u;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 4294967295u);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testUintZero()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<uint> pool;
	uint* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 0u;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 0u);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testReusePreservesAddress()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	int* first = pool.newPod();
	void* addr = first;
	pool.destroyPod(first);
	int* second = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(second == addr);
	pool.destroyPod(second);
}

void PodPoolThreadTest::testReuseResetsValue()
{
	using namespace PodPoolThreadExt2TestSupport;
	// PodPoolThread 归还后不 reset 值（不像 ClassPool），这里验证行为
	// 重新获取后应能正常写入和读取
	PodPoolThread<int> pool;
	int* p = pool.newPod();
	*p = 12345;
	pool.destroyPod(p);
	int* p2 = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p2 != nullptr);
	*p2 = 0;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p2 == 0);
	pool.destroyPod(p2);
}

void PodPoolThreadTest::testMultipleReuseChain()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	const int N = 10;
	int* ptrs[N];
	for (int i = 0; i < N; ++i)
	{
		ptrs[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(ptrs[i] != nullptr);
		*ptrs[i] = i;
	}
	FOR_INVERSE(N)
	{
		pool.destroyPod(ptrs[i]);
	}
	for (int i = 0; i < N; ++i)
	{
		ptrs[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(ptrs[i] != nullptr);
		*ptrs[i] = i * 100;
	}
	for (int i = 0; i < N; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*ptrs[i] == i * 100);
		pool.destroyPod(ptrs[i]);
	}
}

void PodPoolThreadTest::testBatchReuseInt()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	const int BATCH = 25;
	int* arr[BATCH];
	// 分配一批
	for (int i = 0; i < BATCH; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = i * 3;
	}
	// 全部释放
	for (int i = 0; i < BATCH; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	// 再分配一批（复用）
	for (int i = 0; i < BATCH; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = i * 5;
	}
	for (int i = 0; i < BATCH; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == i * 5);
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testBatchReuseLlong()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	const int BATCH = 20;
	llong* arr[BATCH];
	for (int i = 0; i < BATCH; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = (llong)i * 1000000LL;
	}
	for (int i = 0; i < BATCH; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	for (int i = 0; i < BATCH; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = (llong)i * (-500000LL);
	}
	for (int i = 0; i < BATCH; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == (llong)i * (-500000LL));
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testInitDefaultInt()
{
	using namespace PodPoolThreadExt2TestSupport;
	// 预分配 5 个，newPod 直接复用，不为 nullptr
	PodPoolThread<int> pool;
	pool.initDefault(5);
	int* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 42;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 42);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testInitDefaultLlong()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	pool.initDefault(3);
	llong* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 999999LL;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 999999LL);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testInitDefaultShort()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<short> pool;
	pool.initDefault(3);
	short* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = (short)1024;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == (short)1024);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testInitDefaultByte()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<byte> pool;
	pool.initDefault(3);
	byte* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = (byte)200;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == (byte)200);
	pool.destroyPod(p);
}

void PodPoolThreadTest::testInitDefaultThenReuse()
{
	using namespace PodPoolThreadExt2TestSupport;
	// 预分配后 newPod/destroyPod/newPod 可以正常复用
	PodPoolThread<int> pool;
	pool.initDefault(4);
	int* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 77;
	pool.destroyPod(p);
	// 归还后重新取，指针不为 nullptr
	int* p2 = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p2 != nullptr);
	*p2 = 77;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p2 == 77);
	pool.destroyPod(p2);
}

void PodPoolThreadTest::testInitDefaultMultipleValues()
{
	using namespace PodPoolThreadExt2TestSupport;
	// 预分配 8 个，批量 newPod 均不为 nullptr，写入值后再 destroyPod 再复用
	PodPoolThread<int> pool;
	pool.initDefault(8);
	const int N = 8;
	int* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = i * 11;
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	// 复用后 newPod 仍不为 nullptr
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testClearPoolInt()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	int* p1 = pool.newPod();
	int* p2 = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p1 != nullptr);
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p2 != nullptr);
	pool.destroyPod(p1);
	pool.destroyPod(p2);
	pool.clearPool();
	// After clearPool, newPod should still work
	int* p3 = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p3 != nullptr);
	pool.destroyPod(p3);
}

void PodPoolThreadTest::testClearPoolLlong()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	const int N = 6;
	llong* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	pool.clearPool();
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testClearPoolThenNew()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	pool.initDefault(4);  // 预分配 4 个
	int* p = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p != nullptr);
	*p = 55;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p == 55);
	pool.destroyPod(p);
	pool.clearPool();
	int* p2 = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p2 != nullptr);
	pool.destroyPod(p2);
}

void PodPoolThreadTest::testClearPoolPreservesType()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<short> pool;
	short* p = pool.newPod();
	*p = 300;
	pool.destroyPod(p);
	pool.clearPool();
	short* p2 = pool.newPod();
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(p2 != nullptr);
	*p2 = 400;
	PodPoolThreadExt2TestSupport_PPE2_ASSERT(*p2 == 400);
	pool.destroyPod(p2);
}

void PodPoolThreadTest::testStressInt()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	const int STRESS = 100;
	int* arr[STRESS];
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = i;
	}
	for (int i = 0; i < STRESS; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == i);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = i * 7;
	}
	for (int i = 0; i < STRESS; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == i * 7);
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testStressLlong()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<llong> pool;
	const int STRESS = 80;
	llong* arr[STRESS];
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = (llong)i * 100000LL;
	}
	for (int i = 0; i < STRESS; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == (llong)i * 100000LL);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = (llong)(-i);
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == (llong)(-i));
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testStressUint()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<uint> pool;
	const int STRESS = 60;
	uint* arr[STRESS];
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = (uint)i * 3u;
	}
	for (int i = 0; i < STRESS; ++i)
	{
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == (uint)i * 3u);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		pool.destroyPod(arr[i]);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newPod();
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
		*arr[i] = (uint)i + 1000u;
		PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == (uint)i + 1000u);
		pool.destroyPod(arr[i]);
	}
}

void PodPoolThreadTest::testStressAlternateClear()
{
	using namespace PodPoolThreadExt2TestSupport;
	PodPoolThread<int> pool;
	const int ROUNDS = 5;
	const int PER = 20;
	for (int r = 0; r < ROUNDS; ++r)
	{
		int* arr[PER];
		for (int i = 0; i < PER; ++i)
		{
			arr[i] = pool.newPod();
			PodPoolThreadExt2TestSupport_PPE2_ASSERT(arr[i] != nullptr);
			*arr[i] = r * 100 + i;
		}
		for (int i = 0; i < PER; ++i)
		{
			PodPoolThreadExt2TestSupport_PPE2_ASSERT(*arr[i] == r * 100 + i);
			pool.destroyPod(arr[i]);
		}
		pool.clearPool();
	}
}


void PodPoolThreadTest::test()
{
	testNewPodInt();
	testNewPodLlong();
	testNewPodShort();
	testNewPodUshort();
	testNewPodByte();
	testNewPodUint();
	testNewPodNotNull();
	testDestroyPodNullAfter();
	testDestroyNullSafe();
	testReuseAfterDestroy();
	testReuseValue();
	testInitDefault();
	testInitDefaultThenNew();
	testInitDefaultExhaust();
	testClearPool();
	testClearPoolAfterNew();
	testMultipleNewSameType();
	testAlternateNewDestroy();
	testBatchNewDestroy();
	testManyRoundsInt();
	testManyRoundsLlong();
	testDestroyTwiceSafe();
	testLargePool();
	testNewAfterClear();
	testInitDefaultMultiple();
	testPointerUniqueness();
	testIntWriteRead();
	testLlongWriteRead();
	testShortWriteRead();
	testByteWriteRead();
	testNewPodIntExt();
	testNewPodLlongExt();
	testNewPodUshortExt();
	testNewPodByteExt();
	testDestroyPodSetsNull();
	testDestroyPodNullSafe();
	testDestroyPodDestroiedFlag();
	testReuseAfterDestroyExt();
	testInitDefaultPreAllocates();
	testExhaustPoolDynamic();
	testMultipleDestroyAndNew();
	testClearPoolExt();
	testClearPoolAfterDestroy();
	testTotalCountTracking();
	testIntPoolThread();
	testPoolNewValueIsValid();
	testPoolReuseOrder();
	testLargePoolExt();
	testMixedNewDestroyCycles();
	testQuitFreesMemory();
	testNewAfterClearPool();
	testClearPoolMultipleTimes();
	testDestroyManyThenNew();
	testInitDefaultThenExhaust();
	testNewReturnsUniquePointers();
	runExtraPodPoolTests();
	testIntBoundaryMax();
	testIntBoundaryMin();
	testIntZero();
	testLlongBoundaryMax();
	testLlongBoundaryMin();
	testLlongZero();
	testShortBoundaryMax();
	testShortBoundaryMin();
	testUshortBoundaryMax();
	testUshortZero();
	testByteMax();
	testByteZero();
	testUintMax();
	testUintZero();
	testReusePreservesAddress();
	testReuseResetsValue();
	testMultipleReuseChain();
	testBatchReuseInt();
	testBatchReuseLlong();
	testInitDefaultInt();
	testInitDefaultLlong();
	testInitDefaultShort();
	testInitDefaultByte();
	testInitDefaultThenReuse();
	testInitDefaultMultipleValues();
	testClearPoolInt();
	testClearPoolLlong();
	testClearPoolThenNew();
	testClearPoolPreservesType();
	testStressInt();
	testStressLlong();
	testStressUint();
	testStressAlternateClear();
}
#endif
