#ifndef _MATH_UTILITY_H_
#define _MATH_UTILITY_H_

#include "StringUtility.h"

class MathUtility : public StringUtility
{
public:
	static void checkInt(float& value, float precision = MIN_DELTA); // 判断传入的参数是否已经接近于整数,如果接近于整数,则设置为整数
	static bool isFloatZero(float value, float precision = MIN_DELTA) { return value >= -precision && value <= precision; }
	static bool isFloatEqual(float value1, float value2, float precision = MIN_DELTA) { return isFloatZero(value1 - value2, precision); }
	// 是否为偶数
	static bool isEven(int value) { return (value & 1) == 0; }
	// 是否为2的n次幂
	static bool isPow2(int value) { return (value & (value - 1)) == 0; }
	// 得到比value大的第一个pow的n次方的数
	static uint getGreaterPowerValue(uint value, uint pow);
	// 得到比value大的第一个2的n次方的数
	static uint getGreaterPower2(uint value);
	static uint pow2(uint pow) { return 1 << pow; }
	static uint pow10(uint pow);
	// 得到数轴上浮点数右边的第一个整数,向上取整
	static int ceil(float value)
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
	static int ceil(const double& value)
	{
		int intValue = (int)(value);
		if (value >= 0.0f && value > (double)intValue + 0.0001)
		{
			++intValue;
		}
		return intValue;
	}
	// 得到数轴上浮点数左边的第一个整数,向下取整
	static int floor(float value)
	{
		int intValue = (int)(value);
		if (value < 0.0f && value < intValue - 0.0001f)
		{
			--intValue;
		}
		return intValue;
	}
	// 得到数轴上浮点数左边的第一个整数,向下取整
	static int floor(const double& value)
	{
		int intValue = (int)(value);
		if (value < 0.0f && value < (double)intValue - 0.0001)
		{
			--intValue;
		}
		return intValue;
	}
	static void saturate(float& value) { clamp(value, 0.0f, 1.0f); }
	// value1大于等于value0则返回1,否则返回0
	static int step(float value0, float value1) { return value1 >= value0 ? 1 : 0; }
	// 得到value0除以value1的余数
	static float fmod(float value0, float value1) { return value0 - value1 * (int)(value0 / value1); }
	// 返回value的小数部分
	static float frac(float value) { return value - (int)value; }
	// 帧换算成秒
	static float frameToSecond(int frame) { return frame * 0.0333f; }
	template<typename T>
	static void clamp(T& value, T minValue, T maxValue)
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
	static void clampMin(T& value, T minValue = (T)0)
	{
		if (value < minValue)
		{
			value = minValue;
		}
	}
	template<typename T>
	static void clampMax(T& value, T maxValue)
	{
		if (value > maxValue)
		{
			value = maxValue;
		}
	}
	// 根据一定几率随机返回true或false,probability范围为0到1
	static bool randomHit(float probability)
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
	// 根据一定几率随机返回true或false,实际几率为probability除以scale
	// 一般scale应该为100,1000或者10000,以确保随机几率正常,不能超过30000
	static bool randomHit(uint probability, uint scale)
	{
		if (probability <= 0)
		{
			return false;
		}
		if (probability >= scale)
		{
			return true;
		}
		return randomInt(0, scale - 1) < probability;
	}
	// 根据几率随机选择一个下标
	static uint randomHit(const myVector<float>& oddsList);
	static uint randomHit(const float* oddsList, uint count);
	static float randomFloat(float minFloat, float maxFloat) 
	{
		if (minFloat >= maxFloat)
		{
			return minFloat;
		}
		return ((rand() % (10000 + 1)) * 0.0001f) * (maxFloat - minFloat) + minFloat; 
	}
	static uint randomInt(uint minInt, uint maxInt)
	{
		if (minInt >= maxInt)
		{
			return minInt;
		}
		return rand() % (maxInt - minInt + 1) + minInt;
	}
	static void clampCycle(float& value, float min, float max, float cycle)
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
	static void clampCycle(float& value, float min, float max) { clampCycle(value, min, max, max - min); }
	static void clampRadian180(float& radianAngle) { clampCycle(radianAngle, -MATH_PI, MATH_PI); }
	static void clampDegree180(float& radianAngle) { clampCycle(radianAngle, -180.0f, 180.0f); }
	static void clampRadian360(float& radianAngle) { clampCycle(radianAngle, 0.0f, MATH_PI * 2.0f); }
	static void clampDegree360(float& radianAngle) { clampCycle(radianAngle, 0.0f, 360.0f); }
	static bool inFixedRange(int value, int range0, int range1) { return value >= range0 && value <= range1; }
	static bool inFixedRange(float value, float range0, float range1) { return value >= range0 && value <= range1; }
	static bool inRange(int value, int range0, int range1) { return value >= getMin(range0, range1) && value <= getMax(range0, range1); }
	static bool inRange(float value, float range0, float range1) { return value >= getMin(range0, range1) && value <= getMax(range0, range1); }
	template<typename T>
	static T getMin(T a, T b) { return a < b ? a : b; }
	template<typename T>
	static T getMax(T a, T b) { return a > b ? a : b; }
	template<typename T>
	static T lerpSimple(const T& start, const T& end, float t) { return start + (end - start) * t; }
	template<typename T>
	static T lerp(const T& start, const T& end, float t, T minAbsDelta = 0.0f)
	{
		clamp(t, 0.0f, 1.0f);
		T value = start + (end - start) * t;
		// 如果值已经在end的一定范围内了,则直接设置为end
		if (abs(value - end) <= minAbsDelta)
		{
			value = end;
		}
		return value;
	}
	static float inverseLerp(float a, float b, float value)
	{
		return (value - a) / (b - a);
	}
	// 将表达式str中的keyword替换为replaceValue,然后计算str的值,返回值表示str中是否有被替换的值,str只能是算术表达式
	static bool replaceKeywordAndCalculate(string& str, const string& keyword, int replaceValue, bool floatOrInt);
	// 将表达式str中的所有\\()包含的部分中的keyword替换为keyValue,并且计算包含的表达式,返回值表示str中是否有被替换的部分,str可以是任意表达式
	static bool replaceStringKeyword(string& str, const string& keyword, int keyValue, bool floatOrInt);
	static float powerFloat(float f, int p);
	static float calculateFloat(string str);	// 以浮点数的计算法则计算一个表达式,只支持加减乘除和括号
	static int calculateInt(string str);		// 以整数的计算法则计算一个表达式,支持取余,加减乘除和括号
	// 秒数转换为分数和秒数
	static void secondsToMinutesSeconds(uint seconds, uint& outMin, uint& outSec)
	{
		outMin = seconds / 60;
		outSec = seconds % 60;
	}
	static void secondsToHoursMinutesSeconds(uint seconds, uint& outHour, uint& outMin, uint& outSec)
	{
		outHour = seconds / (60 * 60);
		outMin = (seconds % (60 * 60)) / 60;
		outSec = seconds % 60;
	}
	static float speedToInterval(float speed) { return 0.0333f / speed; }
	static float intervalToSpeed(float interval) { return 0.0333f / interval; }
	template<typename T>
	static void swap(T& value0, T& value1)
	{
		T temp = value0;
		value0 = value1;
		value1 = temp;
	}
	template<typename T>
	static T sign(T value)
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
	static int round(float value)
	{
		if (value > 0.0f)
		{
			return (int)(value + 0.5f);
		}
		else
		{
			return (int)(value - 0.5f);
		}
	}
	static float HueToRGB(float v1, float v2, float vH);
	static uint findPointIndex(const myVector<float>& distanceListFromStart, float curDistance, uint startIndex, uint endIndex);
	template<typename T>
	static bool binarySearch(myVector<T>& list, int start, int end, const T& value)
	{
		// 没有可查找的值
		if (start >= end)
		{
			return list[start] == value;
		}
		int middle = (start + end) >> 1;
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
	template<typename T>
	void quickSort(myVector<T>& arr, int (*compare)(T&, T&), bool ascend = true)
	{
		quickSort(arr.data(), 0, arr.size() - 1, compare);
	}
	template<typename T>
	void quickSort(T* arr, int count, int (*compare)(T&, T&), bool ascend = true)
	{
		quickSort(arr, 0, count - 1, compare);
	}
protected:
	template<typename T>
	void quickSort(T* arr, int low, int high, int (*compare)(T&, T&), bool ascend = true)
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
			if (compare != NULL)
			{
				if (ascend)
				{
					// 升序
					// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
					// 然后将找到的两个值交换,确保小的在左边,大的在右边
					while (compare(arr[++i], key) < 0 && i != high) {}
					while (compare(arr[--j], key) > 0 && j != low) {}
				}
				else
				{
					// 降序
					// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
					// 然后将找到的两个值交换,确保大的在左边,小的在右边
					while (compare(arr[++i], key) > 0 && i != high) {}
					while (compare(arr[--j], key) < 0 && j != low) {}
				}
			}
			else
			{
				if (ascend)
				{
					// 升序
					// 从左向右找到一个比key大的值,从右向左找到一个比key小的值
					// 然后将找到的两个值交换,确保小的在左边,大的在右边
					while (arr[++i] < key && i != high) {}
					while (arr[--j] > key && j != low) {}
				}
				else
				{
					// 降序
					// 从左向右找到一个比key小的值,从右向左找到一个比key大的值
					// 然后将找到的两个值交换,确保大的在左边,小的在右边
					while (arr[++i] > key && i != high) {}
					while (arr[--j] < key && j != low) {}
				}
			}
			if (i >= j)
			{
				break;
			}
			// 交换i,j对应的值
			T temp0 = arr[i];
			arr[i] = arr[j];
			arr[j] = temp0;
		}
		// 中枢值与j对应值交换
		arr[low] = arr[j];
		arr[j] = key;
		quickSort(arr, low, j - 1, compare, ascend);
		quickSort(arr, j + 1, high, compare, ascend);
	}
protected:
	static void initGreaterPow2();
public:
	static array<uint, 513> mGreaterPow2;		// 存储比数值数值大的第一个2的次方数,比如获得比5大的第一个数,则是mGreaterPow2[5]
	static const float MATH_PI;
	static const float MIN_DELTA;
	static const float Deg2Rad;
	static const float Rad2Deg;
	static const float TWO_PI_DEGREE;
	static const float TWO_PI_RADIAN;
	static const float HALF_PI_DEGREE;
	static const float HALF_PI_RADIAN;
	static const float PI_DEGREE;
	static const float PI_RADIAN;
};

#endif