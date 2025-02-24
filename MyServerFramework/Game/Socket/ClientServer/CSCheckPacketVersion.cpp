#include "GameHeader.h"

// auto generate start
string CSCheckPacketVersion::mPacketName = STR(CSCheckPacketVersion);
// auto generate end

void CSCheckPacketVersion::execute()
{
	SCCheckPacketVersion::send(mClient);
}