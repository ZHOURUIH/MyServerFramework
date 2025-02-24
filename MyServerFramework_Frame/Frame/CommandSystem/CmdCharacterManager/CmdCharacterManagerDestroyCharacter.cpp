#include "FrameHeader.h"

void CmdCharacterManagerDestroyCharacter::execute(Character* character)
{
	mEventSystem->removeCharacterEvent(character->getGUID());
	mCharacterManager->destroyCharacter(character);
}