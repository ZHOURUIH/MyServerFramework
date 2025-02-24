#pragma once

#include "ClassTypePoolThread.h"
#include "PacketTCPFactoryManager.h"

class MICRO_LEGEND_FRAME_API PacketTCPPool : public ClassTypePool<PacketTCP, ushort>
{
protected:
	PacketTCP* create(const ushort type) override
	{
		auto* factory = mPacketTCPFactoryManager->getFactory(type);
		if (factory == nullptr)
		{
			return nullptr;
		}
		return factory->create();
	}
};