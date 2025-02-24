#pragma once

#include "FrameDefine.h"

//-------------------------------------------------------------------------------------------------------------------------------------------------------
// 枚举定义
// 角色类型
class CHARACTER_TYPE
{
public:
	static constexpr byte NONE = 0;			// 无效值
	static constexpr byte PLAYER = 1;		// 玩家
	static constexpr byte MAX = 5;			// 最大值,用于获取类型数量
};
