#pragma once

#include "ComponentTrackTarget.h"
#include "FrameStringDefine.h"

class MICRO_LEGEND_FRAME_API MovableObjectComponentTrackTarget : public ComponentTrackTarget
{
	BASE(MovableObjectComponentTrackTarget, ComponentTrackTarget);
public:
	static ushort staticType() { return FrameStringDefine::MovableObjectComponentTrackTarget; }
	void setTrackTarget(void* target) override;
	Vector3 getPosition() const override;
	void setPosition(const Vector3& pos) override;
	Vector3 getTargetPosition() const override;
protected:
	static void onTargetDestroy(MovableObject* obj, void* userData);
protected:
};