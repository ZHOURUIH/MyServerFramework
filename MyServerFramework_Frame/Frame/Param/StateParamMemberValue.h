#pragma once

#include "FrameDefine.h"
#include "Utility.h"

// 存储buff参数的值
class MICRO_LEGEND_FRAME_API StateParamMemberValue
{
public:
	int mTypeHash = 0;
	float mFloatValue = 0.0f;
	string mStringValue;
public:
	void copyValue(const StateParamMemberValue& other)
	{
		mFloatValue = other.mFloatValue;
		mStringValue = other.mStringValue;
	}
	bool canBeFloat() const
	{
		return mTypeHash == mIntType ||
			   mTypeHash == mShortType ||
			   mTypeHash == mUShortType ||
			   mTypeHash == mFloatType ||
			   mTypeHash == mBoolType ||
			   mTypeHash == mByteType;
	}
	float asFloat() const
	{
		if (!canBeFloat())
		{
			ERROR_PROFILE("不能获取为浮点类型");
			return 0.0f;
		}
		return mFloatValue;
	}
	const string& asString() const
	{
		if (canBeFloat())
		{
			ERROR_PROFILE("不能获取为字符串类型");
			return FrameDefine::EMPTY;
		}
		return mStringValue;
	}
	void parse(void* pointer)
	{
		if (canBeFloat())
		{
			parseAsFloat(pointer);
		}
		else
		{
			parseAsString(pointer);
		}
	}
protected:
	void parseAsFloat(void* pointer)
	{
		if (mTypeHash == mIntType)
		{
			mFloatValue = (float)(*(int*)pointer);
		}
		else if (mTypeHash == mShortType)
		{
			mFloatValue = (float)(*(short*)pointer);
		}
		else if (mTypeHash == mUShortType)
		{
			mFloatValue = (float)(*(ushort*)pointer);
		}
		else if (mTypeHash == mFloatType)
		{
			mFloatValue = *(float*)pointer;
		}
		else if (mTypeHash == mBoolType)
		{
			mFloatValue = (*(bool*)pointer) ? 1.0f : 0.0f;
		}
		else if (mTypeHash == mByteType)
		{
			mFloatValue = (float)(*(byte*)pointer);
		}
		else
		{
			mFloatValue = 0.0f;
		}
	}
	void parseAsString(void* pointer)
	{
		if (mTypeHash == mVector3Type)
		{
			mStringValue = V3ToS(*(Vector3*)pointer);
		}
		else if (mTypeHash == mVector2Type)
		{
			mStringValue = V2ToS(*(Vector2*)pointer);
		}
		else if (mTypeHash == mIntListType)
		{
			mStringValue = IsToS(*(Vector<int>*)pointer);
		}
		else if (mTypeHash == mShortListType)
		{
			mStringValue = SsToS(*(Vector<short>*)pointer);
		}
		else if (mTypeHash == mUShortListType)
		{
			mStringValue = USsToS(*(Vector<ushort>*)pointer);
		}
		else if (mTypeHash == mFloatListType)
		{
			mStringValue = FsToS(*(Vector<float>*)pointer);
		}
		else
		{
			mStringValue.clear();
		}
	}
};