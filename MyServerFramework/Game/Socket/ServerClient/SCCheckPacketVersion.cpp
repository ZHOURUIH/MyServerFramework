#include "GameHeader.h"

// auto generate start
SCCheckPacketVersion SCCheckPacketVersion::mStaticObject;
string SCCheckPacketVersion::mPacketName = STR(SCCheckPacketVersion);
// auto generate end

void SCCheckPacketVersion::send(TCPServerClient* client)
{
	auto& packet = get();
	packet.mResult = true;
	sendPacketTCP(&packet, client);
}