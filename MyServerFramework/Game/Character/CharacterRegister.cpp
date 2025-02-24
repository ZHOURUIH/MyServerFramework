#include "GameHeader.h"

void CharacterRegister::registeAll()
{
	mCharacterFactoryManager->addFactory<CharacterPlayer>(CHARACTER_TYPE::PLAYER);
}