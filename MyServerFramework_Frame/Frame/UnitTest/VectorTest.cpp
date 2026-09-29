#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// VectorTest — Vector<T> 扩展全方位测试
// 覆盖：add(多参数)/addUnique/addNotEqual/addUniqueOrErase/
//        addRange/addRangeCopy/addRangePtr/setRange/setRangeCopy/
//        erase/removeAt/remove/eraseAllElement/eraseLast/
//        popBack/contains/isAll/findFirstIndex/findElementCount/
//        swapIndex/random/randomRemove/insert/cloneTo/sort/
//        resize/reserve/get/operator[]/operator==/operator!=
// ============================================================

#define VET_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("VectorTest FAILED: ") + (msg)); }

// -----------------------------------------------------------
// add 系列
// -----------------------------------------------------------
void VectorTest::testVector_PushBackSingle()
{
    Vector<int> v;
    v.add(10);
    VET_ASSERT(v.size() == 1, "pushBackSingle size==1");
    VET_ASSERT(v[0] == 10, "pushBackSingle [0]==10");
}

void VectorTest::testVector_PushBack2()
{
    Vector<int> v;
    v.add(1, 2);
    VET_ASSERT(v.size() == 2, "pushBack2 size==2");
    VET_ASSERT(v[0] == 1 && v[1] == 2, "pushBack2 values");
}

void VectorTest::testVector_PushBack3()
{
    Vector<int> v;
    v.add(10, 20, 30);
    VET_ASSERT(v.size() == 3, "pushBack3 size==3");
    VET_ASSERT(v[0] == 10 && v[1] == 20 && v[2] == 30, "pushBack3 values");
}

void VectorTest::testVector_PushBack4()
{
    Vector<int> v;
    v.add(1, 2, 3, 4);
    VET_ASSERT(v.size() == 4, "pushBack4 size==4");
    VET_ASSERT(v[3] == 4, "pushBack4 [3]==4");
}

void VectorTest::testVector_PushBack5()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    VET_ASSERT(v.size() == 5, "pushBack5 size==5");
    VET_ASSERT(v[4] == 5, "pushBack5 [4]==5");
}

void VectorTest::testVector_PushBack6()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5, 6);
    VET_ASSERT(v.size() == 6, "pushBack6 size==6");
    VET_ASSERT(v[5] == 6, "pushBack6 [5]==6");
}

void VectorTest::testVector_PushBackMove()
{
    Vector<string> v;
    string s = "hello";
    v.add(move(s));
    VET_ASSERT(v.size() == 1, "pushBackMove size==1");
    VET_ASSERT(v[0] == "hello", "pushBackMove [0]==hello");
}

// -----------------------------------------------------------
// addUnique / addNotEqual / addUniqueOrErase
// -----------------------------------------------------------
void VectorTest::testVector_AddUnique_VET()
{
    Vector<int> v;
    bool r1 = v.addUnique(10);
    bool r2 = v.addUnique(20);
    bool r3 = v.addUnique(10); // 重复
    VET_ASSERT(r1 && r2, "addUnique new elements return true");
    VET_ASSERT(!r3, "addUnique duplicate returns false");
    VET_ASSERT(v.size() == 2, "addUnique size==2");
}

void VectorTest::testVector_AddNotEqual()
{
    Vector<int> v;
    v.addNotEqual(10, 0); // 不等于 0，添加
    v.addNotEqual(0, 0);  // 等于 0，不添加
    v.addNotEqual(5, 0);  // 不等于 0，添加
    VET_ASSERT(v.size() == 2, "addNotEqual size==2");
    VET_ASSERT(v[0] == 10 && v[1] == 5, "addNotEqual values");
}

void VectorTest::testVector_AddUniqueOrErase_Add()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.addUniqueOrRemove(3, true); // add
    VET_ASSERT(v.contains(3), "addUniqueOrErase add contains 3");
    VET_ASSERT(v.size() == 3, "addUniqueOrErase add size==3");
}

void VectorTest::testVector_AddUniqueOrErase_Erase()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    v.add(3);
    v.addUniqueOrRemove(2, false); // erase
    VET_ASSERT(!v.contains(2), "addUniqueOrErase erase not contains 2");
    VET_ASSERT(v.size() == 2, "addUniqueOrErase erase size==2");
}

// -----------------------------------------------------------
// addRange / setRange (基础类型)
// -----------------------------------------------------------
void VectorTest::testVector_AddRange()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    Vector<int> src;
    src.add(3);
    src.add(4);
    src.add(5);
    v.addRange(src);
    VET_ASSERT(v.size() == 5, "addRange size==5");
    VET_ASSERT(v[4] == 5, "addRange [4]==5");
}

void VectorTest::testVector_SetRange()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    Vector<int> src;
    src.add(10);
    src.add(20);
    v.setRange(src);
    VET_ASSERT(v.size() == 2, "setRange size==2");
    VET_ASSERT(v[0] == 10 && v[1] == 20, "setRange values");
}

void VectorTest::testVector_AddRangeRawPtr()
{
    Vector<int> v;
    int arr[] = { 10, 20, 30, 40, 50 };
    v.addRange(arr, 5);
    VET_ASSERT(v.size() == 5, "addRangeRawPtr size==5");
    VET_ASSERT(v[0] == 10 && v[4] == 50, "addRangeRawPtr values");
}

