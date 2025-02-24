#pragma once

#include "GameDefine.h"

// 下面包含自动生成的代码,以// FrameSystem,// MySQL,// SQLite为起始,以第一个空行或者};为结束
namespace GameBase
{
	void constructGameDone();
	void clearGameSystem();

	// FrameSystem
	extern GameConfigSystem* mGameConfigSystem;

	// MySQL
	extern MySQLAccount* mMySQLAccount;

	// SQLite
};

using namespace GameBase;