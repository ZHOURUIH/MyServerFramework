#pragma once

#include "ClassPooledObject.h"

class MICRO_LEGEND_FRAME_API LogInfo : public ClassPooledObject
{
	BASE(LogInfo, ClassPooledObject);
public:
	void resetProperty() override;
public:
	llong mPlayerGUID = 0;
	llong mTime = 0;
	string mLog;
	string mStack;
	bool mError = false;
};