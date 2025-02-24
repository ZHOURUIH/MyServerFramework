#pragma once

#include "ComponentKeyframe.h"
#include "FrameStringDefine.h"
#include "IComponentInterface.h"

class MICRO_LEGEND_FRAME_API MovableObjectComponentMoveCurve : public ComponentKeyframe, public IComponentModifyPosition
{
	BASE(MovableObjectComponentMoveCurve, ComponentKeyframe);
public:
	static ushort staticType() { return FrameStringDefine::MovableObjectComponentMoveCurve; }
	void setKeyPosList(const Vector<Vector3>& posList);
	void resetProperty() override;
	void notifyBreak() override;
protected:
	void applyTrembling(float value) override;
protected:
	Vector<float> mPointToStartLengthList;
	Vector<Vector3> mKeyPosList;   // 移动开始时的位置
	float mTotalLength = 0.0f;
};