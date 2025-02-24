#pragma once

#include "ArrayPoolSingle.h"
#include "ArrayPoolSingleChar.h"
#include "Utility.h"
#include "FrameComponent.h"

// 仅支持在主线程使用,T类型不能为需要调用构造析构的类型,释放时不会进行析构
class MICRO_LEGEND_FRAME_API ArrayPool : public FrameComponent
{
	BASE(ArrayPool, FrameComponent);
public:
	void init() override;
	void quit() override;
	~ArrayPool() override;
	bool isClassPool() const override { return true; }
	template<typename T>
	static T* newArray(const int count, const bool zeroMemory = false)
	{
		checkMainThread();
		return mArrayPoolSingle->get<T>(count, zeroMemory);
	}
	template<typename T>
	static void deleteArray(T*& data, const int elementCount)
	{
		checkMainThread();
		mArrayPoolSingle->release<T>(data, elementCount);
		data = nullptr;
	}
	static char* newCharArray(const int count, const bool zeroMemory = false, bool* isNewAlloc = nullptr)
	{
		checkMainThread();
		return mArrayPoolSingleChar->get(count, zeroMemory, isNewAlloc);
	}
	static void deleteCharArray(char*& data, const int size)
	{
		checkMainThread();
		mArrayPoolSingleChar->release(data, size);
		data = nullptr;
	}
private:
	static void onHour(void* userData);
protected:
	static ArrayPoolSingle* mArrayPoolSingle;
	static ArrayPoolSingleChar* mArrayPoolSingleChar;
};