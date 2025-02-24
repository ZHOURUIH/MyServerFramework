#include "FrameHeader.h"

namespace MathUtility
{
	Array<513, int> mGreaterPow2;
	ThreadLock mGreaterPow2Lock;

	void initGreaterPow2()
	{
		// double check 防止在多线程执行时多次重复初始化
		if (mGreaterPow2[0] == 0)
		{
			THREAD_LOCK(mGreaterPow2Lock);
			if (mGreaterPow2[0] != 0)
			{
				return;
			}
			FOR_I(mGreaterPow2.size())
			{
				mGreaterPow2[i] = i > 1 ? getGreaterPowerValue(i, 2) : 2;
			}
		}
	}

	void checkInt(float& value, const float precision)
	{
		// 先判断是否为0
		if (isZero(value, precision))
		{
			value = 0.0f;
			return;
		}
		const int intValue = (int)value;
		// 大于0
		if (value > 0.0f)
		{
			// 如果原值减去整数值小于0.5f,则表示原值可能接近于整数值
			if (value - (float)intValue < 0.5f)
			{
				if (isZero(value - intValue, precision))
				{
					value = (float)intValue;
				}
			}
			// 如果原值减去整数值大于0.5f, 则表示原值可能接近于整数值+1
			else
			{
				if (isZero(value - (intValue + 1), precision))
				{
					value = (float)(intValue + 1);
				}
			}
		}
		// 小于0
		else if (value < 0.0f)
		{
			// 如果原值减去整数值的结果的绝对值小于0.5f,则表示原值可能接近于整数值
			if (abs(value - (float)intValue) < 0.5f)
			{
				if (isZero(value - intValue, precision))
				{
					value = (float)intValue;
				}
			}
			else
			{
				// 如果原值减去整数值的结果的绝对值大于0.5f, 则表示原值可能接近于整数值-1
				if (isZero(value - (intValue - 1), precision))
				{
					value = (float)(intValue - 1);
				}
			}
		}
	}

	constexpr int getGreaterPowerValue(const int value, const int pow)
	{
		int powValue = 1;
		FOR_I(30)
		{
			if (powValue >= value)
			{
				break;
			}
			powValue *= pow;
		}
		return powValue;
	}

	int getGreaterPower2(const int value)
	{
		if (mGreaterPow2[0] == 0)
		{
			initGreaterPow2();
		}
		if (value < mGreaterPow2.size())
		{
			return mGreaterPow2[value];
		}
		if (isPow2(value))
		{
			return value;
		}
		// 由于2的9次方以下都可以通过查表的方式获得,所以此处直接从10次方开始
		// 分2个档位,2的15次方,这样处理更快一些,比二分查找或顺序查找都快
		constexpr int Level0 = 15;
		if (value < (1 << Level0))
		{
			for (int i = 10; i <= Level0; ++i)
			{
				if ((1 << i) >= value)
				{
					return 1 << i;
				}
			}
		}
		else
		{
			for (int i = Level0 + 1; i < 32; ++i)
			{
				if ((1 << i) >= value)
				{
					return 1 << i;
				}
			}
		}
		ERROR("无法获取大于指定数的第一个2的n次方的数:" + IToS(value));
		return 0;
	}

	// 根据几率随机选择一个下标
	int randomHit(const Vector<float>& oddsList)
	{
		float max = 0.0f;
		for (const float value : oddsList)
		{
			max += value;
		}
		const float random = randomFloat(0.0f, max);
		float curValue = 0.0f;
		FOR_I(oddsList.size())
		{
			curValue += oddsList[i];
			if (random <= curValue)
			{
				return i;
			}
		}
		return 0;
	}

	float randomFloat(const float minFloat, const float maxFloat)
	{
		if (minFloat >= maxFloat)
		{
			return minFloat;
		}
		const int randValue = ((rand() & 0x7FFF) << 15) + (rand() & 0x7FFF);
		return (float)((randValue % 100001) * 0.00001 * ((double)maxFloat - (double)minFloat) + (double)minFloat);
	}

	int randomInt(const int minInt, const int maxInt)
	{
		if (minInt >= maxInt)
		{
			return minInt;
		}
		const int randValue = ((rand() & 0x7FFF) << 15) + (rand() & 0x7FFF);
		return randValue % (maxInt - minInt + 1) + minInt;
	}

