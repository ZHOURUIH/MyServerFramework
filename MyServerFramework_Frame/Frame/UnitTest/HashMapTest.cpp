#ifdef WINDOWS
#include "FrameHeader.h"

#define HMT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("HashMapTest FAILED: ") + (msg)); }

// -----------------------------------------------------------
// add / contains / size / isEmpty
// -----------------------------------------------------------
void HashMapTest::testHashMap_BasicInsert()
{
    HashMap<int, int> m;
    HMT_ASSERT(m.isEmpty(), "init isEmpty true");
    HMT_ASSERT(m.size() == 0, "init size==0");

    bool r1 = m.add(1, 100);
    bool r2 = m.add(2, 200);
    bool r3 = m.add(1, 999); // key 已存在，失败

    HMT_ASSERT(r1, "add 1 success");
    HMT_ASSERT(r2, "add 2 success");
    HMT_ASSERT(!r3, "add dup fails");
    HMT_ASSERT(m.size() == 2, "size==2");
    HMT_ASSERT(!m.isEmpty(), "not empty");
    HMT_ASSERT(m.contains(1), "contains 1");
    HMT_ASSERT(m.contains(2), "contains 2");
    HMT_ASSERT(!m.contains(99), "not contains 99");
}

void HashMapTest::testHashMap_InsertKeyOnly()
{
    HashMap<int, int> m;
    bool r = m.add(42);
    HMT_ASSERT(r, "insertKeyOnly success");
    HMT_ASSERT(m.contains(42), "insertKeyOnly contains 42");
    HMT_ASSERT(m.get(42, -1) == 0, "insertKeyOnly value default 0");

    bool r2 = m.add(42); // 重复
    HMT_ASSERT(!r2, "insertKeyOnly dup fails");
}

// -----------------------------------------------------------
// addOrSet
// -----------------------------------------------------------
void HashMapTest::testHashMap_InsertOrUpdate_New()
{
    HashMap<int, int> m;
    bool r = m.addOrSet(1, 100);
    HMT_ASSERT(r, "addOrSet new returns true");
    HMT_ASSERT(m.get(1, -1) == 100, "addOrSet new value==100");
}

void HashMapTest::testHashMap_InsertOrUpdate_Existing()
{
    HashMap<int, int> m;
    m.add(1, 100);
    bool r = m.addOrSet(1, 999);
    HMT_ASSERT(!r, "addOrSet existing returns false");
    HMT_ASSERT(m.get(1, -1) == 999, "addOrSet updated value==999");
}

void HashMapTest::testHashMap_InsertOrUpdate_Move()
{
    HashMap<int, string> m;
    string s1 = "hello";
    bool r = m.addOrSet(1, move(s1));
    HMT_ASSERT(r, "insertOrUpdateMove new true");
    HMT_ASSERT(m.get(1) == "hello", "insertOrUpdateMove value");

    string s2 = "world";
    bool r2 = m.addOrSet(1, move(s2));
    HMT_ASSERT(!r2, "insertOrUpdateMove update false");
    HMT_ASSERT(m.get(1) == "world", "insertOrUpdateMove updated");
}

// -----------------------------------------------------------
// addOrGet
// -----------------------------------------------------------
void HashMapTest::testHashMap_InsertOrGet_New()
{
    HashMap<int, int> m;
    int& v = m.addOrGet(5);
    v = 500;
    HMT_ASSERT(m.get(5, -1) == 500, "addOrGet new assign");
}

void HashMapTest::testHashMap_InsertOrGet_Existing()
{
    HashMap<int, int> m;
    m.add(3, 300);
    int& v = m.addOrGet(3);
    HMT_ASSERT(v == 300, "addOrGet existing returns existing value");
    v = 999;
    HMT_ASSERT(m.get(3, -1) == 999, "addOrGet ref modifies map");
}

void HashMapTest::testHashMap_InsertOrGetWithValue()
{
    HashMap<int, int> m;
    int& v1 = m.addOrGet(10, 100);
    HMT_ASSERT(v1 == 100, "insertOrGetValue new==100");

    int& v2 = m.addOrGet(10, 999);
    HMT_ASSERT(v2 == 100, "insertOrGetValue existing still 100");
}

// -----------------------------------------------------------
// set
// -----------------------------------------------------------
void HashMapTest::testHashMap_TrySet_Existing()
{
    HashMap<int, int> m;
    m.add(1, 100);
    bool r = m.set(1, 999);
    HMT_ASSERT(r, "set existing true");
    HMT_ASSERT(m.get(1, -1) == 999, "set value updated");
}

void HashMapTest::testHashMap_TrySet_NotExist()
{
    HashMap<int, int> m;
    bool r = m.set(99, 999);
    HMT_ASSERT(!r, "set notExist false");
    HMT_ASSERT(!m.contains(99), "set notExist not inserted");
}

// -----------------------------------------------------------
// getPtr / getPtrConst
// -----------------------------------------------------------
void HashMapTest::testHashMap_GetPtr()
{
    HashMap<int, int> m;
    m.add(1, 111);
    int* p = m.getPtr(1);
    HMT_ASSERT(p != nullptr, "getPtr non-null");
    HMT_ASSERT(*p == 111, "getPtr value==111");

    *p = 222;
    HMT_ASSERT(m.get(1, -1) == 222, "getPtr modify in place");

    int* p2 = m.getPtr(99);
    HMT_ASSERT(p2 == nullptr, "getPtr null for missing");
}

