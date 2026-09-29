#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API ThreadManager : public FrameComponent
{
	BASE(ThreadManager, FrameComponent);
public:
	~ThreadManager() override;
	CustomThread* createThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback = nullptr, VoidCallback endCmdCallback = nullptr);
	CustomThread* createEventThread(const string& name, VoidCallback callback, VoidCallback preCmdCallback = nullptr, VoidCallback endCmdCallback = nullptr);
	CustomThread* getThread(const MY_THREAD handle) const { return mThreadList.get(handle); }
	void destroyThread(CustomThread*& thread);
protected:
	HashMap<MY_THREAD, CustomThread*> mThreadList;
};