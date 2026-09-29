#pragma once

#include "FrameMacro.h"

// LogFileInfo 日志文件信息单元测试
// 覆盖: 文件名生成格式/GUID路径分离/默认状态/Error与Log区分
class MICRO_LEGEND_FRAME_API LogFileInfoTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testDefaultState();
	static void testGenerateLogFileNameFormat();
	static void testGenerateErrorFileNameFormat();
	static void testDifferentGUIDsProduceDifferentPaths();
	static void testGUIDPathBucketing();       // GUID/100 分桶
	static void testLogVsErrorFileNameDiffers();
#endif
};