#pragma once

#include "FrameComponent.h"

// 当前程序是一个UDP服务器
class MICRO_LEGEND_FRAME_API UDPServerSystem : public FrameComponent
{
	BASE(UDPServerSystem, FrameComponent);
public:
	UDPServerSystem();
	void init() override;
	void quit() override;
	bool isAvailable() const			{ return mSocket != INVALID_SOCKET; }
	UDPServerClient* getClient() const	{ return mClient; }
	ushort getPort() const				{ return mPort; }
	const SerializerBitWrite* getPacketDataBuffer() const { return mPacketDataBuffer; }
	// 将消息数据写入到缓冲区,在发送消息前调用
	void writePacket(PacketTCP* packet);
protected:
	static void sendThread(CustomThread* thread);
	static void receiveThread(CustomThread* thread);
protected:
	UDPServerClient* mClient = nullptr;				// 客户端,用于解析数据
	CustomThread* mSendThread = nullptr;			// 发送线程
	CustomThread* mReceiveThread = nullptr;			// 接收线程
	SerializerBitWrite* mPacketDataBuffer;			// 用于在发送消息时消息序列化,序列化以后再加密然后写入到发送缓冲区,之前是存在Packet中,每个Packet都有一个,但是那样很占内存
	MY_SOCKET mSocket = INVALID_SOCKET;				// 服务器套接字
	char* mTempRecvBuffer = nullptr;				// 数据接收缓冲区
	ushort mPort = 0;								// UDP服务器端口
};