#pragma once

#include "ClassPooledObject.h"
#include "Utility.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API GameCommand : public ClassPooledObject
{
	BASE(GameCommand, ClassPooledObject);
public:
	GameCommand()
	{
		mCmdID = makeIDThread();
	}
	~GameCommand() override;
	void resetProperty() override;
	virtual void reset() = 0;
	// 命令执行
	virtual void execute() = 0;
	// 当命令压入命令系统时调用,无论是立即命令还是延迟命令或者是子线程命令
	virtual void onPush() {}
	virtual void onInterrupted() {}
	// 调试信息，由CommandSystem调用
	virtual void debugInfo(MyString<1024>& buffer)		{ }
	bool getShowDebugInfo() const					{ return mShowDebugInfo; }
	bool isDelayCommand() const						{ return mDelayCommand; }
	bool isThreadCommand() const					{ return mThreadCommand; }
	CommandReceiver* getReceiver() const			{ return mReceiver; }
	COMMAND_STATE getCommandState() const			{ return mCommandState; }
	llong getCmdID() const							{ return mCmdID; }
	void setShowDebugInfo(const bool show)			{ mShowDebugInfo = show; }
	void setDelayCommand(const bool delay)			{ mDelayCommand = delay; }
	void setThreadCommand(const bool threadCommand)	{ mThreadCommand = threadCommand; }
	void setReceiver(CommandReceiver* Reciver)		{ mReceiver = Reciver; }
	void setCommandState(const COMMAND_STATE state)	{ mCommandState = state; }
	void setID(const llong id)						{ mCmdID = id; }
	void addEndCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback == nullptr)
		{
			return;
		}
		if (mEndCallback == nullptr)
		{
			mEndCallback = new Vector<pair<CommandCallback, void*>>();
		}
		mEndCallback->emplace_back(cmdCallback, userData);
	}
	void addStartCommandCallback(CommandCallback cmdCallback, void* userData)
	{
		if (cmdCallback == nullptr)
		{
			return;
		}
		if (mStartCallback == nullptr)
		{
			mStartCallback = new Vector<pair<CommandCallback, void*>>();
		}
		mStartCallback->emplace_back(cmdCallback, userData);
	}
	void runEndCallBack();
	void runStartCallBack();
protected:
	template<typename... TypeList>
	void debug(MyString<1024>& buffer, TypeList&&... params)
	{
		strcat_t(buffer, forward<TypeList>(params)...);
	}
protected:
	Vector<pair<CommandCallback, void*>>* mStartCallback = nullptr;	// 命令开始执行时的回调函数
	Vector<pair<CommandCallback, void*>>* mEndCallback = nullptr;	// 命令执行完毕时的回调函数
	CommandReceiver* mReceiver = nullptr;							// 命令接受者
	llong mCmdID = 0;												// 命令唯一ID,在命令被创建时设置
	COMMAND_STATE mCommandState = COMMAND_STATE::NONE;				// 命令状态
	bool mShowDebugInfo = false;									// 是否显示调试信息,默认为false,不显示调试信息
	bool mThreadCommand = false;									// 是否在子线程执行的命令
	bool mDelayCommand = false;										// 是否是延迟执行的命令
};