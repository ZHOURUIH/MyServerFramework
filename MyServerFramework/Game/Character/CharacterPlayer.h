#pragma once

#include "CharacterGame.h"
#include "CharacterPlayerData.h"

class CharacterPlayer : public CharacterGame
{
	BASE(CharacterPlayer, CharacterGame);
public:
	CharacterPlayer();
	~CharacterPlayer() override;
	void initComponents() override;
	void resetProperty() override;
	CharacterPlayerData* getPlayerData() const				{ return mPlayerData; }
	TCPServerClient* getClient() const						{ return mPlayerData->mClient; }
protected:
	CharacterPlayerData* mPlayerData = nullptr;				// 角色所有数据
};