#ifdef WINDOWS
#include "FrameHeader.h"
namespace ArrayScopeTestSupport
{
#define ArrayScopeTestSupport_ASD_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("ArrayScopeTest FAILED: ") + (msg)); } } while(0)
}

void ArrayScopeTest::testArrayScopeIntBasic()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(10, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "int scope not null");
	for (int i = 0; i < 10; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == 0, "zeroMemory init to 0");
	}
}

void ArrayScopeTest::testArrayScopeZeroMemory()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(5, true);
	for (int i = 0; i < 5; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == 0, "zero memory int");
	}
}

void ArrayScopeTest::testArrayScopeNoZeroMemory()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(4, false);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "no-zero scope not null");
	scope.mArray[0] = 42;
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 42, "write then read ok");
}

void ArrayScopeTest::testArrayScopeSize1()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(1, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "size 1 not null");
	scope.mArray[0] = 999;
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 999, "size 1 write ok");
}

void ArrayScopeTest::testArrayScopeSize128()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(128, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "size 128 not null");
	for (int i = 0; i < 128; ++i)
	{
	    scope.mArray[i] = i * 2;
	}
	for (int i = 0; i < 128; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == i * 2, "size 128 values ok");
	}
}

void ArrayScopeTest::testArrayScopeLargeAlloc()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(4096, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "large alloc not null");
	scope.mArray[0] = 1;
	scope.mArray[4095] = 2;
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 1, "large alloc first");
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[4095] == 2, "large alloc last");
}

void ArrayScopeTest::testArrayScopeWriteRead()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> scope(8, false);
	for (int i = 0; i < 8; ++i)
	{
	    scope.mArray[i] = i * i;
	}
	for (int i = 0; i < 8; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == i * i, "write-read int*int");
	}
}

void ArrayScopeTest::testArrayScopeFloat()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<float> scope(6, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "float scope not null");
	for (int i = 0; i < 6; ++i)
	{
	    scope.mArray[i] = 0.1f * i;
	}
	ArrayScopeTestSupport_ASD_ASSERT(fabsf(scope.mArray[5] - 0.5f) < 1e-5f, "float scope value ok");
}

void ArrayScopeTest::testArrayScopeDouble()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<double> scope(4, true);
	scope.mArray[0] = 3.14159265358979;
	scope.mArray[3] = -2.71828;
	ArrayScopeTestSupport_ASD_ASSERT(fabs(scope.mArray[0] - 3.14159265358979) < 1e-10, "double scope ok");
	ArrayScopeTestSupport_ASD_ASSERT(fabs(scope.mArray[3] - (-2.71828)) < 1e-10, "double scope neg ok");
}

void ArrayScopeTest::testArrayScopeConsecutive()
{
	using namespace ArrayScopeTestSupport;
	for (int round = 0; round < 20; ++round)
	{
	    ArrayScope<int> scope(16, true);
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "consecutive scope not null");
	    scope.mArray[0] = round;
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == round, "consecutive scope value");
	}
}

void ArrayScopeTest::testArrayScopeShort()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<short> scope(10, true);
	for (int i = 0; i < 10; ++i)
	{
	    scope.mArray[i] = (short)(i * 100);
	}
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[9] == 900, "short scope value ok");
}

void ArrayScopeTest::testArrayScopeByte()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<byte> scope(8, true);
	for (int i = 0; i < 8; ++i)
	{
	    scope.mArray[i] = (byte)(i * 10);
	}
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[7] == 70, "byte scope value ok");
}

void ArrayScopeTest::testArrayScopeMultipleScopes()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> s1(4, true);
	ArrayScope<int> s2(8, true);
	ArrayScope<float> s3(3, true);
	s1.mArray[0] = 10;
	s2.mArray[0] = 20;
	s3.mArray[0] = 3.0f;
	ArrayScopeTestSupport_ASD_ASSERT(s1.mArray[0] == 10, "multiple scopes s1 ok");
	ArrayScopeTestSupport_ASD_ASSERT(s2.mArray[0] == 20, "multiple scopes s2 ok");
	ArrayScopeTestSupport_ASD_ASSERT(fabsf(s3.mArray[0] - 3.0f) < 1e-5f, "multiple scopes s3 ok");
}

