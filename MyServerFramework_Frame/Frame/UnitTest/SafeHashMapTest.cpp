#ifdef WINDOWS
#include "FrameHeader.h"
namespace SafeHashMapScopeTestSupport
{
#define SafeHashMapScopeTestSupport_SHMS_ASSERT(condition) if (!(condition)) { ERROR("SafeHashMapScopeTest failed: " #condition); }
}

void SafeHashMapTest::testSafeHashMapBasicInsertScope()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, string> m;
	bool r1 = m.add(1, "one");
	bool r2 = m.add(2, "two");
	bool r3 = m.add(3, "three");
	SafeHashMapScopeTestSupport_SHMS_ASSERT(r1);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(r2);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(r3);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 3);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(1));
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(2));
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(3));
}

void SafeHashMapTest::testSafeHashMapBasicEraseScope()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, int> m;
	m.add(10, 100);
	m.add(20, 200);
	m.add(30, 300);
	bool erased = m.remove(20);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(erased);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 2);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.contains(20));
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(10));
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(30));

	bool notErased = m.remove(999);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!notErased);
}

void SafeHashMapTest::testSafeHashMapScopeReadOnly()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);
	int count = 0;
	{
	    SafeHashMapScope<int, int> scope(m);
	    const HashMap<int, int>& readList = scope.getReadList();
	    for (const auto& item : readList)
	    {
	        if (item.first > 0)
	        {
	            count++;
	        }
	    }
	}
	SafeHashMapScopeTestSupport_SHMS_ASSERT(count == 3);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.isForeaching());
}

void SafeHashMapTest::testSafeHashMapInsertDuringForeachScope()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, string> m;
	m.add(1, "one");
	m.add(2, "two");
	{
	    SafeHashMapScope<int, string> scope(m);
	    const HashMap<int, string>& readList = scope.getReadList();
	    int startSize = readList.size();
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(startSize == 2);
	    m.add(3, "three");
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(3));
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 3);
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(readList.size() == 2);
	}
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 3);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains(3));
}

void SafeHashMapTest::testSafeHashMapEraseDuringForeachScope()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, string> m;
	m.add(1, "one");
	m.add(2, "two");
	m.add(3, "three");
	{
	    SafeHashMapScope<int, string> scope(m);
	    const HashMap<int, string>& readList = scope.getReadList();
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(readList.size() == 3);
	    m.remove(2);
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 2);
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.contains(2));
	    SafeHashMapScopeTestSupport_SHMS_ASSERT(readList.size() == 3);
	}
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 2);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.contains(2));
}

void SafeHashMapTest::testSafeHashMapClearAfterForeach()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, int> m;
	m.add(1, 11);
	m.add(2, 22);
	{
	    SafeHashMapScope<int, int> scope(m);
	    const HashMap<int, int>& readList = scope.getReadList();
	    for (const auto& item : readList)
	    {
	        (void)item;
	    }
	}
	m.clear();
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 0);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.isEmpty());
}

void SafeHashMapTest::testSafeHashMapContainsAndGet()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<string, int> m;
	m.add("alpha", 1);
	m.add("beta", 2);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains("alpha"));
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.contains("beta"));
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.contains("gamma"));

	const int& val = m.get("alpha", -1);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(val == 1);

	const int& missing = m.get("gamma", -99);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(missing == -99);
}

void SafeHashMapTest::testSafeHashMapEraseWithValueScope()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, int> m;
	m.add(5, 500);
	m.add(6, 600);
	int val = 0;
	bool erased = m.remove(5, val);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(erased);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(val == 500);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.contains(5));
}

void SafeHashMapTest::testSafeHashMapSizeAndEmptyScope()
{
	using namespace SafeHashMapScopeTestSupport;
	SafeHashMap<int, int> m;
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.isEmpty());
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 0);
	m.add(1, 10);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(!m.isEmpty());
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.size() == 1);
	m.remove(1);
	SafeHashMapScopeTestSupport_SHMS_ASSERT(m.isEmpty());
}


