// auto generate start
#include "EDGlobal.h"

Vector2Int EDGlobal::TEST_GLOBAL_3;
Vector2 EDGlobal::TEST_GLOBAL_4;
Vector3 EDGlobal::TEST_GLOBAL_5;
Vector3Int EDGlobal::TEST_GLOBAL_6;
Vector<int> EDGlobal::TEST_GLOBAL_7;
Vector<float> EDGlobal::TEST_GLOBAL_8;
Vector<llong> EDGlobal::TEST_GLOBAL_9;
string EDGlobal::TEST_GLOBAL_10;

void EDGlobal::postLoadAll(ExcelTableBase* tableBase)
{
	Map<string, string> paramMap;
	for (const auto& item : mExcelGlobal->getAllData())
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

void EDGlobal::cloneTo(ExcelData* target)
{
	base::cloneTo(target);
	auto* targetData = static_cast<This*>(target);
	targetData->mParamName = mParamName;
	targetData->mParamType = mParamType;
	targetData->mParamValue = mParamValue;
}

void EDGlobal::read(SerializerRead* reader)
{
	base::read(reader);
	reader->readString(mParamName);
	reader->readString(mParamType);
	reader->readString(mParamValue);
}
// auto generate end