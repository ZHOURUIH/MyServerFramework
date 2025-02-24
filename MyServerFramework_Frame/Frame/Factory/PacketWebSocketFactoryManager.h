#pragma once

#include "PacketWebSocket.h"
#include "FactoryManager.h"

class MICRO_LEGEND_FRAME_API PacketWebSocketFactoryManager : public FactoryManager<PacketWebSocket, ushort>
{
public:
	template<typename ClassType, typename TypeCheck = typename IsSubClassOf<PacketWebSocket, ClassType>::mType>
	void addFactory(const ushort type)
	{
		addFactoryInstance<ClassType>(type);
	}
};