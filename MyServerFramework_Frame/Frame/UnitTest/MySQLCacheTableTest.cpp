#ifdef WINDOWS
#ifdef _MYSQL
#include "FrameHeader.h"

namespace MySQLCacheTableTestLocal
{
#define MCTT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MySQLCacheTableTest FAILED: ") + (msg)); } } while(0)
}

#ifdef _MYSQL
void MySQLCacheTableTest::testConstructor()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getCacheData(0) == nullptr, "empty cache: getCacheData(0) should be nullptr");
		MCTT_ASSERT(table.getCacheData(1) == nullptr, "empty cache: getCacheData(1) should be nullptr");
		MCTT_ASSERT(table.getCacheData(-1) == nullptr, "empty cache: getCacheData(-1) should be nullptr");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testColumnToFlag()
{
	using namespace MySQLCacheTableTestLocal;
		// 对任意非负 colName, columnToFlag 产生该 bit 置 1 的 flag
		MySQLCacheTable table(nullptr);
		ullong flag0 = table.columnToFlag(0);
		MCTT_ASSERT(hasBit(flag0, 0), "columnToFlag(0) should have bit 0 set");

		ullong flag1 = table.columnToFlag(1);
		MCTT_ASSERT(hasBit(flag1, 1), "columnToFlag(1) should have bit 1 set");
		MCTT_ASSERT(!hasBit(flag1, 0), "columnToFlag(1) should NOT have bit 0 set");

		ullong flag63 = table.columnToFlag(63);
		MCTT_ASSERT(hasBit(flag63, 63), "columnToFlag(63) should have bit 63 set");

		// 不同 colName 产生不同 flag
		MCTT_ASSERT(flag0 != flag1, "columnToFlag should produce different flags for different columns");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testColumnsToFlag()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);

		// 空列表 -> flag = 0
		Vector<int> empty;
		ullong flagEmpty = table.columnsToFlag(empty);
		MCTT_ASSERT(flagEmpty == 0, "columnsToFlag(empty) should be 0");

		// 单字段等价于 columnToFlag
		Vector<int> singleCol;
		singleCol.add(3);
		ullong flagSingle = table.columnsToFlag(singleCol);
		MCTT_ASSERT(flagSingle == table.columnToFlag(3), "columnsToFlag({3}) == columnToFlag(3)");

		// 多字段组合
		Vector<int> multiCol;
		multiCol.add(0);
		multiCol.add(5);
		multiCol.add(10);
		ullong flagMulti = table.columnsToFlag(multiCol);
		MCTT_ASSERT(hasBit(flagMulti, 0), "should have bit 0");
		MCTT_ASSERT(hasBit(flagMulti, 5), "should have bit 5");
		MCTT_ASSERT(hasBit(flagMulti, 10), "should have bit 10");
		MCTT_ASSERT(!hasBit(flagMulti, 1), "should NOT have bit 1");

		// 组合与单个之和一致
		ullong expected = table.columnToFlag(0) | table.columnToFlag(5) | table.columnToFlag(10);
		MCTT_ASSERT(flagMulti == expected, "columnsToFlag equals OR of individual columnToFlag");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetCacheData_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getCacheData(0) == nullptr, "getCacheData on empty cache returns nullptr");
		MCTT_ASSERT(table.getCacheData(LLONG_MAX) == nullptr, "getCacheData(LLONG_MAX) on empty cache returns nullptr");
		MCTT_ASSERT(table.getCacheData(LLONG_MIN) == nullptr, "getCacheData(LLONG_MIN) on empty cache returns nullptr");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByInt_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByInt(0, 0) == nullptr, "getDataByInt on empty cache returns nullptr");
		MCTT_ASSERT(table.getDataByInt(5, 999) == nullptr, "getDataByInt(5, 999) on empty cache returns nullptr");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByLLong_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByLLong(0, 0) == nullptr, "getDataByLLong on empty cache returns nullptr");
		MCTT_ASSERT(table.getDataByLLong(3, 12345LL) == nullptr, "getDataByLLong(3, 12345) on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByString_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByString(0, "test") == nullptr, "getDataByString on empty cache returns nullptr");
		MCTT_ASSERT(table.getDataByString(2, "") == nullptr, "getDataByString(2, empty) on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByLLong2Or_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByLLong2Or(0, 1LL, 1, 2LL) == nullptr, "getDataByLLong2Or on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByLLong2And_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByLLong2And(0, 1LL, 1, 2LL) == nullptr, "getDataByLLong2And on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByString2And_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByString2And(0, "a", 1, "b") == nullptr, "getDataByString2And on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByLLongIntAnd_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByLLongIntAnd(0, 1LL, 1, 2) == nullptr, "getDataByLLongIntAnd on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetDataByLLongStringAnd_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		MCTT_ASSERT(table.getDataByLLongStringAnd(0, 1LL, 1, "x") == nullptr, "getDataByLLongStringAnd on empty cache");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testGetCacheDataListByInt_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		Vector<MySQLData*> dataList;
		table.getCacheDataListByInt(0, 42, dataList);
		MCTT_ASSERT(dataList.size() == 0, "getCacheDataListByInt on empty should yield empty list");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testUpdateNonExistent_NoCrash()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		// 这些方法内部通过 mCacheDataList.get(id) 查找, 找不到就跳过
		table.updateBool(999, true, 0);
		table.updateInt(999, 42, 1);
		table.updateInt2(999, 1, 0, 2, 1);
		table.updateFloat(999, 3.14f, 2);
		table.updateLLong(999, 100LL, 3);
		table.updateLLongInt(999, 10LL, 0, 20, 1);
		table.updateLLongBool(999, 10LL, 0, true, 1);
		table.updateString(999, "hello", 4);
		table.updateString2(999, "a", 0, "b", 1);
		MCTT_ASSERT(true, "update methods on non-existent ID should not crash");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testUpdateAll_Empty_NoCrash()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		table.updateAllString(0, "x");
		table.updateAllInt(1, 0);
		table.updateAllLLong(2, 0LL);
		table.updateAllFloat(3, 0.0f);
		table.updateAllBool(4, true);
		MCTT_ASSERT(true, "updateAll methods on empty cache should not crash");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testAddOrUpdateData_NullPtr()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		// addOrUpdateData(nullptr) 应该直接 return, 不访问 pool
		table.addOrUpdateData(nullptr);
		MCTT_ASSERT(true, "addOrUpdateData(nullptr) should not crash");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testAddOrUpdateDataList_EmptyOrNull()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		// 空列表
		{
			Vector<MySQLData*> emptyList;
			table.addOrUpdateDataList(emptyList);
			MCTT_ASSERT(true, "addOrUpdateDataList(empty) should not crash");
		}
		// 列表中全是 nullptr
		{
			Vector<MySQLData*> nullList;
			nullList.add(nullptr);
			nullList.add(nullptr);
			table.addOrUpdateDataList(nullList);
			MCTT_ASSERT(true, "addOrUpdateDataList({nullptr,nullptr}) should not crash");
		}
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testDeleteData_NonExistent()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		table.deleteData(999);
		table.deleteData(0);
		table.deleteData(-1);
		MCTT_ASSERT(true, "deleteData on non-existent ID should not crash");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testDeleteDataInIDRange_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		table.deleteDataInIDRange(0, 100);
		table.deleteDataInIDRange(LLONG_MIN, LLONG_MAX);
		MCTT_ASSERT(true, "deleteDataInIDRange on empty cache should not crash");
	}
#endif

#ifdef _MYSQL
void MySQLCacheTableTest::testDeleteDataByColumn_Empty()
{
	using namespace MySQLCacheTableTestLocal;
		MySQLCacheTable table(nullptr);
		// 单字段: onlyOnce = false
		table.deleteDataByColumnBool(0, true, false);
		table.deleteDataByColumnInt(1, 42, false);
		table.deleteDataByColumnFloat(2, 3.14f, false);
		table.deleteDataByColumnLLong(3, 100LL, false);
		table.deleteDataByColumnString(4, "test", false);
		// 双字段组合
		table.deleteDataByColumnLLong2And(0, 1LL, 1, 2LL, false);
		table.deleteDataByColumnLLongIntAnd(0, 1LL, 1, 2, false);
		table.deleteDataByColumnLLong2Or(0, 1LL, 1, 2LL, false);
		// onlyOnce = true 的版本
		table.deleteDataByColumnBool(0, true, true);
		table.deleteDataByColumnInt(1, 42, true);
		table.deleteDataByColumnFloat(2, 3.14f, true);
		table.deleteDataByColumnLLong(3, 100LL, true);
		table.deleteDataByColumnString(4, "test", true);
		table.deleteDataByColumnLLong2And(0, 1LL, 1, 2LL, true);
		table.deleteDataByColumnLLongIntAnd(0, 1LL, 1, 2, true);
		table.deleteDataByColumnLLong2Or(0, 1LL, 1, 2LL, true);
		MCTT_ASSERT(true, "all deleteDataByColumn methods on empty cache should not crash");
	}
#endif

void MySQLCacheTableTest::test()
{
	testConstructor();
	testColumnToFlag();
	testColumnsToFlag();
	testGetCacheData_Empty();
	testGetDataByInt_Empty();
	testGetDataByLLong_Empty();
	testGetDataByString_Empty();
	testGetDataByLLong2Or_Empty();
	testGetDataByLLong2And_Empty();
	testGetDataByString2And_Empty();
	testGetDataByLLongIntAnd_Empty();
	testGetDataByLLongStringAnd_Empty();
	testGetCacheDataListByInt_Empty();
	testUpdateNonExistent_NoCrash();
	testUpdateAll_Empty_NoCrash();
	testAddOrUpdateData_NullPtr();
	testAddOrUpdateDataList_EmptyOrNull();
	testDeleteData_NonExistent();
	testDeleteDataInIDRange_Empty();
	testDeleteDataByColumn_Empty();
}
#endif
#endif
