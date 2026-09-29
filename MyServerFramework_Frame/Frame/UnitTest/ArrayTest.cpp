#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// ArrayTest
// 覆盖 Array<N,T> 的全部方法：
//   begin/end/cbegin/cend/data(offset)
//   remove / eraseLastElement / eraseFirstElement / removeAt
//   contains
//   copy (多重载：Array→Array、带destOffset、带copyCount、裸指针)
//   operator[] (越界安全)
//   size
//   zero
//   fillArray (无startIndex / 有startIndex)
// ============================================================

#define AET_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("ArrayTest FAILED: ") + (msg)); } } while(0)
#define AET_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("ArrayTest FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

// ============================================================
// begin / end / cbegin / cend
// ============================================================
void ArrayTest::testArrayIterators()
{
    Array<8, int> arr;
    arr.zero();

    AET_ASSERT(arr.begin() != nullptr, "begin not null");
    AET_ASSERT(arr.end() != nullptr, "end not null");
    AET_ASSERT(arr.cbegin() != nullptr, "cbegin not null");
    AET_ASSERT(arr.cend() != nullptr, "cend not null");
    AET_ASSERT(arr.end() - arr.begin() == 8, "end-begin == size");
    AET_ASSERT(arr.cend() - arr.cbegin() == 8, "cend-cbegin == size");

    // 可写迭代器写入
    int idx = 0;
    for (int* it = arr.begin(); it != arr.end(); ++it)
    {
        *it = idx++;
    }
    AET_ASSERT_EQ(arr[0], 0, "begin iterator write [0]==0");
    AET_ASSERT_EQ(arr[7], 7, "begin iterator write [7]==7");

    // const 迭代器读出
    const Array<8, int>& constRef = arr;
    idx = 0;
    for (const int* it = constRef.cbegin(); it != constRef.cend(); ++it)
    {
        AET_ASSERT_EQ(*it, idx, "cbegin iterator read correct");
        ++idx;
    }

}

// ============================================================
// data() / data(offset)
// ============================================================
void ArrayTest::testArrayData()
{
    Array<10, int> arr;
    arr.zero();
    for (int i = 0; i < 10; ++i)
    {
        arr[i] = i * 10;
    }

    // data()
    const int* ptr = arr.data();
    AET_ASSERT(ptr != nullptr, "data() not null");
    AET_ASSERT_EQ(ptr[0], 0, "data()[0]==0");
    AET_ASSERT_EQ(ptr[9], 90, "data()[9]==90");

    // data(offset) 合法
    const int* ptr3 = arr.data(3);
    AET_ASSERT(ptr3 != nullptr, "data(3) not null");
    AET_ASSERT_EQ(*ptr3, 30, "data(3) value==30");

    int* mutablePtr = arr.data(5);
    AET_ASSERT(mutablePtr != nullptr, "data(5) mutable not null");
    *mutablePtr = 999;
    AET_ASSERT_EQ(arr[5], 999, "data(5) write-through works");

}

// ============================================================
// size
// ============================================================
void ArrayTest::testArraySize()
{
    Array<1, int> a1;
    AET_ASSERT_EQ(a1.size(), 1, "size==1");

    Array<16, int> a16;
    AET_ASSERT_EQ(a16.size(), 16, "size==16");

    Array<256, char> a256;
    AET_ASSERT_EQ(a256.size(), 256, "size==256");

    Array<1024, byte> a1024;
    AET_ASSERT_EQ(a1024.size(), 1024, "size==1024");

}

// ============================================================
// zero
// ============================================================
void ArrayTest::testArrayZero()
{
    Array<16, int> arr;
    // 先填充非零数据
    arr.setAll(0xABCDEF01);
    for (int i = 0; i < 16; ++i)
    {
        AET_ASSERT(arr[i] != 0, "pre-zero filled");
    }

    arr.zero();
    for (int i = 0; i < 16; ++i)
    {
        AET_ASSERT_EQ(arr[i], 0, "after zero all==0");
    }

    // byte array
    Array<32, byte> barr;
    barr.setAll((byte)0xFF);
    barr.zero();
    for (int i = 0; i < 32; ++i)
    {
        AET_ASSERT_EQ((int)barr[i], 0, "byte array zero all==0");
    }

    // 多次 zero 不崩溃
    arr.zero();
    arr.zero();
    AET_ASSERT(true, "double zero no crash");

}