void VectorTest::testVector_SetRangeRawPtr()
{
    Vector<int> v;
    v.add(999);
    int arr[] = { 1, 2, 3 };
    v.setRange(arr, 3);
    VET_ASSERT(v.size() == 3, "setRangeRawPtr size==3");
    VET_ASSERT(v[0] == 1 && v[2] == 3, "setRangeRawPtr values");
}

void VectorTest::testVector_AddRangeCopy()
{
    Vector<string> v;
    v.add("a");
    // addRangeCopy with Vector<string>
    Vector<string> src;
    src.add("b");
    src.add("c");
    src.add("d");
    v.addRangeCopy(src);
    VET_ASSERT(v.size() == 4, "addRangeCopy size==4");
    VET_ASSERT(v[1] == "b" && v[3] == "d", "addRangeCopy values");
}

void VectorTest::testVector_AddRangeMove()
{
    Vector<int> v;
    v.add(1);
    v.add(2);
    Vector<int> src;
    src.add(3);
    src.add(4);
    v.addRange(move(src));
    VET_ASSERT(v.size() == 4, "addRangeMove size==4");
    VET_ASSERT(src.isEmpty(), "addRangeMove src cleared");
}

void VectorTest::testVector_AddRangeFromArrayList()
{
    Vector<int> v;
    ArrayList<10, int> al;
    al.add(1);
    al.add(2);
    al.add(3);
    v.addRange(al);
    VET_ASSERT(v.size() == 3, "addRangeArrayList size==3");
    VET_ASSERT(v[0] == 1 && v[2] == 3, "addRangeArrayList values");
}

// -----------------------------------------------------------
// erase 系列
// -----------------------------------------------------------
void VectorTest::testVector_EraseAt()
{
    Vector<int> v;
    v.add(10, 20, 30, 40, 50);
    v.removeAt(2); // 删除 30
    VET_ASSERT(v.size() == 4, "removeAt size==4");
    VET_ASSERT(v[2] == 40, "removeAt [2]==40");
}

void VectorTest::testVector_EraseAtOutOfRange()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.removeAt(-1); // 不应崩溃
    v.removeAt(99); // 不应崩溃
    VET_ASSERT(v.size() == 3, "eraseAtOutOfRange size unchanged");
}

void VectorTest::testVector_EraseAtCount()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.add(i);
    }
    v.removeRange(2, 3); // 删除 index 2,3,4 三个元素
    VET_ASSERT(v.size() == 7, "eraseAtCount size==7");
    VET_ASSERT(v[2] == 5, "eraseAtCount [2]==5");
}

void VectorTest::testVector_EraseElement()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    bool r1 = v.remove(3);
    bool r2 = v.remove(99);
    VET_ASSERT(r1, "remove found returns true");
    VET_ASSERT(!r2, "remove notFound returns false");
    VET_ASSERT(v.size() == 4, "remove size==4");
    VET_ASSERT(!v.contains(3), "remove not contains 3");
}

void VectorTest::testVector_EraseAllElement_VET()
{
    Vector<int> v;
    v.add(1, 2, 3, 2, 4, 2);
    int count = v.removeAll(2);
    VET_ASSERT(count == 3, "eraseAll count==3");
    VET_ASSERT(v.size() == 3, "eraseAll size==3");
    VET_ASSERT(!v.contains(2), "eraseAll not contains 2");
}

void VectorTest::testVector_EraseLast()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.removeLast();
    VET_ASSERT(v.size() == 2, "eraseLast size==2");
    VET_ASSERT(v[1] == 2, "eraseLast [1]==2");
}

void VectorTest::testVector_EraseLastEmpty()
{
    Vector<int> v;
    v.removeLast(); // 不应崩溃
    VET_ASSERT(v.isEmpty(), "eraseLastEmpty still empty");
}

void VectorTest::testVector_PopBack()
{
    Vector<int> v;
    v.add(10, 20, 30);
    int val = v.popBack(-1);
    VET_ASSERT(val == 30, "popBack val==30");
    VET_ASSERT(v.size() == 2, "popBack size==2");
}

void VectorTest::testVector_PopBackEmpty()
{
    Vector<int> v;
    int val = v.popBack(-99);
    VET_ASSERT(val == -99, "popBackEmpty returns default");
    VET_ASSERT(v.isEmpty(), "popBackEmpty still empty");
}

// -----------------------------------------------------------
// contains / isAll / findFirstIndex / findElementCount
// -----------------------------------------------------------
void VectorTest::testVector_Contains()
{
    Vector<int> v;
    v.add(5, 10, 15);
    VET_ASSERT(v.contains(10), "contains 10 true");
    VET_ASSERT(!v.contains(99), "contains 99 false");
    VET_ASSERT(!Vector<int>().contains(1), "empty contains false");
}

void VectorTest::testVector_IsAll()
{
    Vector<int> v;
    v.add(5, 5, 5);
    VET_ASSERT(v.isAll(5), "isAll(5) true");
    VET_ASSERT(!v.isAll(4), "isAll(4) false");

    Vector<int> empty;
    VET_ASSERT(empty.isAll(0), "isAll empty true");
}

