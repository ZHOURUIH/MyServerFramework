#include "FrameHeader.h"

void PacketWebSocket::resetProperty()
{
	base::resetProperty();
	mClient = nullptr;
}