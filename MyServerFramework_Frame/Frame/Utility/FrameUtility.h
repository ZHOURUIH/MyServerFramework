#pragma once

#include "SystemUtility.h"
#include "Set.h"
#include "Map.h"
#include "HashMap.h"
#include "Array.h"
#include "ArrayList.h"
#include "MyString.h"
#include "Vector.h"
#include "FrameCallback.h"
#include "TCPServerClient.h"
#include "TCPServerSystem.h"

namespace FrameUtility
{
	MICRO_LEGEND_FRAME_API extern llong mGUIDSeed;
	inline llong generateGUID() { return ++mGUIDSeed; }
	// 用于简化ClassPool相关定义和使用的代码
	template<typename T>
	T* CLASS()
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type");
		if constexpr (!hasNoPool)
		{
			return PoolOf<T>::type::getInstance()->newClass();
		}
		else
		{
			return nullptr;
		}
	}
	template<typename T>
	void CLASS(T*& obj)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type");
		if constexpr (!hasNoPool)
		{
			obj = PoolOf<T>::type::getInstance()->newClass();
		}
		else
		{
			obj = nullptr;
		}
	}
	template<typename T>
	void UN_CLASS(T*& obj)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			PoolOf<T>::type::getInstance()->destroyClass(obj);
		}
	}
	template<typename T>
	void UN_CLASS(Vector<T*>& objList)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			for (T* item : objList)
			{
				PoolOf<T>::type::getInstance()->destroyClass(item);
			}
			objList.clear();
		}
	}
	template<typename T>
	void UN_CLASS(SafeVector<T*>& objList)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			for (T* item : objList.getMainList())
			{
				PoolOf<T>::type::getInstance()->destroyClass(item);
			}
			objList.clear();
		}
	}
	template<typename T>
	void UN_CLASS(SafeFastDeepVector<T*>& objList)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			for (T* item : objList.getMainList())
			{
				PoolOf<T>::type::getInstance()->destroyClass(item);
			}
			objList.clear();
		}
	}
	template<typename T, int Length>
	void UN_CLASS(ArrayList<Length, T*>& objList)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			for (T* item : objList)
			{
				PoolOf<T>::type::getInstance()->destroyClass(item);
			}
			objList.clear();
		}
	}
	template<typename T, typename Key>
	void UN_CLASS(HashMap<Key, T*>& objList)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			for (auto& item : objList)
			{
				PoolOf<T>::type::getInstance()->destroyClass(item.second);
			}
			objList.clear();
		}
	}
	template<typename T, typename Key>
	void UN_CLASS(SafeHashMap<Key, T*>& objList)
	{
		constexpr bool hasNoPool = is_same<typename PoolOf<T>::type, void>::value;
		static_assert(!hasNoPool, "type has no pool, please add PoolOf<Type> of your ClassObject Type, or add CLASS_POOL(yourType) at the end of yourType file");
		if constexpr (!hasNoPool)
		{
			for (auto& item : objList.getMainList())
			{
				T* temp = item.second;
				PoolOf<T>::type::getInstance()->destroyClass(temp);
			}
			objList.clear();
		}
	}
	MICRO_LEGEND_FRAME_API bool tickTimerLoop(float& time, float elapsedTime, float interval);
	MICRO_LEGEND_FRAME_API bool tickTimerLoopEnsure(float& time, float elapsedTime, float interval);
	MICRO_LEGEND_FRAME_API bool tickTimerOnce(float& time, float elapsedTime);
	template<typename T>
	void setToVector(const Set<T>& valueSet, Vector<T>& valueVec)
	{
		valueVec.clearAndReserve(valueSet.size());
		for (const auto& iter : valueSet)
		{
			valueVec.add(iter);
		}
	}
	template<typename T>
	Vector<T> setToVector(const Set<T>& valueSet)
	{
		Vector<T> valueVec;
		valueVec.clearAndReserve(valueSet.size());
		for (const auto& iter : valueSet)
		{
			valueVec.add(iter);
		}
		return valueVec;
	}
	template<typename T, int Length>
	int setToArray(const Set<T>& set, Array<Length, T>& arr, const int startIndex)
	{
		int curCount = 0;
		int i = 0;
		for (const auto& iter : set)
		{
			if (i++ >= startIndex)
			{
				arr[curCount++] = iter;
				if (curCount >= Length)
				{
					break;
				}
			}
		}
		return curCount;
	}
	template<typename T, int Length>
	void setToArray(const Set<T>& set, ArrayList<Length, T>& arr, const int startIndex)
	{
		int i = 0;
		for (const auto& iter : set)
		{
			if (i++ >= startIndex && !arr.add(iter))
			{
				break;
			}
		}
	}
	template<typename Key, typename Value, int Length>
	int mapValueToArray(const Map<Key, Value>& map, Array<Length, Value>& arr, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= Length)
			{
				break;
			}
			arr[curDataCount++] = iter.second;
		}
		return curDataCount;
	}
	template<typename Key, typename Value, int Length>
	int hashMapValueToArray(const HashMap<Key, Value>& map, Array<Length, Value>& arr, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= Length)
			{
				break;
			}
			arr[curDataCount++] = iter.second;
		}
		return curDataCount;
	}
	template<typename Key, typename Value, int Length>
	void hashMapValueToArray(const HashMap<Key, Value>& map, ArrayList<Length, Value>& arr, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return;
		}
		arr.clear();
		int indexInMap = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (!arr.add(iter.second))
			{
				break;
			}
		}
	}
	template<typename Key, typename Value>
	int mapValueToList(const Map<Key, Value>& map, Value* valueList, const int maxCount, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= maxCount)
			{
				break;
			}
			valueList[curDataCount++] = iter.second;
		}
		return curDataCount;
	}
	template<typename Key, typename Value>
	int hashMapValueToList(const HashMap<Key, Value>& map, Value* valueList, const int maxCount, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= maxCount)
			{
				break;
			}
			valueList[curDataCount++] = iter.second;
		}
		return curDataCount;
	}
	template<typename Key, typename Value, int Length>
	int mapValueToArrayFilter(const Map<Key, Value>& map, Array<Length, Value>& valueList, const Value& exceptValue, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= Length)
			{
				break;
			}
			if (iter.second != exceptValue)
			{
				valueList[curDataCount++] = iter.second;
			}
		}
		return curDataCount;
	}
	template<typename Key, typename Value, int Length>
	int hashMapValueToArrayFilter(const HashMap<Key, Value>& map, Array<Length, Value>& valueList, const Value& exceptValue, const int startIndex = 0)
	{
		if (startIndex >= map.size())
		{
			return 0;
		}
		int indexInMap = 0;
		int curDataCount = 0;
		for (const auto& iter : map)
		{
			if (++indexInMap <= startIndex)
			{
				continue;
			}
			if (curDataCount >= Length)
			{
				break;
			}
			if (iter.second != exceptValue)
			{
				valueList[curDataCount++] = iter.second;
			}
		}
		return curDataCount;
	}
	template<typename Key, typename Value>
	void mapKeyToVector(const Map<Key, Value>& map, Vector<Key>& keyList)
	{
		if (map.isEmpty())
		{
			return;
		}
		keyList.clearAndReserve(map.size());
		for (const auto& iter : map)
		{
			keyList.add(iter.first);
		}
	}
	template<typename Key, typename Value>
	void hashMapKeyToVector(const HashMap<Key, Value>& map, Vector<Key>& keyList)
	{
		if (map.isEmpty())
		{
			return;
		}
		keyList.clearAndReserve(map.size());
		for (const auto& iter : map)
		{
			keyList.add(iter.first);
		}
	}
	template<typename Key, typename Value>
	Vector<Key> hashMapKeyToVector(const HashMap<Key, Value>& map)
	{
		Vector<Key> keyList;
		if (map.isEmpty())
		{
			return keyList;
		}
		keyList.clearAndReserve(map.size());
		for (const auto& iter : map)
		{
			keyList.add(iter.first);
		}
		return keyList;
	}
	template<typename Key, typename Value, int Length>
	void hashMapKeyToArrayList(const HashMap<Key, Value>& map, ArrayList<Length, Key>& keyList)
	{
		if (map.isEmpty())
		{
			return;
		}
		keyList.clear();
		for (const auto& iter : map)
		{
			keyList.add(iter.first);
		}
	}
	template<typename Key, typename Value, int Length>
	void hashMapKeyValueToArrayList(const HashMap<Key, Value>& map, ArrayList<Length, Key>& keyList, ArrayList<Length, Value>& valueList)
	{
		if (map.isEmpty())
		{
			return;
		}
		keyList.clear();
		valueList.clear();
		for (const auto& iter : map)
		{
			keyList.add(iter.first);
			valueList.add(iter.second);
		}
	}
	template<typename Key, typename Value>
	bool mapKeyToList(const Map<Key, Value>& map, Key* keyList, const int maxCount)
	{
		if (map.isEmpty())
		{
			return true;
		}
		int index = 0;
		for (const auto& iter : map)
		{
			if (index >= maxCount)
			{
				return false;
			}
			keyList[index++] = iter.first;
		}
		return true;
	}
	template<typename Key, typename Value>
	bool hashMapKeyToList(const HashMap<Key, Value>& map, Key* keyList, const int maxCount)
	{
		if (map.isEmpty())
		{
			return true;
		}
		int index = 0;
		for (const auto& iter : map)
		{
			if (index >= maxCount)
			{
				return false;
			}
			keyList[index++] = iter.first;
		}
		return true;
	}
	template<typename Key, typename Value>
	void mapValueToVector(const Map<Key, Value>& map, Vector<Value>& valueList)
	{
		if (map.isEmpty())
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.add(iter.second);
		}
	}
	template<typename Key, typename Value>
	void hashMapValueToVector(const HashMap<Key, Value>& map, Vector<Value>& valueList)
	{
		if (map.isEmpty())
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.add(iter.second);
		}
	}
	template<typename Key, typename Value>
	void hashMapToVector(const HashMap<Key, Value>& map, Vector<pair<Key, Value>>& valueList)
	{
		if (map.isEmpty())
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.add(make_pair(iter.first, iter.second));
		}
	}
	template<typename Key, typename Value>
	void mapValueToVectorFilter(const Map<Key, Value>& map, Vector<Value>& valueList, const Value& exceptValue)
	{
		if (map.isEmpty())
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.addNotEqual(iter.second, exceptValue);
		}
	}
	template<typename Key, typename Value>
	void hashMapValueToVectorFilter(const HashMap<Key, Value>& map, Vector<Value>& valueList, const Value& exceptValue)
	{
		if (map.isEmpty())
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.addNotEqual(iter.second, exceptValue);
		}
	}
	template<typename T>
	void vectorToSet(const Vector<T>& vec, Set<T>& set, bool append)
	{
		if (!append)
		{
			set.clear();
		}
		for (const T& item : vec)
		{
			set.add(item);
		}
	}
	template<typename T>
	void removeDumplicate(Vector<T>& vec)
	{
		if (vec.size() <= 1)
		{
			return;
		}
		Set<T> dataSet;
		vectorToSet(vec, dataSet, false);
		if (dataSet.size() != vec.size())
		{
			setToVector(dataSet, vec);
		}
	}
	// 适用于基础数据类型的列表
	template<typename T, typename TypeCheck = enable_if_t<IsPodOrPointerType<T>::mValue>>
	void getListDiffPod(const T* array0, const int count0, const T* array1, const int count1, Vector<T>& diffInArray0, Vector<T>& diffInArray1)
	{
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.setRange(array0, count0);
		diffInArray1.setRange(array1, count1);
		FOR_INVERSE(count0)
		{
			if (diffInArray1.remove(diffInArray0[i]))
			{
				diffInArray0.removeAt(i);
			}
		}
	}
	// 适用于基础数据类型的列表
	template<typename T, typename TypeCheck = enable_if_t<IsPodOrPointerType<T>::mValue>, int Length0, int Length1>
	void getListDiffPod(const T* array0, const int count0, const T* array1, const int count1, ArrayList<Length0, T>& diffInArray0, ArrayList<Length1, T>& diffInArray1)
	{
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.addRange(array0, count0);
		diffInArray1.addRange(array1, count1);
		FOR_INVERSE(count0)
		{
			if (diffInArray1.remove(diffInArray0[i]))
			{
				diffInArray0.removeAt(i);
			}
		}
	}
	template<typename T, typename TypeCheck = enable_if_t<IsPodOrPointerType<T>::mValue>, int Length0, int Length1, int Length2, int Length3>
	void getListDiffPod(const ArrayList<Length0, T>& array0, const ArrayList<Length1, T>& array1, ArrayList<Length2, T>& diffInArray0, ArrayList<Length3, T>& diffInArray1)
	{
		const int count0 = array0.size();
		const int count1 = array1.size();
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.addRange(array0.data(), count0);
		diffInArray1.addRange(array1.data(), count1);
		FOR_INVERSE(count0)
		{
			if (diffInArray1.remove(diffInArray0[i]))
			{
				diffInArray0.removeAt(i);
			}
		}
	}
	// 适用于非基础数据类型的列表
	template<typename T, typename TypeCheck = enable_if_t<IsNotPodAndPointerType<T>::mValue>>
	void getListDiff(const T* array0, const int count0, const T* array1, const int count1, Vector<T>& diffInArray0, Vector<T>& diffInArray1)
	{
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.setRangeCopy(array0, count0);
		diffInArray1.setRangeCopy(array1, count1);
		FOR_VECTOR_INVERSE(diffInArray0)
		{
			if (diffInArray1.remove(diffInArray0[i]))
			{
				diffInArray0.removeAt(i);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void sendPacketTypeTCP(ushort packetType, const string& name, TCPServerClient* client);
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<PacketTCP, T>::mType>
	void sendPacketTCP(T* packet, TCPServerClient* client)
	{
		if (client == nullptr)
		{
			return;
		}
		if constexpr (T::hasMember())
		{
			mTCPServerSystem->writePacket(packet);
			client->sendPacket(packet);
		}
		else
		{
			client->sendPacket(T::getStaticType(), T::getStaticPacketName());
		}
	}
	MICRO_LEGEND_FRAME_API void sendPacketUDP(PacketTCP* packet, TCPServerClient* client);
	MICRO_LEGEND_FRAME_API void sendPacketWebSocket(PacketWebSocket* packet, WebSocketServerClient* client);
	// 延迟执行一个lambda表达式,回调会在主线程中执行,0秒表示在下一帧执行
	MICRO_LEGEND_FRAME_API void delayCall(float time, const VoidCallback& func);
	MICRO_LEGEND_FRAME_API void delayCall(const VoidCallback& func);
	inline ushort generateCRC16(const char* buffer, int count) { return crc16(0x1F, buffer, count) ^ 0x123F; }
	template<typename T>
	ushort generateCRC16(T value) { return generateCRC16((char*)(&value), sizeof(T)); }
}

using namespace FrameUtility;