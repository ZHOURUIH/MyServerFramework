// auto generate start
#include "EDTest.h"

void EDTest::cloneTo(ExcelData* target)
{
	base::cloneTo(target);
	auto* targetData = static_cast<This*>(target);
	targetData->mTestString = mTestString;
	targetData->mTestInt = mTestInt;
	targetData->mTestLong = mTestLong;
	targetData->mTestVector2Int = mTestVector2Int;
	targetData->mTestVector2 = mTestVector2;
	targetData->mTestEnum = mTestEnum;
	targetData->mTestList0 = mTestList0;
	targetData->mTestList1 = mTestList1;
	targetData->mTestFloat = mTestFloat;
	targetData->mTestStringList = mTestStringList;
	targetData->mTestLinkTable0 = mTestLinkTable0;
	targetData->mTestLinkTable1 = mTestLinkTable1;
	targetData->mTestPath = mTestPath;
}

void EDTest::read(SerializerRead* reader)
{
	base::read(reader);
	reader->readString(mTestString);
	reader->read(mTestInt);
	reader->read(mTestLong);
	reader->readVector2Int(mTestVector2Int);
	reader->readVector2(mTestVector2);
	reader->read(mTestEnum);
	reader->readList(mTestList0);
	reader->readList(mTestList1);
	reader->read(mTestFloat);
	reader->readStringList(mTestStringList);
	reader->read(mTestLinkTable0);
	reader->readList(mTestLinkTable1);
	reader->readString(mTestPath);
}
// auto generate end