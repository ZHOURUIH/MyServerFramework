#include "FrameHeader.h"

void OT::MOVE(MovableObject* obj, const Vector3& start)
{
	if (obj == nullptr)
	{
		return;
	}
	auto* component = obj->getComponent<MovableObjectComponentMove>();
	if (component != nullptr && component->isActive())
	{
		component->setDoingCallback(nullptr, nullptr);
		component->setDoneCallback(nullptr, nullptr);
		component->setActive(false);
	}
	obj->setPosition(start);
}

void OT::MOVE_EX(MovableObject* obj, const int keyframe, const Vector3& startPos, const Vector3& targetPos, const float onceLength, const bool loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (obj == nullptr)
	{
		return;
	}
	
	auto* component = obj->getComponent<MovableObjectComponentMove>();
	if (component == nullptr)
	{
		component = obj->addComponent<MovableObjectComponentMove>(false);
	}
	// 停止其他移动组件
	obj->breakComponent<IComponentModifyPosition, MovableObjectComponentMove>();
	component->setDoingCallback(doingCallback, doingUserData);
	component->setDoneCallback(doneCallback, doneUserData);
	component->setActive(true);
	component->setTargetPos(targetPos);
	component->setStartPos(startPos);
	component->play(keyframe, loop, onceLength);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void OT::MOVE_CURVE(MovableObject* obj)
{
	if (obj == nullptr)
	{
		return;
	}
	auto* component = obj->getComponent<MovableObjectComponentMoveCurve>();
	if (component != nullptr && component->isActive())
	{
		component->setDoingCallback(nullptr, nullptr);
		component->setDoneCallback(nullptr, nullptr);
		component->setActive(false);
	}
}

void OT::MOVE_CURVE_EX(MovableObject* obj, const int keyframe, const Vector<Vector3>& posList, const float onceLength, const bool loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
{
	if (obj == nullptr)
	{
		return;
	}
	auto* component = obj->getComponent<MovableObjectComponentMoveCurve>();
	if (component == nullptr)
	{
		component = obj->addComponent<MovableObjectComponentMoveCurve>(false);
	}
	// 停止其他移动组件
	obj->breakComponent<IComponentModifyPosition, MovableObjectComponentMoveCurve>();
	component->setActive(true);
	component->setDoingCallback(doingCallback, doingUserData);
	component->setDoneCallback(doneCallback, doneUserData);
	component->setKeyPosList(posList);
	component->play(keyframe, loop, onceLength);
}

//------------------------------------------------------------------------------------------------------------------------------
void OT::TRACK_TARGET(MovableObject* obj)
{
	if (obj == nullptr)
	{
		return;
	}
	auto* component = obj->getComponent<MovableObjectComponentTrackTarget>();
	if (component != nullptr && component->isActive())
	{
		component->setActive(false);
		component->setMoveDoneTrack(nullptr, nullptr);
		component->setTrackingCallback(nullptr, nullptr);
		component->setTrackTarget(nullptr);
	}
}

void OT::TRACK_TARGET_EX(MovableObject* obj, MovableObject* target, const float speed, const Vector3& offset, TrackCallback doingCallback, void* doingUserData, TrackCallback doneCallback, void* doneUserData)
{
	if (obj == nullptr)
	{
		return;
	}
	auto* component = obj->getComponent<MovableObjectComponentTrackTarget>();
	if (component == nullptr)
	{
		component = obj->addComponent<MovableObjectComponentTrackTarget>(false);
	}
	// 停止其他移动组件
	obj->breakComponent<IComponentModifyPosition, MovableObjectComponentTrackTarget>();
	component->setActive(true);
	component->setSpeed(speed);
	component->setTargetOffset(offset);
	component->setMoveDoneTrack(doneCallback, doneUserData);
	component->setTrackingCallback(doingCallback, doingUserData);
	component->setTrackTarget(target);
}