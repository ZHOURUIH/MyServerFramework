#include "GameHeader.h"

// auto generate start
SCCharacterFullGameData SCCharacterFullGameData::mStaticObject;
string SCCharacterFullGameData::mPacketName = STR(SCCharacterFullGameData);
// auto generate end

void SCCharacterFullGameData::send(CharacterPlayer* player)
{
	auto& packet = get();
	// 这里设置不发送MaxHP字段,从而节省带宽
	packet.setFieldInvalid(Field::MaxHP);
	packet.mHP = player->getPlayerData()->mHP;
	packet.mMaxHP = player->getPlayerData()->mMaxHP;
	packet.mName = player->getUTF8Name();
	sendPacketTCP(&packet, player->getClient());
}