void ArrayScopeTest::testArrayScopeNestedScopes()
{
	using namespace ArrayScopeTestSupport;
	ArrayScope<int> outer(10, true);
	outer.mArray[0] = 1;
	{
	    ArrayScope<int> inner(5, true);
	    inner.mArray[0] = 2;
	    ArrayScopeTestSupport_ASD_ASSERT(outer.mArray[0] == 1, "nested outer not clobbered");
	    ArrayScopeTestSupport_ASD_ASSERT(inner.mArray[0] == 2, "nested inner ok");
	}
	ArrayScopeTestSupport_ASD_ASSERT(outer.mArray[0] == 1, "nested outer after inner release ok");
}

void ArrayScopeTest::testCharArrayScopeBasic()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(10);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "char scope not null");
}

void ArrayScopeTest::testCharArrayScopeZeroMemory()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(8, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "char zero scope not null");
	for (int i = 0; i < 8; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == 0, "char zero init ok");
	}
}

void ArrayScopeTest::testCharArrayScopeWriteString()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(16, true);
	const char* msg = "Hello";
	memcpy(scope.mArray, msg, 6);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 'H', "char scope H");
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[4] == 'o', "char scope o");
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[5] == '\0', "char scope null term");
}

void ArrayScopeTest::testCharArrayScopeSize1()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(1, false);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "char size 1 not null");
	scope.mArray[0] = 'Z';
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 'Z', "char size 1 write ok");
}

void ArrayScopeTest::testCharArrayScopeSize512()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(512, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "char size 512 not null");
	scope.mArray[0] = 'A';
	scope.mArray[511] = 'B';
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 'A', "char 512 first ok");
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[511] == 'B', "char 512 last ok");
}

void ArrayScopeTest::testCharArrayScopeNullTerminate()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(64, true);
	const char* src = "TestString123";
	int len = (int)strlen(src);
	memcpy(scope.mArray, src, len + 1);
	ArrayScopeTestSupport_ASD_ASSERT(strcmp(scope.mArray, "TestString123") == 0, "char scope strcmp ok");
}

void ArrayScopeTest::testCharArrayScopeConsecutive()
{
	using namespace ArrayScopeTestSupport;
	for (int i = 0; i < 20; ++i)
	{
	    CharArrayScope scope(32, true);
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "consecutive char scope not null");
	    scope.mArray[0] = (char)(i + 65);
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == (char)(i + 65), "consecutive char scope value");
	}
}

void ArrayScopeTest::testCharArrayScopeLargeAlloc()
{
	using namespace ArrayScopeTestSupport;
	CharArrayScope scope(8192, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "large char alloc not null");
	scope.mArray[0] = 'X';
	scope.mArray[8191] = 'Y';
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == 'X', "large char first ok");
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[8191] == 'Y', "large char last ok");
}

void ArrayScopeTest::testArrayScopeThreadIntBasic()
{
	using namespace ArrayScopeTestSupport;
	ArrayScopeThread<int> scope(10, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "thread scope int not null");
	for (int i = 0; i < 10; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == 0, "thread scope zero init");
	}
}

void ArrayScopeTest::testArrayScopeThreadZeroMemory()
{
	using namespace ArrayScopeTestSupport;
	ArrayScopeThread<int> scope(5, true);
	for (int i = 0; i < 5; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == 0, "thread zero memory");
	}
}

void ArrayScopeTest::testArrayScopeThreadWriteRead()
{
	using namespace ArrayScopeTestSupport;
	ArrayScopeThread<int> scope(8, false);
	for (int i = 0; i < 8; ++i)
	{
	    scope.mArray[i] = i + 100;
	}
	for (int i = 0; i < 8; ++i)
	{
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[i] == i + 100, "thread scope write read");
	}
}

void ArrayScopeTest::testArrayScopeThreadFloat()
{
	using namespace ArrayScopeTestSupport;
	ArrayScopeThread<float> scope(4, true);
	scope.mArray[0] = 1.5f;
	scope.mArray[3] = -2.5f;
	ArrayScopeTestSupport_ASD_ASSERT(fabsf(scope.mArray[0] - 1.5f) < 1e-5f, "thread float scope ok");
	ArrayScopeTestSupport_ASD_ASSERT(fabsf(scope.mArray[3] - (-2.5f)) < 1e-5f, "thread float neg ok");
}

