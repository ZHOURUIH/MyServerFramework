#include "FrameHeader.h"

namespace FrameUtility
{
	bool tickTimerLoop(float& time, const float elapsedTime, const float interval, const bool ensureInterval)
	{
		if (time >= 0.0f)
		{
			time -= elapsedTime;
			if (time <= 0.0f)
			{
				if (ensureInterval)
				{
					time = interval;
				}
				else
				{
					time += interval;
					// 如果加上间隔以后还是小于0,则可能间隔太小了,需要将计时重置到间隔时间,避免计时停止
					if (time <= 0.0f)
					{
						time = interval;
					}
				}
				return true;
			}
		}
		return false;
	}

	bool tickTimerOnce(float& time, const float elapsedTime)
	{
		if (time >= 0.0f)
		{
			time -= elapsedTime;
			if (time <= 0.0f)
			{
				time = -1.0f;
				return true;
			}
		}
		return false;
	}

	void sendPacketTCP(PacketTCP* packet, TCPServerClient* client)
	{
		if (client == nullptr)
		{
			return;
		}
		mTCPServerSystem->writePacket(packet);
		client->sendPacket(packet);
	}

	void sendPacketTypeTCP(const ushort packetType, const string& name, TCPServerClient* client)
	{
		if (client != nullptr)
		{

			client->sendPacket(packetType, name);
		}
	}

	void sendPacketUDP(PacketTCP* packet, TCPServerClient* client)
	{
		if (client == nullptr)
		{
			return;
		}
		mUDPServerSystem->writePacket(packet);
		const llong token = client->getPlayerGUID();
		if (token != 0)
		{
			mUDPServerSystem->getClient()->sendPacket(packet, token, client->getUDPAddress());
		}
	}

	void sendPacketWebSocket(PacketWebSocket* packet, WebSocketServerClient* client)
	{
		if (client == nullptr)
		{
			return;
		}
		mWebSocketServerSystem->writePacket(packet);
		client->sendPacket(packet);
	}

	void delayCall(const VoidCallback& func, const float time)
	{
		if (isMainThread())
		{
			CMD_DELAY(CmdGlobalDelayLambda, cmd);
			cmd->mFunction = func;
			mCommandSystem->pushCommandDelay(cmd, mCommandSystem, time);
		}
		else
		{
			CMD_THREAD_DELAY(CmdGlobalDelayLambda, cmd);
			cmd->mFunction = func;
			mCommandSystem->pushCommandDelay(cmd, mCommandSystem, time);
		}
	}
}