// ============================================================
// fillArray (no startIndex)
// ============================================================
void ArrayTest::testArrayFillAll()
{
    // int 类型
    Array<8, int> arr;
    arr.setAll(42);
    for (int i = 0; i < 8; ++i)
    {
        AET_ASSERT_EQ(arr[i], 42, "fillArray all 42");
    }

    // 不同值覆盖
    arr.setAll(-1);
    for (int i = 0; i < 8; ++i)
    {
        AET_ASSERT_EQ(arr[i], -1, "fillArray all -1");
    }

    // float 类型
    Array<4, float> farr;
    farr.setAll(3.14f);
    for (int i = 0; i < 4; ++i)
    {
        AET_ASSERT(fabsf(farr[i] - 3.14f) < 1e-4f, "fillArray float 3.14");
    }

    // bool 类型
    Array<4, bool> barr;
    barr.setAll(true);
    for (int i = 0; i < 4; ++i)
    {
        AET_ASSERT(barr[i], "fillArray bool true");
    }
    barr.setAll(false);
    for (int i = 0; i < 4; ++i)
    {
        AET_ASSERT(!barr[i], "fillArray bool false");
    }

}

// ============================================================
// fillArray (with startIndex)
// ============================================================
void ArrayTest::testArrayFillFromStart()
{
    Array<8, int> arr;
    arr.zero();

    // start=4，填充后半段
    arr.setAll(4, 99);
    for (int i = 0; i < 4; ++i)
    {
        AET_ASSERT_EQ(arr[i], 0, "fillArray(4,99) first half unchanged");
    }
    for (int i = 4; i < 8; ++i)
    {
        AET_ASSERT_EQ(arr[i], 99, "fillArray(4,99) second half==99");
    }

    // start=0，等价于 fillArray(val)
    arr.setAll(0, 7);
    for (int i = 0; i < 8; ++i)
    {
        AET_ASSERT_EQ(arr[i], 7, "fillArray(0,7) all==7");
    }

    // start == size（不写任何元素，不崩溃）
    arr.setAll(8, 123);
    for (int i = 0; i < 8; ++i)
    {
        AET_ASSERT_EQ(arr[i], 7, "fillArray(8,...) unchanged");
    }

    // start=7，只填最后一个
    arr.zero();
    arr.setAll(7, 55);
    AET_ASSERT_EQ(arr[7], 55, "fillArray(7,55) last==55");
    AET_ASSERT_EQ(arr[6], 0, "fillArray(7,55) [6]==0");

}

// ============================================================
// operator[] 读写 & 越界安全
// ============================================================
void ArrayTest::testArrayOperatorBracket()
{
    Array<5, int> arr;
    arr.zero();

    // 正常读写
    arr[0] = 100;
    arr[4] = 400;
    AET_ASSERT_EQ(arr[0], 100, "arr[0]==100");
    AET_ASSERT_EQ(arr[4], 400, "arr[4]==400");

    // 链式写入
    for (int i = 0; i < 5; ++i)
    {
        arr[i] = i * 11;
    }
    for (int i = 0; i < 5; ++i)
    {
        AET_ASSERT_EQ(arr[i], i * 11, "arr[i]==i*11");
    }

    // 越界框架会触发 ERROR+pause，不在此测试（框架设计即报错）

}

// ============================================================
// contains
// ============================================================
void ArrayTest::testArrayContains()
{
    Array<8, int> arr;
    arr.zero();
    for (int i = 0; i < 8; ++i)
    {
        arr[i] = (i + 1) * 10;
    }
    // [10, 20, 30, 40, 50, 60, 70, 80]

    // 存在的元素
    AET_ASSERT(arr.contains(10), "contains 10 true");
    AET_ASSERT(arr.contains(80), "contains 80 true");
    AET_ASSERT(arr.contains(50), "contains 50 true");

    // 不存在的元素
    AET_ASSERT(!arr.contains(0), "contains 0 false");
    AET_ASSERT(!arr.contains(90), "contains 90 false");
    AET_ASSERT(!arr.contains(-10), "contains -10 false");

    // 带 length 参数（只查前N个）
    AET_ASSERT(arr.contains(30, 3), "contains 30 in length=3");
    AET_ASSERT(!arr.contains(40, 3), "contains 40 NOT in length=3");
    AET_ASSERT(arr.contains(40, 4), "contains 40 in length=4");

    // length=0
    AET_ASSERT(!arr.contains(10, 0), "contains 10 in length=0 false");

    // length=-1（使用全长）
    AET_ASSERT(arr.contains(80, -1), "contains 80 length=-1 true");

    // bool 类型
    Array<4, bool> barr;
    barr[0] = true;
    barr[1] = false;
    barr[2] = true;
    barr[3] = false;
    AET_ASSERT(barr.contains(true), "bool contains true");
    AET_ASSERT(barr.contains(false), "bool contains false");

}

