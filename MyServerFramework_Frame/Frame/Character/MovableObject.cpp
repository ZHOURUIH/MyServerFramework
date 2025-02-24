#include "FrameHeader.h"

MovableObject::~MovableObject()
{
	if (mDestroyCallbackList != nullptr)
	{
		delete mDestroyCallbackList;
		mDestroyCallbackList = nullptr;
	}
}

void MovableObject::destroy()
{
	base::destroy();
	if (mDestroyCallbackList != nullptr && mDestroyCallbackList->size() > 0)
	{
		for (const auto& pair : *mDestroyCallbackList)
		{
			pair.first(this, pair.second);
		}
		mDestroyCallbackList->clear();
	}
}

void MovableObject::resetProperty()
{
	base::resetProperty();
	if (mDestroyCallbackList != nullptr)
	{
		mDestroyCallbackList->clear();
	}
	mLastPosition.clear();
	mPosition.clear();
	mEnableLastPosition = false;
	mMovedDuringFrame = false;
	mHasLastPosition = false;
}

void MovableObject::update(const float elapsedTime)
{
	base::update(elapsedTime);
	if (mEnableLastPosition)
	{
		const Vector3& curPos = getPosition();
		mMovedDuringFrame = mHasLastPosition && !isVectorEqual(curPos, mLastPosition);
		mLastPosition = curPos;
		mHasLastPosition = true;
	}
}

void MovableObject::removeDestroyCallback(const OnObjectDestroy callback, const void* userData)
{
	if (mDestroyCallbackList == nullptr || mDestroyCallbackList->size() == 0)
	{
		return;
	}
	FOR_VECTOR(*mDestroyCallbackList)
	{
		const auto& value = (*mDestroyCallbackList)[i];
		if (value.first == callback && value.second == userData)
		{
			mDestroyCallbackList->eraseAt(i);
			break;
		}
	}
}