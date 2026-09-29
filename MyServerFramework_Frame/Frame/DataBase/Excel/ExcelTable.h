#pragma once

#include "ExcelTableBase.h"
#include "VectorPoolManager.h"
#include "SerializerRead.h"
#include "FileContent.h"

template<class T, typename TypeCheck = typename IsSubClassOf<ExcelData, T>::mType>
class ExcelTable : public ExcelTableBase
{
	BASE(ExcelTable, ExcelTableBase);
public:
	~ExcelTable() override
	{
		DELETE_MAP(mDataList);
	}
	virtual void init(const string& tableName)
	{
		mTableName = tableName;
		FileContent file;
		string path = FrameDefine::EXCEL_PATH + mTableName + ".bytes";
		if (!openBinaryFile(path, &file))
		{
			ERROR("加载表格文件失败:" + path);
			return;
		}
		// 解密
		decodeFile(file.mBuffer, file.mFileSize);
		SerializerRead reader(file.mBuffer, file.mFileSize);
		while (reader.getIndex() < reader.getBufferSize())
		{
			T* data = new T();
			data->read(&reader);
			mDataList.add(data->mID, data);
		}
		T::postLoadAll(this);
	}
	T* getData(const int id, const bool showError = true) const 
	{
		if (id <= 0)
		{
			return nullptr;
		}
		T* data = mDataList.get(id); 
		if (data == nullptr && showError)
		{
			ERROR("找不到数据, table:" + mTableName + ", ID:" + IToS(id));
		}
		return data;
	}
	bool hasData(const int id) const { return mDataList.contains(id); }
	const HashMap<int, T*>& getAllData() const { return mDataList; }
	void checkEnumResult(bool result, const char* varName, int dataID)
	{
		if (!result)
		{
			ERROR(string("enum value error,name:") + varName + " in " + mTableName + ", ID:" + IToS(dataID) + ", Table:" + getTableName());
		}
	}

	void checkData(const int checkID, const int dataID, ExcelTableBase* refTable)
	{
		if (checkID > 0 && !hasData(checkID))
		{
			ERROR("can not find item id:" + IToS(checkID) + " in " + mTableName + ", ref ID:" + IToS(dataID) + ", ref Table:" + refTable->getTableName());
		}
	}
	template<typename T0>
	void checkData(const Vector<T0>& checkIDList, const int dataID, ExcelTableBase* refTable)
	{
		for (const T0 id : checkIDList)
		{
			if (!hasData(id))
			{
				ERROR("can not find item id:" + IToS(id) + " in " + mTableName + ", ref ID:" + IToS(dataID) + ", ref Table:" + refTable->getTableName());
			}
		}
	}
	template<typename T0, int Length>
	void checkData(const ArrayList<Length, T0>& checkIDList, const int dataID, ExcelTableBase* refTable)
	{
		for (const T0 id : checkIDList)
		{
			if (!hasData(id))
			{
				ERROR("can not find item id:" + IToS(id) + " in " + mTableName + ", ref ID:" + IToS(dataID) + ", ref Table:" + refTable->getTableName());
			}
		}
	}
	template<typename T0, typename T1>
	void checkListPair(const Vector<T0>& list0, const Vector<T1>& list1, const int dataID)
	{
		if (list0.size() != list1.size())
		{
			ERROR(string("list pair size not match, table:") + mTableName + ", ref ID:" + IToS(dataID));
		}
	}
protected:
	void decodeFile(char* fileBuffer, const int bufferSize)
	{
		const string key = generateMD5("ASLD" + mTableName, true) + "23y35y983";
		int keyIndex = 0;
		FOR(bufferSize)
		{
			fileBuffer[i] = (byte)((fileBuffer[i] ^ (byte)key[keyIndex]) + ((i << 1) & 0xFF));
			if (++keyIndex >= (int)key.length())
			{
				keyIndex = 0;
			}
		}
	}
protected:
	HashMap<int, T*> mDataList;
};