#ifdef WINDOWS
#include "FrameHeader.h"
namespace SafeVectorTestSupport
{
	// SafeVectorTestSupport_SV_ASSERT: SafeVectorTest 专用断言宏
#define SafeVectorTestSupport_SV_ASSERT(expr) \
	if (!(expr)) { ERROR("SafeVectorTest FAILED: " #expr); }
}

void SafeVectorTest::testSafeVectorBasic()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	SafeVectorTestSupport_SV_ASSERT(sv.isEmpty());
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 0);
	SafeVectorTestSupport_SV_ASSERT(!sv.contains(1));

	// add
	sv.add(10);
	sv.add(20);
	sv.add(30);
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(!sv.isEmpty());
	SafeVectorTestSupport_SV_ASSERT(sv.contains(10));
	SafeVectorTestSupport_SV_ASSERT(sv.contains(20));
	SafeVectorTestSupport_SV_ASSERT(sv.contains(30));
	SafeVectorTestSupport_SV_ASSERT(!sv.contains(99));

	// getMainList
	const auto& ml = sv.getMainList();
	SafeVectorTestSupport_SV_ASSERT(ml.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(ml[0] == 10);
	SafeVectorTestSupport_SV_ASSERT(ml[1] == 20);
	SafeVectorTestSupport_SV_ASSERT(ml[2] == 30);

	// removeAt
	SafeVectorTestSupport_SV_ASSERT(sv.removeAt(1));  // 移除20
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!sv.contains(20));
	SafeVectorTestSupport_SV_ASSERT(!sv.removeAt(5));  // 越界返回 false

	// remove
	sv.add(40);
	SafeVectorTestSupport_SV_ASSERT(sv.remove(40));
	SafeVectorTestSupport_SV_ASSERT(!sv.contains(40));
	SafeVectorTestSupport_SV_ASSERT(!sv.remove(99));  // 不存在
}

void SafeVectorTest::testSafeVectorClear()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	FOR(10)
	{
		sv.add(i);
	}
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 10);
	sv.clear();
	SafeVectorTestSupport_SV_ASSERT(sv.isEmpty());
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 0);

	// 再次 push
	sv.add(1);
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 1);
}

void SafeVectorTest::testSafeVectorNonForeachSync()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	// 非遍历时 startForeach 返回的 updateList 应与 mainList 一致
	const auto& readList = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 3);
	sv.endForeach();

	// 移除后再 startForeach
	sv.remove(2);
	const auto& readList2 = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 2);
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorForeachPush()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);

	// 在 foreach 中添加元素：mainList 立即更新，updateList 本次不包含新加的
	const auto& readList = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(sv.isForeaching());

	// 在遍历中添加
	sv.add(100);
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 4);     // mainList 已更新
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 3);  // updateList 本次不变

	sv.endForeach();
	// 遍历结束后 sync，updateList 应与 mainList 一致
	const auto& readList2 = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 4);
	SafeVectorTestSupport_SV_ASSERT(sv.contains(100));
	sv.endForeach();

	SafeVectorTestSupport_SV_ASSERT(!sv.isForeaching());
}

void SafeVectorTest::testSafeVectorForeachErase()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.add(40);

	const auto& readList = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 4);

	// 在遍历中移除：mainList 立即更新，updateList 元素被 reset 为默认值
	sv.remove(20);
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 3);  // mainList 已删除
	// updateList 由于在遍历中，元素只是被 reset 为 0（int 的默认值）

	sv.endForeach();
	// endForeach 后 clearDefaultElement 清理 reset 的元素
	// 再次 startForeach 应得到与 mainList 一致的数据
	const auto& readList2 = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(!sv.contains(20));
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorForeachAddThenRemove()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int*> sv;
	int value0 = 0;
	int value1 = 1;
	int value2 = 2;
	int value3 = 3;
	sv.add(&value0);
	sv.add(&value1);
	sv.add(&value2);
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 3);
	const auto& readList = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(!readList.contains(&value3));
	// 遍历过程中新增value3,此时只进入mMainList,当前遍历使用的mUpdateList中不存在value3
	sv.add(&value3);
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 4);
	SafeVectorTestSupport_SV_ASSERT(sv.contains(&value3));
	SafeVectorTestSupport_SV_ASSERT(!readList.contains(&value3));
	// 同一次遍历中立即删除刚新增的value3
	SafeVectorTestSupport_SV_ASSERT(sv.remove(&value3));
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(!sv.contains(&value3));
	SafeVectorTestSupport_SV_ASSERT(!readList.contains(&value3));
	sv.endForeach();
	// 下一次遍历时,mUpdateList必须与mMainList保持一致
	const auto& readList2 = sv.startForeach();
	const int mainSize = sv.size();
	const int updateSize = readList2.size();
	const bool mainContains = sv.contains(&value3);
	const bool updateContains = readList2.contains(&value3);
	if (mainSize != 3 || mainContains || updateSize != 3 || updateContains)
	{
		ERROR("SafeVector遍历中add后remove问题已复现,mainSize:" + IToS(mainSize) + ",updateSize:" + IToS(updateSize) + ",mainContains:" + IToS((int)mainContains) + ",updateContains:" + IToS((int)updateContains));
	}
	SafeVectorTestSupport_SV_ASSERT(mainSize == 3);
	SafeVectorTestSupport_SV_ASSERT(!mainContains);
	SafeVectorTestSupport_SV_ASSERT(updateSize == 3);
	SafeVectorTestSupport_SV_ASSERT(!updateContains);
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorForeachClear()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	for (int i = 1; i <= 5; ++i)
	{
		sv.add(i);
	}

	const auto& readList = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 5);

	sv.clear();  // 遍历中清空
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 0);

	sv.endForeach();
	SafeVectorTestSupport_SV_ASSERT(sv.isEmpty());
	const auto& readList2 = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 0);
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorData()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	sv.add(5);
	sv.add(10);
	sv.add(15);

	int* ptr = sv.data();
	SafeVectorTestSupport_SV_ASSERT(ptr[0] == 5 && ptr[1] == 10 && ptr[2] == 15);
}

void SafeVectorTest::testSafeVectorStress()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	// 批量插入 100 个
	FOR(100)
	{
		sv.add(i);
	}
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 100);

	// 逐一删除奇数
	for (int i = 1; i < 100; i += 2)
	{
		sv.remove(i);
	}
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 50);
	for (int i = 1; i < 100; i += 2)
	{
		SafeVectorTestSupport_SV_ASSERT(!sv.contains(i));
	}
	for (int i = 0; i < 100; i += 2)
	{
		SafeVectorTestSupport_SV_ASSERT(sv.contains(i));
	}

	// 遍历中批量插入
	const auto& readList = sv.startForeach();
	int countBefore = readList.size();  // 50
	for (int i = 200; i < 210; ++i)
	{
		sv.add(i);
	}
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 60);
	SafeVectorTestSupport_SV_ASSERT(readList.size() == (int)countBefore);  // updateList 未变
	sv.endForeach();

	// 遍历结束后同步
	const auto& readList2 = sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 60);
	sv.endForeach();
}

