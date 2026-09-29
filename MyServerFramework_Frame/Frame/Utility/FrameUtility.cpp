#include "FrameHeader.h"

namespace FrameUtility
{
	llong mGUIDSeed = 0;
	bool tickTimerLoop(float& time, const float elapsedTime, const float interval)
	{
		if (time < 0.0f)
		{
			return false;
		}
		time -= elapsedTime;
		if (time > 0.0f)
		{
			return false;
		}
		time += interval;
		// 如果加上间隔以后还是小于0,则可能间隔太小了,需要将计时重置到间隔时间,避免计时停止
		if (time <= 0.0f)
		{
			time = interval;
		}
		return true;
	}

	bool tickTimerLoopEnsure(float& time, const float elapsedTime, const float interval)
	{
		if (time < 0.0f)
		{
			return false;
		}
		time -= elapsedTime;
		if (time > 0.0f)
		{
			return false;
		}
		time = interval;
		return true;
	}

	bool tickTimerOnce(float& time, const float elapsedTime)
	{
		if (time < 0.0f)
		{
			return false;
		}
		time -= elapsedTime;
		if (time > 0.0f)
		{
			return false;
		}
		time = -1.0f;
		return true;
	}

	void sendPacketTypeTCP(const ushort packetType, const string& name, TCPServerClient* client)
	{
		if (client == nullptr)
		{
			return;
		}
		client->sendPacket(packetType, name);
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

	void delayCall(const float time, const VoidCallback& func)
	{
		if (isMainThread())
		{
			auto* cmd = CMD_DELAY<CmdGlobalDelayLambda>();
			cmd->mFunction = func;
			mCommandSystem->pushCommandDelay(cmd, mCommandSystem, time);
		}
		else
		{
			auto* cmd = CMD_THREAD_DELAY<CmdGlobalDelayLambda>();
			cmd->mFunction = func;
			mCommandSystem->pushCommandDelay(cmd, mCommandSystem, time);
		}
	}

	void delayCall(const VoidCallback& func)
	{
		if (isMainThread())
		{
			mDelayCallSystem->addCall(func);
		}
		else
		{
			mDelayCallSystem->addCallCrossThread(func);
		}
	}
}