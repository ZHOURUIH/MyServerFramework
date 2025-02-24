#pragma once

#include "ClassPooledObject.h"

class MICRO_LEGEND_FRAME_API DelayCommand : public ClassPooledObject
{
	BASE(DelayCommand, ClassPooledObject);
public:
	void init(CommandReceiver* receiver, CustomThread* thread, GameCommand* cmd, const float delayTime)
	{
		mReceiver = receiver;
		mThread = thread;
		mCommand = cmd;
		mDelayTime = delayTime;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mReceiver = nullptr;
		mThread = nullptr;
		mCommand = nullptr;
		mDelayTime = 0.0f;
	}
public:
	CommandReceiver* mReceiver = nullptr;	// 命令接收者
	CustomThread* mThread = nullptr;		// 执行该命令的线程,为nullptr表示在主线程执行
	GameCommand* mCommand = nullptr;		// 命令对象
	float mDelayTime = 0.0f;				// 延迟时间
};