void SafeVectorTest::testSafeHashMapBasic()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, string> shm;
	SafeVectorTestSupport_SV_ASSERT(shm.isEmpty());
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 0);
	SafeVectorTestSupport_SV_ASSERT(!shm.contains(1));

	// add
	SafeVectorTestSupport_SV_ASSERT(shm.add(1, "one"));
	SafeVectorTestSupport_SV_ASSERT(shm.add(2, "two"));
	SafeVectorTestSupport_SV_ASSERT(shm.add(3, "three"));
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(shm.contains(1));
	SafeVectorTestSupport_SV_ASSERT(shm.contains(2));
	SafeVectorTestSupport_SV_ASSERT(!shm.contains(99));

	// 重复 add 返回 false
	SafeVectorTestSupport_SV_ASSERT(!shm.add(1, "ONE"));
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 3);

	// getPtr
	const string* p = shm.getPtrConst(1);
	SafeVectorTestSupport_SV_ASSERT(p != nullptr && *p == "one");
	SafeVectorTestSupport_SV_ASSERT(shm.getPtrConst(99) == nullptr);

	// get
	const string& val = shm.get(2, string("default"));
	SafeVectorTestSupport_SV_ASSERT(val == "two");
	// 注意：get 返回 const ref，不能对临时 defaultValue 使用；key 不存在时用 getPtr 判空
	SafeVectorTestSupport_SV_ASSERT(shm.getPtr(99) == nullptr);  // 确认 key=99 不存在

	// remove
	SafeVectorTestSupport_SV_ASSERT(shm.remove(2));
	SafeVectorTestSupport_SV_ASSERT(!shm.contains(2));
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!shm.remove(99));
}

void SafeVectorTest::testSafeHashMapClear()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, int> shm;
	FOR(10)
	{
		shm.add(i, i * 10);
	}
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 10);
	shm.clear();
	SafeVectorTestSupport_SV_ASSERT(shm.isEmpty());

	// 再次插入
	shm.add(1, 100);
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 1);
}

void SafeVectorTest::testSafeHashMapForeachInsert()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, int> shm;
	shm.add(1, 10);
	shm.add(2, 20);

	const auto& readList = shm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(shm.isForeaching());

	// 遍历中插入：mainList 立即更新，updateList 本次不变
	shm.add(3, 30);
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 2);

	shm.endForeach();
	// 结束后 sync
	const auto& readList2 = shm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(shm.contains(3));
	shm.endForeach();
}

void SafeVectorTest::testSafeHashMapForeachErase()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, int> shm;
	shm.add(1, 10);
	shm.add(2, 20);
	shm.add(3, 30);

	const auto& readList = shm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 3);

	shm.remove(2);
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 3);  // updateList 未同步

	shm.endForeach();
	const auto& readList2 = shm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!shm.contains(2));
	shm.endForeach();
}

void SafeVectorTest::testSafeHashMapEraseWithValue()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, int> shm;
	shm.add(10, 100);
	shm.add(20, 200);

	int outVal = 0;
	SafeVectorTestSupport_SV_ASSERT(shm.remove(10, outVal));
	SafeVectorTestSupport_SV_ASSERT(outVal == 100);
	SafeVectorTestSupport_SV_ASSERT(!shm.contains(10));

	// 不存在的 key
	SafeVectorTestSupport_SV_ASSERT(!shm.remove(99, outVal));
}

void SafeVectorTest::testSafeMapBasic()
{
	using namespace SafeVectorTestSupport;
	SafeMap<int, int> sm;
	SafeVectorTestSupport_SV_ASSERT(sm.isEmpty());
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 0);

	SafeVectorTestSupport_SV_ASSERT(sm.add(3, 30));
	SafeVectorTestSupport_SV_ASSERT(sm.add(1, 10));
	SafeVectorTestSupport_SV_ASSERT(sm.add(2, 20));
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(sm.contains(1));
	SafeVectorTestSupport_SV_ASSERT(sm.contains(2));
	SafeVectorTestSupport_SV_ASSERT(sm.contains(3));
	SafeVectorTestSupport_SV_ASSERT(!sm.contains(99));

	// 重复 add
	SafeVectorTestSupport_SV_ASSERT(!sm.add(1, 100));

	// getPtr
	int* p = sm.getPtr(2);
	SafeVectorTestSupport_SV_ASSERT(p != nullptr && *p == 20);
	SafeVectorTestSupport_SV_ASSERT(sm.getPtr(99) == nullptr);

	// get
	const int& v = sm.get(3, -1);
	SafeVectorTestSupport_SV_ASSERT(v == 30);
	const int& v2 = sm.get(99, -1);
	SafeVectorTestSupport_SV_ASSERT(v2 == -1);

	// remove
	SafeVectorTestSupport_SV_ASSERT(sm.remove(2));
	SafeVectorTestSupport_SV_ASSERT(!sm.contains(2));
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!sm.remove(99));
}

void SafeVectorTest::testSafeMapForeachModify()
{
	using namespace SafeVectorTestSupport;
	SafeMap<int, string> sm;
	sm.add(1, "a");
	sm.add(2, "b");

	const auto& readList = sm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 2);

	sm.add(3, "c");  // 遍历中插入
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 2);  // updateList 未变

	sm.remove(1);  // 遍历中删除
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(readList.size() == 2);

	sm.endForeach();
	const auto& readList2 = sm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readList2.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!sm.contains(1));
	SafeVectorTestSupport_SV_ASSERT(sm.contains(3));
	sm.endForeach();
}

void SafeVectorTest::testSafeMapClear()
{
	using namespace SafeVectorTestSupport;
	SafeMap<int, float> sm;
	FOR(5)
	{
		sm.add(i, (float)i);
	}
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 5);
	sm.clear();
	SafeVectorTestSupport_SV_ASSERT(sm.isEmpty());
}

void SafeVectorTest::testSafeSetBasic()
{
	using namespace SafeVectorTestSupport;
	SafeSet<int> ss;
	SafeVectorTestSupport_SV_ASSERT(ss.isEmpty());
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 0);

	SafeVectorTestSupport_SV_ASSERT(ss.add(10));
	SafeVectorTestSupport_SV_ASSERT(ss.add(20));
	SafeVectorTestSupport_SV_ASSERT(ss.add(30));
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(ss.contains(10));
	SafeVectorTestSupport_SV_ASSERT(!ss.contains(99));

	// 重复 add
	SafeVectorTestSupport_SV_ASSERT(!ss.add(10));
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 3);

	// getMainList
	const auto& ml = ss.getMainList();
	SafeVectorTestSupport_SV_ASSERT(ml.size() == 3);
}

