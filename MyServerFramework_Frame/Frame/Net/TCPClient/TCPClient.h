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
	void init(const string& ip, ushort port);
	virtual void update(float elapsedTime);
	MY_SOCKET getSocket() const { return mSocket; }
	bool isAvailable() const { return mSocket != INVALID_SOCKET; }
	bool hasDataToSend() const { return mSendBuffer->getDataLength() > 0 || mWillSendBuffer1->getDataLength() > 0; }
	// 只在Send线程调用
	void writeToSendBuffer();
	void sendPacket(PacketTCP* packet);
	void sendThread();
	void recvThread();
protected:
	PARSE_RESULT packetRead(int& bitIndex, PacketTCP*& packet, MyString<256>& reason);
	virtual void heartBeat() {}
	bool processSend(bool needSelect);
	bool processRecv(bool needSelect);
	void checkSendRecvError(int successLength);
protected:
#ifndef STRESS_TEST
	CustomThread* mSendThread = nullptr;			// 发送线程
	CustomThread* mReceiveThread = nullptr;			// 接收线程
#endif
	ArrayList<512, PacketTCP*> mWillDestroyPackets;	// 用于缓存
	DoubleBuffer<PacketTCP*> mExecutePacketList;	// 客户端收到消息的列表
	StreamBuffer* mSendBuffer = nullptr;			// 客户端发送数据缓冲区,只在Send线程访问
	StreamBuffer* mWillSendBuffer0 = nullptr;		// 缓存一帧的客户端发送数据缓冲区,只在主线程访问
	StreamBuffer* mWillSendBuffer1 = nullptr;		// Send线程中每帧写入到mSendBuffer,在主线程和Send线程访问
	StreamBuffer* mRecvBuffer = nullptr;			// 客户端接收数据缓冲区
	SerializerBitWrite* mSendWriter = nullptr;		// 只在发送消息时用将要发送的数据写入到发送缓冲区,避免频繁构造析构
	SerializerBitWrite* mPacketDataBuffer = nullptr;// 用于在发送消息时消息序列化,序列化以后再加密然后写入到发送缓冲区,之前是存在Packet中,每个Packet都有一个,但是那样很占内存
	ThreadLock mWillSendBuffer1Lock;				// mWillSendBuffer1的线程锁
	MY_SOCKET mSocket = INVALID_SOCKET;				// 客户端套接字
	float mCurServerHeartBeatTime = 0.0f;			// 当前服务器心跳计时
	float mHeartBeatTimeOut = 10.0f;				// 心跳超时时间
	string mServerIP;								// 服务器IP
	ushort mServerPort = 0;							// 服务器端口号
	string mServerName;								// 服务器名字
	uint mSequenceNumber = 0;						// 当前发送包的序列号
	int mParsedCount = 0;							// 已成功解析的消息数量
	uint mLastReceiveSequenceNumber = 0;			// 上一次接收到的消息序列号
	bool mWinsockInitialized = false;				// 是否已经初始化了winsock,防止重复调用WSACleanup
private:
	Vector<PacketTCP*> mTempPacketList;
};