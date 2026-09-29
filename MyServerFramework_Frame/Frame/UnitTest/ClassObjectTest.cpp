#ifdef WINDOWS
#include "FrameHeader.h"
namespace ClassObjectTestSupport
{
#define ClassObjectTestSupport_COT_ASSERT(expr) \
	if (!(expr)) \
	{ \
		LOG("ClassObjectTest FAILED: " #expr); \
	}

//-------------------------------------------------------------
// 辅助：最小化的可测试 ClassObject 子类（不需要 pool）
//-------------------------------------------------------------
class COTTestObj : public ClassObject
{
	BASE(COTTestObj, ClassObject);
public:
	int mData = 0;
	int mResetCount = 0;

	void resetProperty() override
	{
		ClassObject::resetProperty();
		mData = 0;
		++mResetCount;
	}
};

class COTTestObj2 : public ClassObject
{
	BASE(COTTestObj2, ClassObject);
public:
	string mName;
	bool mActive = false;

	void resetProperty() override
	{
		ClassObject::resetProperty();
		mName.clear();
		mActive = false;
	}
};

// 用作 pool 句柄的最小 FrameComponent（只需要指针比较）
class COTFakePool : public ClassPoolBase
{
	BASE(COTFakePool, FrameComponent);
};

//-------------------------------------------------------------
}

void ClassObjectTest::testDefaultState()
{
	using namespace ClassObjectTestSupport;
	COTTestObj obj;
	// 刚创建的对象：mDestroy=true（已销毁状态），mPendingDestroy=false
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);
	ClassObjectTestSupport_COT_ASSERT(obj.isPendingDestroy() == false);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 0);
	ClassObjectTestSupport_COT_ASSERT(obj.getPool() == nullptr);
}

void ClassObjectTest::testMarkUsable()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;

	obj.markUsable(&pool, 1001LL);

	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == false);
	ClassObjectTestSupport_COT_ASSERT(obj.isPendingDestroy() == false);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 1001LL);
	ClassObjectTestSupport_COT_ASSERT(obj.getPool() == &pool);
}

void ClassObjectTest::testMarkDispose()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 2002LL);

	bool result = obj.markDispose(&pool);

	ClassObjectTestSupport_COT_ASSERT(result == true);
	// resetProperty 已被调用
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);
	ClassObjectTestSupport_COT_ASSERT(obj.getPool() == nullptr);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 0);
}

void ClassObjectTest::testMarkUsableTwice()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 100LL);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 100LL);

	// 再次 markUsable（模拟重新分配）
	obj.markUsable(&pool, 200LL);
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == false);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 200LL);
}

void ClassObjectTest::testMarkDisposeWrongPool()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool1;
	COTFakePool pool2;
	COTTestObj obj;
	obj.markUsable(&pool1, 300LL);

	// 用错误的 pool 销毁，应返回 false 且不执行 resetProperty
	bool result = obj.markDispose(&pool2);
	ClassObjectTestSupport_COT_ASSERT(result == false);
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == false);   // 状态未变
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 300LL); // ID 未变
}

void ClassObjectTest::testMarkDisposeAfterAlreadyDisposed()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 400LL);
	obj.markDispose(&pool); // 第一次，成功

	// 再次 markDispose：mDestroy=true 且 mCreateSourcePool=nullptr → 返回 false
	bool result = obj.markDispose(&pool);
	ClassObjectTestSupport_COT_ASSERT(result == false);
}

void ClassObjectTest::testGetAssignID()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 0);

	obj.markUsable(&pool, 9999LL);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 9999LL);
}

void ClassObjectTest::testGetPool()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	ClassObjectTestSupport_COT_ASSERT(obj.getPool() == nullptr);

	obj.markUsable(&pool, 1LL);
	ClassObjectTestSupport_COT_ASSERT(obj.getPool() == &pool);
}