void ArrayScopeTest::testArrayScopeThreadSize256()
{
	using namespace ArrayScopeTestSupport;
	ArrayScopeThread<int> scope(256, true);
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "thread size 256 not null");
	for (int i = 0; i < 256; ++i)
	{
	    scope.mArray[i] = i;
	}
	ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[255] == 255, "thread size 256 last ok");
}

void ArrayScopeTest::testArrayScopeThreadConsecutive()
{
	using namespace ArrayScopeTestSupport;
	for (int round = 0; round < 15; ++round)
	{
	    ArrayScopeThread<int> scope(32, true);
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray != nullptr, "thread consecutive not null");
	    scope.mArray[0] = round * 7;
	    ArrayScopeTestSupport_ASD_ASSERT(scope.mArray[0] == round * 7, "thread consecutive value");
	}
}

namespace ArrayScopeTest2Support
{
#define ArrayScopeTest2Support_ASD2_ASSERT(expr) \
    do { if (!(expr)) { ERROR("ArrayScopeTest2 FAIL: " #expr); } } while(0)
}

void ArrayScopeTest::testArrayDefaultConstruct()
{
	using namespace ArrayScopeTest2Support;
	Array<8, int> arr;
	ArrayScopeTest2Support_ASD2_ASSERT(arr.size() == 8);
}

void ArrayScopeTest::testArrayFillArray()
{
	using namespace ArrayScopeTest2Support;
	Array<5, int> arr;
	arr.setAll(99);
	for (int i = 0; i < 5; ++i)
	{
	    ArrayScopeTest2Support_ASD2_ASSERT(arr[i] == 99);
	}
}

void ArrayScopeTest::testArrayFillArrayPartial()
{
	using namespace ArrayScopeTest2Support;
	Array<10, int> arr;
	arr.setAll(0);
	arr.setAll(3, 7);
	for (int i = 3; i < 10; ++i)
	{
	    ArrayScopeTest2Support_ASD2_ASSERT(arr[i] == 7);
	}
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 0);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[2] == 0);
}

void ArrayScopeTest::testArraySetElement()
{
	using namespace ArrayScopeTest2Support;
	Array<4, int> arr;
	arr.setAll(0);
	arr[2] = 42;
	ArrayScopeTest2Support_ASD2_ASSERT(arr[2] == 42);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 0);
}

void ArrayScopeTest::testArraySize()
{
	using namespace ArrayScopeTest2Support;
	Array<16, float> arr;
	ArrayScopeTest2Support_ASD2_ASSERT(arr.size() == 16);
}

void ArrayScopeTest::testArrayData()
{
	using namespace ArrayScopeTest2Support;
	Array<3, int> arr;
	arr.setAll(5);
	int* ptr = arr.data();
	ArrayScopeTest2Support_ASD2_ASSERT(ptr != nullptr);
	ArrayScopeTest2Support_ASD2_ASSERT(ptr[0] == 5);
	ArrayScopeTest2Support_ASD2_ASSERT(ptr[1] == 5);
	ArrayScopeTest2Support_ASD2_ASSERT(ptr[2] == 5);
}

void ArrayScopeTest::testArrayContains()
{
	using namespace ArrayScopeTest2Support;
	Array<5, int> arr;
	arr.setAll(0);
	arr[2] = 10;
	ArrayScopeTest2Support_ASD2_ASSERT(arr.contains(10) == true);
	ArrayScopeTest2Support_ASD2_ASSERT(arr.contains(99) == false);
}

void ArrayScopeTest::testArrayIterator()
{
	using namespace ArrayScopeTest2Support;
	Array<4, int> arr;
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	int sum = 0;
	for (auto& v : arr)
	{
	    sum += v;
	}
	ArrayScopeTest2Support_ASD2_ASSERT(sum == 10);
}

