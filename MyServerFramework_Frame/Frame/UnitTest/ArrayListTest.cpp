#ifdef WINDOWS
#include "FrameHeader.h"
namespace ArrayListTestSupport
{
    // ============================================================
    // ArrayListTest — ArrayList<Length,T> 全方法深度测试
    // legacy STL aggregate test 已覆盖：add(1值)/removeAt/clear/size/isEmpty/contains/operator[]/迭代器
    // ContainerTest 无 ArrayList 完整测试
    // 本文件覆盖所有未测试的方法：
    //   add(2~6值批量) / addNotEqual / addUnique / add(&&移动语义)
    //   remove / removeLast / remove
    //   isAll / addRange(ArrayList) / addRange(ptr,count) / setRange
    //   data() / data(offset) / get() / random() / setSize / resize
    //   zero() / setAll(val) / setAll(start,val) / maxSize / cbegin/cend
    // ============================================================

#define ArrayListTestSupport_ALE_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("ArrayListTest FAILED: ") + (msg)); }
#define ArrayListTestSupport_ALE_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("ArrayListTest FAILED: ") + (msg) + \
        " | got=" + IToS((int)(a))); }
}

void ArrayListTest::testALE_AddMulti_Two()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	bool r = al.add(10, 20);
	ArrayListTestSupport_ALE_ASSERT(r, "add(2) success");
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "add(2) size==2");
	ArrayListTestSupport_ALE_EQ(al[0], 10, "add(2) [0]==10");
	ArrayListTestSupport_ALE_EQ(al[1], 20, "add(2) [1]==20");
}

void ArrayListTest::testALE_AddMulti_Three()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	bool r = al.add(1, 2, 3);
	ArrayListTestSupport_ALE_ASSERT(r, "add(3) success");
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "add(3) size==3");
	ArrayListTestSupport_ALE_EQ(al[2], 3, "add(3) [2]==3");
}

void ArrayListTest::testALE_AddMulti_Four()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	bool r = al.add(10, 20, 30, 40);
	ArrayListTestSupport_ALE_ASSERT(r, "add(4) success");
	ArrayListTestSupport_ALE_EQ(al.size(), 4, "add(4) size==4");
	ArrayListTestSupport_ALE_EQ(al[3], 40, "add(4) [3]==40");
}

void ArrayListTest::testALE_AddMulti_Five()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	bool r = al.add(1, 2, 3, 4, 5);
	ArrayListTestSupport_ALE_ASSERT(r, "add(5) success");
	ArrayListTestSupport_ALE_EQ(al.size(), 5, "add(5) size==5");
	ArrayListTestSupport_ALE_EQ(al[4], 5, "add(5) [4]==5");
}

void ArrayListTest::testALE_AddMulti_Six()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	bool r = al.add(1, 2, 3, 4, 5, 6);
	ArrayListTestSupport_ALE_ASSERT(r, "add(6) success");
	ArrayListTestSupport_ALE_EQ(al.size(), 6, "add(6) size==6");
	ArrayListTestSupport_ALE_EQ(al[5], 6, "add(6) [5]==6");
}

void ArrayListTest::testALE_AddMulti_Full()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	bool r1 = al.add(1, 2);
	bool r2 = al.add(3, 4);
	ArrayListTestSupport_ALE_ASSERT(r1 && r2, "add two pairs success");
	ArrayListTestSupport_ALE_EQ(al.size(), 4, "two pairs size==4");
	// 容量已满，再加 2 个应失败
	bool r3 = al.add(5, 6);
	ArrayListTestSupport_ALE_ASSERT(!r3, "add(2) full fails");
	ArrayListTestSupport_ALE_EQ(al.size(), 4, "size unchanged after fail");
}

void ArrayListTest::testALE_AddMulti_OverflowThree()
{
	using namespace ArrayListTestSupport;
	ArrayList<5, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	// 剩余 2 个，add(3) 需要 3 个位置应失败
	bool r = al.add(4, 5, 6);
	ArrayListTestSupport_ALE_ASSERT(!r, "add(3) overflow fails");
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "size unchanged after overflow");
}

void ArrayListTest::testALE_AddNotEqual_AddWhenDiff()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	bool r = al.addNotEqual(5, 0);  // 5 != 0，添加
	ArrayListTestSupport_ALE_ASSERT(r, "addNotEqual adds when diff");
	ArrayListTestSupport_ALE_EQ(al.size(), 1, "addNotEqual size==1");
	ArrayListTestSupport_ALE_EQ(al[0], 5, "addNotEqual [0]==5");
}

void ArrayListTest::testALE_AddNotEqual_SkipWhenSame()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	bool r = al.addNotEqual(5, 5);  // 相等，不添加
	ArrayListTestSupport_ALE_ASSERT(!r, "addNotEqual skips when same");
	ArrayListTestSupport_ALE_EQ(al.size(), 0, "addNotEqual size==0");
}

void ArrayListTest::testALE_AddNotEqual_Mixed()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.addNotEqual(1, 0);  // adds 1
	al.addNotEqual(0, 0);  // skips
	al.addNotEqual(2, 0);  // adds 2
	al.addNotEqual(0, 0);  // skips
	al.addNotEqual(3, 0);  // adds 3
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "addNotEqual mixed size==3");
	ArrayListTestSupport_ALE_EQ(al[0], 1, "addNotEqual [0]==1");
	ArrayListTestSupport_ALE_EQ(al[1], 2, "addNotEqual [1]==2");
	ArrayListTestSupport_ALE_EQ(al[2], 3, "addNotEqual [2]==3");
}

void ArrayListTest::testALE_AddNotEqual_Full()
{
	using namespace ArrayListTestSupport;
	ArrayList<2, int> al;
	al.addNotEqual(1, 0);
	al.addNotEqual(2, 0);
	bool r = al.addNotEqual(3, 0);  // full
	ArrayListTestSupport_ALE_ASSERT(!r, "addNotEqual full fails");
}

