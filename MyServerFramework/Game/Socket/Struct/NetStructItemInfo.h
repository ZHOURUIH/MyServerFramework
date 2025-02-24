#pragma once

#include "SerializableBitData.h"

// 通用的物品信息结构体
class NetStructItemInfo : public SerializableBitData
{
	BASE(NetStructItemInfo, SerializableBitData);
public:
	int mItemID = 0;
	int mItemCount = 0;
public:
	NetStructItemInfo() = default;
	NetStructItemInfo(int itemID, int itemCount);
	NetStructItemInfo& operator=(const NetStructItemInfo& other);
	bool readFromBuffer(SerializerBitRead* reader) override;
	bool writeToBuffer(SerializerBitWrite* serializer) const override;
	void resetProperty() override;
};