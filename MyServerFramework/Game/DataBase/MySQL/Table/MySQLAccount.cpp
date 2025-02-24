#include "GameHeader.h"

void MySQLAccount::lateInit()
{
	base::lateInit();
	executeNonQuery((string("ALTER TABLE ") + mTableName + " ADD INDEX Account(" + MDAccount::Name_Account + ")").c_str(), false, true);
}