void SafeHashMapTest::test()
{
	testSafeHashMapBasicInsertScope();
	testSafeHashMapBasicEraseScope();
	testSafeHashMapScopeReadOnly();
	testSafeHashMapInsertDuringForeachScope();
	testSafeHashMapEraseDuringForeachScope();
	testSafeHashMapClearAfterForeach();
	testSafeHashMapContainsAndGet();
	testSafeHashMapEraseWithValueScope();
	testSafeHashMapSizeAndEmptyScope();
	testSafeHashMapBasicInsert();
	testSafeHashMapBasicErase();
	testSafeHashMapContains();
	testSafeHashMapTryGet();
	testSafeHashMapGetPtr();
	testSafeHashMapClear();
	testSafeHashMapInsertDuringForeach();
	testSafeHashMapEraseDuringForeach();
	testSafeHashMapInsertAndEraseDuringForeach();
	testSafeHashMapLargeModifyDuringForeach();
	testSafeHashMapForeachEmpty();
	testSafeHashMapSizeAndEmpty();
	testSafeHashMapMainListAccess();
	testSafeHashMapEraseWithValue();
}

// ============================================================
// Consolidated safe container tests
// ============================================================
#define SAFEHASHMAPTEST_SCT_ASSERT(expr) \
	if (!(expr)) { LOG("SafeHashMapTest FAILED: " #expr); }

void SafeHashMapTest::testSafeHashMapBasicInsert()
{
	SafeHashMap<int, int> m;
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 0);
	SAFEHASHMAPTEST_SCT_ASSERT(m.isEmpty());
	SAFEHASHMAPTEST_SCT_ASSERT(m.add(1, 100));
	SAFEHASHMAPTEST_SCT_ASSERT(m.add(2, 200));
	SAFEHASHMAPTEST_SCT_ASSERT(m.add(3, 300));
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 3);
	SAFEHASHMAPTEST_SCT_ASSERT(!m.isEmpty());
	// 重复 key 插入失败
	SAFEHASHMAPTEST_SCT_ASSERT(!m.add(1, 999));
}

void SafeHashMapTest::testSafeHashMapBasicErase()
{
	SafeHashMap<int, int> m;
	m.add(10, 1000);
	m.add(20, 2000);
	m.add(30, 3000);
	SAFEHASHMAPTEST_SCT_ASSERT(m.remove(20));
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 2);
	SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(20));
	// 删除不存在的 key 返回 false
	SAFEHASHMAPTEST_SCT_ASSERT(!m.remove(999));
}

void SafeHashMapTest::testSafeHashMapContains()
{
	SafeHashMap<int, int> m;
	m.add(5, 50);
	SAFEHASHMAPTEST_SCT_ASSERT(m.contains(5));
	SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(6));
}

void SafeHashMapTest::testSafeHashMapTryGet()
{
	SafeHashMap<int, int> m;
	m.add(7, 77);
	const int& v = m.get(7);
	SAFEHASHMAPTEST_SCT_ASSERT(v == 77);
	const int& def = m.get(999, -1);
	SAFEHASHMAPTEST_SCT_ASSERT(def == -1);
}

void SafeHashMapTest::testSafeHashMapGetPtr()
{
	SafeHashMap<int, int> m;
	m.add(42, 420);
	int* p = m.getPtr(42);
	SAFEHASHMAPTEST_SCT_ASSERT(p != nullptr);
	SAFEHASHMAPTEST_SCT_ASSERT(*p == 420);
	int* np = m.getPtr(99);
	SAFEHASHMAPTEST_SCT_ASSERT(np == nullptr);
}

void SafeHashMapTest::testSafeHashMapClear()
{
	SafeHashMap<int, int> m;
	m.add(1, 1);
	m.add(2, 2);
	m.clear();
	SAFEHASHMAPTEST_SCT_ASSERT(m.isEmpty());
	SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(1));
}

void SafeHashMapTest::testSafeHashMapInsertDuringForeach()
{
	SafeHashMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);

	// 遍历期间插入 key=4
	{
		const HashMap<int, int>& readList = m.startForeach();
		SAFEHASHMAPTEST_SCT_ASSERT(m.isForeaching());
		int sum = 0;
		for (const auto& pair : readList)
		{
			sum += pair.second;
		}
		SAFEHASHMAPTEST_SCT_ASSERT(sum == 60);
		// 遍历中插入（会记录到 mModifyList，不立即同步到 readList）
		m.add(4, 40);
		SAFEHASHMAPTEST_SCT_ASSERT(m.contains(4));   // mainList 已包含
		m.endForeach();
	}

	// endForeach 后 mUpdateList 已同步
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 4);
	SAFEHASHMAPTEST_SCT_ASSERT(m.contains(4));

	// 再次遍历，新元素出现在 readList
	{
		const HashMap<int, int>& readList = m.startForeach();
		int sum = 0;
		for (const auto& pair : readList)
		{
			sum += pair.second;
		}
		m.endForeach();
		SAFEHASHMAPTEST_SCT_ASSERT(sum == 100);
	}
}

