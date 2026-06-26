// auto generate start
#pragma once

#include "ExcelData.h"
#include "GameEnum.h"

// 测试例子的表格
class EDTest : public ExcelData
{
	BASE(EDTest, ExcelData);
public:
	static constexpr int TEST_0_ID = 1;							// 测试字段
	static constexpr int TEST_1_ID = 2;							// 测试字段

	static EDTest* TEST_0;										// 测试字段
	static EDTest* TEST_1;										// 测试字段

	string mTestString;											// 测试字符串
	int mTestInt = 0;											// 测试整数
	llong mTestLong = 0;										// 测试长整数
	Vector2Int mTestVector2Int;									// 测试整数二维向量
	Vector2 mTestVector2;										// 测试二维向量
	TEST_ENUM mTestEnum = (TEST_ENUM)0;							// 测试枚举
	Vector<int> mTestList0;										// 测试整数列表
	Vector<int> mTestList1;										// 测试整数列表
	float mTestFloat = 0.0f;									// 测试浮点数
	Vector<string> mTestStringList;								// 测试字符串列表
	int mTestLinkTable0 = 0;									// 测试索引到其他表格
	Vector<int> mTestLinkTable1;								// 测试索引到其他表格
	string mTestPath;											// 测试文件路径,GameResources下的相对路径
public:
	void cloneTo(ExcelData* target) override;
	void read(SerializerRead* reader) override;
	static void postLoadAll(ExcelTableBase* tableBase);
};

class EDTest_TEST_0
{
public:
	static constexpr int mTestParam0 = 1;						// 用于生成测试的ID参数
	static constexpr float mTestParam1 = 1.5f;					// 用于生成测试的ID参数
};
// auto generate end