void SafeVectorTest::testSafeSetErase()
{
	using namespace SafeVectorTestSupport;
	SafeSet<int> ss;
	ss.add(1);
	ss.add(2);
	ss.add(3);

	SafeVectorTestSupport_SV_ASSERT(ss.remove(2));
	SafeVectorTestSupport_SV_ASSERT(!ss.contains(2));
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!ss.remove(99));
}

void SafeVectorTest::testSafeSetForeachInsert()
{
	using namespace SafeVectorTestSupport;
	SafeSet<int> ss;
	ss.add(1);
	ss.add(2);
	ss.add(3);

	const auto& readSet = ss.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readSet.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(ss.isForeaching());

	ss.add(100);
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 4);
	SafeVectorTestSupport_SV_ASSERT(readSet.size() == 3);  // updateList 未变

	ss.endForeach();
	const auto& readSet2 = ss.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readSet2.size() == 4);
	SafeVectorTestSupport_SV_ASSERT(ss.contains(100));
	ss.endForeach();
}

void SafeVectorTest::testSafeSetForeachErase()
{
	using namespace SafeVectorTestSupport;
	SafeSet<int> ss;
	ss.add(10);
	ss.add(20);
	ss.add(30);

	const auto& readSet = ss.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readSet.size() == 3);

	ss.remove(20);
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(readSet.size() == 3);  // updateList 本次未变

	ss.endForeach();
	const auto& readSet2 = ss.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readSet2.size() == 2);
	SafeVectorTestSupport_SV_ASSERT(!ss.contains(20));
	ss.endForeach();
}

void SafeVectorTest::testSafeSetClear()
{
	using namespace SafeVectorTestSupport;
	SafeSet<int> ss;
	FOR(8)
	{
		ss.add(i);
	}
	SafeVectorTestSupport_SV_ASSERT(ss.size() == 8);
	ss.clear();
	SafeVectorTestSupport_SV_ASSERT(ss.isEmpty());
}

void SafeVectorTest::testSafeSetForeachSync()
{
	using namespace SafeVectorTestSupport;
	SafeSet<float> ss;
	ss.add(1.0f);
	ss.add(2.0f);
	ss.add(3.0f);

	const auto& readSet = ss.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readSet.size() == 3);
	ss.endForeach();

	ss.remove(1.0f);
	const auto& readSet2 = ss.startForeach();
	SafeVectorTestSupport_SV_ASSERT(readSet2.size() == 2);
	ss.endForeach();
}

void SafeVectorTest::testSafeVectorForeachState()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	SafeVectorTestSupport_SV_ASSERT(!sv.isForeaching());
	sv.startForeach();
	SafeVectorTestSupport_SV_ASSERT(sv.isForeaching());
	sv.endForeach();
	SafeVectorTestSupport_SV_ASSERT(!sv.isForeaching());
}

void SafeVectorTest::testSafeHashMapForeachState()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, int> shm;
	SafeVectorTestSupport_SV_ASSERT(!shm.isForeaching());
	shm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(shm.isForeaching());
	shm.endForeach();
	SafeVectorTestSupport_SV_ASSERT(!shm.isForeaching());
}

void SafeVectorTest::testSafeVectorMultiForeach()
{
	using namespace SafeVectorTestSupport;
	SafeVector<int> sv;
	for (int i = 1; i <= 10; ++i)
	{
		sv.add(i);
	}

	// 多次 foreach，每次删一个元素
	for (int round = 0; round < 5; ++round)
	{
		const auto& rl = sv.startForeach();
		if (!rl.isEmpty())
		{
			sv.remove(rl[0]);  // 在遍历中删第一个（读的是 updateList[0]）
		}
		sv.endForeach();
	}
	// 5轮删掉5个（每轮删的都是 updateList[0]，即上轮末尾 updateList 的第一个）
	SafeVectorTestSupport_SV_ASSERT(sv.size() == 5);
}

void SafeVectorTest::testSafeHashMapStress()
{
	using namespace SafeVectorTestSupport;
	SafeHashMap<int, int> shm;
	FOR(50)
	{
		shm.add(i, i * 100);
	}
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 50);

	// 删除偶数
	for (int i = 0; i < 50; i += 2)
	{
		shm.remove(i);
	}
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 25);
	for (int i = 0; i < 50; i += 2)
	{
		SafeVectorTestSupport_SV_ASSERT(!shm.contains(i));
	}
	for (int i = 1; i < 50; i += 2)
	{
		SafeVectorTestSupport_SV_ASSERT(shm.contains(i));
	}

	// 遍历中批量插入
	const auto& rl = shm.startForeach();
	int before = rl.size();
	for (int i = 100; i < 110; ++i)
	{
		shm.add(i, i);
	}
	SafeVectorTestSupport_SV_ASSERT(shm.size() == 35);
	SafeVectorTestSupport_SV_ASSERT(rl.size() == before);
	shm.endForeach();

	const auto& rl2 = shm.startForeach();
	SafeVectorTestSupport_SV_ASSERT(rl2.size() == 35);
	shm.endForeach();
}

void SafeVectorTest::testSafeMapComplex()
{
	using namespace SafeVectorTestSupport;
	SafeMap<string, int> sm;
	sm.add("apple", 1);
	sm.add("banana", 2);
	sm.add("cherry", 3);
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 3);
	SafeVectorTestSupport_SV_ASSERT(sm.contains("apple"));
	SafeVectorTestSupport_SV_ASSERT(!sm.contains("grape"));

	int* p = sm.getPtr("banana");
	SafeVectorTestSupport_SV_ASSERT(p != nullptr && *p == 2);

	sm.remove("banana");
	SafeVectorTestSupport_SV_ASSERT(!sm.contains("banana"));
	SafeVectorTestSupport_SV_ASSERT(sm.size() == 2);

	// get 默认值
	const int& v = sm.get("grape", -999);
	SafeVectorTestSupport_SV_ASSERT(v == -999);
}

namespace SafeVectorDeepTestSupport
{
#define SafeVectorDeepTestSupport_SVD_ASSERT(cond, msg) \
	if (!(cond)) { ERROR(string("SafeVectorDeepTest FAIL: ") + (msg)); }

	// ---------------------------------------------------------------------------
	// 辅助：计算 SafeVector<int> 快照行数（通过 startForeach/endForeach 读取）
	// ---------------------------------------------------------------------------
	// ---------------------------------------------------------------------------


