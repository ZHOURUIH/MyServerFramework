// auto generate start
#pragma once

#include "EDGlobal.h"
#include "ExcelTable.h"

class ExcelGlobal : public ExcelTable<EDGlobal>
{
	BASE(ExcelGlobal, ExcelTable<EDGlobal>);
public:
	void init(const string& tableName) override;
	void checkAllDataDefault() override;
public:
	static constexpr int TEST_GLOBAL_0 = 10;								// 测试全局参数0
	static constexpr float TEST_GLOBAL_1 = 3.5f;							// 测试全局参数1
	static constexpr llong TEST_GLOBAL_2 = 99999999999;						// 测试全局参数2
	static Vector2Int TEST_GLOBAL_3;										// 测试全局参数3
	static Vector2 TEST_GLOBAL_4;											// 测试全局参数4
	static Vector3 TEST_GLOBAL_5;											// 测试全局参数5
	static Vector3Int TEST_GLOBAL_6;										// 测试全局参数6
	static Vector<int> TEST_GLOBAL_7;										// 测试全局参数7
	static Vector<float> TEST_GLOBAL_8;										// 测试全局参数8
	static Vector<llong> TEST_GLOBAL_9;										// 测试全局参数9
	static string TEST_GLOBAL_10;											// 测试全局参数10
};
// auto generate end