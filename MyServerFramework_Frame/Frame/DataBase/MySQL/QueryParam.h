#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API QueryParam
{
public:
	virtual ~QueryParam()
	{
		if (mIDList != nullptr)
		{
			delete mIDList;
			mIDList = nullptr;
		}
	}
public:
	Vector<llong>* mIDList = nullptr;			// 要查询的ID列表,优先判断mCondition,mCondition为空时才会判断mIDList
	string mCondition;							// 查询的条件
	llong mLimitStart = 0;						// 返回结果在查询结果中的起始下标,0表示从查询结果的第一个开始返回
	llong mLimitCount = 0;						// 返回结果的最大数量
	string mOrderColumn;						// 排序的列
	MYSQL_ORDER mOrder = MYSQL_ORDER::NONE;		// 返回结果的排序
	bool mQueryAll = false;						// 是否是查询全部,当没有指定条件,也没有指定ID时,设置了查询全部,则返回表格全部数据,否则视为错误
};