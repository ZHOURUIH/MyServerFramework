#include "GameHeader.h"

// auto generate start
string CSAttack::mPacketName = STR(CSAttack);
// auto generate end

void CSAttack::execute()
{
	CharacterPlayer* player = getPlayer(mClient->getPlayerGUID());
	if (player == nullptr)
	{
		LOG("玩家未登录");
		return;
	}
	LOG("玩家进行了攻击");
	SCAttack::send(player);
}