#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// MapTest — Map<K,V> 全方法深度测试
// 注意：ContainerTest 只覆盖了 Map 的基础操作（add/[]/get/remove/clear/iterate）
// 本文件覆盖所有未测试的方法：
//   addOrSet / addOrGet / set / addRange / cloneTo / dispose /
//   getPtrConst / find(iterator) / rbegin/rend / remove(iterator) / remove(key,value) /
//   add(key-only) / move-ctor / move-assign / initializer-list ctor /
//   copy-assign / getAtIndex(含边界) / clear(disposeMemory=true)
//   大量场景组合测试
// ============================================================

#define MAP_ET_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("MapTest FAILED: ") + (msg)); }
#define MET_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("MapTest FAILED: ") + (msg) + " | got=" + IToS((int)(a))); }

// -----------------------------------------------------------
// addOrSet — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_InsertOrUpdate_New()
{
    Map<int, int> m;
    bool r = m.addOrSet(1, 100);
    MAP_ET_ASSERT(r, "addOrSet new returns true");
    MET_EQ(m.get(1, -1), 100, "addOrSet new value==100");
}

void MapTest::testMap_InsertOrUpdate_Existing()
{
    Map<int, int> m;
    m.add(1, 100);
    bool r = m.addOrSet(1, 999);
    MAP_ET_ASSERT(!r, "addOrSet existing returns false");
    MET_EQ(m.get(1, -1), 999, "addOrSet existing updated value==999");
}

void MapTest::testMap_InsertOrUpdate_Move()
{
    Map<int, string> m;
    string s1 = "hello";
    bool r = m.addOrSet(1, move(s1));
    MAP_ET_ASSERT(r, "addOrSet move new returns true");
    MAP_ET_ASSERT(m.get(1) == "hello", "addOrSet move value==hello");
    string s2 = "world";
    bool r2 = m.addOrSet(1, move(s2));
    MAP_ET_ASSERT(!r2, "addOrSet move update returns false");
    MAP_ET_ASSERT(m.get(1) == "world", "addOrSet move updated==world");
}

void MapTest::testMap_InsertOrUpdate_Multiple()
{
    Map<int, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.addOrSet(i, i * 10);
    }
    MET_EQ(m.size(), 100, "addOrSet loop size==100");
    for (int i = 0; i < 100; ++i)
    {
        m.addOrSet(i, i * 100);
    }
    MET_EQ(m.size(), 100, "addOrSet update loop size unchanged");
    for (int i = 0; i < 100; ++i)
    {
        MET_EQ(m.get(i, -1), i * 100, "addOrSet loop updated value");
    }
}

void MapTest::testMap_InsertOrUpdate_StringKey()
{
    Map<string, int> m;
    bool r1 = m.addOrSet("alpha", 1);
    bool r2 = m.addOrSet("beta", 2);
    bool r3 = m.addOrSet("alpha", 99);
    MAP_ET_ASSERT(r1 && r2, "addOrSet str new true");
    MAP_ET_ASSERT(!r3, "addOrSet str existing false");
    MET_EQ(m.get("alpha", -1), 99, "addOrSet str updated");
    MET_EQ(m.get("beta", -1), 2, "addOrSet str beta unchanged");
}

// -----------------------------------------------------------
// addOrGet — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_InsertOrGet_New()
{
    Map<int, int> m;
    int& v = m.addOrGet(5);
    v = 500;
    MET_EQ(m.get(5, -1), 500, "addOrGet new assign");
}

void MapTest::testMap_InsertOrGet_Existing()
{
    Map<int, int> m;
    m.add(3, 300);
    int& v = m.addOrGet(3);
    MET_EQ(v, 300, "addOrGet existing returns existing value");
    v = 999;
    MET_EQ(m.get(3, -1), 999, "addOrGet ref modifies map");
}

void MapTest::testMap_InsertOrGet_Chain()
{
    Map<string, int> m;
    m.addOrGet("a") = 1;
    m.addOrGet("b") = 2;
    m.addOrGet("a") = 99;
    MET_EQ(m.get("a", -1), 99, "addOrGet chain a==99");
    MET_EQ(m.get("b", -1), 2, "addOrGet chain b==2");
}

void MapTest::testMap_InsertOrGet_LargeCount()
{
    Map<int, int> m;
    for (int i = 0; i < 200; ++i)
    {
        m.addOrGet(i) = i * 3;
    }
    MET_EQ(m.size(), 200, "addOrGet large size==200");
    for (int i = 0; i < 200; ++i)
    {
        MET_EQ(m.get(i, -1), i * 3, "addOrGet large value correct");
    }
}

// -----------------------------------------------------------
// set — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_TrySet_Existing()
{
    Map<int, int> m;
    m.add(1, 100);
    bool r = m.set(1, 999);
    MAP_ET_ASSERT(r, "set existing true");
    MET_EQ(m.get(1, -1), 999, "set value updated");
}

