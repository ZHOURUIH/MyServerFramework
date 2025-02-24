#include "FrameHeader.h"

void UDPClientInfo::clear()
{
	for (const auto& item : mSendList)
	{
		delete[] item.first;
	}
	mSendList.clear();
}