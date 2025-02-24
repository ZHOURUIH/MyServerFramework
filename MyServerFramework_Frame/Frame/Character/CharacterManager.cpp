#include "FrameHeader.h"

void CharacterManager::update(const float elapsedTime)
{
	for (const auto& iter : mCharacterUpdateList)
	{
		Character* character = iter.second;
		if (character != nullptr)
		{
			character->update(elapsedTime);
		}
	}
}

void CharacterManager::quit()
{
	for (const auto& iter : mCharacterIDList)
	{
		iter.second->destroy();
	}
	mCharacterPool->destroyClassList(mCharacterIDList);
	mCharacterIDList.clear();
	mCharacterUpdateList.clear();
	mCharacterTypeList.clear();
}

Character* CharacterManager::createCharacter(const string& name, const byte type, const llong guid, const bool managed)
{
	// 创建一个新角色
	Character* newChar = mCharacterPool->newClass(type);
	newChar->setGUID(guid);
	newChar->setName(name);
	newChar->init();
	// 加入ID索引表,仅仅在插入时检查是否有重复,因为大部分时候肯定能插入成功,如果每次都先检查是否存在,会浪费不少的性能
	// 所以即使看起来像是先创建后加入失败再销毁,显得有点冗余,但是性能是最好的
	if (!mCharacterIDList.insert(guid, newChar))
	{
		ERROR("there is a character id : " + LLToS(guid));
		newChar->destroy();
		mCharacterPool->destroyClass(newChar);
		return nullptr;
	}
	// 如果需要管理器负责更新,则加入到更新列表中
	if (managed)
	{
		mCharacterUpdateList.insert(guid, newChar);
	}
	// 加入到角色分类列表
	mCharacterTypeList.insertOrGet(type).insert(guid, newChar);
	return newChar;
}

void CharacterManager::destroyCharacter(Character* character)
{
	if (character == nullptr)
	{
		return;
	}
	if (character->isDestroy())
	{
		ERROR_PROFILE("0销毁了已经被销毁的角色");
		return;
	}
	// 从ID索引表中移除
	mCharacterIDList.erase(character->getGUID());
	// 从更新列表中移除
	mCharacterUpdateList.erase(character->getGUID());
	// 从角色分类列表中移除
	mCharacterTypeList.insertOrGet(character->getType()).erase(character->getGUID());
	character->destroy();
	mCharacterPool->destroyClass(character);
}

void CharacterManager::resetProperty()
{
	base::resetProperty();
	mCharacterIDList.clear();
	mCharacterUpdateList.clear();
	mCharacterTypeList.clear();
}