void MapTest::testMap_TrySet_NotExist()
{
    Map<int, int> m;
    bool r = m.set(99, 999);
    MAP_ET_ASSERT(!r, "set notExist false");
    MAP_ET_ASSERT(!m.contains(99), "set notExist not inserted");
}

void MapTest::testMap_TrySet_Multiple()
{
    Map<int, int> m;
    for (int i = 0; i < 50; ++i)
    {
        m.add(i, i);
    }
    for (int i = 0; i < 50; ++i)
    {
        bool r = m.set(i, i * 10);
        MAP_ET_ASSERT(r, "set multiple success");
    }
    for (int i = 0; i < 50; ++i)
    {
        MET_EQ(m.get(i, -1), i * 10, "set multiple updated");
    }
}

void MapTest::testMap_TrySet_StringKey()
{
    Map<string, string> m;
    m.add("name", "Alice");
    bool r1 = m.set("name", "Bob");
    bool r2 = m.set("city", "Shanghai");
    MAP_ET_ASSERT(r1, "set str existing true");
    MAP_ET_ASSERT(!r2, "set str missing false");
    MAP_ET_ASSERT(m.get("name") == "Bob", "set str value updated");
    MAP_ET_ASSERT(!m.contains("city"), "set str not inserted");
}

// -----------------------------------------------------------
// getPtrConst — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_GetPtrConst_Found()
{
    Map<int, int> m;
    m.add(7, 77);
    const Map<int, int>& cm = m;
    const int* p = cm.getPtrConst(7);
    MAP_ET_ASSERT(p != nullptr, "getPtrConst found non-null");
    MET_EQ(*p, 77, "getPtrConst value==77");
}

void MapTest::testMap_GetPtrConst_Missing()
{
    Map<int, int> m;
    m.add(1, 10);
    const Map<int, int>& cm = m;
    const int* p = cm.getPtrConst(99);
    MAP_ET_ASSERT(p == nullptr, "getPtrConst missing null");
}

void MapTest::testMap_GetPtrConst_ModifyViaGetPtr()
{
    Map<int, int> m;
    m.add(5, 50);
    int* p = m.getPtr(5);
    MAP_ET_ASSERT(p != nullptr, "getPtr non-null");
    *p = 500;
    MET_EQ(m.get(5, -1), 500, "getPtr modify in-place");
    const int* cp = m.getPtrConst(5);
    MAP_ET_ASSERT(cp != nullptr && *cp == 500, "getPtrConst after modify");
}

// -----------------------------------------------------------
// find(iterator) + remove(iterator) — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_FindIterator()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    auto it = m.find(2);
    MAP_ET_ASSERT(it != m.end(), "find(2) found");
    MET_EQ(it->first, 2, "find(2) key==2");
    MET_EQ(it->second, 20, "find(2) value==20");
    auto it2 = m.find(99);
    MAP_ET_ASSERT(it2 == m.end(), "find(99)==end");
}

void MapTest::testMap_FindIteratorConst()
{
    Map<int, int> m;
    m.add(10, 100);
    const Map<int, int>& cm = m;
    auto it = cm.find(10);
    MAP_ET_ASSERT(it != cm.end(), "const find found");
    MET_EQ(it->second, 100, "const find value==100");
}

void MapTest::testMap_EraseByIterator()
{
    Map<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    m.add(30, 300);
    auto it = m.find(20);
    MAP_ET_ASSERT(it != m.end(), "find before remove");
    m.remove(it);
    MAP_ET_ASSERT(!m.contains(20), "eraseIter removed");
    MET_EQ(m.size(), 2, "eraseIter size==2");
    MAP_ET_ASSERT(m.contains(10), "eraseIter 10 still there");
    MAP_ET_ASSERT(m.contains(30), "eraseIter 30 still there");
}

void MapTest::testMap_EraseByIterator_AllElements()
{
    Map<int, int> m;
    for (int i = 0; i < 10; ++i)
    {
        m.add(i, i * 100);
    }
    auto it = m.begin();
    while (it != m.end())
    {
        it = m.remove(it);
    }
    MAP_ET_ASSERT(m.isEmpty(), "eraseIter all isEmpty");
}

// -----------------------------------------------------------
// remove(key, value) — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_EraseGetValue()
{
    Map<int, int> m;
    m.add(5, 555);
    int out = 0;
    bool r = m.remove(5, out);
    MAP_ET_ASSERT(r, "eraseGetValue true");
    MET_EQ(out, 555, "eraseGetValue out==555");
    MAP_ET_ASSERT(!m.contains(5), "eraseGetValue not contains 5");
}

void MapTest::testMap_EraseGetValue_Missing()
{
    Map<int, int> m;
    m.add(1, 10);
    int out = -1;
    bool r = m.remove(99, out);
    MAP_ET_ASSERT(!r, "eraseGetValue missing false");
    MET_EQ(out, -1, "eraseGetValue out unchanged");
    MET_EQ(m.size(), 1, "eraseGetValue size unchanged");
}

