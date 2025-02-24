#include "FrameHeader.h"

void ExcelData::read(SerializerRead* reader)
{
	reader->read(mID);
}