// ============================================================
// removeAt
// ============================================================
void ArrayTest::testArrayEraseAt()
{
    // removeAt(curCount, index): 将 index 后面的元素向前移动
    Array<8, int> arr;
    arr.zero();
    // 初始化 [1,2,3,4,5,6,7,8]，curCount=8
    for (int i = 0; i < 8; ++i)
    {
        arr[i] = i + 1;
    }

    // 删除 index=2（元素 3），curCount=8 → 变成 [1,2,4,5,6,7,8,8]
    arr.removeAt(8, 2);
    AET_ASSERT_EQ(arr[0], 1, "removeAt(2): [0]==1");
    AET_ASSERT_EQ(arr[1], 2, "removeAt(2): [1]==2");
    AET_ASSERT_EQ(arr[2], 4, "removeAt(2): [2]==4");
    AET_ASSERT_EQ(arr[3], 5, "removeAt(2): [3]==5");
    AET_ASSERT_EQ(arr[6], 8, "removeAt(2): [6]==8");

    // 删除最后一个（index=6=curCount-1）→ 不移动
    arr.removeAt(7, 6);
    AET_ASSERT_EQ(arr[5], 7, "removeAt last: [5]==7");

    // 删除 index=0
    Array<4, int> arr2;
    arr2[0] = 10;
    arr2[1] = 20;
    arr2[2] = 30;
    arr2[3] = 40;
    arr2.removeAt(4, 0);
    AET_ASSERT_EQ(arr2[0], 20, "removeAt(0): [0]==20");
    AET_ASSERT_EQ(arr2[1], 30, "removeAt(0): [1]==30");
    AET_ASSERT_EQ(arr2[2], 40, "removeAt(0): [2]==40");

}

// ============================================================
// remove (erase ALL occurrences)
// ============================================================
void ArrayTest::testArrayEraseElement()
{
    Array<8, int> arr;
    arr.zero();
    // [1, 2, 3, 2, 4, 2, 5, 6]
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 2;
    arr[4] = 4;
    arr[5] = 2;
    arr[6] = 5;
    arr[7] = 6;

    // 删除所有 2，curCount=8 → 返回新 count
    const int newCount = arr.removeAll(2, 8);
    AET_ASSERT_EQ(newCount, 5, "remove(2) newCount==5");
    // 剩余 [1,3,4,5,6,...]
    AET_ASSERT(!arr.contains(2, newCount), "remove(2) no 2 left");
    AET_ASSERT(arr.contains(1, newCount), "remove(2) 1 still present");
    AET_ASSERT(arr.contains(5, newCount), "remove(2) 5 still present");

    // 删除不存在的元素
    const int newCount2 = arr.removeAll(99, newCount);
    AET_ASSERT_EQ(newCount2, newCount, "remove nonexist count unchanged");

    // 删除唯一元素
    Array<4, int> arr2;
    arr2[0] = 7;
    arr2[1] = 7;
    arr2[2] = 7;
    arr2[3] = 7;
    const int newCount3 = arr2.removeAll(7, 4);
    AET_ASSERT_EQ(newCount3, 0, "remove all same newCount==0");

    // 空列表
    Array<4, int> arr3;
    arr3.zero();
    const int newCount4 = arr3.removeAll(0, 0);
    AET_ASSERT_EQ(newCount4, 0, "remove empty count==0");

}

