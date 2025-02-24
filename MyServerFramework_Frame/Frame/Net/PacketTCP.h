#pragma once

#include "PacketBit.h"

class MICRO_LEGEND_FRAME_API PacketTCP : public PacketBit
{
	BASE(PacketTCP, PacketBit);
public:
	TCPServerClient* getClient() const				{ return mClient; }
	void setClient(TCPServerClient* client)			{ mClient = client; }
	void resetProperty() override;
protected:
	TCPServerClient* mClient = nullptr;					// 消息所属客户端
};