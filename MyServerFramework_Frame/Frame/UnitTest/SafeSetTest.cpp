#ifdef WINDOWS
#include "FrameHeader.h"

void SafeSetTest::test()
{
	testSafeSetBasicInsert();
	testSafeSetBasicErase();
	testSafeSetContains();
	testSafeSetClear();
	testSafeSetInsertDuringForeach();
	testSafeSetEraseDuringForeach();
	testSafeSetSizeAndEmpty();
	testSafeSetInsertAndEraseDuringForeach();
	testSafeSetScopeBasicInsert();
	testSafeSetScopeBasicErase();
	testSafeSetScopeReadOnly();
	testSafeSetScopeInsertDuringForeach();
	testSafeSetScopeEraseDuringForeach();
	testSafeSetScopeClearAfterForeach();
	testSafeSetScopeContains();
	testSafeSetScopeSizeAndEmpty();
}

// ============================================================
// Consolidated safe container tests
// ============================================================
#define SAFESETTEST_SCT_ASSERT(expr) \
	if (!(expr)) { LOG("SafeSetTest FAILED: " #expr); }

void SafeSetTest::testSafeSetBasicInsert()
{
	SafeSet<int> s;
	SAFESETTEST_SCT_ASSERT(s.isEmpty());
	SAFESETTEST_SCT_ASSERT(s.add(10));
	SAFESETTEST_SCT_ASSERT(s.add(20));
	SAFESETTEST_SCT_ASSERT(s.add(30));
	SAFESETTEST_SCT_ASSERT(s.size() == 3);
	// 重复插入失败
	SAFESETTEST_SCT_ASSERT(!s.add(10));
}

void SafeSetTest::testSafeSetBasicErase()
{
	SafeSet<int> s;
	s.add(5);
	s.add(15);
	SAFESETTEST_SCT_ASSERT(s.remove(5));
	SAFESETTEST_SCT_ASSERT(!s.contains(5));
	SAFESETTEST_SCT_ASSERT(s.size() == 1);
	SAFESETTEST_SCT_ASSERT(!s.remove(999));
}

void SafeSetTest::testSafeSetContains()
{
	SafeSet<string> s;
	s.add("abc");
	SAFESETTEST_SCT_ASSERT(s.contains("abc"));
	SAFESETTEST_SCT_ASSERT(!s.contains("xyz"));
}

void SafeSetTest::testSafeSetClear()
{
	SafeSet<int> s;
	s.add(1);
	s.add(2);
	s.add(3);
	s.clear();
	SAFESETTEST_SCT_ASSERT(s.isEmpty());
}

void SafeSetTest::testSafeSetInsertDuringForeach()
{
	SafeSet<int> s;
	s.add(1);
	s.add(2);
	s.add(3);

	{
		const Set<int>& readList = s.startForeach();
		int sum = 0;
		for (int v : readList)
		{
			sum += v;
		}
		SAFESETTEST_SCT_ASSERT(sum == 6);
		// 遍历期间插入 4
		s.add(4);
		SAFESETTEST_SCT_ASSERT(s.contains(4));
		s.endForeach();
	}

	SAFESETTEST_SCT_ASSERT(s.size() == 4);

	// 再次遍历包含 4
	{
		const Set<int>& readList = s.startForeach();
		int sum = 0;
		for (int v : readList)
		{
			sum += v;
		}
		s.endForeach();
		SAFESETTEST_SCT_ASSERT(sum == 10);
	}
}

void SafeSetTest::testSafeSetEraseDuringForeach()
{
	SafeSet<int> s;
	s.add(10);
	s.add(20);
	s.add(30);

	{
		const Set<int>& readList = s.startForeach();
		for (int v : readList)
		{
			(void)v;
		}
		s.remove(20);
		s.endForeach();
	}

	SAFESETTEST_SCT_ASSERT(s.size() == 2);
	SAFESETTEST_SCT_ASSERT(!s.contains(20));

	{
		const Set<int>& readList = s.startForeach();
		bool found20 = false;
		for (int v : readList)
		{
			if (v == 20)
			{
				found20 = true;
			}
		}
		s.endForeach();
		SAFESETTEST_SCT_ASSERT(!found20);
	}
}

void SafeSetTest::testSafeSetSizeAndEmpty()
{
	SafeSet<int> s;
	SAFESETTEST_SCT_ASSERT(s.isEmpty());
	s.add(99);
	SAFESETTEST_SCT_ASSERT(!s.isEmpty());
	SAFESETTEST_SCT_ASSERT(s.size() == 1);
	s.remove(99);
	SAFESETTEST_SCT_ASSERT(s.isEmpty());
}

void SafeSetTest::testSafeSetInsertAndEraseDuringForeach()
{
	SafeSet<int> s;
	for (int i = 1; i <= 6; ++i)
	{
		s.add(i);
	}

	{
		const Set<int>& readList = s.startForeach();
		for (int v : readList)
		{
			(void)v;
		}
		s.add(7);
		s.add(8);
		s.remove(3);
		s.remove(4);
		s.endForeach();
	}

	SAFESETTEST_SCT_ASSERT(s.size() == 6); // 6 - 2 + 2 = 6
	SAFESETTEST_SCT_ASSERT(s.contains(7));
	SAFESETTEST_SCT_ASSERT(s.contains(8));
	SAFESETTEST_SCT_ASSERT(!s.contains(3));
	SAFESETTEST_SCT_ASSERT(!s.contains(4));
}


// ============================================================
// SafeSetScope tests consolidated from SafeHashMap scope tests
// ============================================================

void SafeSetTest::testSafeSetScopeBasicInsert()
{
    SafeSet<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    SAFESETTEST_SCT_ASSERT(s.size() == 3);
    SAFESETTEST_SCT_ASSERT(s.contains(10));
    SAFESETTEST_SCT_ASSERT(s.contains(20));
    SAFESETTEST_SCT_ASSERT(s.contains(30));
    SAFESETTEST_SCT_ASSERT(!s.contains(99));
}

void SafeSetTest::testSafeSetScopeBasicErase()
{
    SafeSet<int> s;
    s.add(1);
    s.add(2);
    s.add(3);
    bool erased = s.remove(2);
    SAFESETTEST_SCT_ASSERT(erased);
    SAFESETTEST_SCT_ASSERT(s.size() == 2);
    SAFESETTEST_SCT_ASSERT(!s.contains(2));

    bool notErased = s.remove(999);
    SAFESETTEST_SCT_ASSERT(!notErased);
}

void SafeSetTest::testSafeSetScopeReadOnly()
{
    SafeSet<string> s;
    s.add("a");
    s.add("b");
    s.add("c");
    int count = 0;
    {
        SafeSetScope<string> scope(s);
        const Set<string>& readSet = scope.getReadList();
        for (const auto& item : readSet)
        {
            if (item != "")
            {
                count++;
            }
        }
    }
    SAFESETTEST_SCT_ASSERT(count == 3);
    SAFESETTEST_SCT_ASSERT(!s.isForeaching());
}

void SafeSetTest::testSafeSetScopeInsertDuringForeach()
{
    SafeSet<int> s;
    s.add(1);
    s.add(2);
    {
        SafeSetScope<int> scope(s);
        const Set<int>& readSet = scope.getReadList();
        SAFESETTEST_SCT_ASSERT(readSet.size() == 2);
        s.add(3);
        SAFESETTEST_SCT_ASSERT(s.contains(3));
        SAFESETTEST_SCT_ASSERT(s.size() == 3);
        SAFESETTEST_SCT_ASSERT(readSet.size() == 2);
    }
    SAFESETTEST_SCT_ASSERT(s.size() == 3);
}

void SafeSetTest::testSafeSetScopeEraseDuringForeach()
{
    SafeSet<int> s;
    s.add(10);
    s.add(20);
    s.add(30);
    {
        SafeSetScope<int> scope(s);
        const Set<int>& readSet = scope.getReadList();
        SAFESETTEST_SCT_ASSERT(readSet.size() == 3);
        s.remove(20);
        SAFESETTEST_SCT_ASSERT(!s.contains(20));
        SAFESETTEST_SCT_ASSERT(s.size() == 2);
        SAFESETTEST_SCT_ASSERT(readSet.size() == 3);
    }
    SAFESETTEST_SCT_ASSERT(s.size() == 2);
    SAFESETTEST_SCT_ASSERT(!s.contains(20));
}

void SafeSetTest::testSafeSetScopeClearAfterForeach()
{
    SafeSet<int> s;
    s.add(100);
    s.add(200);
    {
        SafeSetScope<int> scope(s);
        const Set<int>& readSet = scope.getReadList();
        for (const int val : readSet)
        {
            (void)val;
        }
    }
    s.clear();
    SAFESETTEST_SCT_ASSERT(s.size() == 0);
    SAFESETTEST_SCT_ASSERT(s.isEmpty());
}

void SafeSetTest::testSafeSetScopeContains()
{
    SafeSet<string> s;
    s.add("foo");
    s.add("bar");
    SAFESETTEST_SCT_ASSERT(s.contains("foo"));
    SAFESETTEST_SCT_ASSERT(s.contains("bar"));
    SAFESETTEST_SCT_ASSERT(!s.contains("baz"));
}

void SafeSetTest::testSafeSetScopeSizeAndEmpty()
{
    SafeSet<int> s;
    SAFESETTEST_SCT_ASSERT(s.isEmpty());
    SAFESETTEST_SCT_ASSERT(s.size() == 0);
    s.add(42);
    SAFESETTEST_SCT_ASSERT(!s.isEmpty());
    SAFESETTEST_SCT_ASSERT(s.size() == 1);
    s.remove(42);
    SAFESETTEST_SCT_ASSERT(s.isEmpty());
}
#endif
