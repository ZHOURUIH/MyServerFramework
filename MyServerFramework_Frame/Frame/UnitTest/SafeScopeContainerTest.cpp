#ifdef WINDOWS
#include "FrameHeader.h"
namespace SafeScopeContainerTest2Support
{
#define SafeScopeContainerTest2Support_SSC2_ASSERT(expr) \
	if (!(expr)) { ERROR(string("SafeScopeContainerTest2 FAIL: ") + #expr); }
}

void SafeScopeContainerTest::testSafeVectorScopeEmpty()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.isEmpty());
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 0);
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 0);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.isForeaching());
}

void SafeScopeContainerTest::testSafeVectorScopeAddDuringForeach()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 3);
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		// Add during foreach
		sv.add(4);
		sv.add(5);
		// MainList now has 5 elements, readList still has 3
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 5);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
	}
	// After scope ends, sync should have happened
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 5);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.isForeaching());
}

void SafeScopeContainerTest::testSafeVectorScopeRemoveDuringForeach()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.add(40);
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 4);
		// Remove one element during foreach
		sv.remove(20);
		// Main list size should be 3 now
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 3);
		// ReadList still has 4 (update deferred)
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.contains(20));
}

void SafeScopeContainerTest::testSafeVectorScopeReadList()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	sv.add(100);
	sv.add(200);
	sv.add(300);
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		int count = 0;
		for (int val : readList)
		{
			SafeScopeContainerTest2Support_SSC2_ASSERT(val == 100 || val == 200 || val == 300);
			++count;
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(count == 3);
	}
}

void SafeScopeContainerTest::testSafeVectorScopeNestedScope()
{
	using namespace SafeScopeContainerTest2Support;
	// SafeVector errors on nested scope — just test single scope works
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	{
		SafeVectorScope<int> scope(sv);
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.isForeaching());
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 2);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.isForeaching());
}

void SafeScopeContainerTest::testSafeVectorScopeMultiAdd()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	for (int i = 0; i < 5; ++i)
	{
		sv.add(i);
	}
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 5);
		// Add multiple during foreach
		for (int i = 5; i < 10; ++i)
		{
			sv.add(i);
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 10);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 5);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 10);
}

void SafeScopeContainerTest::testSafeVectorScopeIntType()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	sv.add(-1);
	sv.add(0);
	sv.add(1);
	sv.add(2147483647);
	sv.add(-2147483647);
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 5);
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 5);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.contains(-1));
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.contains(0));
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.contains(2147483647));
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.isForeaching());
}

void SafeScopeContainerTest::testSafeVectorScopePtrType()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int*> sv;
	int a = 1, b = 2, c = 3;
	sv.add(&a);
	sv.add(&b);
	sv.add(&c);
	{
		SafeVectorScope<int*> scope(sv);
		const Vector<int*>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		int sum = 0;
		for (int* p : readList)
		{
			sum += *p;
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(sum == 6);
	}
	sv.clear();
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.isEmpty());
}

void SafeScopeContainerTest::testSafeVectorScopeModifyBetweenScopes()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	// First scope
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
	}
	// Modify between scopes
	sv.add(4);
	sv.remove(1);
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.contains(1));
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.contains(4));
	// Second scope
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
	}
}

void SafeScopeContainerTest::testSafeVectorScopeSize()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 0);
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.isEmpty());
	for (int i = 0; i < 10; ++i)
	{
		sv.add(i);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 10);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!sv.isEmpty());
	{
		SafeVectorScope<int> scope(sv);
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.isForeaching());
		sv.remove(0);
		sv.remove(9);
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 8);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 8);
	sv.clear();
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 0);
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.isEmpty());
}

void SafeScopeContainerTest::testSafeSetScopeEmpty()
{
	using namespace SafeScopeContainerTest2Support;
	SafeSet<int> ss;
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.isEmpty());
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 0);
	{
		SafeSetScope<int> scope(ss);
		const Set<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 0);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(!ss.isForeaching());
}

void SafeScopeContainerTest::testSafeSetScopeAddDuringForeach()
{
	using namespace SafeScopeContainerTest2Support;
	SafeSet<int> ss;
	ss.add(10);
	ss.add(20);
	ss.add(30);
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 3);
	{
		SafeSetScope<int> scope(ss);
		const Set<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		// Insert during foreach
		ss.add(40);
		ss.add(50);
		SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 5);
		// readList still has 3 during traverse
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 5);
}

