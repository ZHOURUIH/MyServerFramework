#pragma once

#include "ThreadLock.h"
#include "StreamBuffer.h"
#include "DoubleBuffer.h"
#include "HashMap.h"
#include "Vector.h"

// 当前程序是服务器时,表示连接到服务器的一个客户端
class MICRO_LEGEND_FRAME_API WebSocketServerClient
{
public:
	WebSocketServerClient(int clientGUID, MY_SOCKET s, const string& ip);
	virtual ~WebSocketServerClient();
	virtual void init();
	void update(float elapsedTime);
	void sendPacket(PacketWebSocket* packet);
	void sendPacket(ushort packetType);
	// 此函数在sendThread调用
	void writeToSendBuffer();
	// 消耗已经成功发送的数据,当前数据全部发送完成后会自动释放
	void consumeSendData(int sendCount);
	// 此函数在sendThread调用
	void clearSendBuffer();
	void recvData(const char* data, int dataCount);
	void setPlayerGUID(const llong guid)                        { mPlayerGUID = guid; }
	void setAccountGUID(const llong guid)                       { mAccountGUID = guid; }
	void setUDPAddress(const sockaddr_in& addr)                 { mUDPAddress = addr; }
	void setDeadClient(string&& reason, DEAD_TYPE type)
	{
		mDeadReason = move(reason);
		mIsDeadClient = true;
		mDeadType = type;
	}
	// 获得成员变量
	int getClientGUID() const                                   { return mClientGUID; }
	llong getAccountGUID() const                                { return mAccountGUID; }
	llong getPlayerGUID() const                                 { return mPlayerGUID; }
	MY_SOCKET getSocket() const                                 { return mSocket; }
	bool isDeadClient() const                                   { return mIsDeadClient; }
	const string& getDeadReason() const                         { return mDeadReason; }
	const Vector<pair<char*, Vector2Int>>& getSendBuffer() const{ return mSendBuffer; }
	const pair<char*, Vector2Int>& getFirstSendData() const     { return mSendBuffer[0]; }
	int getSendOffset() const                                   { return mSendOffset; }
	int getRecvDataCount() const                                { return mRecvBuffer->getDataLength(); }
	bool getIsLogin() const                                     { return mAccountGUID != 0; }
	const string& getIP() const                                 { return mIP; }
	const sockaddr_in& getUDPAddress() const                    { return mUDPAddress; }
	bool isClientHighLag() const                                { return mServerPingWaitTime >= mHighLagTime; }
	DEAD_TYPE getDeadType() const                               { return mDeadType; }
	void notifyPing();
	void notifyServerPing(int index);
protected:
	PARSE_RESULT packetRead(int& index, PacketWebSocket*& packet, string& reason);
	void debugInfo(const string& info) const;
	void debugError(const string& info) const;
	void executeAllPacket();
	void destroyAllPacket();
	bool queueRawSendData(const char* data, int dataCount);
	static HashMap<string, string> parseRequestHeader(const string& request);
	static string handle_websocket_handshake(const string& request);
	void handle_complete_message(char* data, int dataCount);
	// 返回值表示最终生成的数据长度
	static int generateSend(const char* message, int length, char* destBuffer, int destBufferSize);
protected:
	ArrayList<512, PacketWebSocket*> mWillDestroyList;          // 用于缓存一批待销毁的消息对象,减少频繁调用对象池销毁接口
	DoubleBuffer<PacketWebSocket*> mExecutePacketList;          // 客户端收到并等待主线程执行的消息列表
	Vector<llong> mLastPingList;                                // 最近一组客户端ping到达时间
	string mIP;                                                 // 客户端的IP地址
	Vector<pair<char*, Vector2Int>> mSendBuffer;                // 客户端最终发送数据缓冲区,只在发送线程访问,Vector2Int的x是数据长度,y是buffer大小
	StreamBuffer* mRawSendBuffer = nullptr;                     // 接收线程产生的原始待发送数据缓冲区,用于HTTP握手响应等无需WebSocket封装的数据
	StreamBuffer* mSocketRecvBuffer = nullptr;                  // Socket原始接收数据缓冲区,用于处理HTTP握手和WebSocket帧的TCP拆包粘包
	StreamBuffer* mRecvBuffer = nullptr;                        // WebSocket完整消息解包后的业务消息接收缓冲区,只在接收线程访问
	StreamBuffer* mMessageBuffer = nullptr;                     // 用于临时存储WebSocket分片消息合并后的数据
	StreamBuffer* mPayloadBuffer = nullptr;                     // 用于临时存储单个WebSocket帧解码后的Payload数据
	ThreadLock mPacketTempBufferLock;                           // mPacketTempBuffer0和mRawSendBuffer的线程锁
	SerializerWrite* mSendWriter = nullptr;                     // 将业务消息转换为最终WebSocket消息前使用的序列化缓冲区
	SerializerWrite* mPacketTempBuffer0 = nullptr;              // 主线程待发送消息临时缓冲区,存储已序列化但还未转移到发送线程的数据
	SerializerWrite* mPacketTempBuffer1 = nullptr;              // 发送线程中转缓冲区,用于缩短mPacketTempBufferLock持锁时间
	sockaddr_in mUDPAddress;                                    // 发送给客户端UDP消息的地址
	MY_SOCKET mSocket = 0;                                      // 客户端套接字
	llong mPlayerGUID = 0;                                      // 客户端的角色唯一ID,未登录角色时为无效ID
	llong mAccountGUID = 0;                                     // 客户端的账号唯一ID,未登录账号时为无效ID
	uint mSequenceNumber = 0;                                   // 服务器发送给该客户端的消息序列号
	uint mLastReceiveNumber = 0;                                // 上一次接收到的客户端消息序列号
	int mSendOffset = 0;                                        // mSendBuffer第一个数据已经成功发送的字节数,用于处理Socket部分发送
	int mServerPingIndex = 0;                                   // 当前正在等待客户端回复的主动ping消息序号
	float mServerPingWaitTime = -1.0f;                          // 服务器主动ping已经等待的时间,小于0表示当前没有等待回复
	float mCurServerPingTime = mServerPingTime;                 // 服务器主动发送ping消息的计时器
	float mHeartBeatTime = 0.0f;                                // 客户端上一次心跳到当前的剩余超时时间,秒
	float mConnectTime = 0.0f;                                  // 客户端连接到服务器的时间,秒
	int mClientGUID = 0;                                        // 客户端唯一ID
	int mParsedCount = 0;                                       // 已成功解析的业务消息包数量
	bool mHttpResponsed = false;                                // 是否已经处理过WebSocket HTTP握手请求
	bool mIsFragmented = false;                                 // 是否正在处理WebSocket分片消息
	string mDeadReason;                                         // 客户端断开的原因
	DEAD_TYPE mDeadType = DEAD_TYPE::NONE;                      // 客户端断开的原因类型
	atomic<bool> mIsDeadClient;                                 // 该客户端是否已经断开连接或者被标记为断开
	static constexpr float mClientPingTime = 2.0f;              // 客户端主动心跳的间隔时间
	static constexpr float mServerPingTime = 2.0f;              // 服务器主动心跳的间隔时间
	static constexpr float mHighLagTime = 1.0f;                 // 当客户端延迟超过1秒时,不处理攻击等关键消息
private:
	HashMap<ushort, int> mTempExecutePacketCountList;           // 临时统计一帧内每种消息的执行数量
	Vector<PacketWebSocket*> mTempPacketList;                   // 接收线程解析消息时使用的临时消息列表
	static string MAGIC_KEY;                                    // WebSocket握手协议规定的固定GUID
};