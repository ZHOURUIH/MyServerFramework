#ifdef WINDOWS
#include "FrameHeader.h"
#include <string>

// ─── Assertion macros ───
#define APS_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("ArrayPoolSingleTest FAIL: ") + (msg)); }
#define APS_ASSERT_EQ(a, b, msg) \
	if ((a) != (b)) { ERROR(string("ArrayPoolSingleTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); }

// =====================================================================
// ArrayPoolSingle — int 类型
// =====================================================================

void ArrayPoolSingleTest::testApsIntGetRelease()
{
	ArrayPoolSingle pool;
	int* arr = pool.get<int>(10, false);
	APS_ASSERT(arr != nullptr, "int get non-null");

	// 写入数据
	for (int i = 0; i < 10; ++i)
	{
		arr[i] = i * 100;
	}
	// 验证数据
	for (int i = 0; i < 10; ++i)
	{
		APS_ASSERT_EQ(arr[i], i * 100, "int get data integrity");
	}
	pool.release(arr, 10);
	APS_ASSERT(arr == nullptr, "int release nulls pointer");
}

void ArrayPoolSingleTest::testApsIntReuse()
{
	ArrayPoolSingle pool;
	int* arr1 = pool.get<int>(10, false);
	// 写入数据
	for (int i = 0; i < 10; ++i) arr1[i] = 42;
	pool.release(arr1, 10);

	// 再次获取相同大小，应复用同一块内存
	int* arr2 = pool.get<int>(10, false);
	APS_ASSERT(arr2 != nullptr, "int reuse non-null");
	// 数据可能已被覆盖，但不应崩溃
	for (int i = 0; i < 10; ++i)
	{
		arr2[i] = i;
	}
	pool.release(arr2, 10);
}

void ArrayPoolSingleTest::testApsIntMultipleSizes()
{
	ArrayPoolSingle pool;
	// 分配不同大小的数组
	int* tempSmall = pool.get<int>(5, false);
	int* medium = pool.get<int>(50, false);
	int* large = pool.get<int>(500, false);
	APS_ASSERT(tempSmall != nullptr, "int small alloc");
	APS_ASSERT(medium != nullptr, "int medium alloc");
	APS_ASSERT(large != nullptr, "int large alloc");

	pool.release(tempSmall, 5);
	pool.release(medium, 50);
	pool.release(large, 500);
}

void ArrayPoolSingleTest::testApsIntZeroMemory()
{
	ArrayPoolSingle pool;
	int* arr = pool.get<int>(20, true);
	APS_ASSERT(arr != nullptr, "int zeroMemory non-null");
	// 验证全部为零
	for (int i = 0; i < 20; ++i)
	{
		APS_ASSERT(arr[i] == 0, "int zeroMemory element zero");
	}
	pool.release(arr, 20);
}

void ArrayPoolSingleTest::testApsIntZeroMemoryReuse()
{
	ArrayPoolSingle pool;
	int* arr1 = pool.get<int>(10, true);
	for (int i = 0; i < 10; ++i) arr1[i] = 99;
	pool.release(arr1, 10);

	int* arr2 = pool.get<int>(10, true);
	APS_ASSERT(arr2 != nullptr, "int zeroMemory reuse non-null");
	// zeroMemory=true 应重新清零
	for (int i = 0; i < 10; ++i)
	{
		APS_ASSERT(arr2[i] == 0, "int zeroMemory reuse cleared");
	}
	pool.release(arr2, 10);
}

void ArrayPoolSingleTest::testApsIntReleaseNull()
{
	ArrayPoolSingle pool;
	int* ptr = nullptr;
	// release nullptr 不应崩溃
	pool.release(ptr, 0);
	pool.release(ptr, 10);
	APS_ASSERT(true, "int release nullptr no crash");
}

void ArrayPoolSingleTest::testApsIntReleaseZeroCount()
{
	ArrayPoolSingle pool;
	int* arr = pool.get<int>(10, false);
	// 以 elementCount=0 释放
	pool.release(arr, 0);
	// 此时指针已被赋值为nullptr
	APS_ASSERT(true, "int release zero count no crash");
}

void ArrayPoolSingleTest::testApsIntMultiplePools()
{
	ArrayPoolSingle pool1;
	ArrayPoolSingle pool2;
	int* arr1 = pool1.get<int>(10, false);
	int* arr2 = pool2.get<int>(10, false);
	APS_ASSERT(arr1 != nullptr && arr2 != nullptr, "int multiple pools alloc");
	arr1[0] = 1;
	arr2[0] = 2;
	APS_ASSERT(arr1[0] != arr2[0], "int multiple pools independent");
	pool1.release(arr1, 10);
	pool2.release(arr2, 10);
}

void ArrayPoolSingleTest::testApsIntGetLargerThanPrevious()
{
	ArrayPoolSingle pool;
	int* tempSmall = pool.get<int>(5, false);
	pool.release(tempSmall, 5);

	// 请求更大的数组
	int* large = pool.get<int>(100, false);
	APS_ASSERT(large != nullptr, "int larger alloc non-null");
	for (int i = 0; i < 100; ++i) large[i] = i;
	pool.release(large, 100);
}

// =====================================================================
// ArrayPoolSingle — char 类型
// =====================================================================

void ArrayPoolSingleTest::testApsCharGetRelease()
{
	ArrayPoolSingle pool;
	char* arr = pool.get<char>(16, false);
	APS_ASSERT(arr != nullptr, "char get non-null");
	// char 类型首元素应自动设为 0
	APS_ASSERT(arr[0] == 0, "char first element auto-zero");
	arr[0] = 'A';
	arr[1] = 'B';
	arr[15] = 'Z';
	APS_ASSERT(arr[0] == 'A', "char data write[0]");
	APS_ASSERT(arr[1] == 'B', "char data write[1]");
	APS_ASSERT(arr[15] == 'Z', "char data write[15]");
	pool.release(arr, 16);
}

void ArrayPoolSingleTest::testApsCharMultipleSizes()
{
	ArrayPoolSingle pool;
	char* tempSmall = pool.get<char>(4, false);
	char* medium = pool.get<char>(64, false);
	char* large = pool.get<char>(1024, false);
	APS_ASSERT(tempSmall != nullptr && medium != nullptr && large != nullptr, "char multiple sizes");
	pool.release(tempSmall, 4);
	pool.release(medium, 64);
	pool.release(large, 1024);
}

void ArrayPoolSingleTest::testApsCharReleaseNull()
{
	ArrayPoolSingle pool;
	char* ptr = nullptr;
	pool.release(ptr, 0);
	pool.release(ptr, 100);
	APS_ASSERT(true, "char release nullptr no crash");
}

// =====================================================================
// ArrayPoolSingle — double 类型
// =====================================================================

void ArrayPoolSingleTest::testApsDoubleGetRelease()
{
	ArrayPoolSingle pool;
	double* arr = pool.get<double>(8, false);
	APS_ASSERT(arr != nullptr, "double get non-null");
	arr[0] = 3.14159;
	arr[7] = -2.71828;
	APS_ASSERT(arr[0] == 3.14159, "double data[0]");
	APS_ASSERT(arr[7] == -2.71828, "double data[7]");
	pool.release(arr, 8);
}

void ArrayPoolSingleTest::testApsDoubleZeroMemory()
{
	ArrayPoolSingle pool;
	double* arr = pool.get<double>(10, true);
	for (int i = 0; i < 10; ++i)
	{
		APS_ASSERT(arr[i] == 0.0, "double zeroMemory");
	}
	pool.release(arr, 10);
}

// =====================================================================
// ArrayPoolSingle — 多个类型混合
// =====================================================================

void ArrayPoolSingleTest::testApsMixedTypes()
{
	ArrayPoolSingle pool;
	int* intArr = pool.get<int>(10, false);
	char* charArr = pool.get<char>(20, false);
	double* doubleArr = pool.get<double>(5, false);

	APS_ASSERT(intArr != nullptr, "mixed int");
	APS_ASSERT(charArr != nullptr, "mixed char");
	APS_ASSERT(doubleArr != nullptr, "mixed double");

	intArr[0] = 100;
	charArr[0] = 'X';
	doubleArr[0] = 1.5;
	APS_ASSERT(intArr[0] == 100, "mixed int value");
	APS_ASSERT(charArr[0] == 'X', "mixed char value");
	APS_ASSERT(doubleArr[0] == 1.5, "mixed double value");

	pool.release(intArr, 10);
	pool.release(charArr, 20);
	pool.release(doubleArr, 5);
}

void ArrayPoolSingleTest::testApsReuseAfterMixedRelease()
{
	ArrayPoolSingle pool;
	int* arr1 = pool.get<int>(5, false);
	for (int i = 0; i < 5; ++i) arr1[i] = i;
	pool.release(arr1, 5);

	double* arr2 = pool.get<double>(5, false);
	for (int i = 0; i < 5; ++i) arr2[i] = i * 1.5;
	pool.release(arr2, 5);

	// 不同类型不共用池
	int* arr3 = pool.get<int>(5, false);
	APS_ASSERT(arr3 != nullptr, "mixed reuse int type");
	pool.release(arr3, 5);
}

// =====================================================================
// ArrayPoolSingle — 批量操作
// =====================================================================

void ArrayPoolSingleTest::testApsBatchAllocFree()
{
	ArrayPoolSingle pool;
	constexpr int COUNT = 100;
	int* arrays[COUNT];

	// 批量分配
	for (int i = 0; i < COUNT; ++i)
	{
		arrays[i] = pool.get<int>(i + 1, false);
		APS_ASSERT(arrays[i] != nullptr, "batch alloc");
	}

	// 批量释放（逆序）
	for (int i = COUNT - 1; i >= 0; --i)
	{
		pool.release(arrays[i], i + 1);
	}
	APS_ASSERT(true, "batch alloc/free done");
}

void ArrayPoolSingleTest::testApsBatchSameSize()
{
	ArrayPoolSingle pool;
	constexpr int COUNT = 50;
	constexpr int SIZE = 10;
	int* arrays[COUNT];

	for (int i = 0; i < COUNT; ++i)
	{
		arrays[i] = pool.get<int>(SIZE, false);
		APS_ASSERT(arrays[i] != nullptr, "batch same size alloc");
		arrays[i][0] = i * 10;
	}
	for (int i = 0; i < COUNT; ++i)
	{
		pool.release(arrays[i], SIZE);
	}
	APS_ASSERT(true, "batch same size free");

	// 再次分配相同大小，应该复用
	int* reused = pool.get<int>(SIZE, false);
	APS_ASSERT(reused != nullptr, "batch same size reuse");
	pool.release(reused, SIZE);
}

void ArrayPoolSingleTest::testApsAllocAllSizes()
{
	ArrayPoolSingle pool;
	// 从 1 到 100 的连续大小
	for (int size = 1; size <= 100; ++size)
	{
		int* arr = pool.get<int>(size, false);
		APS_ASSERT(arr != nullptr, "alloc size");
		arr[size - 1] = size;
		pool.release(arr, size);
	}
	APS_ASSERT(true, "alloc all sizes 1-100 done");
}

// =====================================================================
// ArrayPoolSingle — edge cases
// =====================================================================

void ArrayPoolSingleTest::testApsSizeOne()
{
	ArrayPoolSingle pool;
	int* arr = pool.get<int>(1, false);
	APS_ASSERT(arr != nullptr, "size 1 alloc");
	arr[0] = 999;
	APS_ASSERT(arr[0] == 999, "size 1 value");
	pool.release(arr, 1);
}

void ArrayPoolSingleTest::testApsSizeLarge()
{
	ArrayPoolSingle pool;
	int* arr = pool.get<int>(10000, false);
	APS_ASSERT(arr != nullptr, "large alloc");
	arr[0] = 1;
	arr[9999] = 9999;
	APS_ASSERT(arr[0] == 1, "large[0]");
	APS_ASSERT(arr[9999] == 9999, "large[9999]");
	pool.release(arr, 10000);
}

void ArrayPoolSingleTest::testApsDestroy()
{
	ArrayPoolSingle pool;
	int* arr = pool.get<int>(10, false);
	APS_ASSERT(arr != nullptr, "destroy pre alloc");
	pool.release(arr, 10);
	pool.destroy();
	APS_ASSERT(true, "destroy no crash");
}

// =====================================================================
// ArrayPoolSingleChar
// =====================================================================

void ArrayPoolSingleTest::testApsCharPoolGetRelease()
{
	ArrayPoolSingleChar pool;
	char* buf = pool.get(32, false);
	APS_ASSERT(buf != nullptr, "charPool get non-null");
	buf[0] = 'H';
	buf[1] = 'i';
	buf[31] = 0;
	APS_ASSERT(buf[0] == 'H', "charPool data[0]");
	APS_ASSERT(buf[1] == 'i', "charPool data[1]");
	pool.release(buf, 32);
}

void ArrayPoolSingleTest::testApsCharPoolGetZeroMemory()
{
	ArrayPoolSingleChar pool;
	char* buf = pool.get(64, true);
	APS_ASSERT(buf != nullptr, "charPool zeroMemory non-null");
	for (int i = 0; i < 64; ++i)
	{
		APS_ASSERT(buf[i] == 0, "charPool zeroMemory element");
	}
	pool.release(buf, 64);
}

void ArrayPoolSingleTest::testApsCharPoolReuse()
{
	ArrayPoolSingleChar pool;
	char* buf1 = pool.get(16, false);
	buf1[0] = 'X';
	pool.release(buf1, 16);

	char* buf2 = pool.get(16, false);
	APS_ASSERT(buf2 != nullptr, "charPool reuse non-null");
	pool.release(buf2, 16);
}

void ArrayPoolSingleTest::testApsCharPoolMultipleSizes()
{
	ArrayPoolSingleChar pool;
	char* sizes[5];
	int values[] = {8, 16, 32, 64, 128};
	for (int i = 0; i < 5; ++i)
	{
		sizes[i] = pool.get(values[i], false);
		APS_ASSERT(sizes[i] != nullptr, "charPool multiple sizes");
	}
	for (int i = 4; i >= 0; --i)
	{
		pool.release(sizes[i], values[i]);
	}
	APS_ASSERT(true, "charPool multiple sizes release");
}

void ArrayPoolSingleTest::testApsCharPoolReleaseNull()
{
	ArrayPoolSingleChar pool;
	char* ptr = nullptr;
	pool.release(ptr, 0);
	pool.release(ptr, 100);
	APS_ASSERT(true, "charPool release nullptr no crash");
}

void ArrayPoolSingleTest::testApsCharPoolBatch()
{
	ArrayPoolSingleChar pool;
	constexpr int COUNT = 30;
	constexpr int SIZE = 20;
	char* buffers[COUNT];

	for (int i = 0; i < COUNT; ++i)
	{
		buffers[i] = pool.get(SIZE, false);
		APS_ASSERT(buffers[i] != nullptr, "charPool batch alloc");
	}
	for (int i = 0; i < COUNT; ++i)
	{
		pool.release(buffers[i], SIZE);
	}
	APS_ASSERT(true, "charPool batch free");
}

void ArrayPoolSingleTest::testApsCharPoolIsNewAlloc()
{
	ArrayPoolSingleChar pool;
	bool isNew = false;
	char* buf1 = pool.get(10, false, &isNew);
	APS_ASSERT(isNew, "charPool isNewAlloc first time");
	pool.release(buf1, 10);

	char* buf2 = pool.get(10, false, &isNew);
	// 复用时不一定是新分配
	pool.release(buf2, 10);
	APS_ASSERT(true, "charPool isNewAlloc second time no crash");
}

void ArrayPoolSingleTest::testApsCharPoolDestroy()
{
	ArrayPoolSingleChar pool;
	char* buf = pool.get(32, false);
	pool.release(buf, 32);
	pool.destroy();
	APS_ASSERT(true, "charPool destroy no crash");
}

void ArrayPoolSingleTest::testApsCharPoolConsecutiveReuse()
{
	ArrayPoolSingleChar pool;
	constexpr int CYCLE = 10;
	for (int cycle = 0; cycle < CYCLE; ++cycle)
	{
		char* buf = pool.get(50, false);
		APS_ASSERT(buf != nullptr, "charPool cycle alloc");
		buf[0] = (char)('A' + cycle);
		pool.release(buf, 50);
	}
	APS_ASSERT(true, "charPool consecutive reuse done");
}

// =====================================================================
// 主入口
// =====================================================================
void ArrayPoolSingleTest::test()
{
	// ArrayPoolSingle int
	testApsIntGetRelease();
	testApsIntReuse();
	testApsIntMultipleSizes();
	testApsIntZeroMemory();
	testApsIntZeroMemoryReuse();
	testApsIntReleaseNull();
	testApsIntReleaseZeroCount();
	testApsIntMultiplePools();
	testApsIntGetLargerThanPrevious();

	// ArrayPoolSingle char
	testApsCharGetRelease();
	testApsCharMultipleSizes();
	testApsCharReleaseNull();

	// ArrayPoolSingle double
	testApsDoubleGetRelease();
	testApsDoubleZeroMemory();

	// Mixed types
	testApsMixedTypes();
	testApsReuseAfterMixedRelease();

	// Batch
	testApsBatchAllocFree();
	testApsBatchSameSize();
	testApsAllocAllSizes();

	// Edge cases
	testApsSizeOne();
	testApsSizeLarge();
	testApsDestroy();

	// ArrayPoolSingleChar
	testApsCharPoolGetRelease();
	testApsCharPoolGetZeroMemory();
	testApsCharPoolReuse();
	testApsCharPoolMultipleSizes();
	testApsCharPoolReleaseNull();
	testApsCharPoolBatch();
	testApsCharPoolIsNewAlloc();
	testApsCharPoolDestroy();
	testApsCharPoolConsecutiveReuse();
}
#endif
