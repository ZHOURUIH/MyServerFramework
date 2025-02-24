#include "FrameHeader.h"

void PacketTCP::resetProperty()
{
	base::resetProperty();
	mClient = nullptr;
}