void ArrayScopeTest::testArrayCopyConstruct()
{
	using namespace ArrayScopeTest2Support;
	Array<3, int> arr;
	arr[0] = 10;
	arr[1] = 20;
	arr[2] = 30;
	Array<3, int> copyArr = arr;
	ArrayScopeTest2Support_ASD2_ASSERT(copyArr[0] == 10);
	ArrayScopeTest2Support_ASD2_ASSERT(copyArr[1] == 20);
	ArrayScopeTest2Support_ASD2_ASSERT(copyArr[2] == 30);
}

void ArrayScopeTest::testArrayAssign()
{
	using namespace ArrayScopeTest2Support;
	Array<3, int> arr;
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	Array<3, int> arr2;
	arr2 = arr;
	ArrayScopeTest2Support_ASD2_ASSERT(arr2[0] == 1);
	ArrayScopeTest2Support_ASD2_ASSERT(arr2[1] == 2);
	ArrayScopeTest2Support_ASD2_ASSERT(arr2[2] == 3);
}

void ArrayScopeTest::testArrayEqualManual()
{
	using namespace ArrayScopeTest2Support;
	Array<3, int> a;
	Array<3, int> b;
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	b[0] = 1;
	b[1] = 2;
	b[2] = 3;
	bool eq = true;
	for (int i = 0; i < 3; ++i)
	{
	    if (a[i] != b[i])
	    {
	        eq = false;
	    }
	}
	ArrayScopeTest2Support_ASD2_ASSERT(eq);
}

void ArrayScopeTest::testArrayNotEqualManual()
{
	using namespace ArrayScopeTest2Support;
	Array<3, int> a;
	Array<3, int> b;
	a[0] = 1;
	a[1] = 2;
	a[2] = 3;
	b[0] = 1;
	b[1] = 2;
	b[2] = 9;
	bool diff = false;
	for (int i = 0; i < 3; ++i)
	{
	    if (a[i] != b[i])
	    {
	        diff = true;
	    }
	}
	ArrayScopeTest2Support_ASD2_ASSERT(diff);
}

void ArrayScopeTest::testArrayOutOfBounds()
{
	using namespace ArrayScopeTest2Support;
	Array<4, int> arr;
	arr.setAll(0);
	arr[3] = 5;
	ArrayScopeTest2Support_ASD2_ASSERT(arr[3] == 5);
}

void ArrayScopeTest::testArrayLargeType()
{
	using namespace ArrayScopeTest2Support;
	Array<64, int> big;
	big.setAll(42);
	ArrayScopeTest2Support_ASD2_ASSERT(big[0] == 42);
	ArrayScopeTest2Support_ASD2_ASSERT(big[63] == 42);
}

void ArrayScopeTest::testArrayIntArray()
{
	using namespace ArrayScopeTest2Support;
	Array<6, int> arr;
	for (int i = 0; i < 6; ++i)
	{
	    arr[i] = i * i;
	}
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 0);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[3] == 9);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[5] == 25);
}

void ArrayScopeTest::testArrayFloatArray()
{
	using namespace ArrayScopeTest2Support;
	Array<4, float> arr;
	arr[0] = 1.1f;
	arr[1] = 2.2f;
	arr[2] = 3.3f;
	arr[3] = 4.4f;
	float sum = 0.0f;
	for (int i = 0; i < 4; ++i)
	{
	    sum += arr[i];
	}
	ArrayScopeTest2Support_ASD2_ASSERT(fabsf(sum - 11.0f) < 1e-3f);
}

void ArrayScopeTest::testArrayStringArray()
{
	using namespace ArrayScopeTest2Support;
	Array<3, string> arr;
	arr[0] = "hello";
	arr[1] = "world";
	arr[2] = "test";
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == "hello");
	ArrayScopeTest2Support_ASD2_ASSERT(arr[1] == "world");
	ArrayScopeTest2Support_ASD2_ASSERT(arr[2] == "test");
}

void ArrayScopeTest::testArrayZero()
{
	using namespace ArrayScopeTest2Support;
	Array<5, int> arr;
	arr.setAll(100);
	arr.zero();
	for (int i = 0; i < 5; ++i)
	{
	    ArrayScopeTest2Support_ASD2_ASSERT(arr[i] == 0);
	}
}

