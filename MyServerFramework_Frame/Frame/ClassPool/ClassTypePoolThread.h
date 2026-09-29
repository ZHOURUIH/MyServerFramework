#pragma once

#include "ClassPoolBase.h"
#include "ClassObject.h"
#include "Utility.h"
#include "ErrorProfile.h"
#include "SPSCQueue.h"

// 根据key创建对应类型的对象
// 线程安全：只允许一个线程 newClass，另一个线程 destroyClass
// 跨线程交接：优先无锁（SPSCQueue），溢出时回退加锁 HashMap
template<typename ClassType, typename KeyType, typename TypeCheck = typename IsSubClassOf<ClassObject, ClassType>::mType>
class ClassTypePoolThread : public ClassPoolBase
{
	BASE(ClassTypePoolThread, ClassPoolBase);
public:
	ClassTypePoolThread():
		mAssignIDSeed(0)
	{
		mFreeQueue = new SPSCQueue<ClassType, 1 << 20>();
	}
	void initDefault(const KeyType type, const int count)
	{
		if (count == 0)
		{
			return;
		}
		Vector<ClassType*> list(count);
		FOR(count)
		{
			ClassType* obj = create(type);
			obj->resetProperty();
			list.add(obj);
		}
		// mPool 只在线程1访问，无需加锁
		mPool.addOrGet(type).addRange(list);
		{
			THREAD_LOCK(mTotalCountLock);
			mTotalCount.addOrGet(type, make_pair(typeid(*list[0]).name(), 0)).second += count;
		}
	}
	void quit() override
	{
		// quit 时两线程都已停止，无需加锁
		for (auto& item : mPool)
		{
			DELETE_LIST(item.second);
		}
		mPool.clear();
		for (auto& item : mOverflow)
		{
			DELETE_LIST(item.second);
		}
		mOverflow.clear();
	}
	// 这里函数名中的Temp的意思是创建的对象必须在一定时间内被回收,否则就会报错,提示内存泄露
	void newClassListTemp(const KeyType type, Vector<ClassType*>& classList, const int dataCount)
	{
		newClassList(type, classList, dataCount);
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
		for (ClassType* obj : classList)
		{
			mClassRemainTimeList.add(obj, DEFAULT_LIFE_TIME);
		}
#endif
	}
	void newClassList(const KeyType type, Vector<ClassType*>& classList, const int dataCount)
	{
		checkNewClassThread();
		classList.clearAndReserve(dataCount);

		// 1. 先从本地池取
		auto* poolSet = mPool.getPtr(type);
		if (poolSet != nullptr && !poolSet->isEmpty())
		{
			FOR(dataCount - classList.size())
			{
				ClassType* obj = poolSet->popBack(nullptr);
				if (obj == nullptr) break;
				classList.add(obj);
			}
		}

		// 2. 本地池不够，从回收队列拉一批出来
		if (classList.size() < dataCount)
		{
			recycleToPool();
			// 再试一次
			poolSet = mPool.getPtr(type);
			if (poolSet != nullptr && !poolSet->isEmpty())
			{
				FOR(dataCount - classList.size())
				{
					ClassType* obj = poolSet->popBack(nullptr);
					if (obj == nullptr)
					{
						break;
					}
					classList.add(obj);
				}
			}
		}

		// 3. 还不够就创建新的
		if (classList.size() < dataCount)
		{
			ClassType* tempValidObj = nullptr;
			const int needCreateCount = dataCount - classList.size();
			int createCount = 0;
			FOR(needCreateCount)
			{
				ClassType* obj = create(type);
				if (obj != nullptr)
				{
					++createCount;
					obj->resetProperty();
					tempValidObj = obj;
				}
				classList.add(obj);
			}
			THREAD_LOCK(mTotalCountLock);
			mTotalCount.addOrGet(type, make_pair(typeid(*tempValidObj).name(), 0)).second += createCount;
			if (mShowCountLog && (mTotalCount[type].second & (4096 - 1)) == 0 && tempValidObj != nullptr)
			{
				LOG(string(typeid(*tempValidObj).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}

		for (ClassType* item : classList)
		{
			if (item != nullptr)
			{
				prepareObject(item, type);
			}
		}
	}
	ClassType* newClassTemp(const KeyType type)
	{
		ClassType* obj = newClass(type);
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
		mClassRemainTimeList.add(obj, DEFAULT_LIFE_TIME);
#endif
		return obj;
	}
	ClassType* newClass(const KeyType type)
	{
		checkNewClassThread();
		ClassType* obj = nullptr;

		// 1. 从本地池取
		auto* poolSet = mPool.getPtr(type);
		if (poolSet != nullptr && !poolSet->isEmpty())
		{
			obj = poolSet->popBack(nullptr);
		}

		// 2. 本地池空了，从回收队列补充
		if (obj == nullptr)
		{
			recycleToPool();
			poolSet = mPool.getPtr(type);
			if (poolSet != nullptr && !poolSet->isEmpty())
			{
				obj = poolSet->popBack(nullptr);
			}
		}

		// 3. 还没有就创建新的
		if (obj == nullptr)
		{
			obj = create(type);
			if (obj == nullptr)
			{
				return nullptr;
			}
			obj->resetProperty();
			THREAD_LOCK(mTotalCountLock);
			++mTotalCount.addOrGet(type, make_pair(typeid(*obj).name(), 0)).second;
			if (mShowCountLog && (mTotalCount[type].second & (4096 - 1)) == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}
		// 设置为可用状态
		prepareObject(obj, type);
		return obj;
	}
	// 这里函数名中的Temp的意思是创建的对象必须在一定时间内被回收,否则就会报错,提示内存泄露
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	T* newClassTemp(const KeyType type)
	{
		T* obj = newClass(type);
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
		mClassRemainTimeList.add(obj, DEFAULT_LIFE_TIME);
#endif
		return obj;
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	T* newClass(const KeyType type)
	{
		checkNewClassThread();
		T* obj = nullptr;

		// 1. 从本地池取
		auto* poolSet = mPool.getPtr(type);
		if (poolSet != nullptr && !poolSet->isEmpty())
		{
			obj = static_cast<T*>(poolSet->popBack(nullptr));
		}

		// 2. 从回收队列补充
		if (obj == nullptr)
		{
			recycleToPool();
			poolSet = mPool.getPtr(type);
			if (poolSet != nullptr && !poolSet->isEmpty())
			{
				obj = static_cast<T*>(poolSet->popBack(nullptr));
			}
		}

		// 3. 创建新的
		if (obj == nullptr)
		{
			obj = new T();
			obj->resetProperty();
			THREAD_LOCK(mTotalCountLock);
			++mTotalCount.addOrGet(type, make_pair(typeid(*obj).name(), 0)).second;
			if (mShowCountLog && (mTotalCount[type].second & (4096 - 1)) == 0)
			{
				LOG(string(typeid(*obj).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}

		// 设置为可用状态
		prepareObject(obj, type);
		return obj;
	}
	// 这里函数名中的Temp的意思是创建的对象必须在一定时间内被回收,否则就会报错,提示内存泄露
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void newClassListTemp(const KeyType type, Vector<ClassType*>& classList, const int dataCount)
	{
		newClassList<T>(type, classList, dataCount);
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
		for (T* obj : classList)
		{
			mClassRemainTimeList.add(obj, DEFAULT_LIFE_TIME);
		}
#endif
	}
	template<class T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void newClassList(const KeyType type, Vector<ClassType*>& classList, const int dataCount)
	{
		checkNewClassThread();
		classList.clearAndReserve(dataCount);

		// 1. 从本地池取
		auto* poolSet = mPool.getPtr(type);
		if (poolSet != nullptr && !poolSet->isEmpty())
		{
			FOR(dataCount - classList.size())
			{
				T* obj = nullptr;
				if (auto* objVector = mPool.getPtr(type))
				{
					obj = static_cast<T*>(objVector->popBack(nullptr));
				}
				if (obj == nullptr) break;
				classList.add(obj);
			}
		}

		// 2. 从回收队列补充
		if (classList.size() < dataCount)
		{
			recycleToPool();
			FOR(dataCount - classList.size())
			{
				T* obj = nullptr;
				if (auto* objVector = mPool.getPtr(type))
				{
					obj = static_cast<T*>(objVector->popBack(nullptr));
				}
				if (obj == nullptr) break;
				classList.add(obj);
			}
		}

		// 3. 创建新的
		if (classList.size() < dataCount)
		{
			const int needCreateCount = dataCount - classList.size();
			FOR(needCreateCount)
			{
				T* obj = new T();
				obj->resetProperty();
				classList.add(obj);
			}
			THREAD_LOCK(mTotalCountLock);
			mTotalCount.addOrGet(type, make_pair(typeid(*classList[0]).name(), 0)).second += needCreateCount;
			if (mShowCountLog && (mTotalCount[type].second & (4096 - 1)) == 0)
			{
				LOG(string(typeid(*classList[0]).name()) + "的数量已经达到了" + IToS(mTotalCount[type].second) + "个");
			}
		}

		// 设置为可用状态
		for (ClassType* item : classList)
		{
			prepareObject(item, type);
		}
	}

	// ── destroyClass：线程2调用，push 到无锁队列，溢出时回退加锁 HashMap ──
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClass(T*& obj)
	{
		checkDestroyClassThread();

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || obj == nullptr)
		{
			return;
		}
		// 在 destroy/markDispose 之前保存类型
		// 因为 destroy() 和 resetProperty()（由 markDispose 调用）可能清除 type 字段
		// 线程1 pop 后需要 getType() 来确定归还到哪个桶
		const KeyType savedType = obj->getType();
		obj->destroy();
		if (!obj->markDispose(this))
		{
			ERROR_PROFILE((string("0重复销毁对象:") + typeid(ClassType).name()).c_str());
			return;
		}
		// 恢复类型，确保线程1 pop 后 getType() 返回正确值
		obj->setType(savedType);
#if (defined WINDOWS) && (!defined STRESS_TEST)
		{
			THREAD_LOCK(mRemainTimeLock);
			mClassRemainTimeList.remove(obj);
		}
#endif
		pushToFreeQueue(obj);
		obj = nullptr;
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T0, typename T1, typename TypeCheck0 = typename IsSubClassOf<ClassType, T1>::mType>
	void destroyClassList(HashMap<T0, T1*>& objMap)
	{
		checkDestroyClassThread();

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (objMap.isEmpty() || mDestroied)
		{
			return;
		}
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
#endif
		for (const auto& iter : objMap)
		{
			ClassType* obj = iter.second;
			if (obj == nullptr)
			{
				continue;
			}
			const KeyType savedType = obj->getType();
			obj->destroy();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("2重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			obj->setType(savedType);
#if (defined WINDOWS) && (!defined STRESS_TEST)
			mClassRemainTimeList.remove(obj);
#endif
			pushToFreeQueue(obj);
		}
		objMap.clear();
	}
	// 由于允许传入ClassType子类的列表,所以重新定义了一个类型
	template<typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClassList(Vector<T*>& objList)
	{
		checkDestroyClassThread();

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objList.isEmpty())
		{
			return;
		}
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
#endif
		for (T* obj : objList)
		{
			if (obj == nullptr)
			{
				continue;
			}
			const KeyType savedType = obj->getType();
			obj->destroy();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("3重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			obj->setType(savedType);
#if (defined WINDOWS) && (!defined STRESS_TEST)
			mClassRemainTimeList.remove(obj);
#endif
			pushToFreeQueue(obj);
		}
		objList.clear();
	}
	template<int Length, typename T, typename TypeCheck0 = typename IsSubClassOf<ClassType, T>::mType>
	void destroyClassList(ArrayList<Length, T*>& objList)
	{
		checkDestroyClassThread();

		// 如果当前对象池已经被销毁,则不能再重复销毁任何对象
		if (mDestroied || objList.isEmpty())
		{
			return;
		}
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
#endif
		for (T* obj : objList)
		{
			if (obj == nullptr)
			{
				continue;
			}
			const KeyType savedType = obj->getType();
			obj->destroy();
			if (!obj->markDispose(this))
			{
				ERROR_PROFILE((string("4重复销毁对象:") + typeid(ClassType).name()).c_str());
				continue;
			}
			obj->setType(savedType);
#if (defined WINDOWS) && (!defined STRESS_TEST)
			mClassRemainTimeList.remove(obj);
#endif
			pushToFreeQueue(obj);
		}
		objList.clear();
	}
	void dump() override
	{
		for (const auto& item : mTotalCount)
		{
			const int itemCount = item.second.second;
			if (itemCount > 1000)
			{
				const int poolCount = mPool.get(item.first).size();
				LOG("ClassTypePoolThread: " + item.second.first + "的数量:" + IToS(itemCount) + ",总大小:" + LLToS(itemCount * sizeof(ClassType) / 1024) + "KB" + ", 池中数量:" + IToS(poolCount));
			}
		}
	}
	// 通知此对象池,调用newClass的线程已经被销毁,需要清空对这个线程的ID记录
	void notifyNewClassThreadDestroy()
	{
		mNewClassThreadID = 0;
	}
protected:
	virtual ClassType* create(const KeyType type) = 0;
	void onSecond() override
	{
		if (mFailedCount > 0)
		{
			LOG("入队失败数量:" + IToS(mFailedCount));
			mFailedCount = 0;
		}
#if (defined WINDOWS) && (!defined STRESS_TEST)
		THREAD_LOCK(mRemainTimeLock);
		for (auto& item : mClassRemainTimeList)
		{
			item.second -= 1.0f;
			if (item.second <= 0.0f)
			{
				LOG(string("ClassTypePoolThread: ") + typeid(*item.first).name() + ",对象内存泄漏");
			}
		}
#endif
	}
	ClassType* prepareObject(ClassType* obj, const KeyType type)
	{
		obj->markUsable(this, ++mAssignIDSeed);
		obj->setType(type);
		return obj;
	}
	// 将回收的对象推入跨线程队列，SPSCQueue 满时回退到加锁溢出容器
	void pushToFreeQueue(ClassType* obj)
	{
		const auto& key = obj->getType();
		auto& willList = mWillFreeList.addOrGet(key);
		willList.add(obj);
		if (willList.size() >= 256)
		{
			bool hasFailed = false;
			FOR_VECTOR(willList)
			{
				if (!mFreeQueue->push(willList[i]))
				{
					willList.removeRange(0, i);
					hasFailed = true;
					break;
				}
			}
			if (!hasFailed)
			{
				willList.clear();
			}
			if (!willList.isEmpty())
			{
				mFailedCount += willList.size();
				// SPSCQueue 已满，回退到加锁溢出容器
				THREAD_LOCK(mOverflowLock);
				mOverflow.addOrGet(key).addRange(willList);
				willList.clear();
			}
		}
	}
	// 将回收队列中的对象归还到本地池（仅线程1调用）
	void recycleToPool()
	{
		// 1. 先 drain 无锁队列
		while (ClassType* recycled = mFreeQueue->pop())
		{
			mPool.addOrGet(recycled->getType()).add(recycled);
		}
		// 2. 再 drain 溢出容器（加锁，但溢出是低频路径）
		if (!mOverflow.isEmpty())
		{
			HashMap<KeyType, Vector<ClassType*>> localOverflow;
			{
				THREAD_LOCK(mOverflowLock);
				for (auto& item : mOverflow)
				{
					localOverflow.addOrGet(item.first) = move(item.second);
				}
			}
			for (auto& item : localOverflow)
			{
				mPool.addOrGet(item.first).addRange(item.second);
			}
		}
	}
	void checkNewClassThread()
	{
		const int threadID = getThreadID();
		if (mNewClassThreadID == 0)
		{
			mNewClassThreadID = threadID;
		}
		if (mNewClassThreadID != threadID)
		{
			ERROR("不能在多个线程中调用ClassTypePoolThread的newClass");
		}
	}
	void checkDestroyClassThread()
	{
		const int threadID = getThreadID();
		if (mDestroyClassThreadID == 0)
		{
			mDestroyClassThreadID = threadID;
		}
		if (mDestroyClassThreadID != threadID)
		{
			ERROR("不能在多个线程中调用ClassTypePoolThread的destroyClass");
		}
	}
protected:
#if (defined WINDOWS) && (!defined STRESS_TEST)
	HashMap<ClassType*, float> mClassRemainTimeList;
	ThreadLock mRemainTimeLock;
#endif
	HashMap<KeyType, Vector<ClassType*>> mPool;			// 本地缓存，仅线程1访问，零竞争
	SPSCQueue<ClassType, 1 << 20>* mFreeQueue;			// 无锁队列：线程2 push，线程1 pop（快路径）
	HashMap<KeyType, Vector<ClassType*>> mWillFreeList;	// 缓存即将进入mFreeQueue的对象
	HashMap<KeyType, Vector<ClassType*>> mOverflow;		// 溢出容器：队列满时加锁写入（慢路径兜底）
	ThreadLock mOverflowLock;							// 保护 mOverflow
	HashMap<KeyType, pair<string, int>> mTotalCount;
	ThreadLock mTotalCountLock;							// 仅统计时用，低频
	llong mAssignIDSeed;
	int mNewClassThreadID = 0;
	int mDestroyClassThreadID = 0;
	bool mShowCountLog = true;
	int mFailedCount = 0;
};
