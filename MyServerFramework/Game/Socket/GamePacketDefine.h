#pragma once

#include "FrameDefine.h"

class PACKET_TYPE
{
public:
	constexpr static ushort GATE_CS_MIN = 1000;
	constexpr static ushort GateCSHeartBeat = 1001;
	constexpr static ushort GateCSServerInfo = 1002;

	constexpr static ushort GATE_SC_MIN = 1200;
	constexpr static ushort GateSCHeartBeat = 1201;

	constexpr static ushort MAIL_CS_MIN = 1500;
	constexpr static ushort MailCSHeartBeat = 1501;
	constexpr static ushort MailCSSendMail = 1502;

	constexpr static ushort MAIL_SC_MIN = 2000;
	constexpr static ushort MailSCHeartBeat = 2001;

	// auto generate start
	constexpr static ushort MIN = 0;

	constexpr static ushort CS_MIN = 10000;
	constexpr static ushort CSCheckPacketVersion = 10001;
	constexpr static ushort CSServerCheckPing = 10002;
	constexpr static ushort CSAttack = 10003;
	constexpr static ushort CSLogin = 10004;

	constexpr static ushort SC_MIN = 20000;
	constexpr static ushort SCCheckPacketVersion = 20001;
	constexpr static ushort SCServerCheckPing = 20002;
	constexpr static ushort SCCharacterFullGameData = 20003;
	constexpr static ushort SCGetItemTip = 20004;
	constexpr static ushort SCAttack = 20005;
	// auto generate end
};