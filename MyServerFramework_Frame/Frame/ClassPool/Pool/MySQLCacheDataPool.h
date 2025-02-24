#pragma once

#ifdef _MYSQL
#include "ClassPool.h"
#include "MySQLCacheData.h"

class MICRO_LEGEND_FRAME_API MySQLCacheDataPool : public ClassPoolThread<MySQLCacheData>
{
	BASE(MySQLCacheDataPool, ClassPoolThread<MySQLCacheData>);
};
#endif