void MapTest::testMap_EraseGetValue_StringValue()
{
    Map<int, string> m;
    m.add(1, "one");
    m.add(2, "two");
    string got;
    bool r = m.remove(1, got);
    MAP_ET_ASSERT(r, "eraseGetValue string true");
    MAP_ET_ASSERT(got == "one", "eraseGetValue got==one");
    MAP_ET_ASSERT(!m.contains(1), "eraseGetValue 1 gone");
    MET_EQ(m.size(), 1, "eraseGetValue size==1");
}

// -----------------------------------------------------------
// add(key-only) — 未测试
// -----------------------------------------------------------
void MapTest::testMap_InsertKeyOnly()
{
    Map<int, int> m;
    bool r = m.add(42);
    MAP_ET_ASSERT(r, "insertKeyOnly success");
    MAP_ET_ASSERT(m.contains(42), "insertKeyOnly contains 42");
    MET_EQ(m.get(42, -1), 0, "insertKeyOnly default value==0");
    bool r2 = m.add(42);
    MAP_ET_ASSERT(!r2, "insertKeyOnly dup fails");
}

void MapTest::testMap_InsertKeyOnly_StringValue()
{
    Map<int, string> m;
    bool r = m.add(7);
    MAP_ET_ASSERT(r, "insertKeyOnlyStr success");
    MAP_ET_ASSERT(m.get(7).empty(), "insertKeyOnlyStr value empty");
}

// -----------------------------------------------------------
// add(key, value&&) move semantics
// -----------------------------------------------------------
void MapTest::testMap_InsertMoveValue()
{
    Map<int, string> m;
    string s = "moved_string";
    bool r = m.add(1, move(s));
    MAP_ET_ASSERT(r, "insertMove success");
    MAP_ET_ASSERT(m.get(1) == "moved_string", "insertMove value correct");
}

// -----------------------------------------------------------
// addRange — ContainerTest 有但不完整，本文件测试更多场景
// -----------------------------------------------------------
void MapTest::testMap_Merge_NoConflict()
{
    Map<int, int> m1;
    m1.add(1, 100);
    m1.add(2, 200);
    Map<int, int> m2;
    m2.add(3, 300);
    m2.add(4, 400);
    bool r = m1.addRange(m2);
    MAP_ET_ASSERT(r, "addRange no conflict true");
    MET_EQ(m1.size(), 4, "addRange size==4");
    MET_EQ(m1.get(3, -1), 300, "addRange key3==300");
    MET_EQ(m1.get(4, -1), 400, "addRange key4==400");
}

void MapTest::testMap_Merge_WithConflict()
{
    Map<int, int> m1;
    m1.add(1, 100);
    m1.add(2, 200);
    Map<int, int> m2;
    m2.add(2, 999);
    m2.add(3, 300);
    bool r = m1.addRange(m2);
    MAP_ET_ASSERT(!r, "addRange conflict returns false");
    MET_EQ(m1.size(), 3, "addRange conflict size==3");
    MET_EQ(m1.get(2, -1), 200, "addRange conflict original kept");
    MET_EQ(m1.get(3, -1), 300, "addRange new key added");
}

void MapTest::testMap_Merge_Empty()
{
    Map<int, int> m1;
    m1.add(1, 1);
    Map<int, int> empty;
    bool r = m1.addRange(empty);
    MAP_ET_ASSERT(r, "addRange empty returns true");
    MET_EQ(m1.size(), 1, "addRange empty size unchanged");
}

void MapTest::testMap_Merge_IntoEmpty()
{
    Map<int, int> m1;
    Map<int, int> m2;
    for (int i = 0; i < 10; ++i)
    {
        m2.add(i, i * 10);
    }
    bool r = m1.addRange(m2);
    MAP_ET_ASSERT(r, "addRange intoEmpty true");
    MET_EQ(m1.size(), 10, "addRange intoEmpty size==10");
}

void MapTest::testMap_Merge_LargeNoConflict()
{
    Map<int, int> a;
    Map<int, int> b;
    for (int i = 0; i < 100; ++i)
    {
        a.add(i, i);
    }
    for (int i = 100; i < 200; ++i)
    {
        b.add(i, i);
    }
    bool r = a.addRange(b);
    MAP_ET_ASSERT(r, "addRange large noConflict true");
    MET_EQ(a.size(), 200, "addRange large size==200");
}

void MapTest::testMap_Merge_AllConflict()
{
    Map<int, int> a;
    Map<int, int> b;
    for (int i = 0; i < 5; ++i)
    {
        a.add(i, i * 10);
        b.add(i, i * 100);
    }
    bool r = a.addRange(b);
    MAP_ET_ASSERT(!r, "addRange allConflict false");
    MET_EQ(a.size(), 5, "addRange allConflict size unchanged");
    for (int i = 0; i < 5; ++i)
    {
        MET_EQ(a.get(i, -1), i * 10, "addRange allConflict original kept");
    }
}

