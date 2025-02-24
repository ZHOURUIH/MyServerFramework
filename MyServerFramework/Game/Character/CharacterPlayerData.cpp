#include "GameHeader.h"

void CharacterPlayerData::resetProperty()
{
	base::resetProperty();
	// 构造时赋值,不重置
	//mPlayer = nullptr;
	mClient = nullptr;
}