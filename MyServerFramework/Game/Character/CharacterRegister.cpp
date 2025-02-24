#include "GameHeader.h"

void CharacterRegister::registeAll()
{
	mCharacterFactoryManager->addFactory<CharacterPlayer>(CHARACTER_TYPE::PLAYER);
	mCharacterFactoryManager->addFactory<CharacterMonster>(CHARACTER_TYPE::MONSTER);
	mCharacterFactoryManager->addFactory<CharacterNPC>(CHARACTER_TYPE::NPC);
	mCharacterFactoryManager->addFactory<CharacterEnvironment>(CHARACTER_TYPE::ENVIRONMENT);
}