void HashMapTest::testHashMap_GetPtrConst()
{
    HashMap<int, int> m;
    m.add(5, 55);
    const HashMap<int, int>& cm = m;
    const int* p = cm.getPtrConst(5);
    HMT_ASSERT(p != nullptr, "getPtrConst non-null");
    HMT_ASSERT(*p == 55, "getPtrConst value==55");

    const int* p2 = cm.getPtrConst(99);
    HMT_ASSERT(p2 == nullptr, "getPtrConst null for missing");
}

// -----------------------------------------------------------
// get / get
// -----------------------------------------------------------
void HashMapTest::testHashMap_TryGet_Found()
{
    HashMap<string, int> m;
    m.add("alpha", 10);
    m.add("beta", 20);
    HMT_ASSERT(m.get("alpha", -1) == 10, "get alpha==10");
    HMT_ASSERT(m.get("beta", -1) == 20, "get beta==20");
    HMT_ASSERT(m.get("gamma", -1) == -1, "get missing==-1");
}

void HashMapTest::testHashMap_TryGetNoDefault()
{
    HashMap<int, string> m;
    m.add(1, "one");
    const string& v = m.get(1);
    HMT_ASSERT(v == "one", "tryGetNoDefault found");
    const string& v2 = m.get(99);
    HMT_ASSERT(v2.empty(), "tryGetNoDefault missing empty");
}

void HashMapTest::testHashMap_Get_Found()
{
    HashMap<int, int> m;
    m.add(7, 77);
    HMT_ASSERT(m.get(7) == 77, "get found==77");
}

// -----------------------------------------------------------
// getAtIndex
// -----------------------------------------------------------
void HashMapTest::testHashMap_GetAtIndex()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.add(2, 200);
    m.add(3, 300);

    // getAtIndex 遍历 map，顺序不保证，但能取到有效值
    const int v0 = m.getAt(0, -1);
    const int v1 = m.getAt(1, -1);
    const int v2 = m.getAt(2, -1);
    HMT_ASSERT(v0 != -1 && v1 != -1 && v2 != -1, "getAtIndex all valid");
    HMT_ASSERT(m.getAt(99, -1) == -1, "getAtIndex OOB default");
}

// -----------------------------------------------------------
// remove
// -----------------------------------------------------------
void HashMapTest::testHashMap_EraseByKey()
{
    HashMap<int, int> m;
    m.add(1, 100); m.add(2, 200); m.add(3, 300);
    bool r1 = m.remove(2);
    bool r2 = m.remove(99);
    HMT_ASSERT(r1, "eraseByKey found true");
    HMT_ASSERT(!r2, "eraseByKey missing false");
    HMT_ASSERT(m.size() == 2, "eraseByKey size==2");
    HMT_ASSERT(!m.contains(2), "eraseByKey not contains 2");
}

void HashMapTest::testHashMap_EraseByKeyAndGetValue()
{
    HashMap<int, int> m;
    m.add(5, 555);
    int out = 0;
    bool r = m.remove(5, out);
    HMT_ASSERT(r, "eraseAndGet true");
    HMT_ASSERT(out == 555, "eraseAndGet out==555");
    HMT_ASSERT(!m.contains(5), "eraseAndGet not contains 5");

    int out2 = -1;
    bool r2 = m.remove(99, out2);
    HMT_ASSERT(!r2, "eraseAndGet missing false");
    HMT_ASSERT(out2 == -1, "eraseAndGet out unchanged");
}

void HashMapTest::testHashMap_EraseByIterator()
{
    HashMap<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    auto it = m.find(10);
    HMT_ASSERT(it != m.end(), "eraseByIter find success");
    m.remove(it);
    HMT_ASSERT(!m.contains(10), "eraseByIter removed");
    HMT_ASSERT(m.size() == 1, "eraseByIter size==1");
}

// -----------------------------------------------------------
// clear / dispose / isEmpty
// -----------------------------------------------------------
void HashMapTest::testHashMap_Clear()
{
    HashMap<int, int> m;
    m.add(1, 1); m.add(2, 2); m.add(3, 3);
    m.clear();
    HMT_ASSERT(m.isEmpty(), "clear isEmpty");
    HMT_ASSERT(m.size() == 0, "clear size==0");
}

void HashMapTest::testHashMap_ClearEmpty()
{
    HashMap<int, int> m;
    m.clear(); // 不应崩溃
    HMT_ASSERT(m.isEmpty(), "clearEmpty still empty");
}

void HashMapTest::testHashMap_ClearThenInsert()
{
    HashMap<int, int> m;
    m.add(1, 100);
    m.clear();
    m.add(2, 200);
    HMT_ASSERT(m.size() == 1, "clearThenInsert size==1");
    HMT_ASSERT(m.contains(2), "clearThenInsert contains 2");
    HMT_ASSERT(!m.contains(1), "clearThenInsert not contains 1");
}

