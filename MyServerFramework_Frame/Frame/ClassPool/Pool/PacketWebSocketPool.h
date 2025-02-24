#pragma once

#include "ClassTypePoolThread.h"
#include "PacketWebSocketFactoryManager.h"

class MICRO_LEGEND_FRAME_API PacketWebSocketPool : public ClassTypePool<PacketWebSocket, ushort>
{
protected:
	PacketWebSocket* create(const ushort type) override
	{
		auto* factory = mPacketWebSocketFactoryManager->getFactory(type);
		if (factory == nullptr)
		{
			return nullptr;
		}
		return factory->create();
	}
};