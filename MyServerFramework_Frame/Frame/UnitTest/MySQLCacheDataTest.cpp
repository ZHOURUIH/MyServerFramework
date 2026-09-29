#ifdef WINDOWS
#ifdef _MYSQL
#include "FrameHeader.h"

namespace MySQLCacheDataTestLocal
{
#define MCD_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MySQLCacheDataTest FAILED: ") + (msg)); } } while(0)
}

#ifdef _MYSQL
void MySQLCacheDataTest::testResetProperty()
{
	using namespace MySQLCacheDataTestLocal;
		MySQLCacheData data;
		// 构造后直接 resetProperty
		data.resetProperty();
		MCD_ASSERT(data.getValidFlag() == 0, "after resetProperty, validFlag should be 0");
		MCD_ASSERT(data.getData() == nullptr, "after resetProperty, mData should be nullptr");
		MCD_ASSERT(data.isColumnValid(0) == false, "column 0 should be invalid after reset");
		MCD_ASSERT(data.isColumnValid(63) == false, "column 63 should be invalid after reset");
	}
#endif

#ifdef _MYSQL
void MySQLCacheDataTest::testAddValidFlag()
{
	using namespace MySQLCacheDataTestLocal;
		MySQLCacheData data;
		data.resetProperty();

		MCD_ASSERT(data.getValidFlag() == 0, "initial validFlag = 0");

		data.addValidFlag(1);
		MCD_ASSERT(data.getValidFlag() == 1, "after addValidFlag(1), flag = 1");

		data.addValidFlag(4);
		ullong expected = 1 | 4;
		MCD_ASSERT(data.getValidFlag() == expected, "flags should accumulate (1 | 4)");

		// 再次添加相同的 flag, 因为 | 操作不会导致变化
		data.addValidFlag(1);
		MCD_ASSERT(data.getValidFlag() == expected, "re-adding same flag should not change value");
	}
#endif

#ifdef _MYSQL
void MySQLCacheDataTest::testSetAndCheckColumnValid()
{
	using namespace MySQLCacheDataTestLocal;
		MySQLCacheData data;
		data.resetProperty();

		// 初始全部无效
		for (int i = 0; i < 10; ++i)
		{
			MCD_ASSERT(data.isColumnValid(i) == false, "initial: column i should be invalid");
		}

		// 设置 column 2 有效
		data.setColumnValid(2);
		MCD_ASSERT(data.isColumnValid(2) == true, "column 2 should be valid after setColumnValid(2)");
		MCD_ASSERT(data.isColumnValid(0) == false, "column 0 should still be invalid");
		MCD_ASSERT(data.isColumnValid(1) == false, "column 1 should still be invalid");
		MCD_ASSERT(data.isColumnValid(3) == false, "column 3 should still be invalid");

		// 设置 column 5
		data.setColumnValid(5);
		MCD_ASSERT(data.isColumnValid(2) == true, "column 2 still valid");
		MCD_ASSERT(data.isColumnValid(5) == true, "column 5 now valid");

		// 设置边界: bit 63
		data.setColumnValid(63);
		MCD_ASSERT(data.isColumnValid(63) == true, "column 63 should be valid");

		// 设置已有 bit 不影响结果
		data.setColumnValid(2);
		MCD_ASSERT(data.isColumnValid(2) == true, "re-setting column 2 should still be valid");
	}
#endif

#ifdef _MYSQL
void MySQLCacheDataTest::testIsColumnNameValid()
{
	using namespace MySQLCacheDataTestLocal;
		MySQLCacheData data;
		data.resetProperty();

		// colName < 0 直接返回 false, 不检查有效性
		MCD_ASSERT(data.isColumnNameValid(-1) == false, "colName < 0 => false");
		MCD_ASSERT(data.isColumnNameValid(-100) == false, "colName < 0 => false");

		// colName >= 0 且 column 无效 => false
		MCD_ASSERT(data.isColumnNameValid(0) == false, "colName >= 0 but not set => false");

		// 设置后 => true
		data.setColumnValid(0);
		MCD_ASSERT(data.isColumnNameValid(0) == true, "colName=0 set valid => true");
	}
#endif

#ifdef _MYSQL
void MySQLCacheDataTest::testIsColumnNamesValid_NonEmpty()
{
	using namespace MySQLCacheDataTestLocal;
		MySQLCacheData data;
		data.resetProperty();

		// 设置几个有效字段
		data.setColumnValid(1);
		data.setColumnValid(3);
		data.setColumnValid(5);

		// 所有指定字段都有效 => true
		{
			Vector<int> cols;
			cols.add(1);
			cols.add(3);
			MCD_ASSERT(data.isColumnNamesValid(cols) == true, "colNames {1,3} all valid => true");
		}

		// 包含无效字段 => false
		{
			Vector<int> cols;
			cols.add(1);
			cols.add(4);  // 未设置
			MCD_ASSERT(data.isColumnNamesValid(cols) == false, "colNames {1,4} has invalid => false");
		}

		// 全部无效 => false
		{
			Vector<int> cols;
			cols.add(0);
			cols.add(2);
			MCD_ASSERT(data.isColumnNamesValid(cols) == false, "colNames {0,2} all invalid => false");
		}

		// 单个有效字段
		{
			Vector<int> cols;
			cols.add(5);
			MCD_ASSERT(data.isColumnNamesValid(cols) == true, "single valid column => true");
		}

		// 单个无效字段
		{
			Vector<int> cols;
			cols.add(7);
			MCD_ASSERT(data.isColumnNamesValid(cols) == false, "single invalid column => false");
		}

		// colName < 0 在列表中 => false
		{
			Vector<int> cols;
			cols.add(1);
			cols.add(-1);
			MCD_ASSERT(data.isColumnNamesValid(cols) == false, "colName < 0 in list => false");
		}
	}
#endif

#ifdef _MYSQL
void MySQLCacheDataTest::testValidFlagConsistency()
{
	using namespace MySQLCacheDataTestLocal;
		MySQLCacheData data;
		data.resetProperty();

		data.setColumnValid(0);
		data.setColumnValid(10);
		data.setColumnValid(20);

		ullong expected = (1ULL << 0) | (1ULL << 10) | (1ULL << 20);
		MCD_ASSERT(data.getValidFlag() == expected, "validFlag should match set columns");
	}
#endif

void MySQLCacheDataTest::test()
{
	testResetProperty();
	testAddValidFlag();
	testSetAndCheckColumnValid();
	testIsColumnNameValid();
	testIsColumnNamesValid_NonEmpty();
	testValidFlagConsistency();
}
#endif
#endif
