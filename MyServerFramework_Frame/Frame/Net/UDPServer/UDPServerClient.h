#pragma once

#include "FrameBase.h"
#include "ThreadLock.h"
#include "HashMap.h"

// 当前程序是UDP服务器时,处理所有客户端的逻辑
class MICRO_LEGEND_FRAME_API UDPServerClient
{
public:
	UDPServerClient();
	virtual ~UDPServerClient();
	void sendPacket(PacketTCP* packet, llong token, const sockaddr_in& addr);
	static void recvData(char* data, int dataCount, const sockaddr_in& addr);
	void processSend(MY_SOCKET socket);
	void removeClientToken(llong token);
protected:
	static PARSE_RESULT packetRead(char* buffer, int dataLength, int& bitIndex, PacketTCP*& packet, llong& token);
protected:
	HashMap<llong, UDPClientInfo*> mSendList;	// 所有地址的待发送消息列表
	SerializerBitWrite* mSendWriter = nullptr;	// 只在发送消息时用将要发送的数据写入到发送缓冲区,避免频繁构造析构
	char* mEncryptBuffer = nullptr;				// 用于临时存放加密后数据的缓冲区
	ThreadLock mSendListLock;					// mSendList的线程锁
};