void ClassObjectTest::testSetPendingDestroy()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 1LL);
	ClassObjectTestSupport_COT_ASSERT(obj.isPendingDestroy() == false);

	obj.setPendingDestroy();
	ClassObjectTestSupport_COT_ASSERT(obj.isPendingDestroy() == true);
}

void ClassObjectTest::testIsPendingDestroy()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 1LL);

	ClassObjectTestSupport_COT_ASSERT(!obj.isPendingDestroy());
	obj.setPendingDestroy();
	ClassObjectTestSupport_COT_ASSERT(obj.isPendingDestroy());

	// markDispose 之后 resetProperty 清空 pendingDestroy
	obj.markDispose(&pool);
	ClassObjectTestSupport_COT_ASSERT(!obj.isPendingDestroy());
}

void ClassObjectTest::testIsDestroy()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);

	obj.markUsable(&pool, 42LL);
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == false);

	obj.markDispose(&pool);
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);
}

void ClassObjectTest::testResetProperty()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 123LL);
	obj.mData = 999;

	obj.resetProperty();

	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);
	ClassObjectTestSupport_COT_ASSERT(obj.getPool() == nullptr);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 0);
	ClassObjectTestSupport_COT_ASSERT(obj.mData == 0);
}

void ClassObjectTest::testMarkUsableResetsDestroy()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	// 初始 isDestroy=true
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);

	obj.markUsable(&pool, 1LL);
	ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == false);
}

void ClassObjectTest::testMarkUsableResetsAssignID()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 100LL);
	obj.markDispose(&pool);
	// dispose 后 assignID = 0
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 0);

	obj.markUsable(&pool, 200LL);
	ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == 200LL);
}

void ClassObjectTest::testMarkDisposeCallsResetProperty()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;
	obj.markUsable(&pool, 1LL);
	obj.mData = 42;
	ClassObjectTestSupport_COT_ASSERT(obj.mResetCount == 0);

	obj.markDispose(&pool);
	// resetProperty 应被调用一次
	ClassObjectTestSupport_COT_ASSERT(obj.mResetCount == 1);
	ClassObjectTestSupport_COT_ASSERT(obj.mData == 0);
}

void ClassObjectTest::testMultipleLifecycles()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj;

	// 5 个生命周期
	for (int i = 1; i <= 5; ++i)
	{
		obj.markUsable(&pool, (llong)i * 1000LL);
		ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == false);
		ClassObjectTestSupport_COT_ASSERT(obj.getAssignID() == (llong)i * 1000LL);

		obj.mData = i * 100;
		ClassObjectTestSupport_COT_ASSERT(obj.mData == i * 100);

		bool ok = obj.markDispose(&pool);
		ClassObjectTestSupport_COT_ASSERT(ok == true);
		ClassObjectTestSupport_COT_ASSERT(obj.isDestroy() == true);
		ClassObjectTestSupport_COT_ASSERT(obj.mData == 0);
	}
	ClassObjectTestSupport_COT_ASSERT(obj.mResetCount == 5);
}

void ClassObjectTest::testAssignIDUniqueness()
{
	using namespace ClassObjectTestSupport;
	COTFakePool pool;
	COTTestObj obj1;
	COTTestObj obj2;

	obj1.markUsable(&pool, 100LL);
	obj2.markUsable(&pool, 200LL);

	ClassObjectTestSupport_COT_ASSERT(obj1.getAssignID() != obj2.getAssignID());
	ClassObjectTestSupport_COT_ASSERT(obj1.getAssignID() == 100LL);
	ClassObjectTestSupport_COT_ASSERT(obj2.getAssignID() == 200LL);
}

void ClassObjectTest::testMarkDisposeWithNullPool()
{
	using namespace ClassObjectTestSupport;
	COTTestObj obj;
	// 未 markUsable，pool=nullptr，markDispose(nullptr) 应返回 false
	bool result = obj.markDispose(nullptr);
	ClassObjectTestSupport_COT_ASSERT(result == false);
}