// -----------------------------------------------------------
// cloneTo — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_CloneTo_Basic()
{
    Map<int, int> src;
    src.add(1, 10);
    src.add(2, 20);
    src.add(3, 30);
    Map<int, int> dst;
    src.cloneTo(dst);
    MET_EQ(dst.size(), 3, "cloneTo size==3");
    MET_EQ(dst.get(1, -1), 10, "cloneTo key1==10");
    MET_EQ(dst.get(3, -1), 30, "cloneTo key3==30");
    src.add(4, 40);
    MAP_ET_ASSERT(!dst.contains(4), "cloneTo independent");
}

void MapTest::testMap_CloneTo_OverwriteExisting()
{
    Map<int, int> src;
    src.add(1, 10);
    src.add(2, 20);
    Map<int, int> dst;
    dst.add(100, 999);
    dst.add(200, 888);
    src.cloneTo(dst);
    MET_EQ(dst.size(), 2, "cloneTo overwrite size==2");
    MET_EQ(dst.get(1, -1), 10, "cloneTo overwrite key1==10");
    MAP_ET_ASSERT(!dst.contains(100), "cloneTo overwrite old keys gone");
}

void MapTest::testMap_CloneTo_EmptySource()
{
    Map<int, int> src;
    Map<int, int> dst;
    dst.add(1, 100);
    src.cloneTo(dst);
    MET_EQ(dst.size(), 0, "cloneTo emptySource dst cleared");
}

void MapTest::testMap_CloneTo_LargeMap()
{
    Map<int, int> src;
    for (int i = 0; i < 500; ++i)
    {
        src.add(i, i * 3);
    }
    Map<int, int> dst;
    src.cloneTo(dst);
    MET_EQ(dst.size(), 500, "cloneTo large size==500");
    for (int i = 0; i < 500; i += 50)
    {
        MET_EQ(dst.get(i, -1), i * 3, "cloneTo large value");
    }
}

// -----------------------------------------------------------
// dispose — 未在 ContainerTest 中测试
// -----------------------------------------------------------
void MapTest::testMap_Dispose()
{
    Map<int, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.add(i, i);
    }
    m.dispose();
    MAP_ET_ASSERT(m.isEmpty(), "dispose isEmpty");
    m.add(1, 1);
    MET_EQ(m.size(), 1, "dispose reuse size==1");
}

void MapTest::testMap_ClearDisposeMemory()
{
    Map<int, int> m;
    for (int i = 0; i < 50; ++i)
    {
        m.add(i, i);
    }
    m.clear(true);
    MAP_ET_ASSERT(m.isEmpty(), "clear(true) isEmpty");
    m.add(42, 420);
    MET_EQ(m.get(42, -1), 420, "clear(true) reuse ok");
}

// -----------------------------------------------------------
// rbegin / rend — 反向迭代器，未测试
// -----------------------------------------------------------
void MapTest::testMap_ReverseIterator()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.add(4, 40);
    m.add(5, 50);
    // map 是有序的，rbegin 指向最大 key
    auto rit = m.rbegin();
    MET_EQ(rit->first, 5, "rbegin key==5");
    MET_EQ(rit->second, 50, "rbegin value==50");
    ++rit;
    MET_EQ(rit->first, 4, "rbegin+1 key==4");
}

void MapTest::testMap_ReverseIterator_Sum()
{
    Map<int, int> m;
    for (int i = 1; i <= 10; ++i)
    {
        m.add(i, i);
    }
    int sum = 0;
    for (auto rit = m.rbegin(); rit != m.rend(); ++rit)
    {
        sum += rit->second;
    }
    MET_EQ(sum, 55, "rbegin/rend sum==55");
}

// -----------------------------------------------------------
// getAtIndex — ContainerTest 只测了基础，本文件测试更多场景
// -----------------------------------------------------------
void MapTest::testMap_GetAtIndex_Ordered()
{
    Map<int, int> m;
    m.add(10, 100);
    m.add(20, 200);
    m.add(30, 300);
    // map 按 key 有序
    MET_EQ(m.getAt(0, -1), 100, "getAtIndex(0)==100");
    MET_EQ(m.getAt(1, -1), 200, "getAtIndex(1)==200");
    MET_EQ(m.getAt(2, -1), 300, "getAtIndex(2)==300");
    MET_EQ(m.getAt(3, -1), -1, "getAtIndex(3)==-1 OOB");
    MET_EQ(m.getAt(-1, -1), -1, "getAtIndex(-1)==-1 OOB");
}

void MapTest::testMap_GetAtIndex_AfterErase()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.remove(2);
    MET_EQ(m.getAt(0, -1), 10, "getAtIndex after remove [0]==10");
    MET_EQ(m.getAt(1, -1), 30, "getAtIndex after remove [1]==30");
    MET_EQ(m.getAt(2, -1), -1, "getAtIndex after remove [2]==-1");
}

// -----------------------------------------------------------
// move constructor / move assignment — 未测试
// -----------------------------------------------------------
void MapTest::testMap_MoveCtor()
{
    Map<int, int> src;
    src.add(1, 10);
    src.add(2, 20);
    Map<int, int> dst(move(src));
    MET_EQ(dst.size(), 2, "moveCtor dst size==2");
    MET_EQ(dst.get(1, -1), 10, "moveCtor dst key1==10");
    MAP_ET_ASSERT(src.isEmpty(), "moveCtor src empty");
}

