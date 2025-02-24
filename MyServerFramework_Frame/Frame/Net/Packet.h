#pragma once

#include "ClassPooledObject.h"
#include "MyString.h"
#include "BinaryUtility.h"

class MICRO_LEGEND_FRAME_API Packet : public ClassPooledObject
{
	BASE(Packet, ClassPooledObject);
public:
	virtual void execute(){}
	virtual void debugInfo(MyString<1024>& buffer) {}
	ushort getType() const							{ return mType; }
	bool showInfo() const							{ return mShowInfo; }
	int getSequenceNumber() const					{ return mSequenceNumber; }
	ullong getFieldFlag() const						{ return mFieldFlag; }
	virtual const string& getPacketName()			{ return FrameDefine::EMPTY; }
	template<typename T>
	bool isFieldValid(T index) const				{ return hasBit(mFieldFlag, (byte)index); }
	void setFieldFlag(ullong flag)					{ mFieldFlag = flag; }
	void clearFieldFlag()							{ mFieldFlag = 0; }
	template<typename T>
	void setFieldValid(T index)						{ setBitOne(mFieldFlag, (byte)index); }
	template<typename T>
	void setFieldInvalid(T index)					{ setBitZero(mFieldFlag, (byte)index); }
	void setType(ushort type)						{ mType = type; }
	void setSequenceNumber(int sequence)			{ mSequenceNumber = sequence; }
	void resetProperty() override;
protected:
	template<typename... TypeList>
	void debug(MyString<1024>& buffer, TypeList&&... params)
	{
		strcat_t(buffer, ":", forward<TypeList>(params)...);
	}
protected:
	ullong mFieldFlag = FrameDefine::FULL_FIELD_FLAG;	// 成员变量是否有效的标记位
	int mSequenceNumber = 0;							// 包的序列号,用于校验是否发送重复相同的包
	ushort mType = 0;									// 消息类型枚举
	bool mShowInfo = true;								// 是否打印调试信息到日志
};