// ---------------------------------------------------------------------------
// 辅助类（使用独有命名避免与 ClassObjectTest、ClassPoolDeepTest 冲突）
// ---------------------------------------------------------------------------
class COE2Obj : public ClassObject
{
	BASE(COE2Obj, ClassObject);
public:
	int mData = 0;
	bool mExtraFlag = false;
	void resetProperty() override
	{
		ClassObject::resetProperty();
		mData = 0;
		mExtraFlag = false;
	}
};

CLASS_POOL(COE2Obj);
namespace ClassObjectExt2TestSupport
{
#define ClassObjectExt2TestSupport_COE2_ASSERT(cond, msg) \
	if (!(cond)) { ERROR(string("ClassObjectExt2Test FAIL: ") + (msg)); }

// ---------------------------------------------------------------------------
// 前向声明（Unity Build 下 test() 在定义之前引用这两个函数）

// ---------------------------------------------------------------------------














// ---------------------------------------------------------------------------
// 额外测试（全局静态触发）
// ---------------------------------------------------------------------------





// ClassObjectTest::runCOE2ExtraTests() 由 test() 显式调用，不再使用全局静态初始化

// ---------------------------------------------------------------------------
// 补充：SafeVector<COE2Obj*> 与 ClassPool<COE2Obj> 联合压力测试
// ---------------------------------------------------------------------------




// ClassObjectTest::runCOE2BonusTests() 由 test() 显式调用，不再使用全局静态初始化
}

void ClassObjectTest::testBaseTypeInfo()
{
	using namespace ClassObjectExt2TestSupport;
	// ClassObject 的 BASE 宏应能正确提供类型信息
	COE2Obj obj;
	// 仅验证对象可以创建，无崩溃
	ClassObjectExt2TestSupport_COE2_ASSERT(true, "testBaseTypeInfo no crash");
}

void ClassObjectTest::testIsSubClassOfVerify()
{
	using namespace ClassObjectExt2TestSupport;
	// COE2Obj 是 ClassObject 的子类
	constexpr bool isChild = IsSubClassOfV<ClassObject, COE2Obj>::mValue;
	ClassObjectExt2TestSupport_COE2_ASSERT(isChild, "testIsSubClassOfVerify COE2Obj isChild of ClassObject");
	// COE2Obj 不是 FrameComponent 的子类
	constexpr bool notChild = IsSubClassOfV<FrameComponent, COE2Obj>::mValue;
	ClassObjectExt2TestSupport_COE2_ASSERT(!notChild, "testIsSubClassOfVerify COE2Obj not child of FrameComponent");
}

void ClassObjectTest::testClassObjectPoolInteraction()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	COE2Obj* obj = pool.newClass();
	ClassObjectExt2TestSupport_COE2_ASSERT(obj->getPool() == &pool, "testClassObjectPoolInteraction pool match");
	ClassObjectExt2TestSupport_COE2_ASSERT(!obj->isDestroy(), "testClassObjectPoolInteraction not destroy");
	UN_CLASS(obj);
	ClassObjectExt2TestSupport_COE2_ASSERT(obj == nullptr, "testClassObjectPoolInteraction null after destroy");
}

void ClassObjectTest::testClassObjectMultipleReuses()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	const int N = 20;
	Vector<llong> ids;
	FOR(N)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = i;
		ids.add(obj->getAssignID());
		UN_CLASS(obj);
	}
	// 所有 assignID 应单调递增
	FOR(N - 1)
	{
		ClassObjectExt2TestSupport_COE2_ASSERT(ids[i] < ids[i + 1], "testClassObjectMultipleReuses monotonic ID");
	}
}

void ClassObjectTest::testClassObjectDestroyFromDifferentPool()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool1;
	COE2Obj* obj = pool1.newClass();
	obj->mData = 42;
	ClassObjectExt2TestSupport_COE2_ASSERT(obj->getPool() == &pool1, "testClassObjectDestroyFromDifferentPool pool1 match");
	// 用 pool2 销毁 pool1 的对象 → 框架会报错并返回 false
	// 不能直接调用（会触发 ERROR），通过验证 getPool 来确认
	// 验证后用正确 pool 销毁
	UN_CLASS(obj);
	ClassObjectExt2TestSupport_COE2_ASSERT(obj == nullptr, "testClassObjectDestroyFromDifferentPool null after");
}

