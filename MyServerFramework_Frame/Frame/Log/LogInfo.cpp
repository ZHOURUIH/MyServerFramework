#include "FrameHeader.h"

void LogInfo::resetProperty()
{
	base::resetProperty();
	mPlayerGUID = 0;
	mTime = 0;
	mLog.clear();
	mStack.clear();
	mError = false;
}