	// ---------------------------------------------------------------------------






























	// ---------------------------------------------------------------------------
	// 额外覆盖：SafeVector<string> 类型
	// ---------------------------------------------------------------------------











	// SafeVectorTest::runExtraTests() 由 test() 显式调用
}

void SafeVectorTest::testBasicPushBack()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 3, "testBasicPushBack size");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.getMainList()[0] == 10, "testBasicPushBack [0]");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.getMainList()[1] == 20, "testBasicPushBack [1]");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.getMainList()[2] == 30, "testBasicPushBack [2]");
}

void SafeVectorTest::testBasicEraseAt()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	bool ok = sv.removeAt(1);
	SafeVectorDeepTestSupport_SVD_ASSERT(ok, "testBasicEraseAt return true");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "testBasicEraseAt size after remove");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.getMainList()[0] == 1, "testBasicEraseAt [0]");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.getMainList()[1] == 3, "testBasicEraseAt [1]");
}

void SafeVectorTest::testBasicEraseElement()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(100);
	sv.add(200);
	sv.add(300);
	bool ok = sv.remove(200);
	SafeVectorDeepTestSupport_SVD_ASSERT(ok, "testBasicEraseElement return true");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "testBasicEraseElement size");
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.contains(200), "testBasicEraseElement not contains 200");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.contains(100), "testBasicEraseElement still has 100");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.contains(300), "testBasicEraseElement still has 300");
}

void SafeVectorTest::testBasicClear()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	sv.clear();
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testBasicClear size");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isEmpty(), "testBasicClear isEmpty");
}

void SafeVectorTest::testSizeAndIsEmpty()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testSizeAndIsEmpty initial size");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isEmpty(), "testSizeAndIsEmpty initial isEmpty");
	sv.add(5);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 1, "testSizeAndIsEmpty after push size");
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.isEmpty(), "testSizeAndIsEmpty after push not empty");
	sv.removeAt(0);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testSizeAndIsEmpty after remove size");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isEmpty(), "testSizeAndIsEmpty after remove isEmpty");
}

void SafeVectorTest::testContains()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(42);
	sv.add(99);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.contains(42), "testContains 42");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.contains(99), "testContains 99");
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.contains(0), "testContains 0 not found");
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.contains(-1), "testContains -1 not found");
}

void SafeVectorTest::testGetMainList()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(7);
	sv.add(8);
	sv.add(9);
	const Vector<int>& ml = sv.getMainList();
	SafeVectorDeepTestSupport_SVD_ASSERT(ml.size() == 3, "testGetMainList size");
	SafeVectorDeepTestSupport_SVD_ASSERT(ml[0] == 7, "testGetMainList [0]");
	SafeVectorDeepTestSupport_SVD_ASSERT(ml[1] == 8, "testGetMainList [1]");
	SafeVectorDeepTestSupport_SVD_ASSERT(ml[2] == 9, "testGetMainList [2]");
}

void SafeVectorTest::testForeachFlag()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.isForeaching(), "testForeachFlag before start");
	sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isForeaching(), "testForeachFlag during foreach");
	sv.endForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.isForeaching(), "testForeachFlag after end");
}

void SafeVectorTest::testPushBackDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	// 开始遍历：快照为 {1,2}
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 2, "testPushBackDuringForeach snapshot size");
	// 遍历期间新增
	sv.add(3);
	// mainList 已更新
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 3, "testPushBackDuringForeach mainList updated");
	// 快照仍为旧值（updateList 未同步）
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 2, "testPushBackDuringForeach snapshot unchanged");
	sv.endForeach();
	// 结束后 updateList 同步为 {1,2,3}
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 3, "testPushBackDuringForeach after sync");
	sv.endForeach();
}

void SafeVectorTest::testEraseAtDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 3, "testEraseAtDuringForeach snapshot before");
	sv.removeAt(1); // 移除 20
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "testEraseAtDuringForeach mainList");
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 3, "testEraseAtDuringForeach snapshot unchanged");
	sv.endForeach();
	// 同步后
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 2, "testEraseAtDuringForeach synced size");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(20), "testEraseAtDuringForeach 20 removed");
	sv.endForeach();
}

void SafeVectorTest::testEraseElementDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(100);
	sv.add(200);
	sv.add(300);
	sv.startForeach();
	// 遍历期间 remove：主列表立即移除，updateList 重置元素（不直接删除）
	sv.remove(200);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "testEraseElementDuringForeach mainList size");
	sv.endForeach();
	// endForeach 后 clearDefaultElement 清理重置元素
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(200), "testEraseElementDuringForeach 200 gone");
	sv.endForeach();
}

void SafeVectorTest::testClearDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 3, "testClearDuringForeach snapshot before");
	sv.clear();
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testClearDuringForeach mainList cleared");
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 3, "testClearDuringForeach snapshot unchanged during");
	sv.endForeach();
	// 同步后
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 0, "testClearDuringForeach synced empty");
	sv.endForeach();
}

void SafeVectorTest::testMoveMainListToOutsideForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(11);
	sv.add(22);
	sv.add(33);
	Vector<int> target;
	sv.moveMainListTo(target);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testMoveMainListToOutsideForeach sv empty");
	SafeVectorDeepTestSupport_SVD_ASSERT(target.size() == 3, "testMoveMainListToOutsideForeach target size");
	SafeVectorDeepTestSupport_SVD_ASSERT(target[0] == 11, "testMoveMainListToOutsideForeach target[0]");
	SafeVectorDeepTestSupport_SVD_ASSERT(target[1] == 22, "testMoveMainListToOutsideForeach target[1]");
	SafeVectorDeepTestSupport_SVD_ASSERT(target[2] == 33, "testMoveMainListToOutsideForeach target[2]");
}

void SafeVectorTest::testMoveMainListToDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(5);
	sv.add(6);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 2, "testMoveMainListToDuringForeach snapshot before");
	Vector<int> target;
	sv.moveMainListTo(target);
	// 主列表已被移走
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testMoveMainListToDuringForeach sv empty");
	SafeVectorDeepTestSupport_SVD_ASSERT(target.size() == 2, "testMoveMainListToDuringForeach target size");
	sv.endForeach();
	// 同步后 updateList 也应为空
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 0, "testMoveMainListToDuringForeach synced empty");
	sv.endForeach();
}

void SafeVectorTest::testMultiplePushBackDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 2, "testMultiplePushBackDuringForeach before");
	sv.add(3);
	sv.add(4);
	sv.add(5);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 5, "testMultiplePushBackDuringForeach mainList");
	sv.endForeach();
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 5, "testMultiplePushBackDuringForeach synced");
	sv.endForeach();
}

