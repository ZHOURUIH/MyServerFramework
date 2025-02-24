#include "FrameHeader.h"

// 下面包含自动生成的代码,以// FrameSystem为起始,以第一个空行或者}为结束

namespace FrameBase
{
	// FrameSystem Define
	ServerFramework* mServerFramework;
	CharacterManager* mCharacterManager;
	VectorPoolManager* mVectorPoolManager;
	CommandSystem* mCommandSystem;
	RandomSystem* mRandomSystem;
	FrameConfigSystem* mFrameConfigSystem;
	ExcelManager* mExcelManager;
#ifdef _MYSQL
	MySQLManager* mMySQLManager;
#endif
	SQLiteManager* mSQLiteManager;
	EventSystem* mEventSystem;
	InputSystem* mInputSystem;
	KeyframeManager* mKeyframeManager;
	LogSystem* mLogSystem;
	HttpServerSystem* mHttpServerSystem;
	TCPServerSystem* mTCPServerSystem;
	UDPServerSystem* mUDPServerSystem;
	WebSocketServerSystem* mWebSocketServerSystem;
	FrameStateManager* mFrameStateManager;
	ThreadManager* mThreadManager;
	TimePointSystem* mTimePointSystem;
	TimeTaskSystem* mTimeTaskSystem;
	ArrayPool* mArrayPool;
	ArrayPoolThread* mArrayPoolThread;
	IntPoolThread* mIntPoolThread;
	CharacterPool* mCharacterPool;
	CommandPool* mCommandPool;
	CommandThreadPool* mCommandThreadPool;
	CounterThreadPool* mCounterThreadPool;
	DelayCommandPool* mDelayCommandPool;
	DTNodePool* mDTNodePool;
	EventInfoPool* mEventInfoPool;
	GameComponentPool* mGameComponentPool;
	LogInfoPool* mLogInfoPool;
#ifdef _MYSQL
	MySQLCacheDataPool* mMySQLCacheDataPool;
#endif
#ifdef _MYSQL
	MySQLDataPool* mMySQLDataPool;
#endif
	PacketTCPPool* mPacketTCPPool;
	PacketTCPThreadPool* mPacketTCPThreadPool;
	PacketWebSocketPool* mPacketWebSocketPool;
	PacketWebSocketThreadPool* mPacketWebSocketThreadPool;
	StateParamPool* mStateParamPool;
	StatePool* mStatePool;
	TimePointPool* mTimePointPool;
	CharacterFactoryManager* mCharacterFactoryManager;
	CharacterStateFactoryManager* mCharacterStateFactoryManager;
	GameComponentFactoryManager* mGameComponentFactoryManager;
	PacketTCPFactoryManager* mPacketTCPFactoryManager;
	PacketWebSocketFactoryManager* mPacketWebSocketFactoryManager;
	StateParamFactoryManager* mStateParamFactoryManager;