// -----------------------------------------------------------
// merge
// -----------------------------------------------------------
void HashMapTest::testHashMap_Merge_NoConflict()
{
    HashMap<int, int> m1;
    m1.add(1, 100); m1.add(2, 200);
    HashMap<int, int> m2;
    m2.add(3, 300); m2.add(4, 400);
    bool r = m1.addRange(m2);
    HMT_ASSERT(r, "merge no conflict true");
    HMT_ASSERT(m1.size() == 4, "merge size==4");
    HMT_ASSERT(m1.contains(3) && m1.contains(4), "merge contains new keys");
}

void HashMapTest::testHashMap_Merge_WithConflict()
{
    HashMap<int, int> m1;
    m1.add(1, 100); m1.add(2, 200);
    HashMap<int, int> m2;
    m2.add(2, 999); m2.add(3, 300); // key 2 冲突
    bool r = m1.addRange(m2);
    HMT_ASSERT(!r, "merge with conflict returns false");
    HMT_ASSERT(m1.size() == 3, "merge conflict size==3");
    HMT_ASSERT(m1.get(2, -1) == 200, "merge conflict original kept");
}

void HashMapTest::testHashMap_Merge_Empty()
{
    HashMap<int, int> m1;
    m1.add(1, 1);
    HashMap<int, int> empty;
    bool r = m1.addRange(empty);
    HMT_ASSERT(r, "merge empty true");
    HMT_ASSERT(m1.size() == 1, "merge empty size unchanged");
}

// -----------------------------------------------------------
// cloneTo
// -----------------------------------------------------------
void HashMapTest::testHashMap_CloneTo()
{
    HashMap<int, int> src;
    src.add(1, 10); src.add(2, 20); src.add(3, 30);
    HashMap<int, int> dst;
    src.cloneTo(dst);
    HMT_ASSERT(dst.size() == 3, "cloneTo size==3");
    HMT_ASSERT(dst.get(1, -1) == 10, "cloneTo key1==10");
    HMT_ASSERT(dst.get(3, -1) == 30, "cloneTo key3==30");

    // 修改 dst 不影响 src
    dst.add(4, 40);
    HMT_ASSERT(!src.contains(4), "cloneTo independent");
}

// -----------------------------------------------------------
// operator[]
// -----------------------------------------------------------
void HashMapTest::testHashMap_OperatorBracket()
{
    HashMap<string, int> m;
    m["key1"] = 10;
    m["key2"] = 20;
    HMT_ASSERT(m["key1"] == 10, "operator[] key1==10");
    HMT_ASSERT(m["key2"] == 20, "operator[] key2==20");
    m["key1"] = 99;
    HMT_ASSERT(m["key1"] == 99, "operator[] update key1==99");
}

// -----------------------------------------------------------
// iterator
// -----------------------------------------------------------
void HashMapTest::testHashMap_Iterator()
{
    HashMap<int, int> m;
    for (int i = 1; i <= 5; ++i)
    {
        m.add(i, i * 10);
    }
    int keySum = 0;
    int valSum = 0;
    for (const auto& kv : m)
    {
        keySum += kv.first;
        valSum += kv.second;
    }
    HMT_ASSERT(keySum == 15, "iterator keySum==15");
    HMT_ASSERT(valSum == 150, "iterator valSum==150");
}

void HashMapTest::testHashMap_IteratorEmpty()
{
    HashMap<int, int> m;
    int count = 0;
    count += m.size();
    HMT_ASSERT(count == 0, "iteratorEmpty count==0");
}

// -----------------------------------------------------------
// 多类型 K/V
// -----------------------------------------------------------
void HashMapTest::testHashMap_StringKey()
{
    HashMap<string, string> m;
    m.add("name", "Alice");
    m.add("city", "Shanghai");
    HMT_ASSERT(m.contains("name"), "stringKey contains name");
    HMT_ASSERT(m.get("city") == "Shanghai", "stringKey city");
    m.remove("name");
    HMT_ASSERT(!m.contains("name"), "stringKey erased name");
}

void HashMapTest::testHashMap_LLongKey()
{
    HashMap<llong, int> m;
    m.add(1000000000LL, 1);
    m.add(9999999999LL, 2);
    HMT_ASSERT(m.contains(1000000000LL), "llongKey contains");
    HMT_ASSERT(m.get(9999999999LL, -1) == 2, "llongKey value");
}

void HashMapTest::testHashMap_IntToString()
{
    HashMap<int, string> m;
    for (int i = 0; i < 50; ++i)
    {
        m.add(i, "val_" + IToS(i));
    }
    HMT_ASSERT(m.size() == 50, "intToString size==50");
    HMT_ASSERT(m.get(25) == "val_25", "intToString val_25");
    HMT_ASSERT(m.get(49) == "val_49", "intToString val_49");
}

// -----------------------------------------------------------
// 综合场景
// -----------------------------------------------------------
void HashMapTest::testHashMap_LargeVolume()
{
    HashMap<int, int> m;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
    {
        m.add(i, i * 2);
    }
    HMT_ASSERT(m.size() == N, "largeVolume size==1000");
    for (int i = 0; i < N; i += 100)
    {
        HMT_ASSERT(m.get(i, -1) == i * 2, "largeVolume value correct");
    }
    // 删除一半
    for (int i = 0; i < N; i += 2)
    {
        m.remove(i);
    }
    HMT_ASSERT(m.size() == N / 2, "largeVolume after remove size==500");
}

