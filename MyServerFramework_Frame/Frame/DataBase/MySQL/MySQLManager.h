#pragma once

#ifdef _MYSQL
#include "FrameComponent.h"
#include "MySQLDataPool.h"

class MICRO_LEGEND_FRAME_API MySQLManager : public FrameComponent
{
	BASE(MySQLManager, FrameComponent);
public:
	MySQLManager()
	{
		mInTransaction = false;
	}
	void init() override;
	void lateInit() override;
	void finalInit() override;
	void quit() override;
	void setToolMode(bool toolMode) { mToolMode = toolMode; }
	bool isToolMode() const { return mToolMode; }
	bool selectDatabase(const string& database);
	bool createDatabase(const string& database);
	bool dropDatabase(const string& database);
	bool executeToolSQL(const string& sql) const;
	void clearAllTableCache();
	void initializeCurrentDatabaseTables();
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLTable, T>::mType>
	T* registeTable(const char* name)
	{
		T* table = new T(name);
		table->init();
		mTableList.add(table);
		return table;
	}
	CustomThread* getThread() const { return mMySQLWriteThread; }
	void resetProperty() override;
	// 开始事务,如果是外部调用,则仅限于未开启mysql线程前调用
	void startTransaction(bool check = true) const;
	// 结束事务
	void commit() const;
	void rollback() const;
	bool isInTransaction() const { return mInTransaction; }
	bool connectDataBase();
	void destroyMySQL();
	bool checkReconnect(const char* str) const;
	static void backup();
protected:
	void onThreadPreCmd();
	void onThreadEndCmd();
	void mysqlThread();
public:
	Vector<MySQLTable*> mTableList;
	CustomThread* mMySQLWriteThread = nullptr;
	MYSQL* mMySQL = nullptr;
	float mCurTimeCount = 0.0f;
	atomic<bool> mInTransaction;
	string mCurrentDatabase;
	bool mToolMode = false;
	static constexpr float mTimeOut = 60.0f;
};

#endif