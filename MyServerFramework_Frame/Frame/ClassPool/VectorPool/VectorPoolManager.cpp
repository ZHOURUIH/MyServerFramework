#include "FrameHeader.h"

HashMap<llong, VectorPoolBase*> VectorPoolManager::mVectorPoolThreadList;

VectorPoolManager::~VectorPoolManager()
{
	for (const auto& iter : mVectorPoolThreadList)
	{
		delete iter.second;
	}
	mVectorPoolThreadList.clear();
}