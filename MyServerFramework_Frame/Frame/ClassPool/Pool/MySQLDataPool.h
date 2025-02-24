#pragma once

#ifdef _MYSQL
#include "ClassBaseTypePoolThread.h"
#include "MySQLData.h"

class MICRO_LEGEND_FRAME_API MySQLDataPool : public ClassBaseTypePoolThread<MySQLData>
{
	BASE(MySQLDataPool, ClassBaseTypePoolThread<MySQLData>);
};
#endif