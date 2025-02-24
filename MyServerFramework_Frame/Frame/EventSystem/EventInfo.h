#pragma once

#include "ClassPooledObject.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API EventInfo : public ClassPooledObject
{
	BASE(EventInfo, ClassPooledObject);
public:
	void resetProperty() override
	{
		base::resetProperty();
		mType = 0;
		mCharacterGUID = 0;
		mEventProcess = nullptr;
		mListener = nullptr;
	}
public:
	int mType = 0;
	llong mCharacterGUID = 0;
	EventProcessBase* mEventProcess = nullptr;
	IEventListener* mListener = nullptr;
};