#pragma once

#include "CharacterGameData.h"
#include "GameEnum.h"

class CharacterPlayerData : public CharacterGameData
{
	BASE(CharacterPlayerData, CharacterGameData);
public:
	void resetProperty() override;
public:
	CharacterPlayer* mPlayer = nullptr;			// 所属角色
	TCPServerClient* mClient = nullptr;			// 玩家所在客户端
};