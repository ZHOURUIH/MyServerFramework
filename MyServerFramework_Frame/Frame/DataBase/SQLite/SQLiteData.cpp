#include "FrameHeader.h"

SQLiteData::~SQLiteData()
{
	for (const auto& item : mParameters)
	{
		delete item.second;
	}
	mParameters.clear();
}

void SQLiteData::parse(SQLiteDataReader* reader)
{
	for (const auto& item : mParameters)
	{
		const int index = item.first;
		const SQLiteTableParamBase* param = item.second;
		if (param->mTypeHashCode == mIntType)
		{
			*static_cast<int*>(param->mPointer) = reader->getInt(index);
		}
		else if (param->mTypeHashCode == mUIntType)
		{
			*static_cast<uint*>(param->mPointer) = reader->getInt(index);
		}
		else if (param->mTypeHashCode == mBoolType)
		{
			*static_cast<bool*>(param->mPointer) = reader->getInt(index) != 0;
		}
		else if (param->mTypeHashCode == mCharType)
		{
			*static_cast<char*>(param->mPointer) = reader->getInt(index);
		}
		else if (param->mTypeHashCode == mByteType)
		{
			*static_cast<byte*>(param->mPointer) = reader->getInt(index);
		}
		else if (param->mTypeHashCode == mShortType)
		{
			*static_cast<short*>(param->mPointer) = reader->getInt(index);
		}
		else if (param->mTypeHashCode == mUShortType)
		{
			*static_cast<ushort*>(param->mPointer) = reader->getInt(index);
		}
		else if (param->mTypeHashCode == mFloatType)
		{
			*static_cast<float*>(param->mPointer) = reader->getFloat(index);
		}
		else if (param->mTypeHashCode == mLLongType)
		{
			*static_cast<llong*>(param->mPointer) = reader->getLLong(index);
		}
		else if (param->mTypeHashCode == mByteListType)
		{
			SToBs(reader->getString(index), *static_cast<Vector<byte>*>(param->mPointer));
		}
		else if (param->mTypeHashCode == mUShortListType)
		{
			SToUSs(reader->getString(index), *static_cast<Vector<ushort>*>(param->mPointer));
		}
		else if (param->mTypeHashCode == mIntListType)
		{
			SToIs(reader->getString(index), *static_cast<Vector<int>*>(param->mPointer));
		}
		else if (param->mTypeHashCode == mUIntListType)
		{
			SToUIs(reader->getString(index), *static_cast<Vector<uint>*>(param->mPointer));
		}
		else if (param->mTypeHashCode == mFloatListType)
		{
			SToFs(reader->getString(index), *static_cast<Vector<float>*>(param->mPointer));
		}
		else if (param->mTypeHashCode == mVector2IntType)
		{
			*static_cast<Vector2Int*>(param->mPointer) = SToV2I(reader->getString(index));
		}
		else if (param->mTypeHashCode == mVector2UIntType)
		{
			*static_cast<Vector2UInt*>(param->mPointer) = SToV2UI(reader->getString(index));
		}
		else if (param->mTypeHashCode == mVector2ShortType)
		{
			*static_cast<Vector2Short*>(param->mPointer) = SToV2S(reader->getString(index));
		}
		else if (param->mTypeHashCode == mVector2UShortType)
		{
			*static_cast<Vector2UShort*>(param->mPointer) = SToV2US(reader->getString(index));
		}
		else if (param->mTypeHashCode == mStringType)
		{
			*static_cast<string*>(param->mPointer) = reader->getString(index, false);
		}
		else if (param->mTypeHashCode == mVector2ListType)
		{
			SToV2s(reader->getString(index), *static_cast<Vector<Vector2>*>(param->mPointer), "|");
		}
		else if (param->mTypeHashCode == mVector2IntListType)
		{
			SToV2Is(reader->getString(index), *static_cast<Vector<Vector2Int>*>(param->mPointer), "|");
		}
		else if (param->mTypeHashCode == mVector3ListType)
		{
			SToV3s(reader->getString(index), *static_cast<Vector<Vector3>*>(param->mPointer), "|");
	}
		else if (param->mTypeHashCode == mVector3IntListType)
		{
			SToV3Is(reader->getString(index), *static_cast<Vector<Vector3Int>*>(param->mPointer), "|");
		}
		else
		{
#ifdef WINDOWS
			ERROR("SQLite参数类型错误:" + param->mTypeName);
#else
			ERROR("SQLite参数类型错误");
#endif
			continue;
		}
	}
}