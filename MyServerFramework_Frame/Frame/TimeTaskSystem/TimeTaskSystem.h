#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API TimeTaskSystem : public FrameComponent
{
	BASE(TimeTaskSystem, FrameComponent);
public:
	void quit() override;
	void update(const float elapsedTime) override;
	template<typename T>
	void addTimeTask()
	{
		T* task = new T();
		task->init();
		mTimeTaskList.push_back(task);
	}
protected:
	Vector<TimeTask*> mTimeTaskList;
};