void MapTest::testMap_MoveAssign()
{
    Map<int, int> src;
    src.add(5, 50);
    src.add(6, 60);
    Map<int, int> dst;
    dst.add(99, 999);
    dst = move(src);
    MET_EQ(dst.size(), 2, "moveAssign dst size==2");
    MET_EQ(dst.get(5, -1), 50, "moveAssign dst key5==50");
    MAP_ET_ASSERT(src.isEmpty(), "moveAssign src empty");
}

// -----------------------------------------------------------
// initializer_list ctor — 未测试
// -----------------------------------------------------------
void MapTest::testMap_InitializerList()
{
    Map<int, int> m({ {1, 10}, {2, 20}, {3, 30} });
    MET_EQ(m.size(), 3, "initList size==3");
    MET_EQ(m.get(1, -1), 10, "initList key1==10");
    MET_EQ(m.get(2, -1), 20, "initList key2==20");
    MET_EQ(m.get(3, -1), 30, "initList key3==30");
}

void MapTest::testMap_InitializerList_StringKey()
{
    Map<string, int> m({ {"a", 1}, {"b", 2}, {"c", 3} });
    MET_EQ(m.size(), 3, "initListStr size==3");
    MET_EQ(m.get("b", -1), 2, "initListStr b==2");
}

// -----------------------------------------------------------
// cbegin / cend — 未测试
// -----------------------------------------------------------
void MapTest::testMap_ConstIterator()
{
    Map<int, int> m;
    for (int i = 1; i <= 5; ++i)
    {
        m.add(i, i * 10);
    }
    const Map<int, int>& cm = m;
    int keySum = 0;
    int valSum = 0;
    for (auto it = cm.cbegin(); it != cm.cend(); ++it)
    {
        keySum += it->first;
        valSum += it->second;
    }
    MET_EQ(keySum, 15, "cbegin/cend keySum==15");
    MET_EQ(valSum, 150, "cbegin/cend valSum==150");
}

void MapTest::testMap_ConstBeginEnd()
{
    Map<int, int> m;
    m.add(10, 1);
    m.add(20, 2);
    const Map<int, int>& cm = m;
    int count = 0;
    for (auto it = cm.begin(); it != cm.end(); ++it)
    {
        ++count;
    }
    MET_EQ(count, 2, "const begin/end count==2");
}

// -----------------------------------------------------------
// 综合场景测试
// -----------------------------------------------------------
void MapTest::testMap_LargeVolume()
{
    Map<int, int> m;
    const int N = 1000;
    for (int i = 0; i < N; ++i)
    {
        m.add(i, i * 2);
    }
    MET_EQ(m.size(), N, "largeVolume size==1000");
    for (int i = 0; i < N; i += 100)
    {
        MET_EQ(m.get(i, -1), i * 2, "largeVolume value correct");
    }
    for (int i = 0; i < N; i += 2)
    {
        m.remove(i);
    }
    MET_EQ(m.size(), N / 2, "largeVolume after remove size==500");
}

void MapTest::testMap_InsertUpdateErase()
{
    Map<int, int> m;
    m.add(1, 10);
    m.add(2, 20);
    m.add(3, 30);
    m.addOrSet(2, 200);
    MET_EQ(m.get(2, -1), 200, "insertUpdateErase updated 2");
    m.remove(1);
    MAP_ET_ASSERT(!m.contains(1), "insertUpdateErase erased 1");
    MET_EQ(m.size(), 2, "insertUpdateErase size==2");
}

void MapTest::testMap_MergeAndClone()
{
    Map<int, int> base;
    for (int i = 0; i < 10; ++i)
    {
        base.add(i, i * 100);
    }
    Map<int, int> ext;
    for (int i = 10; i < 20; ++i)
    {
        ext.add(i, i * 100);
    }
    base.addRange(ext);
    MET_EQ(base.size(), 20, "mergeAndClone size==20");
    Map<int, int> copy;
    base.cloneTo(copy);
    MET_EQ(copy.size(), 20, "mergeAndClone cloneTo size==20");
    MET_EQ(copy.get(15, -1), 1500, "mergeAndClone cloneTo val 15");
}

void MapTest::testMap_EraseAllThenReinsert()
{
    Map<int, int> m;
    for (int i = 0; i < 5; ++i)
    {
        m.add(i, i);
    }
    for (int i = 0; i < 5; ++i)
    {
        m.remove(i);
    }
    MAP_ET_ASSERT(m.isEmpty(), "eraseAll empty");
    for (int i = 0; i < 5; ++i)
    {
        m.add(i, i * 10);
    }
    MET_EQ(m.size(), 5, "eraseAllReinsert size==5");
    MET_EQ(m.get(4, -1), 40, "eraseAllReinsert val 4");
}