void SafeVectorTest::testMultipleEraseAtDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.add(40);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 4, "testMultipleEraseAtDuringForeach before");
	sv.removeAt(0); // 移除 10
	sv.removeAt(0); // 移除 20（原 index 1 变为 0）
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "testMultipleEraseAtDuringForeach mainList");
	sv.endForeach();
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 2, "testMultipleEraseAtDuringForeach synced");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(10), "testMultipleEraseAtDuringForeach 10 gone");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(20), "testMultipleEraseAtDuringForeach 20 gone");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.contains(30), "testMultipleEraseAtDuringForeach 30 remains");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.contains(40), "testMultipleEraseAtDuringForeach 40 remains");
	sv.endForeach();
}

void SafeVectorTest::testMixedAddRemoveDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 3, "testMixedAddRemoveDuringForeach before");
	sv.add(4);
	sv.removeAt(0); // 移除 1
	// mainList: {2,3,4}
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 3, "testMixedAddRemoveDuringForeach mainList");
	sv.endForeach();
	const Vector<int>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 3, "testMixedAddRemoveDuringForeach synced");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(1), "testMixedAddRemoveDuringForeach 1 gone");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.contains(4), "testMixedAddRemoveDuringForeach 4 added");
	sv.endForeach();
}

void SafeVectorTest::testLargeModifyTriggersCloneTo()
{
	using namespace SafeVectorDeepTestSupport;
	// 当 modifyCount >= mainCount 时，sync() 走 cloneTo 分支
	SafeVector<int> sv;
	// 先填充 4 个元素
	sv.add(1);
	sv.add(2);
	sv.add(3);
	sv.add(4);
	const Vector<int>& snapshot = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snapshot.size() == 4, "testLargeModifyTriggersCloneTo before");
	// 修改次数（5）>= mainCount（4） → cloneTo 分支
	sv.add(5);
	sv.add(6);
	sv.removeAt(0); // 移除 1
	sv.removeAt(0); // 移除 2
	sv.add(7);
	// modifyList 有 5 项，mainCount 在开始时为 4
	sv.endForeach();
	const Vector<int>& snap2 = sv.startForeach();
	// mainList 最终为 {3,4,5,6,7}
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 5, "testLargeModifyTriggersCloneTo synced size");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.contains(3), "testLargeModifyTriggersCloneTo has 3");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.contains(7), "testLargeModifyTriggersCloneTo has 7");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(1), "testLargeModifyTriggersCloneTo 1 gone");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains(2), "testLargeModifyTriggersCloneTo 2 gone");
	sv.endForeach();
}

void SafeVectorTest::testEraseAtOutOfBounds()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	bool ok1 = sv.removeAt(-1);
	SafeVectorDeepTestSupport_SVD_ASSERT(!ok1, "testEraseAtOutOfBounds -1 returns false");
	bool ok2 = sv.removeAt(1);
	SafeVectorDeepTestSupport_SVD_ASSERT(!ok2, "testEraseAtOutOfBounds 1 returns false (size=1)");
	bool ok3 = sv.removeAt(0);
	SafeVectorDeepTestSupport_SVD_ASSERT(ok3, "testEraseAtOutOfBounds 0 returns true");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testEraseAtOutOfBounds now empty");
}

void SafeVectorTest::testEraseElementNotFound()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	bool ok = sv.remove(999);
	SafeVectorDeepTestSupport_SVD_ASSERT(!ok, "testEraseElementNotFound returns false");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 1, "testEraseElementNotFound size unchanged");
}

void SafeVectorTest::testSyncAfterForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	// 第一轮：加入 30
	sv.startForeach();
	sv.add(30);
	sv.endForeach();
	// 验证快照已同步
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 3, "testSyncAfterForeach snap size");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.contains(30), "testSyncAfterForeach contains 30");
	sv.endForeach();
}

void SafeVectorTest::testUpdateListConsistency()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	// 遍历中删除所有元素
	sv.startForeach();
	sv.removeAt(2);
	sv.removeAt(1);
	sv.removeAt(0);
	sv.endForeach();
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 0, "testUpdateListConsistency empty after delete all");
	sv.endForeach();
}

void SafeVectorTest::testDataPointer()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(111);
	sv.add(222);
	const int* ptr = sv.data();
	SafeVectorDeepTestSupport_SVD_ASSERT(ptr != nullptr, "testDataPointer not null");
	SafeVectorDeepTestSupport_SVD_ASSERT(ptr[0] == 111, "testDataPointer [0]");
	SafeVectorDeepTestSupport_SVD_ASSERT(ptr[1] == 222, "testDataPointer [1]");
}

void SafeVectorTest::testDestructorCleansUp()
{
	using namespace SafeVectorDeepTestSupport;
	// 仅验证析构不崩溃
	{
		SafeVector<int> sv;
		sv.add(1);
		sv.add(2);
		sv.add(3);
	}
	SafeVectorDeepTestSupport_SVD_ASSERT(true, "testDestructorCleansUp no crash");
}

void SafeVectorTest::testResetElementDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	// remove 在遍历期间：主列表移除，updateList 重置为默认值
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.startForeach();
	sv.remove(20);
	// 主列表 size 变 2
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "testResetElementDuringForeach mainList size");
	sv.endForeach();
	// endForeach 之后 clearDefaultElement(1) 清除 updateList 中的重置元素
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 2, "testResetElementDuringForeach snap size");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap.contains(20), "testResetElementDuringForeach 20 gone");
	sv.endForeach();
}

void SafeVectorTest::testHasResetCountCleared()
{
	using namespace SafeVectorDeepTestSupport;
	// endForeach 后 mHasResetCount 应归零
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.startForeach();
	sv.remove(1);
	sv.remove(2);
	sv.endForeach();
	// 再次遍历，不应有遗留的 resetCount
	sv.add(3);
	sv.add(4);
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 2, "testHasResetCountCleared fresh snap size");
	sv.endForeach();
}

void SafeVectorTest::testMultipleForeachCycles()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	// 第一轮：加 4
	sv.startForeach();
	sv.add(4);
	sv.endForeach();
	// 第二轮：加 5，删 1
	sv.startForeach();
	sv.add(5);
	sv.removeAt(0);
	sv.endForeach();
	// 第三轮验证
	const Vector<int>& snap = sv.startForeach();
	// mainList 最终为 {2,3,4,5}
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 4, "testMultipleForeachCycles final size");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.contains(5), "testMultipleForeachCycles has 5");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap.contains(1), "testMultipleForeachCycles 1 gone");
	sv.endForeach();
}

