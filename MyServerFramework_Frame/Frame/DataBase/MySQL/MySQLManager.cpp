#ifdef _MYSQL
#include "FrameHeader.h"

void MySQLManager::init()
{
	connectDataBase();
}

void MySQLManager::lateInit()
{
	for (MySQLTable* table : mTableList)
	{
		table->lateInit();
	}
}

// 在finalInit中才会创建数据库线程,所以其他系统可以在finalInit之前进行同步访问数据库,在finalInit之后只能异步访问数据库
void MySQLManager::finalInit()
{
	if (mMySQL == nullptr)
	{
		return;
	}
	// 创建数据库线程,所有的对数据库的访问都在此线程执行
	mMySQLWriteThread = mThreadManager->createThread("MySQL", mysqlThread, this, onThreadPreCmd, onThreadEndCmd);
	mMySQLWriteThread->setBackground(false);
	mMySQLWriteThread->setCmdDebug(true);
	mMySQLWriteThread->setTime(50);
}

bool MySQLManager::connectDataBase()
{
	if (mMySQL != nullptr)
	{
		ERROR("mysql不能重复创建");
		return false;
	}
	mMySQL = mysql_init(nullptr);
	const string& mysqlHost = mFrameConfigSystem->getMySQLHost();
	const string& mysqlUser = mFrameConfigSystem->getMySQLUser();
	const string& mysqlPassword = mFrameConfigSystem->getMySQLPassword();
	const string& database = mFrameConfigSystem->getMySQLDataBase();
	const int mysqlPort = mFrameConfigSystem->getMySQLPortNumber();
	if (!mysql_real_connect(mMySQL, mysqlHost.c_str(), mysqlUser.c_str(), mysqlPassword.c_str(), database.c_str(), mysqlPort, nullptr, 0))
	{
		const char* error = mysql_error(mMySQL);
		ERROR("can not connect DataBase " + database + "!   " + error);
		destroyMySQL();
		return false;
	}
	// 设置编码格式
	if (mysql_query(mMySQL, "SET NAMES utf8mb4") != 0)
	{
		ERROR("set names error!   " + string(mysql_error(mMySQL)));
		return false;
	}
	for (MySQLTable* table : mTableList)
	{
		table->setMySQL(mMySQL);
		table->notifyDataBaseConnected();
	}
	return true;
}

void MySQLManager::quit()
{
	// 先销毁线程
	mThreadManager->destroyThread(mMySQLWriteThread);
	mMySQLWriteThread = nullptr;

	for (MySQLTable* table : mTableList)
	{
		delete table;
	}
	mTableList.clear();
	// 销毁MySQL
	destroyMySQL();
}

void MySQLManager::destroyMySQL()
{
	if (mMySQL == nullptr)
	{
		return;
	}
	mysql_close(mMySQL);
	mMySQL = nullptr;
}

void MySQLManager::onThreadPreCmd(CustomThread* thread)
{
	mMySQLManager->startTransaction(false);
}

void MySQLManager::onThreadEndCmd(CustomThread* thread)
{
	mMySQLManager->commit();
}

void MySQLManager::mysqlThread(CustomThread* thread)
{
	// 每10秒钟主动连接一次数据库,防止数据库闲置超时而主动关闭
	mMySQLManager->mCurTimeCount += thread->getFrameTime();
	if (mMySQLManager->mCurTimeCount >= mTimeOut)
	{
		if (mMySQLManager->mMySQL != nullptr)
		{
			const int ret = mysql_ping(mMySQLManager->mMySQL);
			if (ret != 0)
			{
				LOG("数据库异常,错误码:" + IToS(ret));
			}
		}
		mMySQLManager->mCurTimeCount = 0.0f;
	}
}

void MySQLManager::resetProperty()
{
	base::resetProperty();
	mTableList.clear();
	mMySQLWriteThread = nullptr;
	mMySQL = nullptr;
	mCurTimeCount = 0.0f;
}

void MySQLManager::startTransaction(const bool check) const
{
	if (mMySQL == nullptr)
	{
		return;
	}
	if (check && mMySQLWriteThread != nullptr)
	{
		ERROR("禁止在mysql线程启动后在外部开启事务");
		return;
	}
	if (mMySQLManager->mInTrasaction)
	{
		ERROR("已开启事务,无法再次开启");
		return;
	}
	mysql_query(mMySQL, "START TRANSACTION"); 
	mMySQLManager->mInTrasaction = true;
}

void MySQLManager::commit() const
{
	if (mMySQL == nullptr)
	{
		return;
	}
	if (!mMySQLManager->mInTrasaction)
	{
		ERROR("未开启事务,无法提交事务");
		return;
	}
	mysql_query(mMySQL, "COMMIT");
	mMySQLManager->mInTrasaction = false;
}

bool MySQLManager::checkReconnect(const char* str) const
{
	if (findString(str, "Lost connection to MySQL server during query") ||
		findString(str, "Commands out of sync; you can't run this command now"))
	{
		// 清空所有缓存
		for (MySQLTable* table : mTableList)
		{
			table->clearAllCache();
		}
		// 出现无法恢复的错误时重新连接
		mMySQLManager->destroyMySQL();
		mMySQLManager->connectDataBase();
		return true;
	}
	return false;
}

#endif