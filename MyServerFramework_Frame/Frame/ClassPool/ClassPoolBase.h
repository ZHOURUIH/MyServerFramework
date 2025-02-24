#pragma once

#include "FrameComponent.h"

// 对象池基类
class MICRO_LEGEND_FRAME_API ClassPoolBase : public FrameComponent
{
	BASE(ClassPoolBase, FrameComponent);
public:
	void init() override;
	void quit() override;
	bool isClassPool() const override { return true; }
protected:
	virtual void onHour(){}
	virtual void onSecond(){}
private:
	static void onHourInternal(void* userData);
	static void onSecondInternal(void* userData);
protected:
	const float DEFAULT_LIFE_TIME = 5.0f;					// 临时对象的默认生命时长为5秒
};