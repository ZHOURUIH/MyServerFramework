// auto generate start
#include "EDAchivement.h"

void EDAchivement::cloneTo(ExcelData* target)
{
	base::cloneTo(target);
	auto* targetData = static_cast<This*>(target);
	targetData->mTestList1 = mTestList1;
	targetData->mReward = mReward;
}

void EDAchivement::read(SerializerRead* reader)
{
	base::read(reader);
	reader->read(mTestList1);
	reader->read(mReward);
}
// auto generate end