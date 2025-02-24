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

void ParamSet::initFromCopy(ParamSet* other)
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
	FOR_VECTOR(*mParamMemberList)
	{
		if ((*mParamMemberList)[i].getPointer() == pointer)
		{
			(*mParamMemberList)[i].valueDirty();
			break;
		}
	}
}

void ParamSet::initAllowMemberType()
{
	if (mAllowMemberType.size() == 0)
	{
		mAllowMemberType.insert(mIntType);
		mAllowMemberType.insert(mFloatType);
		mAllowMemberType.insert(mBoolType);
		mAllowMemberType.insert(mByteType);
		mAllowMemberType.insert(mShortType);
		mAllowMemberType.insert(mUShortType);
		mAllowMemberType.insert(mVector2Type);
		mAllowMemberType.insert(mVector3Type);
		mAllowMemberType.insert(mIntListType);
		mAllowMemberType.insert(mShortListType);
		mAllowMemberType.insert(mUShortListType);
		mAllowMemberType.insert(mFloatListType);
		mAllowMemberType.insert(mLLongType);
	}
}