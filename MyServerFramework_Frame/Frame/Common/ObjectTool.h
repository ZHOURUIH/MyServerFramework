#pragma once

#include "FrameCallback.h"
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API OT
{
public:
	//--------------------------------------------------------------------------------------------------------------------------------------------
	// 直线运动
	static void MOVE(MovableObject* obj, const Vector3& start);
	static void MOVE(MovableObject* obj, const Vector3& start, const Vector3& target, const float onceLength)
	{
		MOVE_EX(obj, KEYFRAME::LINEAR_ZERO_ONE, start, target, onceLength, false, nullptr, nullptr, nullptr, nullptr);
	}
	static void MOVE_EX(MovableObject* obj, const Vector3& start, const Vector3& target, const float onceLength, KeyFrameCallback doneCallback, void* doneUserData)
	{
		MOVE_EX(obj, KEYFRAME::LINEAR_ZERO_ONE, start, target, onceLength, false, nullptr, nullptr, doneCallback, doneUserData);
	}
	static void MOVE_EX(MovableObject* obj, const Vector3& start, const Vector3& target, const float onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
	{
		MOVE_EX(obj, KEYFRAME::LINEAR_ZERO_ONE, start, target, onceLength, false, doingCallback, doingUserData, doneCallback, doneUserData);
	}
	static void MOVE(MovableObject* obj, int keyframeID, const Vector3& startPos, const Vector3& targetPos, const float onceLength)
	{
		MOVE_EX(obj, keyframeID, startPos, targetPos, onceLength, false, nullptr, nullptr, nullptr, nullptr);
	}
	static void MOVE(MovableObject* obj, int keyframeID, const Vector3& startPos, const Vector3& targetPos, const float onceLength, const bool loop)
	{
		MOVE_EX(obj, keyframeID, startPos, targetPos, onceLength, loop, nullptr, nullptr, nullptr, nullptr);
	}
	static void MOVE_EX(MovableObject* obj, int keyframeID, const Vector3& startPos, const Vector3& targetPos, const float onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
	{
		MOVE_EX(obj, keyframeID, startPos, targetPos, onceLength, false, doingCallback, doingUserData, doneCallback, doneUserData);
	}
	static void MOVE_EX(MovableObject* obj, int keyframeID, const Vector3& startPos, const Vector3& targetPos, const float onceLength, const bool loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallBack, void* doneUserData);
	//---------------------------------------------------------------------------------------------------------------------------------------------
	// 指定路线移动
	static void MOVE_CURVE(MovableObject* obj);
	static void MOVE_CURVE(MovableObject* obj, const Vector<Vector3>& posList, const float onceLength)
	{
		MOVE_CURVE_EX(obj, KEYFRAME::LINEAR_ZERO_ONE, posList, onceLength, false, nullptr, nullptr, nullptr, nullptr);
	}
	static void MOVE_CURVE_EX(MovableObject* obj, const Vector<Vector3>& posList, const float onceLength, KeyFrameCallback doneCallback, void* doneUserData)
	{
		MOVE_CURVE_EX(obj, KEYFRAME::LINEAR_ZERO_ONE, posList, onceLength, false, nullptr, nullptr, doneCallback, doneUserData);
	}
	static void MOVE_CURVE_EX(MovableObject* obj, const Vector<Vector3>& posList, const float onceLength, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData)
	{
		MOVE_CURVE_EX(obj, KEYFRAME::LINEAR_ZERO_ONE, posList, onceLength, false, doingCallback, doingUserData, doneCallback, doneUserData);
	}
	static void MOVE_CURVE(MovableObject* obj, const int keyframeID, const Vector<Vector3>& posList, const float onceLength)
	{
		MOVE_CURVE_EX(obj, keyframeID, posList, onceLength, false, nullptr, nullptr, nullptr, nullptr);
	}
	static void MOVE_CURVE(MovableObject* obj, const int keyframeID, const Vector<Vector3>& posList, const float onceLength, const bool loop)
	{
		MOVE_CURVE_EX(obj, keyframeID, posList, onceLength, loop, nullptr, nullptr, nullptr, nullptr);
	}
	static void MOVE_CURVE_EX(MovableObject* obj, const int keyframeID, const Vector<Vector3>& posList, const float onceLength, const bool loop, KeyFrameCallback doingCallback, void* doingUserData, KeyFrameCallback doneCallback, void* doneUserData);
	//---------------------------------------------------------------------------------------------------------------------------------------------
	// 追踪物体
	static void TRACK_TARGET(MovableObject* obj);
	static void TRACK_TARGET(MovableObject* obj, MovableObject* target, const float speed)
	{
		TRACK_TARGET_EX(obj, target, speed, Vector3::ZERO, nullptr, nullptr, nullptr, nullptr);
	}
	static void TRACK_TARGET_EX(MovableObject* obj, MovableObject* target, const float speed, TrackCallback doneCallback, void* doneUserData)
	{
		TRACK_TARGET_EX(obj, target, speed, Vector3::ZERO, nullptr, nullptr, doneCallback, doneUserData);
	}
	static void TRACK_TARGET(MovableObject* obj, MovableObject* target, const float speed, const Vector3& offset)
	{
		TRACK_TARGET_EX(obj, target, speed, offset, nullptr, nullptr, nullptr, nullptr);
	}
	static void TRACK_TARGET_EX(MovableObject* obj, MovableObject* target, const float speed, const Vector3& offset, TrackCallback doingCallback, void* doingUserData, TrackCallback doneCallback, void* doneUserData);
};