void ArrayListTest::testALE_AddUnique_Basic()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	bool r1 = al.addUnique(10);
	bool r2 = al.addUnique(20);
	bool r3 = al.addUnique(10);  // dup
	ArrayListTestSupport_ALE_ASSERT(r1, "addUnique 10 first true");
	ArrayListTestSupport_ALE_ASSERT(r2, "addUnique 20 true");
	ArrayListTestSupport_ALE_ASSERT(!r3, "addUnique 10 dup false");
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "addUnique size==2");
}

void ArrayListTest::testALE_AddUnique_Empty()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	bool r = al.addUnique(0);
	ArrayListTestSupport_ALE_ASSERT(r, "addUnique to empty true");
	ArrayListTestSupport_ALE_EQ(al.size(), 1, "addUnique size==1");
}

void ArrayListTest::testALE_AddUnique_AllUnique()
{
	using namespace ArrayListTestSupport;
	ArrayList<10, int> al;
	for (int i = 0; i < 10; ++i)
	{
	    bool r = al.addUnique(i);
	    ArrayListTestSupport_ALE_ASSERT(r, "addUnique all unique");
	}
	ArrayListTestSupport_ALE_EQ(al.size(), 10, "addUnique all unique size==10");
}

void ArrayListTest::testALE_AddUnique_Full()
{
	using namespace ArrayListTestSupport;
	ArrayList<3, int> al;
	al.addUnique(1);
	al.addUnique(2);
	al.addUnique(3);
	bool r = al.addUnique(4);  // full
	ArrayListTestSupport_ALE_ASSERT(!r, "addUnique full returns false");
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "addUnique full size unchanged");
}

void ArrayListTest::testALE_AddMove_String()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, string> al;
	string s1 = "moved_str1";
	string s2 = "moved_str2";
	bool r1 = al.add(move(s1));
	bool r2 = al.add(move(s2));
	ArrayListTestSupport_ALE_ASSERT(r1, "add(move) r1 true");
	ArrayListTestSupport_ALE_ASSERT(r2, "add(move) r2 true");
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "add(move) size==2");
	ArrayListTestSupport_ALE_ASSERT(al[0] == "moved_str1", "add(move) [0]==moved_str1");
	ArrayListTestSupport_ALE_ASSERT(al[1] == "moved_str2", "add(move) [1]==moved_str2");
}

void ArrayListTest::testALE_AddMove_Int()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	int v = 99;
	al.add(move(v));
	ArrayListTestSupport_ALE_EQ(al[0], 99, "add(move int) [0]==99");
}

void ArrayListTest::testALE_EraseElement_Single()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	al.removeAll(2);
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "removeAll single size==2");
	ArrayListTestSupport_ALE_ASSERT(!al.contains(2), "removeAll single 2 gone");
	ArrayListTestSupport_ALE_EQ(al[0], 1, "removeAll [0]==1");
	ArrayListTestSupport_ALE_EQ(al[1], 3, "removeAll [1]==3");
}

void ArrayListTest::testALE_EraseElement_Multiple()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	al.add(1);
	al.add(2);
	al.add(1);
	al.add(3);
	al.add(1);
	al.removeAll(1);
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "removeAll all 1s size==2");
	ArrayListTestSupport_ALE_ASSERT(!al.contains(1), "removeAll 1 gone all");
	ArrayListTestSupport_ALE_EQ(al[0], 2, "removeAll [0]==2");
	ArrayListTestSupport_ALE_EQ(al[1], 3, "removeAll [1]==3");
}

void ArrayListTest::testALE_EraseElement_NotExist()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.removeAll(99);  // not found, no-op
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "removeAll notExist size unchanged");
}

void ArrayListTest::testALE_EraseElement_AllSame()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	for (int i = 0; i < 5; ++i)
	{
	    al.add(7);
	}
	al.removeAll(7);
	ArrayListTestSupport_ALE_EQ(al.size(), 0, "removeAll all same cleared");
	ArrayListTestSupport_ALE_ASSERT(al.isEmpty(), "removeAll all same isEmpty");
}

void ArrayListTest::testALE_EraseLastElement_Basic()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	al.add(2);
	bool r = al.removeLast(2);
	ArrayListTestSupport_ALE_ASSERT(r, "removeLast success");
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "removeLast size==3");
	ArrayListTestSupport_ALE_EQ(al[1], 2, "removeLast first 2 still at [1]");
	ArrayListTestSupport_ALE_EQ(al[2], 3, "removeLast [2]==3");
}

void ArrayListTest::testALE_EraseLastElement_NotExist()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	bool r = al.removeLast(99);
	ArrayListTestSupport_ALE_ASSERT(!r, "removeLast notExist false");
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "removeLast size unchanged");
}

void ArrayListTest::testALE_EraseLastElement_Single()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(5);
	bool r = al.removeLast(5);
	ArrayListTestSupport_ALE_ASSERT(r, "removeLast single success");
	ArrayListTestSupport_ALE_EQ(al.size(), 0, "removeLast single size==0");
}

void ArrayListTest::testALE_EraseFirstElement_Basic()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(2);
	al.add(1);
	al.add(2);
	al.add(3);
	bool r = al.remove(2);
	ArrayListTestSupport_ALE_ASSERT(r, "remove success");
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "remove size==3");
	ArrayListTestSupport_ALE_EQ(al[0], 1, "remove [0]==1");
	ArrayListTestSupport_ALE_EQ(al[1], 2, "remove second 2 still at [1]");
}

void ArrayListTest::testALE_EraseFirstElement_NotExist()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	bool r = al.remove(99);
	ArrayListTestSupport_ALE_ASSERT(!r, "remove notExist false");
}

void ArrayListTest::testALE_EraseFirstElement_OnlyOne()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(42);
	bool r = al.remove(42);
	ArrayListTestSupport_ALE_ASSERT(r, "remove only one success");
	ArrayListTestSupport_ALE_ASSERT(al.isEmpty(), "remove only one isEmpty");
}

void ArrayListTest::testALE_IsAll_True()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(5);
	al.add(5);
	al.add(5);
	ArrayListTestSupport_ALE_ASSERT(al.isAll(5), "isAll 5 true");
}

