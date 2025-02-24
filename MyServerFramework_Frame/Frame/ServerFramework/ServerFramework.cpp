#include "FrameHeader.h"

template<>ServerFramework* Singleton<ServerFramework>::ms_Singleton = nullptr;

ServerFramework::ServerFramework():
	mStop(false)
{
	Dump::init();
#ifdef LINUX
	struct timeval startTime;
	gettimeofday(&startTime, nullptr);
	mStartMiliTime = startTime.tv_sec * 1000 + (int)(startTime.tv_usec * 0.001f);
#endif
}

// 下面包含自动生成的代码,以FrameSystem Register为起始关键字,以后面的第一个空行或者}为结束
void ServerFramework::registeComponent()
{
	// FrameSystem Register
	registeSystem<CharacterManager>(STR(CharacterManager));
	registeSystem<VectorPoolManager>(STR(VectorPoolManager));
	registeSystem<CommandSystem>(STR(CommandSystem));
	registeSystem<RandomSystem>(STR(RandomSystem));
	registeSystem<FrameConfigSystem>(STR(FrameConfigSystem));
	registeSystem<ExcelManager>(STR(ExcelManager));
#ifdef _MYSQL
	registeSystem<MySQLManager>(STR(MySQLManager));
#endif
	registeSystem<SQLiteManager>(STR(SQLiteManager));
	registeSystem<EventSystem>(STR(EventSystem));
	registeSystem<InputSystem>(STR(InputSystem));
	registeSystem<KeyframeManager>(STR(KeyframeManager));
	registeSystem<LogSystem>(STR(LogSystem));
	registeSystem<HttpServerSystem>(STR(HttpServerSystem));
	registeSystem<TCPServerSystem>(STR(TCPServerSystem));
	registeSystem<UDPServerSystem>(STR(UDPServerSystem));
	registeSystem<WebSocketServerSystem>(STR(WebSocketServerSystem));
	registeSystem<FrameStateManager>(STR(FrameStateManager));
	registeSystem<ThreadManager>(STR(ThreadManager));
	registeSystem<TimePointSystem>(STR(TimePointSystem));
	registeSystem<TimeTaskSystem>(STR(TimeTaskSystem));
	registeSystem<ArrayPool>(STR(ArrayPool));
	registeSystem<ArrayPoolThread>(STR(ArrayPoolThread));
	registeSystem<IntPoolThread>(STR(IntPoolThread));
	registeSystem<CharacterPool>(STR(CharacterPool));
	registeSystem<CommandPool>(STR(CommandPool));
	registeSystem<CommandThreadPool>(STR(CommandThreadPool));
	registeSystem<CounterThreadPool>(STR(CounterThreadPool));
	registeSystem<DelayCommandPool>(STR(DelayCommandPool));
	registeSystem<DTNodePool>(STR(DTNodePool));
	registeSystem<EventInfoPool>(STR(EventInfoPool));
	registeSystem<GameComponentPool>(STR(GameComponentPool));
	registeSystem<LogInfoPool>(STR(LogInfoPool));
#ifdef _MYSQL
	registeSystem<MySQLCacheDataPool>(STR(MySQLCacheDataPool));
#endif
#ifdef _MYSQL
	registeSystem<MySQLDataPool>(STR(MySQLDataPool));
#endif
	registeSystem<PacketTCPPool>(STR(PacketTCPPool));
	registeSystem<PacketTCPThreadPool>(STR(PacketTCPThreadPool));
	registeSystem<PacketWebSocketPool>(STR(PacketWebSocketPool));
	registeSystem<PacketWebSocketThreadPool>(STR(PacketWebSocketThreadPool));
	registeSystem<StateParamPool>(STR(StateParamPool));
	registeSystem<StatePool>(STR(StatePool));
	registeSystem<TimePointPool>(STR(TimePointPool));
	registeSystem<CharacterFactoryManager>(STR(CharacterFactoryManager));
	registeSystem<CharacterStateFactoryManager>(STR(CharacterStateFactoryManager));
	registeSystem<GameComponentFactoryManager>(STR(GameComponentFactoryManager));
	registeSystem<PacketTCPFactoryManager>(STR(PacketTCPFactoryManager));
	registeSystem<PacketWebSocketFactoryManager>(STR(PacketWebSocketFactoryManager));
	registeSystem<StateParamFactoryManager>(STR(StateParamFactoryManager));
}

void ServerFramework::constructDone()
{
	constructFrameDone();
	// 由于其他系统组件的初始化可能会用到列表对象池,所以需要在初始化之前就注册对象池
	FrameSTLPoolRegister::registeAll();
}

void ServerFramework::registe()
{
	ComponentRegisterFrame::registeAll();
}

void ServerFramework::init()
{
	mLastDayOfWeek = getDayOfWeek();
	registeComponent();
	constructDone();
	// 初始化所有组件
	for (FrameComponent* component : mFrameComponentVector)
	{
		LOG(string("init:") + typeid(*component).name());
		component->init();
	}
	// 注册各个系统需要创建的对象类型
	registe();
	// 等待所有表格都注册完毕后才能检查表格数据
	mSQLiteManager->checkAll();
	mExcelManager->checkAll();
	for (FrameComponent* component : mFrameComponentVector)
	{
		LOG(string("lateInit:") + typeid(*component).name());
		component->lateInit();
	}
	for (FrameComponent* component : mFrameComponentVector)
	{
		LOG(string("finalInit:") + typeid(*component).name());
		component->finalInit();
	}
}