void SafeVectorTest::testEraseAllDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	sv.startForeach();
	// 从后往前删所有元素
	sv.removeAt(2);
	sv.removeAt(1);
	sv.removeAt(0);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testEraseAllDuringForeach mainList empty");
	sv.endForeach();
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 0, "testEraseAllDuringForeach synced empty");
	sv.endForeach();
}

void SafeVectorTest::testPushBackThenClearDuringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.startForeach();
	sv.add(3);
	sv.add(4);
	sv.clear(); // clear 期间 foreaching，记录所有 remove 操作
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "testPushBackThenClearDuringForeach mainList empty");
	sv.endForeach();
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 0, "testPushBackThenClearDuringForeach synced empty");
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorString()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<string> sv;
	sv.add("hello");
	sv.add("world");
	sv.add("foo");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 3, "SaVecString size");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.contains("world"), "SaVecString contains world");
	sv.remove("hello");
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.contains("hello"), "SaVecString hello gone");
	sv.clear();
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isEmpty(), "SaVecString clear isEmpty");
}

void SafeVectorTest::testSafeVectorStringForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<string> sv;
	sv.add("a");
	sv.add("b");
	sv.add("c");
	sv.startForeach();
	sv.add("d");
	sv.remove("a");
	sv.endForeach();
	const Vector<string>& snap2 = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.size() == 3, "SaVecStringForeach size");
	SafeVectorDeepTestSupport_SVD_ASSERT(snap2.contains("d"), "SaVecStringForeach has d");
	SafeVectorDeepTestSupport_SVD_ASSERT(!snap2.contains("a"), "SaVecStringForeach a gone");
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorPointer()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int*> sv;
	int a = 1, b = 2, c = 3;
	sv.add(&a);
	sv.add(&b);
	sv.add(&c);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 3, "SaVecPtr size");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.contains(&b), "SaVecPtr contains b");
	sv.remove(&b);
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.contains(&b), "SaVecPtr b removed");
	sv.clear();
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isEmpty(), "SaVecPtr clear");
}

void SafeVectorTest::testSafeVectorLargeData()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	const int N = 200;
	FOR(N)
	{
		sv.add(i * 3);
	}
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == N, "SaVecLarge initial size");
	// 遍历期间删除所有偶数索引元素（从后往前）
	sv.startForeach();
	for (int i = N - 1; i >= 0; i -= 2)
	{
		sv.removeAt(i);
	}
	sv.endForeach();
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == N / 2, "SaVecLarge half removed");
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorRepeatedCycles()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	const int CYCLES = 20;
	FOR(CYCLES)
	{
		sv.add(i + 1);
		sv.add(i + 1001);
		sv.startForeach();
		sv.removeAt(0);
		sv.endForeach();
		sv.startForeach();
		// 每轮：保留上一轮尾 + 新加两个 - 删一个
		sv.endForeach();
	}
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.isEmpty(), "SaVecRepeatedCycles not empty");
}

void SafeVectorTest::testSafeVectorEraseByValueAll()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(42);
	sv.add(42);
	sv.add(42);
	// remove 只移除第一个
	bool ok = sv.remove(42);
	SafeVectorDeepTestSupport_SVD_ASSERT(ok, "SaVecEraseByValueAll first ok");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "SaVecEraseByValueAll size 2");
	sv.remove(42);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 1, "SaVecEraseByValueAll size 1");
	sv.remove(42);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 0, "SaVecEraseByValueAll size 0");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isEmpty(), "SaVecEraseByValueAll isEmpty");
}

void SafeVectorTest::testSafeVectorSingleElement()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	sv.add(999);
	sv.startForeach();
	sv.removeAt(0);
	sv.endForeach();
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 0, "SaVecSingleElement empty after remove");
	sv.endForeach();
}

void SafeVectorTest::testSafeVectorEmptyForeach()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<int> sv;
	const Vector<int>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 0, "SaVecEmptyForeach snap empty");
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.isForeaching(), "SaVecEmptyForeach isForeaching");
	sv.endForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(!sv.isForeaching(), "SaVecEmptyForeach not foreaching after end");
}

void SafeVectorTest::testSafeVectorMixedTypes()
{
	using namespace SafeVectorDeepTestSupport;
	SafeVector<float> sv;
	sv.add(1.0f);
	sv.add(2.5f);
	sv.add(3.14f);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 3, "SaVecFloat size");
	sv.remove(2.5f);
	SafeVectorDeepTestSupport_SVD_ASSERT(sv.size() == 2, "SaVecFloat remove 2.5");
	sv.startForeach();
	sv.add(100.0f);
	sv.endForeach();
	const Vector<float>& snap = sv.startForeach();
	SafeVectorDeepTestSupport_SVD_ASSERT(snap.size() == 3, "SaVecFloat after add 100");
	sv.endForeach();
}

void SafeVectorTest::runExtraTests()
{
	using namespace SafeVectorDeepTestSupport;
	testSafeVectorString();
	testSafeVectorStringForeach();
	testSafeVectorPointer();
	testSafeVectorLargeData();
	testSafeVectorRepeatedCycles();
	testSafeVectorEraseByValueAll();
	testSafeVectorSingleElement();
	testSafeVectorEmptyForeach();
	testSafeVectorMixedTypes();
}

namespace SafeVectorExtTestSupport
{
#define SafeVectorTestSupport_SVEX_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("SafeVectorTest FAILED: ") + (msg)); }
#define SafeVectorTestSupport_SVEX_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("SafeVectorTest FAILED: ") + (msg) + \
        " | got=" + IToS((int)(a))); }
}

void SafeVectorTest::testSVEX_MoveMainListTo_Basic()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);
	sv.add(30);
	SafeVectorTestSupport_SVEX_EQ(sv.size(), 3, "before move size==3");

	Vector<int> target;
	sv.moveMainListTo(target);

	SafeVectorTestSupport_SVEX_EQ(target.size(), 3, "target size==3 after move");
	SafeVectorTestSupport_SVEX_EQ(target[0], 10, "target[0]==10");
	SafeVectorTestSupport_SVEX_EQ(target[1], 20, "target[1]==20");
	SafeVectorTestSupport_SVEX_EQ(target[2], 30, "target[2]==30");
	SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "sv empty after moveMainListTo");
}

void SafeVectorTest::testSVEX_MoveMainListTo_Empty()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	Vector<int> target;
	target.add(999);
	sv.moveMainListTo(target);
	SafeVectorTestSupport_SVEX_ASSERT(target.isEmpty(), "moveMainListTo empty clears target");
	SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "sv empty after move empty");
}

