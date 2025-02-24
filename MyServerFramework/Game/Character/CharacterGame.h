#pragma once

#include "Character.h"
#include "GameCallback.h"
#include "CharacterGameData.h"
#include "GameEnum.h"

class CharacterGame : public Character
{
	BASE(CharacterGame, Character);
public:
	~CharacterGame() override = default;
	void initComponents() override;
	void resetProperty() override;
protected:
	CharacterGameData* mData = nullptr;			// 角色的数据,在子类构造中赋值
};