void VectorTest::testVector_FindFirstIndex()
{
    Vector<int> v;
    v.add(10, 20, 30, 20, 10);
    VET_ASSERT(v.findIndex(20) == 1, "findFirstIndex 20==1");
    VET_ASSERT(v.findIndex(20, 2) == 3, "findFirstIndex 20 from2==3");
    VET_ASSERT(v.findIndex(99) == -1, "findFirstIndex 99==-1");
}

void VectorTest::testVector_FindElementCount()
{
    Vector<int> v;
    v.add(1, 2, 3, 2, 2, 4);
    VET_ASSERT(v.findCount(2) == 3, "findElementCount(2)==3");
    VET_ASSERT(v.findCount(1) == 1, "findElementCount(1)==1");
    VET_ASSERT(v.findCount(99) == 0, "findElementCount(99)==0");
}

// -----------------------------------------------------------
// swapIndex / random / randomRemove
// -----------------------------------------------------------
void VectorTest::testVector_SwapIndex()
{
    Vector<int> v;
    v.add(10, 20, 30, 40, 50);
    v.swapIndex(0, 4);
    VET_ASSERT(v[0] == 50, "swapIndex [0]==50");
    VET_ASSERT(v[4] == 10, "swapIndex [4]==10");
}

void VectorTest::testVector_Random()
{
    Vector<int> v;
    for (int i = 0; i < 100; ++i) { v.add(i); }
    // random 应返回在范围内的值
    for (int t = 0; t < 10; ++t)
    {
        const int& r = v.random();
        VET_ASSERT(r >= 0 && r < 100, "random in range");
    }
}

void VectorTest::testVector_RandomEmpty()
{
    Vector<int> v;
    const int& r = v.random();
    VET_ASSERT(r == 0, "randomEmpty returns default");
}

void VectorTest::testVector_RandomErase()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i) { v.add(i); }
    int before = v.size();
    v.randomRemove();
    VET_ASSERT(v.size() == before - 1, "randomRemove size decreased");
}

// -----------------------------------------------------------
// insert
// -----------------------------------------------------------
void VectorTest::testVector_InsertAt()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.insert(1, 99);
    VET_ASSERT(v.size() == 4, "insertAt size==4");
    VET_ASSERT(v[1] == 99, "insertAt [1]==99");
    VET_ASSERT(v[2] == 2, "insertAt [2]==2");
}

void VectorTest::testVector_InsertAtBegin()
{
    Vector<int> v;
    v.add(10, 20, 30);
    v.insert(0, 5);
    VET_ASSERT(v[0] == 5, "insertAtBegin [0]==5");
    VET_ASSERT(v.size() == 4, "insertAtBegin size==4");
}

void VectorTest::testVector_InsertAtEnd()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.insert(3, 999);
    VET_ASSERT(v[3] == 999, "insertAtEnd [3]==999");
    VET_ASSERT(v.size() == 4, "insertAtEnd size==4");
}

// -----------------------------------------------------------
// cloneTo
// -----------------------------------------------------------
void VectorTest::testVector_CloneTo()
{
    Vector<int> src;
    src.add(10, 20, 30);
    Vector<int> dst;
    src.cloneTo(dst);
    VET_ASSERT(dst.size() == 3, "cloneTo size==3");
    VET_ASSERT(dst[0] == 10 && dst[2] == 30, "cloneTo values");
    // 修改 src 不影响 dst
    src[0] = 999;
    VET_ASSERT(dst[0] == 10, "cloneTo independent");
}

// -----------------------------------------------------------
// sort
// -----------------------------------------------------------
void VectorTest::testVector_Sort()
{
    Vector<int> v;
    v.add(5, 3, 1, 4, 2);
    v.sort();
    VET_ASSERT(v[0] == 1 && v[1] == 2 && v[4] == 5, "sort ascending");
}

void VectorTest::testVector_SortSingleElement()
{
    Vector<int> v;
    v.add(42);
    v.sort();
    VET_ASSERT(v[0] == 42, "sortSingle unchanged");
}

void VectorTest::testVector_SortAlreadySorted()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    v.sort();
    for (int i = 0; i < 5; ++i)
    {
        VET_ASSERT(v[i] == i + 1, "sortAlreadySorted correct");
    }
}

// -----------------------------------------------------------
// resize / reserve / get
// -----------------------------------------------------------
void VectorTest::testVector_Resize()
{
    Vector<int> v;
    v.resize(5);
    VET_ASSERT(v.size() == 5, "resize size==5");
    VET_ASSERT(v[0] == 0, "resize default zero");
}

void VectorTest::testVector_ResizeDown()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i) { v.add(i); }
    v.resize(3);
    VET_ASSERT(v.size() == 3, "resizeDown size==3");
}

void VectorTest::testVector_Reserve()
{
    Vector<int> v;
    v.reserve(100);
    // 只预分配，不改变 size
    VET_ASSERT(v.size() == 0, "reserve size still 0");
    VET_ASSERT(v.isEmpty(), "reserve isEmpty true");
}

