// auto generate start
#include "EDGlobal.h"


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