#pragma once

#include "GameBase.h"
#include "Utility.h"

namespace GameUtility
{
	CharacterPlayer* getPlayer(llong playerGUID);
	CharacterPlayer* toPlayer(Character* character);
};

using namespace GameUtility;