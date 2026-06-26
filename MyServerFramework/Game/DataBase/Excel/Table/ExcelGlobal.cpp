// auto generate start
#include "GameHeader.h"

Vector2Int ExcelGlobal::TEST_GLOBAL_3;
Vector2 ExcelGlobal::TEST_GLOBAL_4;
Vector3 ExcelGlobal::TEST_GLOBAL_5;
Vector3Int ExcelGlobal::TEST_GLOBAL_6;
Vector<int> ExcelGlobal::TEST_GLOBAL_7;
Vector<float> ExcelGlobal::TEST_GLOBAL_8;
Vector<llong> ExcelGlobal::TEST_GLOBAL_9;
string ExcelGlobal::TEST_GLOBAL_10;

void ExcelGlobal::init(const string& tableName)
{
	base::init(tableName);
	Map<string, string> paramMap;
	for (const auto& item : getAllData())
	{
		removeAll(item.second->mParamValue, ' ');
		paramMap.add(item.second->mParamName, item.second->mParamValue);
	}
	TEST_GLOBAL_3 = SToV2I(paramMap[STR(TEST_GLOBAL_3)]);
	TEST_GLOBAL_4 = SToV2(paramMap[STR(TEST_GLOBAL_4)]);
	TEST_GLOBAL_5 = SToV3(paramMap[STR(TEST_GLOBAL_5)]);
	TEST_GLOBAL_6 = SToV3I(paramMap[STR(TEST_GLOBAL_6)]);
	SToIs(paramMap[STR(TEST_GLOBAL_7)], TEST_GLOBAL_7);
	SToFs(paramMap[STR(TEST_GLOBAL_8)], TEST_GLOBAL_8);
	SToLLs(paramMap[STR(TEST_GLOBAL_9)], TEST_GLOBAL_9);
}

void ExcelGlobal::checkAllDataDefault() {}
// auto generate end