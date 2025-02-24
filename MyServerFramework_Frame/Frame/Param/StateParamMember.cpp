#include "FrameHeader.h"

void StateParamMember::setValue(const string& stringParam)
{
	if (mValue.mTypeHash == mIntType)
	{
		*(int*)mPointer = SToI(stringParam);
	}
	else if (mValue.mTypeHash == mShortType)
	{
		*(short*)mPointer = SToI(stringParam);
	}
	else if (mValue.mTypeHash == mUShortType)
	{
		*(ushort*)mPointer = SToI(stringParam);
	}
	else if (mValue.mTypeHash == mFloatType)
	{
		*(float*)mPointer = SToF(stringParam);
	}
	else if (mValue.mTypeHash == mBoolType)
	{
		*(bool*)mPointer = SToI(stringParam) != 0;
	}
	else if (mValue.mTypeHash == mByteType)
	{
		*(byte*)mPointer = (byte)SToI(stringParam);
	}
	else if (mValue.mTypeHash == mVector2Type)
	{
		*(Vector2*)mPointer = SToV2(stringParam);
	}
	else if (mValue.mTypeHash == mVector3Type)
	{
		*(Vector3*)mPointer = SToV3(stringParam);
	}
	else if (mValue.mTypeHash == mIntListType)
	{
		SToIs(stringParam, *(Vector<int>*)mPointer);
	}
	else if (mValue.mTypeHash == mShortListType)
	{
		SToSs(stringParam, *(Vector<short>*)mPointer);
	}
	else if (mValue.mTypeHash == mUShortListType)
	{
		SToUSs(stringParam, *(Vector<ushort>*)mPointer);
	}
	else if (mValue.mTypeHash == mFloatListType)
	{
		SToFs(stringParam, *(Vector<float>*)mPointer);
	}
	else if (mValue.mTypeHash == mLLongType)
	{
		*(llong*)mPointer = SToLL(stringParam);
	}
	else
	{
		ERROR_PROFILE((string("状态参数类型错误,state:") + typeid(*this).name()).c_str());
	}
	mValue.parse(mPointer);
}

void StateParamMember::cloneFrom(StateParamMember& sourceMember)
{
	if (mValue.mTypeHash == mIntType)
	{
		*(int*)mPointer = *(int*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mShortType)
	{
		*(short*)mPointer = *(short*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mUShortType)
	{
		*(ushort*)mPointer = *(ushort*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mFloatType)
	{
		*(float*)mPointer = *(float*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mBoolType)
	{
		*(bool*)mPointer = *(bool*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mByteType)
	{
		*(byte*)mPointer = *(byte*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mVector2Type)
	{
		*(Vector2*)mPointer = *(Vector2*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mVector3Type)
	{
		*(Vector3*)mPointer = *(Vector3*)sourceMember.mPointer;
	}
	else if (mValue.mTypeHash == mIntListType)
	{
		((Vector<int>*)sourceMember.mPointer)->clone(*(Vector<int>*)mPointer);
	}
	else if (mValue.mTypeHash == mShortListType)
	{
		((Vector<short>*)sourceMember.mPointer)->clone(*(Vector<short>*)mPointer);
	}
	else if (mValue.mTypeHash == mUShortListType)
	{
		((Vector<ushort>*)sourceMember.mPointer)->clone(*(Vector<ushort>*)mPointer);
	}
	else if (mValue.mTypeHash == mFloatListType)
	{
		((Vector<float>*)sourceMember.mPointer)->clone(*(Vector<float>*)mPointer);
	}
	else if (mValue.mTypeHash == mLLongType)
	{
		*(llong*)mPointer = *(llong*)sourceMember.mPointer;
	}
	else
	{
		ERROR_PROFILE((string("状态参数类型错误,state:") + typeid(*this).name()).c_str());
	}
	mValue.parse(mPointer);
}