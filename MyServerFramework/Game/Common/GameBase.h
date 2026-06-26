#pragma once

#include "GameDefine.h"

namespace GameBase
{
	void constructGameDone();
	void clearGameSystem();

	// auto generate start FrameSystem Extern
	extern GameConfigSystem* mGameConfigSystem;
	// auto generate end FrameSystem Extern

	// auto generate start MySQL Extern
	extern MySQLAccount* mMySQLAccount;
	// auto generate end MySQL Extern

	// auto generate start SQLite Extern
	// auto generate end SQLite Extern

	// auto generate start Excel Extern
	extern ExcelAchivement* mExcelAchivement;
	extern ExcelGlobal* mExcelGlobal;
	extern ExcelTest* mExcelTest;
	// auto generate end Excel Extern
};

using namespace GameBase;