void ArrayListTest::testALE_IsAll_False()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(5);
	al.add(5);
	al.add(3);
	ArrayListTestSupport_ALE_ASSERT(!al.isAll(5), "isAll 5 false");
}

void ArrayListTest::testALE_IsAll_Empty()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	// 空列表，isAll 应返回 true（无元素则全部满足）
	ArrayListTestSupport_ALE_ASSERT(al.isAll(0), "isAll empty true");
}

void ArrayListTest::testALE_IsAll_Single()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(99);
	ArrayListTestSupport_ALE_ASSERT(al.isAll(99), "isAll single true");
	ArrayListTestSupport_ALE_ASSERT(!al.isAll(0), "isAll single false");
}

void ArrayListTest::testALE_AddRange_ArrayList()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> dst;
	dst.add(1);
	dst.add(2);
	ArrayList<8, int> src;
	src.add(3);
	src.add(4);
	src.add(5);
	bool r = dst.addRange(src);
	ArrayListTestSupport_ALE_ASSERT(r, "addRange(ArrayList) success");
	ArrayListTestSupport_ALE_EQ(dst.size(), 5, "addRange size==5");
	ArrayListTestSupport_ALE_EQ(dst[2], 3, "addRange [2]==3");
	ArrayListTestSupport_ALE_EQ(dst[4], 5, "addRange [4]==5");
}

void ArrayListTest::testALE_AddRange_ArrayList_Empty()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> dst;
	dst.add(1);
	ArrayList<4, int> src;  // empty
	bool r = dst.addRange(src);
	ArrayListTestSupport_ALE_ASSERT(r, "addRange empty src success");
	ArrayListTestSupport_ALE_EQ(dst.size(), 1, "addRange empty src size unchanged");
}

void ArrayListTest::testALE_AddRange_Ptr()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> dst;
	int src[] = { 10, 20, 30, 40, 50 };
	bool r = dst.addRange(src, 5);
	ArrayListTestSupport_ALE_ASSERT(r, "addRange(ptr,5) success");
	ArrayListTestSupport_ALE_EQ(dst.size(), 5, "addRange ptr size==5");
	ArrayListTestSupport_ALE_EQ(dst[0], 10, "addRange ptr [0]==10");
	ArrayListTestSupport_ALE_EQ(dst[4], 50, "addRange ptr [4]==50");
}

void ArrayListTest::testALE_AddRange_Ptr_Partial()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> dst;
	int src[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	bool r = dst.addRange(src, 4);  // only first 4
	ArrayListTestSupport_ALE_ASSERT(r, "addRange ptr partial success");
	ArrayListTestSupport_ALE_EQ(dst.size(), 4, "addRange ptr partial size==4");
	ArrayListTestSupport_ALE_EQ(dst[3], 4, "addRange ptr partial [3]==4");
}

void ArrayListTest::testALE_AddRange_Ptr_AppendToExisting()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> dst;
	dst.add(1);
	dst.add(2);
	int src[] = { 3, 4, 5 };
	bool r = dst.addRange(src, 3);
	ArrayListTestSupport_ALE_ASSERT(r, "addRange ptr append success");
	ArrayListTestSupport_ALE_EQ(dst.size(), 5, "addRange ptr append size==5");
	ArrayListTestSupport_ALE_EQ(dst[2], 3, "addRange ptr append [2]==3");
}

void ArrayListTest::testALE_SetRange_Basic()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	al.add(100);
	al.add(200);
	int src[] = { 1, 2, 3 };
	bool r = al.setRange(src, 3);
	ArrayListTestSupport_ALE_ASSERT(r, "setRange success");
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "setRange size==3");
	ArrayListTestSupport_ALE_EQ(al[0], 1, "setRange [0]==1");
	ArrayListTestSupport_ALE_EQ(al[2], 3, "setRange [2]==3");
}

void ArrayListTest::testALE_SetRange_ClearsFirst()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	for (int i = 0; i < 10; ++i)
	{
	    al.add(i);
	}
	ArrayListTestSupport_ALE_EQ(al.size(), 10, "before setRange size==10");
	int src[] = { 99 };
	al.setRange(src, 1);
	ArrayListTestSupport_ALE_EQ(al.size(), 1, "setRange clears then adds size==1");
	ArrayListTestSupport_ALE_EQ(al[0], 99, "setRange [0]==99");
}

void ArrayListTest::testALE_Data_Basic()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	const int* ptr = al.data();
	ArrayListTestSupport_ALE_ASSERT(ptr != nullptr, "data() non-null");
	ArrayListTestSupport_ALE_EQ(ptr[0], 1, "data()[0]==1");
	ArrayListTestSupport_ALE_EQ(ptr[1], 2, "data()[1]==2");
	ArrayListTestSupport_ALE_EQ(ptr[2], 3, "data()[2]==3");
}

void ArrayListTest::testALE_Data_Mutable()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(10);
	al.add(20);
	int* ptr = al.data();
	ptr[0] = 100;
	ArrayListTestSupport_ALE_EQ(al[0], 100, "data mutable modify [0]==100");
}

void ArrayListTest::testALE_Data_Offset()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	for (int i = 0; i < 5; ++i)
	{
	    al.add(i * 10);
	}
	const int* p = al.data(2);
	ArrayListTestSupport_ALE_ASSERT(p != nullptr, "data(2) non-null");
	ArrayListTestSupport_ALE_EQ(p[0], 20, "data(2)[0]==20");
	ArrayListTestSupport_ALE_EQ(p[1], 30, "data(2)[1]==30");
	ArrayListTestSupport_ALE_EQ(p[2], 40, "data(2)[2]==40");
}

void ArrayListTest::testALE_Data_Offset_Zero()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	al.add(5);
	al.add(6);
	const int* p = al.data(0);
	ArrayListTestSupport_ALE_ASSERT(p != nullptr, "data(0) non-null");
	ArrayListTestSupport_ALE_EQ(p[0], 5, "data(0)[0]==5");
}