void SafeVectorTest::testSVEX_MoveMainListTo_ThenReuse()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	FOR(5)
	{
		sv.add(i);
	}
	Vector<int> target;
	sv.moveMainListTo(target);
	SafeVectorTestSupport_SVEX_EQ(target.size(), 5, "moveMainListTo target size==5");

	// sv 被清空后可以继续使用
	sv.add(100);
	sv.add(200);
	SafeVectorTestSupport_SVEX_EQ(sv.size(), 2, "sv reuse after move size==2");
	SafeVectorTestSupport_SVEX_EQ(sv.getMainList()[0], 100, "sv reuse [0]==100");
}

void SafeVectorTest::testSVEX_MoveMainListTo_LargeList()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	const int N = 1000;
	FOR(N)
	{
		sv.add(i);
	}
	SafeVectorTestSupport_SVEX_EQ(sv.size(), N, "before move large size==1000");

	Vector<int> target;
	sv.moveMainListTo(target);

	SafeVectorTestSupport_SVEX_EQ(target.size(), N, "large move target size==1000");
	SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "large move sv empty");
	// 抽样验证
	for (int i = 0; i < N; i += 100)
	{
		SafeVectorTestSupport_SVEX_EQ(target[i], i, "large move target value correct");
	}
}

void SafeVectorTest::testSVEX_MoveMainListTo_OverwriteTarget()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);

	Vector<int> target;
	target.add(10);
	target.add(20);
	// move 会覆盖 target
	sv.moveMainListTo(target);

	SafeVectorTestSupport_SVEX_EQ(target.size(), 3, "overwrite target size==3");
	SafeVectorTestSupport_SVEX_EQ(target[0], 1, "overwrite target[0]==1");
	SafeVectorTestSupport_SVEX_EQ(target[2], 3, "overwrite target[2]==3");
}

void SafeVectorTest::testSVEX_MoveMainListTo_MultipleRounds()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	FOR(5)
	{
		sv.add(i * 10);
		sv.add(i * 10 + 1);
		Vector<int> target;
		sv.moveMainListTo(target);
		SafeVectorTestSupport_SVEX_EQ(target.size(), 2, "multi round target size==2");
		SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "multi round sv empty");
	}
}

void SafeVectorTest::testSVEX_MoveMainListTo_DuringForeach()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);

	sv.startForeach();
	SafeVectorTestSupport_SVEX_ASSERT(sv.isForeaching(), "isForeaching==true after startForeach");

	// 在遍历中 moveMainListTo
	Vector<int> target;
	sv.moveMainListTo(target);

	// 遍历中：mMainList 被移走，target 得到数据
	SafeVectorTestSupport_SVEX_EQ(target.size(), 3, "during foreach move target size==3");
	SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "during foreach sv.main empty after move");

	sv.endForeach();
	SafeVectorTestSupport_SVEX_ASSERT(!sv.isForeaching(), "isForeaching==false after endForeach");
}

void SafeVectorTest::testSVEX_MoveMainListTo_DuringForeach_ThenPush()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(10);
	sv.add(20);

	sv.startForeach();

	// 遍历中：先 moveMainListTo，再 add
	Vector<int> target;
	sv.moveMainListTo(target);
	sv.add(30);

	sv.endForeach();

	// 结束遍历后，sv 应该只有 30
	SafeVectorTestSupport_SVEX_EQ(sv.getMainList().size(), 1, "after foreach push 30 size==1");
	SafeVectorTestSupport_SVEX_EQ(sv.getMainList()[0], 30, "after foreach push 30 value==30");
}

void SafeVectorTest::testSVEX_IsForeaching_InitFalse()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	SafeVectorTestSupport_SVEX_ASSERT(!sv.isForeaching(), "isForeaching init==false");
}

void SafeVectorTest::testSVEX_IsForeaching_DuringForeach()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.startForeach();
	SafeVectorTestSupport_SVEX_ASSERT(sv.isForeaching(), "isForeaching==true");
	sv.endForeach();
	SafeVectorTestSupport_SVEX_ASSERT(!sv.isForeaching(), "isForeaching==false after end");
}

void SafeVectorTest::testSVEX_IsForeaching_AfterModify()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.startForeach();
	sv.add(3);
	SafeVectorTestSupport_SVEX_ASSERT(sv.isForeaching(), "isForeaching still true after push during");
	sv.endForeach();
	SafeVectorTestSupport_SVEX_ASSERT(!sv.isForeaching(), "isForeaching false after end");
}

void SafeVectorTest::testSVEX_Data_Basic()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(100);
	sv.add(200);
	sv.add(300);
	const int* ptr = sv.data();
	SafeVectorTestSupport_SVEX_ASSERT(ptr != nullptr, "data() non-null");
	SafeVectorTestSupport_SVEX_EQ(ptr[0], 100, "data()[0]==100");
	SafeVectorTestSupport_SVEX_EQ(ptr[1], 200, "data()[1]==200");
	SafeVectorTestSupport_SVEX_EQ(ptr[2], 300, "data()[2]==300");
}

void SafeVectorTest::testSVEX_Data_AfterPushBack()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	FOR(10)
	{
		sv.add(i * 5);
	}
	const int* ptr = sv.data();
	SafeVectorTestSupport_SVEX_ASSERT(ptr != nullptr, "data after push non-null");
	FOR(10)
	{
		SafeVectorTestSupport_SVEX_EQ(ptr[i], i * 5, "data[] value correct");
	}
}

void SafeVectorTest::testSVEX_Data_AfterErase()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);
	sv.removeAt(1);
	const int* ptr = sv.data();
	SafeVectorTestSupport_SVEX_EQ(ptr[0], 1, "data after remove [0]==1");
	SafeVectorTestSupport_SVEX_EQ(ptr[1], 3, "data after remove [1]==3");
}

void SafeVectorTest::testSVEX_FrameTransfer()
{
	using namespace SafeVectorExtTestSupport;
	// 模拟：每帧的活跃对象列表，到一定条件转移给历史列表
	SafeVector<int> activeList;
	Vector<int> historyList;

	FOR(20)
	{
		activeList.add(i);
	}
	SafeVectorTestSupport_SVEX_EQ(activeList.size(), 20, "active size==20");

	// 转移
	activeList.moveMainListTo(historyList);
	SafeVectorTestSupport_SVEX_EQ(historyList.size(), 20, "history size==20");
	SafeVectorTestSupport_SVEX_ASSERT(activeList.isEmpty(), "active empty after transfer");

	// 重新填充
	for (int i = 20; i < 30; ++i)
	{
		activeList.add(i);
	}
	SafeVectorTestSupport_SVEX_EQ(activeList.size(), 10, "active refilled size==10");
}

