#pragma once

#include "PodPoolThread.h"
#include "FrameMySQLUtility.h"

// int类型的对象池
class IntPoolThread : public PodPoolThread<int>
{
	BASE(IntPoolThread, PodPoolThread<int>);
};