void ArrayListTest::testALE_Get_Basic()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(10);
	al.add(20);
	al.add(30);
	ArrayListTestSupport_ALE_EQ(al.get(0), 10, "get(0)==10");
	ArrayListTestSupport_ALE_EQ(al.get(1), 20, "get(1)==20");
	ArrayListTestSupport_ALE_EQ(al.get(2), 30, "get(2)==30");
}

void ArrayListTest::testALE_Get_OutOfBounds()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	al.add(42);
	// get() 越界返回 mDefault（int 的 0）
	int v = al.get(99);
	ArrayListTestSupport_ALE_EQ(v, 0, "get(OOB) returns default 0");
}

void ArrayListTest::testALE_Get_Negative()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	al.add(99);
	int v = al.get(-1);
	ArrayListTestSupport_ALE_EQ(v, 0, "get(-1) returns default 0");
}

void ArrayListTest::testALE_Random_Single()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	al.add(42);
	ArrayListTestSupport_ALE_EQ(al.random(), 42, "random single==42");
}

void ArrayListTest::testALE_Random_Multiple()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	// 多次调用，结果都应在集合内
	for (int i = 0; i < 20; ++i)
	{
	    int v = al.random();
	    ArrayListTestSupport_ALE_ASSERT(v >= 1 && v <= 3, "random in [1,3]");
	}
}

void ArrayListTest::testALE_Random_Empty()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, int> al;
	int v = al.random();
	ArrayListTestSupport_ALE_EQ(v, 0, "random empty returns default 0");
}

void ArrayListTest::testALE_SetSize_Expand()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	al.add(1);
	al.add(2);
	al.setSize(5);
	ArrayListTestSupport_ALE_EQ(al.size(), 5, "setSize(5) size==5");
	ArrayListTestSupport_ALE_EQ(al[0], 1, "setSize [0]==1 unchanged");
	ArrayListTestSupport_ALE_EQ(al[1], 2, "setSize [1]==2 unchanged");
	ArrayListTestSupport_ALE_EQ(al[2], 0, "setSize [2]==default 0");
	ArrayListTestSupport_ALE_EQ(al[4], 0, "setSize [4]==default 0");
}

void ArrayListTest::testALE_SetSize_SameSize()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(10);
	al.add(20);
	al.setSize(2);  // same size, no-op
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "setSize same size unchanged");
}

void ArrayListTest::testALE_SetSize_Smaller()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	al.setSize(1);  // smaller than current, no effect per implementation
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "setSize smaller no effect");
}

void ArrayListTest::testALE_Resize_Up()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	al.add(1);
	al.add(2);
	al.resize(8);
	ArrayListTestSupport_ALE_EQ(al.size(), 8, "resize(8) size==8");
	// 扩展部分仍是内存中的原有值（可能是 0）
}

void ArrayListTest::testALE_Resize_Down()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	for (int i = 0; i < 6; ++i)
	{
	    al.add(i);
	}
	al.resize(3);
	ArrayListTestSupport_ALE_EQ(al.size(), 3, "resize(3) size==3");
	ArrayListTestSupport_ALE_EQ(al[0], 0, "resize down [0]==0");
	ArrayListTestSupport_ALE_EQ(al[2], 2, "resize down [2]==2");
}

void ArrayListTest::testALE_Resize_Zero()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.resize(0);
	ArrayListTestSupport_ALE_EQ(al.size(), 0, "resize(0) size==0");
	ArrayListTestSupport_ALE_ASSERT(al.isEmpty(), "resize(0) isEmpty");
}

void ArrayListTest::testALE_Zero_Int()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	al.zero();
	// zero 只清零内存，不改变 mSize
	for (int i = 0; i < 8; ++i)
	{
	    ArrayListTestSupport_ALE_EQ(al.mValue[i], 0, "zero int all zero");
	}
}

void ArrayListTest::testALE_Zero_Float()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, float> al;
	al.add(1.5f);
	al.add(2.5f);
	al.zero();
	ArrayListTestSupport_ALE_ASSERT(fabsf(al.mValue[0]) < 1e-7f, "zero float [0]==0");
	ArrayListTestSupport_ALE_ASSERT(fabsf(al.mValue[1]) < 1e-7f, "zero float [1]==0");
}

void ArrayListTest::testALE_FillArray_Int()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.setAll(99);
	for (int i = 0; i < 8; ++i)
	{
	    ArrayListTestSupport_ALE_EQ(al.mValue[i], 99, "setAll 99");
	}
}

void ArrayListTest::testALE_FillArray_Float()
{
	using namespace ArrayListTestSupport;
	ArrayList<4, float> al;
	al.setAll(3.14f);
	for (int i = 0; i < 4; ++i)
	{
	    ArrayListTestSupport_ALE_ASSERT(fabsf(al.mValue[i] - 3.14f) < 1e-4f, "setAll 3.14f");
	}
}

void ArrayListTest::testALE_FillArray_AfterAdd()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.setAll(0);
	for (int i = 0; i < 8; ++i)
	{
	    ArrayListTestSupport_ALE_EQ(al.mValue[i], 0, "setAll after add all 0");
	}
	// mSize 未变（setAll 不修改 size）
	ArrayListTestSupport_ALE_EQ(al.size(), 2, "setAll size unchanged");
}

void ArrayListTest::testALE_FillArrayStart_Mid()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.setAll(1);
	al.setAll(4, 9);
	for (int i = 0; i < 4; ++i)
	{
	    ArrayListTestSupport_ALE_EQ(al.mValue[i], 1, "setAll(4,9) first 4 == 1");
	}
	for (int i = 4; i < 8; ++i)
	{
	    ArrayListTestSupport_ALE_EQ(al.mValue[i], 9, "setAll(4,9) last 4 == 9");
	}
}

void ArrayListTest::testALE_FillArrayStart_Zero()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.setAll(5);
	al.setAll(0, 7);  // from start, replaces all
	for (int i = 0; i < 8; ++i)
	{
	    ArrayListTestSupport_ALE_EQ(al.mValue[i], 7, "setAll(0,7) all 7");
	}
}