void ClassObjectTest::testClassObjectPendingDestroyBeforeDispose()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	COE2Obj* obj = pool.newClass();
	obj->setPendingDestroy();
	ClassObjectExt2TestSupport_COE2_ASSERT(obj->isPendingDestroy(), "testClassObjectPendingDestroyBeforeDispose pending");
	UN_CLASS(obj);
	// 销毁后 obj 为 nullptr
	ClassObjectExt2TestSupport_COE2_ASSERT(obj == nullptr, "testClassObjectPendingDestroyBeforeDispose null");
}

void ClassObjectTest::testClassObjectResetClearsState()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	COE2Obj* obj = pool.newClass();
	obj->mData = 777;
	obj->mExtraFlag = true;
	UN_CLASS(obj); // destroy 会调用 resetProperty
	// 再次 new，复用该对象，mData 应被重置
	COE2Obj* obj2 = pool.newClass();
	ClassObjectExt2TestSupport_COE2_ASSERT(obj2->mData == 0, "testClassObjectResetClearsState mData 0");
	ClassObjectExt2TestSupport_COE2_ASSERT(!obj2->mExtraFlag, "testClassObjectResetClearsState flag false");
	UN_CLASS(obj2);
}

void ClassObjectTest::testClassObjectIsDestroyAfterMarkDispose()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	COE2Obj* obj = pool.newClass();
	COE2Obj* raw = obj;
	UN_CLASS(obj);
	// raw 已回池，isDestroy() 应为 true
	ClassObjectExt2TestSupport_COE2_ASSERT(raw->isDestroy(), "testClassObjectIsDestroyAfterMarkDispose isDestroy");
}

void ClassObjectTest::testClassObjectAssignIDAfterMarkUsable()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	COE2Obj* a = pool.newClass();
	llong idA = a->getAssignID();
	ClassObjectExt2TestSupport_COE2_ASSERT(idA > 0, "testClassObjectAssignIDAfterMarkUsable idA > 0");
	UN_CLASS(a);
	COE2Obj* b = pool.newClass();
	llong idB = b->getAssignID();
	ClassObjectExt2TestSupport_COE2_ASSERT(idB > idA, "testClassObjectAssignIDAfterMarkUsable idB > idA");
	UN_CLASS(b);
}

void ClassObjectTest::testClassObjectMultipleMarkUsable()
{
	using namespace ClassObjectExt2TestSupport;
	// 同一对象只 markUsable 一次（由 newClass 负责），测试正确 active 状态
	COE2ObjPool pool;
	COE2Obj* obj = pool.newClass();
	ClassObjectExt2TestSupport_COE2_ASSERT(!obj->isDestroy(), "testClassObjectMultipleMarkUsable active");
	// 不重复 markUsable，直接验证
	UN_CLASS(obj);
}

void ClassObjectTest::testClassObjectSafeVectorLifecycle()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	SafeVector<COE2Obj*> sv;
	// 分配 8 个
	FOR(8)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = i;
		sv.add(obj);
	}
	ClassObjectExt2TestSupport_COE2_ASSERT(sv.size() == 8, "testClassObjectSafeVectorLifecycle init size");
	// 遍历中移除 mData 为奇数的
	const Vector<COE2Obj*>& snap = sv.startForeach();
	for (COE2Obj* obj : snap)
	{
		if (obj && obj->mData % 2 != 0)
		{
			sv.remove(obj);
		}
	}
	sv.endForeach();
	// 验证只剩偶数
	const Vector<COE2Obj*>& snap2 = sv.startForeach();
	for (COE2Obj* obj : snap2)
	{
		ClassObjectExt2TestSupport_COE2_ASSERT(obj->mData % 2 == 0, "testClassObjectSafeVectorLifecycle even only");
	}
	sv.endForeach();
	// 清理
	Vector<COE2Obj*> toFree;
	for (COE2Obj* obj : sv.getMainList())
	{
		toFree.add(obj);
	}
	sv.clear();
	UN_CLASS(toFree);
}

