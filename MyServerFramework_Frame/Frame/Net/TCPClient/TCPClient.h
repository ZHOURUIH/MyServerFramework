#pragma once

#include "ThreadLock.h"
#include "DoubleBuffer.h"

// 当前程序是一个TCP客户端
class MICRO_LEGEND_FRAME_API TCPClient
{
	THIS(TCPClient);
public:
	TCPClient();
	virtual ~TCPClient();
	void disconnect();
	void init(const string& ip, const ushort port);
	virtual void update(const float elapsedTime);
	MY_SOCKET getSocket() const { return mSocket; }
	bool isAvailable() const { return mSocket != INVALID_SOCKET; }
	void sendPacket(PacketTCP* packet);
protected:
	PARSE_RESULT packetRead(int& bitIndex, PacketTCP*& packet, MyString<256>& reason);
	virtual void heartBeat() {}
	static void receiveSendSocket(CustomThread* thread);
	bool processSend();
	bool processRecv();
	void checkSendRecvError(const int successLength) const;
protected:
	CustomThread* mSendReceiveThread = nullptr;		// 发送接收线程
	DoubleBuffer<PacketTCP*> mExecutePacketList;		// 客户端收到消息的列表
	StreamBuffer* mSendBuffer = nullptr;			// 客户端发送数据缓冲区
	StreamBuffer* mRecvBuffer = nullptr;			// 客户端接收数据缓冲区
	SerializerBitWrite* mSendWriter = nullptr;		// 只在发送消息时用将要发送的数据写入到发送缓冲区,避免频繁构造析构
	SerializerBitWrite* mPacketDataBuffer = nullptr;// 用于在发送消息时消息序列化,序列化以后再加密然后写入到发送缓冲区,之前是存在Packet中,每个Packet都有一个,但是那样很占内存
	ThreadLock mSendLock;							// 发送的线程锁
	MY_SOCKET mSocket = INVALID_SOCKET;				// 客户端套接字
	float mCurServerHeartBeatTime = 0.0f;			// 当前服务器心跳计时
	float mHeartBeatTimeOut = 10.0f;				// 心跳超时时间
	string mServerIP;								// 服务器IP
	ushort mServerPort = 0;							// 服务器端口号
	string mServerName;								// 服务器名字
	int mSequenceNumber = 0;						// 当前发送包的序列号
	int mParsedCount = 0;							// 已成功解析的消息数量
	int mLastReceiveSequenceNumber = 0;				// 上一次接收到的消息序列号
private:
	Vector<PacketTCP*> mTempPacketList;
};