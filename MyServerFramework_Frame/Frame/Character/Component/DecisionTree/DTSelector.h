#pragma once

#include "DTControl.h"

// 所有子任务从左到右依次执行，如果有一个任务返回true，后续所有任务就不执行，该任务直接返回true
class MICRO_LEGEND_FRAME_API DTSelector : public DTControl
{
	BASE(DTSelector, DTControl);
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
	// 运行时参数
	int mCurChildIndex = 0;
};