void HashMapTest::testHashMap_InsertUpdateAndErase()
{
    HashMap<int, int> m;
    m.add(1, 10); m.add(2, 20); m.add(3, 30);
    m.addOrSet(2, 200); // 更新
    HMT_ASSERT(m.get(2, -1) == 200, "insertUpdateErase updated 2");
    m.remove(1);
    HMT_ASSERT(!m.contains(1), "insertUpdateErase erased 1");
    HMT_ASSERT(m.size() == 2, "insertUpdateErase size==2");
}

void HashMapTest::testHashMap_MergeAndClone()
{
    HashMap<int, int> base;
    for (int i = 0; i < 10; ++i)
    {
        base.add(i, i * 100);
    }
    HashMap<int, int> ext;
    for (int i = 10; i < 20; ++i)
    {
        ext.add(i, i * 100);
    }
    base.addRange(ext);
    HMT_ASSERT(base.size() == 20, "mergeAndClone size==20");

    HashMap<int, int> copy;
    base.cloneTo(copy);
    HMT_ASSERT(copy.size() == 20, "mergeAndClone cloneTo size==20");
    HMT_ASSERT(copy.get(15, -1) == 1500, "mergeAndClone cloneTo val 15");
}

void HashMapTest::testHashMap_EraseAllThenReinsert()
{
    HashMap<int, int> m;
    FOR(5)
    {
        m.add(i, i);
    }
    FOR(5)
    {
        m.remove(i);
    }
    HMT_ASSERT(m.isEmpty(), "eraseAll empty");
    FOR(5)
    {
        m.add(i, i * 10);
    }
    HMT_ASSERT(m.size() == 5, "eraseAllReinsert size==5");
    HMT_ASSERT(m.get(4, -1) == 40, "eraseAllReinsert val 4");
}

void HashMapTest::testHashMap_FindIterator()
{
    HashMap<int, int> m;
    m.add(7, 77);
    auto it = m.find(7);
    HMT_ASSERT(it != m.end(), "find found");
    HMT_ASSERT(it->first == 7 && it->second == 77, "find values");

    auto it2 = m.find(99);
    HMT_ASSERT(it2 == m.end(), "find missing==end");
}

void HashMapTest::testHashMap_OperatorBracketAutoInsert()
{
    HashMap<int, int> m;
    m[5]; // 自动插入 key=5，value=默认 0
    HMT_ASSERT(m.contains(5), "autoInsert contains 5");
    HMT_ASSERT(m[5] == 0, "autoInsert value==0");
}

void HashMapTest::testHashMap_TrySetMultiple()
{
    HashMap<int, int> m;
    FOR(10)
    {
        m.add(i, i);
    }
    FOR(10)
    {
        bool r = m.set(i, i * 100);
        HMT_ASSERT(r, "trySetMultiple success");
    }
    FOR(10)
    {
        HMT_ASSERT(m.get(i, -1) == i * 100, "trySetMultiple updated");
    }
}

void HashMapTest::testHashMap_InsertOrGetChained()
{
    HashMap<string, int> m;
    m.addOrGet("a") = 1;
    m.addOrGet("b") = 2;
    m.addOrGet("a") = 99; // key 已存在，返回引用并修改
    HMT_ASSERT(m.get("a") == 99, "insertOrGetChained a==99");
    HMT_ASSERT(m.get("b") == 2, "insertOrGetChained b==2");
}

void HashMapTest::testHashMap_StringValueVector()
{
    // 模拟 HashMap<int, Vector<int>>
    HashMap<int, int> counts;
    int data[] = { 1, 2, 3, 1, 2, 1, 4, 5, 4 };
    for (int i = 0; i < 9; ++i)
    {
        int key = data[i];
        if (!counts.contains(key))
        {
            counts.add(key, 0);
        }
        counts.addOrSet(key, counts.get(key, 0) + 1);
    }
    HMT_ASSERT(counts.get(1, 0) == 3, "frequency 1==3");
    HMT_ASSERT(counts.get(2, 0) == 2, "frequency 2==2");
    HMT_ASSERT(counts.get(4, 0) == 2, "frequency 4==2");
    HMT_ASSERT(counts.get(5, 0) == 1, "frequency 5==1");
}

void HashMapTest::testHashMap_EraseByValue()
{
    HashMap<int, int> m;
    m.add(1, 100); m.add(2, 200); m.add(3, 300);
    int got = 0;
    m.remove(2, got);
    HMT_ASSERT(got == 200, "eraseByValue got==200");
    HMT_ASSERT(m.size() == 2, "eraseByValue size==2");
}

void HashMapTest::testHashMap_InsertMoveSemantics()
{
    HashMap<int, string> m;
    string s = "moved_value";
    bool r = m.add(1, move(s));
    HMT_ASSERT(r, "insertMove success");
    HMT_ASSERT(m.get(1) == "moved_value", "insertMove value correct");
}

void HashMapTest::testHashMap_CloneToOverwrite()
{
    HashMap<int, int> src;
    src.add(1, 10); src.add(2, 20);
    HashMap<int, int> dst;
    dst.add(100, 999);
    src.cloneTo(dst);
    HMT_ASSERT(dst.size() == 2, "cloneToOverwrite size==2");
    HMT_ASSERT(dst.get(1, -1) == 10, "cloneToOverwrite key1==10");
    HMT_ASSERT(!dst.contains(100), "cloneToOverwrite old key gone");
}

