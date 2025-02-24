#include "GameHeader.h"

void CharacterGame::initComponents()
{
	base::initComponents();
}

void CharacterGame::resetProperty()
{
	base::resetProperty();
	// mData在子类构造中赋值,不重置
	//mData = nullptr;
}