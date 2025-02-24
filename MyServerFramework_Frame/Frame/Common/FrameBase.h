#pragma once

#include "FrameDefine.h"

// 下面包含自动生成的代码,以// FrameSystem为起始,以第一个空行或者};为结束
namespace FrameBase
{
	MICRO_LEGEND_FRAME_API void constructFrameDone();
	MICRO_LEGEND_FRAME_API void clearFrameSystem();

	// FrameSystem
	MICRO_LEGEND_FRAME_API extern ServerFramework* mServerFramework;
	MICRO_LEGEND_FRAME_API extern CharacterManager* mCharacterManager;
	MICRO_LEGEND_FRAME_API extern VectorPoolManager* mVectorPoolManager;
	MICRO_LEGEND_FRAME_API extern CommandSystem* mCommandSystem;
	MICRO_LEGEND_FRAME_API extern RandomSystem* mRandomSystem;
	MICRO_LEGEND_FRAME_API extern FrameConfigSystem* mFrameConfigSystem;
	MICRO_LEGEND_FRAME_API extern ExcelManager* mExcelManager;
#ifdef _MYSQL
	MICRO_LEGEND_FRAME_API extern MySQLManager* mMySQLManager;
#endif
	MICRO_LEGEND_FRAME_API extern SQLiteManager* mSQLiteManager;
	MICRO_LEGEND_FRAME_API extern EventSystem* mEventSystem;
	MICRO_LEGEND_FRAME_API extern InputSystem* mInputSystem;
	MICRO_LEGEND_FRAME_API extern KeyframeManager* mKeyframeManager;
	MICRO_LEGEND_FRAME_API extern LogSystem* mLogSystem;
	MICRO_LEGEND_FRAME_API extern HttpServerSystem* mHttpServerSystem;
	MICRO_LEGEND_FRAME_API extern TCPServerSystem* mTCPServerSystem;
	MICRO_LEGEND_FRAME_API extern UDPServerSystem* mUDPServerSystem;
	MICRO_LEGEND_FRAME_API extern WebSocketServerSystem* mWebSocketServerSystem;
	MICRO_LEGEND_FRAME_API extern FrameStateManager* mFrameStateManager;
	MICRO_LEGEND_FRAME_API extern ThreadManager* mThreadManager;
	MICRO_LEGEND_FRAME_API extern TimePointSystem* mTimePointSystem;
	MICRO_LEGEND_FRAME_API extern TimeTaskSystem* mTimeTaskSystem;
	MICRO_LEGEND_FRAME_API extern ArrayPool* mArrayPool;
	MICRO_LEGEND_FRAME_API extern ArrayPoolThread* mArrayPoolThread;
	MICRO_LEGEND_FRAME_API extern IntPoolThread* mIntPoolThread;
	MICRO_LEGEND_FRAME_API extern CharacterPool* mCharacterPool;
	MICRO_LEGEND_FRAME_API extern CommandPool* mCommandPool;
	MICRO_LEGEND_FRAME_API extern CommandThreadPool* mCommandThreadPool;
	MICRO_LEGEND_FRAME_API extern CounterThreadPool* mCounterThreadPool;
	MICRO_LEGEND_FRAME_API extern DelayCommandPool* mDelayCommandPool;
	MICRO_LEGEND_FRAME_API extern DTNodePool* mDTNodePool;
	MICRO_LEGEND_FRAME_API extern EventInfoPool* mEventInfoPool;
	MICRO_LEGEND_FRAME_API extern GameComponentPool* mGameComponentPool;
	MICRO_LEGEND_FRAME_API extern LogInfoPool* mLogInfoPool;
#ifdef _MYSQL
	MICRO_LEGEND_FRAME_API extern MySQLCacheDataPool* mMySQLCacheDataPool;
#endif
#ifdef _MYSQL
	MICRO_LEGEND_FRAME_API extern MySQLDataPool* mMySQLDataPool;
#endif
	MICRO_LEGEND_FRAME_API extern PacketTCPPool* mPacketTCPPool;
	MICRO_LEGEND_FRAME_API extern PacketTCPThreadPool* mPacketTCPThreadPool;
	MICRO_LEGEND_FRAME_API extern PacketWebSocketPool* mPacketWebSocketPool;
	MICRO_LEGEND_FRAME_API extern PacketWebSocketThreadPool* mPacketWebSocketThreadPool;
	MICRO_LEGEND_FRAME_API extern StateParamPool* mStateParamPool;
	MICRO_LEGEND_FRAME_API extern StatePool* mStatePool;
	MICRO_LEGEND_FRAME_API extern TimePointPool* mTimePointPool;
	MICRO_LEGEND_FRAME_API extern CharacterFactoryManager* mCharacterFactoryManager;
	MICRO_LEGEND_FRAME_API extern CharacterStateFactoryManager* mCharacterStateFactoryManager;
	MICRO_LEGEND_FRAME_API extern GameComponentFactoryManager* mGameComponentFactoryManager;
	MICRO_LEGEND_FRAME_API extern PacketTCPFactoryManager* mPacketTCPFactoryManager;
	MICRO_LEGEND_FRAME_API extern PacketWebSocketFactoryManager* mPacketWebSocketFactoryManager;
	MICRO_LEGEND_FRAME_API extern StateParamFactoryManager* mStateParamFactoryManager;
};

using namespace FrameBase;