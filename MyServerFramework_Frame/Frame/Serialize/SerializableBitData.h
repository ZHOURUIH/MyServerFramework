#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API SerializableBitData
{
protected:
	ullong mFieldFlag = FrameDefine::FULL_FIELD_FLAG;	// 成员变量是否有效的标记位
public:
	SerializableBitData& operator=(const SerializableBitData& other)
	{
		mFieldFlag = other.mFieldFlag;
		return *this;
	}
	virtual bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) = 0;
	virtual bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const = 0;
	virtual bool hasSign() const		{ return false; }
	virtual void resetProperty()		{ mFieldFlag = FrameDefine::FULL_FIELD_FLAG; }
	virtual bool isOptional()			{ return false; }
	ullong getFieldFlag() const			{ return mFieldFlag; }
	template<typename T>
	bool isFieldValid(T index) const	{ return hasBit(mFieldFlag, (byte)index); }
	template<typename T>
	void setFieldValid(T index)			{ setBitOne(mFieldFlag, (byte)index); }
	void clearFieldFlag()				{ mFieldFlag = 0; }
};