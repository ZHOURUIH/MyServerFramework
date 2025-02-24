#pragma once

#include "ComponentOwner.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API MovableObject : public ComponentOwner
{
	BASE(MovableObject, ComponentOwner);
public:
	~MovableObject() override;
	virtual void init() { initComponents(); }
	void destroy() override;
	virtual void initComponents() {}
	void update(float elapsedTime) override;
	virtual void setPosition(const Vector3& pos) { mPosition = pos; }
	void addDestroyCallback(OnObjectDestroy callback, void* userData) 
	{
		if (mDestroyCallbackList == nullptr)
		{
			mDestroyCallbackList = new Vector<pair<OnObjectDestroy, void*>>();
		}
		mDestroyCallbackList->emplace_back(callback, userData);
	}
	void removeDestroyCallback(const OnObjectDestroy callback, const void* userData);
	const Vector3& getPosition() const		{ return mPosition; }
	const Vector3& getLastPosition() const	{ return mLastPosition; }
	bool hasLastPosition() const			{ return mHasLastPosition; }
	bool isMoved() const					{ return mMovedDuringFrame; }
	void resetProperty() override;
protected:
	Vector<pair<OnObjectDestroy, void*>>* mDestroyCallbackList = nullptr;	// 销毁物体时的回调列表,注册了销毁回调的就会在销毁对象时进行通知
	Vector3 mLastPosition;				// 上一帧的位置
	Vector3 mPosition;					// 物体所在像素坐标
	bool mEnableLastPosition = false;	// 是否启用LastPosition的更新,为了提高效率,默认不启用
	bool mMovedDuringFrame = false;		// 在这一帧是否移动过
	bool mHasLastPosition = false;		// 是否有上一帧位置
};