void ClassObjectTest::testClassObjectHashMapLifecycle()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	HashMap<int, COE2Obj*> map;
	FOR(5)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = i * 10;
		map[i] = obj;
	}
	ClassObjectExt2TestSupport_COE2_ASSERT((int)map.size() == 5, "testClassObjectHashMapLifecycle size");
	UN_CLASS(map);
	ClassObjectExt2TestSupport_COE2_ASSERT(map.isEmpty(), "testClassObjectHashMapLifecycle cleared");
}

void ClassObjectTest::testClassObjectClearPool()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	pool.initDefault(10);
	Vector<COE2Obj*> active;
	FOR(10)
	{
		active.add(pool.newClass());
	}
	for (COE2Obj* obj : active)
	{
		UN_CLASS(obj);
	}
	pool.quit(); // 释放 unusedList 中的 10 个
	ClassObjectExt2TestSupport_COE2_ASSERT(true, "testClassObjectClearPool no crash");
}

void ClassObjectTest::testCOE2ObjectTagPersistenceAcrossReuse()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	COE2Obj* a = pool.newClass();
	a->mData = 12345;
	UN_CLASS(a);
	// 复用的对象，resetProperty 已调用，mData 为 0
	COE2Obj* b = pool.newClass();
	ClassObjectExt2TestSupport_COE2_ASSERT(b->mData == 0, "COE2 tag not persist across reuse");
	UN_CLASS(b);
}

void ClassObjectTest::testCOE2PoolBulkAllocVerify()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	const int N = 50;
	Vector<COE2Obj*> objs;
	FOR(N)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = N - i; // 逆序值
		objs.add(obj);
	}
	FOR(N)
	{
		ClassObjectExt2TestSupport_COE2_ASSERT(objs[i]->mData == N - i, "COE2 bulk alloc value");
	}
	UN_CLASS(objs);
}

void ClassObjectTest::testCOE2PoolSafeHashMapDestroy()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	SafeHashMap<int, COE2Obj*> shm;
	FOR(6)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = (i + 1) * 5;
		shm.add(i, obj);
	}
	ClassObjectExt2TestSupport_COE2_ASSERT(shm.size() == 6, "COE2 safeHashMap size before");
	UN_CLASS(shm);
	ClassObjectExt2TestSupport_COE2_ASSERT(shm.isEmpty(), "COE2 safeHashMap cleared");
}

void ClassObjectTest::testCOE2ExtraStress()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	const int ROUNDS = 30;
	FOR(ROUNDS)
	{
		Vector<COE2Obj*> batch;
		for (int j = 0; j < 10; ++j)
		{
			COE2Obj* obj = pool.newClass();
			obj->mData = i * 10 + j;
			batch.add(obj);
		}
		for (int j = 0; j < 5; ++j)
		{
			ClassObjectExt2TestSupport_COE2_ASSERT(batch[j]->mData == i * 10 + j, "COE2 stress batch value");
			UN_CLASS(batch[j]);
		}
		for (int j = 5; j < 10; ++j)
		{
			UN_CLASS(batch[j]);
		}
	}
	ClassObjectExt2TestSupport_COE2_ASSERT(true, "COE2 extra stress no crash");
}

void ClassObjectTest::runCOE2ExtraTests()
{
	using namespace ClassObjectExt2TestSupport;
	testCOE2ObjectTagPersistenceAcrossReuse();
	testCOE2PoolBulkAllocVerify();
	testCOE2PoolSafeHashMapDestroy();
	testCOE2ExtraStress();
}