void SafeScopeContainerTest::testSafeSetScopeRemoveDuringForeach()
{
	using namespace SafeScopeContainerTest2Support;
	SafeSet<int> ss;
	ss.add(1);
	ss.add(2);
	ss.add(3);
	ss.add(4);
	{
		SafeSetScope<int> scope(ss);
		const Set<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 4);
		ss.remove(2);
		SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 3);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!ss.contains(2));
}

void SafeScopeContainerTest::testSafeSetScopeContains()
{
	using namespace SafeScopeContainerTest2Support;
	SafeSet<int> ss;
	ss.add(100);
	ss.add(200);
	ss.add(300);
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.contains(100));
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.contains(200));
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.contains(300));
	SafeScopeContainerTest2Support_SSC2_ASSERT(!ss.contains(400));
	{
		SafeSetScope<int> scope(ss);
		const Set<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.contains(100));
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.contains(200));
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.contains(300));
	}
}

void SafeScopeContainerTest::testSafeSetScopeMultiple()
{
	using namespace SafeScopeContainerTest2Support;
	SafeSet<int> ss;
	for (int i = 0; i < 20; ++i)
	{
		ss.add(i * 10);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 20);
	{
		SafeSetScope<int> scope(ss);
		const Set<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 20);
		int count = 0;
		for (int val : readList)
		{
			SafeScopeContainerTest2Support_SSC2_ASSERT(val >= 0 && val < 200);
			SafeScopeContainerTest2Support_SSC2_ASSERT(val % 10 == 0);
			++count;
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(count == 20);
	}
	ss.clear();
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.isEmpty());
}

void SafeScopeContainerTest::testSafeHashMapScopeEmpty()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<int, int> shm;
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.isEmpty());
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 0);
	{
		SafeHashMapScope<int, int> scope(shm);
		const HashMap<int, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 0);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(!shm.isForeaching());
}

void SafeScopeContainerTest::testSafeHashMapScopeAddDuringForeach()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<int, string> shm;
	shm.add(1, "one");
	shm.add(2, "two");
	shm.add(3, "three");
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 3);
	{
		SafeHashMapScope<int, string> scope(shm);
		const HashMap<int, string>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		shm.add(4, "four");
		shm.add(5, "five");
		SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 5);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 5);
}

void SafeScopeContainerTest::testSafeHashMapScopeRemoveDuringForeach()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<int, int> shm;
	shm.add(10, 100);
	shm.add(20, 200);
	shm.add(30, 300);
	shm.add(40, 400);
	{
		SafeHashMapScope<int, int> scope(shm);
		const HashMap<int, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 4);
		shm.remove(20);
		SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 3);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!shm.contains(20));
}

void SafeScopeContainerTest::testSafeHashMapScopeReadMap()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<int, int> shm;
	shm.add(1, 10);
	shm.add(2, 20);
	shm.add(3, 30);
	{
		SafeHashMapScope<int, int> scope(shm);
		const HashMap<int, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(1, 0) == 10);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(2, 0) == 20);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(3, 0) == 30);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(99, 0) == 0);
	}
}

void SafeScopeContainerTest::testSafeHashMapScopeInt()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<int, int> shm;
	const int N = 15;
	for (int i = 0; i < N; ++i)
	{
		shm.add(i, i * i);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == N);
	{
		SafeHashMapScope<int, int> scope(shm);
		const HashMap<int, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == N);
		for (int i = 0; i < N; ++i)
		{
			SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(i, -1) == i * i);
		}
	}
	shm.clear();
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.isEmpty());
}

void SafeScopeContainerTest::testSafeHashMapScopeString()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<string, int> shm;
	shm.add("alpha", 1);
	shm.add("beta", 2);
	shm.add("gamma", 3);
	shm.add("delta", 4);
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 4);
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.contains("alpha"));
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.contains("delta"));
	SafeScopeContainerTest2Support_SSC2_ASSERT(!shm.contains("epsilon"));
	{
		SafeHashMapScope<string, int> scope(shm);
		const HashMap<string, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 4);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(string("alpha"), 0) == 1);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(string("beta"), 0) == 2);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(string("gamma"), 0) == 3);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(string("delta"), 0) == 4);
		// Remove during foreach
		shm.remove("beta");
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!shm.contains("beta"));
}

void SafeScopeContainerTest::testSafeVectorScopeComplexType()
{
	using namespace SafeScopeContainerTest2Support;
	struct Point
	{
		int x = 0;
		int y = 0;
		bool operator==(const Point& o) const { return x == o.x && y == o.y; }
	};
	SafeVector<Point> sv;
	sv.add({ 1, 2 });
	sv.add({ 3, 4 });
	sv.add({ 5, 6 });
	{
		SafeVectorScope<Point> scope(sv);
		const Vector<Point>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList[0].x == 1 && readList[0].y == 2);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList[1].x == 3 && readList[1].y == 4);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList[2].x == 5 && readList[2].y == 6);
		// Add during foreach
		sv.add({ 7, 8 });
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 4);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == 4);
}