void SafeVectorTest::testSVEX_ForeachWithMoveAndPush()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	sv.add(3);

	// 遍历所有元素，同时将列表移出、再加回新元素
	Vector<int> old;
	int sum = 0;
	for (int v : sv.startForeach())
	{
		sum += v;
	}
	SafeVectorTestSupport_SVEX_EQ(sum, 6, "foreach sum==6");

	sv.moveMainListTo(old);
	sv.add(10);
	sv.add(20);
	sv.endForeach();

	SafeVectorTestSupport_SVEX_EQ(old.size(), 3, "old list size==3");
	SafeVectorTestSupport_SVEX_EQ(sv.size(), 2, "sv after move+push size==2");
}

void SafeVectorTest::testSVEX_ContainsAfterMove()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(5);
	sv.add(10);
	sv.add(15);
	SafeVectorTestSupport_SVEX_ASSERT(sv.contains(10), "contains 10 before move");

	Vector<int> target;
	sv.moveMainListTo(target);

	SafeVectorTestSupport_SVEX_ASSERT(!sv.contains(10), "contains 10 after move==false");
	SafeVectorTestSupport_SVEX_ASSERT(target.contains(10), "target contains 10 after move");
}

void SafeVectorTest::testSVEX_SizeAfterMove()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	FOR(7)
	{
		sv.add(i);
	}
	SafeVectorTestSupport_SVEX_EQ(sv.size(), 7, "before move size==7");

	Vector<int> t;
	sv.moveMainListTo(t);
	SafeVectorTestSupport_SVEX_EQ(sv.size(), 0, "after move sv size==0");
	SafeVectorTestSupport_SVEX_EQ(t.size(), 7, "after move target size==7");
}

void SafeVectorTest::testSVEX_MoveEmpty_ToNonEmpty()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;  // empty
	Vector<int> target;
	FOR(5)
	{
		target.add(i);
	}
	sv.moveMainListTo(target);
	SafeVectorTestSupport_SVEX_ASSERT(target.isEmpty(), "move empty to non-empty: target cleared");
}

void SafeVectorTest::testSVEX_IsEmpty_AfterPushAndMove()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "init isEmpty");
	sv.add(1);
	SafeVectorTestSupport_SVEX_ASSERT(!sv.isEmpty(), "after push not empty");
	Vector<int> t;
	sv.moveMainListTo(t);
	SafeVectorTestSupport_SVEX_ASSERT(sv.isEmpty(), "after move isEmpty");
}

void SafeVectorTest::testSVEX_GetMainListAfterMove()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(42);
	sv.add(43);
	Vector<int> t;
	sv.moveMainListTo(t);
	SafeVectorTestSupport_SVEX_ASSERT(sv.getMainList().isEmpty(), "getMainList after move isEmpty");
	SafeVectorTestSupport_SVEX_EQ(sv.getMainList().size(), 0, "getMainList after move size==0");
}

void SafeVectorTest::testSVEX_PushAfterMove_GetsCorrectData()
{
	using namespace SafeVectorExtTestSupport;
	SafeVector<int> sv;
	sv.add(1);
	sv.add(2);
	Vector<int> t;
	sv.moveMainListTo(t);
	sv.add(99);
	sv.add(100);
	SafeVectorTestSupport_SVEX_EQ(sv.size(), 2, "push after move size==2");
	SafeVectorTestSupport_SVEX_EQ(sv.getMainList()[0], 99, "push after move [0]==99");
	SafeVectorTestSupport_SVEX_EQ(sv.getMainList()[1], 100, "push after move [1]==100");
}


void SafeVectorTest::test()
{
	testSafeVectorBasic();
	testSafeVectorClear();
	testSafeVectorNonForeachSync();
	testSafeVectorForeachPush();
	testSafeVectorForeachErase();
	testSafeVectorForeachAddThenRemove();
	testSafeVectorForeachClear();
	testSafeVectorData();
	testSafeVectorStress();
	testSafeHashMapBasic();
	testSafeHashMapClear();
	testSafeHashMapForeachInsert();
	testSafeHashMapForeachErase();
	testSafeHashMapEraseWithValue();
	testSafeMapBasic();
	testSafeMapForeachModify();
	testSafeMapClear();
	testSafeSetBasic();
	testSafeSetErase();
	testSafeSetForeachInsert();
	testSafeSetForeachErase();
	testSafeSetClear();
	testSafeSetForeachSync();
	testSafeVectorForeachState();
	testSafeHashMapForeachState();
	testSafeVectorMultiForeach();
	testSafeHashMapStress();
	testSafeMapComplex();
	testBasicPushBack();
	testBasicEraseAt();
	testBasicEraseElement();
	testBasicClear();
	testSizeAndIsEmpty();
	testContains();
	testGetMainList();
	testForeachFlag();
	testPushBackDuringForeach();
	testEraseAtDuringForeach();
	testEraseElementDuringForeach();
	testClearDuringForeach();
	testMoveMainListToOutsideForeach();
	testMoveMainListToDuringForeach();
	testMultiplePushBackDuringForeach();
	testMultipleEraseAtDuringForeach();
	testMixedAddRemoveDuringForeach();
	testLargeModifyTriggersCloneTo();
	testEraseAtOutOfBounds();
	testEraseElementNotFound();
	testSyncAfterForeach();
	testUpdateListConsistency();
	testDataPointer();
	testDestructorCleansUp();
	testResetElementDuringForeach();
	testHasResetCountCleared();
	testMultipleForeachCycles();
	testEraseAllDuringForeach();
	testPushBackThenClearDuringForeach();
	runExtraTests();
	testSVEX_MoveMainListTo_Basic();
	testSVEX_MoveMainListTo_Empty();
	testSVEX_MoveMainListTo_ThenReuse();
	testSVEX_MoveMainListTo_LargeList();
	testSVEX_MoveMainListTo_OverwriteTarget();
	testSVEX_MoveMainListTo_MultipleRounds();
	testSVEX_MoveMainListTo_DuringForeach();
	testSVEX_MoveMainListTo_DuringForeach_ThenPush();
	testSVEX_IsForeaching_InitFalse();
	testSVEX_IsForeaching_DuringForeach();
	testSVEX_IsForeaching_AfterModify();
	testSVEX_Data_Basic();
	testSVEX_Data_AfterPushBack();
	testSVEX_Data_AfterErase();
	testSVEX_FrameTransfer();
	testSVEX_ForeachWithMoveAndPush();
	testSVEX_ContainsAfterMove();
	testSVEX_SizeAfterMove();
	testSVEX_MoveEmpty_ToNonEmpty();
	testSVEX_IsEmpty_AfterPushAndMove();
	testSVEX_GetMainListAfterMove();
	testSVEX_PushAfterMove_GetsCorrectData();
}
#endif
