#pragma once

#include "Singleton.h"
#include "ThreadLock.h"
#include "Map.h"
#include "Vector.h"
#include "Set.h"
#include "Utility.h"

class MICRO_LEGEND_FRAME_API ServerFramework : public Singleton<ServerFramework>
{
	BASE(ServerFramework, Singleton<ServerFramework>);
public:
	ServerFramework();
	virtual ~ServerFramework() = default;
	virtual void init();
	void destroy();
	virtual void update(float elapsedTime);
	virtual void launch();
	bool isStop() const { return mStop; }
	void stop() { mStop = true; }
	llong getFrameID() const { return mFrameID; }
	void addFrameTime(llong frameTimeMS)
	{
		mAllFrameMS += frameTimeMS; 
		++mAllFrameCount;
	}
	// 获得成员变量
#ifdef LINUX
	const ulong & getStartMiliTime() const { return mStartMiliTime; }
#endif
	template<typename T, typename TypeCheck = typename IsSubClassOf<FrameComponent, T>::mType>
	T* getSystem(const string& name) const
	{
		return static_cast<T*>(mFrameComponentMap.tryGet(name));
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<FrameComponent, T>::mType>
	void getSystem(const string& name, T*& component) const
	{
		component = static_cast<T*>(mFrameComponentMap.tryGet(name));
	}
	template<typename T, typename TypeCheck = typename IsSubClassOf<FrameComponent, T>::mType>
	void registeSystem(const char* name)
	{
		T* component = new T();
		component->setName(name);
		mFrameComponentVector.push_back(component);
		mFrameComponentMap.insert(name, component);
		if (component->isClassPool())
		{
			mPoolSystemList.insert(component);
		}
		else if (component->isFactory())
		{
			mFactorySystemList.insert(component);
		}
	}
	void registeSecond(void* userData, VoidPtrCallback callback)	{ mSecondCallback.insert(userData, callback); }
	void unregisteSecond(void* userData)							{ mSecondCallback.erase(userData); }
	void registeMinute(void* userData, VoidPtrCallback callback)	{ mMinuteCallback.insert(userData, callback); }
	void unregisteMinute(void* userData)							{ mMinuteCallback.erase(userData); }
	void registeHour(void* userData, VoidPtrCallback callback)		{ mHourCallback.insert(userData, callback); }
	void unregisteHour(void* userData)								{ mHourCallback.erase(userData); }
	void registeDay(void* userData, VoidPtrCallback callback)		{ mDayCallback.insert(userData, callback); }
	void unregisteDay(void* userData)								{ mDayCallback.erase(userData); }
	void registeWeek(void* userData, VoidPtrCallback callback)		{ mWeekCallback.insert(userData, callback); }
	void unregisteWeek(void* userData)								{ mWeekCallback.erase(userData); }
protected:
	virtual void registeComponent();
	virtual void constructDone();
	virtual void clearSystem();
	virtual void registe();
protected:
	Vector<FrameComponent*> mFrameComponentVector;
	HashMap<string, FrameComponent*> mFrameComponentMap;
	HashMap<void*, VoidPtrCallback> mSecondCallback;
	HashMap<void*, VoidPtrCallback> mMinuteCallback;
	HashMap<void*, VoidPtrCallback> mHourCallback;
	HashMap<void*, VoidPtrCallback> mDayCallback;
	HashMap<void*, VoidPtrCallback> mWeekCallback;
	Set<FrameComponent*> mPoolSystemList;
	Set<FrameComponent*> mFactorySystemList;
#ifdef LINUX
	ulong mStartMiliTime;
#endif
	atomic<bool> mStop;
	ThreadLock mLock;
	llong mFrameID = 0;
	int mFPS = 0;
	int mCurFrameCount = 0;
	int mLastDayOfWeek = 0;
	llong mAllFrameMS = 0;			// 用于计算每帧的平均耗时
	llong mAllFrameCount = 0;		// 用于计算每帧的平均耗时
};