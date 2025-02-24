#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API SerializableData
{
public:
	virtual bool readFromBuffer(SerializerRead* reader) = 0;
	virtual bool writeToBuffer(SerializerWrite* serializer) const = 0;
	virtual void resetProperty() {}
};