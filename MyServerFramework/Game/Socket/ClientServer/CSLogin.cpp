#include "GameHeader.h"

// auto generate start
string CSLogin::mPacketName = STR(CSLogin);
// auto generate end

void CSLogin::execute()
{
	UNIFIED_UTF8_STRING(name, "测试玩家");
	CharacterPlayer* player = toPlayer(mCharacterManager->createCharacter(name, CHARACTER_TYPE::PLAYER, 1, true));
	if (player == nullptr)
	{
		return;
	}
	// 玩家登陆后,设置玩家属性
	player->getPlayerData()->mClient = mClient;
	player->getPlayerData()->mMaxHP = 10;
	player->getPlayerData()->mHP = 10;

	// 通知网络客户端角色登录
	mClient->setPlayerGUID(player->getGUID());
	SCCharacterFullGameData::send(player);

	MyString<256> info;
	LLONG_STR(idStr, player->getGUID());
	strcat_t(info, "玩家登录：", player->getPrintName().c_str(), ", ID:", idStr.str());
	LOG(info.str());
}