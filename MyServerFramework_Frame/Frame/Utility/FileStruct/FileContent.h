#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API FileContent
{
public:
	virtual ~FileContent()
	{
		delete[] mBuffer;
	}
	void createBuffer(int bufferSize);
public:
	char* mBuffer = nullptr;
	int mFileSize = 0;
};