void MapTest::testMap_LLongKey()
{
    Map<llong, string> m;
    m.add(1000000000LL, "billion");
    m.add(9999999999LL, "ten_billion");
    MAP_ET_ASSERT(m.contains(1000000000LL), "llongKey contains");
    MAP_ET_ASSERT(m.get(9999999999LL) == "ten_billion", "llongKey value");
}

void MapTest::testMap_StringKeyLargeVolume()
{
    Map<string, int> m;
    for (int i = 0; i < 100; ++i)
    {
        m.add("key_" + IToS(i), i);
    }
    MET_EQ(m.size(), 100, "strKeyLargeVol size==100");
    for (int i = 0; i < 100; i += 10)
    {
        MET_EQ(m.get("key_" + IToS(i), -1), i, "strKeyLargeVol value");
    }
}

void MapTest::testMap_FindAfterInsertOrUpdate()
{
    Map<int, int> m;
    m.addOrSet(1, 10);
    m.addOrSet(2, 20);
    m.addOrSet(1, 100);
    auto it = m.find(1);
    MAP_ET_ASSERT(it != m.end(), "findAfterUpdate found");
    MET_EQ(it->second, 100, "findAfterUpdate value==100");
}

void MapTest::testMap_GetPtrModifyThenVerify()
{
    Map<int, int> m;
    m.add(1, 1);
    m.add(2, 2);
    m.add(3, 3);
    for (int i = 1; i <= 3; ++i)
    {
        int* p = m.getPtr(i);
        MAP_ET_ASSERT(p != nullptr, "getPtr loop non-null");
        *p *= 10;
    }
    MET_EQ(m.get(1, -1), 10, "getPtr modify 1");
    MET_EQ(m.get(2, -1), 20, "getPtr modify 2");
    MET_EQ(m.get(3, -1), 30, "getPtr modify 3");
}

void MapTest::testMap_ClearAndReinsertion()
{
    Map<int, int> m;
    for (int i = 0; i < 20; ++i)
    {
        m.add(i, i);
    }
    m.clear();
    MAP_ET_ASSERT(m.isEmpty(), "clear isEmpty");
    for (int i = 0; i < 5; ++i)
    {
        m.add(i, i * 100);
    }
    MET_EQ(m.size(), 5, "clearReinsertion size==5");
    MET_EQ(m.get(3, -1), 300, "clearReinsertion val 3");
}

void MapTest::testMap_IterationOrderedByKey()
{
    Map<int, int> m;
    m.add(50, 5);
    m.add(10, 1);
    m.add(30, 3);
    m.add(20, 2);
    m.add(40, 4);
    int prevKey = -1;
    bool isOrdered = true;
    for (const auto& kv : m)
    {
        if (kv.first < prevKey)
        {
            isOrdered = false;
        }
        prevKey = kv.first;
    }
    MAP_ET_ASSERT(isOrdered, "map iterate in order");
    MET_EQ(m.getAt(0, -1), 1, "map ordered [0]==1");
    MET_EQ(m.getAt(4, -1), 5, "map ordered [4]==5");
}

void MapTest::testMap_TryGetDefault()
{
    Map<int, int> m;
    m.add(1, 100);
    const int& v1 = m.get(1, -1);
    MET_EQ(v1, 100, "get found==100");
    const int& v2 = m.get(99, -1);
    MET_EQ(v2, -1, "get missing==-1");
}

void MapTest::testMap_TryGetNoDefault()
{
    Map<int, string> m;
    m.add(1, "one");
    const string& v1 = m.get(1);
    MAP_ET_ASSERT(v1 == "one", "tryGetNoDefault found");
    const string& v2 = m.get(99);
    MAP_ET_ASSERT(v2.empty(), "tryGetNoDefault missing empty");
}

void MapTest::testMap_OperatorBracket_AutoInsert()
{
    Map<int, int> m;
    m[5];
    MAP_ET_ASSERT(m.contains(5), "operator[] autoInsert contains 5");
    MET_EQ(m[5], 0, "operator[] autoInsert value==0");
}

void MapTest::testMap_OperatorBracket_Update()
{
    Map<string, int> m;
    m["x"] = 10;
    m["y"] = 20;
    MET_EQ(m["x"], 10, "op[] x==10");
    m["x"] = 99;
    MET_EQ(m["x"], 99, "op[] update x==99");
}

void MapTest::testMap_ContainsMany()
{
    Map<int, int> m;
    for (int i = 0; i < 200; ++i)
    {
        m.add(i, i);
    }
    for (int i = 0; i < 200; ++i)
    {
        MAP_ET_ASSERT(m.contains(i), "containsMany found");
    }
    for (int i = 200; i < 250; ++i)
    {
        MAP_ET_ASSERT(!m.contains(i), "containsMany not found");
    }
}

void MapTest::testMap_IsEmpty_AfterAllOps()
{
    Map<int, int> m;
    MAP_ET_ASSERT(m.isEmpty(), "init empty");
    m.add(1, 1);
    MAP_ET_ASSERT(!m.isEmpty(), "after add not empty");
    m.remove(1);
    MAP_ET_ASSERT(m.isEmpty(), "after remove empty again");
    m.addOrSet(2, 2);
    MAP_ET_ASSERT(!m.isEmpty(), "after addOrSet not empty");
    m.clear();
    MAP_ET_ASSERT(m.isEmpty(), "after clear empty");
}