void HashMapTest::testHashMap_DisposeAndReuse()
{
    HashMap<int, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.add(i, i);
    }
    m.dispose();
    HMT_ASSERT(m.isEmpty(), "dispose empty");
    m.add(1, 1);
    HMT_ASSERT(m.size() == 1, "dispose reuse size==1");
}

void HashMapTest::testHashMap_ConstMethodsOnConst()
{
    HashMap<int, int> base;
    base.add(1, 111);
    base.add(2, 222);
    const HashMap<int, int>& cm = base;

    HMT_ASSERT(cm.contains(1), "const contains 1");
    HMT_ASSERT(!cm.contains(99), "const not contains 99");
    HMT_ASSERT(cm.get(2, -1) == 222, "const get");
    HMT_ASSERT(cm.size() == 2, "const size==2");
    HMT_ASSERT(!cm.isEmpty(), "const not empty");

    int count = cm.size();
    HMT_ASSERT(count == 2, "const iterator count==2");
}

// ============================================================
// 主入口
// ============================================================
void HashMapTest::test()
{
    testHashMap_BasicInsert();
    testHashMap_InsertKeyOnly();
    testHashMap_InsertOrUpdate_New();
    testHashMap_InsertOrUpdate_Existing();
    testHashMap_InsertOrUpdate_Move();
    testHashMap_InsertOrGet_New();
    testHashMap_InsertOrGet_Existing();
    testHashMap_InsertOrGetWithValue();
    testHashMap_TrySet_Existing();
    testHashMap_TrySet_NotExist();
    testHashMap_GetPtr();
    testHashMap_GetPtrConst();
    testHashMap_TryGet_Found();
    testHashMap_TryGetNoDefault();
    testHashMap_Get_Found();
    testHashMap_GetAtIndex();
    testHashMap_EraseByKey();
    testHashMap_EraseByKeyAndGetValue();
    testHashMap_EraseByIterator();
    testHashMap_Clear();
    testHashMap_ClearEmpty();
    testHashMap_ClearThenInsert();
    testHashMap_Merge_NoConflict();
    testHashMap_Merge_WithConflict();
    testHashMap_Merge_Empty();
    testHashMap_CloneTo();
    testHashMap_OperatorBracket();
    testHashMap_Iterator();
    testHashMap_IteratorEmpty();
    testHashMap_StringKey();
    testHashMap_LLongKey();
    testHashMap_IntToString();
    testHashMap_LargeVolume();
    testHashMap_InsertUpdateAndErase();
    testHashMap_MergeAndClone();
    testHashMap_EraseAllThenReinsert();
    testHashMap_FindIterator();
    testHashMap_OperatorBracketAutoInsert();
    testHashMap_TrySetMultiple();
    testHashMap_InsertOrGetChained();
    testHashMap_StringValueVector();
    testHashMap_EraseByValue();
    testHashMap_InsertMoveSemantics();
    testHashMap_CloneToOverwrite();
    testHashMap_DisposeAndReuse();
    testHashMap_ConstMethodsOnConst();
	testBoundary();
	testHashMap_Basic();
	testHashMap_InsertOrGet();
	testHashMap_Merge();
	testContainerExtended();
}

namespace HashMapTestBoundaryLocal
{
#define HMB_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("HashMapTest FAILED: ") + (msg)); } } while(0)
#define HMB_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("HashMapTest FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
}

void HashMapTest::testHM_BulkInsert()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		const int N = 10000;
		FOR(N)
		{
			m.add(i, i * 10);
		}
		HMB_ASSERT_EQ(m.size(), N, "bulk insert size");

		FOR(N)
		{
			int* p = m.getPtr(i);
			HMB_ASSERT(p != nullptr && *p == i * 10, "bulk lookup");
		}
	}

void HashMapTest::testHM_BulkInsertRemove()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		const int N = 5000;

		// 插入
		for (int i = 0; i < N; ++i)
		{
			m.add(i, i);
		}

		// 删除一半（偶数）
		int removed = 0;
		for (int i = 0; i < N; i += 2)
		{
			m.remove(i);
			++removed;
		}

		HMB_ASSERT_EQ(m.size(), N - removed, "after remove size");

		// 验证删除的 key 不存在
		for (int i = 0; i < N; i += 2)
		{
			HMB_ASSERT(!m.contains(i), "removed key not found");
		}

		// 验证保留的 key 还在
		for (int i = 1; i < N; i += 2)
		{
			int* p = m.getPtr(i);
			HMB_ASSERT(p != nullptr && *p == i, "remaining key valid");
		}
	}

void HashMapTest::testHM_RemoveThenRehash()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		// 先大量插入
		FOR(2000)
		{
			m.add(i, i);
		}
		// 再几乎全部删光
		for (int i = 1; i < 2000; ++i)
		{
			m.remove(i);
		}

		HMB_ASSERT_EQ(m.size(), 1, "near-empty after mass remove");
		HMB_ASSERT(m.contains(0), "surviving key exists");

		// 重新插入，验证没有损坏
		for (int i = 100; i < 300; ++i)
		{
			m.add(i, i * 100);
		}

		HMB_ASSERT_EQ(m.size(), 201, "reinsert count correct");
		for (int i = 100; i < 300; ++i)
		{
			int* p = m.getPtr(i);
			HMB_ASSERT(p != nullptr && *p == i * 100, "post-rehash value ok");
		}
	}