void VectorTest::testVector_GetDefault()
{
    Vector<int> v;
    v.add(10, 20, 30);
    VET_ASSERT(v.get(1) == 20, "get[1]==20");
    VET_ASSERT(v.get(-1) == 0, "get[-1] default 0");
    VET_ASSERT(v.get(99) == 0, "get[99] default 0");
}

void VectorTest::testVector_GetWithCustomDefault()
{
    Vector<int> v;
    v.add(1, 2, 3);
    VET_ASSERT(v.get(10, -99) == -99, "getCustomDefault -99");
    VET_ASSERT(v.get(1, -99) == 2, "getCustomDefault in range 2");
}

// -----------------------------------------------------------
// operator== / operator!=
// -----------------------------------------------------------
void VectorTest::testVector_EqualityOp()
{
    Vector<int> v1;
    v1.add(1, 2, 3);
    Vector<int> v2;
    v2.add(1, 2, 3);
    VET_ASSERT(v1 == v2, "equality true");
    VET_ASSERT(!(v1 != v2), "inequality false");
}

void VectorTest::testVector_InequalityOp()
{
    Vector<int> v1;
    v1.add(1, 2, 3);
    Vector<int> v2;
    v2.add(1, 2, 4);
    VET_ASSERT(v1 != v2, "inequality true");
    VET_ASSERT(!(v1 == v2), "equality false");
}

void VectorTest::testVector_EqualityDifferentSize()
{
    Vector<int> v1;
    v1.add(1, 2);
    Vector<int> v2;
    v2.add(1, 2, 3);
    VET_ASSERT(v1 != v2, "diffSize inequality true");
}

// -----------------------------------------------------------
// clear / dispose / isEmpty
// -----------------------------------------------------------
void VectorTest::testVector_Clear()
{
    Vector<int> v;
    v.add(1, 2, 3);
    v.clear();
    VET_ASSERT(v.isEmpty(), "clear isEmpty");
    VET_ASSERT(v.size() == 0, "clear size==0");
}

void VectorTest::testVector_ClearEmpty()
{
    Vector<int> v;
    v.clear(); // 不应崩溃
    VET_ASSERT(v.isEmpty(), "clearEmpty still empty");
}

void VectorTest::testVector_IsEmpty()
{
    Vector<int> v;
    VET_ASSERT(v.isEmpty(), "isEmpty true on fresh");
    v.add(1);
    VET_ASSERT(!v.isEmpty(), "isEmpty false after push");
}

// -----------------------------------------------------------
// 综合场景测试
// -----------------------------------------------------------
void VectorTest::testVector_LargeVolumePushBack()
{
    Vector<int> v;
    const int N = 10000;
    FOR(N)
    {
        v.add(i);
    }
    VET_ASSERT(v.size() == N, "largeVolume size==10000");
    VET_ASSERT(v[0] == 0 && v[N - 1] == N - 1, "largeVolume boundary values");
}

void VectorTest::testVector_EraseAllThenAdd()
{
    Vector<int> v;
    v.add(5, 5, 5, 5, 5);
    int c = v.removeAll(5);
    VET_ASSERT(c == 5, "eraseAllThenAdd erased 5");
    VET_ASSERT(v.isEmpty(), "eraseAllThenAdd empty");
    v.add(1);
    VET_ASSERT(v.size() == 1 && v[0] == 1, "eraseAllThenAdd re-add");
}

void VectorTest::testVector_MixedOperations()
{
    Vector<int> v;
    FOR(20)
    {
        v.add(i);
    }
    // erase 一半
    for (int i = 19; i >= 0; i -= 2)
    {
        v.removeAt(i);
    }
    VET_ASSERT(v.size() == 10, "mixedOps size==10");
    // 确保只剩偶数
    FOR_VECTOR(v)
    {
        VET_ASSERT(v[i] % 2 == 0, "mixedOps only evens");
    }
}

void VectorTest::testVector_AddRangeThenSort()
{
    Vector<int> v1;
    Vector<int> v2;
    v2.add(5, 3, 1, 4, 2);
    v1.addRange(v2);
    v1.sort();
    VET_ASSERT(v1.size() == 5, "addRangeSort size==5");
    VET_ASSERT(v1[0] == 1 && v1[4] == 5, "addRangeSort sorted");
}

void VectorTest::testVector_SwapIndexMultiple()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.add(i);
    }
    // 手动反转
    for (int i = 0; i < 5; ++i)
    {
        v.swapIndex(i, 9 - i);
    }
    VET_ASSERT(v[0] == 9 && v[9] == 0, "swapMultiple reversed ends");
    VET_ASSERT(v[4] == 5 && v[5] == 4, "swapMultiple middle");
}

void VectorTest::testVector_FindFirstIndexMultipleOccurrences()
{
    Vector<int> v;
    v.add(0, 1, 2, 0, 1, 2);
    v.add(0);
    VET_ASSERT(v.findIndex(0) == 0, "findIndex 0 start==0");
    VET_ASSERT(v.findIndex(0, 1) == 3, "findIndex 0 from1==3");
    VET_ASSERT(v.findIndex(0, 4) == 6, "findIndex 0 from4==6");
    VET_ASSERT(v.findIndex(0, 7) == -1, "findIndex 0 from7==-1");
}