void ArrayListTest::testALE_MaxSize()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	ArrayListTestSupport_ALE_EQ(al.maxSize(), 16, "maxSize==16");
	ArrayList<128, int> al2;
	ArrayListTestSupport_ALE_EQ(al2.maxSize(), 128, "maxSize==128");
}

void ArrayListTest::testALE_CBeginCEnd()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(10);
	al.add(20);
	al.add(30);
	const ArrayList<8, int>& cal = al;
	int sum = 0;
	for (auto it = cal.cbegin(); it != cal.cend(); ++it)
	{
	    sum += *it;
	}
	ArrayListTestSupport_ALE_EQ(sum, 60, "cbegin/cend sum==60");
}

void ArrayListTest::testALE_BeginEnd_Mutable()
{
	using namespace ArrayListTestSupport;
	ArrayList<8, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	for (int* it = al.begin(); it != al.end(); ++it)
	{
	    *it *= 2;
	}
	ArrayListTestSupport_ALE_EQ(al[0], 2, "begin/end mutable [0]==2");
	ArrayListTestSupport_ALE_EQ(al[1], 4, "begin/end mutable [1]==4");
	ArrayListTestSupport_ALE_EQ(al[2], 6, "begin/end mutable [2]==6");
}

void ArrayListTest::testALE_BatchProcess()
{
	using namespace ArrayListTestSupport;
	ArrayList<32, int> al;
	// 批量添加
	int batch[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	al.addRange(batch, 8);
	ArrayListTestSupport_ALE_EQ(al.size(), 8, "batch size==8");

	// 移除所有偶数
	ArrayList<16, int> toRemove;
	for (int i = 0; i < al.size(); ++i)
	{
	    if (al[i] % 2 == 0)
	    {
	        toRemove.add(al[i]);
	    }
	}
	for (int i = 0; i < toRemove.size(); ++i)
	{
	    al.removeAll(toRemove[i]);
	}
	// 只剩奇数：1,3,5,7
	ArrayListTestSupport_ALE_EQ(al.size(), 4, "batch after erase evens size==4");
	ArrayListTestSupport_ALE_ASSERT(al.isAll(1) || !al.isAll(1), "isAll check not crash");
}

void ArrayListTest::testALE_SortLikePattern()
{
	using namespace ArrayListTestSupport;
	ArrayList<16, int> al;
	al.add(5);
	al.add(3);
	al.add(8);
	al.add(1);
	al.add(9);
	al.add(2);
	// 手工冒泡排序
	for (int i = 0; i < al.size() - 1; ++i)
	{
	    for (int j = 0; j < al.size() - 1 - i; ++j)
	    {
	        if (al[j] > al[j + 1])
	        {
	            int tmp = al[j];
	            al[j] = al[j + 1];
	            al[j + 1] = tmp;
	        }
	    }
	}
	ArrayListTestSupport_ALE_EQ(al[0], 1, "sort [0]==1");
	ArrayListTestSupport_ALE_EQ(al[5], 9, "sort [5]==9");
}

void ArrayListTest::testALE_AddRangeFromPtr_ThenSearch()
{
	using namespace ArrayListTestSupport;
	ArrayList<32, int> al;
	int data[] = { 10, 20, 30, 40, 50, 60, 70, 80 };
	al.addRange(data, 8);
	ArrayListTestSupport_ALE_ASSERT(al.contains(50), "contains 50");
	ArrayListTestSupport_ALE_ASSERT(!al.contains(99), "not contains 99");
	al.removeAll(50);
	ArrayListTestSupport_ALE_ASSERT(!al.contains(50), "after erase 50 gone");
	ArrayListTestSupport_ALE_EQ(al.size(), 7, "after erase size==7");
}

namespace ArrayListExt2TestSupport
{
#define ArrayListExt2TestSupport_ALE2_ASSERT(expr) \
	if (!(expr)) { ERROR(string("ArrayListExt2Test FAILED: ") + (#expr)); }

    // ========== removeAt ==========




    // ========== contains ==========




    // ========== maxSize ==========


    // ========== clear and reuse ==========


    // ========== multi-argument add ==========






    // ========== addNotEqual ==========


    // ========== addUnique ==========



    // ========== get out-of-bounds default ==========


    // ========== random single element ==========


    // ========== operator[] write ==========


    // ========== isAll ==========


    // ========== addRange from same-type ==========


    // ========== setRange clears first ==========


    // ========== setAll partial ==========


    // ========== iterator mutable ==========
}

void ArrayListTest::testALE2_EraseAtBasic()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(10);
	a.add(20);
	a.add(30);
	a.removeAt(1);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 10);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 30);
}

void ArrayListTest::testALE2_EraseAtFirst()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(1);
	a.add(2);
	a.add(3);
	a.removeAt(0);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 3);
}

void ArrayListTest::testALE2_EraseAtLast()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(100);
	a.add(200);
	a.add(300);
	a.removeAt(2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 100);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 200);
}

void ArrayListTest::testALE2_ContainsTrue()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(5);
	a.add(10);
	a.add(15);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.contains(5));
	ArrayListExt2TestSupport_ALE2_ASSERT(a.contains(10));
	ArrayListExt2TestSupport_ALE2_ASSERT(a.contains(15));
}

void ArrayListTest::testALE2_ContainsFalse()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(5);
	a.add(10);
	ArrayListExt2TestSupport_ALE2_ASSERT(!a.contains(3));
	ArrayListExt2TestSupport_ALE2_ASSERT(!a.contains(100));
}

void ArrayListTest::testALE2_ContainsEmpty()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	ArrayListExt2TestSupport_ALE2_ASSERT(!a.contains(0));
	ArrayListExt2TestSupport_ALE2_ASSERT(!a.contains(42));
}

void ArrayListTest::testALE2_MaxSize()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<16, int> a;
	ArrayListExt2TestSupport_ALE2_ASSERT(a.maxSize() == 16);
	ArrayList<4, float> b;
	ArrayListExt2TestSupport_ALE2_ASSERT(b.maxSize() == 4);
	ArrayList<100, short> c;
	ArrayListExt2TestSupport_ALE2_ASSERT(c.maxSize() == 100);
}