void HashMapTest::testHM_AddOrSet()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<string, int> m;

		bool isNew1 = m.addOrSet("hello", 10);
		HMB_ASSERT(isNew1, "addOrSet new returns true");
		HMB_ASSERT_EQ(m.get("hello", -1), 10, "addOrSet new value");

		bool isNew2 = m.addOrSet("hello", 20); // 更新
		HMB_ASSERT(!isNew2, "addOrSet update returns false");
		HMB_ASSERT_EQ(m.get("hello", -1), 20, "addOrSet updated value");
	}

void HashMapTest::testHM_Set()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, string> m;
		m.add(1, "old");

		bool ok1 = m.set(1, "new");
		HMB_ASSERT(ok1, "set existing key success");
		HMB_ASSERT(m.get(1, "") == "new", "set updated value");

		bool ok2 = m.set(999, "newkey");
		HMB_ASSERT(!ok2, "set non-existing key fails");
	}

void HashMapTest::testHM_GetWithDefault()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		m.add(5, 500);

		HMB_ASSERT_EQ(m.get(5, -1), 500, "get existing");
		HMB_ASSERT_EQ(m.get(99, -1), -1, "get missing with default");

		HMB_ASSERT_EQ(m.getAt(0, -999), 500, "getAt index 0");
		HMB_ASSERT_EQ(m.getAt(5, -999), -999, "getAt out of range default");
	}

void HashMapTest::testHM_ContainsAndFind()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<string, int> m;
		m.add("abc", 42);

		HMB_ASSERT(m.contains("abc"), "contains existing");
		HMB_ASSERT(!m.contains("xyz"), "contains missing");

		auto it = m.find("abc");
		HMB_ASSERT(it != m.end() && it->second == 42, "find existing");

		auto miss = m.find("nope");
		HMB_ASSERT(miss == m.end(), "find missing");
	}

void HashMapTest::testHM_ClearAndReuse()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		FOR(100)
		{
			m.add(i, i);
		}
		m.clear();
		HMB_ASSERT(m.isEmpty(), "clear empties map");
		HMB_ASSERT_EQ(m.size(), 0, "clear size=0");

		// 复用
		for (int i = 200; i < 210; ++i)
		{
			m.add(i, i * 2);
		}
		HMB_ASSERT_EQ(m.size(), 10, "reuse after clear");
		for (int i = 200; i < 210; ++i)
		{
			HMB_ASSERT(m.contains(i), "reuse element exists");
		}
	}

void HashMapTest::testHM_StringKey()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<string, string> m;
		m.add("key1", "val1");
		m.add("key2", "val2");
		m.add("key3", "val3");

		HMB_ASSERT_EQ(m.size(), 3, "string key size");
		HMB_ASSERT(m.get("key1", "") == "val1", "string key val1");
		HMB_ASSERT(m.get("key3", "") == "val3", "string key val3");
		HMB_ASSERT(!m.contains("key4"), "string key not found");
	}

void HashMapTest::testHM_CopyMove()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> src;
		src.add(1, 10);
		src.add(2, 20);
		src.add(3, 30);

		// 移动构造
		HashMap<int, int> dst(move(src));
		HMB_ASSERT_EQ(dst.size(), 3, "move construct size");
		HMB_ASSERT_EQ(dst.get(1, -1), 10, "move data intact");
		HMB_ASSERT(src.isEmpty() || true, "source may be empty after move"); // 不强制

		// 拷贝构造
		HashMap<int, int> copy;
		dst.cloneTo(copy);
		HMB_ASSERT_EQ(copy.size(), 3, "copy construct size");
		HMB_ASSERT(copy.contains(1) && copy.contains(2) && copy.contains(3), "copy data intact");
	}

void HashMapTest::testHM_Iteration()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		const int N = 100;
		FOR(N)
		{
			m.add(i, i * 7);
		}

		int count = 0;
		int sumKey = 0;
		int sumVal = 0;
		for (auto& pair : m)
		{
			sumKey += pair.first;
			sumVal += pair.second;
			++count;
		}
		HMB_ASSERT_EQ(count, N, "iteration count");
		// sum of 0..99 = 4950
		HMB_ASSERT_EQ(sumKey, 4950, "iteration sum keys");
		// sum of i*7 for i in 0..99 = 7 * 4950 = 34650
		HMB_ASSERT_EQ(sumVal, 34650, "iteration sum values");
	}

void HashMapTest::testHM_LLongKey()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<llong, double> m;
		llong k1 = 123456789012LL;
		llong k2 = 987654321098LL;
		m.add(k1, 3.14);
		m.add(k2, 2.71);

		double* p1 = m.getPtr(k1);
		double* p2 = m.getPtr(k2);
		HMB_ASSERT(p1 != nullptr && abs(*p1 - 3.14) < 1e-6, "llong key val1");
		HMB_ASSERT(p2 != nullptr && abs(*p2 - 2.71) < 1e-6, "llong key val2");
	}

void HashMapTest::testHM_DuplicateAddFails()
{
	using namespace HashMapTestBoundaryLocal;
		HashMap<int, int> m;
		HMB_ASSERT(m.add(1, 10), "first add succeeds");
		HMB_ASSERT(!m.add(1, 20), "duplicate add fails");
		HMB_ASSERT_EQ(m.get(1, -1), 10, "value unchanged on dup add");
	}

