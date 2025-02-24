#pragma once

#include "StringUtility.h"
#include "Line2.h"
#include "Line3.h"
#include "ThreadLock.h"
#include "HashMap.h"
#include "Set.h"
#include "Vector.h"

namespace MathUtility
{
	MICRO_LEGEND_FRAME_API extern Array<513, int> mGreaterPow2;		// 存储比数值数值大的第一个2的次方数,比如获得比5大的第一个数,则是mGreaterPow2[5]
	MICRO_LEGEND_FRAME_API extern ThreadLock mGreaterPow2Lock;
	constexpr float MATH_PI = 3.1415926f;
	constexpr float MIN_DELTA = 0.00001f;
	constexpr float MIN_DELTA_DOUBLE = 0.000000001f;
	constexpr float Deg2Rad = 0.0174532924f;
	constexpr float Rad2Deg = 57.29578f;
	constexpr float TWO_PI_DEGREE = MATH_PI * Rad2Deg * 2.0f;
	constexpr float TWO_PI_RADIAN = MATH_PI * 2.0f;
	constexpr float HALF_PI_DEGREE = MATH_PI * Rad2Deg * 0.5f;
	constexpr float HALF_PI_RADIAN = MATH_PI * 0.5f;
	constexpr float PI_DEGREE = MATH_PI * Rad2Deg;
	constexpr float PI_RADIAN = MATH_PI;
	//------------------------------------------------------------------------------------------------------------------------------
	// private
	MICRO_LEGEND_FRAME_API void initGreaterPow2();
	// 不使用函数指针的方法,但是要求T重载了比较运算符
	// 递归版本的快速排序返回值是中枢值
	template<typename T>
	int quickSortNonRecursiveAscend(T* arr, const int low, const int high)
	{
		if (high <= low)
		{
			return -1;
		}
		int i = low;
		int j = high + 1;
		const T key = arr[low];
		// 升序
		while (true)
		{
			// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
			// 然后将找到的两个值交换,确保小的在左边,大的在右边
			while (arr[++i] < key && i != high) {}
			while (arr[--j] > key && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		return j;
	}
	template<typename T>
	int quickSortNonRecursiveDescend(T* arr, const int low, const int high)
	{
		if (high <= low)
		{
			return -1;
		}
		int i = low;
		int j = high + 1;
		const T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
			// 然后将找到的两个值交换,确保大的在左边,小的在右边
			while (arr[++i] > key && i != high) {}
			while (arr[--j] < key && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		return j;
	}
	// 使用函数指针的方式,但是此方式效率较低,因为调用函数指针的效率较低,无法进行内联
	template<typename T>
	int quickSortNonRecursiveAscend(T* arr, const int low, const int high, int (*compare)(T&, T&))
	{
		if (compare == nullptr)
		{
			return -1;
		}
		if (high <= low)
		{
			return -1;
		}
		int i = low;
		int j = high + 1;
		const T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
			// 然后将找到的两个值交换,确保小的在左边,大的在右边
			while (compare(arr[++i], key) < 0 && i != high) {}
			while (compare(arr[--j], key) > 0 && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		return j;
	}
	// 使用函数指针的方式,但是此方式效率较低,因为调用函数指针的效率较低,无法进行内联
	template<typename T>
	int quickSortNonRecursiveDescend(T* arr, const int low, const int high, int (*compare)(T&, T&))
	{
		if (compare == nullptr)
		{
			return -1;
		}
		if (high <= low)
		{
			return -1;
		}
		int i = low;
		int j = high + 1;
		const T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
			// 然后将找到的两个值交换,确保大的在左边,小的在右边
			while (compare(arr[++i], key) > 0 && i != high) {}
			while (compare(arr[--j], key) < 0 && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		return j;
	}
	// 不使用函数指针的方法,但是要求T重载了比较运算符
	template<typename T>
	void quickSortAscend(T* arr, const int low, const int high)
	{
		if (high <= low)
		{
			return;
		}
		int i = low;
		int j = high + 1;
		T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
			// 然后将找到的两个值交换,确保小的在左边,大的在右边
			while (arr[++i] < key && i != high) {}
			while (arr[--j] > key && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSortAscend(arr, low, j - 1);
		quickSortAscend(arr, j + 1, high);
	}
	// 不使用函数指针的方法,但是要求T重载了比较运算符
	template<typename T>
	void quickSortAscendPtr(T** arr, const int low, const int high)
	{
		if (high <= low)
		{
			return;
		}
		int i = low;
		int j = high + 1;
		T* key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
			// 然后将找到的两个值交换,确保小的在左边,大的在右边
			while (*(arr[++i]) < *key && i != high) {}
			while (*(arr[--j]) > *key && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			T* temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSortAscendPtr(arr, low, j - 1);
		quickSortAscendPtr(arr, j + 1, high);
	}
	// 不使用函数指针的方法,但是要求T重载了比较运算符
	template<typename T>
	void quickSortDescend(T* arr, const int low, const int high)
	{
		if (high <= low)
		{
			return;
		}
		int i = low;
		int j = high + 1;
		const T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
			// 然后将找到的两个值交换,确保大的在左边,小的在右边
			while (arr[++i] > key && i != high) {}
			while (arr[--j] < key && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSortDescend(arr, low, j - 1);
		quickSortDescend(arr, j + 1, high);
	}
	// 不使用函数指针的方法,但是要求T重载了比较运算符
	template<typename T>
	void quickSortDescendPtr(T** arr, const int low, const int high)
	{
		if (high <= low)
		{
			return;
		}
		int i = low;
		int j = high + 1;
		T* key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
			// 然后将找到的两个值交换,确保大的在左边,小的在右边
			while (*(arr[++i]) > *key && i != high) {}
			while (*(arr[--j]) < *key && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			T* temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSortDescendPtr(arr, low, j - 1);
		quickSortDescendPtr(arr, j + 1, high);
	}
	// 使用函数指针的方式,但是此方式效率较低,因为调用函数指针的效率较低,无法进行内联
	template<typename T>
	void quickSortAscend(T* arr, const int low, const int high, int (*compare)(T&, T&))
	{
		if (compare == nullptr)
		{
			return;
		}
		if (high <= low)
		{
			return;
		}
		int i = low;
		int j = high + 1;
		T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
			// 然后将找到的两个值交换,确保小的在左边,大的在右边
			while (compare(arr[++i], key) < 0 && i != high) {}
			while (compare(arr[--j], key) > 0 && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSortAscend(arr, low, j - 1, compare);
		quickSortAscend(arr, j + 1, high, compare);
	}
	// 使用函数指针的方式,但是此方式效率较低,因为调用函数指针的效率较低,无法进行内联
	template<typename T>
	void quickSortDescend(T* arr, const int low, const int high, int (*compare)(T&, T&))
	{
		if (compare == nullptr)
		{
			return;
		}
		if (high <= low)
		{
			return;
		}
		int i = low;
		int j = high + 1;
		T key = arr[low];
		while (true)
		{
			// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
			// 然后将找到的两个值交换,确保大的在左边,小的在右边
			while (compare(arr[++i], key) > 0 && i != high) {}
			while (compare(arr[--j], key) < 0 && j != low) {}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			const T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}

		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSortDescend(arr, low, j - 1, compare);
		quickSortDescend(arr, j + 1, high, compare);
	}
	//------------------------------------------------------------------------------------------------------------------------------
	// public
	Vector3 toVec3(Vector2Int vec2) { return { (float)vec2.x, (float)vec2.y, 0.0f }; }
	Vector3 toVec3(Vector2UInt vec2) { return { (float)vec2.x, (float)vec2.y, 0.0f }; }
	Vector3 toVec3(Vector2UShort vec2) { return { (float)vec2.x, (float)vec2.y, 0.0f }; }
	Vector3 toVec3(Vector2 vec2) { return { vec2.x, vec2.y, 0.0f }; }
	Vector2 toVec2(Vector3 vec3) { return { vec3.x, vec3.y }; }
	Vector2Int toVec2Int(Vector2 vec2) { return { (int)vec2.x, (int)vec2.y }; }
	Vector2Int toVec2Int(Vector3 vec3) { return { (int)vec3.x, (int)vec3.y }; }
	Vector2Int toVec2Int(Vector2UInt vec2) { return { (int)vec2.x, (int)vec2.y }; }
	Vector2Int toVec2Int(Vector2Short vec2) { return { vec2.x, vec2.y }; }
	Vector2Int toVec2Int(Vector2UShort vec2) { return { vec2.x, vec2.y }; }
	Vector2UInt toVec2UInt(Vector2 vec2) { return { (uint)vec2.x, (uint)vec2.y }; }
	Vector2UInt toVec2UInt(Vector3 vec3) { return { (uint)vec3.x, (uint)vec3.y }; }
	Vector2UInt toVec2UInt(Vector2Int vec2) { return { (uint)vec2.x, (uint)vec2.y }; }
	Vector2UInt toVec2UInt(Vector2Short vec2) { return { (uint)vec2.x, (uint)vec2.y }; }
	Vector2UInt toVec2UInt(Vector2UShort vec2) { return { vec2.x, vec2.y }; }
	MICRO_LEGEND_FRAME_API void checkInt(float& value, float precision = MIN_DELTA); // 判断传入的参数是否已经接近于整数,如果接近于整数,则设置为整数
	constexpr bool isZero(const float value, const float precision = MIN_DELTA) { return value >= -precision && value <= precision; }
	constexpr bool isZero(const double value, const double precision = MIN_DELTA_DOUBLE) { return value >= -precision && value <= precision; }
	constexpr bool isEqual(const float value1, const float value2, const float precision = MIN_DELTA) { return isZero(value1 - value2, precision); }
	constexpr int indexToX(int index, int width) { return index % width; }
	constexpr int indexToY(int index, int width) { return index / width; }
	Vector2Int indexToIntPos(int index, int width) { return { index % width, index / width }; }
	int intPosToIndex(Vector2Int pos, int width) { return pos.x + pos.y * width; }
	int intPosToIndex(Vector2Short pos, int width) { return pos.x + pos.y * width; }
	int intPosToIndex(Vector2UShort pos, int width) { return pos.x + pos.y * width; }
	int intPosToIndex(Vector2UInt pos, int width) { return pos.x + pos.y * width; }
	constexpr int intPosToIndex(int x, int y, int width) { return x + y * width; }
	// 是否为偶数
	// 对于a % b的计算,如果b为2的n次方,则a % b等效于a & (b - 1)
	constexpr bool isEven(const int value) { return (value & 1) == 0; }
	// 是否为2的n次幂
	constexpr bool isPow2(const int value) { return (value & (value - 1)) == 0; }
	constexpr float divide(float value0, float value1) { return isZero(value1) ? 0.0f : value0 / value1; }
	constexpr float divide(int value0, int value1) { return value1 == 0 ? 0.0f : (float)value0 / value1; }
	constexpr float divide(float value0, int value1) { return value1 == 0 ? 0.0f : value0 / value1; }
	constexpr float divide(int value0, float value1) { return isZero(value1) ? 0.0f : value0 / value1; }
	constexpr double divide(double value0, double value1) { return isZero(value1) ? 0.0 : value0 / value1; }
	constexpr int divideInt(int value0, int value1) { return value1 == 0 ? 0 : value0 / value1; }
	constexpr ullong divideInt(ullong value0, ullong value1) { return value1 == 0ull ? 0ull : value0 / value1; }
	// 计算出count个元素分为batch个批次时,批次的数量
	constexpr int generateBatchCount(const ullong count, const ullong batch)
	{
		const ullong batchCount = divideInt(count, batch);
		return (int)(batch * batchCount != count ? batchCount + 1 : batchCount);
	}
	// 计算出第batchIndex个批次的元素个数
	constexpr int generateBatchSize(const ullong count, const ullong batch, const int batchIndex)
	{
		if (batchIndex * batch >= count)
		{
			return 0;
		}
		int curCount = (int)(count - batchIndex * batch);
		return curCount > (int)batch ? (int)batch : curCount;
	}
	// 得到比value大的第一个pow的n次方的数
	constexpr int getGreaterPowerValue(int value, int pow);
	// 得到比value大的第一个2的n次方的数
	MICRO_LEGEND_FRAME_API int getGreaterPower2(int value);
	// 得到的一个2的pow次方的数
	constexpr int pow2(const int pow) { return 1 << pow; }
	// 得到一个10的pow次方的数
	constexpr int pow10(int pow) { return (int)POWER_INT_10[pow]; }
	constexpr llong pow10LLong(int pow) { return POWER_LLONG_10[pow]; }
	constexpr float inversePow10(int pow) { return INVERSE_POWER_INT_10[pow]; }
	constexpr double inversePow10LLong(int pow) { return INVERSE_POWER_LLONG_10[pow]; }
	// 得到数轴上浮点数右边的第一个整数,向上取整
	constexpr int ceiling(const float value)
	{
		int intValue = (int)(value);
		// 当差值极小时,认为两个值相等
		if (value >= 0.0f && value > intValue + 0.0001f)
		{
			++intValue;
		}
		return intValue;
	}
	// 得到数轴上浮点数右边的第一个整数,向上取整
	constexpr int ceiling(const double value)
	{
		int intValue = (int)(value);
		if (value >= 0.0f && value > (double)intValue + 0.000001)
		{
			++intValue;
		}
		return intValue;
	}
	// 得到数轴上浮点数左边的第一个整数,向下取整
	constexpr int floor(const float value)
	{
		int intValue = (int)(value);
		if (value < 0.0f && value < intValue - 0.0001f)
		{
			--intValue;
		}
		return intValue;
	}
	// 得到数轴上浮点数左边的第一个整数,向下取整
	constexpr int floor(const double value)
	{
		int intValue = (int)(value);
		if (value < 0.0f && value < (double)intValue - 0.000001)
		{
			--intValue;
		}
		return intValue;
	}
	template<typename T>
	constexpr T abs(const T value) { return value >= (T)0 ? value : -value; }
	constexpr float abs(const float value) { return value >= 0.0f ? value : -value; }
	constexpr void ceiling(Vector2& value)
	{
		value.x = (float)ceiling(value.x);
		value.y = (float)ceiling(value.y);
	}
	constexpr void ceiling(Vector3& value)
	{
		value.x = (float)ceiling(value.x);
		value.y = (float)ceiling(value.y);
		value.z = (float)ceiling(value.z);
	}
	template<typename T>
	constexpr void clampRef(T& value, const T minValue, const T maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
		else if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	constexpr T clamp(const T value, const T minValue, const T maxValue)
	{
		if (value > maxValue)
		{
			return maxValue;
		}
		else if (value < minValue)
		{
			return minValue;
		}
		return value;
	}
	template<typename T>
	constexpr void clampMinRef(T& value, const T minValue = (T)0)
	{
		if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	constexpr T clampMin(const T value, const T minValue = (T)0) { return value < minValue ? minValue : value; }
	template<typename T>
	constexpr T clampZero(const T value) { return value < (T)0 ? (T)0 : value; }
	template<typename T>
	constexpr void clampMaxRef(T& value, const T maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
	}
	template<typename T>
	constexpr T clampMax(const T value, const T maxValue) { return value > maxValue ? maxValue : value; }
	constexpr void saturate(float& value) { clampRef(value, 0.0f, 1.0f); }
	// value1大于等于value0则返回1,否则返回0
	constexpr int step(const float value0, const float value1) { return value1 >= value0 ? 1 : 0; }
	// 得到value0除以value1的余数
	constexpr float fmod(const float value0, const float value1) { return value0 - value1 * (int)divide(value0, value1); }
	// 返回value的小数部分
	constexpr float frac(const float value) { return value - (int)value; }
	// 帧换算成秒
	constexpr float frameToSecond(const int frame) { return frame * 0.0333f; }
	constexpr float getSquaredLength(const Vector2& vec) { return vec.x * vec.x + vec.y * vec.y; }
	constexpr float getSquaredLength(const Vector3& vec) { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z; }
	float getLength(const float x, const float y) { return sqrt(x * x + y * y); }
	float getLength(const float x, const float y, float z) { return sqrt(x * x + y * y + z * z); }
	float getLength(const Vector2& vec) { return sqrt(vec.x * vec.x + vec.y * vec.y); }
	float getLength(const Vector3& vec) { return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z); }
	MICRO_LEGEND_FRAME_API Vector3 normalize(const Vector3& value);
	MICRO_LEGEND_FRAME_API Vector2 normalize(const Vector2& value);
	// 此处计算结果与unity中的Vector3.Cross()一致
	Vector3 cross(const Vector3& v0, const Vector3& v1) { return { v1.y * v0.z - v0.y * v1.z, v1.x * v0.z - v0.x * v1.z, v1.x * v0.y - v0.x * v1.y }; }
	constexpr float dot(const Vector3& v0, const Vector3& v1) { return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z; }
	constexpr float dot(const Vector2& v0, const Vector2& v1) { return v0.x * v1.x + v0.y * v1.y; }
	constexpr float toDegree(float radian) { return radian * Rad2Deg; }
	constexpr float toRadian(float degree) { return degree * Deg2Rad; }
	constexpr bool lengthLess(const Vector2& vec, const float length) { return vec.x * vec.x + vec.y * vec.y < length* length; }
	constexpr bool lengthGreater(const Vector2& vec, const float length) { return vec.x * vec.x + vec.y * vec.y > length * length; }
	constexpr bool lengthLess(const Vector3& vec, const float length) { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z < length * length; }
	constexpr bool lengthGreater(const Vector3& vec, const float length) { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z > length * length; }
	constexpr bool isVectorEqual(const Vector3& vec0, const Vector3& vec1) { return isEqual(vec0.x, vec1.x) && isEqual(vec0.y, vec1.y) && isEqual(vec0.z, vec1.z); }
	constexpr bool isVectorEqual(const Vector2& vec0, const Vector2& vec1) { return isEqual(vec0.x, vec1.x) && isEqual(vec0.y, vec1.y); }
	constexpr bool isVectorEqual(const Vector3& vec0, const Vector3& vec1, float precision) { return isEqual(vec0.x, vec1.x, precision) && isEqual(vec0.y, vec1.y, precision) && isEqual(vec0.z, vec1.z, precision); }
	constexpr bool isVectorEqual(const Vector2& vec0, const Vector2& vec1, float precision) { return isEqual(vec0.x, vec1.x, precision) && isEqual(vec0.y, vec1.y, precision); }
	Vector3 resetY(const Vector3& vec) { return { vec.x, 0.0f, vec.z }; }
	Line3 resetY(const Line3& line) { return { {line.mStart.x, 0.0f, line.mStart.z},{line.mEnd.x, 0.0f, line.mEnd.z} }; }
	// 非线程安全
	MICRO_LEGEND_FRAME_API float randomFloat(float minFloat, float maxFloat);
	// 非线程安全,范围[minInt, maxInt]  
	MICRO_LEGEND_FRAME_API int randomInt(int minInt, int maxInt);
	// 非线程安全,根据一定几率随机返回true或false,probability范围为0到1
	bool randomHit(const float probability)
	{
		if (probability <= 0.0f)
		{
			return false;
		}
		if (probability >= 1.0f)
		{
			return true;
		}
		return randomFloat(0.0f, 1.0f) < probability;
	}
	// 非线程安全,根据一定几率随机返回true或false,实际几率为probability除以scale
	// 一般scale应该尽量大一些,比如1万,10万,100万,以确保随机几率正常
	bool randomHit(const int probability, const int scale)
	{
		if (probability == 0)
		{
			return false;
		}
		if (probability >= scale)
		{
			return true;
		}
		return randomInt(0, scale - 1) < probability;
	}
	// 非线程安全,返回值是随机选择的下标
	template<int Length>
	int randomHit(const Array<Length, float>& oddsList, int count = 0)
	{
		if (count == 0)
		{
			count = oddsList.size();
		}
		float max = 0.0f;
		FOR_I(count)
		{
			max += oddsList[i];
		}
		const float random = randomFloat(0.0f, max);
		float curValue = 0.0f;
		FOR_I(count)
		{
			curValue += oddsList[i];
			if (random <= curValue)
			{
				return i;
			}
		}
		return 0;
	}
	// 非线程安全,返回值是随机选择的下标
	template<int Length>
	int randomHit(const ArrayList<Length, float>& oddsList)
	{
		const int count = oddsList.size();
		float max = 0.0f;
		FOR_I(count)
		{
			max += oddsList[i];
		}
		const float random = randomFloat(0.0f, max);
		float curValue = 0.0f;
		FOR_I(count)
		{
			curValue += oddsList[i];
			if (random <= curValue)
			{
				return i;
			}
		}
		return 0;
	}
	// 非线程安全,根据几率随机选择一个下标
	MICRO_LEGEND_FRAME_API int randomHit(const Vector<float>& oddsList);
	// 非线程安全,second是权重
	template<typename T>
	T randomHit(const HashMap<T, int>& oddsList, const T& defaultValue)
	{
		int max = 0;
		for (const auto& iter0 : oddsList)
		{
			max += iter0.second;
		}
		const int random = randomInt(0, max);
		int curValue = 0;
		for (const auto& iter1 : oddsList)
		{
			curValue += iter1.second;
			if (random <= curValue)
			{
				return iter1.first;
			}
		}
		return defaultValue;
	}
	// 洗牌算法,非线程安全
	template<typename T>
	void randomOrder(T* list, const int count)
	{
		FOR_I(count)
		{
			// 随机数生成器，范围[i, count - 1]  
			const int rand = randomInt(i, count - 1);
			T temp = list[i];
			list[i] = list[rand];
			list[rand] = temp;
		}
	}
	// 洗牌算法,非线程安全
	template<int Length, typename T>
	void randomOrder(ArrayList<Length, T>& list)
	{
		const int count = list.size();
		FOR_I(count)
		{
			// 随机数生成器，范围[i, count - 1]  
			const int rand = randomInt(i, count - 1);
			T temp = list[i];
			list[i] = list[rand];
			list[rand] = temp;
		}
	}
	// 洗牌算法,非线程安全
	template<int Length, typename T>
	void randomOrder(Vector<T>& list)
	{
		const int count = list.size();
		FOR_I(count)
		{
			// 随机数生成器，范围[i, count - 1]  
			const int rand = randomInt(i, count - 1);
			T temp = list[i];
			list[i] = list[rand];
			list[rand] = temp;
		}
	}
	// 非线程安全,更快的随机选择多个,适用于count较大的情况,比如count大于1万,如果count较小,则可能比randomSelect更慢
	MICRO_LEGEND_FRAME_API void randomSelectQuick(int count, int selectCount, Vector<int>& selectIndexes);
	// 非线程安全
	MICRO_LEGEND_FRAME_API void randomSelect(int count, int selectCount, Vector<int>& selectIndexes, bool needSort = true);
	template<int Length>
	void randomSelect(int count, int selectCount, ArrayList<Length, int>& selectIndexes, bool needSort = true)
	{
		selectIndexes.clear();
		if (selectCount >= count)
		{
			FOR_I(count)
			{
				selectIndexes.add(i);
			}
			return;
		}

		Vector<int> indexList(count);
		FOR_I(count)
		{
			indexList.push_back(i);
		}
		FOR_I(selectCount)
		{
			// 随机数生成器，范围[i, count - 1]  
			const int randIndex = randomInt(i, count - 1);
			selectIndexes.add(indexList[randIndex]);
			// 由于随机的下标范围在不断缩小,所以已经不会在小于i的范围内随机,所以只需要将i下标上的值覆盖到已经选中的下标上即可
			indexList[randIndex] = indexList[i];
		}
		if (needSort)
		{
			quickSort(selectIndexes);
		}
	}
	// 非线程安全
	MICRO_LEGEND_FRAME_API void randomSelect(int count, int selectCount, Set<int>& selectIndexes);
	// 非线程安全,根据权重随机选择多个下标
	MICRO_LEGEND_FRAME_API void randomSelect(const Vector<int>& oddsList, int selectCount, Vector<int>& selectIndexes);
	// 根据权重随机选择一个下标
	MICRO_LEGEND_FRAME_API int randomSelect(const Vector<int>& oddsList);
	// 根据权重随机选择多个下标
	template<int Length0, int Length1>
	void randomSelect(const ArrayList<Length0, int>& oddsList, int selectCount, ArrayList<Length1, int>& selectIndexes)
	{
		const int allCount = oddsList.size();
		if (selectCount >= allCount)
		{
			selectIndexes.addRange(oddsList);
			return;
		}
		clampMaxRef(selectCount, selectIndexes.maxSize());
		int max = 0;
		FOR_I(allCount)
		{
			max += oddsList[i];
		}
		FOR_I(selectCount)
		{
			const int random = randomInt(0, max);
			int curValue = 0;
			FOR_J(allCount)
			{
				// 已经被选中的下标就需要排除掉
				if (selectIndexes.contains(j))
				{
					continue;
				}
				curValue += oddsList[j];
				if (random <= curValue)
				{
					selectIndexes.add(j);
					// 选出一个就去除此下标的权重
					max -= oddsList[j];
					break;
				}
			}
		}
	}
	// 根据权重随机选择一个下标
	template<int Length0>
	int randomSelect(const ArrayList<Length0, int>& oddsList)
	{
		int max = 0;
		FOR_I(oddsList.size())
		{
			max += oddsList[i];
		}
		const int random = randomInt(0, max);
		int curValue = 0;
		FOR_I(oddsList.size())
		{
			curValue += oddsList[i];
			if (random <= curValue)
			{
				return i;
			}
		}
		return -1;
	}
	// 非线程安全,从一个map中随机选择一个
	template<typename KEY, typename VALUE>
	VALUE* randomSelect(const HashMap<KEY, VALUE>& list)
	{
		const int count = list.size();
		if (count == 0)
		{
			return nullptr;
		}
		if (count == 1)
		{
			return &(list.begin()->second);
		}
		const int index = randomInt(0, count - 1);
		int curIndex = 0;
		for (const auto& iter : list)
		{
			if (curIndex++ == index)
			{
				return &(iter.second);
			}
		}
		return nullptr;
	}
	// 非线程安全,从一个map中随机选择多个
	template<typename KEY, typename VALUE>
	void randomSelect(const HashMap<KEY, VALUE>& list, const int selectCount, Vector<VALUE*>& selectResult)
	{
		const int count = list.size();
		if (count == 0 || selectCount == 0)
		{
			return;
		}
		if (selectCount >= count)
		{
			selectResult.reserve(count);
			for (const auto& item : list)
			{
				selectResult.push_back(item.second);
			}
		}
		Vector<int> temp;
		randomSelect(count, selectCount, temp);
		selectResult.reserve(selectCount);
		int indexInResult = 0;
		int curIndex = 0;
		for (const auto& iter : list)
		{
			if (++curIndex == temp[indexInResult])
			{
				selectResult.push_back(&(iter.second));
				if (++indexInResult >= selectCount)
				{
					break;
				}
			}
		}
	}
	// 非线程安全,从一个map中随机选择多个
	template<typename KEY, typename VALUE>
	Vector<VALUE*> randomSelect(const HashMap<KEY, VALUE>& list, const int selectCount)
	{
		Vector<VALUE*> selectResult;
		const int count = list.size();
		if (count == 0 || selectCount == 0)
		{
			return selectResult;
		}
		if (selectCount >= count)
		{
			selectResult.reserve(count);
			for (const auto& item : list)
			{
				selectResult.push_back(item.second);
			}
		}
		Vector<int> temp;
		randomSelect(count, selectCount, temp);
		selectResult.reserve(selectCount);
		int indexInResult = 0;
		int curIndex = 0;
		for (const auto& iter : list)
		{
			if (++curIndex == temp[indexInResult])
			{
				selectResult.push_back(&(iter.second));
				if (++indexInResult >= selectCount)
				{
					break;
				}
			}
		}
		return selectResult;
	}
	// 非线程安全,从一个map中随机选择多个,VALUE是一个指针类型的重载
	template<typename KEY, typename VALUE>
	void randomSelect(const HashMap<KEY, VALUE*>& list, const int selectCount, Vector<VALUE*>& selectResult)
	{
		int count = list.size();
		if (count == 0 || selectCount == 0)
		{
			return;
		}
		if (selectCount >= count)
		{
			selectResult.reserve(count);
			for (const auto& iter : list)
			{
				selectResult.push_back(iter.second);
			}
			return;
		}

		Vector<int> temp;
		randomSelect(count, selectCount, temp);
		selectResult.reserve(selectCount);
		int indexInResult = 0;
		int curIndex = 0;
		for (const auto& iter : list)
		{
			if (++curIndex == temp[indexInResult])
			{
				selectResult.push_back(iter.second);
				if (++indexInResult >= selectCount)
				{
					break;
				}
			}
		}
	}
	// 非线程安全,从一个map中随机选择多个,VALUE是一个指针类型的重载
	template<typename KEY, typename VALUE>
	Vector<VALUE*> randomSelect(const HashMap<KEY, VALUE*>& list, const int selectCount)
	{
		Vector<VALUE*> selectResult;
		int count = list.size();
		if (count == 0 || selectCount == 0)
		{
			return selectResult;
		}
		if (selectCount >= count)
		{
			selectResult.reserve(count);
			for (const auto& iter : list)
			{
				selectResult.push_back(iter.second);
			}
			return selectResult;
		}

		Vector<int> temp;
		randomSelect(count, selectCount, temp);
		selectResult.reserve(selectCount);
		int indexInResult = 0;
		int curIndex = 0;
		for (const auto& iter : list)
		{
			if (++curIndex == temp[indexInResult])
			{
				selectResult.push_back(iter.second);
				if (++indexInResult >= selectCount)
				{
					break;
				}
			}
		}
		return selectResult;
	}
	constexpr void clampCycle(float& value, const float min, const float max, const float cycle)
	{
		while (value < min)
		{
			value += cycle;
		}
		while (value > max)
		{
			value -= cycle;
		}
	}
	constexpr void clampCycle(float& value, const float min, const float max) { clampCycle(value, min, max, max - min); }
	constexpr void clampRadian180(float& radianAngle) { clampCycle(radianAngle, -MATH_PI, MATH_PI); }
	constexpr void clampDegree180(float& radianAngle) { clampCycle(radianAngle, -180.0f, 180.0f); }
	constexpr void clampRadian360(float& radianAngle) { clampCycle(radianAngle, 0.0f, MATH_PI * 2.0f); }
	constexpr void clampDegree360(float& radianAngle) { clampCycle(radianAngle, 0.0f, 360.0f); }
	template<typename T>
	constexpr T getMin(const T a, const T b) { return a < b ? a : b; }
	template<typename T>
	constexpr T getMax(const T a, const T b) { return a > b ? a : b; }
	constexpr bool inFixedRange(const int value, const int range0, const int range1) { return value >= range0 && value <= range1; }
	constexpr bool inFixedRange(const float value, const float range0, const float range1) { return value >= range0 && value <= range1; }
	constexpr bool inRange(const int value, const int range0, const int range1) { return value >= getMin(range0, range1) && value <= getMax(range0, range1); }
	constexpr bool inRange(const float value, const float range0, const float range1) { return value >= getMin(range0, range1) && value <= getMax(range0, range1); }
	constexpr bool inRange(const Vector2& value, const Vector2& range0, const Vector2& range1) { return inRange(value.x, range0.x, range1.x) && inRange(value.y, range0.y, range1.y); }
	constexpr bool inRange(const Vector3& value, const Vector3& range0, const Vector3& range1) { return inRange(value.x, range0.x, range1.x) && inRange(value.y, range0.y, range1.y) && inRange(value.z, range0.z, range1.z); }
	template<typename T>
	constexpr T lerpSimple(const T& start, const T& end, const float t) { return start + (end - start) * t; }
	constexpr float lerp(const float start, const float end, float t, const float minAbsDelta = 0.0f)
	{
		clampRef(t, 0.0f, 1.0f);
		float value = start + (end - start) * t;
		// 如果值已经在end的一定范围内了,则直接设置为end
		if (abs(value - end) <= minAbsDelta)
		{
			value = end;
		}
		return value;
	}
	constexpr int lerp(const int start, const int end, float t)
	{
		clampRef(t, 0.0f, 1.0f);
		return start + (int)((end - start) * t);
	}
	constexpr float inverseLerp(const float a, const float b, const float value) { return divide(value - a, b - a); }
	// 绕Y轴旋转向量
	void rotateVector3Ref(Vector3& vec, const Vector3& axis, float radian) { vec = Quaternion(axis, radian) * vec; }
	void rotateVector3AroundYRef(Vector3& vec, float radian) { rotateVector3Ref(vec, Vector3::UP, radian); }
	void rotateVector3AroundXRef(Vector3& vec, float radian) { rotateVector3Ref(vec, Vector3::RIGHT, radian); }
	void rotateVector3AroundZRef(Vector3& vec, float radian) { rotateVector3Ref(vec, Vector3::FORWARD, radian); }
	MICRO_LEGEND_FRAME_API Vector3 rotateVector3(const Vector3& vec, const Vector3& axis, float radian);
	Vector3 rotateVector3AroundY(const Vector3& vec, float radian) { return rotateVector3(vec, Vector3::UP, radian); }
	Vector3 rotateVector3AroundX(const Vector3& vec, float radian) { return rotateVector3(vec, Vector3::RIGHT, radian); }
	Vector3 rotateVector3AroundZ(const Vector3& vec, float radian) { return rotateVector3(vec, Vector3::BACK, radian); }
	float getAngleBetweenVector(const Vector3& vec1, const Vector3& vec2) { return acos(dot(normalize(vec1), normalize(vec2))); }
	float getAngleBetweenVector(const Vector2& vec1, const Vector2& vec2) { return acos(dot(normalize(vec1), normalize(vec2))); }
	MICRO_LEGEND_FRAME_API float getAngleFromVector2ToVector2(const Vector2& from, const Vector2& to, bool radian = true);
	// 求Z轴顺时针旋转一定角度后的向量,角度范围是-MATH_PI 到 MATH_PI
	MICRO_LEGEND_FRAME_API Vector3 getVectorFromAngle(float angle);
	// 将表达式str中的keyword替换为replaceValue,然后计算str的值,返回值表示str中是否有被替换的值,str只能是算术表达式
	MICRO_LEGEND_FRAME_API bool replaceKeywordAndCalculate(string& str, const string& keyword, int replaceValue, bool floatOrInt);
	// 将表达式str中的所有\\()包含的部分中的keyword替换为keyValue,并且计算包含的表达式,返回值表示str中是否有被替换的部分,str可以是任意表达式
	MICRO_LEGEND_FRAME_API bool replaceStringKeyword(string& str, const string& keyword, int keyValue, bool floatOrInt);
	constexpr float powerFloat(const float f, int p)
	{
		clampMinRef(p);
		float ret = 1.0f;
		while (p--)
		{
			ret *= f;
		}
		return ret;
	}
	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	MICRO_LEGEND_FRAME_API float calculateFloat(const string& str);
	// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	MICRO_LEGEND_FRAME_API int calculateInt(const string& str);
	// 秒数转换为分数和秒数
	constexpr void secondsToMinutesSeconds(const int seconds, int& outMin, int& outSec)
	{
		outMin = seconds / 60;
		outSec = seconds % 60;
	}
	constexpr void secondsToHoursMinutesSeconds(const int seconds, int& outHour, int& outMin, int& outSec)
	{
		outHour = seconds / (60 * 60);
		outMin = (seconds % (60 * 60)) / 60;
		outSec = seconds % 60;
	}
	constexpr float speedToInterval(const float speed) { return divide(0.0333f, speed); }
	constexpr float intervalToSpeed(const float interval) { return divide(0.0333f, interval); }
	template<typename T>
	constexpr void swap(T& value0, T& value1)
	{
		const T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	template<typename T>
	constexpr T sign(const T value)
	{
		if (value > (T)0)
		{
			return (T)1;
		}
		if (value < (T)0)
		{
			return (T)-1;
		}
		return (T)0;
	}
	// 四舍五入
	constexpr llong roundDouble(const double value) { return value > 0.0 ? (llong)(value + 0.5) : (llong)(value - 0.5); }
	constexpr int round(const float value) { return value > 0.0f ? (int)(value + 0.5f) : (int)(value - 0.5f); }
	constexpr void round(Vector3& value)
	{
		value.x = (float)round(value.x);
		value.y = (float)round(value.y);
		value.z = (float)round(value.z);
	}
	constexpr float HueToRGB(float v1, float v2, float vH);
	MICRO_LEGEND_FRAME_API int findPointIndex(const Vector<float>& distanceListFromStart, float curDistance, int startIndex, int endIndex);
	// 计算两条直线的交点,返回值表示两条直线是否相交
	MICRO_LEGEND_FRAME_API bool intersectLine2(const Line2& line0, const Line2& line1, Vector2& intersect);
	// 计算两条直线的交点,返回值表示两条直线是否相交,忽略Y轴
	MICRO_LEGEND_FRAME_API bool intersectLine3IgnoreY(const Line3& line0, const Line3& line1, Vector2& intersect);
	// k为斜率,也就是cotan(直线与y轴的夹角)
	MICRO_LEGEND_FRAME_API bool generateLineExpression(const Line2& line, float& k, float& b);
	// 计算两条线段的交点,返回值表示两条线段是否相交,checkEndPoint为是否判断两条线段的端点,为false表示即使端点重合也不认为线段相交
	MICRO_LEGEND_FRAME_API bool intersectLineSection(const Line2& line0, const Line2& line1, Vector2& intersect, bool checkEndPoint = false);
	// 计算两条线段的交点,返回值表示两条线段是否相交,checkEndPoint为是否判断两条线段的端点,为false表示即使端点重合也不认为线段相交,忽略Y轴
	MICRO_LEGEND_FRAME_API bool intersectLineSectionIgnoreY(const Line3& line0, const Line3& line1, Vector3& intersect, bool checkEndPoint = false);
	// 计算线段与三角形是否相交
	MICRO_LEGEND_FRAME_API bool intersectLineTriangle(const Line2& line, const Triangle2& triangle, TriangleIntersect& intersectResult, bool checkEndPoint = false);
	// 计算线段与三角形是否相交,忽略Y轴
	MICRO_LEGEND_FRAME_API bool intersectLineTriangleIgnoreY(const Line3& line, const Triangle3& triangle, TriangleIntersect3& intersectResult, bool checkEndPoint = false);
	// 当忽略端点重合,如果有端点重合，则判断为不相交
	// 线段与矩形是否相交,checkEndPoint为是否判断两条线段的端点,为false表示即使端点重合也不认为线段相交
	MICRO_LEGEND_FRAME_API bool intersect(const Line2& line, const Rect& rect, Vector2* intersectPtr = nullptr, bool checkEndPoint = false);
	// 三维的线段与矩形是否相交,只不过忽略Y轴,checkEndPoint为是否判断两条线段的端点,为false表示即使端点重合也不认为线段相交
	MICRO_LEGEND_FRAME_API bool intersectIgnoreY(const Line3& line, const Rect3& rect, Vector3* intersectPtr = nullptr, bool checkEndPoint = false);
	// 计算点到线的距离
	MICRO_LEGEND_FRAME_API float getDistanceToLine(const Vector2& point, const Line2& line);
	// 计算点到线的距离,忽略Y轴
	MICRO_LEGEND_FRAME_API float getDistanceToLineIgnoreY(const Vector3& point, const Line3& line);
	// 计算点在线上的投影
	MICRO_LEGEND_FRAME_API Vector2 getProjectPoint(const Vector2& point, const Line2& line);
	// 计算点在线上的投影,忽略Y轴
	MICRO_LEGEND_FRAME_API Vector3 getProjectPointIgnoreY(const Vector3& point, const Line3& line);
	// 计算一个向量在另一个向量上的投影
	MICRO_LEGEND_FRAME_API Vector2 getProjection(const Vector2& v1, const Vector2& v2);
	// 计算一个向量在另一个向量上的投影,忽略Y轴
	MICRO_LEGEND_FRAME_API Vector3 getProjectionIgnoreY(const Vector3& v1, const Vector3& v2);
	template<typename T>
	bool binarySearch(Vector<T>& list, const int start, const int end, const T& value)
	{
		// 没有可查找的值
		if (start >= end)
		{
			return list[start] == value;
		}
		const int middle = (start + end) >> 1;
		const T& middleValue = list[middle];
		// 中间值就是要查找的值
		if (middleValue == value)
		{
			return true;
		}
		// 要查找的值比中间值小,查找左边
		if (value < middleValue)
		{
			return binarySearch(list, start, middle - 1, value);
		}
		// 要查找的值比中间值大,查找右边
		else
		{
			return binarySearch(list, middle + 1, end, value);
		}
	}
	// 递归版本的快速排序,效率比std::sort高
	template<typename T>
	void quickSortPtr(Vector<T*>& arr, const bool ascend = true)
	{
		if (ascend)
		{
			quickSortAscendPtr(arr.data(), 0, arr.size() - 1);
		}
		else
		{
			quickSortDescendPtr(arr.data(), 0, arr.size() - 1);
		}
	}
	// 递归版本的快速排序,效率比std::sort高
	template<typename T>
	void quickSort(Vector<T>& arr, const bool ascend = true)
	{
		if (ascend)
		{
			quickSortAscend(arr.data(), 0, arr.size() - 1);
		}
		else
		{
			quickSortDescend(arr.data(), 0, arr.size() - 1);
		}
	}
	// 递归版本的快速排序,效率比std::sort高
	template<typename T, int Length>
	void quickSort(ArrayList<Length, T>& arr, const bool ascend = true)
	{
		if (ascend)
		{
			quickSortAscend(arr.data(), 0, arr.size() - 1);
		}
		else
		{
			quickSortDescend(arr.data(), 0, arr.size() - 1);
		}
	}
	template<typename T>
	void quickSort(T* arr, int count, const bool ascend = true)
	{
		if (ascend)
		{
			quickSortAscend(arr, 0, count - 1);
		}
		else
		{
			quickSortDescend(arr, 0, count - 1);
		}
	}
	template<typename T>
	void quickSort(Vector<T>& arr, int (*compare)(T&, T&), const bool ascend = true)
	{
		if (ascend)
		{
			quickSortAscend(arr.data(), 0, arr.size() - 1, compare);
		}
		else
		{
			quickSortDescend(arr.data(), 0, arr.size() - 1, compare);
		}
	}
	template<typename T>
	void quickSort(T* arr, int count, int (*compare)(T&, T&), const bool ascend = true)
	{
		if (ascend)
		{
			quickSortAscend(arr, 0, count - 1, compare);
		}
		else
		{
			quickSortDescend(arr, 0, count - 1, compare);
		}
	}
	template<typename T>
	void quickSortNonRecursive(Vector<T>& arr, const bool ascend = true)
	{
		quickSortNonRecursive(arr.data(), arr.size(), ascend);
	}
	// 非递归版本的快速排序,但是执行了大量的额外逻辑,所以效率比较低,比std::sort的效率低
	template<typename T>
	void quickSortNonRecursive(T* arr, const int count, const bool ascend = true)
	{
		Vector<int> startList;
		Vector<int> endList;
		Vector<int> lastStartList;
		Vector<int> lastEndList;
		lastStartList.push_back(0);
		lastEndList.push_back(count - 1);
		auto* curStartList = &lastStartList;
		auto* curEndList = &lastEndList;
		auto* lastStartPtr = &startList;
		auto* lastEndPtr = &endList;
		if (ascend)
		{
			while (true)
			{
				lastStartPtr->clear();
				lastEndPtr->clear();
				// 遍历每一个分段数据,将分段数据进行排序,然后返回获得中枢值,根据中枢值计算出下一次要分段的数据
				auto& temp = *curStartList;
				FOR_VECTOR(temp)
				{
					const int start = (*curStartList)[i];
					const int end = (*curEndList)[i];
					const int middle = quickSortNonRecursiveAscend(arr, start, end);
					if (middle < 0)
					{
						continue;
					}
					lastStartPtr->push_back(start, middle + 1);
					lastEndPtr->push_back(middle - 1, end);
				}
				if (startList.size() == 0)
				{
					break;
				}
				swap(curStartList, lastStartPtr);
				swap(curEndList, lastEndPtr);
			}
		}
		else
		{
			while (true)
			{
				lastStartPtr->clear();
				lastEndPtr->clear();
				// 遍历每一个分段数据,将分段数据进行排序,然后返回获得中枢值,根据中枢值计算出下一次要分段的数据
				auto& temp = *curStartList;
				FOR_VECTOR(temp)
				{
					const int start = (*curStartList)[i];
					const int end = (*curEndList)[i];
					const int middle = quickSortNonRecursiveDecend(arr, start, end);
					if (middle < 0)
					{
						continue;
					}
					lastStartPtr->push_back(start, middle + 1);
					lastEndPtr->push_back(middle - 1, end);
				}
				if (startList.size() == 0)
				{
					break;
				}
				swap(curStartList, lastStartPtr);
				swap(curEndList, lastEndPtr);
			}
		}
	}
	template<typename T>
	void quickSortNonRecursive(Vector<T>& arr, int (*compare)(T&, T&), const bool ascend = true)
	{
		quickSort(arr.data(), arr.size(), compare, ascend);
	}
	template<typename T>
	void quickSortNonRecursive(T* arr, int count, int (*compare)(T&, T&), const bool ascend = true)
	{
		Vector<int> startList;
		Vector<int> endList;
		Vector<int> lastStartList;
		Vector<int> lastEndList;
		lastStartList.push_back(0);
		lastEndList.push_back(count - 1);
		auto* curStartList = &lastStartList;
		auto* curEndList = &lastEndList;
		auto* lastStartPtr = &startList;
		auto* lastEndPtr = &endList;
		if (ascend)
		{
			while (true)
			{
				lastStartPtr->clear();
				lastEndPtr->clear();
				// 遍历每一个分段数据,将分段数据进行排序,然后返回获得中枢值,根据中枢值计算出下一次要分段的数据
				auto& temp = *curStartList;
				FOR_VECTOR(temp)
				{
					const int start = (*curStartList)[i];
					const int end = (*curEndList)[i];
					const int middle = quickSortNonRecursiveAscend(arr, start, end, compare);
					if (middle < 0)
					{
						continue;
					}
					lastStartPtr->push_back(start, middle + 1);
					lastEndPtr->push_back(middle - 1, end);
				}
				if (startList.size() == 0)
				{
					break;
				}
				swap(curStartList, lastStartPtr);
				swap(curEndList, lastEndPtr);
			}
		}
		else
		{
			while (true)
			{
				lastStartPtr->clear();
				lastEndPtr->clear();
				// 遍历每一个分段数据,将分段数据进行排序,然后返回获得中枢值,根据中枢值计算出下一次要分段的数据
				auto& temp = *curStartList;
				FOR_VECTOR(temp)
				{
					const int start = (*curStartList)[i];
					const int end = (*curEndList)[i];
					const int middle = quickSortNonRecursiveDecend(arr, start, end, compare);
					if (middle < 0)
					{
						continue;
					}
					lastStartPtr->push_back(start, middle + 1);
					lastEndPtr->push_back(middle - 1, end);
				}
				if (startList.size() == 0)
				{
					break;
				}
				swap(curStartList, lastStartPtr);
				swap(curEndList, lastEndPtr);
			}
		}
	}
}

using MathUtility::isZero;
using MathUtility::quickSort;
using MathUtility::clampZero;
using MathUtility::getMin;
using MathUtility::randomHit;
using MathUtility::randomInt;
using MathUtility::generateBatchCount;
using MathUtility::clampRef;
using MathUtility::clampMinRef;
using MathUtility::randomSelect;
using MathUtility::clampMax;
using MathUtility::lengthGreater;
using MathUtility::randomFloat;
using MathUtility::clampMin;
using MathUtility::inRange;
using MathUtility::clampMaxRef;
using MathUtility::getLength;
using MathUtility::lengthLess;
using MathUtility::isEqual;
using MathUtility::normalize;
using MathUtility::getMax;
using MathUtility::generateBatchSize;
using MathUtility::ceiling;
using MathUtility::inFixedRange;
using MathUtility::rotateVector3AroundX;
using MathUtility::rotateVector3AroundY;
using MathUtility::rotateVector3AroundZ;
using MathUtility::toRadian;
using MathUtility::toDegree;
using MathUtility::getAngleBetweenVector;
using MathUtility::resetY;
using MathUtility::indexToX;
using MathUtility::indexToY;
using MathUtility::indexToIntPos;
using MathUtility::intPosToIndex;
using MathUtility::intPosToIndex;
using MathUtility::divide;
using MathUtility::divideInt;
using MathUtility::toVec3;
using MathUtility::toVec2;
using MathUtility::toVec2Int;
using MathUtility::toVec2UInt;
using MathUtility::pow10;
using MathUtility::pow10LLong;
using MathUtility::inversePow10;
using MathUtility::inversePow10LLong;
using MathUtility::roundDouble;
using MathUtility::clamp;
using MathUtility::lerp;
using MathUtility::lerpSimple;
using MathUtility::randomOrder;
using MathUtility::quickSortPtr;
using MathUtility::isVectorEqual;
using MathUtility::getSquaredLength;
using MathUtility::getGreaterPower2;
using MathUtility::powerFloat;
using MathUtility::getAngleFromVector2ToVector2;
using MathUtility::clampRadian360;
using MathUtility::binarySearch;
using MathUtility::speedToInterval;
using MathUtility::findPointIndex;
using MathUtility::inFixedRange;
using MathUtility::randomSelectQuick;
using MathUtility::TWO_PI_RADIAN;
using MathUtility::HALF_PI_RADIAN;
using MathUtility::PI_RADIAN;