void ArrayListTest::testALE2_ClearAndReuse()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(1);
	a.add(2);
	a.add(3);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 3);
	a.clear();
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 0);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.isEmpty());
	a.add(99);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 1);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 99);
}

void ArrayListTest::testALE2_AddSixElements()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	bool ok = a.add(1, 2, 3, 4, 5, 6);
	ArrayListExt2TestSupport_ALE2_ASSERT(ok);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 6);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 1);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[5] == 6);
}

void ArrayListTest::testALE2_AddFiveElements()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	bool ok = a.add(10, 20, 30, 40, 50);
	ArrayListExt2TestSupport_ALE2_ASSERT(ok);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 5);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[4] == 50);
}

void ArrayListTest::testALE2_AddFourElements()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	bool ok = a.add(1, 2, 3, 4);
	ArrayListExt2TestSupport_ALE2_ASSERT(ok);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 4);
}

void ArrayListTest::testALE2_AddThreeElements()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	bool ok = a.add(11, 22, 33);
	ArrayListExt2TestSupport_ALE2_ASSERT(ok);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 3);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[2] == 33);
}

void ArrayListTest::testALE2_AddTwoElements()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	bool ok = a.add(7, 8);
	ArrayListExt2TestSupport_ALE2_ASSERT(ok);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 7);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 8);
}

void ArrayListTest::testALE2_AddNotEqualMixed()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	// addNotEqual(val, other): add val only if val != other
	a.addNotEqual(5, 3);
	a.addNotEqual(3, 3);
	a.addNotEqual(7, 0);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 5);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 7);
}

void ArrayListTest::testALE2_AddUniqueAllSame()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.addUnique(5);
	a.addUnique(5);
	a.addUnique(5);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 1);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 5);
}

void ArrayListTest::testALE2_AddUniqueAllDifferent()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.addUnique(1);
	a.addUnique(2);
	a.addUnique(3);
	a.addUnique(4);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 4);
}

void ArrayListTest::testALE2_GetOutOfBoundsDefault()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(10);
	// get with out-of-bounds index returns mDefault (0 for int)
	int v = a.get(99);
	ArrayListExt2TestSupport_ALE2_ASSERT(v == 0);
	int v2 = a.get(-1);
	ArrayListExt2TestSupport_ALE2_ASSERT(v2 == 0);
}

void ArrayListTest::testALE2_RandomSingleElem()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(42);
	int v = a.random();
	ArrayListExt2TestSupport_ALE2_ASSERT(v == 42);
}

void ArrayListTest::testALE2_OperatorBracketWrite()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(1);
	a.add(2);
	a.add(3);
	a[1] = 99;
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 99);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 1);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[2] == 3);
}

void ArrayListTest::testALE2_IsAllMixed()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(5);
	a.add(5);
	a.add(5);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.isAll(5));
	a.add(6);
	ArrayListExt2TestSupport_ALE2_ASSERT(!a.isAll(5));
}

void ArrayListTest::testALE2_AddRangeFromSelf()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> src;
	src.add(10);
	src.add(20);
	ArrayList<16, int> dst;
	dst.add(1);
	dst.addRange(src);
	ArrayListExt2TestSupport_ALE2_ASSERT(dst.size() == 3);
	ArrayListExt2TestSupport_ALE2_ASSERT(dst[0] == 1);
	ArrayListExt2TestSupport_ALE2_ASSERT(dst[1] == 10);
	ArrayListExt2TestSupport_ALE2_ASSERT(dst[2] == 20);
}

void ArrayListTest::testALE2_SetRangeClearsFirst()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(1);
	a.add(2);
	a.add(3);
	int arr[] = { 99, 88 };
	a.setRange(arr, 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a.size() == 2);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[0] == 99);
	ArrayListExt2TestSupport_ALE2_ASSERT(a[1] == 88);
}

void ArrayListTest::testALE2_FillArrayPartial()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.resize(8);
	a.setAll(2, -1);
	for (int i = 2; i < 8; ++i)
	{
	    ArrayListExt2TestSupport_ALE2_ASSERT(a[i] == -1);
	}
}

void ArrayListTest::testALE2_IteratorMutable()
{
	using namespace ArrayListExt2TestSupport;
	ArrayList<8, int> a;
	a.add(1);
	a.add(2);
	a.add(3);
	int sum = 0;
	for (int* it = a.begin(); it != a.end(); ++it)
	{
	    sum += *it;
	}
	ArrayListExt2TestSupport_ALE2_ASSERT(sum == 6);
}


