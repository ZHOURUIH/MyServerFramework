#pragma once

#include "ClassPooledObject.h"

class MICRO_LEGEND_FRAME_API GameComponent : public ClassPooledObject
{
	BASE(GameComponent, ClassPooledObject);
public:
	virtual void init(ComponentOwner* owner) { mComponentOwner = owner; }
	virtual void destroy() {}
	virtual void setActive(bool active, bool isInit = false);
	void setType(ushort type)						{ mComponentType = type; }
	void setDefaultActive(const bool defaultActive)	{ mDefaultActive = defaultActive; }
	ComponentOwner* getOwner() const				{ return mComponentOwner; }
	ushort getType() const							{ return mComponentType; }
	bool isActive() const							{ return mActive; }
	bool getDefaultActive() const					{ return mDefaultActive; }
	void resetProperty() override;
protected:
	// 只是为子类提供一个公用的函数
	template<typename FUNC, typename CLASS, typename TypeCheck = typename IsSubClassOf<GameComponent, CLASS>::mType>
	static void setCallback(FUNC callback, void* userData, FUNC& curCallback, void*& curUserData, CLASS* component)
	{
		FUNC tempCallback = curCallback;
		void* tempUserData = curUserData;
		curCallback = nullptr;
		curUserData = nullptr;
		// 如果回调函数当前不为空,则是中断了正在进行的变化
		CALL(tempCallback, component, tempUserData, true);
		curCallback = callback;
		curUserData = userData;
	}
	template<typename FUNC, typename CLASS, typename TypeCheck = typename IsSubClassOf<GameComponent, CLASS>::mType>
	void doneCallback(FUNC& curDoneCallback, void*& curDoneUserData, CLASS* component)
	{
		// 先保存回调,然后再调用回调之前就清空回调,确保在回调函数执行时已经完全完成
		FUNC tempCallback = curDoneCallback;
		void* tempUserData = curDoneUserData;
		clearCallback();
		CALL(tempCallback, component, tempUserData, false);
	}
	virtual void clearCallback() {}
	// 为了效率考虑,基类不提供update虚函数,需要每帧更新时可以重写以下两个函数进行update函数的注册
	// 因为在一条继承链上的所有组件都可能会有自己的FrameTick或者SecondTick,所以只能让每个子类自己注册
	// 不能将函数指针存到基类,由基类来注册或者注销
	virtual void registeTick() {}
	virtual void unregisteTick() {}
protected:
	ComponentOwner* mComponentOwner = nullptr;	// 该组件的拥有者
	ushort mComponentType = 0;					// 组件类型
	bool mActive = true;						// 是否激活组件,默认为false
	bool mDefaultActive = true;					// 组件默认激活状态
};