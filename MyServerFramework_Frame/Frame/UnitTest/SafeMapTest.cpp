#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// SafeMapTest
// Testing SafeMap<Key, Value>
// SafeMap: Map-backed container safe for modify-during-foreach
// API: add, remove, contains, get, getPtr, size, isEmpty,
//      startForeach, endForeach, isForeaching, getMainList, clear
// ============================================================

#define SMT_ASSERT(expr) \
	if (!(expr)) { ERROR(string("SafeMapTest FAIL: ") + #expr); }

void SafeMapTest::test()
{
	testSafeMapEmpty();
	testSafeMapInsert();
	testSafeMapInsertDuplicate();
	testSafeMapErase();
	testSafeMapEraseNotExist();
	testSafeMapContains();
	testSafeMapTryGet();
	testSafeMapGetPtr();
	testSafeMapSize();
	testSafeMapClear();
	testSafeMapForeachEmpty();
	testSafeMapForeachBasic();
	testSafeMapInsertDuringForeach();
	testSafeMapEraseDuringForeach();
	testSafeMapModifyBetweenForeach();
	testSafeMapStringKey();
	testSafeMapIntKey();
	testSafeMapLargeCount();
	testSafeMapMultiRound();
	testSafeMapGetMainList();
	testSafeMapNotForeachingAfterEnd();
	testSafeMapForeachAllRead();
	testSafeMapAddThenEraseInForeach();
	testSafeMapChainInsertErase();
	testSafeMapBasicInsert();
	testSafeMapBasicErase();
	testSafeMapContainerContains();
	testSafeMapContainerTryGet();
	testSafeMapContainerClear();
	testSafeMapContainerInsertDuringForeach();
	testSafeMapContainerEraseDuringForeach();
	testSafeMapSizeAndEmpty();
}

void SafeMapTest::testSafeMapEmpty()
{
	SafeMap<int, int> sm;
	SMT_ASSERT(sm.isEmpty());
	SMT_ASSERT(sm.size() == 0);
	SMT_ASSERT(!sm.isForeaching());
}

void SafeMapTest::testSafeMapInsert()
{
	SafeMap<int, int> sm;
	bool r1 = sm.add(1, 100);
	SMT_ASSERT(r1);
	SMT_ASSERT(sm.size() == 1);
	SMT_ASSERT(!sm.isEmpty());
	bool r2 = sm.add(2, 200);
	SMT_ASSERT(r2);
	SMT_ASSERT(sm.size() == 2);
	bool r3 = sm.add(3, 300);
	SMT_ASSERT(r3);
	SMT_ASSERT(sm.size() == 3);
}

void SafeMapTest::testSafeMapInsertDuplicate()
{
	SafeMap<int, int> sm;
	sm.add(5, 50);
	SMT_ASSERT(sm.size() == 1);
	bool r = sm.add(5, 99);
	// Insert duplicate should fail (Map semantics)
	SMT_ASSERT(!r);
	SMT_ASSERT(sm.size() == 1);
	SMT_ASSERT(sm.get(5, 0) == 50);
}

void SafeMapTest::testSafeMapErase()
{
	SafeMap<int, int> sm;
	sm.add(10, 1000);
	sm.add(20, 2000);
	sm.add(30, 3000);
	SMT_ASSERT(sm.size() == 3);
	bool r = sm.remove(20);
	SMT_ASSERT(r);
	SMT_ASSERT(sm.size() == 2);
	SMT_ASSERT(!sm.contains(20));
	SMT_ASSERT(sm.contains(10));
	SMT_ASSERT(sm.contains(30));
}

void SafeMapTest::testSafeMapEraseNotExist()
{
	SafeMap<int, int> sm;
	sm.add(1, 10);
	bool r = sm.remove(99);
	SMT_ASSERT(!r);
	SMT_ASSERT(sm.size() == 1);
}

void SafeMapTest::testSafeMapContains()
{
	SafeMap<int, int> sm;
	sm.add(7, 70);
	sm.add(8, 80);
	SMT_ASSERT(sm.contains(7));
	SMT_ASSERT(sm.contains(8));
	SMT_ASSERT(!sm.contains(9));
	sm.remove(7);
	SMT_ASSERT(!sm.contains(7));
	SMT_ASSERT(sm.contains(8));
}

void SafeMapTest::testSafeMapTryGet()
{
	SafeMap<int, int> sm;
	sm.add(100, 999);
	sm.add(200, 888);
	SMT_ASSERT(sm.get(100, 0) == 999);
	SMT_ASSERT(sm.get(200, 0) == 888);
	SMT_ASSERT(sm.get(300, -1) == -1);
	SMT_ASSERT(sm.get(999, 0) == 0);
}

void SafeMapTest::testSafeMapGetPtr()
{
	SafeMap<int, int> sm;
	sm.add(42, 420);
	int* ptr = sm.getPtr(42);
	SMT_ASSERT(ptr != nullptr);
	SMT_ASSERT(*ptr == 420);
	int* ptrNull = sm.getPtr(99);
	SMT_ASSERT(ptrNull == nullptr);
	const SafeMap<int, int>& smConst = sm;
	const int* constPtr = smConst.getPtrConst(42);
	SMT_ASSERT(constPtr != nullptr);
	SMT_ASSERT(*constPtr == 420);
}

void SafeMapTest::testSafeMapSize()
{
	SafeMap<int, int> sm;
	SMT_ASSERT(sm.size() == 0);
	for (int i = 0; i < 10; ++i)
	{
		sm.add(i, i * 10);
	}
	SMT_ASSERT(sm.size() == 10);
	sm.remove(5);
	SMT_ASSERT(sm.size() == 9);
	sm.clear();
	SMT_ASSERT(sm.size() == 0);
}

void SafeMapTest::testSafeMapClear()
{
	SafeMap<int, int> sm;
	sm.add(1, 11);
	sm.add(2, 22);
	sm.add(3, 33);
	sm.clear();
	SMT_ASSERT(sm.isEmpty());
	SMT_ASSERT(sm.size() == 0);
	SMT_ASSERT(!sm.contains(1));
	// Can add again after clear
	sm.add(10, 100);
	SMT_ASSERT(sm.size() == 1);
	SMT_ASSERT(sm.contains(10));
}

void SafeMapTest::testSafeMapForeachEmpty()
{
	SafeMap<int, int> sm;
	const Map<int, int>& readList = sm.startForeach();
	SMT_ASSERT(sm.isForeaching());
	SMT_ASSERT(readList.size() == 0);
	sm.endForeach();
	SMT_ASSERT(!sm.isForeaching());
}

void SafeMapTest::testSafeMapForeachBasic()
{
	SafeMap<int, int> sm;
	sm.add(1, 10);
	sm.add(2, 20);
	sm.add(3, 30);
	const Map<int, int>& readList = sm.startForeach();
	SMT_ASSERT(readList.size() == 3);
	int sum = 0;
	for (const auto& pair : readList)
	{
		sum += pair.second;
	}
	SMT_ASSERT(sum == 60);
	sm.endForeach();
	SMT_ASSERT(!sm.isForeaching());
}

void SafeMapTest::testSafeMapInsertDuringForeach()
{
	SafeMap<int, int> sm;
	sm.add(1, 10);
	sm.add(2, 20);
	sm.add(3, 30);
	const Map<int, int>& readList = sm.startForeach();
	SMT_ASSERT(readList.size() == 3);
	// Insert during foreach
	sm.add(4, 40);
	sm.add(5, 50);
	SMT_ASSERT(sm.size() == 5);
	// ReadList still has 3 during traverse
	SMT_ASSERT(readList.size() == 3);
	sm.endForeach();
	SMT_ASSERT(sm.size() == 5);
}

void SafeMapTest::testSafeMapEraseDuringForeach()
{
	SafeMap<int, int> sm;
	sm.add(10, 100);
	sm.add(20, 200);
	sm.add(30, 300);
	sm.add(40, 400);
	const Map<int, int>& readList = sm.startForeach();
	SMT_ASSERT(readList.size() == 4);
	sm.remove(20);
	SMT_ASSERT(sm.size() == 3);
	sm.endForeach();
	SMT_ASSERT(sm.size() == 3);
	SMT_ASSERT(!sm.contains(20));
}

void SafeMapTest::testSafeMapModifyBetweenForeach()
{
	SafeMap<int, int> sm;
	sm.add(1, 10);
	sm.add(2, 20);
	// First foreach
	{
		const Map<int, int>& readList = sm.startForeach();
		SMT_ASSERT(readList.size() == 2);
		sm.endForeach();
	}
	// Modify between
	sm.add(3, 30);
	sm.remove(1);
	SMT_ASSERT(sm.size() == 2);
	SMT_ASSERT(!sm.contains(1));
	SMT_ASSERT(sm.contains(3));
	// Second foreach
	{
		const Map<int, int>& readList = sm.startForeach();
		SMT_ASSERT(readList.size() == 2);
		SMT_ASSERT(readList.get(3, 0) == 30);
		sm.endForeach();
	}
}

void SafeMapTest::testSafeMapStringKey()
{
	SafeMap<string, int> sm;
	sm.add("apple", 1);
	sm.add("banana", 2);
	sm.add("cherry", 3);
	SMT_ASSERT(sm.size() == 3);
	SMT_ASSERT(sm.contains("apple"));
	SMT_ASSERT(sm.contains("banana"));
	SMT_ASSERT(!sm.contains("grape"));
	SMT_ASSERT(sm.get("apple", 0) == 1);
	SMT_ASSERT(sm.get("cherry", 0) == 3);
	sm.remove("banana");
	SMT_ASSERT(!sm.contains("banana"));
	SMT_ASSERT(sm.size() == 2);
}

void SafeMapTest::testSafeMapIntKey()
{
	SafeMap<int, string> sm;
	sm.add(100, "hundred");
	sm.add(200, "two hundred");
	sm.add(300, "three hundred");
	SMT_ASSERT(sm.size() == 3);
	SMT_ASSERT(sm.get(100, string("")) == "hundred");
	SMT_ASSERT(sm.get(200, string("")) == "two hundred");
	SMT_ASSERT(sm.get(999, string("default")) == "default");
}

void SafeMapTest::testSafeMapLargeCount()
{
	SafeMap<int, int> sm;
	const int N = 50;
	for (int i = 0; i < N; ++i)
	{
		sm.add(i, i * i);
	}
	SMT_ASSERT(sm.size() == N);
	for (int i = 0; i < N; ++i)
	{
		SMT_ASSERT(sm.contains(i));
		SMT_ASSERT(sm.get(i, -1) == i * i);
	}
	// Erase half
	for (int i = 0; i < N; i += 2)
	{
		sm.remove(i);
	}
	SMT_ASSERT(sm.size() == N / 2);
	for (int i = 0; i < N; i += 2)
	{
		SMT_ASSERT(!sm.contains(i));
	}
	for (int i = 1; i < N; i += 2)
	{
		SMT_ASSERT(sm.contains(i));
	}
}

void SafeMapTest::testSafeMapMultiRound()
{
	SafeMap<int, int> sm;
	const int ROUNDS = 10;
	const int PER_ROUND = 5;
	for (int r = 0; r < ROUNDS; ++r)
	{
		for (int i = 0; i < PER_ROUND; ++i)
		{
			sm.add(r * PER_ROUND + i, r * 100 + i);
		}
		SMT_ASSERT(sm.size() == (r + 1) * PER_ROUND);
	}
	SMT_ASSERT(sm.size() == ROUNDS * PER_ROUND);
	for (int r = 0; r < ROUNDS; ++r)
	{
		for (int i = 0; i < PER_ROUND; ++i)
		{
			int key = r * PER_ROUND + i;
			SMT_ASSERT(sm.get(key, -1) == r * 100 + i);
			sm.remove(key);
		}
	}
	SMT_ASSERT(sm.isEmpty());
}

void SafeMapTest::testSafeMapGetMainList()
{
	SafeMap<int, int> sm;
	sm.add(1, 10);
	sm.add(2, 20);
	const Map<int, int>& main = sm.getMainList();
	SMT_ASSERT(main.size() == 2);
	SMT_ASSERT(main.contains(1));
	SMT_ASSERT(main.contains(2));
	SMT_ASSERT(main.get(1, 0) == 10);
}

void SafeMapTest::testSafeMapNotForeachingAfterEnd()
{
	SafeMap<int, int> sm;
	sm.add(1, 100);
	SMT_ASSERT(!sm.isForeaching());
	const Map<int, int>& readList = sm.startForeach();
	SMT_ASSERT(sm.isForeaching());
	SMT_ASSERT(readList.size() == 1);
	sm.endForeach();
	SMT_ASSERT(!sm.isForeaching());
}

void SafeMapTest::testSafeMapForeachAllRead()
{
	SafeMap<int, int> sm;
	for (int i = 0; i < 8; ++i)
	{
		sm.add(i * 10, i * 100);
	}
	const Map<int, int>& readList = sm.startForeach();
	int count = 0;
	for (const auto& pair : readList)
	{
		SMT_ASSERT(pair.first % 10 == 0);
		SMT_ASSERT(pair.second == pair.first * 10);
		++count;
	}
	SMT_ASSERT(count == 8);
	sm.endForeach();
}

void SafeMapTest::testSafeMapAddThenEraseInForeach()
{
	SafeMap<int, int> sm;
	sm.add(1, 10);
	sm.add(2, 20);
	sm.add(3, 30);
	const Map<int, int>& readList = sm.startForeach();
	SMT_ASSERT(readList.size() == 3);
	// Add and remove during same foreach
	sm.add(4, 40);
	sm.remove(2);
	SMT_ASSERT(sm.size() == 3);
	SMT_ASSERT(!sm.contains(2));
	SMT_ASSERT(sm.contains(4));
	sm.endForeach();
	SMT_ASSERT(sm.size() == 3);
}

void SafeMapTest::testSafeMapChainInsertErase()
{
	SafeMap<int, int> sm;
	// Chain: add 1..10, then remove odd, then re-add them
	for (int i = 1; i <= 10; ++i)
	{
		sm.add(i, i * 5);
	}
	SMT_ASSERT(sm.size() == 10);
	for (int i = 1; i <= 10; i += 2)
	{
		sm.remove(i);
	}
	SMT_ASSERT(sm.size() == 5);
	for (int i = 1; i <= 10; i += 2)
	{
		sm.add(i, i * 10);
	}
	SMT_ASSERT(sm.size() == 10);
	for (int i = 1; i <= 10; i += 2)
	{
		SMT_ASSERT(sm.get(i, 0) == i * 10);
	}
	for (int i = 2; i <= 10; i += 2)
	{
		SMT_ASSERT(sm.get(i, 0) == i * 5);
	}
}

// ============================================================
// Consolidated safe container tests
// ============================================================
#define SAFEMAPTEST_SCT_ASSERT(expr) \
	if (!(expr)) { LOG("SafeMapTest FAILED: " #expr); }

void SafeMapTest::testSafeMapBasicInsert()
{
	SafeMap<int, int> m;
	SAFEMAPTEST_SCT_ASSERT(m.isEmpty());
	SAFEMAPTEST_SCT_ASSERT(m.add(3, 30));
	SAFEMAPTEST_SCT_ASSERT(m.add(1, 10));
	SAFEMAPTEST_SCT_ASSERT(m.add(2, 20));
	SAFEMAPTEST_SCT_ASSERT(m.size() == 3);
	// 重复插入失败
	SAFEMAPTEST_SCT_ASSERT(!m.add(1, 99));
}

void SafeMapTest::testSafeMapBasicErase()
{
	SafeMap<int, int> m;
	m.add(5, 50);
	m.add(10, 100);
	SAFEMAPTEST_SCT_ASSERT(m.remove(5));
	SAFEMAPTEST_SCT_ASSERT(!m.contains(5));
	SAFEMAPTEST_SCT_ASSERT(m.size() == 1);
	SAFEMAPTEST_SCT_ASSERT(!m.remove(999));
}

void SafeMapTest::testSafeMapContainerContains()
{
	SafeMap<string, int> m;
	m.add("hello", 1);
	SAFEMAPTEST_SCT_ASSERT(m.contains("hello"));
	SAFEMAPTEST_SCT_ASSERT(!m.contains("world"));
}

void SafeMapTest::testSafeMapContainerTryGet()
{
	SafeMap<int, int> m;
	m.add(42, 420);
	const int& v = m.get(42);
	SAFEMAPTEST_SCT_ASSERT(v == 420);
	const int& def = m.get(99, -1);
	SAFEMAPTEST_SCT_ASSERT(def == -1);
}

void SafeMapTest::testSafeMapContainerClear()
{
	SafeMap<int, int> m;
	m.add(1, 1);
	m.add(2, 2);
	m.clear();
	SAFEMAPTEST_SCT_ASSERT(m.isEmpty());
}

void SafeMapTest::testSafeMapContainerInsertDuringForeach()
{
	SafeMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);

	{
		int cnt = m.startForeach().size();
		SAFEMAPTEST_SCT_ASSERT(cnt == 2);
		m.add(3, 30);
		m.endForeach();
	}

	SAFEMAPTEST_SCT_ASSERT(m.size() == 3);
	SAFEMAPTEST_SCT_ASSERT(m.contains(3));

	// 再次遍历包含新元素
	{
		int sum = 0;
		for (const auto& pair : m.startForeach())
		{
			sum += pair.second;
		}
		m.endForeach();
		SAFEMAPTEST_SCT_ASSERT(sum == 60);
	}
}

void SafeMapTest::testSafeMapContainerEraseDuringForeach()
{
	SafeMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);

	{
		for (const auto& pair : m.startForeach())
		{
			(void)pair;
		}
		m.remove(2);
		m.endForeach();
	}

	SAFEMAPTEST_SCT_ASSERT(m.size() == 2);
	SAFEMAPTEST_SCT_ASSERT(!m.contains(2));

	{
		bool found2 = false;
		for (const auto& pair : m.startForeach())
		{
			if (pair.first == 2)
			{
				found2 = true;
			}
		}
		m.endForeach();
		SAFEMAPTEST_SCT_ASSERT(!found2);
	}
}

void SafeMapTest::testSafeMapSizeAndEmpty()
{
	SafeMap<int, int> m;
	SAFEMAPTEST_SCT_ASSERT(m.isEmpty());
	m.add(1, 1);
	SAFEMAPTEST_SCT_ASSERT(!m.isEmpty());
	SAFEMAPTEST_SCT_ASSERT(m.size() == 1);
}
#endif
