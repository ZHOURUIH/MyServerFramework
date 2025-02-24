#include "GameHeader.h"

// 下面包含自动生成的代码,以// FrameSystem,// MySQL,// SQLite为起始,以第一个空行或者}为结束
namespace GameBase
{
	// FrameSystem Define
	GameConfigSystem* mGameConfigSystem;

	// MySQL Define
	MySQLAccount* mMySQLAccount;

	// SQLite Define

	void constructGameDone()
	{
		// FrameSystem Get
		mServerFramework->getSystem(STR(GameConfigSystem), mGameConfigSystem);
	}

	void clearGameSystem()
	{
		// FrameSystem Clear
		mGameConfigSystem = nullptr;

		// MySQL Clear
		mMySQLAccount = nullptr;

		// SQLite Clear
	}
}