void VectorTest::testVector_EraseElementNotExist()
{
    Vector<int> v;
    v.add(1, 2, 3);
    bool r = v.remove(99);
    VET_ASSERT(!r, "eraseNotExist false");
    VET_ASSERT(v.size() == 3, "eraseNotExist size unchanged");
}

void VectorTest::testVector_PopBackUntilEmpty()
{
    Vector<int> v;
    v.add(10, 20, 30);
    VET_ASSERT(v.popBack(0) == 30, "popBack 30");
    VET_ASSERT(v.popBack(0) == 20, "popBack 20");
    VET_ASSERT(v.popBack(0) == 10, "popBack 10");
    VET_ASSERT(v.popBack(-1) == -1, "popBack empty default");
    VET_ASSERT(v.isEmpty(), "popBack until empty");
}

void VectorTest::testVector_CloneToOverwrite()
{
    Vector<int> src;
    src.add(1, 2, 3);
    Vector<int> dst;
    dst.add(99, 88);
    src.cloneTo(dst);
    VET_ASSERT(dst.size() == 3, "cloneToOverwrite size==3");
    VET_ASSERT(dst[0] == 1, "cloneToOverwrite [0]==1");
}

void VectorTest::testVector_StringVector()
{
    Vector<string> v;
    v.add("alpha");
    v.add("beta");
    v.add("gamma");
    VET_ASSERT(v.size() == 3, "stringVec size==3");
    VET_ASSERT(v.contains("beta"), "stringVec contains beta");
    VET_ASSERT(!v.contains("delta"), "stringVec not contains delta");

    v.remove("beta");
    VET_ASSERT(v.size() == 2, "stringVec erase size==2");
    VET_ASSERT(!v.contains("beta"), "stringVec erased beta");
}

void VectorTest::testVector_IntegrationSortAndSearch()
{
    Vector<int> v;
    for (int i = 100; i >= 1; --i)
    {
        v.add(i);
    }
    v.sort();
    VET_ASSERT(v[0] == 1 && v[99] == 100, "integrationSort boundary");
    VET_ASSERT(v.findIndex(50) == 49, "integrationSort findIndex 50");
    VET_ASSERT(v.findCount(50) == 1, "integrationSort count 50==1");
}

void VectorTest::testVector_AddUniqueMultiple()
{
    Vector<int> v;
    for (int i = 0; i < 100; ++i)
    {
        v.addUnique(i % 10);
    }
    VET_ASSERT(v.size() == 10, "addUniqueMultiple size==10");
    for (int i = 0; i < 10; ++i)
    {
        VET_ASSERT(v.contains(i), "addUniqueMultiple contains " + IToS(i));
    }
}

void VectorTest::testVector_InsertInMiddle()
{
    Vector<int> v;
    for (int i = 0; i < 5; ++i)
    {
        v.add(i * 10);
    }
    v.insert(2, 999);
    VET_ASSERT(v.size() == 6, "insertMiddle size==6");
    VET_ASSERT(v[2] == 999, "insertMiddle [2]==999");
    VET_ASSERT(v[3] == 20, "insertMiddle [3]==20 (shifted)");
}

void VectorTest::testVector_EraseAtMultipleSegments()
{
    Vector<int> v;
    for (int i = 0; i < 10; ++i)
    {
        v.add(i);
    }
    v.removeRange(0, 3); // 删除前 3 个
    VET_ASSERT(v.size() == 7, "eraseAtMulti size==7");
    VET_ASSERT(v[0] == 3, "eraseAtMulti [0]==3");
}

void VectorTest::testVector_EmplaceBack()
{
    Vector<int> v;
    v.emplace(42);
    v.emplace(100);
    VET_ASSERT(v.size() == 2, "emplaceBack size==2");
    VET_ASSERT(v[0] == 42 && v[1] == 100, "emplaceBack values");
}

void VectorTest::testVector_ClearAndReserve()
{
    Vector<int> v;
    for (int i = 0; i < 50; ++i)
    {
        v.add(i);
    }
    v.clear();
    v.reserve(200);
    VET_ASSERT(v.isEmpty(), "clearAndReserve empty");
    v.add(1);
    VET_ASSERT(v.size() == 1, "clearAndReserve after push size==1");
}

void VectorTest::testVector_SetRangeCopy_String()
{
    Vector<string> v;
    v.add("old1"); v.add("old2");
    Vector<string> src;
    src.add("new1"); src.add("new2"); src.add("new3");
    v.setRangeCopy(src);
    VET_ASSERT(v.size() == 3, "setRangeCopyStr size==3");
    VET_ASSERT(v[0] == "new1" && v[2] == "new3", "setRangeCopyStr values");
}

void VectorTest::testVector_ResetElement()
{
    Vector<int> v;
    v.add(10, 20, 30, 20, 40);
    bool r = v.resetElement(20);
    VET_ASSERT(r, "resetElement found true");
    VET_ASSERT(v[1] == 0, "resetElement [1]==0 (default)");
    VET_ASSERT(v[3] == 20, "resetElement only first occurrence");
}

