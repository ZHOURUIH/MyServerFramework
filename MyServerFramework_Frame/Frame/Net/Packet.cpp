#include "FrameHeader.h"

void Packet::resetProperty()
{
	base::resetProperty();
	mFieldFlag = FrameDefine::FULL_FIELD_FLAG;
	mSequenceNumber = 0;
	// mType,mShowInfo不需要重置
	// mType = 0;
	// mShowInfo = true;
}