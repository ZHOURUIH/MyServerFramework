#include "GameHeader.h"

void GameSTLPoolRegister::registeAll()
{
	// 自定义类型

	// auto generate start Excel数据类型
	mVectorPoolManager->registeVectorPool<EDAchivement*>();
	mVectorPoolManager->registeVectorPool<EDGlobal*>();
	mVectorPoolManager->registeVectorPool<EDTest*>();
	// auto generate end Excel数据类型
}