	void randomSelectQuick(const int count, const int selectCount, Vector<int>& selectIndexes)
	{
		selectIndexes.clear();
		if (selectCount >= count)
		{
			selectIndexes.reserve(count);
			FOR_I(count)
			{
				selectIndexes.push_back(i);
			}
			return;
		}

		// 对应下标上的最新的值,用于代替交换下标的逻辑
		HashMap<int, int> replacedList;
		selectIndexes.reserve(selectCount);
		FOR_I(selectCount)
		{
			// 随机范围下限每次递减1
			const int randIndex = randomInt(i, count - 1);
			int* ptr = replacedList.getPtr(randIndex);
			int randValue = randIndex;
			// 替换后的值也可能是之前替换过的,所以还是要查询一下指定下标上当前的值
			const int oldValue = replacedList.tryGet(i, i);
			if (ptr != nullptr)
			{
				randValue = *ptr;
				*ptr = oldValue;
			}
			else
			{
				replacedList.insert(randIndex, oldValue);
			}
			selectIndexes.push_back(randValue);
		}
	}

	void randomSelect(const int count, const int selectCount, Vector<int>& selectIndexes, bool needSort)
	{
		selectIndexes.clear();
		if (selectCount >= count)
		{
			selectIndexes.reserve(count);
			FOR_I(count)
			{
				selectIndexes.push_back(i);
			}
			return;
		}

		Vector<int> indexList(count);
		FOR_I(count)
		{
			indexList.push_back(i);
		}
		selectIndexes.reserve(selectCount);
		FOR_I(selectCount)
		{
			// 随机数生成器，范围[i, count - 1]  
			const int randIndex = randomInt(i, count - 1);
			selectIndexes.push_back(indexList[randIndex]);
			// 由于随机的下标范围在不断缩小,所以已经不会在小于i的范围内随机,所以只需要将i下标上的值覆盖到已经选中的下标上即可
			indexList[randIndex] = indexList[i];
		}
		if (needSort)
		{
			quickSort(selectIndexes);
		}
	}

