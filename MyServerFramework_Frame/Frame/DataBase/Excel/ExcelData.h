#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API ExcelData
{
public:
	virtual void read(SerializerRead* reader);
	virtual ~ExcelData() = default;
public:
	int mID = 0;					// 唯一ID,由于列名是定义在基类中,所以为了对应关系,将ID变量也定义在基类中
};