#pragma once

#include "ThreadLock.h"
#include "StreamBuffer.h"
#include "DoubleBuffer.h"
#include "SerializerBitRead.h"
#include <deque>
#include <vector>

// 当前程序是服务器时,表示连接到服务器的一个客户端
class MICRO_LEGEND_FRAME_API TCPServerClient
{
public:
	TCPServerClient(int clientGUID, MY_SOCKET s, const string& ip);
	virtual ~TCPServerClient();
	virtual void init();
	void update(float elapsedTime);
	// 主线程在endFrame调用,按Buffer2剩余空间尽可能提交待发送数据给Send线程
	bool submitSendData();
	void sendPacket(PacketTCP* packet);
	void sendPacket(ushort packetType, const string& name);
	// 此函数在sendThread调用
	void writeToSendBuffer();
	void recvData(const char* data, int dataCount);
	void setPlayerGUID(const llong guid)		{ mPlayerGUID = guid; }
	void setAccountGUID(const llong guid)		{ mAccountGUID = guid; }
	void setUDPAddress(const sockaddr_in& addr)	{ mUDPAddress = addr; }
	void setDeadClient(string&& reason, DEAD_TYPE type)
	{
		mDeadReason = move(reason);
		mIsDeadClient = true;
		mDeadType = type;
	}
	// 获得成员变量
	int getClientGUID() const					{ return mClientGUID; }
	llong getAccountGUID() const				{ return mAccountGUID; }
	llong getPlayerGUID() const					{ return mPlayerGUID; }
	MY_SOCKET getSocket() const					{ return mSocket; }
	bool isDeadClient() const					{ return mIsDeadClient; }
	DEAD_TYPE getDeadType() const				{ return mDeadType; }
	const string& getDeadReason() const			{ return mDeadReason;}
	StreamBuffer* getSendBuffer() const			{ return mSendBuffer; }
	int getRecvDataCount()	const				{ return mRecvBuffer->getDataLength(); }
	bool getIsLogin() const						{ return mAccountGUID != 0; }
	const string& getIP() const					{ return mIP; }
	const sockaddr_in& getUDPAddress() const	{ return mUDPAddress; }
	bool isClientHighLag() const				{ return mServerPingWaitTime >= mHighLagTime; }
	void notifyPing();
	void notifyServerPing(int index);
protected:
	PARSE_RESULT packetRead(int& bitIndex, PacketTCP*& packet, string& reason);
	void debugInfo(const string& info) const;
	void debugError(const string& info) const;
	void executeAllPacket();
	void writeToSendBuffer1(ushort packetType, bool hasSign, ullong fieldFlag, uint bodySize, const char* bodyBuffer);
	// Buffer1放不下时进入这里。延迟队列只在主线程访问,不需要额外加锁。
	bool addPendingSendData(const char* data, int dataCount);
	// 将延迟队列按FIFO顺序尽可能写入target,返回实际写入的字节数。
	int flushPendingSendData(StreamBuffer* target);
	void destroyAllPacket();
protected:
	ArrayList<2048, PacketTCP*> mWillDestroyPackets;// 用于缓存
	DoubleBuffer<PacketTCP*> mExecutePacketList;	// 客户端收到消息的列表
	Vector<llong> mLastPingList;					// 最近10次的ping
	string mIP;										// 客户端的IP地址
	StreamBuffer* mSendBuffer = nullptr;			// 客户端发送数据缓冲区,只在发送线程中访问
	StreamBuffer* mRecvBuffer = nullptr;			// 客户端接收数据缓冲区,只在接收线程中访问
	SerializerBitRead mRecvReader;					// 只在接收线程解析消息时使用,避免每个消息重复构造Reader
	ThreadLock mPacketTempBuffer2Lock;				// mPacketTempBuffer2的线程锁
	SerializerBitWrite* mSendWriter = nullptr;		// 只在发送消息时用将要发送的数据写入到发送缓冲区,避免频繁构造析构
	StreamBuffer* mPacketTempBuffer1 = nullptr;		// 待发送消息的临时缓冲区,存储已经序列化,但是还没有写入发送缓冲区的数据,中转数据用,减少加锁时间,只在主线程中访问
	StreamBuffer* mPacketTempBuffer2 = nullptr;		// 待发送消息的临时缓冲区,存储已经序列化,但是还没有写入发送缓冲区的数据,中转数据用,减少加锁时间,在主线程和Send线程中访问
	std::deque<std::vector<char>> mPendingSendQueue;	// Buffer1空间不足时的延迟发送FIFO,只在主线程访问,按最终序列化字节保存
	int mPendingSendFrontOffset = 0;			// 延迟队列队首已经提交的字节偏移
	int mPendingSendBytes = 0;				// 延迟队列中尚未提交到Buffer2的总字节数
	sockaddr_in mUDPAddress;						// 发送给客户端udp消息的地址
	MY_SOCKET mSocket = 0;							// 客户端套接字
	llong mPlayerGUID = 0;							// 客户端的角色唯一ID,未登录角色时为无效ID
	llong mAccountGUID = 0;							// 客户端的账号唯一ID,未登录账号时为无效ID
	uint mSequenceNumber = 0;						// 消息包的序列号
	uint mLastReceiveNumber = 0;					// 上一次接收到的消息序列号
	int mServerPingIndex = 0;						// 当前正在等待客户端回复的主动ping消息的序号
	float mServerPingWaitTime = -1.0f;				// 服务器主动发送的ping消息已经等待的时间,小于0表示没有正在等待,用于主动检测客户端的延迟情况,如果延迟较大,则会忽略某些攻击等关键消息
	float mCurServerPingTime = mServerPingTime;		// 服务器主动发送ping消息的计时器
	float mHeartBeatTime = 0.0f;					// 客户端上一次的心跳到当前的时间,秒
	float mConnectTime = 0.0f;						// 客户端连接到服务器的时间,秒
	int mClientGUID = 0;							// 客户端唯一ID
	int mParsedCount = 0;							// 已成功解析的消息包数量
	string mDeadReason;								// 客户端断开的原因详细说明
	DEAD_TYPE mDeadType = DEAD_TYPE::NONE;			// 客户端断开的原因类型
	atomic<bool> mIsDeadClient;						// 该客户端是否已经断开连接或者心跳超
	static constexpr float mClientPingTime = 2.0f;	// 客户端主动心跳的间隔时间
	static constexpr float mServerPingTime = 2.0f;	// 服务器主动心跳的间隔时间
	static constexpr float mHighLagTime = 1.0f;		// 当客户端的延迟超过1秒时,不处理攻击消息
private:
	HashMap<ushort, int> mTempExecutePacketCountList;
	Vector<PacketTCP*> mTempPacketList;
};