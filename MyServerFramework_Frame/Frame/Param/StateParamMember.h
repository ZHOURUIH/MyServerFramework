#pragma once

#include "FrameDefine.h"
#include "Utility.h"
#include "StateParamMemberValue.h"

// 存储buff参数,包含变量地址和值
class MICRO_LEGEND_FRAME_API StateParamMember
{
protected:
	StateParamMemberValue mValue;		// 存储类型和已经缓存的值
	void* mPointer = nullptr;			// 当单独改变mPointer指向地址的内容时,需要通知标记已修改
public:
	StateParamMember() {}
	StateParamMember(void* pointer, const int typeHash):
		mPointer(pointer)
	{
		mValue.mTypeHash = typeHash;
	}
	void* getPointer() const							{ return mPointer; }
	void valueDirty()									{ mValue.parse(mPointer); }
	int getTypeHash() const								{ return mValue.mTypeHash; }
	bool canBeFloat() const								{ return mValue.canBeFloat(); }
	const StateParamMemberValue& getValue() const		{ return mValue; }
	void setValue(const StateParamMemberValue& value)	{ mValue.copyValue(value); }
	void setValue(const string& stringParam);
	void cloneFrom(StateParamMember& sourceMember);
	float asFloat() const								{ return mValue.asFloat(); }
	const string& asString() const						{ return mValue.asString(); }
};