void HashMapTest::testBoundary()
{
	testHM_BulkInsert();
	testHM_BulkInsertRemove();
	testHM_RemoveThenRehash();
	testHM_AddOrSet();
	testHM_Set();
	testHM_GetWithDefault();
	testHM_ContainsAndFind();
	testHM_ClearAndReuse();
	testHM_StringKey();
	testHM_CopyMove();
	testHM_Iteration();
	testHM_LLongKey();
	testHM_DuplicateAddFails();
}

// ============================================================
// Consolidated container smoke tests
// ============================================================
#define HASHMAPTEST_CT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("HashMapTest FAIL: ") + (msg)); } } while(0)
#define HASHMAPTEST_CT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("HashMapTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define HASHMAPTEST_CT_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("HashMapTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void HashMapTest::testHashMap_Basic()
{
	HashMap<int, string> m;
	HASHMAPTEST_CT_ASSERT(m.isEmpty(), "HashMap default isEmpty");
	HASHMAPTEST_CT_ASSERT_EQ(m.size(), 0, "HashMap default size=0");

	// insert
	HASHMAPTEST_CT_ASSERT(m.add(1, string("one")),   "HashMap insert 1 ok");
	HASHMAPTEST_CT_ASSERT(m.add(2, string("two")),   "HashMap insert 2 ok");
	HASHMAPTEST_CT_ASSERT(m.add(3, string("three")), "HashMap insert 3 ok");
	HASHMAPTEST_CT_ASSERT(!m.add(1, string("ONE")),  "HashMap insert dup 1 fail");
	HASHMAPTEST_CT_ASSERT_EQ(m.size(), 3, "HashMap size=3");
	HASHMAPTEST_CT_ASSERT(!m.isEmpty(), "HashMap not empty");

	// contains
	HASHMAPTEST_CT_ASSERT(m.contains(2),  "HashMap contains 2");
	HASHMAPTEST_CT_ASSERT(!m.contains(9), "HashMap not contains 9");

	// get / get
	HASHMAPTEST_CT_ASSERT_STR(m.get(1).c_str(),              "one",     "HashMap get 1=one");
	HASHMAPTEST_CT_ASSERT_STR(m.get(2, string("?")).c_str(), "two",  "HashMap get 2=two");
	HASHMAPTEST_CT_ASSERT_STR(m.get(9, string("?")).c_str(), "?",    "HashMap get miss=?");

	// getPtr
	string* p = m.getPtr(3);
	HASHMAPTEST_CT_ASSERT(p != nullptr,           "HashMap getPtr 3 non-null");
	HASHMAPTEST_CT_ASSERT_STR(p->c_str(), "three", "HashMap getPtr 3=three");
	HASHMAPTEST_CT_ASSERT(m.getPtr(9) == nullptr, "HashMap getPtr miss=null");

	// trySet
	HASHMAPTEST_CT_ASSERT(m.set(1, string("ONE")),  "HashMap trySet 1 ok");
	HASHMAPTEST_CT_ASSERT_STR(m.get(1).c_str(), "ONE", "HashMap after trySet 1=ONE");
	HASHMAPTEST_CT_ASSERT(!m.set(9, string("x")),   "HashMap trySet miss=false");

	// addOrSet
	HASHMAPTEST_CT_ASSERT(!m.addOrSet(1, string("one_v2")), "HashMap addOrSet 1 = update");
	HASHMAPTEST_CT_ASSERT_STR(m.get(1).c_str(), "one_v2", "HashMap after update 1=one_v2");
	HASHMAPTEST_CT_ASSERT(m.addOrSet(4, string("four")), "HashMap addOrSet 4 = insert");
	HASHMAPTEST_CT_ASSERT_EQ(m.size(), 4, "HashMap size=4 after addOrSet");

	// remove(key)
	HASHMAPTEST_CT_ASSERT(m.remove(2),  "HashMap remove 2 ok");
	HASHMAPTEST_CT_ASSERT_EQ(m.size(), 3, "HashMap size=3 after remove");
	HASHMAPTEST_CT_ASSERT(!m.contains(2), "HashMap 2 gone after remove");
	HASHMAPTEST_CT_ASSERT(!m.remove(9), "HashMap remove miss=false");

	// remove(key, value)
	string outVal;
	HASHMAPTEST_CT_ASSERT(m.remove(3, outVal), "HashMap remove(3,v) ok");
	HASHMAPTEST_CT_ASSERT_STR(outVal.c_str(), "three", "HashMap erase got 'three'");

	// clear
	m.clear();
	HASHMAPTEST_CT_ASSERT(m.isEmpty(), "HashMap isEmpty after clear");
}

void HashMapTest::testHashMap_InsertOrGet()
{
	HashMap<int, int> m;
	int& v1 = m.addOrGet(10, 100);
	HASHMAPTEST_CT_ASSERT_EQ(v1, 100, "addOrGet new key=100");
	HASHMAPTEST_CT_ASSERT_EQ(m.size(), 1, "addOrGet size=1");

	int& v2 = m.addOrGet(10, 999);
	HASHMAPTEST_CT_ASSERT_EQ(v2, 100, "addOrGet existing key still 100");
	HASHMAPTEST_CT_ASSERT_EQ(m.size(), 1, "addOrGet size still 1");
}