	void randomSelect(const int count, const int selectCount, Set<int>& selectIndexes)
	{
		selectIndexes.clear();
		if (selectCount >= count)
		{
			FOR_I(count)
			{
				selectIndexes.insert(i);
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
			selectIndexes.insert(indexList[randIndex]);
			// 由于随机的下标范围在不断缩小,所以已经不会在小于i的范围内随机,所以只需要将i下标上的值覆盖到已经选中的下标上即可
			indexList[randIndex] = indexList[i];
		}
	}

	void randomSelect(const Vector<int>& oddsList, const int selectCount, Vector<int>& selectIndexes)
	{
		const int allCount = oddsList.size();
		selectIndexes.clear();
		if (selectCount >= allCount)
		{
			selectIndexes.reserve(allCount);
			FOR_I(allCount)
			{
				selectIndexes.push_back(i);
			}
			return;
		}

		int max = 0;
		for (const int value : oddsList)
		{
			max += value;
		}

		selectIndexes.reserve(selectCount);
		FOR_I(selectCount)
		{
			const int random = randomInt(0, max);
			int curValue = 0;
			const int count = oddsList.size();
			FOR_J(count)
			{
				// 已经被选中的下标就需要排除掉
				if (selectIndexes.contains(j))
				{
					continue;
				}
				curValue += oddsList[j];
				if (random <= curValue)
				{
					selectIndexes.push_back(j);
					// 选出一个就去除此下标的权重
					max -= oddsList[j];
					break;
				}
			}
		}
	}

	int randomSelect(const Vector<int>& oddsList)
	{
		int max = 0;
		for (const int value : oddsList)
		{
			max += value;
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

	Vector3 normalize(const Vector3& value)
	{
		const float inverseLength = divide(1.0f, getLength(value));
		return { value.x * inverseLength, value.y * inverseLength, value.z * inverseLength };
	}

	Vector2 normalize(const Vector2& value)
	{
		const float inverseLength = divide(1.0f, getLength(value));
		return { value.x * inverseLength, value.y * inverseLength };
	}

	Vector3 rotateVector3(const Vector3& vec, const Vector3& axis, const float radian)
	{
		Vector3 newVec = Quaternion(axis, radian) * vec;
		checkInt(newVec.x);
		checkInt(newVec.y);
		checkInt(newVec.z);
		return newVec;
	}

	float getAngleFromVector2ToVector2(const Vector2& from, const Vector2& to, const bool radian)
	{
		if (isVectorEqual(from, to))
		{
			return 0.0f;
		}
		const Vector3 from3 = normalize({ from.x, 0.0f, from.y });
		const Vector3 to3 = normalize({ to.x, 0.0f, to.y });
		float angle = getAngleBetweenVector(from3, to3);
		if (cross(from3, to3).y < 0.0f)
		{
			angle = -angle;
		}
		if (!radian)
		{
			angle = toDegree(angle);
		}
		return angle;
	}

	// 求Z轴顺时针旋转一定角度后的向量,角度范围是-MATH_PI 到 MATH_PI
	Vector3 getVectorFromAngle(float angle)
	{
		clampRadian180(angle);
		Vector3 temp(-sin(angle), 0.0f, cos(angle));
		// 在unity坐标系中x轴需要取反
		temp.x = -temp.x;
		return temp;
	}

	bool replaceKeywordAndCalculate(string& str, const string& keyword, const int replaceValue, const bool floatOrInt)
	{
		// 如果最后的表达式中存在i,则需要把i替换为i具体的值,然后计算出最后的表达式的值
		bool replaced = false;
		int iPos;
		while ((iPos = (int)str.find_first_of(keyword)) != -1)
		{
			replaced = true;
			replace(str, iPos, iPos + (int)keyword.length(), IToS(replaceValue));
		}
		str = floatOrInt ? FToS(calculateFloat(str)) : IToS(calculateInt(str));
		return replaced;
	}

	bool replaceStringKeyword(string& str, const string& keyword, const int keyValue, const bool floatOrInt)
	{
		bool replaced = false;
		int expressionBegin = -1;
		int expressionEnd = -1;
		// 倒序寻找
		while (findString(str, "\\(", &expressionBegin, 0, false))
		{
			replaced = true;
			// 找到匹配的)
			findString(str, ")", &expressionEnd, 0, false);
			// expressionBegin + 1 去掉 /
			string calculateValue = str.substr(expressionBegin + 1, expressionEnd - expressionBegin + 1);
			replaceKeywordAndCalculate(calculateValue, keyword, keyValue, floatOrInt);
			// 替换掉最后一个\\()之间的内容
			replace(str, expressionBegin, expressionEnd + 1, calculateValue);
		}
		return replaced;
	}

	float calculateFloat(const string& str)
	{
		string newStr = str;
		// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
		if (!checkString(newStr, "0123456789.+-*/()"))
		{
			return 0.0f;
		}
		// 判断左右括号数量是否相等
		if (getCharCount(newStr, '(') != getCharCount(newStr, ')'))
		{
			return 0;
		}

		// 循环判断传入的字符串有没有括号
		while (true)
		{
			// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
			const int leftBracket = (int)newStr.find_first_of('(');
			const int rightBracket = (int)newStr.find_first_of('(');
			if (leftBracket == FrameDefine::NOT_FIND && rightBracket == FrameDefine::NOT_FIND)
			{
				break;
			}
			float ret = calculateFloat(newStr.substr(leftBracket + 1, rightBracket - leftBracket - 1));
			// 如果括号中的计算结果是负数,则标记为负数
			const bool isMinus = ret < 0;
			ret = abs(ret);
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			replace(newStr, leftBracket, rightBracket + 1, FToS(ret));
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = leftBracket - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (newStr[i] == '+')
					{
						newStr[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					if (newStr[i] == '-')
					{
						if (newStr[i - 1] >= '0' && newStr[i - 1] <= '9')
						{
							newStr[i] = '+';
						}
						else
						{
							newStr.erase(i);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					newStr.insert(0, 1, '-');
				}
			}
		}

		Vector<float> numbers;
		Vector<char> factors;
		// 表示上一个运算符的下标+1
		int beginPos = 0;
		const int strLen = (int)newStr.length();
		FOR_I(strLen)
		{
			// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
			if (i == strLen - 1)
			{
				numbers.push_back(SToF(newStr.substr(beginPos, strLen - beginPos)));
				break;
			}
			// 找到第一个运算符
			if ((newStr[i] < '0' || newStr[i] > '9') && newStr[i] != '.')
			{
				if (i != 0)
				{
					numbers.push_back(SToF(newStr.substr(beginPos, i - beginPos)));
				}
				// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
				else
				{
					numbers.push_back(0);
				}
				factors.push_back(newStr[i]);
				beginPos = i + 1;
			}
		}
		if (factors.size() + 1 != numbers.size())
		{
			// 计算错误,运算符与数字数量不符
			return 0;
		}
		// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
		while (true)
		{
			// 表示是否还有乘除表达式
			bool hasMS = false;
			FOR_I(factors.size())
			{
				// 先遍历到哪个就先计算哪个
				if (factors[i] == '*' || factors[i] == '/')
				{
					// 第一个运算数的下标与运算符的下标是相同的
					const float num1 = numbers[i];
					const float num2 = numbers[i + 1];
					float num3 = 0.0f;
					if (factors[i] == '*')
					{
						num3 = num1 * num2;
					}
					else if (factors[i] == '/')
					{
						num3 = divide(num1, num2);
					}
					// 删除第i + 1个数,然后将第i个数替换为计算结果
					numbers.eraseAt(i + 1);
					if (numbers.size() == 0)
					{
						// 计算错误
						return 0;
					}
					numbers[i] = num3;
					// 删除第i个运算符
					factors.eraseAt(i);
					hasMS = true;
					break;
				}
			}
			if (!hasMS)
			{
				break;
			}
		}
		// 再计算加减法
		while (true)
		{
			if (factors.size() == 0)
			{
				break;
			}
			if (factors[0] == '+' || factors[0] == '-')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				const float num1 = numbers[0];
				const float num2 = numbers[1];
				float num3 = 0.0f;
				if (factors[0] == '+')
				{
					num3 = num1 + num2;
				}
				else if (factors[0] == '-')
				{
					num3 = num1 - num2;
				}
				// 删除第1个数,然后将第0个数替换为计算结果
				numbers.eraseAt(1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[0] = num3;
				// 删除第0个运算符
				factors.eraseAt(0);
			}
		}
		float result = 0.0f;
		if (numbers.size() == 1)
		{
			result = numbers[0];
		}
		// 计算错误
		return result;
	}

	int calculateInt(const string& str)
	{
		string newStr = str;
		// 判断字符串是否含有非法字符,也就是除数字,小数点,运算符以外的字符
		if (!checkString(newStr, "0123456789+-*/%()"))
		{
			return 0;
		}
		// 判断左右括号数量是否相等
		if (getCharCount(newStr, '(') != getCharCount(newStr, ')'))
		{
			return 0;
		}

		// 循环判断传入的字符串有没有括号
		while (true)
		{
			// 先判断有没有括号，如果有括号就先算括号里的,如果没有就退出while循环
			const int leftBracket = (int)newStr.find_first_of('(');
			const int rightBracket = (int)newStr.find_first_of('(');
			if (leftBracket == FrameDefine::NOT_FIND && rightBracket == FrameDefine::NOT_FIND)
			{
				break;
			}
			int ret = calculateInt(newStr.substr(leftBracket + 1, rightBracket - leftBracket - 1));
			// 如果括号中的计算结果是负数,则标记为负数
			const bool isMinus = ret < 0;
			ret = abs(ret);
			// 将括号中的计算结果替换原来的表达式,包括括号也一起替换
			replace(newStr, leftBracket, rightBracket + 1, IToS(ret));
			if (isMinus)
			{
				// 如果括号中计算出来是负数,则将负号提取出来,将左边的第一个加减号改为相反的符号
				bool changeMark = false;
				for (int i = leftBracket - 1; i >= 0; --i)
				{
					// 找到第一个+号,则直接改为减号,然后退出遍历
					if (newStr[i] == '+')
					{
						newStr[i] = '-';
						changeMark = true;
						break;
					}
					// 找到第一个减号,如果减号的左边有数字,则直接改为+号
					// 如果减号的左边不是数字,则该减号是负号,将减号去掉,
					if (newStr[i] == '-')
					{
						if (newStr[i - 1] >= '0' && newStr[i - 1] <= '9')
						{
							newStr[i] = '+';
						}
						else
						{
							newStr.erase(i);
						}
						changeMark = true;
						break;
					}
				}
				// 如果遍历完了还没有找到可以替换的符号,则在表达式最前面加一个负号
				if (!changeMark)
				{
					newStr.insert(0, 1, '-');
				}
			}
		}

		Vector<int> numbers;
		Vector<char> factors;
		// 表示上一个运算符的下标+1
		int beginPos = 0;
		const int strLen = (int)newStr.length();
		FOR_I(strLen)
		{
			// 遍历到了最后一个字符,则直接把最后一个数字放入列表,然后退出循环
			if (i == strLen - 1)
			{
				numbers.push_back(SToI(newStr.substr(beginPos, strLen - beginPos)));
				break;
			}
			// 找到第一个运算符
			if ((newStr[i] < '0' || newStr[i] > '9') && newStr[i] != '.')
			{
				if (i != 0)
				{
					numbers.push_back(SToI(newStr.substr(beginPos, i - beginPos)));
				}
				// 如果在表达式的开始就发现了运算符,则表示第一个数是负数,那就处理为0减去这个数的绝对值
				else
				{
					numbers.push_back(0);
				}
				factors.push_back(newStr[i]);
				beginPos = i + 1;
			}
		}
		if (factors.size() + 1 != numbers.size())
		{
			// 计算错误,运算符与数字数量不符
			return 0;
		}
		// 现在开始计算表达式,按照运算优先级,先计算乘除和取余
		while (true)
		{
			// 表示是否还有乘除表达式
			bool hasMS = false;
			FOR_I(factors.size())
			{
				// 先遍历到哪个就先计算哪个
				if (factors[i] == '*' || factors[i] == '/' || factors[i] == '%')
				{
					// 第一个运算数的下标与运算符的下标是相同的
					const int num1 = numbers[i];
					const int num2 = numbers[i + 1];
					int num3 = 0;
					if (factors[i] == '*')
					{
						num3 = num1 * num2;
					}
					else if (factors[i] == '/')
					{
						num3 = divideInt(num1, num2);
					}
					else if (factors[i] == '%')
					{
						num3 = num1 % num2;
					}
					// 删除第i + 1个数,然后将第i个数替换为计算结果
					numbers.eraseAt(i + 1);
					if (numbers.size() == 0)
					{
						// 计算错误
						return 0;
					}
					numbers[i] = num3;
					// 删除第i个运算符
					factors.eraseAt(i);
					hasMS = true;
					break;
				}
			}
			if (!hasMS)
			{
				break;
			}
		}
		// 再计算加减法
		while (true)
		{
			if (factors.size() == 0)
			{
				break;
			}
			if (factors[0] == '+' || factors[0] == '-')
			{
				// 第一个运算数的下标与运算符的下标是相同的
				const int num1 = numbers[0];
				const int num2 = numbers[1];
				int num3 = 0;
				if (factors[0] == '+')
				{
					num3 = num1 + num2;
				}
				else if (factors[0] == '-')
				{
					num3 = num1 - num2;
				}
				// 删除第1个数,然后将第0个数替换为计算结果
				numbers.eraseAt(1);
				if (numbers.size() == 0)
				{
					// 计算错误
					return 0;
				}
				numbers[0] = num3;
				// 删除第0个运算符
				factors.eraseAt(0);
			}
		}
		int result = 0;
		if (numbers.size() == 1)
		{
			result = numbers[0];
		}
		return result;
	}

	constexpr float HueToRGB(const float v1, const float v2, float vH)
	{
		if (vH < 0.0f)
		{
			vH += 1.0f;
		}
		if (vH > 1.0f)
		{
			vH -= 1.0f;
		}
		if (6.0f * vH < 1.0f)
		{
			return v1 + (v2 - v1) * 6.0f * vH;
		}
		else if (2.0f * vH < 1.0f)
		{
			return v2;
		}
		else if (3.0f * vH < 2.0f)
		{
			return v1 + (v2 - v1) * (2.0f / 3.0f - vH) * 6.0f;
		}
		return v1;
	}

	int findPointIndex(const Vector<float>& distanceListFromStart, const float curDistance, const int startIndex, const int endIndex)
	{
		if (curDistance < distanceListFromStart[startIndex])
		{
			return startIndex - 1;
		}
		if (curDistance >= distanceListFromStart[endIndex])
		{
			return endIndex;
		}
		if (endIndex == startIndex || endIndex - startIndex == 1)
		{
			return startIndex;
		}
		const int middleIndex = (int)((endIndex - startIndex) >> 1) + startIndex;
		// 当前距离比中间的距离小,则在列表前半部分查找
		if (curDistance < distanceListFromStart[middleIndex])
		{
			return findPointIndex(distanceListFromStart, curDistance, startIndex, middleIndex);
		}
		// 当前距离比中间的距离小,则在列表后半部分查找
		else if (curDistance > distanceListFromStart[middleIndex])
		{
			return findPointIndex(distanceListFromStart, curDistance, middleIndex, endIndex);
		}
		// 距离刚好等于列表中间的值,则返回该下标
		else
		{
			return middleIndex;
		}
	}

	// 计算两条直线的交点,返回值表示两条直线是否相交
	bool intersectLine2(const Line2& line0, const Line2& line1, Vector2& intersect)
	{
		// 计算两条线的k和b
		float k0 = 0.0f;
		float b0 = 0.0f;
		const bool hasK0 = generateLineExpression(line0, k0, b0);
		float k1 = 0.0f;
		float b1 = 0.0f;
		const bool hasK1 = generateLineExpression(line1, k1, b1);
		// 两条竖直的线没有交点,即使两条竖直的线重合也不计算交点
		if (!hasK0 && !hasK1)
		{
			return false;
		}
		// 直线0为竖直的线
		else if (!hasK0)
		{
			intersect.x = line0.mStart.x;
			intersect.y = k1 * intersect.x + b1;
			return true;
		}
		// 直线1为竖直的线
		else if (!hasK1)
		{
			intersect.x = line1.mStart.x;
			intersect.y = k0 * intersect.x + b0;
			return true;
		}
		else
		{
			// 两条不重合且不平行的两条线才计算交点
			if (!isEqual(k0, k1))
			{
				intersect.x = divide(b1 - b0, k0 - k1);
				intersect.y = k0 * intersect.x + b0;
				return true;
			}
		}
		return false;
	}

	// 计算两条直线的交点,返回值表示两条直线是否相交
	bool intersectLine3IgnoreY(const Line3& line0, const Line3& line1, Vector3& intersect)
	{
		// 计算两条线的k和b
		float k0 = 0.0f;
		float b0 = 0.0f;
		const bool hasK0 = generateLineExpression(line0.toLine2IgnoreY(), k0, b0);
		float k1 = 0.0f;
		float b1 = 0.0f;
		const bool hasK1 = generateLineExpression(line1.toLine2IgnoreY(), k1, b1);
		// 两条竖直的线没有交点,即使两条竖直的线重合也不计算交点
		if (!hasK0 && !hasK1)
		{
			return false;
		}
		// 直线0为竖直的线
		else if (!hasK0)
		{
			intersect.x = line0.mStart.x;
			intersect.z = k1 * intersect.x + b1;
			return true;
		}
		// 直线1为竖直的线
		else if (!hasK1)
		{
			intersect.x = line1.mStart.x;
			intersect.z = k0 * intersect.x + b0;
			return true;
		}
		else
		{
			// 两条不重合且不平行的两条线才计算交点
			if (!isEqual(k0, k1))
			{
				intersect.x = divide(b1 - b0, k0 - k1);
				intersect.z = k0 * intersect.x + b0;
				return true;
			}
		}
		return false;
	}

	// k为斜率,也就是cotan(直线与y轴的夹角)
	bool generateLineExpression(const Line2& line, float& k, float& b)
	{
		// 一条横着的线,斜率为0
		if (isZero(line.mStart.y - line.mEnd.y))
		{
			k = 0.0f;
			b = line.mStart.y;
		}
		// 直线是一条竖直的线,没有斜率
		else if (isZero(line.mStart.x - line.mEnd.x))
		{
			return false;
		}
		else
		{
			k = divide(line.mStart.y - line.mEnd.y, line.mStart.x - line.mEnd.x);
			b = line.mStart.y - k * line.mStart.x;
		}
		return true;
	}

	bool intersectLineSection(const Line2& line0, const Line2& line1, Vector2& intersect, const bool checkEndPoint)
	{
		// 有端点重合
		if (isVectorEqual(line0.mStart, line1.mStart) ||
			isVectorEqual(line0.mStart, line1.mEnd) ||
			isVectorEqual(line0.mEnd, line1.mStart) ||
			isVectorEqual(line0.mEnd, line1.mEnd))
		{
			// 考虑端点时认为两条线段相交
			// 不考虑端点时,则两条线段不相交
			return checkEndPoint;
		}
		if (intersectLine2(line0, line1, intersect))
		{
			// 如果交点都在两条线段内,则两条线段相交
			return inRange(intersect, line0.mStart, line0.mEnd) && inRange(intersect, line1.mStart, line1.mEnd);
		}
		return false;
	}

	bool intersectLineSectionIgnoreY(const Line3& line0, const Line3& line1, Vector3& intersect, const bool checkEndPoint)
	{
		// 有端点重合
		if (isVectorEqual(line0.mStart, line1.mStart) ||
			isVectorEqual(line0.mStart, line1.mEnd) ||
			isVectorEqual(line0.mEnd, line1.mStart) ||
			isVectorEqual(line0.mEnd, line1.mEnd))
		{
			// 考虑端点时认为两条线段相交
			// 不考虑端点时,则两条线段不相交
			return checkEndPoint;
		}
		if (intersectLine3IgnoreY(line0, line1, intersect))
		{
			// 如果交点都在两条线段内,则两条线段相交
			return inRange(intersect, resetY(line0.mStart), resetY(line0.mEnd)) && 
				   inRange(intersect, resetY(line1.mStart), resetY(line1.mEnd));
		}
		return false;
	}

	// 计算线段与三角形是否相交
	bool intersectLineTriangle(const Line2& line, const Triangle2& triangle, TriangleIntersect& intersectResult, const bool checkEndPoint)
	{
		Vector2 intersect0;
		Vector2 intersect1;
		Vector2 intersect2;
		// 对三条边都要检测,计算出最近的一个交点
		const bool result0 = intersectLineSection(line, Line2(triangle.mPoint0, triangle.mPoint1), intersect0, checkEndPoint);
		const bool result1 = intersectLineSection(line, Line2(triangle.mPoint1, triangle.mPoint2), intersect1, checkEndPoint);
		const bool result2 = intersectLineSection(line, Line2(triangle.mPoint2, triangle.mPoint0), intersect2, checkEndPoint);
		Vector2 point;
		Vector2 linePoint0;
		Vector2 linePoint1;
		float closestDistance = 99999.0f * 99999.0f;
		// 与第一条边相交
		if (result0)
		{
			const float squaredLength = getSquaredLength(intersect0 - line.mStart);
			if (squaredLength < closestDistance)
			{
				closestDistance = squaredLength;
				point = intersect0;
				linePoint0 = triangle.mPoint0;
				linePoint1 = triangle.mPoint1;
			}
		}
		// 与第二条边相交
		if (result1)
		{
			const float squaredLength = getSquaredLength(intersect1 - line.mStart);
			if (squaredLength < closestDistance)
			{
				closestDistance = squaredLength;
				point = intersect1;
				linePoint0 = triangle.mPoint1;
				linePoint1 = triangle.mPoint2;
			}
		}
		// 与第三条边相交
		if (result2)
		{
			const float squaredLength = getSquaredLength(intersect2 - line.mStart);
			if (squaredLength < closestDistance)
			{
				point = intersect2;
				linePoint0 = triangle.mPoint2;
				linePoint1 = triangle.mPoint0;
			}
		}
		intersectResult.mIntersectPoint = point;
		intersectResult.mLinePoint0 = linePoint0;
		intersectResult.mLinePoint1 = linePoint1;
		return result0 || result1 || result2;
	}

	// 计算线段与三角形是否相交
	bool intersectLineTriangleIgnoreY(const Line3& line, const Triangle3& triangle, TriangleIntersect3& intersectResult, const bool checkEndPoint)
	{
		Vector3 intersect0;
		Vector3 intersect1;
		Vector3 intersect2;
		// 对三条边都要检测,计算出最近的一个交点
		const bool result0 = intersectLineSectionIgnoreY(line, Line3(triangle.mPoint0, triangle.mPoint1), intersect0, checkEndPoint);
		const bool result1 = intersectLineSectionIgnoreY(line, Line3(triangle.mPoint1, triangle.mPoint2), intersect1, checkEndPoint);
		const bool result2 = intersectLineSectionIgnoreY(line, Line3(triangle.mPoint2, triangle.mPoint0), intersect2, checkEndPoint);
		Vector3 point;
		Vector3 linePoint0;
		Vector3 linePoint1;
		float closestDistance = 99999.0f * 99999.0f;
		const Vector3 startNoY = resetY(line.mStart);
		// 与第一条边相交
		if (result0)
		{
			const float squaredLength = getSquaredLength(intersect0 - startNoY);
			if (squaredLength < closestDistance)
			{
				closestDistance = squaredLength;
				point = intersect0;
				linePoint0 = triangle.mPoint0;
				linePoint1 = triangle.mPoint1;
			}
		}
		// 与第二条边相交
		if (result1)
		{
			const float squaredLength = getSquaredLength(intersect1 - startNoY);
			if (squaredLength < closestDistance)
			{
				closestDistance = squaredLength;
				point = intersect1;
				linePoint0 = triangle.mPoint1;
				linePoint1 = triangle.mPoint2;
			}
		}
		// 与第三条边相交
		if (result2)
		{
			if (getSquaredLength(intersect2 - startNoY) < closestDistance)
			{
				point = intersect2;
				linePoint0 = triangle.mPoint2;
				linePoint1 = triangle.mPoint0;
			}
		}
		intersectResult.mIntersectPoint = point;
		intersectResult.mLinePoint0 = resetY(linePoint0);
		intersectResult.mLinePoint1 = resetY(linePoint1);
		return result0 || result1 || result2;
	}

	bool intersectIgnoreY(const Line3& line, const Rect3& rect, Vector3* intersectPtr, const bool checkEndPoint)
	{
		const float dis = getDistanceToLineIgnoreY(rect.getCenter(), line);
		// 距离大于对角线的一半,则不与矩形相交
		if (dis > rect.getDiagonalIgnoreY() * 0.5f)
		{
			return false;
		}
		// 直线是否与任何一条边相交
		const Vector3 cornor0(rect.mMin.x, 0.0f, rect.mMin.z);
		const Vector3 cornor1(rect.mMin.x, 0.0f, rect.mMax.z);
		const Vector3 cornor2(rect.mMax.x, 0.0f, rect.mMax.z);
		const Vector3 cornor3(rect.mMax.x, 0.0f, rect.mMin.z);
		Vector3 intersect;
		bool result = intersectLineSectionIgnoreY(line, Line3(cornor0, cornor1), intersect, checkEndPoint) ||
					  intersectLineSectionIgnoreY(line, Line3(cornor3, cornor2), intersect, checkEndPoint) ||
					  intersectLineSectionIgnoreY(line, Line3(cornor3, cornor0), intersect, checkEndPoint) ||
					  intersectLineSectionIgnoreY(line, Line3(cornor2, cornor1), intersect, checkEndPoint);
		if (result && intersectPtr != nullptr)
		{
			*intersectPtr = intersect;
		}
		return result;
	}

	bool intersect(const Line2& line, const Rect& rect, Vector2* intersectPtr, const bool checkEndPoint)
	{
		const float dis = getDistanceToLine(rect.getCenter(), line);
		// 距离大于对角线的一半,则不与矩形相交
		if (dis > rect.getDiagonal() * 0.5f)
		{
			return false;
		}
		// 直线是否与任何一条边相交
		const Vector2 leftTop(rect.x, rect.y + rect.height);
		const Vector2 rightTop(rect.x + rect.width, rect.y + rect.height);
		const Vector2 rightBottom(rect.x + rect.width, rect.y);
		const Vector2 leftBottom(rect.x, rect.y);
		bool result = false;
		Vector2 intersect;
		result = intersectLineSection(line, Line2(leftTop, rightTop), intersect, checkEndPoint) ||
				 intersectLineSection(line, Line2(leftBottom, rightBottom), intersect, checkEndPoint) ||
				 intersectLineSection(line, Line2(leftBottom, leftTop), intersect, checkEndPoint) ||
				 intersectLineSection(line, Line2(rightBottom, rightTop), intersect, checkEndPoint);
		if (result && intersectPtr != nullptr)
		{
			*intersectPtr = intersect;
		}
		return result;
	}

	// 计算点到线的距离
	float getDistanceToLine(const Vector2& point, const Line2& line)
	{
		return getLength(point - getProjectPoint(point, line));
	}

	// 计算点到线的距离
	float getDistanceToLineIgnoreY(const Vector3& point, const Line3& line)
	{
		return getLength(resetY(point) - getProjectPointIgnoreY(point, line));
	}

	// 计算点在线上的投影
	Vector2 getProjectPoint(const Vector2& point, const Line2& line)
	{
		// 计算出点到线一段的向量在线上的投影
		const Vector2 projectOnLine = getProjection(point - line.mStart, line.mEnd - line.mStart);
		// 点到线垂线的交点
		return line.mStart + projectOnLine;
	}

	// 计算点在线上的投影
	Vector3 getProjectPointIgnoreY(const Vector3& point, const Line3& line)
	{
		// 计算出点到线一段的向量在线上的投影
		const Vector3 projectOnLine = getProjectionIgnoreY(point - line.mStart, line.mEnd - line.mStart);
		// 点到线垂线的交点
		return resetY(line.mStart) + projectOnLine;
	}

	// 计算一个向量在另一个向量上的投影
	Vector2 getProjection(const Vector2& v1, const Vector2& v2)
	{
		return normalize(v2) * getLength(v1) * cos(getAngleBetweenVector(v1, v2));
	}

	// 计算一个向量在另一个向量上的投影
	Vector3 getProjectionIgnoreY(const Vector3& v1, const Vector3& v2)
	{
		const Vector3 v1NoY = resetY(v1);
		const Vector3 v2NoY = resetY(v2);
		return normalize(v2NoY) * getLength(v1NoY) * cos(getAngleBetweenVector(v1NoY, v2NoY));
	}
}