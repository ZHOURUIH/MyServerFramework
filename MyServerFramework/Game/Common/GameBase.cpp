#include "GameHeader.h"

namespace GameBase
{
	// auto generate start Excel Define
	ExcelAchivement* mExcelAchivement;
	ExcelGlobal* mExcelGlobal;
	ExcelTest* mExcelTest;
	// auto generate end Excel Define

	// auto generate start FrameSystem Define
	GameConfigSystem* mGameConfigSystem;
	// auto generate end FrameSystem Define

	// auto generate start MySQL Define
	MySQLAccount* mMySQLAccount;
	// auto generate end MySQL Define

	// auto generate start SQLite Define
	// auto generate end SQLite Define

	void constructGameDone()
	{
		// auto generate start FrameSystem Get
		mServerFramework->getSystem(STR(GameConfigSystem), mGameConfigSystem);
		// auto generate end FrameSystem Get
	}

	void clearGameSystem()
	{
		// auto generate start FrameSystem Clear
		mGameConfigSystem = nullptr;
		// auto generate end FrameSystem Clear

		// auto generate start MySQL Clear
		mMySQLAccount = nullptr;
		// auto generate end MySQL Clear

		// auto generate start SQLite Clear
		// auto generate end SQLite Clear

		// auto generate start Excel Clear
		mExcelAchivement = nullptr;
		mExcelGlobal = nullptr;
		mExcelTest = nullptr;
		// auto generate end Excel Clear
	}
}