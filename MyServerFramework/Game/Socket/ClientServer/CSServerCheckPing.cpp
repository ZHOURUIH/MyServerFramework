#include "GameHeader.h"

// auto generate start
string CSServerCheckPing::mPacketName = STR(CSServerCheckPing);
// auto generate end

void CSServerCheckPing::execute()
{
	SCCheckPacketVersion::send(mClient);
}