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

namespace FrameUtility
{
	MICRO_LEGEND_FRAME_API bool tickTimerLoop(float& time, float elapsedTime, float interval, bool ensureInterval = false);
	MICRO_LEGEND_FRAME_API bool tickTimerOnce(float& time, float elapsedTime);
	template<typename T>
	void setToVector(const Set<T>& valueSet, Vector<T>& valueVec)
	{
		valueVec.clearAndReserve(valueSet.size());
		for (const auto& iter : valueSet)
		{
			valueVec.push_back(iter);
		}
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
		if (map.size() == 0)
		{
			return;
		}
		keyList.clearAndReserve(map.size());
		for (const auto& iter : map)
		{
			keyList.push_back(iter.first);
		}
	}
	template<typename Key, typename Value>
	void hashMapKeyToVector(const HashMap<Key, Value>& map, Vector<Key>& keyList)
	{
		if (map.size() == 0)
		{
			return;
		}
		keyList.clearAndReserve(map.size());
		for (const auto& iter : map)
		{
			keyList.push_back(iter.first);
		}
	}
	template<typename Key, typename Value, int Length>
	void hashMapKeyToArrayList(const HashMap<Key, Value>& map, ArrayList<Length, Key>& keyList)
	{
		if (map.size() == 0)
		{
			return;
		}
		keyList.clear();
		for (const auto& iter : map)
		{
			keyList.add(iter.first);
		}
	}
	template<typename Key, typename Value>
	bool mapKeyToList(const Map<Key, Value>& map, Value* keyList, const int maxCount)
	{
		if (map.size() == 0)
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
	bool hashMapKeyToList(const HashMap<Key, Value>& map, Value* keyList, const int maxCount)
	{
		if (map.size() == 0)
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
		if (map.size() == 0)
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.push_back(iter.second);
		}
	}
	template<typename Key, typename Value>
	void hashMapValueToVector(const HashMap<Key, Value>& map, Vector<Value>& valueList)
	{
		if (map.size() == 0)
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.push_back(iter.second);
		}
	}
	template<typename Key, typename Value>
	void hashMapToVector(const HashMap<Key, Value>& map, Vector<pair<Key, Value>>& valueList)
	{
		if (map.size() == 0)
		{
			return;
		}
		valueList.clearAndReserve(valueList.size() + map.size());
		for (const auto& iter : map)
		{
			valueList.push_back(make_pair(iter.first, iter.second));
		}
	}
	template<typename Key, typename Value>
	void mapValueToVectorFilter(const Map<Key, Value>& map, Vector<Value>& valueList, const Value& exceptValue)
	{
		if (map.size() == 0)
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
		if (map.size() == 0)
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
			set.insert(item);
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
	template<typename T, typename TypeCheck = typename IsPodOrPointerType<T>::mType>
	void getListDiffPod(const T* array0, const int count0, const T* array1, const int count1, Vector<T>& diffInArray0, Vector<T>& diffInArray1)
	{
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.setRange(array0, count0);
		diffInArray1.setRange(array1, count1);
		for (int i = count0 - 1; i >= 0; --i)
		{
			if (diffInArray1.eraseElement(diffInArray0[i]))
			{
				diffInArray0.eraseAt(i);
			}
		}
	}
	// 适用于基础数据类型的列表
	template<typename T, typename TypeCheck = typename IsPodOrPointerType<T>::mType, int Length0, int Length1>
	void getListDiffPod(const T* array0, const int count0, const T* array1, const int count1, ArrayList<Length0, T>& diffInArray0, ArrayList<Length1, T>& diffInArray1)
	{
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.addRange(array0, count0);
		diffInArray1.addRange(array1, count1);
		for (int i = count0 - 1; i >= 0; --i)
		{
			if (diffInArray1.eraseFirstElement(diffInArray0[i]))
			{
				diffInArray0.eraseAt(i);
			}
		}
	}
	// 适用于非基础数据类型的列表
	template<typename T, typename TypeCheck = typename IsNotPodAndPointerType<T>::mType>
	void getListDiff(const T* array0, const int count0, const T* array1, const int count1, Vector<T>& diffInArray0, Vector<T>& diffInArray1)
	{
		// 先复制一份,然后删除其中相同的元素,剩下的就是两个列表的差异元素
		diffInArray0.setRangeCopy(array0, count0);
		diffInArray1.setRangeCopy(array1, count1);
		for (int i = diffInArray0.size() - 1; i >= 0; --i)
		{
			if (diffInArray1.eraseElement(diffInArray0[i]))
			{
				diffInArray0.eraseAt(i);
			}
		}
	}
	MICRO_LEGEND_FRAME_API void sendPacketTCP(PacketTCP* packet, TCPServerClient* client);
	MICRO_LEGEND_FRAME_API void sendPacketTypeTCP(ushort packetType, const string& name, TCPServerClient* client);
	MICRO_LEGEND_FRAME_API void sendPacketUDP(PacketTCP* packet, TCPServerClient* client);
	MICRO_LEGEND_FRAME_API void sendPacketWebSocket(PacketWebSocket* packet, WebSocketServerClient* client);
	// 如果消息没有任何参数,则推荐使用此函数发送消息,避免不必要的逻辑执行
	template<typename T, 
		typename TypeCheck0 = typename IsSubClassOf<PacketTCP, T>::mType,
		typename TypeCheck1 = enable_if_t<!T::hasMember()>>
	void sendPacketTCP(TCPServerClient* client)
	{
		if (client != nullptr)
		{
			sendPacketTypeTCP(T::getStaticType(), T::getStaticPacketName(), client);
		}
	}
	// 延迟执行一个lambda表达式,回调会在主线程中执行,0秒表示在下一帧执行
	MICRO_LEGEND_FRAME_API void delayCall(const VoidCallback& func, float time = 0.0f);
	ushort generateCRC16(const char* buffer, int count) { return crc16(0x1F, buffer, count) ^ 0x123F; }
	template<typename T>
	ushort generateCRC16(T value) { return generateCRC16((char*)(&value), sizeof(T)); }
}

using namespace FrameUtility;