void MapTest::testMap_SizeTracking()
{
    Map<int, int> m;
    MET_EQ(m.size(), 0, "size init==0");
    for (int i = 0; i < 30; ++i)
    {
        m.add(i, i);
        MET_EQ(m.size(), i + 1, "size track after add");
    }
    for (int i = 0; i < 10; ++i)
    {
        m.remove(i);
        MET_EQ(m.size(), 29 - i, "size track after remove");
    }
}

// ============================================================
// 主入口
// ============================================================
void MapTest::test()
{
    testMap_InsertOrUpdate_New();
    testMap_InsertOrUpdate_Existing();
    testMap_InsertOrUpdate_Move();
    testMap_InsertOrUpdate_Multiple();
    testMap_InsertOrUpdate_StringKey();
    testMap_InsertOrGet_New();
    testMap_InsertOrGet_Existing();
    testMap_InsertOrGet_Chain();
    testMap_InsertOrGet_LargeCount();
    testMap_TrySet_Existing();
    testMap_TrySet_NotExist();
    testMap_TrySet_Multiple();
    testMap_TrySet_StringKey();
    testMap_GetPtrConst_Found();
    testMap_GetPtrConst_Missing();
    testMap_GetPtrConst_ModifyViaGetPtr();
    testMap_FindIterator();
    testMap_FindIteratorConst();
    testMap_EraseByIterator();
    testMap_EraseByIterator_AllElements();
    testMap_EraseGetValue();
    testMap_EraseGetValue_Missing();
    testMap_EraseGetValue_StringValue();
    testMap_InsertKeyOnly();
    testMap_InsertKeyOnly_StringValue();
    testMap_InsertMoveValue();
    testMap_Merge_NoConflict();
    testMap_Merge_WithConflict();
    testMap_Merge_Empty();
    testMap_Merge_IntoEmpty();
    testMap_Merge_LargeNoConflict();
    testMap_Merge_AllConflict();
    testMap_CloneTo_Basic();
    testMap_CloneTo_OverwriteExisting();
    testMap_CloneTo_EmptySource();
    testMap_CloneTo_LargeMap();
    testMap_Dispose();
    testMap_ClearDisposeMemory();
    testMap_ReverseIterator();
    testMap_ReverseIterator_Sum();
    testMap_GetAtIndex_Ordered();
    testMap_GetAtIndex_AfterErase();
    testMap_MoveCtor();
    testMap_MoveAssign();
    testMap_InitializerList();
    testMap_InitializerList_StringKey();
    testMap_ConstIterator();
    testMap_ConstBeginEnd();
    testMap_LargeVolume();
    testMap_InsertUpdateErase();
    testMap_MergeAndClone();
    testMap_EraseAllThenReinsert();
    testMap_LLongKey();
    testMap_StringKeyLargeVolume();
    testMap_FindAfterInsertOrUpdate();
    testMap_GetPtrModifyThenVerify();
    testMap_ClearAndReinsertion();
    testMap_IterationOrderedByKey();
    testMap_TryGetDefault();
    testMap_TryGetNoDefault();
    testMap_OperatorBracket_AutoInsert();
    testMap_OperatorBracket_Update();
    testMap_ContainsMany();
    testMap_IsEmpty_AfterAllOps();
    testMap_SizeTracking();
	testSTLBasic();
	testSTLOrdering();
}

// ============================================================
// Consolidated from legacy STL aggregate test::testMap
// ============================================================
#ifndef MAPTEST_STL_ASSERT
#define MAPTEST_STL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MapTest FAIL: ") + (msg)); } } while(0)
#endif
#ifndef MAPTEST_STL_ASSERT_EQ
#define MAPTEST_STL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MapTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#endif
#ifndef MAPTEST_STL_ASSERT_STR
#define MAPTEST_STL_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("MapTest FAIL: ") + (msg)); } } while(0)
#endif

