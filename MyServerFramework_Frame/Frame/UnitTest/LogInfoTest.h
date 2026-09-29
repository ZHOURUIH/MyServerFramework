#pragma once

#include "FrameMacro.h"

// LogInfo 日志信息单元测试
// 覆盖: 默认状态/resetProperty重置/字段独立性
class MICRO_LEGEND_FRAME_API LogInfoTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testDefaultState();
	static void testResetPropertyClearsAllFields();
	static void testResetPropertyAfterModification();
#endif
};