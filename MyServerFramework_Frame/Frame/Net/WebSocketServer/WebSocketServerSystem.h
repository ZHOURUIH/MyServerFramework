#pragma once

#include "ThreadLock.h"
#include "FrameComponent.h"
#include "HashMap.h"
#include "Vector.h"
#include "CustomThread.h"

// 当前程序是一个服务器
class MICRO_LEGEND_FRAME_API WebSocketServerSystem : public FrameComponent
{
	BASE(WebSocketServerSystem, FrameComponent);
public:
	WebSocketServerSystem();
	void init() override;
	void quit() override;
	void update(const float elapsedTime) override;
	float getHeartBeatTimeOut()	const											{ return mHeartBeatTimeOut; }
	bool getOutputLog() const 													{ return mOutputLog; }
	bool isAvailable() const													{ return mSocket != INVALID_SOCKET; }
	int getClientCount() const													{ return mClientList.size(); }
	ushort getPort() const														{ return mPort; }
	WebSocketServerClient* getClient(const int clientGUID) const				{ return mClientList.tryGet(clientGUID); }
	WebSocketServerClient* getClientByAccountGUID(const llong accountGUID) const{ return mClientAccountIDList.tryGet(accountGUID); }
	WebServerCheckPingCallback getServerCheckPingCallback() const				{ return mServerCheckPing; }
	void setServerCheckPingCallback(WebServerCheckPingCallback callback)		{ mServerCheckPing = callback; }
	void setFreezeAccountCallback(FreezeAccountCallback callback)				{ mFreezeAccount = callback; }
	void increaseSendPacketCount()												{ ++mSendPacketCount; }
	void increaseReceivePacketCount(int count)									{ mReceivePacketCount += count; }
	void freezeAccount(llong accountGUID, llong timeSecond, const char* reason) { CALL(mFreezeAccount, accountGUID, timeSecond, reason); }
	// 将消息数据写入到缓冲区,在发送消息前调用
	void writePacket(PacketWebSocket* packet);
	const SerializerWrite* getPacketDataBuffer() const							{ return mPacketDataBuffer; }
	void logoutAll();
	void notifyAccountLogin(WebSocketServerClient* client);
	void notifyAccountLogout(WebSocketServerClient* client);
	static void encrypt(char* data, int length, const byte* key, int keyLen, byte param);
	static void decrypt(char* data, int length, const byte* key, int keyLen, byte param);
protected:
	static void acceptThread(CustomThread* thread);
	static void receiveThread(CustomThread* thread) { static_cast<This*>(thread->getArgs())->processRecv(); }
	static void sendThread(CustomThread* thread) { static_cast<This*>(thread->getArgs())->processSend(); }
	int notifyAcceptClient(const MY_SOCKET socket, const string& ip);
	void disconnectSocket(const int clientGUID, const string& reason);	// 与客户端断开连接,只能在主线程中调用
	void processSend();
	void processRecv();
	int generateSocketGUID() { return mSocketGUIDSeed++; }
	void checkSendRecvError(WebSocketServerClient* client, const int successLength) const;
protected:
	Vector<pair<MY_SOCKET, string>> mAcceptBuffer;				// 连接缓存列表
	HashMap<llong, WebSocketServerClient*> mClientAccountIDList;// 客户端列表,索引为账号ID,用于根据账号ID查找客户端
	HashMap<int, WebSocketServerClient*> mClientList;			// 客户端列表,用于主线程访问
	Vector<WebSocketServerClient*> mSendClientList;				// 客户端列表,用于发送线程访问,修改客户端列表时,需要同步修改这三个列表
	Vector<WebSocketServerClient*> mRecvClientList;				// 客户端列表,用于接收线程访问
	CustomThread* mSendThread = nullptr;						// 发送线程
	CustomThread* mReceiveThread = nullptr;						// 接收线程
	CustomThread* mAcceptThread = nullptr;						// 连接线程
	ThreadLock mSendLock;										// mSendClientList的锁,其实可以使用读写锁来代替,可以不用复制多份列表
	ThreadLock mRecvLock;										// mRecvClientList的锁
	ThreadLock mAcceptLock;										// mAcceptBuffer的锁
	SerializerWrite* mPacketDataBuffer = nullptr;				// 用于在发送消息时消息序列化,序列化以后再加密然后写入到发送缓冲区,之前是存在PacketWebSocket中,每个Packet都有一个,但是那样很占内存
	WebServerCheckPingCallback mServerCheckPing = nullptr;		// 服务器主动检查客户端延迟的函数
	FreezeAccountCallback mFreezeAccount = nullptr;				// 冻结账号的函数
	MY_SOCKET mSocket = INVALID_SOCKET;							// 服务器套接字
	char* mRecvBuffer = nullptr;								// 数据接收缓冲区
	float mCurServerHeartBeatTime = mServerHeartBeatTimeOut;	// 当前服务器心跳计时
	float mHeartBeatTimeOut = 0.0f;								// 心跳超时时间
	float mDumpPacketTimer = 0.0f;								// 用于每隔一定时间打印一次收发包信息的计时器
	int mServerHeartBeat = 0;									// 服务器当前心跳次数
	int mSocketGUIDSeed = 1;									// 客户端连接的ID种子
	int mMaxSocket = 0;											// 连接的客户端的套接字的最大值
	int mWritePacketBytes = 0;									// 记录写包数据的大小
	int mWritePacketCount = 0;									// 记录写包数据的数量
	int mSendPacketCount = 0;									// 记录发包的数量
	atomic<int> mReceiveBytesCount;								// 记录接收到的包字节数
	atomic<int> mReceivePacketCount;							// 记录接收到的包数量
	ushort mPort = 0;											// 端口号
	bool mOutputLog = true;										// 是否输出日志
	static constexpr float mDumpPacketTimeInternal = 10.0f;		// 每10秒打印一次收发数据信息
	static constexpr float mServerHeartBeatTimeOut = 60.0f;		// 服务器自身心跳间隔时间
	static constexpr int mKey0 = 41;
	static constexpr int mKey1 = 3;
	static constexpr int mKey2 = 600;
	static constexpr int mKey3 = 34;
};