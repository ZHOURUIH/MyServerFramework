#pragma once

#include "ThreadLock.h"
#include "CommandPool.h"
#include "FrameComponent.h"
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API CommandSystem : public FrameComponent
{
	BASE(CommandSystem, FrameComponent);
public:
	void quit() override;
	void update(const float elapsedTime) override;
	static DelayCommand* createDelayCommand(CommandReceiver* receiver, CustomThread* thread, GameCommand* cmd, float delayTime);
	// 设置为静态函数,这样即使命令系统销毁了,仍然可以使用该函数销毁命令对象
	static void destroyCmd(GameCommand* cmd);
	// 中断延时命令,在命令还未执行时,将命令销毁
	bool interruptCommand(const llong assignID);
	// 在任意线程中发送立即执行的命令,则该命令将在该线程中执行
	void pushCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const;
	// 仅限于执行已经被压入的命令,新创建的命令则需要使用pushCommand或pushCommandDelay等以push开头的函数进行发送
	void executeCommand(GameCommand* cmd, CommandReceiver* cmdReceiver) const;
	// 延迟执行的命令都会在主线程中执行
	// delayExecute是命令延时执行的时间,默认为0.001
	void pushCommandDelay(GameCommand* cmd, CommandReceiver* cmdReceiver, float delayExecute = 0.001f);
	void pushCommandThread(GameCommand* cmd, CommandReceiver* cmdReceiver, CustomThread* thread);
	void pushCommandThreadDelay(GameCommand* cmd, CommandReceiver* cmdReceiver, CustomThread* thread, float delayExecute);
	virtual void notifyReceiverDestroied(CommandReceiver* receiver);
protected:
	static void destroyDelayCommand(DelayCommand* delayCommand);
	bool checkExecuteCmd(const GameCommand* cmd, CommandReceiver* cmdReceiver) const;
	bool checkCmd(GameCommand* cmd, CommandReceiver* cmdReceiver) const;
	bool checkDelayCmd(GameCommand* cmd) const;
protected:
	HashMap<CustomThread*, Vector<DelayCommand*>> mThreadCommandList;	// 要发送到子线程的命令
	Vector<DelayCommand*> mDelayCommandList;		// 延迟命令列表
	Vector<DelayCommand*> mExecuteList;				// 即将在这一帧执行的命令,该列表只在主线程进行写入,其他线程都是读取,所以暂不添加锁
	ThreadLock mThreadCommandListLock;
	ThreadLock mDelayCommandListLock;
};