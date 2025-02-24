#pragma once

#include "ServerFramework.h"

class Game : public ServerFramework
{
	BASE(Game, ServerFramework);
public:
	void launch() override;
protected:
	void registeComponent() override;
	void constructDone() override;
	void registe() override;
	void clearSystem() override;
protected:
};