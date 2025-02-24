#include "FrameHeader.h"

void MovableObjectComponentMove::applyTrembling(const float value)
{
	auto* obj = static_cast<MovableObject*>(mComponentOwner);
	obj->setPosition(lerpSimple(mStartPos, mTargetPos, value));
}

void MovableObjectComponentMove::resetProperty()
{
	base::resetProperty();
	mStartPos.clear();
	mTargetPos.clear();
}