void ArrayListTest::test()
{
    testALE_AddMulti_Two();
    testALE_AddMulti_Three();
    testALE_AddMulti_Four();
    testALE_AddMulti_Five();
    testALE_AddMulti_Six();
    testALE_AddMulti_Full();
    testALE_AddMulti_OverflowThree();
    testALE_AddNotEqual_AddWhenDiff();
    testALE_AddNotEqual_SkipWhenSame();
    testALE_AddNotEqual_Mixed();
    testALE_AddNotEqual_Full();
    testALE_AddUnique_Basic();
    testALE_AddUnique_Empty();
    testALE_AddUnique_AllUnique();
    testALE_AddUnique_Full();
    testALE_AddMove_String();
    testALE_AddMove_Int();
    testALE_EraseElement_Single();
    testALE_EraseElement_Multiple();
    testALE_EraseElement_NotExist();
    testALE_EraseElement_AllSame();
    testALE_EraseLastElement_Basic();
    testALE_EraseLastElement_NotExist();
    testALE_EraseLastElement_Single();
    testALE_EraseFirstElement_Basic();
    testALE_EraseFirstElement_NotExist();
    testALE_EraseFirstElement_OnlyOne();
    testALE_IsAll_True();
    testALE_IsAll_False();
    testALE_IsAll_Empty();
    testALE_IsAll_Single();
    testALE_AddRange_ArrayList();
    testALE_AddRange_ArrayList_Empty();
    testALE_AddRange_Ptr();
    testALE_AddRange_Ptr_Partial();
    testALE_AddRange_Ptr_AppendToExisting();
    testALE_SetRange_Basic();
    testALE_SetRange_ClearsFirst();
    testALE_Data_Basic();
    testALE_Data_Mutable();
    testALE_Data_Offset();
    testALE_Data_Offset_Zero();
    testALE_Get_Basic();
    testALE_Get_OutOfBounds();
    testALE_Get_Negative();
    testALE_Random_Single();
    testALE_Random_Multiple();
    testALE_Random_Empty();
    testALE_SetSize_Expand();
    testALE_SetSize_SameSize();
    testALE_SetSize_Smaller();
    testALE_Resize_Up();
    testALE_Resize_Down();
    testALE_Resize_Zero();
    testALE_Zero_Int();
    testALE_Zero_Float();
    testALE_FillArray_Int();
    testALE_FillArray_Float();
    testALE_FillArray_AfterAdd();
    testALE_FillArrayStart_Mid();
    testALE_FillArrayStart_Zero();
    testALE_MaxSize();
    testALE_CBeginCEnd();
    testALE_BeginEnd_Mutable();
    testALE_BatchProcess();
    testALE_SortLikePattern();
    testALE_AddRangeFromPtr_ThenSearch();
    testALE2_EraseAtBasic();
    testALE2_EraseAtFirst();
    testALE2_EraseAtLast();
    testALE2_ContainsTrue();
    testALE2_ContainsFalse();
    testALE2_ContainsEmpty();
    testALE2_MaxSize();
    testALE2_ClearAndReuse();
    testALE2_AddSixElements();
    testALE2_AddFiveElements();
    testALE2_AddFourElements();
    testALE2_AddThreeElements();
    testALE2_AddTwoElements();
    testALE2_AddNotEqualMixed();
    testALE2_AddUniqueAllSame();
    testALE2_AddUniqueAllDifferent();
    testALE2_GetOutOfBoundsDefault();
    testALE2_RandomSingleElem();
    testALE2_OperatorBracketWrite();
    testALE2_IsAllMixed();
    testALE2_AddRangeFromSelf();
    testALE2_SetRangeClearsFirst();
    testALE2_FillArrayPartial();
    testALE2_IteratorMutable();
	testSTLBasic();
	testSTLAdvanced();
}

// ============================================================
// Consolidated from legacy STL aggregate test::testArrayList
// ============================================================
#ifndef ARRAYLISTTEST_STL_ASSERT
#define ARRAYLISTTEST_STL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("ArrayListTest FAIL: ") + (msg)); } } while(0)
#endif
#ifndef ARRAYLISTTEST_STL_ASSERT_EQ
#define ARRAYLISTTEST_STL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("ArrayListTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#endif
#ifndef ARRAYLISTTEST_STL_ASSERT_STR
#define ARRAYLISTTEST_STL_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("ArrayListTest FAIL: ") + (msg)); } } while(0)
#endif

