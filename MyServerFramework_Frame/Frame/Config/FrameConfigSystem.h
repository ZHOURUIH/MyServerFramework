#pragma once

#include "FrameComponent.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API FrameConfigSystem : public FrameComponent
{
	BASE(FrameConfigSystem, FrameComponent);
public:
	~FrameConfigSystem() override { destory(); }
	void init() override;
	void destory() {}
	int getTCPPort() const					{ return mTCPPort; }
	int getUDPPort() const					{ return mUDPPort; }
	int getWebSocketPort() const			{ return mWebSocketPort; }
	int getHttpPort() const					{ return mHttpPort; }
	float getHeartBeatTimeout() const		{ return mHeartBeatTimeout; }
	int getBacklog() const					{ return mBacklog; }
	bool getShowCommandDebugLog() const		{ return mShowCommandDebugLog > 0; }
	bool getOutputNetLog() const			{ return mOutputNetLog > 0; }
	int getMySQLPortNumber() const			{ return mMySQLPortNumber; }
	const string& getDomainName() const		{ return mDomainName; }
	const string& getServerName() const		{ return mServerName; }
	const string& getMySQLHost() const		{ return mMySQLHost; }
	const string& getMySQLUser() const		{ return mMySQLUser; }
	const string& getMySQLPassword() const	{ return mMySQLPassword; }
	const string& getMySQLDataBase() const	{ return mMySQLDataBase; }
	static Vector<pair<string, string>> parseConfig(const string& fileName);
protected:
	void readConfig(const Vector<pair<string, string>>& valueList);
protected:
	int mTCPPort = 0;					// tcp绑定的端口号
	int mUDPPort = 0;					// udp绑定的接收消息的端口号
	int mWebSocketPort = 0;				// WebSocket绑定的端口号
	int mHttpPort = 0;					// http端口号
	float mHeartBeatTimeout = 0.0f;		// 心跳超时时间
	int mBacklog = 0;					// 连接请求队列的最大长度
	int mShowCommandDebugLog = 0;		// 是否显示命令调试信息
	int mOutputNetLog = 0;				// 是否显示网络日志信息
	int mMySQLPortNumber = 0;			// 数据库端口号
	string mDomainName;					// 连接的服务器域名
	string mServerName;					// 服务器名字
	string mMySQLHost;					// 数据库域名或者IP
	string mMySQLUser;					// 数据库用户名
	string mMySQLPassword;				// 数据库密码
	string mMySQLDataBase;				// 数据库名
};