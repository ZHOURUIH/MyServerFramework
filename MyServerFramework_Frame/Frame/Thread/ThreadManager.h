#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API ThreadManager : public FrameComponent
{
	BASE(ThreadManager, FrameComponent);
public:
	~ThreadManager() override;
	CustomThread* createThread(const string& name, CustomThreadCallback callback, void* args, CustomThreadCallback preCmdCallback = nullptr, CustomThreadCallback endCmdCallback = nullptr);
	CustomThread* getThread(const MY_THREAD handle) const { return mThreadList.tryGet(handle); }
	void destroyThread(CustomThread*& thread);
protected:
	HashMap<MY_THREAD, CustomThread*> mThreadList;
};