#pragma once

#include "DTControl.h"

// 从所有子任务中随机选择一个执行
class MICRO_LEGEND_FRAME_API DTRandom : public DTControl
{
	BASE(DTRandom, DTControl);
public:
	TASK_STATE start() override;
	TASK_STATE update(const float elapsedTime) override;
	TASK_STATE startExecute(DTNode* startChild) override;
	void notifyChildStart(DTNode* child) override;
	void clearChild() override;
protected:
	TASK_STATE checkState();
protected:
	int mCurChildIndex = 0;
};