void MapTest::testSTLBasic()
{
	// default: empty
	Map<int, string> m;
	MAPTEST_STL_ASSERT(m.isEmpty(), "Map default isEmpty");
	MAPTEST_STL_ASSERT_EQ(m.size(), 0, "Map default size=0");

	// insert via [] operator
	m[1] = "one";
	m[2] = "two";
	m[3] = "three";
	MAPTEST_STL_ASSERT_EQ(m.size(), 3, "Map size=3");
	MAPTEST_STL_ASSERT(!m.isEmpty(), "Map not isEmpty");

	// contains
	MAPTEST_STL_ASSERT(m.contains(1), "Map contains 1");
	MAPTEST_STL_ASSERT(m.contains(2), "Map contains 2");
	MAPTEST_STL_ASSERT(!m.contains(99), "Map not contains 99");

	// get via get
	MAPTEST_STL_ASSERT(m.get(1) == "one", "Map get(1)=one");
	MAPTEST_STL_ASSERT(m.get(2) == "two", "Map get(2)=two");

	// getPtr
	string* ptr = m.getPtr(2);
	MAPTEST_STL_ASSERT(ptr != nullptr, "Map getPtr(2) not null");
	MAPTEST_STL_ASSERT(*ptr == "two", "Map *getPtr(2)=two");
	string* noptr = m.getPtr(99);
	MAPTEST_STL_ASSERT(noptr == nullptr, "Map getPtr(99) null");

	// get with out via getPtr
	string* vptr = m.getPtr(3);
	MAPTEST_STL_ASSERT(vptr != nullptr, "Map getPtr(3) not null");
	MAPTEST_STL_ASSERT(*vptr == "three", "Map *getPtr(3)=three");
	MAPTEST_STL_ASSERT(m.getPtr(99) == nullptr, "Map getPtr(99) null");

	// getAtIndex
	const string& first = m.getAt(0, "");
	MAPTEST_STL_ASSERT(!first.empty(), "Map getAtIndex(0) not empty");

	// erase
	MAPTEST_STL_ASSERT(m.remove(2), "Map erase(2) ok");
	MAPTEST_STL_ASSERT_EQ(m.size(), 2, "Map size=2 after erase");
	MAPTEST_STL_ASSERT(!m.contains(2), "Map 2 gone after erase");
	MAPTEST_STL_ASSERT(!m.remove(99), "Map erase(99) false");

	// iteration
	Map<int, int> numMap;
	numMap[10] = 100;
	numMap[20] = 200;
	numMap[30] = 300;
	int sum = 0;
	for (const auto& kv : numMap)
	{
		sum += kv.second;
	}
	MAPTEST_STL_ASSERT_EQ(sum, 600, "Map iterate sum=600");

	// clear
	m.clear();
	MAPTEST_STL_ASSERT(m.isEmpty(), "Map clear -> isEmpty");
	MAPTEST_STL_ASSERT_EQ(m.size(), 0, "Map size=0 after clear");

	// addOrSet: key exists -> update
	Map<int, int> iouMap;
	iouMap[5] = 50;
	iouMap.addOrSet(5, 55);
	MAPTEST_STL_ASSERT_EQ(iouMap.get(5), 55, "Map addOrSet update");

	// addOrSet: key missing -> insert
	iouMap.addOrSet(6, 60);
	MAPTEST_STL_ASSERT_EQ(iouMap.get(6), 60, "Map addOrSet insert");
	MAPTEST_STL_ASSERT_EQ(iouMap.size(), 2, "Map size=2 after addOrSet");

	// initializer_list constructor
	Map<int, int> initMap({ {1, 11}, {2, 22}, {3, 33} });
	MAPTEST_STL_ASSERT_EQ(initMap.size(), 3, "Map init list size=3");
	MAPTEST_STL_ASSERT_EQ(initMap.get(2), 22, "Map init list get(2)=22");
}

// ============================================================
// Consolidated from legacy STL aggregate test::testMapOrdering
// ============================================================
#ifndef MAPTEST_STL_ASSERT
#define MAPTEST_STL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MapTest FAIL: ") + (msg)); } } while(0)
#endif
#ifndef MAPTEST_STL_ASSERT_EQ
#define MAPTEST_STL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MapTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#endif
#ifndef MAPTEST_STL_ASSERT_STR
#define MAPTEST_STL_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("MapTest FAIL: ") + (msg)); } } while(0)
#endif

void MapTest::testSTLOrdering()
{
	Map<int, int> m;
	m[30] = 3;
	m[10] = 1;
	m[20] = 2;

	// std::map is ordered by key ascending
	int prevKey = -1;
	for (const auto& kv : m)
	{
		MAPTEST_STL_ASSERT(kv.first > prevKey, "Map keys ascending");
		prevKey = kv.first;
	}

	// reverse iterator: descending
	int prevKeyRev = 9999;
	for (auto it = m.rbegin(); it != m.rend(); ++it)
	{
		MAPTEST_STL_ASSERT(it->first < prevKeyRev, "Map rbegin descending");
		prevKeyRev = it->first;
	}

	// trySet: update if exists
	MAPTEST_STL_ASSERT(m.set(10, 99), "Map trySet existing key");
	MAPTEST_STL_ASSERT_EQ(m.get(10), 99, "Map trySet updated value");
	MAPTEST_STL_ASSERT(!m.set(999, 0), "Map trySet missing key false");

	// mapKeyToVector style via iteration
	Vector<int> keys;
	for (const auto& kv : m)
	{
		keys.add(kv.first);
	}
	MAPTEST_STL_ASSERT_EQ(keys.size(), 3, "Map keys vector size=3");
	MAPTEST_STL_ASSERT_EQ(keys[0], 10, "Map key[0]=10");
	MAPTEST_STL_ASSERT_EQ(keys[1], 20, "Map key[1]=20");
	MAPTEST_STL_ASSERT_EQ(keys[2], 30, "Map key[2]=30");
}
#endif
