#pragma once

#include "DTDecorate.h"

class MICRO_LEGEND_FRAME_API DTRepeater : public DTDecorate
{
	BASE(DTRepeater, DTDecorate);
public:
	TASK_STATE start() override;
	TASK_STATE update(const float elapsedTime) override;
	void reset() override;
	void resetProperty() override;
	void setParam(int repeatCount) { mMaxRepeat = repeatCount; }
protected:
	TASK_STATE checkState();
protected:
	// 设置的参数
	int mMaxRepeat = 0;
protected:
	// 运行时参数
	int mCurRepeat = 0;
};