#include "FrameHeader.h"

CommandReceiver::~CommandReceiver()
{
	// 通知命令系统有一个命令接收者已经被销毁了,需要取消命令缓冲区中的即将发给该接收者的命令
	// 此时需要确认命令系统没有被销毁
	if (mCommandSystem != nullptr && mCommandSystem != this)
	{
		mCommandSystem->notifyReceiverDestroied(this);
	}
}

void CommandReceiver::resetProperty()
{
	base::resetProperty();
#ifdef WINDOWS
	mPrintName.clear();
#endif
	mUTF8Name.clear();
}