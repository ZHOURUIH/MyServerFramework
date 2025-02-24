#include "FrameHeader.h"

GameCommand::~GameCommand()
{
	if (mStartCallback != nullptr)
	{
		delete mStartCallback;
		mStartCallback = nullptr;
	}
	if (mEndCallback != nullptr)
	{
		delete mEndCallback;
		mEndCallback = nullptr;
	}
}

void GameCommand::resetProperty()
{
	base::resetProperty();
	if (mStartCallback != nullptr)
	{
		mStartCallback->clear();
	}
	if (mEndCallback != nullptr)
	{
		mEndCallback->clear();
	}
	mReceiver = nullptr;
	mCommandState = COMMAND_STATE::NONE;
	mShowDebugInfo = false;
	mThreadCommand = false;
	mDelayCommand = false;
	// mCmdID不重置
	// mCmdID = 0;
	reset();
}

void GameCommand::runEndCallBack()
{
	if (mEndCallback == nullptr)
	{
		return;
	}
	for (const auto& value : *mEndCallback)
	{
		CALL(value.first, this, value.second);
	}
	mEndCallback->clear();
}

void GameCommand::runStartCallBack()
{
	if (mStartCallback == nullptr)
	{
		return;
	}
	for (const auto& value : *mStartCallback)
	{
		CALL(value.first, this, value.second);
	}
	mStartCallback->clear();
}