// ============================================================
// eraseFirstElement
// ============================================================
void ArrayTest::testArrayEraseFirstElement()
{
    // [1, 2, 3, 2, 4]
    Array<8, int> arr;
    arr.zero();
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 2;
    arr[4] = 4;

    // 删除第一个 2
    bool r = arr.remove(2, 5);
    AET_ASSERT(r, "eraseFirstElement found true");
    // 结果：[1, 3, 2, 4, ...]
    AET_ASSERT_EQ(arr[0], 1, "eraseFirst[0]==1");
    AET_ASSERT_EQ(arr[1], 3, "eraseFirst[1]==3");
    AET_ASSERT_EQ(arr[2], 2, "eraseFirst[2]==2 (second 2 remains)");
    AET_ASSERT_EQ(arr[3], 4, "eraseFirst[3]==4");

    // 再删第一个 2
    bool r2 = arr.remove(2, 4);
    AET_ASSERT(r2, "eraseFirstElement second 2 found");
    AET_ASSERT(!arr.contains(2, 3), "eraseFirst no 2 left");

    // 删除不存在的元素
    bool r3 = arr.remove(99, 3);
    AET_ASSERT(!r3, "eraseFirstElement nonexist false");

    // 删除第一个元素
    Array<4, int> arr2;
    arr2[0] = 10;
    arr2[1] = 20;
    arr2[2] = 30;
    arr2[3] = 40;
    bool r4 = arr2.remove(10, 4);
    AET_ASSERT(r4, "eraseFirstElement first elem found");
    AET_ASSERT_EQ(arr2[0], 20, "after erase first: [0]==20");
    AET_ASSERT_EQ(arr2[1], 30, "after erase first: [1]==30");

    // 删除最后一个元素
    Array<4, int> arr3;
    arr3[0] = 10;
    arr3[1] = 20;
    arr3[2] = 30;
    arr3[3] = 40;
    bool r5 = arr3.remove(40, 4);
    AET_ASSERT(r5, "eraseFirstElement last elem found");
    AET_ASSERT_EQ(arr3[0], 10, "after erase last: [0]==10");
    AET_ASSERT_EQ(arr3[2], 30, "after erase last: [2]==30");

}

// ============================================================
// eraseLastElement
// ============================================================
void ArrayTest::testArrayEraseLastElement()
{
    // [1, 2, 3, 2, 4]
    Array<8, int> arr;
    arr.zero();
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    arr[3] = 2;
    arr[4] = 4;

    // 删除最后一个 2（index=3）
    bool r = arr.removeLast(2, 5);
    AET_ASSERT(r, "eraseLastElement found true");
    // 结果：[1, 2, 3, 4, ...]
    AET_ASSERT_EQ(arr[0], 1, "eraseLast[0]==1");
    AET_ASSERT_EQ(arr[1], 2, "eraseLast[1]==2 (first 2 remains)");
    AET_ASSERT_EQ(arr[2], 3, "eraseLast[2]==3");
    AET_ASSERT_EQ(arr[3], 4, "eraseLast[3]==4");

    // 再删最后一个 2（现在 index=1）
    bool r2 = arr.removeLast(2, 4);
    AET_ASSERT(r2, "eraseLastElement second call found");
    AET_ASSERT(!arr.contains(2, 3), "eraseLast no 2 left");

    // 删除不存在的元素
    bool r3 = arr.removeLast(99, 3);
    AET_ASSERT(!r3, "eraseLastElement nonexist false");

}

// ============================================================
// copy (Array→Array 全量)
// ============================================================
void ArrayTest::testArrayCopyFull()
{
    Array<8, int> src;
    for (int i = 0; i < 8; ++i)
    {
        src[i] = (i + 1) * 100;
    }

    Array<8, int> dst;
    dst.zero();
    dst.copy(src);

    for (int i = 0; i < 8; ++i)
    {
        AET_ASSERT_EQ(dst[i], (i + 1) * 100, "copy full dst[i]");
    }

    // 确认是深拷贝（修改 src 不影响 dst）
    src[0] = 9999;
    AET_ASSERT_EQ(dst[0], 100, "copy deep: modify src, dst unchanged");

}

