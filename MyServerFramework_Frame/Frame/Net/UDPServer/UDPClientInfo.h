#pragma once

#include "FrameBase.h"

// udp通信的单个客户端的信息
class MICRO_LEGEND_FRAME_API UDPClientInfo
{
protected:
	Vector<pair<char*, int>> mSendList;	// 待发送的消息列表
	sockaddr_in mAddress;				// 发送的地址
	llong mToken = 0;					// 客户端凭证,一般是角色ID
public:
	~UDPClientInfo() { clear(); }
	llong getToken() const								{ return mToken; }
	const sockaddr_in& getAddress() const				{ return mAddress; }
	const Vector<pair<char*, int>>& getSendList() const { return mSendList; }
	void moveSendList(Vector<pair<char*, int>>& target) { target = move(mSendList); }
	void setToken(const llong token)					{ mToken = token; }
	void setAddress(const sockaddr_in& address)			{ mAddress = address; }
	void addSendData(char* data, const int length)		{ mSendList.emplace_back(data, length); }
	void clear();
};