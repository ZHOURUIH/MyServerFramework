#pragma once

#ifdef _LIBEVENT
#include "FrameBase.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API HttpListenInfo
{
public:
	HttpCallback mCallback = nullptr;
	void* mUserData = nullptr;
	string mContentType;
	string mURI;
};

#endif