// ============================================================
// copy (Array→Array 带 destOffset)
// ============================================================
void ArrayTest::testArrayCopyWithOffset()
{
    Array<8, int> src;
    src[0] = 11;
    src[1] = 22;
    src[2] = 33;
    src[3] = 44;

    Array<8, int> dst;
    dst.zero();

    // 拷贝 src 到 dst[4..]
    // copy(destOffset, src)：这要求 SourceLength(4) <= Length(8) - destOffset(4) = 4
    // 这里用 Array<4,int>
    Array<4, int> src4;
    src4[0] = 55;
    src4[1] = 66;
    src4[2] = 77;
    src4[3] = 88;

    dst.copy(4, src4);
    AET_ASSERT_EQ(dst[0], 0, "copy(offset=4): [0]==0");
    AET_ASSERT_EQ(dst[4], 55, "copy(offset=4): [4]==55");
    AET_ASSERT_EQ(dst[5], 66, "copy(offset=4): [5]==66");
    AET_ASSERT_EQ(dst[6], 77, "copy(offset=4): [6]==77");
    AET_ASSERT_EQ(dst[7], 88, "copy(offset=4): [7]==88");

}

// ============================================================
// copy (Array→Array 带 copyCount)
// ============================================================
void ArrayTest::testArrayCopyWithCount()
{
    Array<8, int> src;
    for (int i = 0; i < 8; ++i)
    {
        src[i] = (i + 1) * 10;
    }

    Array<8, int> dst;
    dst.zero();

    // 只拷贝前3个
    dst.copy(src, 3);
    AET_ASSERT_EQ(dst[0], 10, "copy(count=3): [0]==10");
    AET_ASSERT_EQ(dst[1], 20, "copy(count=3): [1]==20");
    AET_ASSERT_EQ(dst[2], 30, "copy(count=3): [2]==30");
    AET_ASSERT_EQ(dst[3], 0, "copy(count=3): [3]==0 (untouched)");

    // 拷贝全部
    dst.zero();
    dst.copy(src, 8);
    for (int i = 0; i < 8; ++i)
    {
        AET_ASSERT_EQ(dst[i], (i + 1) * 10, "copy(count=8): all match");
    }

}

// ============================================================
// copy (裸指针 T*)
// ============================================================
void ArrayTest::testArrayCopyFromPointer()
{
    const int srcData[] = { 1, 2, 3, 4, 5 };
    const int srcLen = 5;

    Array<8, int> dst;
    dst.zero();

    // 从裸指针拷贝5个
    dst.copy(srcData, srcLen);
    for (int i = 0; i < srcLen; ++i)
    {
        AET_ASSERT_EQ(dst[i], i + 1, "copy(ptr): dst[i]==i+1");
    }
    AET_ASSERT_EQ(dst[5], 0, "copy(ptr): dst[5] untouched");

    // 带 destOffset 的裸指针版本
    const int srcData2[] = { 10, 20, 30 };
    dst.copy(5, srcData2, 3);
    AET_ASSERT_EQ(dst[5], 10, "copy(ptr,offset): [5]==10");
    AET_ASSERT_EQ(dst[6], 20, "copy(ptr,offset): [6]==20");
    AET_ASSERT_EQ(dst[7], 30, "copy(ptr,offset): [7]==30");

}

// ============================================================
// 混合操作：fill + erase + copy 组合
// ============================================================
void ArrayTest::testArrayMixedOperations()
{
    // 创建一个 16 元素数组，填充 [1..16]
    Array<16, int> arr;
    for (int i = 0; i < 16; ++i)
    {
        arr[i] = i + 1;
    }

    // 确认初始状态
    AET_ASSERT(arr.contains(1), "mixed: contains 1");
    AET_ASSERT(arr.contains(16), "mixed: contains 16");

    // 删除所有偶数
    int curCount = 16;
    for (int even = 2; even <= 16; even += 2)
    {
        curCount = arr.removeAll(even, curCount);
    }
    AET_ASSERT_EQ(curCount, 8, "mixed: after erase evens count==8");
    for (int odd = 1; odd <= 15; odd += 2)
    {
        AET_ASSERT(arr.contains(odd, curCount), "mixed: odd present");
    }
    for (int even = 2; even <= 16; even += 2)
    {
        AET_ASSERT(!arr.contains(even, curCount), "mixed: even absent");
    }

    // 用 zero 清空
    arr.zero();
    for (int i = 0; i < 16; ++i)
    {
        AET_ASSERT_EQ(arr[i], 0, "mixed: after zero all==0");
    }

    // 重新 fill 并 copy
    arr.setAll(7);
    Array<16, int> dst;
    dst.zero();
    dst.copy(arr);
    for (int i = 0; i < 16; ++i)
    {
        AET_ASSERT_EQ(dst[i], 7, "mixed: copy after fill all==7");
    }

}

