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
		mInTrasaction = false;
	}
	void init() override;
	void lateInit() override;
	void finalInit() override;
	void quit() override;
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLTable, T>::mType>
	T* registeTable(const char* name)
	{
		T* table = new T(name);
		table->init();
		mTableList.push_back(table);
		return table;
	}
	CustomThread* getThread() const { return mMySQLWriteThread; }
	void resetProperty() override;
	// 开始事务,如果是外部调用,则仅限于未开启mysql线程前调用
	void startTransaction(bool check = true) const;
	// 结束事务
	void commit() const;
	bool isInTransaction() const { return mInTrasaction; }
	bool connectDataBase();
	void destroyMySQL();
	bool checkReconnect(const char* str) const;
protected:
	static void onThreadPreCmd(CustomThread* thread);
	static void onThreadEndCmd(CustomThread* thread);
	static void mysqlThread(CustomThread* thread);
public:
	Vector<MySQLTable*> mTableList;
	CustomThread* mMySQLWriteThread = nullptr;
	MYSQL* mMySQL = nullptr;
	float mCurTimeCount = 0.0f;
	atomic<bool> mInTrasaction;
	static constexpr float mTimeOut = 60.0f;
};

#endif