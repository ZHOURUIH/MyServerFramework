#pragma once

#include "DTControl.h"

// 所有子任务按照从左到右的顺序依次执行，如果有一个子任务返回false，后续子任务都不执行，并且该任务直接返回false
class MICRO_LEGEND_FRAME_API DTSequence : public DTControl
{
	BASE(DTSequence, DTControl);
public:
	TASK_STATE start() override;
	TASK_STATE update(const float elapsedTime) override;
	TASK_STATE startExecute(DTNode* startChild) override;
	void notifyChildStart(DTNode* child) override;
	void clearChild() override;
	void reset() override;
	void resetProperty() override;
protected:
	TASK_STATE checkState();
protected:
	int mCurChildIndex = 0;
};