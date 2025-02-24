#include "GameHeader.h"

namespace GameUtility
{
	CharacterPlayer* getPlayer(const llong playerGUID)
	{
		return static_cast<CharacterPlayer*>(mCharacterManager->getCharacter(playerGUID));
	}
	CharacterPlayer* toPlayer(Character* character)
	{
		return static_cast<CharacterPlayer*>(character);
	}
}