void ArrayListTest::testSTLBasic()
{
	// default: empty
	ArrayList<16, int> list;
	ARRAYLISTTEST_STL_ASSERT(list.isEmpty(), "ArrayList default isEmpty");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 0, "ArrayList default size=0");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.maxSize(), 16, "ArrayList maxSize=16");

	// add single element
	ARRAYLISTTEST_STL_ASSERT(list.add(10), "ArrayList add 10");
	ARRAYLISTTEST_STL_ASSERT(list.add(20), "ArrayList add 20");
	ARRAYLISTTEST_STL_ASSERT(list.add(30), "ArrayList add 30");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 3, "ArrayList size=3");
	ARRAYLISTTEST_STL_ASSERT(!list.isEmpty(), "ArrayList not isEmpty");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[0], 10, "ArrayList[0]=10");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[1], 20, "ArrayList[1]=20");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[2], 30, "ArrayList[2]=30");

	// get (bounds-safe)
	ARRAYLISTTEST_STL_ASSERT_EQ(list.get(1), 20, "ArrayList get(1)=20");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.get(-1), 0, "ArrayList get(-1)=default");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.get(100), 0, "ArrayList get(100)=default");

	// contains
	ARRAYLISTTEST_STL_ASSERT(list.contains(20),  "ArrayList contains 20");
	ARRAYLISTTEST_STL_ASSERT(!list.contains(99), "ArrayList not contains 99");

	// eraseFirstElement
	ARRAYLISTTEST_STL_ASSERT(list.remove(20), "ArrayList eraseFirst 20");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 2, "ArrayList size=2 after eraseFirst");
	ARRAYLISTTEST_STL_ASSERT(!list.contains(20), "ArrayList 20 gone");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[0], 10, "ArrayList[0]=10 after erase");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[1], 30, "ArrayList[1]=30 after erase");

	// eraseLastElement
	list.add(10); // now [10, 30, 10]
	ARRAYLISTTEST_STL_ASSERT(list.removeLast(10), "ArrayList eraseLast 10");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 2, "ArrayList size=2 after eraseLast");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[0], 10, "ArrayList[0]=10 after eraseLast");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[1], 30, "ArrayList[1]=30 after eraseLast");

	// addUnique
	ARRAYLISTTEST_STL_ASSERT(list.addUnique(99), "ArrayList addUnique 99");
	ARRAYLISTTEST_STL_ASSERT(!list.addUnique(99), "ArrayList addUnique 99 dup fails");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 3, "ArrayList size=3 after addUnique");

	// remove (remove all occurrences)
	list.add(30);    // now [10, 30, 99, 30]
	list.removeAll(30);
	ARRAYLISTTEST_STL_ASSERT(!list.contains(30), "ArrayList remove removes all 30");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 2, "ArrayList size=2 after remove(30)");

	// removeAt
	list.add(100);
	list.add(200); // now [10, 99, 100, 200]
	list.removeAt(0);
	ARRAYLISTTEST_STL_ASSERT_EQ(list[0], 99, "ArrayList removeAt(0) shifts");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 3, "ArrayList size=3 after removeAt");

	// addRange from raw array
	const int src[] = { 1, 2, 3 };
	ARRAYLISTTEST_STL_ASSERT(list.addRange(src, 3), "ArrayList addRange ok");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 6, "ArrayList size=6 after addRange");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[3], 1, "ArrayList[3]=1 from addRange");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[5], 3, "ArrayList[5]=3 from addRange");

	// setRange (clears first)
	const int src2[] = { 5, 6, 7 };
	ARRAYLISTTEST_STL_ASSERT(list.setRange(src2, 3), "ArrayList setRange ok");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 3, "ArrayList size=3 after setRange");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[0], 5, "ArrayList[0]=5 after setRange");
	ARRAYLISTTEST_STL_ASSERT_EQ(list[2], 7, "ArrayList[2]=7 after setRange");

	// clear
	list.clear();
	ARRAYLISTTEST_STL_ASSERT(list.isEmpty(), "ArrayList clear -> isEmpty");
	ARRAYLISTTEST_STL_ASSERT_EQ(list.size(), 0, "ArrayList size=0 after clear");

	// capacity limit enforcement
	ArrayList<4, int> capList;
	ARRAYLISTTEST_STL_ASSERT(capList.add(1),  "small add 1");
	ARRAYLISTTEST_STL_ASSERT(capList.add(2),  "small add 2");
	ARRAYLISTTEST_STL_ASSERT(capList.add(3),  "small add 3");
	ARRAYLISTTEST_STL_ASSERT(capList.add(4),  "small add 4");
	ARRAYLISTTEST_STL_ASSERT(!capList.add(5), "small add 5 overflow fails");
	ARRAYLISTTEST_STL_ASSERT_EQ(capList.size(), 4, "small size=4 at capacity");

	// addNotEqual
	ArrayList<8, int> notEqList;
	ARRAYLISTTEST_STL_ASSERT(notEqList.addNotEqual(10, 99), "addNotEqual 10 != 99 ok");
	ARRAYLISTTEST_STL_ASSERT(!notEqList.addNotEqual(99, 99), "addNotEqual 99 == 99 skip");
	ARRAYLISTTEST_STL_ASSERT_EQ(notEqList.size(), 1, "addNotEqual only 1 added");

	// add two elements
	ArrayList<8, int> pairList;
	ARRAYLISTTEST_STL_ASSERT(pairList.add(1, 2), "add(1,2) ok");
	ARRAYLISTTEST_STL_ASSERT_EQ(pairList.size(), 2, "size=2 after add(1,2)");
	ARRAYLISTTEST_STL_ASSERT_EQ(pairList[0], 1, "pair[0]=1");
	ARRAYLISTTEST_STL_ASSERT_EQ(pairList[1], 2, "pair[1]=2");

	// add three elements
	ArrayList<8, int> triList;
	ARRAYLISTTEST_STL_ASSERT(triList.add(10, 20, 30), "add(10,20,30) ok");
	ARRAYLISTTEST_STL_ASSERT_EQ(triList.size(), 3, "size=3 after add(10,20,30)");

	// isAll
	ArrayList<4, int> uniform;
	uniform.add(7);
	uniform.add(7);
	uniform.add(7);
	ARRAYLISTTEST_STL_ASSERT(uniform.isAll(7), "isAll(7) true");
	ARRAYLISTTEST_STL_ASSERT(!uniform.isAll(8), "isAll(8) false");

	// setAll
	ArrayList<4, int> fillList;
	fillList.setAll(42);
	FOR(4)
	{
		ARRAYLISTTEST_STL_ASSERT_EQ(fillList.mValue[i], 42, "fillArray[i]=42");
	}
}

// ============================================================
// Consolidated from legacy STL aggregate test::testArrayListAdvanced
// ============================================================
#ifndef ARRAYLISTTEST_STL_ASSERT
#define ARRAYLISTTEST_STL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("ArrayListTest FAIL: ") + (msg)); } } while(0)
#endif
#ifndef ARRAYLISTTEST_STL_ASSERT_EQ
#define ARRAYLISTTEST_STL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("ArrayListTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#endif
#ifndef ARRAYLISTTEST_STL_ASSERT_STR
#define ARRAYLISTTEST_STL_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("ArrayListTest FAIL: ") + (msg)); } } while(0)
#endif

void ArrayListTest::testSTLAdvanced()
{
	// addRange from another ArrayList
	ArrayList<8, int> src;
	src.add(1);
	src.add(2);
	src.add(3);

	ArrayList<8, int> dst;
	dst.add(10);
	ARRAYLISTTEST_STL_ASSERT(dst.addRange(src), "ArrayList addRange(ArrayList) ok");
	ARRAYLISTTEST_STL_ASSERT_EQ(dst.size(), 4, "ArrayList size=4 after addRange");
	ARRAYLISTTEST_STL_ASSERT_EQ(dst[1], 1, "ArrayList[1]=1 after addRange");
	ARRAYLISTTEST_STL_ASSERT_EQ(dst[3], 3, "ArrayList[3]=3 after addRange");

	// zero (memset to 0)
	ArrayList<4, int> z;
	z.add(1);
	z.add(2);
	z.zero();
	FOR(4)
	{
		ARRAYLISTTEST_STL_ASSERT_EQ(z.mValue[i], 0, "ArrayList zero mValue[i]=0");
	}
	// zero does NOT reset mSize
	ARRAYLISTTEST_STL_ASSERT_EQ(z.size(), 2, "ArrayList zero mSize unchanged");

	// fillArray with start index
	ArrayList<8, int> fillList;
	fillList.add(100);
	fillList.add(200);
	fillList.setAll(2, 999);
	for (int i = 2; i < 8; ++i)
	{
		ARRAYLISTTEST_STL_ASSERT_EQ(fillList.mValue[i], 999, "ArrayList fillArray(2) mValue[i]=999");
	}
	// first two should be unchanged
	ARRAYLISTTEST_STL_ASSERT_EQ(fillList.mValue[0], 100, "ArrayList fillArray keeps [0]");
	ARRAYLISTTEST_STL_ASSERT_EQ(fillList.mValue[1], 200, "ArrayList fillArray keeps [1]");
}
#endif
