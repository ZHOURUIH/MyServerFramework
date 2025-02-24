#include "FrameHeader.h"

void StateParam::resetProperty()
{
	base::resetProperty();
	// mParamMemberList构造中填充的,不重置
	//if (mParamMemberList != nullptr)
	//{
	//	mParamMemberList->clear();
	//}
	mTarget = nullptr;
	mSource = nullptr;
	mSourceCharacterGUID = 0;
	mType = 0;
	mStateTime = -1.0f;
}