void ArrayScopeTest::testArraySortAscend()
{
	using namespace ArrayScopeTest2Support;
	Array<5, int> arr;
	arr[0] = 5;
	arr[1] = 3;
	arr[2] = 1;
	arr[3] = 4;
	arr[4] = 2;
	MathUtility::quickSort(arr.data(), arr.size());
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 1);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[4] == 5);
	for (int i = 0; i < 4; ++i)
	{
	    ArrayScopeTest2Support_ASD2_ASSERT(arr[i] <= arr[i + 1]);
	}
}

void ArrayScopeTest::testArraySortDescend()
{
	using namespace ArrayScopeTest2Support;
	Array<5, int> arr;
	arr[0] = 1;
	arr[1] = 5;
	arr[2] = 3;
	arr[3] = 2;
	arr[4] = 4;
	MathUtility::quickSort(arr.data(), arr.size(), false);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 5);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[4] == 1);
	for (int i = 0; i < 4; ++i)
	{
	    ArrayScopeTest2Support_ASD2_ASSERT(arr[i] >= arr[i + 1]);
	}
}

void ArrayScopeTest::testArraySortWithData()
{
	using namespace ArrayScopeTest2Support;
	Array<4, int> arr;
	arr[0] = 4;
	arr[1] = 1;
	arr[2] = 3;
	arr[3] = 2;
	MathUtility::quickSort(arr.data(), arr.size());
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 1);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[1] == 2);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[2] == 3);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[3] == 4);
}

void ArrayScopeTest::testArrayBinarySearch()
{
	using namespace ArrayScopeTest2Support;
	Array<5, int> arr;
	arr[0] = 1;
	arr[1] = 3;
	arr[2] = 5;
	arr[3] = 7;
	arr[4] = 9;
	bool f5 = arr.contains(5);
	ArrayScopeTest2Support_ASD2_ASSERT(f5 == true);
	bool f6 = arr.contains(6);
	ArrayScopeTest2Support_ASD2_ASSERT(f6 == false);
}

void ArrayScopeTest::testArrayFindAndModify()
{
	using namespace ArrayScopeTest2Support;
	Array<4, int> arr;
	arr.setAll(0);
	for (int i = 0; i < 4; ++i)
	{
	    if (arr[i] == 0)
	    {
	        arr[i] = i + 1;
	    }
	}
	ArrayScopeTest2Support_ASD2_ASSERT(arr[0] == 1);
	ArrayScopeTest2Support_ASD2_ASSERT(arr[3] == 4);
}


void ArrayScopeTest::test()
{
    testArrayScopeIntBasic();
    testArrayScopeZeroMemory();
    testArrayScopeNoZeroMemory();
    testArrayScopeSize1();
    testArrayScopeSize128();
    testArrayScopeLargeAlloc();
    testArrayScopeWriteRead();
    testArrayScopeFloat();
    testArrayScopeDouble();
    testArrayScopeConsecutive();
    testArrayScopeShort();
    testArrayScopeByte();
    testArrayScopeMultipleScopes();
    testArrayScopeNestedScopes();
    testCharArrayScopeBasic();
    testCharArrayScopeZeroMemory();
    testCharArrayScopeWriteString();
    testCharArrayScopeSize1();
    testCharArrayScopeSize512();
    testCharArrayScopeNullTerminate();
    testCharArrayScopeConsecutive();
    testCharArrayScopeLargeAlloc();
    testArrayScopeThreadIntBasic();
    testArrayScopeThreadZeroMemory();
    testArrayScopeThreadWriteRead();
    testArrayScopeThreadFloat();
    testArrayScopeThreadSize256();
    testArrayScopeThreadConsecutive();
    testArrayDefaultConstruct();
    testArrayFillArray();
    testArrayFillArrayPartial();
    testArraySetElement();
    testArraySize();
    testArrayData();
    testArrayContains();
    testArrayIterator();
    testArrayCopyConstruct();
    testArrayAssign();
    testArrayEqualManual();
    testArrayNotEqualManual();
    testArrayOutOfBounds();
    testArrayLargeType();
    testArrayIntArray();
    testArrayFloatArray();
    testArrayStringArray();
    testArrayZero();
    testArraySortAscend();
    testArraySortDescend();
    testArraySortWithData();
    testArrayBinarySearch();
    testArrayFindAndModify();
}
#endif