void VectorTest::testVector_ResetElementAt()
{
    Vector<int> v;
    v.add(1, 2, 3, 4);
    bool r = v.resetElementAt(2);
    VET_ASSERT(r, "resetElementAt found true");
    VET_ASSERT(v[2] == 0, "resetElementAt [2]==0");
    VET_ASSERT(!v.resetElementAt(-1), "resetElementAt -1 false");
    VET_ASSERT(!v.resetElementAt(99), "resetElementAt 99 false");
}

void VectorTest::testVector_DataPointer()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    const int* p = v.data();
    VET_ASSERT(p != nullptr, "data() non-null");
    VET_ASSERT(p[0] == 1 && p[4] == 5, "data() values");
}

void VectorTest::testVector_IteratorBasic()
{
    Vector<int> v;
    v.add(10, 20, 30, 40, 50);
    int sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it)
    {
        sum += *it;
    }
    VET_ASSERT(sum == 150, "iterator sum==150");
}

void VectorTest::testVector_ReverseIterator()
{
    Vector<int> v;
    v.add(1, 2, 3, 4, 5);
    int last = -1;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
    {
        last = *it;
        break;
    }
    VET_ASSERT(last == 5, "reverseIterator first==5");
}

// ============================================================
// 主入口
// ============================================================
void VectorTest::test()
{
    testVector_PushBackSingle();
    testVector_PushBack2();
    testVector_PushBack3();
    testVector_PushBack4();
    testVector_PushBack5();
    testVector_PushBack6();
    testVector_PushBackMove();
    testVector_AddUnique_VET();
    testVector_AddNotEqual();
    testVector_AddUniqueOrErase_Add();
    testVector_AddUniqueOrErase_Erase();
    testVector_AddRange();
    testVector_SetRange();
    testVector_AddRangeRawPtr();
    testVector_SetRangeRawPtr();
    testVector_AddRangeCopy();
    testVector_AddRangeMove();
    testVector_AddRangeFromArrayList();
    testVector_EraseAt();
    testVector_EraseAtOutOfRange();
    testVector_EraseAtCount();
    testVector_EraseElement();
    testVector_EraseAllElement_VET();
    testVector_EraseLast();
    testVector_EraseLastEmpty();
    testVector_PopBack();
    testVector_PopBackEmpty();
    testVector_Contains();
    testVector_IsAll();
    testVector_FindFirstIndex();
    testVector_FindElementCount();
    testVector_SwapIndex();
    testVector_Random();
    testVector_RandomEmpty();
    testVector_RandomErase();
    testVector_InsertAt();
    testVector_InsertAtBegin();
    testVector_InsertAtEnd();
    testVector_CloneTo();
    testVector_Sort();
    testVector_SortSingleElement();
    testVector_SortAlreadySorted();
    testVector_Resize();
    testVector_ResizeDown();
    testVector_Reserve();
    testVector_GetDefault();
    testVector_GetWithCustomDefault();
    testVector_EqualityOp();
    testVector_InequalityOp();
    testVector_EqualityDifferentSize();
    testVector_Clear();
    testVector_ClearEmpty();
    testVector_IsEmpty();
    testVector_LargeVolumePushBack();
    testVector_EraseAllThenAdd();
    testVector_MixedOperations();
    testVector_AddRangeThenSort();
    testVector_SwapIndexMultiple();
    testVector_FindFirstIndexMultipleOccurrences();
    testVector_EraseElementNotExist();
    testVector_PopBackUntilEmpty();
    testVector_CloneToOverwrite();
    testVector_StringVector();
    testVector_IntegrationSortAndSearch();
    testVector_AddUniqueMultiple();
    testVector_InsertInMiddle();
    testVector_EraseAtMultipleSegments();
    testVector_EmplaceBack();
    testVector_ClearAndReserve();
    testVector_SetRangeCopy_String();
    testVector_ResetElement();
    testVector_ResetElementAt();
    testVector_DataPointer();
    testVector_IteratorBasic();
    testVector_ReverseIterator();
	testVector_Basic();
	testVector_AddUnique();
	testVector_Initializer();
	testVector_EraseAllElement();
	testContainerExtended();
}

