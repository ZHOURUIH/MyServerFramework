#include "GameHeader.h"

// auto generate start
SCGetItemTip SCGetItemTip::mStaticObject;
string SCGetItemTip::mPacketName = STR(SCGetItemTip);
// auto generate end
void SCGetItemTip::send(CharacterPlayer* player)
{
	auto& packet = get();
	sendPacketTCP(&packet, player->getClient());
}