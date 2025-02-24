#pragma once

#include "ComponentKeyframe.h"
#include "FrameStringDefine.h"

class MICRO_LEGEND_FRAME_API MovableObjectComponentMove : public ComponentKeyframe, public IComponentModifyPosition
{
	BASE(MovableObjectComponentMove, ComponentKeyframe);
public:
	static ushort staticType() { return FrameStringDefine::MovableObjectComponentMove; }
	void setTargetPos(const Vector3& pos) { mTargetPos = pos; }
	void setStartPos(const Vector3& pos) { mStartPos = pos; }
	void resetProperty() override;
	void notifyBreak() override
	{
		setDoingCallback(nullptr, nullptr);
		setDoneCallback(nullptr, nullptr);
	}
protected:
	void applyTrembling(const float value) override;
protected:
	Vector3 mStartPos;
	Vector3 mTargetPos;
};