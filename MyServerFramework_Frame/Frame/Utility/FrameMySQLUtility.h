#pragma once

#ifdef _MYSQL
#include "FrameCallback.h"
#include "FrameDefine.h"
#include "CmdUtility.h"
#include "MySQLTableT.h"
#include "CommandThreadPool.h"
#include "FrameUtility.h"

namespace FrameMySQLUtility
{
	MICRO_LEGEND_FRAME_API void pushCommandToMySQL(VoidCallback call, const char* name);
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateLLong(MySQLTableT<T>* table, llong id, typename T::Column colName, llong value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->updateLLongT(id, colName, value);
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateLLongInt(MySQLTableT<T>* table, llong id, typename T::Column colName0, llong value0, typename T::Column colName1, int value1, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool result = table->updateLLongIntT(id, colName0, value0, colName1, value1);
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateBool(MySQLTableT<T>* table, llong id, typename T::Column colName, bool value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool result = table->updateBoolT(id, colName, value);
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateInt(MySQLTableT<T>* table, llong id, typename T::Column colName, int value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool result = table->updateIntT(id, colName, value); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateIntFloat(MySQLTableT<T>* table, llong id, typename T::Column colName0, int value0, typename T::Column colName1, float value1, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->updateIntFloatT(id, colName0, value0, colName1, value1);
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateInt2(MySQLTableT<T>* table, llong id, typename T::Column column0, int value0, typename T::Column column1, int value1, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool result = true;
			if (column0 != T::Column::NONE)
			{
				result &= table->updateIntT(id, column0, value0);
			}
			if (column1 != T::Column::NONE)
			{
				result &= table->updateIntT(id, column1, value1);
			}
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateFloat(MySQLTableT<T>* table, llong id, typename T::Column colName, float value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool result = table->updateFloatT(id, colName, value);
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateString(MySQLTableT<T>* table, llong id, typename T::Column colName, const string& value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{ 
			bool result = table->updateStringT(id, colName, value); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateString(MySQLTableT<T>* table, llong id, typename T::Column colName, string value, int length, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		if (length > 0 && length < (int)value.length())
		{
			value.erase(length);
		}
		pushCommandToMySQL([=]() 
		{
			bool result = table->updateStringT(id, colName, value); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateString2(MySQLTableT<T>* table, llong id, typename T::Column column0, const string& value0, typename T::Column column1, const string& value1, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool result = table->updateString2T(id, column0, value0, column1, value1); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateString(MySQLTableT<T>* table, llong id, typename T::Column colName, string&& value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=, value = move(value)]() 
		{
			bool result = table->updateStringT(id, colName, value); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateLLongList(MySQLTableT<T>* table, llong id, typename T::Column column, const Vector<llong>& value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->updateStringT(id, column, LLsToS(value)); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateAllInt(MySQLTableT<T>* table, typename T::Column column, int value, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->updateAllIntT(column, value); 
			if (callback != nullptr)
			{
				CALL(callback, result);
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryAll(MySQLTableT<T>* table, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto data = table->queryAllT();
			// 延迟到主线程执行回调通知
			delayCall([callback, data = move(data)]() { CALL(callback, data.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryCountByInt(MySQLTableT<T>* table, typename T::Column colName, int value, const char* cmdName, const LLongCallback& callback)
	{
		pushCommandToMySQL([=]() 
		{
			MyString<64> condition;
			sqlConditionLLong(condition, table->getColNameT(colName), value);
			const llong dataCount = table->queryCount(condition.str());

			// 延迟到主线程执行回调通知
			delayCall([=]() { CALL(callback, dataCount); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryCountByLLong(MySQLTableT<T>* table, typename T::Column colName, llong value, const char* cmdName, const LLongCallback& callback)
	{
		pushCommandToMySQL([=]() 
		{
			MyString<64> condition;
			sqlConditionLLong(condition, table->getColNameT(colName), value);
			const llong dataCount = table->queryCount(condition.str());

			// 延迟到主线程执行回调通知
			delayCall([=]() { CALL(callback, dataCount); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteByLLong2And(MySQLTableT<T>* table, typename T::Column colName0, llong value0, typename T::Column colName1, llong value1, bool onlyOnce, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->deleteByLLong2AndT(colName0, value0, colName1, value1, onlyOnce);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, result); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteByLLongIntAnd(MySQLTableT<T>* table, typename T::Column colName0, llong value0, typename T::Column colName1, int value1, bool onlyOnce, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->deleteByLLongIntAndT(colName0, value0, colName1, value1, onlyOnce);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, result); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteByLLong(MySQLTableT<T>* table, typename T::Column colName, llong value, bool onlyOnce, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			// 第0列固定为ID
			bool result = true;
			if (colName == T::Column::ID)
			{
				table->deleteByID(value);
			}
			else
			{
				result = table->deleteByLLongT(colName, value, onlyOnce);
			}
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, result); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteByString(MySQLTableT<T>* table, typename T::Column colName, const string& value, bool onlyOnce, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]()
		{
			bool result = table->deleteByStringT(colName, value, onlyOnce);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, result); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteByID(MySQLTableT<T>* table, llong id, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			bool success = table->deleteByID(id);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, success); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteByIDList(MySQLTableT<T>* table, Vector<llong>&& idList, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		if (idList.isEmpty())
		{
			return;
		}
		pushCommandToMySQL([table, idList = move(idList), callback]() mutable
		{
			// id去重
			removeDumplicate(idList);
			table->deleteByIDList(idList);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, true); });
			}
		}, cmdName);
	}
	template<typename T0, typename T1, typename TypeCheck0 = typename IsSubClassOf<MySQLData, T0>::mType, typename TypeCheck1 = typename IsSubClassOf<MySQLData, T1>::mType>
	void deleteCombineByID(MySQLTableT<T0>* table0, MySQLTableT<T1>* table1, llong id, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			table0->deleteByID(id);
			table1->deleteByID(id);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, true); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void deleteAll(MySQLTableT<T>* table, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() 
		{
			table->deleteAll(); 
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, true); });
			}
		}, cmdName);
	}
	// data需要是从对象池中创建的
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void insertData(MySQLTableT<T>* table, T* data, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() mutable
		{
			if (data->mID == 0)
			{
				data->mID = table->generateMaxGUID();
			}
			bool success = table->insertDataT(data, false);
			mMySQLDataPool->destroyClass(data);
			if (callback != nullptr)
			{
				delayCall([=]() {CALL(callback, success); });
			}
		}, cmdName);
	}
	// data需要是从对象池中创建的
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void insertOrUpdate(MySQLTableT<T>* table, T* data, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() mutable
		{
			if (data == nullptr || table == nullptr)
			{
				ERROR("Data或者Table为空!");
				if (callback != nullptr)
				{
					delayCall([=]() { CALL(callback, false); });
				}
				return;
			}
			if (data->isDestroy())
			{
				ERROR_PROFILE(("0数据已经被销毁,type:" + string(typeid(*data).name())).c_str());
				if (callback != nullptr)
				{
					delayCall([=]() { CALL(callback, false); });
				}
				return;
			}
			bool result = table->queryExistByID(data->mID)
				? (table->updateDataT(data, FrameDefine::FULL_MYSQL_FLAG), true)
				: table->insertDataT(data);
			mMySQLDataPool->destroyClass(data);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, result); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void updateData(MySQLTableT<T>* table, T* data, const char* cmdName, const BoolCallback& callback = nullptr)
	{
		pushCommandToMySQL([=]() mutable
		{
			bool result = table->updateDataT(data, FrameDefine::FULL_MYSQL_FLAG);
			mMySQLDataPool->destroyClass(data);
			if (callback != nullptr)
			{
				delayCall([=]() { CALL(callback, result); });
			}
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryByString(MySQLTableT<T>* table, typename T::Column column, const string& value, const char* cmdName, const QueryDataCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto data = table->queryByStringT(column, value);
			// 延迟到主线程执行回调通知
			delayCall([callback, data = move(data)]() { CALL(callback, data.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByString(MySQLTableT<T>* table, typename T::Column column, const string& value, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto dataList = table->queryListByStringT(column, value);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryByID(MySQLTableT<T>* table, llong id, const char* cmdName, const QueryDataCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto data = table->queryByIDT(id);
			// 延迟到主线程执行回调通知
			delayCall([callback, data = move(data)]() { CALL(callback, data.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryExistByString(MySQLTableT<T>* table, typename T::Column column, const string& value, const char* cmdName, const BoolCallback& callback)
	{
		pushCommandToMySQL([=]() 
		{
			const bool exist = table->queryExistByStringT(column, value);
			// 延迟到主线程执行回调通知
			delayCall([callback, exist]() { CALL(callback, exist); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByID(MySQLTableT<T>* table, Vector<llong>&& idList, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([table, valueList = move(idList), callback]() mutable
		{
			// 去重
			removeDumplicate(valueList);
			auto dataList = table->queryListByIDListT(valueList);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByID(MySQLTableT<T>* table, const Vector<llong>& idList, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([=, idList = Vector<llong>(idList)]() mutable
		{
			// 去重
			removeDumplicate(idList);
			auto dataList = table->queryListByIDListT(idList);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByInt(MySQLTableT<T>* table, typename T::Column column, int value, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto dataList = table->queryListByIntT(column, value, 0);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByLLong(MySQLTableT<T>* table, typename T::Column column, int maxCount, llong value, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto dataList = table->queryListByLLongT(column, value, maxCount);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByLLong(MySQLTableT<T>* table, typename T::Column column, llong value, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		pushCommandToMySQL([=]() 
		{
			auto dataList = table->queryListByLLongT(column, value, 0);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryListByStringList(MySQLTableT<T>* table, typename T::Column column, Vector<string>&& valueList, const char* cmdName, const QueryDataListCallbackT<T>& callback)
	{
		if (valueList.isEmpty())
		{
			CALL(callback, Vector<T*>::mDefaultList);
			return;
		}
		pushCommandToMySQL([table, column, valueList = move(valueList), callback]()
		{
			auto dataList = table->queryListByStringListT(column, valueList);
			// 延迟到主线程执行回调通知
			delayCall([callback, dataList = move(dataList)]() { CALL(callback, dataList.get()); });
		}, cmdName);
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<MySQLData, T>::mType>
	void queryCountByLLong2Or(MySQLTableT<T>* table, typename T::Column column0, typename T::Column column1, llong value0, llong value1, const char* cmdName, const LLongCallback& callback)
	{
		pushCommandToMySQL([=]()
		{
			MyString<128> condition;
			sqlConditionLLong(condition, table->getColNameT(column0), value0, "=", "||");
			sqlConditionLLong(condition, table->getColNameT(column1), value1);
			const llong dataCount = table->queryCount(condition.str());

			// 延迟到主线程执行回调通知
			delayCall([=]() { CALL(callback, dataCount); });
		}, cmdName);
	}
};
using namespace FrameMySQLUtility;
#endif