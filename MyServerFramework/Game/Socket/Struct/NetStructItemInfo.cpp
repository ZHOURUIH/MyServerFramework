#include "GameHeader.h"

NetStructItemInfo::NetStructItemInfo(int itemID, int itemCount) :
	mItemID(itemID),
	mItemCount(itemCount)
{}

NetStructItemInfo& NetStructItemInfo::operator=(const NetStructItemInfo& other)
{
	mItemID = other.mItemID;
	mItemCount = other.mItemCount;
	return *this;
}

bool NetStructItemInfo::readFromBuffer(SerializerBitRead* reader)
{
	bool success = true;
	success = success && reader->readSigned(mItemID, mItemCount);
	return success;
}

bool NetStructItemInfo::writeToBuffer(SerializerBitWrite* serializer) const
{
	bool success = true;
	success = success && serializer->writeSigned(mItemID, mItemCount);
	return success;
}

void NetStructItemInfo::resetProperty()
{
	base::resetProperty();
	mItemID = 0;
	mItemCount = 0;
}