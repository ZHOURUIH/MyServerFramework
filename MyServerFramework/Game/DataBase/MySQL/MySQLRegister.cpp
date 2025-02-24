#include "GameHeader.h"

void MySQLRegister::registeAll()
{
	mMySQLAccount = mMySQLManager->registeTable<MySQLAccount>("account");
}