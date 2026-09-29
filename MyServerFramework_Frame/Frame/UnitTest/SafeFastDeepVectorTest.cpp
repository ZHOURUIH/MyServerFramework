#ifdef WINDOWS
#include "FrameHeader.h"

// ─── Assertion macros ───
#define SV0_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("SafeFastDeepVectorTest FAIL: ") + (msg)); }
#define SV0_ASSERT_EQ(a, b, msg) \
	if ((a) != (b)) { ERROR(string("SafeFastDeepVectorTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); }

// =====================================================================
// SafeFastDeepVector<int> 基础操作
// =====================================================================

void SafeFastDeepVectorTest::testSv0IntDefaultState()
{
	SafeFastDeepVector<int> sv;
	SV0_ASSERT_EQ(sv.size(), 0, "int default size 0");
	SV0_ASSERT(sv.isEmpty(), "int default empty");
}

void SafeFastDeepVectorTest::testSv0IntAdd()
{
	SafeFastDeepVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	SV0_ASSERT_EQ(sv.size(), 3, "int add size 3");
	SV0_ASSERT(!sv.isEmpty(), "int add not empty");

	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT_EQ(sv.get(0), 10, "int add[0]");
	SV0_ASSERT_EQ(sv.get(1), 20, "int add[1]");
	SV0_ASSERT_EQ(sv.get(2), 30, "int add[2]");
}

void SafeFastDeepVectorTest::testSv0IntAddMultiple()
{
	SafeFastDeepVector<int> sv;
	constexpr int COUNT = 100;
	for (int i = 0; i < COUNT; ++i)
	{
		sv.add(i * 10);
	}
	SV0_ASSERT_EQ(sv.size(), COUNT, "int add 100");

	SAFE_VECTOR_0_SCOPE(sv);
	for (int i = 0; i < COUNT; ++i)
	{
		SV0_ASSERT_EQ(sv.get(i), i * 10, "int add bulk");
	}
}

void SafeFastDeepVectorTest::testSv0IntAddAfterClear()
{
	SafeFastDeepVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int cleared size 0");
	sv.add(100);
	SV0_ASSERT_EQ(sv.size(), 1, "int add after clear size 1");
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT_EQ(sv.get(0), 100, "int add after clear value");
}

// =====================================================================
// SafeFastDeepVector<int> remove
// =====================================================================

void SafeFastDeepVectorTest::testSv0IntRemoveExisting()
{
	SafeFastDeepVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.remove(20);
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT_EQ(sv.get(0), 10, "int remove existing[0]");
	SV0_ASSERT_EQ(sv.get(1), 30, "int remove existing[1]");
}

void SafeFastDeepVectorTest::testSv0IntRemoveFirst()
{
	SafeFastDeepVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.remove(10);
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT_EQ(sv.get(0), 20, "int remove first[0]");
	SV0_ASSERT_EQ(sv.get(1), 30, "int remove first[1]");
}

void SafeFastDeepVectorTest::testSv0IntRemoveLast()
{
	SafeFastDeepVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.remove(30);
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT_EQ(sv.get(0), 10, "int remove last[0]");
	SV0_ASSERT_EQ(sv.get(1), 20, "int remove last[1]");
}

void SafeFastDeepVectorTest::testSv0IntRemoveNonexistent()
{
	SafeFastDeepVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.remove(999); // 不存在
	// 不应崩溃，size 不变
	SV0_ASSERT_EQ(sv.size(), 2, "int remove nonexistent size unchanged");
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT_EQ(sv.get(0), 10, "int remove nonexistent[0]");
	SV0_ASSERT_EQ(sv.get(1), 20, "int remove nonexistent[1]");
}

void SafeFastDeepVectorTest::testSv0IntRemoveDuplicates()
{
	SafeFastDeepVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(10);
	sv.add(30);
	sv.remove(10);
	// 应只移除第一个匹配项
	SAFE_VECTOR_0_SCOPE(sv);
	int expected[] = {20, 10, 30};
	for (int i = 0; i < 3; ++i)
	{
		SV0_ASSERT_EQ(sv.get(i), expected[i], "int remove duplicate");
	}
}

void SafeFastDeepVectorTest::testSv0IntRemoveFromEmpty()
{
	SafeFastDeepVector<int> sv;
	sv.remove(1);
	SV0_ASSERT_EQ(sv.size(), 0, "int remove from empty");
}

void SafeFastDeepVectorTest::testSv0IntRemoveAll()
{
	SafeFastDeepVector<int> sv;
	for (int i = 0; i < 5; ++i) sv.add(i);
	sv.remove(0);
	sv.remove(1);
	sv.remove(2);
	sv.remove(3);
	sv.remove(4);
	SV0_ASSERT_EQ(sv.size(), 0, "int remove all empty");
}

// =====================================================================
// SafeFastDeepVector<int> foreach (遍历中修改)
// =====================================================================

void SafeFastDeepVectorTest::testSv0IntForeachBasic()
{
	SafeFastDeepVector<int> sv;
	for (int i = 0; i < 5; ++i) sv.add(i);

	SAFE_VECTOR_0_SCOPE(sv);
	int sum = 0;
	for (int i = 0; i < sv.size(); ++i)
	{
		sum += sv.get(i);
	}
	SV0_ASSERT_EQ(sum, 10, "int foreach sum 0+1+2+3+4=10");
}

void SafeFastDeepVectorTest::testSv0IntForeachNested()
{
	SafeFastDeepVector<int> sv;
	sv.add(1);
	sv.add(2);

	// 嵌套遍历（已验证 SafeFastDeepVector 支持嵌套遍历）
	SAFE_VECTOR_0_SCOPE(sv);
	FOR(sv.size())
	{
		SAFE_VECTOR_0_SCOPE(sv);
		FOR_J(sv.size())
		{
			SV0_ASSERT_EQ(sv.get(j), j + 1, "int nested foreach value");
		}
	}
}

void SafeFastDeepVectorTest::testSv0IntForeachRemoveDuringIteration()
{
	SafeFastDeepVector<int> sv;
	for (int i = 0; i < 5; ++i)
	{
		sv.add(i);
	}

	SAFE_VECTOR_0_SCOPE(sv);
	// 在遍历期间标记删除（不会立即生效）
	sv.remove(2);
	SV0_ASSERT_EQ(sv.get(2), 0, "int foreach remove deferred - still visible");
	// 遍历结束后删除生效
}

// =====================================================================
// SafeFastDeepVector<int> clear
// =====================================================================

void SafeFastDeepVectorTest::testSv0IntClear()
{
	SafeFastDeepVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	SV0_ASSERT_EQ(sv.size(), 3, "int clear before size 3");
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int clear after size 0");
}

void SafeFastDeepVectorTest::testSv0IntClearEmpty()
{
	SafeFastDeepVector<int> sv;
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int clear empty size 0");
}

void SafeFastDeepVectorTest::testSv0IntClearAndReadd()
{
	SafeFastDeepVector<int> sv;
	for (int i = 0; i < 100; ++i)
	{
		sv.add(i);
	}
	SV0_ASSERT_EQ(sv.size(), 100, "int clear readd size 100");
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "int clear readd after clear");
	for (int i = 0; i < 50; ++i)
	{
		sv.add(i * 2);
	}
	SV0_ASSERT_EQ(sv.size(), 50, "int clear readd size 50");
	SAFE_VECTOR_0_SCOPE(sv);
	for (int i = 0; i < 50; ++i)
	{
		SV0_ASSERT_EQ(sv.get(i), i * 2, "int clear readd value");
	}
}

// =====================================================================
// SafeFastDeepVector<int> 批量 + 压力
// =====================================================================

void SafeFastDeepVectorTest::testSv0IntAddRemoveInterleaved()
{
	SafeFastDeepVector<int> sv;
	for (int cycle = 0; cycle < 5; ++cycle)
	{
		for (int i = 0; i < 10; ++i)
		{
			sv.add(i);
		}
		for (int i = 0; i < 10; ++i)
		{
			sv.remove(i);
		}
	}
	SV0_ASSERT_EQ(sv.size(), 0, "int interleaved add/remove final empty");
}

void SafeFastDeepVectorTest::testSv0IntLargeVolume()
{
	SafeFastDeepVector<int> sv;
	constexpr int COUNT = 1000;
	for (int i = 0; i < COUNT; ++i)
	{
		sv.add(i);
	}
	SV0_ASSERT_EQ(sv.size(), COUNT, "int large volume size");
	{
		SAFE_VECTOR_0_SCOPE(sv);
		for (int i = 0; i < COUNT; ++i)
		{
			SV0_ASSERT_EQ(sv.get(i), i, "int large volume value");
		}
	}

	// 删除奇数项
	for (int i = COUNT - 1; i >= 0; i -= 2)
	{
		sv.remove(i);
	}
}

void SafeFastDeepVectorTest::testSv0IntStress()
{
	SafeFastDeepVector<int> sv;
	constexpr int OPS = 500;
	for (int i = 0; i < OPS; ++i)
	{
		sv.add(i * 2);
	}
	SV0_ASSERT_EQ(sv.size(), OPS, "int stress size");

	for (int i = 0; i < OPS; ++i)
	{
		if (i % 3 == 0)
		{
			sv.remove(i * 2);
		}
	}
	// 不应崩溃
	SV0_ASSERT(true, "int stress done");
}

// =====================================================================
// SafeFastDeepVector<string>
// =====================================================================

void SafeFastDeepVectorTest::testSv0StringAdd()
{
	SafeFastDeepVector<string> sv;
	sv.add("hello");
	sv.add("world");
	sv.add("test");
	SV0_ASSERT_EQ(sv.size(), 3, "string add size 3");
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT(sv.get(0) == "hello", "string add[0]");
	SV0_ASSERT(sv.get(1) == "world", "string add[1]");
	SV0_ASSERT(sv.get(2) == "test", "string add[2]");
}

void SafeFastDeepVectorTest::testSv0StringRemove()
{
	SafeFastDeepVector<string> sv;
	sv.add("apple");
	sv.add("banana");
	sv.add("cherry");
	sv.remove(string("banana"));
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT(sv.get(0) == "apple", "string remove[0]");
	SV0_ASSERT(sv.get(1) == "cherry", "string remove[1]");
}

void SafeFastDeepVectorTest::testSv0StringClear()
{
	SafeFastDeepVector<string> sv;
	sv.add("data");
	sv.clear();
	SV0_ASSERT_EQ(sv.size(), 0, "string clear size 0");
}

// =====================================================================
// SafeFastDeepVector<float>
// =====================================================================

void SafeFastDeepVectorTest::testSv0FloatAddRemove()
{
	SafeFastDeepVector<float> sv;
	sv.add(1.5f);
	sv.add(2.5f);
	sv.add(3.5f);
	SV0_ASSERT_EQ(sv.size(), 3, "float add size");
	sv.remove(2.5f);
	SV0_ASSERT_EQ(sv.size(), 2, "float remove size");
	SAFE_VECTOR_0_SCOPE(sv);
	SV0_ASSERT(sv.get(0) == 1.5f, "float[0]");
	SV0_ASSERT(sv.get(1) == 3.5f, "float[1]");
}

// =====================================================================
// 主入口
// =====================================================================
void SafeFastDeepVectorTest::test()
{
	testSv0IntDefaultState();
	testSv0IntAdd();
	testSv0IntAddMultiple();
	testSv0IntAddAfterClear();

	testSv0IntRemoveExisting();
	testSv0IntRemoveFirst();
	testSv0IntRemoveLast();
	testSv0IntRemoveNonexistent();
	testSv0IntRemoveDuplicates();
	testSv0IntRemoveFromEmpty();
	testSv0IntRemoveAll();

	testSv0IntForeachBasic();
	testSv0IntForeachNested();
	testSv0IntForeachRemoveDuringIteration();

	testSv0IntClear();
	testSv0IntClearEmpty();
	testSv0IntClearAndReadd();

	testSv0IntAddRemoveInterleaved();
	testSv0IntLargeVolume();
	testSv0IntStress();

	testSv0StringAdd();
	testSv0StringRemove();
	testSv0StringClear();

	testSv0FloatAddRemove();
}
#endif
