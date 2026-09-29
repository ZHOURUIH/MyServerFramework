#pragma once

#include "FrameMacro.h"

// FactoryManager模板工厂单元测试
// 覆盖: 注册/创建/查询/计数/清理/SFINAE类型约束/重复注册/不存在key
class MICRO_LEGEND_FRAME_API FactoryManagerTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testCreateSingleFactory();
	static void testCreateMultipleFactories();
	static void testGetFactoryNonExistent();
	static void testGetFactoryCount();
	static void testQuitClearsFactories();
	static void testCreateReturnsCorrectType();
	static void testCreateEachFactoryDistinct();
	static void testOverwriteSameKey();
	static void testZeroKey();
	static void testIsFactoryFlag();
#endif
};