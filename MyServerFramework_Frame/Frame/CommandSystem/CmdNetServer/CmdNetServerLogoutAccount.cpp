#include "FrameHeader.h"

LogoutAccountCallback CmdNetServerLogoutAccount::mLogoutAccount;
LogoutPlayerCallback CmdNetServerLogoutAccount::mLogoutPlayer;
WebSocketLogoutAccountCallback CmdNetServerLogoutAccount::mWebSocketLogoutAccount;
WebSocketLogoutPlayerCallback CmdNetServerLogoutAccount::mWebSocketLogoutPlayer;

void CmdNetServerLogoutAccount::execute(TCPServerClient* client)
{
	if (client == nullptr)
	{
		return;
	}

	// 如果玩家在线,则退出玩家角色
	if (client->getPlayerGUID() > 0)
	{
		// 通知UDP此客户端断开连接
		mUDPServerSystem->getClient()->removeClientToken(client->getPlayerGUID());
		CALL(mLogoutPlayer, client);
	}
	// 退出账号登录
	mTCPServerSystem->notifyAccountLogout(client);
	CALL(mLogoutAccount, client);
}

void CmdNetServerLogoutAccount::execute(WebSocketServerClient* client)
{
	if (client == nullptr)
	{
		return;
	}

	// 如果玩家在线,则退出玩家角色
	if (client->getPlayerGUID() > 0)
	{
		// 通知UDP此客户端断开连接
		mUDPServerSystem->getClient()->removeClientToken(client->getPlayerGUID());
		CALL(mWebSocketLogoutPlayer, client);
	}
	// 退出账号登录
	mWebSocketServerSystem->notifyAccountLogout(client);
	CALL(mWebSocketLogoutAccount, client);
}