void SafeHashMapTest::testSafeHashMapEraseDuringForeach()
{
	SafeHashMap<int, int> m;
	m.add(1, 10);
	m.add(2, 20);
	m.add(3, 30);

	{
		const HashMap<int, int>& readList = m.startForeach();
		int cnt = readList.size();
		SAFEHASHMAPTEST_SCT_ASSERT(cnt == 3);
		// 遍历中删除 key=2
		m.remove(2);
		SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(2));  // mainList 已删除
		m.endForeach();
	}

	// endForeach 后同步
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 2);
	SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(2));

	// 再次遍历，readList 里没有 key=2
	{
		const HashMap<int, int>& readList = m.startForeach();
		bool found2 = false;
		for (const auto& pair : readList)
		{
			if (pair.first == 2)
			{
				found2 = true;
			}
		}
		m.endForeach();
		SAFEHASHMAPTEST_SCT_ASSERT(!found2);
	}
}

void SafeHashMapTest::testSafeHashMapInsertAndEraseDuringForeach()
{
	SafeHashMap<int, int> m;
	for (int i = 1; i <= 5; ++i)
	{
		m.add(i, i * 10);
	}

	{
		const HashMap<int, int>& readList = m.startForeach();
		for (const auto& pair : readList)
		{
			(void)pair;
		}
		m.add(6, 60);
		m.remove(3);
		m.endForeach();
	}

	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 5);
	SAFEHASHMAPTEST_SCT_ASSERT(m.contains(6));
	SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(3));
}

void SafeHashMapTest::testSafeHashMapLargeModifyDuringForeach()
{
	// 修改数量 >= mainCount 时走 cloneTo 分支
	SafeHashMap<int, int> m;
	for (int i = 0; i < 5; ++i)
	{
		m.add(i, i);
	}

	{
		const HashMap<int, int>& readList = m.startForeach();
		for (const auto& pair : readList)
		{
			(void)pair;
		}
		// 修改5次（等于 mainCount=5），触发 cloneTo 分支
		for (int i = 10; i < 15; ++i)
		{
			m.add(i, i);
		}
		m.endForeach();
	}

	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 10);
	for (int i = 10; i < 15; ++i)
	{
		SAFEHASHMAPTEST_SCT_ASSERT(m.contains(i));
	}
}

void SafeHashMapTest::testSafeHashMapForeachEmpty()
{
	SafeHashMap<int, int> m;

	{
		int cnt = m.startForeach().size();
		SAFEHASHMAPTEST_SCT_ASSERT(cnt == 0);
		m.add(99, 990);
		m.endForeach();
	}

	// mainList 清空后同步：mUpdateList.clear()
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 1);

	// 再遍历应看到 99
	{
		int sum = 0;
		for (const auto& pair : m.startForeach())
		{
			sum += pair.second;
		}
		m.endForeach();
		SAFEHASHMAPTEST_SCT_ASSERT(sum == 990);
	}
}

void SafeHashMapTest::testSafeHashMapSizeAndEmpty()
{
	SafeHashMap<int, int> m;
	SAFEHASHMAPTEST_SCT_ASSERT(m.isEmpty());
	m.add(1, 1);
	SAFEHASHMAPTEST_SCT_ASSERT(!m.isEmpty());
	SAFEHASHMAPTEST_SCT_ASSERT(m.size() == 1);
	m.remove(1);
	SAFEHASHMAPTEST_SCT_ASSERT(m.isEmpty());
}

void SafeHashMapTest::testSafeHashMapMainListAccess()
{
	SafeHashMap<int, int> m;
	m.add(100, 1000);
	const HashMap<int, int>& ml = m.getMainList();
	SAFEHASHMAPTEST_SCT_ASSERT(ml.contains(100));
	const int* vp = ml.getPtrConst(100);
	SAFEHASHMAPTEST_SCT_ASSERT(vp != nullptr);
	SAFEHASHMAPTEST_SCT_ASSERT(*vp == 1000);
}

void SafeHashMapTest::testSafeHashMapEraseWithValue()
{
	SafeHashMap<int, int> m;
	m.add(7, 70);
	int outVal = 0;
	SAFEHASHMAPTEST_SCT_ASSERT(m.remove(7, outVal));
	SAFEHASHMAPTEST_SCT_ASSERT(outVal == 70);
	SAFEHASHMAPTEST_SCT_ASSERT(!m.contains(7));
}
#endif
