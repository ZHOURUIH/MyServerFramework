#include "FrameHeader.h"

void MovableObjectComponentTrackTarget::setTrackTarget(void* target)
{
	// 移除对旧目标的监听
	if (mTarget != nullptr)
	{
		(static_cast<MovableObject*>(mTarget))->removeDestroyCallback(onTargetDestroy, this);
	}
	// 添加对新目标的监听
	if (target != nullptr)
	{
		(static_cast<MovableObject*>(target))->addDestroyCallback(onTargetDestroy, this);
	}
	base::setTrackTarget(target);
}

Vector3 MovableObjectComponentTrackTarget::getPosition() const
{
	return static_cast<MovableObject*>(mComponentOwner)->getPosition();
}

void MovableObjectComponentTrackTarget::setPosition(const Vector3& pos)
{
	return static_cast<MovableObject*>(mComponentOwner)->setPosition(pos);
}

Vector3 MovableObjectComponentTrackTarget::getTargetPosition() const
{
	return (static_cast<MovableObject*>(mTarget))->getPosition() + mTargetOffset;
}

void MovableObjectComponentTrackTarget::onTargetDestroy(MovableObject* obj, void* userData)
{
	// 追踪目标被销毁了,则停止追踪
	static_cast<This*>(userData)->notifyBreak();
}