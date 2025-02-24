#pragma once

#include "GameBase.h"
#include "Utility.h"

namespace GameUtility
{
	static CharacterPlayer* getPlayer(llong playerGUID);
	static CharacterPlayer* toPlayer(Character* character);
};

using namespace GameUtility;