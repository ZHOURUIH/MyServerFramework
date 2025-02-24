#pragma once

#include "GameEnum.h"

// 为了提高属性的访问速度,不通过虚函数进行访问,而是可以直接从基类的数据类中获取
class CharacterGameData
{
public:
	virtual ~CharacterGameData() = default;
	virtual void resetProperty();
public:
	int mMaxHP = 0;				// 当前最大HP
	int mHP = 0;				// 当前HP
};