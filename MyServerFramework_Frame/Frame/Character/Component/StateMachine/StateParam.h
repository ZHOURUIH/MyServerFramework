#pragma once

#include "ClassPooledObject.h"
#include "FrameCallback.h"
#include "StateParamMember.h"
#include "FrameDefine.h"
#include "ParamSet.h"

class MICRO_LEGEND_FRAME_API StateParam : public ClassPooledObject
{
	BASE(StateParam, ClassPooledObject);
public:
	// 这里拷贝原始参数是为了同步到客户端时使用
	virtual void initFromCopy(StateParam* other)				{ mParamSet.initFromCopy(&(other->mParamSet)); }
	virtual void afterGenerateExtraParam() {}
	const Vector<StateParamMember>& getParamMemberList() const	{ return mParamSet.getParamMemberList(); }
	int getNeedParamCount() const								{ return mParamSet.getNeedParamCount(); }
	ushort getType() const										{ return mType; }
	float getStateTime() const									{ return mStateTime; }
	void setType(ushort type)									{ mType = type; }
	void setStateTime(float time)								{ mStateTime = time; }
	void setSourceCharacterGUID(llong characterGUID)			{ mSourceCharacterGUID = characterGUID; }
	void valueDirty(void* pointer)								{ mParamSet.valueDirty(pointer); }
	void resetProperty() override;
protected:
	// 注册状态参数字段,只需要注册要发送到客户端的字段即可
	template<typename T>
	void registeParam(T& value) { mParamSet.registeParam(value); }
	template<typename T>
	void registeEnumParam(T& value) { mParamSet.registeEnumParam(value); }
public:
	Character* mTarget = nullptr;
	Character* mSource = nullptr;
	llong mSourceCharacterGUID = 0;
	ushort mType = 0;
	float mStateTime = -1.0f;
	ParamSet mParamSet;					// 参数数据的指针列表，用于统一获取所有的此状态所有的参数,在构造中注册参数变量,这样才能在同步到客户端时正确发送状态参数
};