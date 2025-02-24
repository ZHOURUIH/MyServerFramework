#include "GameHeader.h"

// auto generate start
SCServerCheckPing SCServerCheckPing::mStaticObject;
string SCServerCheckPing::mPacketName = STR(SCServerCheckPing);
// auto generate end

void SCServerCheckPing::send(TCPServerClient* client, const int index)
{
	auto& packet = get();
	packet.mIndex = index;
	sendPacketTCP(&packet, client);
}