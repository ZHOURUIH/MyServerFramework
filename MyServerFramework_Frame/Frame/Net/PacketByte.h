#pragma once

#include "Packet.h"
#include "SerializerRead.h"
#include "SerializerWrite.h"

class MICRO_LEGEND_FRAME_API PacketByte : public Packet
{
	BASE(PacketByte, Packet);
public:
	virtual bool readFromBuffer(SerializerRead* reader) = 0;
	virtual bool writeToBuffer(SerializerWrite* serializer) const = 0;
	void resetProperty() override
	{
		base::resetProperty();
	}
};