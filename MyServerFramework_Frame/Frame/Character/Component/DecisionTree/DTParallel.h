#pragma once

#include "DTControl.h"

// 会在同时执行所有子任务，如果其中一个子任务返回false，则停掉所有子任务返回false，只有全部返回success时，才会返回success
class MICRO_LEGEND_FRAME_API DTParallel : public DTControl
{
	BASE(DTParallel, DTControl);
public:
	TASK_STATE start() override;
	TASK_STATE update(const float elapsedTime) override;
	TASK_STATE startExecute(DTNode* startChild) override;
protected:
	TASK_STATE checkState();
protected:
};