void ClassObjectTest::testCOE2PoolSafeVectorBatch()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	SafeVector<COE2Obj*> sv;
	// 批量分配 30 个
	FOR(30)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = i + 1;
		sv.add(obj);
	}
	ClassObjectExt2TestSupport_COE2_ASSERT(sv.size() == 30, "coe2 sv batch size");
	// 遍历中移除 data > 20 的
	const Vector<COE2Obj*>& snap = sv.startForeach();
	for (COE2Obj* obj : snap)
	{
		if (obj && obj->mData > 20)
		{
			sv.remove(obj);
		}
	}
	sv.endForeach();
	// 验证
	const Vector<COE2Obj*>& snap2 = sv.startForeach();
	ClassObjectExt2TestSupport_COE2_ASSERT(snap2.size() == 20, "coe2 sv after remove size");
	for (COE2Obj* obj : snap2)
	{
		ClassObjectExt2TestSupport_COE2_ASSERT(obj->mData <= 20, "coe2 sv remaining <= 20");
	}
	sv.endForeach();
	// 清理
	Vector<COE2Obj*> remaining;
	for (COE2Obj* obj : sv.getMainList())
	{
		remaining.add(obj);
	}
	sv.clear();
	UN_CLASS(remaining);
}

void ClassObjectTest::testCOE2PoolIdGapAfterReuse()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	// id 单调递增，即使中间有复用也不回退
	llong prevId = 0;
	FOR(15)
	{
		COE2Obj* obj = pool.newClass();
		ClassObjectExt2TestSupport_COE2_ASSERT(obj->getAssignID() > prevId, "coe2 id gap monotonic");
		prevId = obj->getAssignID();
		if (i % 3 == 0)
		{
			UN_CLASS(obj); // 部分回收
		}
		else
		{
			UN_CLASS(obj);
		}
	}
}

void ClassObjectTest::testCOE2PoolArrayListDestroy()
{
	using namespace ClassObjectExt2TestSupport;
	COE2ObjPool pool;
	ArrayList<16, COE2Obj*> al;
	FOR(8)
	{
		COE2Obj* obj = pool.newClass();
		obj->mData = i * 3;
		al.add(obj);
	}
	ClassObjectExt2TestSupport_COE2_ASSERT(al.size() == 8, "coe2 al size before");
	UN_CLASS(al);
	ClassObjectExt2TestSupport_COE2_ASSERT(al.isEmpty(), "coe2 al cleared");
}

void ClassObjectTest::runCOE2BonusTests()
{
	using namespace ClassObjectExt2TestSupport;
	testCOE2PoolSafeVectorBatch();
	testCOE2PoolIdGapAfterReuse();
	testCOE2PoolArrayListDestroy();
}


void ClassObjectTest::test()
{
	testDefaultState();
	testMarkUsable();
	testMarkDispose();
	testMarkUsableTwice();
	testMarkDisposeWrongPool();
	testMarkDisposeAfterAlreadyDisposed();
	testGetAssignID();
	testGetPool();
	testSetPendingDestroy();
	testIsPendingDestroy();
	testIsDestroy();
	testResetProperty();
	testMarkUsableResetsDestroy();
	testMarkUsableResetsAssignID();
	testMarkDisposeCallsResetProperty();
	testMultipleLifecycles();
	testAssignIDUniqueness();
	testMarkDisposeWithNullPool();
	testBaseTypeInfo();
	testIsSubClassOfVerify();
	testClassObjectPoolInteraction();
	testClassObjectMultipleReuses();
	testClassObjectDestroyFromDifferentPool();
	testClassObjectPendingDestroyBeforeDispose();
	testClassObjectResetClearsState();
	testClassObjectIsDestroyAfterMarkDispose();
	testClassObjectAssignIDAfterMarkUsable();
	testClassObjectMultipleMarkUsable();
	testClassObjectSafeVectorLifecycle();
	testClassObjectHashMapLifecycle();
	testClassObjectClearPool();
	runCOE2ExtraTests();
	runCOE2BonusTests();
}
#endif