// ============================================================
// Consolidated container smoke tests
// ============================================================
#define VECTORTEST_CT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("VectorTest FAIL: ") + (msg)); } } while(0)
#define VECTORTEST_CT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("VectorTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define VECTORTEST_CT_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("VectorTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void VectorTest::testVector_Basic()
{
	// default ctor -> empty
	Vector<int> v;
	VECTORTEST_CT_ASSERT(v.isEmpty(), "Vector default isEmpty");
	VECTORTEST_CT_ASSERT_EQ(v.size(), 0, "Vector default size=0");

	// add / size / operator[]
	v.add(10);
	v.add(20);
	v.add(30);
	VECTORTEST_CT_ASSERT_EQ(v.size(), 3, "Vector size after push 3");
	VECTORTEST_CT_ASSERT_EQ(v[0], 10, "Vector[0]=10");
	VECTORTEST_CT_ASSERT_EQ(v[1], 20, "Vector[1]=20");
	VECTORTEST_CT_ASSERT_EQ(v[2], 30, "Vector[2]=30");
	VECTORTEST_CT_ASSERT(!v.isEmpty(), "Vector not empty");

	// contains
	VECTORTEST_CT_ASSERT(v.contains(20),  "Vector contains 20");
	VECTORTEST_CT_ASSERT(!v.contains(99), "Vector not contains 99");

	// remove (removes first match)
	VECTORTEST_CT_ASSERT(v.remove(20), "Vector remove 20 ok");
	VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "Vector size=2 after erase");
	VECTORTEST_CT_ASSERT(!v.contains(20), "Vector 20 gone after erase");

	// removeAt
	v.removeAt(0);
	VECTORTEST_CT_ASSERT_EQ(v.size(), 1, "Vector size=1 after removeAt(0)");
	VECTORTEST_CT_ASSERT_EQ(v[0], 30, "Vector[0]=30 after removeAt");

	// eraseLast
	v.removeLast();
	VECTORTEST_CT_ASSERT_EQ(v.size(), 0, "Vector size=0 after eraseLast");

	// popBack
	v.add(100);
	v.add(200);
	const int popped = v.popBack(-1);
	VECTORTEST_CT_ASSERT_EQ(popped, 200, "Vector popBack=200");
	VECTORTEST_CT_ASSERT_EQ(v.size(), 1, "Vector size=1 after popBack");

	// clear
	v.add(1);
	v.add(2);
	v.clear();
	VECTORTEST_CT_ASSERT_EQ(v.size(), 0, "Vector size=0 after clear");
	VECTORTEST_CT_ASSERT(v.isEmpty(), "Vector isEmpty after clear");
}

void VectorTest::testVector_AddUnique()
{
	Vector<int> v;
	VECTORTEST_CT_ASSERT(v.addUnique(5),  "addUnique 5 first time = true");
	VECTORTEST_CT_ASSERT(!v.addUnique(5), "addUnique 5 again = false");
	VECTORTEST_CT_ASSERT_EQ(v.size(), 1, "addUnique size=1");

	v.addUnique(10);
	VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "addUnique size=2 after 10");
}

void VectorTest::testVector_Initializer()
{
	// initializer_list ctor
	Vector<int> v{ 1, 2, 3, 4, 5 };
	VECTORTEST_CT_ASSERT_EQ(v.size(), 5, "Vector init-list size=5");
	VECTORTEST_CT_ASSERT_EQ(v[4], 5, "Vector init-list [4]=5");
}

void VectorTest::testVector_EraseAllElement()
{
	Vector<int> v{ 1, 2, 2, 3, 2 };
	const int n = v.removeAll(2);
	VECTORTEST_CT_ASSERT_EQ(n, 3, "eraseAllElement count=3");
	VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "eraseAllElement size=2");
	VECTORTEST_CT_ASSERT(!v.contains(2), "eraseAllElement 2 gone");
}

