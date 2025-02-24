#pragma once

#include "ClassPooledObject.h"
#include "FrameDefine.h"
#include "StateParamMember.h"

class MICRO_LEGEND_FRAME_API ParamSet : public ClassPooledObject
{
	BASE(ParamSet, ClassPooledObject);
public:
	void resetProperty() override;
	void setParam(int index, const string& stringParam);
	const Vector<StateParamMember>& getParamMemberList() const { return mParamMemberList != nullptr ? *mParamMemberList : Vector<StateParamMember>::mDefaultList; }
	int getNeedParamCount() const { return mParamMemberList != nullptr ? mParamMemberList->size() : 0; }
	void initFromCopy(ParamSet* other);
	// 注册状态参数字段,只需要注册要发送到客户端的字段即可
	template<typename T>
	void registeParam(T& value)
	{
		const int typeHash = (int)typeid(T).hash_code();
#ifdef WINDOWS
		initAllowMemberType();
		if (!mAllowMemberType.contains(typeHash))
		{
			ERROR("注册类型错误");
		}
#endif
		if (mParamMemberList == nullptr)
		{
			mParamMemberList = new Vector<StateParamMember>();
		}
		mParamMemberList->emplace_back(&value, typeHash);
	}
	// 由于枚举类型比较特殊,所以单独将枚举类型当作byte来处理,暂不支持其他类型的枚举
	template<typename T>
	void registeEnumParam(T& value)
	{
		if (sizeof(T) != sizeof(byte))
		{
			ERROR_PROFILE(("注册类型错误:" + string(typeid(T).name())).c_str());
		}
		if (mParamMemberList == nullptr)
		{
			mParamMemberList = new Vector<StateParamMember>();
		}
		mParamMemberList->emplace_back(&value, (int)typeid(byte).hash_code());
	}
	void valueDirty(void* pointer);
	void initFromParam(const string& param0, const string& param1)
	{
		setParam(0, param0);
		setParam(1, param1);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6, const string& param7)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
		setParam(7, param7);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6, const string& param7, const string& param8)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
		setParam(7, param7);
		setParam(8, param8);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6, const string& param7, const string& param8, const string& param9)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
		setParam(7, param7);
		setParam(8, param8);
		setParam(9, param9);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6, const string& param7, const string& param8, const string& param9, const string& param10)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
		setParam(7, param7);
		setParam(8, param8);
		setParam(9, param9);
		setParam(10, param10);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6, const string& param7, const string& param8, const string& param9, const string& param10, const string& param11)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
		setParam(7, param7);
		setParam(8, param8);
		setParam(9, param9);
		setParam(10, param10);
		setParam(11, param11);
	}
	void initFromParam(const string& param0, const string& param1, const string& param2, const string& param3, const string& param4, const string& param5, const string& param6, const string& param7, const string& param8, const string& param9, const string& param10, const string& param11, const string& param12)
	{
		setParam(0, param0);
		setParam(1, param1);
		setParam(2, param2);
		setParam(3, param3);
		setParam(4, param4);
		setParam(5, param5);
		setParam(6, param6);
		setParam(7, param7);
		setParam(8, param8);
		setParam(9, param9);
		setParam(10, param10);
		setParam(11, param11);
		setParam(12, param12);
	}
protected:
	static void initAllowMemberType();
protected:
	Vector<StateParamMember>* mParamMemberList = nullptr;
private:
	static Set<int> mAllowMemberType;
};