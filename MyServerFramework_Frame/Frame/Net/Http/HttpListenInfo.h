#pragma once

#ifdef _LIBEVENT
#include "FrameBase.h"
#include "FrameCallback.h"

class HttpListenInfo
{
public:
	HttpCallback mCallback = nullptr;
	void* mUserData = nullptr;
	string mContentType;
	string mURI;
};

#endif