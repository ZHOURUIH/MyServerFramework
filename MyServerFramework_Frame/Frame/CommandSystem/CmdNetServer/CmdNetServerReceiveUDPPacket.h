#pragma once

#include "GameCommand.h"

// 通知服务器收到了UDP的消息包
class MICRO_LEGEND_FRAME_API CmdNetServerReceiveUDPPacket : public GameCommand
{
	BASE(CmdNetServerReceiveUDPPacket, GameCommand);
public:
	void reset() override
	{
		memset(&mAddress, 0, sizeof(mAddress));
		mPacketList.clear();
	}
	void execute() override;
public:
	sockaddr_in mAddress;
	HashMap<llong, Vector<PacketTCP*>> mPacketList;		// first是ClientGUID
};