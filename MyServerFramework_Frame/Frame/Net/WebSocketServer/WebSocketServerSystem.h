#pragma once

#include "ThreadLock.h"
#include "FrameComponent.h"
#include "HashMap.h"
#include "Vector.h"
#include "CustomThread.h"

// WebSocket服务器系统,当前程序是一个服务器
class MICRO_LEGEND_FRAME_API WebSocketServerSystem : public FrameComponent
{
	BASE(WebSocketServerSystem, FrameComponent);
public:
	WebSocketServerSystem();
	void init() override;
	void quit() override;
	void update(float elapsedTime) override;
	float getHeartBeatTimeOut() const											{ return mHeartBeatTimeOut; }
	bool getOutputLog() const													{ return mOutputLog; }
	bool isAvailable() const													{ return mSocket != INVALID_SOCKET; }
	int getClientCount() const													{ return mClientList.size(); }
	ushort getPort() const														{ return mPort; }
	WebSocketServerClient* getClient(int clientGUID) const						{ return mClientList.get(clientGUID); }
	WebServerCheckPingCallback getServerCheckPingCallback() const				{ return mServerCheckPing; }
	const SerializerWrite* getPacketDataBuffer() const							{ return mPacketDataBuffer; }
	void setServerCheckPingCallback(WebServerCheckPingCallback callback)		{ mServerCheckPing = callback; }
	void setFreezeAccountCallback(FreezeAccountCallback callback)				{ mFreezeAccount = callback; }
	void increaseSendPacketCount()												{ ++mSendPacketCount; }
	void increaseReceivePacketCount(int count)									{ mReceivePacketCount += count; }
	void freezeAccount(llong accountGUID, llong timeSecond, const char* reason)	{ CALL(mFreezeAccount, accountGUID, timeSecond, reason); }
	// 将消息数据写入到公共序列化缓冲区,在发送消息前调用
	void writePacket(PacketWebSocket* packet);
	// 将所有客户端标记为断开
	void logoutAll();
protected:
	// 主线程处理Accept线程接收到的新连接
	int notifyAcceptClient(MY_SOCKET socket, const string& ip);
	// 主线程断开并销毁指定客户端
	void disconnectSocket(int clientGUID, const string& reason);
	// 接收新客户端连接的线程函数
	void acceptThread();
	// 向客户端发送数据的线程函数
	void sendThread();
	// 接收客户端数据的线程函数
	void recvThread();
	// 生成客户端唯一ID
	int generateSocketGUID()													{ return mSocketGUIDSeed++; }
	// 处理send或者recv的Socket错误
	void checkSendRecvError(WebSocketServerClient* client, int successLength) const;
protected:
	Vector<pair<MY_SOCKET, string>> mAcceptBuffer;					// Accept线程接收到的新连接缓存,first是Socket,second是IP
	Vector<pair<MY_SOCKET, string>> mTempAcceptBuffer;				// 主线程处理新连接时使用的临时缓存
	HashMap<int, WebSocketServerClient*> mClientList;				// 主线程维护的所有WebSocket客户端,key是clientGUID
	Vector<WebSocketServerClient*> mSendClientList;					// 发送线程使用的客户端列表
	Vector<WebSocketServerClient*> mRecvClientList;					// 接收线程使用的客户端列表
	CustomThread* mSendThread = nullptr;							// WebSocket发送线程
	CustomThread* mReceiveThread = nullptr;							// WebSocket接收线程
	CustomThread* mAcceptThread = nullptr;							// WebSocket客户端连接接收线程
	ThreadLock mSendLock;											// mSendClientList线程锁,保证发送过程中客户端不会被销毁
	ThreadLock mRecvLock;											// mRecvClientList线程锁,保证接收过程中客户端不会被销毁
	ThreadLock mAcceptLock;											// mAcceptBuffer线程锁
	SerializerWrite* mPacketDataBuffer = nullptr;					// 主线程序列化待发送消息时使用的公共缓冲区
	WebServerCheckPingCallback mServerCheckPing = nullptr;			// 服务器主动Ping客户端的回调
	FreezeAccountCallback mFreezeAccount = nullptr;					// 冻结账号回调
	MY_SOCKET mSocket = INVALID_SOCKET;								// WebSocket服务器监听Socket
	char* mRecvBuffer = nullptr;									// 接收线程共用的临时Socket接收缓冲区
	int mRecvEpollFD = -1;											// Linux下监听所有客户端可读事件的epoll句柄
	float mCurServerHeartBeatTime = mServerHeartBeatTimeOut;		// 当前服务器心跳计时
	float mHeartBeatTimeOut = 0.0f;									// 客户端心跳超时时间
	float mDumpPacketTimer = 0.0f;									// 网络消息统计输出计时
	int mServerHeartBeat = 0;										// 服务器心跳输出计数
	int mSocketGUIDSeed = 1;										// 客户端GUID生成种子
	int mWritePacketBytes = 0;										// 统计周期内序列化写入的消息总字节数
	int mWritePacketCount = 0;										// 统计周期内序列化写入的消息数量
	int mSendPacketCount = 0;										// 统计周期内发送的消息数量
	atomic<int> mReceiveBytesCount;									// 统计周期内接收到的Socket总字节数,接收线程写入
	atomic<int> mReceivePacketCount;								// 统计周期内接收到的消息数量,可能由子线程写入
	ushort mPort = 0;												// WebSocket服务器监听端口
	bool mOutputLog = true;											// 是否输出网络消息日志
	bool mWinsockInitialized = false;								// Windows下Winsock是否已经初始化
	static constexpr float mDumpPacketTimeInternal = 10.0f;			// 网络消息统计日志输出间隔,单位秒
	static constexpr float mServerHeartBeatTimeOut = 60.0f;			// 服务器自身心跳日志输出间隔,单位秒
};