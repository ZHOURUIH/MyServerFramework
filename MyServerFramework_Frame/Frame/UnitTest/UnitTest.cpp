#ifdef WINDOWS
#include "FrameHeader.h"
#include "TCPServerIOBenchmark.h"

void UnitTest::test()
{
	llong start = getRealTimeMS();
	LOG("开始单元测试...");
	//TCPServerIOBenchmark::test();
#ifdef _MYSQL
	MySQLCacheTableTest::test();
	MySQLCacheDataTest::test();
#endif
	SerializeTest::test();
	FrameUtilityTest::test();
	PacketTest::test();
	GeometryTest::test();
	SerializerTest::test();
	TimePointTest::test();
	AnimationCurveTest::test();
	FileContentTest::test();
	MathUtilityTest::test();
	SafeVectorTest::test();
	BinaryUtilityTest::test();
	DoubleBufferTest::test();
	ClassPoolTest::test();
	VectorTest::test();
	HashMapTest::test();
	MathStructTest::test();
	TypeUtilityTest::test();
	FileUtilityTest::test();
	ArrayTest::test();
	MapTest::test();
	SetTest::test();
	StackTest::test();
	ArrayListTest::test();
	SingletonTest::test();
	ClassTypePoolTest::test();
	ClassBaseTypePoolTest::test();
	VectorPoolTest::test();
	ProfilerScopeTest::test();
	MemoryTrackerTest::test();
	RandomSystemTest::test();
	StreamBufferTest::test();
	SystemUtilityTest::test();
	ClassKeyPoolTest::test();
	ValueModifyTest::test();
	QueueTest::test();
	ArrayPoolSingleTest::test();
	IsPODTypeTraitsTest::test();
	ClassObjectTest::test();
	PodPoolThreadTest::test();
	SafeScopeContainerTest::test();
	SafeMapTest::test();
	IntPoolThreadTest::test();
	ThreadLockTest::test();
	TickerTest::test();
	ArrayScopeTest::test();
	TimeValueTest::test();
	SafeHashMapTest::test();
	StringUtilityTest::test();
	EventSystemTest::test();
	SPSCQueueTest::test();
	QuickSortTest::test();
	CRC16Test::test();
	FactoryManagerTest::test();
	LogFileInfoTest::test();
	LogInfoTest::test();
	SafeFastDeepVectorTest::test();
	MovableObjectTest::test();
	CharacterTest::test();
	CommandReceiverTest::test();
	MyStringTest::test();
	SafeSetTest::test();
	LOG("结束单元测试...耗时:" + IToS((int)(getRealTimeMS() - start)) + "毫秒");
}
#endif
