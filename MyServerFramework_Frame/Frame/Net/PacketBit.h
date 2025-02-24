#pragma once

#include "Packet.h"
#include "SerializerBitRead.h"
#include "SerializerBitWrite.h"

class MICRO_LEGEND_FRAME_API PacketBit : public Packet
{
	BASE(PacketBit, Packet);
public:
	virtual bool readFromBuffer(SerializerBitRead* reader) = 0;
	virtual bool writeToBuffer(SerializerBitWrite* serializer) const = 0;
	void resetProperty() override
	{
		base::resetProperty();
	}
};