void VectorTest::testContainerExtended()
{
// add large volume
	{
		Vector<int> v;
		FOR(1000)
		{
			v.add(i); 
		}
		VECTORTEST_CT_ASSERT_EQ(v.size(), 1000, "Vector large add size=1000");
		VECTORTEST_CT_ASSERT_EQ(v[999], 999, "Vector large [999]=999");
	}

	// contains / findFirstIndex
	{
		Vector<int> v;
		v.add(10); 
		v.add(20); 
		v.add(30);
		VECTORTEST_CT_ASSERT(v.contains(20), "Vector contains 20");
		VECTORTEST_CT_ASSERT(!v.contains(99), "Vector not contains 99");
		VECTORTEST_CT_ASSERT_EQ(v.findIndex(20), 1, "Vector findFirstIndex 20=1");
		VECTORTEST_CT_ASSERT_EQ(v.findIndex(99), -1, "Vector findFirstIndex 99=-1");
	}

	// removeAt single
	{
		Vector<int> v;
		v.add(1); 
		v.add(2);
		v.add(3);
		v.removeAt(1);
		VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "removeAt size=2");
		VECTORTEST_CT_ASSERT_EQ(v[0], 1, "removeAt [0]=1");
		VECTORTEST_CT_ASSERT_EQ(v[1], 3, "removeAt [1]=3");
	}

	// removeAt range
	{
		Vector<int> v;
		FOR(5)
		{
			v.add(i);
		}
		v.removeRange(1, 2); // erase index 1 and 2
		VECTORTEST_CT_ASSERT_EQ(v.size(), 3, "removeAt range size=3");
		VECTORTEST_CT_ASSERT_EQ(v[0], 0, "removeAt range [0]=0");
		VECTORTEST_CT_ASSERT_EQ(v[1], 3, "removeAt range [1]=3");
		VECTORTEST_CT_ASSERT_EQ(v[2], 4, "removeAt range [2]=4");
	}

	// remove
	{
		Vector<int> v;
		v.add(5); 
		v.add(10); 
		v.add(15);
		bool removed = v.remove(10);
		VECTORTEST_CT_ASSERT(removed, "remove found");
		VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "remove size=2");
		VECTORTEST_CT_ASSERT(!v.contains(10), "remove removed 10");
	}

	// eraseLast / popBack
	{
		Vector<int> v;
		v.add(1); 
		v.add(2); 
		v.add(3);
		v.removeLast();
		VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "eraseLast size=2");
		VECTORTEST_CT_ASSERT_EQ(v[v.size()-1], 2, "eraseLast last=2");

		int popped = v.popBack(0);
		VECTORTEST_CT_ASSERT_EQ(popped, 2, "popBack=2");
		VECTORTEST_CT_ASSERT_EQ(v.size(), 1, "popBack size=1");
	}

	// sort
	{
		Vector<int> v;
		v.add(5); 
		v.add(1);
		v.add(3);
		v.add(2);
		v.add(4);
		v.sort();
		for (int i = 0; i < (int)v.size() - 1; ++i)
		{
			VECTORTEST_CT_ASSERT(v[i] <= v[i+1], "Vector sort ascending");
		}
	}

	// addUnique
	{
		Vector<int> v;
		v.addUnique(1);
		v.addUnique(2);
		v.addUnique(1); // duplicate
		VECTORTEST_CT_ASSERT_EQ(v.size(), 2, "addUnique no dup");
	}

	// resize / reserve
	{
		Vector<int> v;
		v.reserve(50);
		VECTORTEST_CT_ASSERT_EQ(v.size(), 0, "reserve size=0");
		v.resize(10);
		FOR(10)
		{
			v[i] = 99;
		}
		VECTORTEST_CT_ASSERT_EQ(v.size(), 10, "resize size=10");
		VECTORTEST_CT_ASSERT_EQ(v[5], 99, "resize fill=99");
	}

	// operator== / operator!=
	{
		Vector<int> v1, v2;
		v1.add(1);
		v1.add(2);
		v2.add(1);
		v2.add(2);
		VECTORTEST_CT_ASSERT(v1 == v2, "Vector == equal");
		v2.add(3);
		VECTORTEST_CT_ASSERT(v1 != v2, "Vector != not equal");
	}

	// addRange
	{
		Vector<int> src;
		for (int i = 10; i < 15; ++i)
		{
			src.add(i);
		}
		Vector<int> dst;
		dst.add(1);
		dst.addRange(src);
		VECTORTEST_CT_ASSERT_EQ(dst.size(), 6, "addRange size=6");
		VECTORTEST_CT_ASSERT_EQ(dst[1], 10, "addRange [1]=10");
		VECTORTEST_CT_ASSERT_EQ(dst[5], 14, "addRange [5]=14");
	}

	// cloneTo
	{
		Vector<int> src;
		FOR(5)
		{
			src.add(i * i);
		}
		Vector<int> dst;
		src.cloneTo(dst);
		VECTORTEST_CT_ASSERT(src == dst, "cloneTo equal");
	}

	// swapIndex
	{
		Vector<int> v;
		v.add(10);
		v.add(20); 
		v.add(30);
		v.swapIndex(0, 2);
		VECTORTEST_CT_ASSERT_EQ(v[0], 30, "swapIndex [0]=30");
		VECTORTEST_CT_ASSERT_EQ(v[2], 10, "swapIndex [2]=10");
	}

	// eraseAllElement
	{
		Vector<int> v;
		v.add(5); 
		v.add(5); 
		v.add(3); 
		v.add(5);
		int cnt = v.removeAll(5);
		VECTORTEST_CT_ASSERT_EQ(cnt, 3, "eraseAllElement count=3");
		VECTORTEST_CT_ASSERT_EQ(v.size(), 1, "eraseAllElement remaining=1");
		VECTORTEST_CT_ASSERT_EQ(v[0], 3, "eraseAllElement remaining[0]=3");
	}

	// findElementCount
	{
		Vector<int> v;
		v.add(1); 
		v.add(2); 
		v.add(1); 
		v.add(1);
		VECTORTEST_CT_ASSERT_EQ(v.findCount(1), 3, "findElementCount 1=3");
		VECTORTEST_CT_ASSERT_EQ(v.findCount(2), 1, "findElementCount 2=1");
		VECTORTEST_CT_ASSERT_EQ(v.findCount(9), 0, "findElementCount 9=0");
	}

	// insert
	{
		Vector<int> v;
		v.add(1); 
		v.add(3);
		v.insert(1, 2);
		VECTORTEST_CT_ASSERT_EQ(v.size(), 3, "insert size=3");
		VECTORTEST_CT_ASSERT_EQ(v[1], 2, "insert [1]=2");
		VECTORTEST_CT_ASSERT_EQ(v[2], 3, "insert [2]=3");
	}

	// isAll (value equality)
	{
		Vector<int> v;
		v.add(2); 
		v.add(2); 
		v.add(2);
		VECTORTEST_CT_ASSERT(v.isAll(2), "isAll 2 true");
		v.add(3);
		VECTORTEST_CT_ASSERT(!v.isAll(2), "isAll 2 false after 3");
	}

	// string vector
	{
		Vector<string> sv;
		sv.add("apple"); 
		sv.add("banana"); 
		sv.add("cherry");
		VECTORTEST_CT_ASSERT_EQ(sv.size(), 3, "string vector size=3");
		VECTORTEST_CT_ASSERT(sv.contains("banana"), "string vector contains banana");
		sv.remove("banana");
		VECTORTEST_CT_ASSERT(!sv.contains("banana"), "string vector removed banana");
	}
//------------------------------------------------------------------------------
}
#endif
