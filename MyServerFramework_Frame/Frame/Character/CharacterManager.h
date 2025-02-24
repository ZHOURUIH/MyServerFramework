#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API CharacterManager : public FrameComponent
{
	BASE(CharacterManager, FrameComponent);
public:
	void update(float elapsedTime) override;
	void quit() override;
	// managed表示是否由CharacterManager来管理创建出来的角色,false表示只创建角色,不负责更新,但是仍然会存储到列表中
	Character* createCharacter(const string& name, byte type, llong guid, bool managed);
	void destroyCharacter(Character* character);
	const HashMap<llong, Character*>& getCharacterList(const byte type) const { return mCharacterTypeList.tryGet(type); }
	Character* getCharacter(const llong characterID) const { return mCharacterIDList.tryGet(characterID); }
	bool isCharacterLogin(const llong guid) const { return mCharacterIDList.contains(guid); }
	void resetProperty() override;
protected:
	HashMap<llong, Character*> mCharacterIDList;					// 角色ID索引表
	HashMap<llong, Character*> mCharacterUpdateList;				// 用于更新角色的表格
	HashMap<byte, HashMap<llong, Character*>> mCharacterTypeList;	// 角色分类列表
};