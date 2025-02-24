#include "FrameHeader.h"

void FrameStateManager::quit()
{
	for (const auto& iter : mGroupStateList)
	{
		delete iter.second;
	}
	mGroupStateList.clear();
	mStateGroupList.clear();
}