// ============================================================
// 大量数据压力测试
// ============================================================
void ArrayTest::testArrayLargeScale()
{
    // 256元素 int 数组
    Array<256, int> arr;
    arr.zero();

    // 填充 0~255
    for (int i = 0; i < 256; ++i)
    {
        arr[i] = i;
    }

    // 验证全部
    for (int i = 0; i < 256; ++i)
    {
        AET_ASSERT_EQ(arr[i], i, "largeScale arr[i]==i");
    }

    // contains 全部存在
    for (int i = 0; i < 256; ++i)
    {
        AET_ASSERT(arr.contains(i), "largeScale contains i");
    }

    // fillArray 覆盖全部
    arr.setAll(100);
    for (int i = 0; i < 256; ++i)
    {
        AET_ASSERT_EQ(arr[i], 100, "largeScale fillArray 100");
    }

    // 深拷贝测试
    Array<256, int> dst;
    dst.zero();
    dst.copy(arr);
    for (int i = 0; i < 256; ++i)
    {
        AET_ASSERT_EQ(dst[i], 100, "largeScale copy all==100");
    }

}

// ============================================================
// char/byte 特化测试（模拟 MyString 场景）
// ============================================================
void ArrayTest::testArrayCharBytes()
{
    Array<64, char> str;
    str.zero();

    const char* hello = "HelloWorld";
    str.copy((const char*)hello, (int)strlen(hello));
    AET_ASSERT(str.contains('H'), "char array contains H");
    AET_ASSERT(str.contains('d'), "char array contains d");
    AET_ASSERT(!str.contains('z'), "char array not contains z");
    AET_ASSERT_EQ(str[0], 'H', "char [0]==H");
    AET_ASSERT_EQ(str[9], 'd', "char [9]==d");

    // zero 后全为 '\0'
    str.zero();
    for (int i = 0; i < 64; ++i)
    {
        AET_ASSERT_EQ(str[i], '\0', "char after zero == \\0");
    }

    // byte 数组
    Array<16, byte> barr;
    barr.setAll((byte)0xAA);
    for (int i = 0; i < 16; ++i)
    {
        AET_ASSERT_EQ((int)barr[i], 0xAA, "byte fillArray 0xAA");
    }
    barr.zero();
    for (int i = 0; i < 16; ++i)
    {
        AET_ASSERT_EQ((int)barr[i], 0, "byte after zero == 0");
    }

}

// ============================================================
// remove + eraseFirstElement 对比
// ============================================================
void ArrayTest::testArrayEraseComparison()
{
    // remove 删除所有，eraseFirstElement 只删一个
    // 准备 [5, 3, 5, 3, 5]
    Array<8, int> arrAll;
    arrAll[0] = 5;
    arrAll[1] = 3;
    arrAll[2] = 5;
    arrAll[3] = 3;
    arrAll[4] = 5;
    arrAll.zero();
    arrAll[0] = 5;
    arrAll[1] = 3;
    arrAll[2] = 5;
    arrAll[3] = 3;
    arrAll[4] = 5;

    Array<8, int> arrFirst;
    for (int i = 0; i < 8; ++i)
    {
        arrFirst[i] = arrAll[i];
    }

    // remove 删除所有 5
    const int newCountAll = arrAll.removeAll(5, 5);
    AET_ASSERT_EQ(newCountAll, 2, "remove all 5: count==2");
    AET_ASSERT(!arrAll.contains(5, newCountAll), "remove no 5 left");

    // eraseFirstElement 只删第一个 5
    bool rFirst = arrFirst.remove(5, 5);
    AET_ASSERT(rFirst, "eraseFirstElement 5 found");
    // 删除 index=0 后：[3, 5, 3, 5, 5]（移位）
    AET_ASSERT_EQ(arrFirst[0], 3, "eraseFirst: [0]==3");
    AET_ASSERT(arrFirst.contains(5, 4), "eraseFirst: 5 still present");

}

