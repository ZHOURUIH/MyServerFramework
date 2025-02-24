#pragma once

#include "FrameBase.h"

// 退出一个账号
class MICRO_LEGEND_FRAME_API CmdNetServerLogoutAccount
{
public:
	static void execute(TCPServerClient* client);
	static void execute(WebSocketServerClient* client);
public:
	static LogoutAccountCallback mLogoutAccount;	// 退出玩家账号的回调,用于通知应用层退出账号登录
	static LogoutPlayerCallback mLogoutPlayer;		// 退出玩家角色的回调,用于通知应用层退出角色登录
	static WebSocketLogoutAccountCallback mWebSocketLogoutAccount;		// 退出玩家账号的回调,用于通知应用层退出账号登录,websocket
	static WebSocketLogoutPlayerCallback mWebSocketLogoutPlayer;		// 退出玩家角色的回调,用于通知应用层退出角色登录,websocket
};