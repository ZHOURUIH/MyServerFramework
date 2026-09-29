#include "GameHeader.h"

NetStructItemInfo::NetStructItemInfo(int itemID, int itemCount) :
	mItemID(itemID),
	mItemCount(itemCount)
{}

NetStructItemInfo& NetStructItemInfo::operator=(const NetStructItemInfo& other)
{
	base::operator=(other);
	mItemID = other.mItemID;
	mItemCount = other.mItemCount;
	return *this;
}

bool NetStructItemInfo::readFromBuffer(SerializerBitRead* reader, const bool needReadSign)
{
	bool success = true;
	success = success && reader->readSigned(needReadSign, mItemID, mItemCount);
	return success;
}

bool NetStructItemInfo::writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const
{
	bool success = true;
	success = success && writer->writeSigned(needWriteSign, mItemID, mItemCount);
	return success;
}

void NetStructItemInfo::resetProperty()
{
	base::resetProperty();
	mItemID = 0;
	mItemCount = 0;
}