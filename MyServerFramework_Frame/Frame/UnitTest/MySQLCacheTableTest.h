#pragma once

#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API MySQLCacheTableTest
{
#ifdef WINDOWS
public:
	static void test();
private:
#ifdef _MYSQL
	static void testConstructor();
	static void testColumnToFlag();
	static void testColumnsToFlag();
	static void testGetCacheData_Empty();
	static void testGetDataByInt_Empty();
	static void testGetDataByLLong_Empty();
	static void testGetDataByString_Empty();
	static void testGetDataByLLong2Or_Empty();
	static void testGetDataByLLong2And_Empty();
	static void testGetDataByString2And_Empty();
	static void testGetDataByLLongIntAnd_Empty();
	static void testGetDataByLLongStringAnd_Empty();
	static void testGetCacheDataListByInt_Empty();
	static void testUpdateNonExistent_NoCrash();
	static void testUpdateAll_Empty_NoCrash();
	static void testAddOrUpdateData_NullPtr();
	static void testAddOrUpdateDataList_EmptyOrNull();
	static void testDeleteData_NonExistent();
	static void testDeleteDataInIDRange_Empty();
	static void testDeleteDataByColumn_Empty();
#endif
#endif
};
