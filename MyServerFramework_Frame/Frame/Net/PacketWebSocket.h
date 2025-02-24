#pragma once

#include "PacketByte.h"

class MICRO_LEGEND_FRAME_API PacketWebSocket : public PacketByte
{
	BASE(PacketWebSocket, PacketByte);
public:
	WebSocketServerClient* getClient() const		{ return mClient; }
	void setClient(WebSocketServerClient* client)	{ mClient = client; }
	void resetProperty() override;
protected:
	WebSocketServerClient* mClient = nullptr;			// 消息所属WebSocket客户端
};