	void constructFrameDone()
	{
		// FrameSystem Get
		mServerFramework = ServerFramework::getSingleton();
		mServerFramework->getSystem(STR(CharacterManager), mCharacterManager);
		mServerFramework->getSystem(STR(VectorPoolManager), mVectorPoolManager);
		mServerFramework->getSystem(STR(CommandSystem), mCommandSystem);
		mServerFramework->getSystem(STR(RandomSystem), mRandomSystem);
		mServerFramework->getSystem(STR(FrameConfigSystem), mFrameConfigSystem);
		mServerFramework->getSystem(STR(ExcelManager), mExcelManager);
#ifdef _MYSQL
		mServerFramework->getSystem(STR(MySQLManager), mMySQLManager);
#endif
		mServerFramework->getSystem(STR(SQLiteManager), mSQLiteManager);
		mServerFramework->getSystem(STR(EventSystem), mEventSystem);
		mServerFramework->getSystem(STR(InputSystem), mInputSystem);
		mServerFramework->getSystem(STR(KeyframeManager), mKeyframeManager);
		mServerFramework->getSystem(STR(LogSystem), mLogSystem);
		mServerFramework->getSystem(STR(HttpServerSystem), mHttpServerSystem);
		mServerFramework->getSystem(STR(TCPServerSystem), mTCPServerSystem);
		mServerFramework->getSystem(STR(UDPServerSystem), mUDPServerSystem);
		mServerFramework->getSystem(STR(WebSocketServerSystem), mWebSocketServerSystem);
		mServerFramework->getSystem(STR(FrameStateManager), mFrameStateManager);
		mServerFramework->getSystem(STR(ThreadManager), mThreadManager);
		mServerFramework->getSystem(STR(TimePointSystem), mTimePointSystem);
		mServerFramework->getSystem(STR(TimeTaskSystem), mTimeTaskSystem);
		mServerFramework->getSystem(STR(ArrayPool), mArrayPool);
		mServerFramework->getSystem(STR(ArrayPoolThread), mArrayPoolThread);
		mServerFramework->getSystem(STR(IntPoolThread), mIntPoolThread);
		mServerFramework->getSystem(STR(CharacterPool), mCharacterPool);
		mServerFramework->getSystem(STR(CommandPool), mCommandPool);
		mServerFramework->getSystem(STR(CommandThreadPool), mCommandThreadPool);
		mServerFramework->getSystem(STR(CounterThreadPool), mCounterThreadPool);
		mServerFramework->getSystem(STR(DelayCommandPool), mDelayCommandPool);
		mServerFramework->getSystem(STR(DTNodePool), mDTNodePool);
		mServerFramework->getSystem(STR(EventInfoPool), mEventInfoPool);
		mServerFramework->getSystem(STR(GameComponentPool), mGameComponentPool);
		mServerFramework->getSystem(STR(LogInfoPool), mLogInfoPool);
#ifdef _MYSQL
		mServerFramework->getSystem(STR(MySQLCacheDataPool), mMySQLCacheDataPool);
#endif
#ifdef _MYSQL
		mServerFramework->getSystem(STR(MySQLDataPool), mMySQLDataPool);
#endif
		mServerFramework->getSystem(STR(PacketTCPPool), mPacketTCPPool);
		mServerFramework->getSystem(STR(PacketTCPThreadPool), mPacketTCPThreadPool);
		mServerFramework->getSystem(STR(PacketWebSocketPool), mPacketWebSocketPool);
		mServerFramework->getSystem(STR(PacketWebSocketThreadPool), mPacketWebSocketThreadPool);
		mServerFramework->getSystem(STR(StateParamPool), mStateParamPool);
		mServerFramework->getSystem(STR(StatePool), mStatePool);
		mServerFramework->getSystem(STR(TimePointPool), mTimePointPool);
		mServerFramework->getSystem(STR(CharacterFactoryManager), mCharacterFactoryManager);
		mServerFramework->getSystem(STR(CharacterStateFactoryManager), mCharacterStateFactoryManager);
		mServerFramework->getSystem(STR(GameComponentFactoryManager), mGameComponentFactoryManager);
		mServerFramework->getSystem(STR(PacketTCPFactoryManager), mPacketTCPFactoryManager);
		mServerFramework->getSystem(STR(PacketWebSocketFactoryManager), mPacketWebSocketFactoryManager);
		mServerFramework->getSystem(STR(StateParamFactoryManager), mStateParamFactoryManager);
	}

	void clearFrameSystem()
	{
		// FrameSystem Clear
		mServerFramework = nullptr;
		mCharacterManager = nullptr;
		mVectorPoolManager = nullptr;
		mCommandSystem = nullptr;
		mRandomSystem = nullptr;
		mFrameConfigSystem = nullptr;
		mExcelManager = nullptr;
#ifdef _MYSQL
		mMySQLManager = nullptr;
#endif
		mSQLiteManager = nullptr;
		mEventSystem = nullptr;
		mInputSystem = nullptr;
		mKeyframeManager = nullptr;
		mLogSystem = nullptr;
		mHttpServerSystem = nullptr;
		mTCPServerSystem = nullptr;
		mUDPServerSystem = nullptr;
		mWebSocketServerSystem = nullptr;
		mFrameStateManager = nullptr;
		mThreadManager = nullptr;
		mTimePointSystem = nullptr;
		mTimeTaskSystem = nullptr;
		mArrayPool = nullptr;
		mArrayPoolThread = nullptr;
		mIntPoolThread = nullptr;
		mCharacterPool = nullptr;
		mCommandPool = nullptr;
		mCommandThreadPool = nullptr;
		mCounterThreadPool = nullptr;
		mDelayCommandPool = nullptr;
		mDTNodePool = nullptr;
		mEventInfoPool = nullptr;
		mGameComponentPool = nullptr;
		mLogInfoPool = nullptr;
#ifdef _MYSQL
		mMySQLCacheDataPool = nullptr;
#endif
#ifdef _MYSQL
		mMySQLDataPool = nullptr;
#endif
		mPacketTCPPool = nullptr;
		mPacketTCPThreadPool = nullptr;
		mPacketWebSocketPool = nullptr;
		mPacketWebSocketThreadPool = nullptr;
		mStateParamPool = nullptr;
		mStatePool = nullptr;
		mTimePointPool = nullptr;
		mCharacterFactoryManager = nullptr;
		mCharacterStateFactoryManager = nullptr;
		mGameComponentFactoryManager = nullptr;
		mPacketTCPFactoryManager = nullptr;
		mPacketWebSocketFactoryManager = nullptr;
		mStateParamFactoryManager = nullptr;
	}
}