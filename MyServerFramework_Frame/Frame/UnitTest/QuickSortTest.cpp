#ifdef WINDOWS
#include "FrameHeader.h"

namespace QuickSortTestLocal
{
#define QS_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("QuickSortTest FAILED: ") + (msg)); } } while(0)
}

template<typename T>
bool QuickSortTest::isSortedAscend(const T* arr, int count)
{
	using namespace QuickSortTestLocal;
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] < arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

template<typename T>
bool QuickSortTest::isSortedDescend(const T* arr, int count)
{
	using namespace QuickSortTestLocal;
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] > arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

void QuickSortTest::testQuickSortAscend_Basic()
{
	using namespace QuickSortTestLocal;
		int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend basic");
		QS_ASSERT(arr[0] == 1 && arr[n - 1] == 9, "ascend range");
	}

void QuickSortTest::testQuickSortAscend_AlreadySorted()
{
	using namespace QuickSortTestLocal;
		int arr[] = {1, 2, 3, 4, 5, 6, 7};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		int copy[n];
		MEMCPY(copy, sizeof(copy), arr, sizeof(arr));
		MathUtility::quickSortAscend(copy, 0, n - 1);
		QS_ASSERT(isSortedAscend(copy, n), "ascend already sorted stable");
	}

void QuickSortTest::testQuickSortAscend_Reverse()
{
	using namespace QuickSortTestLocal;
		int arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend reverse");
	}

void QuickSortTest::testQuickSortAscend_AllSame()
{
	using namespace QuickSortTestLocal;
		int arr[] = {5, 5, 5, 5, 5};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend all same");
	}

void QuickSortTest::testQuickSortAscend_TrivialSize()
{
	using namespace QuickSortTestLocal;
		int a1[] = {42};
		MathUtility::quickSortAscend(a1, 0, 0);
		QS_ASSERT(a1[0] == 42, "single element");

		int a2[] = {10, 5};
		MathUtility::quickSortAscend(a2, 0, 1);
		QS_ASSERT(a2[0] == 5 && a2[1] == 10, "two elements");
	}

void QuickSortTest::testQuickSortAscend_Negative()
{
	using namespace QuickSortTestLocal;
		int arr[] = {-3, 5, -1, 0, -7, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend negative");
		QS_ASSERT(arr[0] == -7 && arr[n - 1] == 5, "ascend negative range");
	}

void QuickSortTest::testQuickSortAscend_Float()
{
	using namespace QuickSortTestLocal;
		float arr[] = {3.14f, -1.5f, 0.0f, 2.71f, -0.01f};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1);
		QS_ASSERT(isSortedAscend(arr, n), "ascend float");
	}

void QuickSortTest::testQuickSortDescend_Basic()
{
	using namespace QuickSortTestLocal;
		int arr[] = {5, 3, 8, 1, 9, 2, 7, 4, 6};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortDescend(arr, 0, n - 1);
		QS_ASSERT(isSortedDescend(arr, n), "descend basic");
		QS_ASSERT(arr[0] == 9 && arr[n - 1] == 1, "descend range");
	}

