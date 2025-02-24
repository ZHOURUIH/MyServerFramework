#include "FrameHeader.h"

void CmdNetServerReceiveUDPPacket::execute()
{
	for (auto& item : mPacketList)
	{
		auto& list = item.second;
		const int clientGUID = (int)item.first;
		TCPServerClient* client = mTCPServerSystem->getClient(clientGUID);
		if (client == nullptr)
		{
			continue;
		}
		// 每次收到udp消息以后都更新此客户端的udp地址,防止此地址变化
		client->setUDPAddress(mAddress);
		for (PacketTCP* packetReply : list)
		{
			if (packetReply == nullptr)
			{
				continue;
			}
			if (packetReply->isDestroy())
			{
				ERROR_PROFILE((string("0消息对象已经被销毁:") + typeid(*packetReply).name()).c_str());
				continue;
			}
			packetReply->setClient(client);
			packetReply->execute();
		}
		mPacketTCPPool->destroyClassList(list);
	}
	mPacketList.clear();
}