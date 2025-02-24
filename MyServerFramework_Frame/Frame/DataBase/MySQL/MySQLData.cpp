#ifdef _MYSQL
#include "FrameHeader.h"

void MySQLData::cloneWithFlag(MySQLData* target, const ullong flag) const
{
	if (hasBit(flag, 0))
	{
		target->mID = mID;
	}
}

void MySQLData::resetProperty()
{
	base::resetProperty();
	mID = 0;
}

#endif