void SafeScopeContainerTest::testSafeVectorScopeLargeCount()
{
	using namespace SafeScopeContainerTest2Support;
	SafeVector<int> sv;
	const int LARGE = 100;
	for (int i = 0; i < LARGE; ++i)
	{
		sv.add(i);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == LARGE);
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == LARGE);
		int sum = 0;
		for (int val : readList)
		{
			sum += val;
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(sum == (LARGE - 1) * LARGE / 2); // 0+1+...+99 = 4950
		// Remove half during foreach
		for (int i = 0; i < LARGE; i += 2)
		{
			sv.remove(i);
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == LARGE / 2);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(sv.size() == LARGE / 2);
	// Verify only odd numbers remain
	{
		SafeVectorScope<int> scope(sv);
		const Vector<int>& readList = scope.getReadList();
		for (int val : readList)
		{
			SafeScopeContainerTest2Support_SSC2_ASSERT(val % 2 != 0);
		}
	}
}

void SafeScopeContainerTest::testSafeSetScopeModifyAndVerify()
{
	using namespace SafeScopeContainerTest2Support;
	SafeSet<int> ss;
	for (int i = 1; i <= 10; ++i)
	{
		ss.add(i);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 10);
	{
		SafeSetScope<int> scope(ss);
		const Set<int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 10);
		// Remove even numbers during foreach
		for (int i = 2; i <= 10; i += 2)
		{
			ss.remove(i);
		}
		SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 5);
	}
	SafeScopeContainerTest2Support_SSC2_ASSERT(ss.size() == 5);
	// Only odd numbers remain
	for (int i = 1; i <= 10; i += 2)
	{
		SafeScopeContainerTest2Support_SSC2_ASSERT(ss.contains(i));
	}
	for (int i = 2; i <= 10; i += 2)
	{
		SafeScopeContainerTest2Support_SSC2_ASSERT(!ss.contains(i));
	}
}

void SafeScopeContainerTest::testSafeHashMapScopeModifyBetweenScopes()
{
	using namespace SafeScopeContainerTest2Support;
	SafeHashMap<int, int> shm;
	shm.add(1, 10);
	shm.add(2, 20);
	shm.add(3, 30);
	// First scope: verify
	{
		SafeHashMapScope<int, int> scope(shm);
		const HashMap<int, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(1, 0) == 10);
	}
	// Modify between scopes
	shm.add(4, 40);
	shm.remove(2);
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.size() == 3);
	SafeScopeContainerTest2Support_SSC2_ASSERT(!shm.contains(2));
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.contains(4));
	// Second scope: verify modifications
	{
		SafeHashMapScope<int, int> scope(shm);
		const HashMap<int, int>& readList = scope.getReadList();
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.size() == 3);
		SafeScopeContainerTest2Support_SSC2_ASSERT(!readList.contains(2));
		SafeScopeContainerTest2Support_SSC2_ASSERT(readList.get(4, 0) == 40);
	}
	shm.clear();
	SafeScopeContainerTest2Support_SSC2_ASSERT(shm.isEmpty());
}


void SafeScopeContainerTest::test()
{
	testSafeVectorScopeEmpty();
	testSafeVectorScopeAddDuringForeach();
	testSafeVectorScopeRemoveDuringForeach();
	testSafeVectorScopeReadList();
	testSafeVectorScopeNestedScope();
	testSafeVectorScopeMultiAdd();
	testSafeVectorScopeIntType();
	testSafeVectorScopePtrType();
	testSafeVectorScopeModifyBetweenScopes();
	testSafeVectorScopeSize();
	testSafeSetScopeEmpty();
	testSafeSetScopeAddDuringForeach();
	testSafeSetScopeRemoveDuringForeach();
	testSafeSetScopeContains();
	testSafeSetScopeMultiple();
	testSafeHashMapScopeEmpty();
	testSafeHashMapScopeAddDuringForeach();
	testSafeHashMapScopeRemoveDuringForeach();
	testSafeHashMapScopeReadMap();
	testSafeHashMapScopeInt();
	testSafeHashMapScopeString();
	testSafeVectorScopeComplexType();
	testSafeVectorScopeLargeCount();
	testSafeSetScopeModifyAndVerify();
	testSafeHashMapScopeModifyBetweenScopes();
}
#endif
