#pragma once

#include "PacketTCP.h"
#include "FactoryManager.h"

class MICRO_LEGEND_FRAME_API PacketTCPFactoryManager : public FactoryManager<PacketTCP, ushort>
{
public:
	bool isUDPPacket(const ushort type) const { return mUDPTypeList.contains(type); }
	template<typename ClassType, typename TypeCheck = typename IsSubClassOf<PacketTCP, ClassType>::mType>
	void addFactory(const ushort type)
	{
		addFactoryInstance<ClassType>(type);
	}
	void addUDPPacket(const ushort type)
	{
		mUDPTypeList.insert(type);
	}
protected:
	Set<ushort> mUDPTypeList;
};