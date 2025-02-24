#include "FrameHeader.h"

void TimeTaskSystem::quit()
{
	for (TimeTask* task : mTimeTaskList)
	{
		task->destroy();
		delete task;
	}
	mTimeTaskList.clear();
}

void TimeTaskSystem::update(const float elapsedTime)
{
	for (TimeTask* task : mTimeTaskList)
	{
		task->update(elapsedTime);
	}
}