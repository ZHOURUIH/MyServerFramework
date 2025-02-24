#include "GameHeader.h"

CharacterPlayer::CharacterPlayer()
{
	mPlayerData = new CharacterPlayerData();
	mPlayerData->mPlayer = this;
	mData = mPlayerData;
}

CharacterPlayer::~CharacterPlayer()
{
	delete mPlayerData;
	mPlayerData = nullptr;
}

void CharacterPlayer::initComponents()
{
	base::initComponents();
	// 初始化默认组件
}

void CharacterPlayer::resetProperty()
{
	base::resetProperty();
	mPlayerData->resetProperty();
}