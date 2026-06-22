#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API QueryParam
{
public:
	virtual ~QueryParam()
	{
		DELETE(mIDList);
		DELETE(mColumns);
	}
public:
	Vector<llong>* mIDList = nullptr;			// 要查询的ID列表,优先判断mCondition,mCondition为空时才会判断mIDList
	Vector<string>* mColumns = nullptr;			// 也支持可以指定只查询某些字段,而不是所有字段
	MyString<128> mConditionStack;				// 查询的条件,如果条件比较短的就用栈字符串存储
	string mConditionHeap;						// 查询的条件,如果条件比较长的就用普通字符串
	llong mLimitStart = 0;						// 返回结果在查询结果中的起始下标,0表示从查询结果的第一个开始返回
	llong mLimitCount = 0;						// 返回结果的最大数量
	string mOrderColumn;						// 排序的列
	MYSQL_ORDER mOrder = MYSQL_ORDER::NONE;		// 返回结果的排序
	bool mQueryAll = false;						// 是否是查询全部,当没有指定条件,也没有指定ID时,设置了查询全部,则返回表格全部数据,否则视为错误
};