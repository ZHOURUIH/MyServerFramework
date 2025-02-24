#include "FrameHeader.h"

void GameConfigSystem::init()
{
	readConfig(FrameConfigSystem::parseConfig(FrameDefine::CONFIG_PATH + "GameConfig.txt"));
}

void GameConfigSystem::readConfig(const Vector<pair<string, string>>& valueList)
{
	for (const auto& iterValue : valueList)
	{
		const string& paramName = iterValue.first;
		const string& paramValue = iterValue.second;
		if (paramName == "TEST_SERVER")
		{
			mTestServer = SToB(paramValue);
		}
		else if (paramName == "SEASON_SERVER")
		{
			mSeasonServer = SToB(paramValue);
		}
	}
}