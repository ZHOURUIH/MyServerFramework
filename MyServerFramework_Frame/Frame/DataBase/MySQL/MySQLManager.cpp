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
	mMySQLWriteThread = mThreadManager->createThread("MySQL", [this] {mysqlThread(); }, [this] {onThreadPreCmd(); }, [this] {onThreadEndCmd(); });
	mMySQLWriteThread->setBackground(false);
	mMySQLWriteThread->setCmdDebug(true);
	mMySQLWriteThread->setTime(10);
}

bool MySQLManager::connectDataBase()
{
	if (mMySQL != nullptr)
	{
		ERROR("mysql不能重复创建");
		return false;
	}
	const string& mysqlHost = mFrameConfigSystem->getMySQLHost();
	const string& mysqlUser = mFrameConfigSystem->getMySQLUser();
	const string& mysqlPassword = mFrameConfigSystem->getMySQLPassword();
	const string& database = mFrameConfigSystem->getMySQLDataBase();
	if (!database.empty())
	{
		mMySQL = mysql_init(nullptr);
		const int mysqlPort = mFrameConfigSystem->getMySQLPortNumber();
		const char* connectDatabase = mToolMode ? nullptr : database.c_str();
		if (!mysql_real_connect(mMySQL, mysqlHost.c_str(), mysqlUser.c_str(), mysqlPassword.c_str(), connectDatabase, mysqlPort, nullptr, 0))
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
			if (!mToolMode)
			{
				table->notifyDataBaseConnected();
			}
		}
		mCurrentDatabase = mToolMode ? "" : database;
	}
	return true;
}

void MySQLManager::quit()
{
	// 先销毁线程
	mThreadManager->destroyThread(mMySQLWriteThread);
	DELETE_LIST(mTableList);
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

void MySQLManager::onThreadPreCmd()
{
	mMySQLManager->startTransaction(false);
}

void MySQLManager::onThreadEndCmd()
{
	mMySQLManager->commit();
}

void MySQLManager::mysqlThread()
{
	// 每10秒钟主动连接一次数据库,防止数据库闲置超时而主动关闭
	mMySQLManager->mCurTimeCount += mMySQLWriteThread->getFrameTime();
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
	mInTransaction = false;
	mCurrentDatabase.clear();
	mToolMode = false;
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
	if (mMySQLManager->mInTransaction)
	{
		ERROR("已开启事务,无法再次开启");
		return;
	}
	mysql_query(mMySQL, "START TRANSACTION");
	mMySQLManager->mInTransaction = true;
}

void MySQLManager::commit() const
{
	if (mMySQL == nullptr)
	{
		return;
	}
	if (!mMySQLManager->mInTransaction)
	{
		ERROR("未开启事务,无法提交事务");
		return;
	}
	mysql_query(mMySQL, "COMMIT");
	mMySQLManager->mInTransaction = false;
}

void MySQLManager::rollback() const
{
	if (mMySQL == nullptr || !mMySQLManager->mInTransaction)
	{
		return;
	}
	mysql_query(mMySQL, "ROLLBACK");
	mMySQLManager->mInTransaction = false;
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

void MySQLManager::clearAllTableCache()
{
	for (MySQLTable* table : mTableList)
	{
		table->clearAllCache();
	}
}

bool MySQLManager::selectDatabase(const string& database)
{
	if (!mToolMode)
	{
		ERROR("只有数据库工具模式允许主动切换数据库");
		return false;
	}
	if (mMySQL == nullptr || mMySQLWriteThread != nullptr || mInTransaction)
	{
		ERROR("当前MySQL状态不允许切换数据库");
		return false;
	}
	if (database.empty())
	{
		ERROR("数据库名为空");
		return false;
	}
	for (const char value : database)
	{
		if (!((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z')) && !(value >= '0' && value <= '9') && value != '_')
		{
			ERROR("数据库名包含非法字符:" + database);
			return false;
		}
	}
	if (mysql_select_db(mMySQL, database.c_str()) != 0)
	{
		ERROR("切换数据库失败:" + database + ",错误:" + mysql_error(mMySQL));
		return false;
	}
	mCurrentDatabase = database;
	clearAllTableCache();
	for (MySQLTable* table : mTableList)
	{
		table->setMaxGUID(0);
	}
	return true;
}

bool MySQLManager::executeToolSQL(const string& sql) const
{
	if (!mToolMode || mMySQL == nullptr || sql.empty())
	{
		return false;
	}
	if (mysql_real_query(mMySQL, sql.c_str(), (ulong)sql.length()) != 0)
	{
		ERROR("执行数据库工具SQL失败:" + string(mysql_error(mMySQL)) + ",SQL:" + sql);
		return false;
	}
	return true;
}

bool MySQLManager::createDatabase(const string& database)
{
	if (database.empty())
	{
		return false;
	}
	for (const char value : database)
	{
		if (!((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z')) && !(value >= '0' && value <= '9') && value != '_')
		{
			ERROR("数据库名包含非法字符:" + database);
			return false;
		}
	}
	return executeToolSQL("CREATE DATABASE `" + database + "` DEFAULT CHARACTER SET utf8mb4");
}

bool MySQLManager::dropDatabase(const string& database)
{
	if (database.empty())
	{
		return false;
	}
	for (const char value : database)
	{
		if (!((value >= 'A' && value <= 'Z') || (value >= 'a' && value <= 'z')) && !(value >= '0' && value <= '9') && value != '_')
		{
			ERROR("数据库名包含非法字符:" + database);
			return false;
		}
	}
	if (mCurrentDatabase == database)
	{
		mCurrentDatabase.clear();
	}
	return executeToolSQL("DROP DATABASE IF EXISTS `" + database + "`");
}

void MySQLManager::initializeCurrentDatabaseTables()
{
	if (!mToolMode || mCurrentDatabase.empty())
	{
		ERROR("当前没有可初始化的工具数据库");
		return;
	}
	for (MySQLTable* table : mTableList)
	{
		table->notifyDataBaseConnected();
		table->lateInit();
	}
}

void MySQLManager::backup()
{
	LOG("开始备份数据库");
	createDir(FrameDefine::MYSQL_BACKUP_PATH);
	const string backupFile = FrameDefine::MYSQL_BACKUP_PATH + "backup.sql";
	const string cmd = "mysqldump -u" + mFrameConfigSystem->getMySQLUser() + " -p" + mFrameConfigSystem->getMySQLPassword() + " " + mFrameConfigSystem->getMySQLDataBase() + " > " + backupFile;
	int ret = std::system(cmd.c_str());
	if (ret == 0)
	{
		LOG("备份数据库完成");
	}
	else
	{
		LOG("备份数据库失败!!");
	}
}

#endif