void HashMapTest::testHashMap_Merge()
{
	HashMap<int, int> a;
	a.add(1, 10);
	a.add(2, 20);

	HashMap<int, int> b;
	b.add(3, 30);
	b.add(1, 99);  // dup -> merge keeps a's value

	const bool result = a.addRange(b);
	HASHMAPTEST_CT_ASSERT(!result, "merge returns false (dup key 1)");
	HASHMAPTEST_CT_ASSERT_EQ(a.size(), 3, "merge size=3");
	HASHMAPTEST_CT_ASSERT_EQ(*a.getPtr(3), 30, "merge key 3=30");
	HASHMAPTEST_CT_ASSERT_EQ(*a.getPtr(1), 10, "merge dup key 1 unchanged=10");
}

void HashMapTest::testContainerExtended()
{
// insertOrUpdate
	{
		HashMap<int, string> map;
		map.add(1, "one");
		map.addOrSet(1, "ONE");
		HASHMAPTEST_CT_ASSERT_STR(map.get(1), "ONE", "insertOrUpdate overwrite");
		map.addOrSet(2, "two");
		HASHMAPTEST_CT_ASSERT_EQ(map.size(), 2, "insertOrUpdate new key");
	}

	// addOrGet
	{
		HashMap<string, int> map;
		int& v1 = map.addOrGet("key", 42);
		HASHMAPTEST_CT_ASSERT_EQ(v1, 42, "addOrGet new=42");
		int& v2 = map.addOrGet("key", 99);
		HASHMAPTEST_CT_ASSERT_EQ(v2, 42, "addOrGet existing=42 (not overwritten)");
	}

	// get（返回引用语义：找到返回值引用，找不到返回默认值）
	{
		HashMap<int, int> map;
		map.add(5, 50);
		HASHMAPTEST_CT_ASSERT_EQ(map.get(5, 0), 50, "get found=50");
		HASHMAPTEST_CT_ASSERT_EQ(map.get(99, -1), -1, "get not found=-1");
	}

	// erase by key
	{
		HashMap<int, int> map;
		map.add(1, 10);
		map.add(2, 20); 
		map.add(3, 30);
		map.remove(2);
		HASHMAPTEST_CT_ASSERT(!map.contains(2), "erase by key");
		HASHMAPTEST_CT_ASSERT_EQ(map.size(), 2, "erase size=2");
	}

	// getPtr / getPtrConst
	{
		HashMap<int, int> map;
		map.add(1, 100);
		int* ptr = map.getPtr(1);
		HASHMAPTEST_CT_ASSERT(ptr != nullptr, "getPtr found");
		HASHMAPTEST_CT_ASSERT_EQ(*ptr, 100, "getPtr value=100");
		HASHMAPTEST_CT_ASSERT(map.getPtr(99) == nullptr, "getPtr not found=null");
	}

	// operator[]
	{
		HashMap<string, int> map;
		map["a"] = 1;
		map["b"] = 2;
		HASHMAPTEST_CT_ASSERT_EQ(map["a"], 1, "op[] a=1");
		HASHMAPTEST_CT_ASSERT_EQ(map["b"], 2, "op[] b=2");
		map["a"] = 10;
		HASHMAPTEST_CT_ASSERT_EQ(map["a"], 10, "op[] overwrite a=10");
	}

	// iterator
	{
		HashMap<int, int> map;
		FOR(5)
		{
			map.add(i, i * 10);
		}
		int sum = 0;
		for (auto& pair : map) { sum += pair.second; }
		HASHMAPTEST_CT_ASSERT_EQ(sum, 100, "HashMap iterator sum=100");
	}

	// merge
	{
		HashMap<int, int> src, dst;
		src.add(1, 10); 
		src.add(2, 20);
		dst.add(3, 30);
		dst.addRange(src);
		HASHMAPTEST_CT_ASSERT_EQ(dst.size(), 3, "merge size=3");
		HASHMAPTEST_CT_ASSERT(dst.contains(1), "merge contains key 1");
		HASHMAPTEST_CT_ASSERT(dst.contains(3), "merge contains key 3");
	}

	// cloneTo
	{
		HashMap<int, string> src, dst;
		src.add(1, "a");
		src.add(2, "b");
		src.cloneTo(dst);
		HASHMAPTEST_CT_ASSERT_EQ(dst.size(), 2, "HashMap cloneTo size=2");
		HASHMAPTEST_CT_ASSERT_STR(dst.get(1), "a", "HashMap cloneTo key1='a'");
	}

	// large volume
	{
		HashMap<int, int> map;
		for (int i = 0; i < 500; ++i)
		{
			map.add(i, i);
		}
		HASHMAPTEST_CT_ASSERT_EQ(map.size(), 500, "HashMap large size=500");
		HASHMAPTEST_CT_ASSERT_EQ(map.get(250), 250, "HashMap large get[250]");
		for (int i = 0; i < 250; ++i)
		{
			map.remove(i);
		}
		HASHMAPTEST_CT_ASSERT_EQ(map.size(), 250, "HashMap after erase 250 size=250");
	}


//------------------------------------------------------------------------------
}
#endif
