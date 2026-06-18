// auto generate start
#pragma once

#include "GameEnum.h"

// auto generated file
class GameEnumCheck
{
public:
	static constexpr bool checkEnum(const TEST_ENUM value)
	{
		switch (value)
		{
		case TEST_ENUM::NONE:break;
		case TEST_ENUM::VALUE0:break;
		case TEST_ENUM::VALUE1:break;
		default:return false;
		}
		return true;
	}

	static bool checkEnum(const Vector<TEST_ENUM>& valueList)
	{
		for (const TEST_ENUM value : valueList)
		{
			if (!checkEnum(value))
			{
				return false;
			}
		}
		return true;
	}
};
// auto generate end