// ============================================================
// 边界：size=1 的数组
// ============================================================
void ArrayTest::testArraySizeOne()
{
    Array<1, int> arr;
    arr[0] = 42;
    AET_ASSERT_EQ(arr.size(), 1, "size-1 array: size==1");
    AET_ASSERT_EQ(arr[0], 42, "size-1 array: [0]==42");
    AET_ASSERT(arr.contains(42), "size-1 contains 42");
    AET_ASSERT(!arr.contains(0), "size-1 not contains 0");

    arr.zero();
    AET_ASSERT_EQ(arr[0], 0, "size-1 after zero==0");

    arr.setAll(99);
    AET_ASSERT_EQ(arr[0], 99, "size-1 fillArray 99");

}

// ============================================================
// 主入口
// ============================================================
void ArrayTest::test()
{
    testArrayIterators();
    testArrayData();
    testArraySize();
    testArrayZero();
    testArrayFillAll();
    testArrayFillFromStart();
    testArrayOperatorBracket();
    testArrayContains();
    testArrayEraseAt();
    testArrayEraseElement();
    testArrayEraseFirstElement();
    testArrayEraseLastElement();
    testArrayCopyFull();
    testArrayCopyWithOffset();
    testArrayCopyWithCount();
    testArrayCopyFromPointer();
    testArrayMixedOperations();
    testArrayLargeScale();
    testArrayCharBytes();
    testArrayEraseComparison();
    testArraySizeOne();
	testArray_Basic();
	testArray_FillAndZero();
	testContainerExtended();
}

// ============================================================
// Consolidated container smoke tests
// ============================================================
#define ARRAYTEST_CT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("ArrayTest FAIL: ") + (msg)); } } while(0)
#define ARRAYTEST_CT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("ArrayTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define ARRAYTEST_CT_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("ArrayTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void ArrayTest::testArray_Basic()
{
	Array<8, int> arr;
	arr.zero();

	// contains (search in full length)
	ARRAYTEST_CT_ASSERT(!arr.contains(1), "Array contains 1 in zeroed = false");

	// set some values and check contains
	arr.mValue[0] = 10;
	arr.mValue[3] = 20;
	ARRAYTEST_CT_ASSERT(arr.contains(10), "Array contains 10");
	ARRAYTEST_CT_ASSERT(arr.contains(20), "Array contains 20");
	ARRAYTEST_CT_ASSERT(!arr.contains(99), "Array not contains 99");

	// eraseFirstElement
	arr.mValue[1] = 10;  // now 10 appears at index 0 and 1
	int count = 2;
	const bool ok = arr.remove(10, count);
	ARRAYTEST_CT_ASSERT(ok, "remove 10 ok");
	// after erase, count stays but arr[0] shifts
	// (removeAt shifts left, so arr[0] should be the old arr[1]=10... wait, arr[0]=10 was removed)
	// arr before: [10,10,0,20,0,0,0,0], removeAt(0) -> [10,0,20,0,0,0,0,0]
	ARRAYTEST_CT_ASSERT_EQ(arr.mValue[0], 10, "remove arr[0] after erase");
}

void ArrayTest::testArray_FillAndZero()
{
	Array<4, int> arr;
	arr.zero();
	FOR(4)
	{
		ARRAYTEST_CT_ASSERT_EQ(arr.mValue[i], 0, "Array zero fills 0");
	}
}

void ArrayTest::testContainerExtended()
{
// default (zero) initialization
	{
		Array<10, int> arr;
		memset(arr.data(), 0, sizeof(int) * 10);
		FOR(10)
		{
			ARRAYTEST_CT_ASSERT_EQ(arr[i], 0, "Array zero init");
		}
	}

	// fill
	{
		Array<5, float> arr;
		FOR(5)
		{
			arr[i] = 3.14f;
		}
		FOR(5)
		{
			ARRAYTEST_CT_ASSERT(fabsf(arr[i] - 3.14f) < 1e-5f, "Array fill 3.14");
		}
	}

	// size
	{
		Array<20, byte> arr;
		ARRAYTEST_CT_ASSERT_EQ((int)arr.size(), 20, "Array size=20");
	}

	// direct assignment
	{
		Array<3, string> arr;
		arr[0] = "hello";
		arr[1] = "world";
		arr[2] = "!";
		ARRAYTEST_CT_ASSERT_STR(arr[0], "hello", "Array string[0]");
		ARRAYTEST_CT_ASSERT_STR(arr[2], "!", "Array string[2]");
	}


//------------------------------------------------------------------------------
}
#endif