void QuickSortTest::testQuickSortDescend_Negative()
{
	using namespace QuickSortTestLocal;
		int arr[] = {-3, 5, -1, 0, -7, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		quickSortDescend(arr, 0, n - 1);
		QS_ASSERT(isSortedDescend(arr, n), "descend negative");
		QS_ASSERT(arr[0] == 5 && arr[n - 1] == -7, "descend negative range");
	}

void QuickSortTest::testQuickSortAscendPtr_Basic()
{
	using namespace QuickSortTestLocal;
		int values[] = {30, 10, 20, 50, 40};
		int* ptrs[5];
		FOR(5)
		{
			ptrs[i] = &values[i];
		}
		quickSortAscendPtr(ptrs, 0, 4);
		// ptrs should now point to values in ascending order
		for (int i = 1; i < 5; ++i)
		{
			QS_ASSERT(*ptrs[i] >= *ptrs[i - 1], "ascend ptr order");
		}
		QS_ASSERT(*ptrs[0] == 10 && *ptrs[4] == 50, "ascend ptr range");
	}

void QuickSortTest::testQuickSortAscendPtr_DuplicateValues()
{
	using namespace QuickSortTestLocal;
		int values[] = {20, 10, 20, 10, 20};
		int* ptrs[5];
		FOR(5)
		{
			ptrs[i] = &values[i];
		}
		MathUtility::quickSortAscendPtr(ptrs, 0, 4);
		for (int i = 1; i < 5; ++i)
		{
			QS_ASSERT(*ptrs[i] >= *ptrs[i - 1], "ascend ptr dup order");
		}
	}

void QuickSortTest::testQuickSortDescendPtr_Basic()
{
	using namespace QuickSortTestLocal;
		int values[] = {30, 10, 20, 50, 40};
		int* ptrs[5];
		FOR(5)
		{
			ptrs[i] = &values[i];
		}
		MathUtility::quickSortDescendPtr(ptrs, 0, 4);
		for (int i = 1; i < 5; ++i)
		{
			QS_ASSERT(*ptrs[i] <= *ptrs[i - 1], "descend ptr order");
		}
		QS_ASSERT(*ptrs[0] == 50 && *ptrs[4] == 10, "descend ptr range");
	}

int QuickSortTest::compareInt(const int& a, const int& b)
{
	using namespace QuickSortTestLocal;
		if (a < b)
		{
			return -1;
		}
		if (a > b)
		{
			return 1;
		}
		return 0;
	}

void QuickSortTest::testQuickSortAscend_Compare()
{
	using namespace QuickSortTestLocal;
		int arr[] = {5, 3, 8, 1, 9, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortAscend(arr, 0, n - 1, compareInt);
		QS_ASSERT(isSortedAscend(arr, n), "ascend compare basic");
	}

void QuickSortTest::testQuickSortDescend_Compare()
{
	using namespace QuickSortTestLocal;
		int arr[] = {5, 3, 8, 1, 9, 2};
		const int n = (int)(sizeof(arr) / sizeof(arr[0]));
		MathUtility::quickSortDescend(arr, 0, n - 1, compareInt);
		QS_ASSERT(isSortedDescend(arr, n), "descend compare basic");
	}

void QuickSortTest::testQuickSort_CompareNullptr()
{
	using namespace QuickSortTestLocal;
		int arr[] = {3, 1, 2};
		// 不应崩溃，直接返回
		quickSortAscend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		quickSortDescend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		QS_ASSERT(true, "nullptr compare does not crash");

		// NonRecursive 版本也测一下
		int pivot = MathUtility::quickSortNonRecursiveAscend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		QS_ASSERT(pivot == -1, "nonrecursive ascend null returns -1");
		pivot = MathUtility::quickSortNonRecursiveDescend(arr, 0, 2, (CompareTCallback<int>)nullptr);
		QS_ASSERT(pivot == -1, "nonrecursive descend null returns -1");
	}

void QuickSortTest::testQuickSort_LargeDataset()
{
	using namespace QuickSortTestLocal;
		const int N = 1000;
		int* arr = new int[N];
		FOR(N)
		{
			arr[i] = N - i; // 逆序
		}
		MathUtility::quickSortAscend(arr, 0, N - 1);
		QS_ASSERT(isSortedAscend(arr, N), "large dataset ascend");
		QS_ASSERT(arr[0] == 1 && arr[N - 1] == N, "large dataset range");

		// 再排降序
		MathUtility::quickSortDescend(arr, 0, N - 1);
		QS_ASSERT(isSortedDescend(arr, N), "large dataset descend");
		delete[] arr;
	}

void QuickSortTest::testQuickSortNonRecursive_PivotReturn()
{
	using namespace QuickSortTestLocal;
		int arr[] = {5, 3, 8, 1, 9};
		int pivot = MathUtility::quickSortNonRecursiveAscend(arr, 0, 4);
		QS_ASSERT(pivot >= 0 && pivot <= 4, "nonrecur pivot valid range");
		// pivot 左侧 <= arr[pivot]，右侧 > arr[pivot]
		FOR(pivot)
		{
			QS_ASSERT(arr[i] <= arr[pivot], "nonrecur left side <= pivot");
		}
		for (int i = pivot + 1; i <= 4; ++i)
		{
			QS_ASSERT(arr[i] > arr[pivot], "nonrecur right side > pivot");
		}
	}

void QuickSortTest::test()
{
	testQuickSortAscend_Basic();
	testQuickSortAscend_AlreadySorted();
	testQuickSortAscend_Reverse();
	testQuickSortAscend_AllSame();
	testQuickSortAscend_TrivialSize();
	testQuickSortAscend_Negative();
	testQuickSortAscend_Float();
	testQuickSortDescend_Basic();
	testQuickSortDescend_Negative();
	testQuickSortAscendPtr_Basic();
	testQuickSortAscendPtr_DuplicateValues();
	testQuickSortDescendPtr_Basic();
	testQuickSortAscend_Compare();
	testQuickSortDescend_Compare();
	testQuickSort_CompareNullptr();
	testQuickSort_LargeDataset();
	testQuickSortNonRecursive_PivotReturn();
}
#endif
