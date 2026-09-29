#include "FrameHeader.h"

Set<int> ParamSet::mAllowMemberType;

void ParamSet::resetProperty()
{
	base::resetProperty();
	// mParamMemberList构造中填充的,不重置
	//if (mParamMemberList != nullptr)
	//{
	//	mParamMemberList->clear();
	//}
}

void ParamSet::setParam(const int index, const string& stringParam)
{
	if (mParamMemberList == nullptr || index >= mParamMemberList->size())
	{
		return;
	}
	(*mParamMemberList)[index].setValue(stringParam);
}

void ParamSet::initFromCopy(const ParamSet* other)
{
	if (mParamMemberList == nullptr || other->mParamMemberList == nullptr)
	{
		return;
	}
	// 这里只拷贝原始数据,具体的变量数据直接写赋值语句即可,如果通过这里来复制,效率较低,会执行很多的if-else
	const auto& otherList = *other->mParamMemberList;
	FOR_VECTOR(otherList)
	{
		(*mParamMemberList)[i].setValue(otherList[i].getValue());
	}
}

void ParamSet::valueDirty(void* pointer)
{
	for (auto& item : * mParamMemberList)
	{
		if (item.getPointer() == pointer)
		{
			item.valueDirty();
			break;
		}
	}
}

void ParamSet::initAllowMemberType()
{
	if (mAllowMemberType.isEmpty())
	{
		mAllowMemberType.add(mIntType);
		mAllowMemberType.add(mFloatType);
		mAllowMemberType.add(mBoolType);
		mAllowMemberType.add(mByteType);
		mAllowMemberType.add(mShortType);
		mAllowMemberType.add(mUShortType);
		mAllowMemberType.add(mVector2Type);
		mAllowMemberType.add(mVector3Type);
		mAllowMemberType.add(mIntListType);
		mAllowMemberType.add(mShortListType);
		mAllowMemberType.add(mUShortListType);
		mAllowMemberType.add(mFloatListType);
		mAllowMemberType.add(mLLongType);
	}
}