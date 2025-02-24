#pragma once

#include "ClassPooledObject.h"
#include "TimeValue.h"

class MICRO_LEGEND_FRAME_API TimeTask : public ClassPooledObject
{
	BASE(TimeTask, ClassPooledObject);
public:
	void destroy();
	virtual void init(){}
	virtual void update(const float elapsedTime){}
	void resetProperty() override;
protected:
	TimePoint* mTimePoint = nullptr;
};