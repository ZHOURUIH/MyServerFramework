#include "FrameHeader.h"

void ExcelManager::quit()
{
	for (const auto& iter : mTableList)
	{
		delete iter.second;
	}
	mTableList.clear();
}

void ExcelManager::checkAll()
{
	for (const auto& iter : mTableList)
	{
		iter.second->checkAllData();
	}
}