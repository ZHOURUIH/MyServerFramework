#include "GameHeader.h"

// auto generate start
void ExcelTest::checkAllDataDefault()
{
	for (const auto& item : getAllData())
	{
		EDTest* data = item.second;
		checkEnumResult(GameEnumCheck::checkEnum(data->mTestEnum), "mTestEnum", item.first);
		mExcelAchivement->checkData(data->mTestLinkTable0, item.first, this);
		mExcelAchivement->checkData(data->mTestLinkTable1, item.first, this);
		checkListPair(item.second->mTestList0, item.second->mTestList1, item.first);
	}
}
// auto generate end