void ServerFramework::update(const float elapsedTime)
{
	++mFrameID;
	++mCurFrameCount;
	TICK_LOOP(elapsedTime, 1.0f)
	{
		for (const auto& item : mSecondCallback)
		{
			item.second(item.first);
		}
	}
	TICK_LOOP(elapsedTime, 60.0f)
	{
		for (const auto& item : mMinuteCallback)
		{
			item.second(item.first);
		}
	}
	TICK_LOOP(elapsedTime, 3600.0f)
	{
		for (const auto& item : mHourCallback)
		{
			item.second(item.first);
		}
	}
	TICK_LOOP(elapsedTime, 1.0f)
	{
		mFPS = mCurFrameCount;
		mCurFrameCount = 0;

		const int dayOfWeek = getDayOfWeek();
		if (mLastDayOfWeek != dayOfWeek)
		{
			if (mLastDayOfWeek == 7 && dayOfWeek == 0)
			{
				for (const auto& item : mWeekCallback)
				{
					item.second(item.first);
				}
			}
			mLastDayOfWeek = dayOfWeek;
			for (const auto& item : mDayCallback)
			{
				item.second(item.first);
			}
		}
	}
	// 每60秒显示一次每帧平均耗时
	TICK_LOOP(elapsedTime, 60.0f)
	{
		LOG("每帧平均耗时:" + FToS(divide((float)mAllFrameMS, (float)mAllFrameCount)));
		mAllFrameMS = 0;
		mAllFrameCount = 0;
	}
	// 每10秒显示一次帧率,或者按F键时也会打印一次
	if (mInputSystem->isKeyCurrentDown('f') || 
		mInputSystem->isKeyCurrentDown('F'))
	{
		LOG("主线程帧率:" + IToS(mFPS));
	}
	TICK_LOOP(elapsedTime, 10.0f)
	{
		LOG("主线程帧率:" + IToS(mFPS));
	}
	
	THREAD_LOCK(mLock);
	const llong time0 = getRealTimeMS();
	ArrayList<256, int> componentUseTime;
	// 正常更新
	FOR_VECTOR(mFrameComponentVector)
	{
		const llong componentTime0 = getRealTimeMS();
		mFrameComponentVector[i]->update(elapsedTime);
		componentUseTime.add((int)(getRealTimeMS() - componentTime0));
	}
	// 当这一帧消耗的时间超过80毫秒时,打印出最耗时的
	if (getRealTimeMS() - time0 >= 80)
	{
		FOR_VECTOR(mFrameComponentVector)
		{
			if (componentUseTime[i] > 5)
			{
				LOG("耗时超过5毫秒的组件:" + mFrameComponentVector[i]->getPrintName() + ", 时间:" + IToS(componentUseTime[i]) + "毫秒");
			}
		}
	}
	// 延迟更新
	for (FrameComponent* component : mFrameComponentVector)
	{
		component->lateUpdate(elapsedTime);
	}
}

void ServerFramework::destroy()
{
	THREAD_LOCK(mLock);
	// 通知所有系统组件退出,即将销毁所有组件,各个组件销毁释放自己的资源,优先释放非对象池,非工厂的系统,在释放资源时不需要再访问其他系统
	for (FrameComponent* system : mFrameComponentVector)
	{
		if (!mPoolSystemList.contains(system) && !mFactorySystemList.contains(system))
		{
			system->quit();
			system->setDestroied(true);
		}
	}

	// 再销毁对象池
	for (FrameComponent* iterPool : mPoolSystemList)
	{
		iterPool->quit();
		iterPool->setDestroied(true);
	}
	mPoolSystemList.clear();

	// 最后销毁工厂
	for (FrameComponent* iterFactory : mFactorySystemList)
	{
		iterFactory->quit();
		iterFactory->setDestroied(true);
	}
	mFactorySystemList.clear();

	// 清空所有系统组件的引用,避免在析构过程中访问出错
	clearSystem();

	// 因为释放内存会出现报错,所以不释放了
	// 在析构阶段,任何对象都不能再访问其他对象实例,只能访问自己
	for (FrameComponent* component : mFrameComponentVector)
	{
		delete component;
	}
	mFrameComponentVector.clear();
	mFrameComponentMap.clear();
	LOG("关闭服务器!");
}

void ServerFramework::launch()
{
	if (mTCPServerSystem != nullptr && mTCPServerSystem->isAvailable())
	{
		LOG("启动TCP服务器, 端口 : " + IToS(mTCPServerSystem->getPort()));
	}
	else if (mWebSocketServerSystem != nullptr && mWebSocketServerSystem->isAvailable())
	{
		LOG("启动WebSocket服务器, 端口 : " + IToS(mWebSocketServerSystem->getPort()));
	}
	else
	{
		LOG("启动服务器失败!");
	}
}

void ServerFramework::clearSystem()
{
	clearFrameSystem();
}