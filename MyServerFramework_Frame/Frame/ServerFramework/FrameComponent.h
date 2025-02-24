#pragma once

#include "CommandReceiver.h"

// 框架组件,一般是各类管理器,工厂,对象池,如果是管理器则名字应该以System或者Manager结尾,方便代码查找
class MICRO_LEGEND_FRAME_API FrameComponent : public CommandReceiver
{
	BASE(FrameComponent, CommandReceiver);
public:
	// 初始化
	virtual void init() {}
	// 延迟初始化,因为在init阶段有很多东西还无法使用,从而影响初始化操作
	// 所以提供一个等待所有东西都可用,该注册的都已经注册完毕时的初始化函数
	virtual void lateInit() {}
	// 初始化阶段还是可能分顺序,所以即使是lateInit,仍然可能存在顺序冲突,所以增加一个最后地初始化
	virtual void finalInit() {}
	// 程序退出前调用,表示即将销毁所有组件
	virtual void quit() {}
	// 程序全部初始化完成后,启动时调用
	virtual void launch() {}
	// 每一帧调用
	virtual void update(const float elapsedTime) {}
	// 所有系统的update调用完以后再调用lateUpdate
	virtual void lateUpdate(const float elapsedTime) {}
	virtual bool isClassPool() const 		{ return false; }
	virtual bool isFactory() const			{ return false; }
	void setDestroied(bool destroied)		{ mDestroied = destroied; }
	void resetProperty() override 
	{
		base::resetProperty();
		mDestroied = false;
	}
protected:
	bool mDestroied = false;
};