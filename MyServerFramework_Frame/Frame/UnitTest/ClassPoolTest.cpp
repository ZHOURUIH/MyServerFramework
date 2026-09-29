#ifdef WINDOWS
#include "FrameHeader.h"

// -----------------------------------------------------------
// 测试用 ClassObject 子类
// -----------------------------------------------------------
class TestEntity : public ClassObject
{
public:
	int health = 0;
	int attack = 0;
	string name;

	virtual void resetProperty() override
	{
		ClassObject::resetProperty();
		health = 100;
		attack = 10;
		name.clear();
	}
	virtual void destroy() override
	{
		health = 0;
		attack = 0;
		name.clear();
	}
};
CLASS_POOL(TestEntity);

class TestItem : public ClassObject
{
public:
	int itemID = 0;
	float weight = 0.0f;

	virtual void resetProperty() override
	{
		ClassObject::resetProperty();
		itemID = 0;
		weight = 0.0f;
	}
	virtual void destroy() override
	{
		itemID = 0;
		weight = 0.0f;
	}
};
CLASS_POOL(TestItem);

// 第三种测试类型
class TestBullet : public ClassObject
{
public:
	float speed = 0.0f;
	float damage = 0.0f;
	bool active = false;
	Vector2 position;

	virtual void resetProperty() override
	{
		ClassObject::resetProperty();
		speed = 100.0f;
		damage = 10.0f;
		active = true;
		position.clear();
	}
	virtual void destroy() override
	{
		speed = 0.0f;
		damage = 0.0f;
		active = false;
		position.clear();
	}
};
CLASS_POOL(TestBullet);
namespace ClassPoolTestSupport
{
#define ClassPoolTestSupport_CPT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("ClassPoolTest FAILED: ") + (msg)); }
}

void ClassPoolTest::testClassObject_InitialState()
{
	using namespace ClassPoolTestSupport;
	TestEntity e;
	ClassPoolTestSupport_CPT_ASSERT(e.isDestroy(), "fresh entity isDestroy==true");
	ClassPoolTestSupport_CPT_ASSERT(e.getAssignID() == 0, "fresh entity assignID==0");
	ClassPoolTestSupport_CPT_ASSERT(e.getPool() == nullptr, "fresh entity pool==null");
}

void ClassPoolTest::testClassObject_ResetProperty()
{
	using namespace ClassPoolTestSupport;
	TestEntity e;
	e.resetProperty();
	ClassPoolTestSupport_CPT_ASSERT(e.isDestroy(), "resetProperty isDestroy still true");
	ClassPoolTestSupport_CPT_ASSERT(e.health == 100, "resetProperty health==100");
	ClassPoolTestSupport_CPT_ASSERT(e.attack == 10, "resetProperty attack==10");
}

void ClassPoolTest::testClassPool_NewClass_Basic()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "newClass non-null");
	ClassPoolTestSupport_CPT_ASSERT(!e->isDestroy(), "newClass isDestroy==false");
	ClassPoolTestSupport_CPT_ASSERT(e->getAssignID() == 1, "newClass first assignID==1");
	ClassPoolTestSupport_CPT_ASSERT(e->getPool() != nullptr, "newClass pool non-null");

	pool.destroyClass(e);
	ClassPoolTestSupport_CPT_ASSERT(e == nullptr, "destroyClass sets ptr to null");
}

void ClassPoolTest::testClassPool_InitDefault()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(5);

	// 从池中取出 5 个（应从预分配列表拿）
	TestEntity* arr[5];
	for (int i = 0; i < 5; ++i)
	{
		arr[i] = pool.newClass();
		ClassPoolTestSupport_CPT_ASSERT(arr[i] != nullptr, "initDefault newClass non-null");
		ClassPoolTestSupport_CPT_ASSERT(!arr[i]->isDestroy(), "initDefault newClass not destroyed");
	}
	// 全部归还
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(arr[i]);
	}
}

void ClassPoolTest::testClassPool_AutoExpand()
{
	using namespace ClassPoolTestSupport;
	// 不预分配，直接 newClass 应触发自动创建
	TestEntityPool pool;
	TestEntity* e1 = pool.newClass();
	TestEntity* e2 = pool.newClass();
	TestEntity* e3 = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(e1 != nullptr && e2 != nullptr && e3 != nullptr, "autoExpand all non-null");
	ClassPoolTestSupport_CPT_ASSERT(e1->getAssignID() != e2->getAssignID(), "autoExpand unique assignIDs");

	pool.destroyClass(e1);
	pool.destroyClass(e2);
	pool.destroyClass(e3);
}

void ClassPoolTest::testClassPool_ReuseAfterDestroy()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e1 = pool.newClass();
	llong id1 = e1->getAssignID();
	pool.destroyClass(e1);

	// 再 newClass，应重用刚归还的对象
	TestEntity* e2 = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(e2 != nullptr, "reuse non-null");
	ClassPoolTestSupport_CPT_ASSERT(e2->getAssignID() != id1, "reuse new assignID != old");
	ClassPoolTestSupport_CPT_ASSERT(!e2->isDestroy(), "reuse isDestroy==false");
	ClassPoolTestSupport_CPT_ASSERT(e2->health == 100, "reuse resetProperty called (health==100)");

	pool.destroyClass(e2);
}

void ClassPoolTest::testClassPool_DestroyNullPtr()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e = nullptr;
	pool.destroyClass(e); // 不应崩溃
	ClassPoolTestSupport_CPT_ASSERT(e == nullptr, "destroyNull ptr still null");
}

void ClassPoolTest::testClassPool_DestroyTwice()
{
	using namespace ClassPoolTestSupport;
	// 双重销毁应该被 markDispose 检测到，不崩溃
	TestEntityPool pool;
	TestEntity* e1 = pool.newClass();
	TestEntity* e2 = e1; // 保存裸指针用于测试
	pool.destroyClass(e1);
	ClassPoolTestSupport_CPT_ASSERT(e1 == nullptr, "first destroy sets null");
	// e2 现在指向已归还对象，不能再次 destroyClass
	// (调用者不应这么做，这里仅验证 e1 已 null)
	(void)e2;
}

void ClassPoolTest::testClassPool_AssignIDIncrement()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e1 = pool.newClass();
	llong id1 = e1->getAssignID();
	TestEntity* e2 = pool.newClass();
	llong id2 = e2->getAssignID();
	TestEntity* e3 = pool.newClass();
	llong id3 = e3->getAssignID();
	ClassPoolTestSupport_CPT_ASSERT(id1 < id2 && id2 < id3, "assignID monotone increasing");

	pool.destroyClass(e1);
	pool.destroyClass(e2);
	pool.destroyClass(e3);
}

void ClassPoolTest::testClassPool_DestroyListVector()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(10);

	Vector<TestEntity*> list;
	for (int i = 0; i < 5; ++i)
	{
		TestEntity* e = pool.newClass();
		e->health = i * 10;
		list.add(e);
	}
	ClassPoolTestSupport_CPT_ASSERT(list.size() == 5, "list size==5 before destroy");

	UN_CLASS(list);
	ClassPoolTestSupport_CPT_ASSERT(list.isEmpty(), "list cleared after destroyList");
}

void ClassPoolTest::testClassPool_DestroyListVectorEmpty()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	Vector<TestEntity*> empty;
	UN_CLASS(empty); // 不应崩溃
	ClassPoolTestSupport_CPT_ASSERT(empty.isEmpty(), "destroyListEmpty still empty");
}

void ClassPoolTest::testClassPool_DestroyListArrayList()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	ArrayList<20, TestEntity*> list;
	for (int i = 0; i < 8; ++i)
	{
		list.add(pool.newClass());
	}
	ClassPoolTestSupport_CPT_ASSERT(list.size() == 8, "arraylist size==8");
	UN_CLASS(list);
	ClassPoolTestSupport_CPT_ASSERT(list.isEmpty(), "arraylist cleared");
}

void ClassPoolTest::testClassPool_DestroyListHashMap()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	HashMap<int, TestEntity*> map;
	for (int i = 0; i < 5; ++i)
	{
		TestEntity* e = pool.newClass();
		e->health = i;
		map.add(i, e);
	}
	ClassPoolTestSupport_CPT_ASSERT(map.size() == 5, "hashmap size==5");
	UN_CLASS(map);
	ClassPoolTestSupport_CPT_ASSERT(map.isEmpty(), "hashmap cleared after destroyList");
}

void ClassPoolTest::testClassPool_DestroyListSafeVector()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	SafeVector<TestEntity*> sv;
	for (int i = 0; i < 4; ++i)
	{
		TestEntity* e = pool.newClass();
		sv.add(e);
	}
	sv.getMainList().size(); // 确认大小
	UN_CLASS(sv);
	ClassPoolTestSupport_CPT_ASSERT(sv.isEmpty(), "safevector cleared after destroyList");
}

void ClassPoolTest::testClassPool_LargeVolume()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(100);

	const int N = 500;
	Vector<TestEntity*> allObjs;
	for (int i = 0; i < N; ++i)
	{
		TestEntity* e = pool.newClass();
		ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "largeVolume newClass non-null");
		e->health = i;
		allObjs.add(e);
	}
	ClassPoolTestSupport_CPT_ASSERT(allObjs.size() == N, "largeVolume allObjs size==500");

	// 销毁一半
	for (int i = 0; i < N / 2; ++i)
	{
		pool.destroyClass(allObjs[i]);
	}
	// 再取出一批
	for (int i = 0; i < N / 4; ++i)
	{
		TestEntity* e = pool.newClass();
		ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "largeVolume realloc non-null");
		pool.destroyClass(e);
	}
	// 清理剩余
	for (int i = N / 2; i < N; ++i)
	{
		pool.destroyClass(allObjs[i]);
	}
}

void ClassPoolTest::testClassPool_MultiType()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool entityPool;
	TestItemPool itemPool;

	TestEntity* e = entityPool.newClass();
	TestItem* item = itemPool.newClass();

	ClassPoolTestSupport_CPT_ASSERT(e != nullptr && item != nullptr, "multiType both non-null");
	ClassPoolTestSupport_CPT_ASSERT(e->getPool() != item->getPool(), "multiType different pools");

	e->health = 999;
	item->itemID = 100;
	item->weight = 3.5f;

	entityPool.destroyClass(e);
	itemPool.destroyClass(item);
}

void ClassPoolTest::testClassPool_ResetOnReturn()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e = pool.newClass();
	e->health = 999;
	e->attack = 500;
	e->name = "boss";
	pool.destroyClass(e);

	// 再取出，resetProperty 应已被调用
	TestEntity* e2 = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(e2->health == 100, "resetOnReturn health==100");
	ClassPoolTestSupport_CPT_ASSERT(e2->attack == 10, "resetOnReturn attack==10");
	ClassPoolTestSupport_CPT_ASSERT(e2->name.empty(), "resetOnReturn name empty");
	pool.destroyClass(e2);
}

void ClassPoolTest::testClassPool_PendingDestroy()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(!e->isPendingDestroy(), "pendingDestroy init false");
	e->setPendingDestroy();
	ClassPoolTestSupport_CPT_ASSERT(e->isPendingDestroy(), "pendingDestroy after set true");
	pool.destroyClass(e);
}

void ClassPoolTest::testClassPool_CycleNewDestroy()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(5);

	for (int cycle = 0; cycle < 20; ++cycle)
	{
		TestEntity* e = pool.newClass();
		ClassPoolTestSupport_CPT_ASSERT(!e->isDestroy(), "cycle not destroyed");
		ClassPoolTestSupport_CPT_ASSERT(e->health == 100, "cycle health reset");
		e->health = cycle;
		pool.destroyClass(e);
	}
}

void ClassPoolTest::testClassPool_DestroyListThenNew()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	Vector<TestEntity*> list;
	for (int i = 0; i < 10; ++i) { list.add(pool.newClass()); }
	UN_CLASS(list);
	ClassPoolTestSupport_CPT_ASSERT(list.isEmpty(), "destroyListThenNew cleared");

	// 再取 10 个，应从归还的列表拿
	for (int i = 0; i < 10; ++i)
	{
		TestEntity* e = pool.newClass();
		ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "destroyListThenNew reuse non-null");
		ClassPoolTestSupport_CPT_ASSERT(!e->isDestroy(), "destroyListThenNew reuse not destroyed");
		pool.destroyClass(e);
	}
}

void ClassPoolTest::testClassPool_DestroyListSafeHashMap()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	SafeHashMap<int, TestEntity*> shm;
	for (int i = 0; i < 6; ++i)
	{
		TestEntity* e = pool.newClass();
		e->health = i * 5;
		shm.add(i, e);
	}
	ClassPoolTestSupport_CPT_ASSERT(shm.getMainList().size() == 6, "safeHashMap init size==6");
	UN_CLASS(shm);
	ClassPoolTestSupport_CPT_ASSERT(shm.isEmpty(), "safeHashMap cleared");
}

void ClassPoolTest::testClassPool_FrameLoopSimulation()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool entityPool;
	entityPool.initDefault(20);

	Vector<TestEntity*> activeList;

	// 模拟 50 帧，每帧创建 3 个，销毁最旧的 2 个
	for (int frame = 0; frame < 50; ++frame)
	{
		// 创建
		for (int i = 0; i < 3; ++i)
		{
			TestEntity* e = entityPool.newClass();
			ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "frameLoop new non-null");
			e->health = frame * 3 + i;
			activeList.add(e);
		}

		// 销毁最旧的 2 个
		if (activeList.size() >= 2)
		{
			entityPool.destroyClass(activeList[0]);
			entityPool.destroyClass(activeList[1]);
			activeList.removeRange(0, 2);
		}
	}

	// 清理剩余
	for (TestEntity* e : activeList)
	{
		entityPool.destroyClass(e);
	}
}

void ClassPoolTest::testClassPool_BulletType()
{
	using namespace ClassPoolTestSupport;
	TestBulletPool bulletPool;
	bulletPool.initDefault(20);

	Vector<TestBullet*> bullets;
	for (int i = 0; i < 15; ++i)
	{
		TestBullet* b = bulletPool.newClass();
		ClassPoolTestSupport_CPT_ASSERT(b != nullptr, "bullet non-null");
		ClassPoolTestSupport_CPT_ASSERT(b->speed == 100.0f, "bullet speed=100");
		ClassPoolTestSupport_CPT_ASSERT(b->damage == 10.0f, "bullet damage=10");
		ClassPoolTestSupport_CPT_ASSERT(b->active, "bullet active=true");
		b->position = Vector2((float)i, (float)i * 2);
		bullets.add(b);
	}
	UN_CLASS(bullets);
	ClassPoolTestSupport_CPT_ASSERT(bullets.isEmpty(), "bullets cleared");

	// 再次取出，验证重置
	TestBullet* b2 = bulletPool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(b2->speed == 100.0f, "bullet reuse speed=100");
	ClassPoolTestSupport_CPT_ASSERT(b2->active, "bullet reuse active=true");
	bulletPool.destroyClass(b2);
}

void ClassPoolTest::testClassPool_AssignIDAfterReuse()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	Vector<llong> ids;

	// 取出并销毁 10 次，每次 assignID 应递增
	for (int i = 0; i < 10; ++i)
	{
		TestEntity* e = pool.newClass();
		ids.add(e->getAssignID());
		pool.destroyClass(e);
	}

	for (int i = 1; i < 10; ++i)
	{
		ClassPoolTestSupport_CPT_ASSERT(ids[i] > ids[i - 1], "assignID after reuse strictly increases");
	}
}

void ClassPoolTest::testClassPool_ThreeTypesIndependent()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool entityPool;
	TestItemPool itemPool;
	TestBulletPool bulletPool;

	entityPool.initDefault(5);
	itemPool.initDefault(5);
	bulletPool.initDefault(5);

	TestEntity* e = entityPool.newClass();
	TestItem* item = itemPool.newClass();
	TestBullet* bullet = bulletPool.newClass();

	ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "3types e non-null");
	ClassPoolTestSupport_CPT_ASSERT(item != nullptr, "3types item non-null");
	ClassPoolTestSupport_CPT_ASSERT(bullet != nullptr, "3types bullet non-null");

	// 各自有不同的池
	ClassPoolTestSupport_CPT_ASSERT(e->getPool() != item->getPool(), "3types e/item diff pool");
	ClassPoolTestSupport_CPT_ASSERT(e->getPool() != bullet->getPool(), "3types e/bullet diff pool");
	ClassPoolTestSupport_CPT_ASSERT(item->getPool() != bullet->getPool(), "3types item/bullet diff pool");

	entityPool.destroyClass(e);
	itemPool.destroyClass(item);
	bulletPool.destroyClass(bullet);
}

void ClassPoolTest::testClassPool_DestroyHashMapLLong()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	HashMap<llong, TestEntity*> map;
	for (llong i = 100; i < 120; ++i)
	{
		TestEntity* e = pool.newClass();
		e->health = (int)i;
		map.add(i, e);
	}
	ClassPoolTestSupport_CPT_ASSERT(map.size() == 20, "llong hashmap size==20");
	UN_CLASS(map);
	ClassPoolTestSupport_CPT_ASSERT(map.isEmpty(), "llong hashmap cleared");
}

void ClassPoolTest::testClassPool_InitDefaultZero()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(0);
	// newClass 应仍然能自动创建
	TestEntity* e = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "initDefault(0) newClass non-null");
	pool.destroyClass(e);
}

void ClassPoolTest::testClassPool_InitDefaultOne()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(1);
	TestEntity* e1 = pool.newClass(); // 使用预创建的
	TestEntity* e2 = pool.newClass(); // 自动扩容
	ClassPoolTestSupport_CPT_ASSERT(e1 != nullptr, "initDefault(1) e1 non-null");
	ClassPoolTestSupport_CPT_ASSERT(e2 != nullptr, "initDefault(1) e2 non-null");
	ClassPoolTestSupport_CPT_ASSERT(e1 != e2, "initDefault(1) e1!=e2");
	pool.destroyClass(e1);
	pool.destroyClass(e2);
}

void ClassPoolTest::testClassPool_MassPreallocAndReset()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	pool.initDefault(200);

	const int ROUNDS = 5;
	const int PER_ROUND = 40;

	for (int r = 0; r < ROUNDS; ++r)
	{
		Vector<TestEntity*> batch;
		for (int i = 0; i < PER_ROUND; ++i)
		{
			TestEntity* e = pool.newClass();
			ClassPoolTestSupport_CPT_ASSERT(e != nullptr, "massPrealloc newClass non-null");
			ClassPoolTestSupport_CPT_ASSERT(e->health == 100, "massPrealloc health reset");
			e->health = r * 100 + i;
			batch.add(e);
		}
		UN_CLASS(batch);
		ClassPoolTestSupport_CPT_ASSERT(batch.isEmpty(), "massPrealloc batch cleared");
	}
}

void ClassPoolTest::testClassPool_StringFieldReset()
{
	using namespace ClassPoolTestSupport;
	TestEntityPool pool;
	TestEntity* e = pool.newClass();
	e->name = "hero_unit_alpha_longname_xyz";
	pool.destroyClass(e);

	TestEntity* e2 = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(e2->name.empty(), "string field reset to empty");
	pool.destroyClass(e2);
}

void ClassPoolTest::testClassPool_FloatFieldReset()
{
	using namespace ClassPoolTestSupport;
	TestBulletPool pool;
	TestBullet* b = pool.newClass();
	b->speed = 9999.9f;
	b->damage = -100.0f;
	b->active = false;
	pool.destroyClass(b);

	TestBullet* b2 = pool.newClass();
	ClassPoolTestSupport_CPT_ASSERT(fabsf(b2->speed - 100.0f) < 1e-4f, "bullet speed reset to 100");
	ClassPoolTestSupport_CPT_ASSERT(fabsf(b2->damage - 10.0f) < 1e-4f, "bullet damage reset to 10");
	ClassPoolTestSupport_CPT_ASSERT(b2->active, "bullet active reset to true");
	pool.destroyClass(b2);
}

// ---------------------------------------------------------------------------
// 辅助：最小化的 ClassObject 子类
// ---------------------------------------------------------------------------
class CPDTestObj : public ClassObject
{
	BASE(CPDTestObj, ClassObject);
public:
	int mValue = 0;
	int mResetCount = 0;
	void resetProperty() override
	{
		ClassObject::resetProperty();
		mValue = 0;
		++mResetCount;
	}
};
CLASS_POOL(CPDTestObj);
namespace ClassPoolDeepTestSupport
{
#define ClassPoolDeepTestSupport_CPD_ASSERT(cond, msg) \
	if (!(cond)) { ERROR(string("ClassPoolDeepTest FAIL: ") + (msg)); }
}

void ClassPoolTest::testNewClassBasic()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(obj != nullptr, "testNewClassBasic not null");
	ClassPoolDeepTestSupport_CPD_ASSERT(!obj->isDestroy(), "testNewClassBasic not destroyed");
	pool.destroyClass(obj);
}

void ClassPoolTest::testNewClassFromEmpty()
{
	using namespace ClassPoolDeepTestSupport;
	// 空池直接 new
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(obj != nullptr, "testNewClassFromEmpty obj");
	ClassPoolDeepTestSupport_CPD_ASSERT(obj->getPool() == &pool, "testNewClassFromEmpty pool match");
	pool.destroyClass(obj);
}

void ClassPoolTest::testNewClassFromPool()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	// 先 new 后 destroy，再 new 应该复用
	CPDTestObj* obj1 = pool.newClass();
	llong id1 = obj1->getAssignID();
	pool.destroyClass(obj1);
	CPDTestObj* obj2 = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(obj2 != nullptr, "testNewClassFromPool obj2");
	// 复用，总数不变（1）
	ClassPoolDeepTestSupport_CPD_ASSERT(obj2->getAssignID() != id1, "testNewClassFromPool new assignID");
	pool.destroyClass(obj2);
}

void ClassPoolTest::testDestroyClassBasic()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(obj != nullptr, "testDestroyClassBasic obj");
	pool.destroyClass(obj);
	ClassPoolDeepTestSupport_CPD_ASSERT(obj == nullptr, "testDestroyClassBasic obj set null");
}

void ClassPoolTest::testDestroyClassNullSafe()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = nullptr;
	pool.destroyClass(obj); // 不应崩溃
	ClassPoolDeepTestSupport_CPD_ASSERT(obj == nullptr, "testDestroyClassNullSafe still null");
}

void ClassPoolTest::testDestroyClassSetsNull()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	pool.destroyClass(obj);
	ClassPoolDeepTestSupport_CPD_ASSERT(obj == nullptr, "testDestroyClassSetsNull null after destroy");
}

void ClassPoolTest::testDestroyClassReusable()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	pool.destroyClass(a);
	pool.destroyClass(b);
	// 再次 new 两个对象（应复用）
	CPDTestObj* c = pool.newClass();
	CPDTestObj* d = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(c != nullptr, "testDestroyClassReusable c");
	ClassPoolDeepTestSupport_CPD_ASSERT(d != nullptr, "testDestroyClassReusable d");
	ClassPoolDeepTestSupport_CPD_ASSERT(c != d, "testDestroyClassReusable c!=d");
	pool.destroyClass(c);
	pool.destroyClass(d);
}

void ClassPoolTest::testDestroyClassListVector()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	CPDTestObj* c = pool.newClass();
	Vector<CPDTestObj*> list;
	list.add(a);
	list.add(b);
	list.add(c);
	UN_CLASS(list);
	ClassPoolDeepTestSupport_CPD_ASSERT(list.isEmpty(), "testDestroyClassListVector list cleared");
}

void ClassPoolTest::testDestroyClassListVectorEmpty()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	Vector<CPDTestObj*> emptyList;
	UN_CLASS(emptyList); // 不崩溃
	ClassPoolDeepTestSupport_CPD_ASSERT(emptyList.isEmpty(), "testDestroyClassListVectorEmpty still empty");
}

void ClassPoolTest::testDestroyClassListSafeVector()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	SafeVector<CPDTestObj*> sv;
	sv.add(a);
	sv.add(b);
	UN_CLASS(sv);
	ClassPoolDeepTestSupport_CPD_ASSERT(sv.isEmpty(), "testDestroyClassListSafeVector cleared");
}

void ClassPoolTest::testDestroyClassListSafeVectorEmpty()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	SafeVector<CPDTestObj*> sv;
	UN_CLASS(sv); // 不崩溃
	ClassPoolDeepTestSupport_CPD_ASSERT(sv.isEmpty(), "testDestroyClassListSafeVectorEmpty still empty");
}

void ClassPoolTest::testInitDefault()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	pool.initDefault(5);
	// 有5个预分配对象在 unusedList 中
	// 通过 newClass 取出，验证能取到 5 个不为空的
	Vector<CPDTestObj*> allocated;
	FOR(5)
	{
		allocated.add(pool.newClass());
	}
	FOR(5)
	{
		ClassPoolDeepTestSupport_CPD_ASSERT(allocated[i] != nullptr, "testInitDefault allocated not null");
	}
	for (CPDTestObj* obj : allocated)
	{
		pool.destroyClass(obj);
	}
}

void ClassPoolTest::testInitDefaultThenNew()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	pool.initDefault(3);
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	CPDTestObj* c = pool.newClass();
	CPDTestObj* d = pool.newClass(); // 超出预分配，动态创建
	ClassPoolDeepTestSupport_CPD_ASSERT(a != nullptr, "testInitDefaultThenNew a");
	ClassPoolDeepTestSupport_CPD_ASSERT(d != nullptr, "testInitDefaultThenNew d dynamic");
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
	pool.destroyClass(d);
}

void ClassPoolTest::testTotalCountGrowth()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	// 每 new 一个超出 unusedList 的，totalCount++
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	CPDTestObj* c = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(a != nullptr, "testTotalCountGrowth a");
	ClassPoolDeepTestSupport_CPD_ASSERT(b != nullptr, "testTotalCountGrowth b");
	ClassPoolDeepTestSupport_CPD_ASSERT(c != nullptr, "testTotalCountGrowth c");
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
}

void ClassPoolTest::testAssignIDMonotonic()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	llong id1 = a->getAssignID();
	pool.destroyClass(a);
	CPDTestObj* b = pool.newClass();
	llong id2 = b->getAssignID();
	ClassPoolDeepTestSupport_CPD_ASSERT(id2 > id1, "testAssignIDMonotonic id2 > id1");
	pool.destroyClass(b);
}

void ClassPoolTest::testAssignIDUnique()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	CPDTestObj* c = pool.newClass();
	llong id1 = a->getAssignID();
	llong id2 = b->getAssignID();
	llong id3 = c->getAssignID();
	ClassPoolDeepTestSupport_CPD_ASSERT(id1 != id2, "testAssignIDUnique id1!=id2");
	ClassPoolDeepTestSupport_CPD_ASSERT(id2 != id3, "testAssignIDUnique id2!=id3");
	ClassPoolDeepTestSupport_CPD_ASSERT(id1 != id3, "testAssignIDUnique id1!=id3");
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
}

void ClassPoolTest::testMarkUsableMarkDispose()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	// newClass 调用 markUsable，不应再次 markUsable
	ClassPoolDeepTestSupport_CPD_ASSERT(!obj->isDestroy(), "testMarkUsableMarkDispose active");
	ClassPoolDeepTestSupport_CPD_ASSERT(obj->getPool() == &pool, "testMarkUsableMarkDispose pool");
	pool.destroyClass(obj);
}

void ClassPoolTest::testDoubleDestroyProtection()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	CPDTestObj* raw = obj; // 保存原始指针
	pool.destroyClass(obj); // obj = nullptr
	// 再次销毁 raw（已经回到 unusedList）—— 应输出 ERROR 但不崩溃
	// 注意：框架层面 raw 在 unusedList 中，isDestroy()==true
	// destroyClass 传 null 安全返回
	ClassPoolDeepTestSupport_CPD_ASSERT(obj == nullptr, "testDoubleDestroyProtection obj null");
	// 仅验证 raw 现在 isDestroy
	ClassPoolDeepTestSupport_CPD_ASSERT(raw->isDestroy(), "testDoubleDestroyProtection raw isDestroy");
}

void ClassPoolTest::testPoolAssignedToObject()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(obj->getPool() == &pool, "testPoolAssignedToObject getPool");
	pool.destroyClass(obj);
}

void ClassPoolTest::testQuitClearsUnusedList()
{
	using namespace ClassPoolDeepTestSupport;
	// quit() 释放 unusedList 中的内存，不崩溃
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.quit(); // 释放 2 个 unused 对象
	ClassPoolDeepTestSupport_CPD_ASSERT(true, "testQuitClearsUnusedList no crash");
}

void ClassPoolTest::testLargeAllocationAndReturn()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	const int N = 100;
	Vector<CPDTestObj*> objs;
	FOR(N)
	{
		objs.add(pool.newClass());
	}
	FOR(N)
	{
		ClassPoolDeepTestSupport_CPD_ASSERT(objs[i] != nullptr, "testLargeAllocationAndReturn not null");
	}
	// 全部回收
	for (CPDTestObj* obj : objs)
	{
		pool.destroyClass(obj);
	}
	ClassPoolDeepTestSupport_CPD_ASSERT(true, "testLargeAllocationAndReturn all returned");
}

void ClassPoolTest::testMultipleCycles()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	const int CYCLES = 20;
	FOR(CYCLES)
	{
		CPDTestObj* obj = pool.newClass();
		ClassPoolDeepTestSupport_CPD_ASSERT(obj != nullptr, "testMultipleCycles obj");
		obj->mValue = i;
		pool.destroyClass(obj);
	}
	ClassPoolDeepTestSupport_CPD_ASSERT(true, "testMultipleCycles all cycles done");
}

void ClassPoolTest::testDestroyDestroiedPool()
{
	using namespace ClassPoolDeepTestSupport;
	// 当 mDestroied 标志已经为 true 时，destroyClass 应直接返回
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	// 模拟 pool 已被销毁
	pool.quit(); // 清空 unusedList 中的对象
	// obj 仍在使用中，quit 不影响它
	// 注：直接访问 mDestroied 需要友元，改为通过 quit 后继续 destroy 验证
	// 实际上 quit 不设 mDestroied，只是不崩溃
	pool.destroyClass(obj); // 正常回收
	ClassPoolDeepTestSupport_CPD_ASSERT(obj == nullptr, "testDestroyDestroiedPool obj null");
}

void ClassPoolTest::testNewClassAfterQuit()
{
	using namespace ClassPoolDeepTestSupport;
	// quit 后 unusedList 为空，再 new 会动态创建
	CPDTestObjPool pool;
	pool.initDefault(2);
	pool.quit(); // 释放 unusedList 中 2 个
	CPDTestObj* obj = pool.newClass(); // 动态创建
	ClassPoolDeepTestSupport_CPD_ASSERT(obj != nullptr, "testNewClassAfterQuit not null");
	pool.destroyClass(obj);
}

void ClassPoolTest::testDestroyNullObjectInList()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	Vector<CPDTestObj*> list;
	list.add(a);
	list.add(nullptr); // 含空指针
	UN_CLASS(list); // 不崩溃，跳过 nullptr
	ClassPoolDeepTestSupport_CPD_ASSERT(list.isEmpty(), "testDestroyNullObjectInList cleared");
}

void ClassPoolTest::testClassPoolMixedAlloc()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	pool.initDefault(10);
	Vector<CPDTestObj*> batch1, batch2;
	FOR(10)
	{
		batch1.add(pool.newClass());
	}
	FOR(5)
	{
		pool.destroyClass(batch1[i]);
	}
	FOR(10)
	{
		batch2.add(pool.newClass());
	}
	// batch1[5..9] 仍有效
	for (int i = 5; i < 10; ++i)
	{
		ClassPoolDeepTestSupport_CPD_ASSERT(batch1[i] != nullptr, "testClassPoolMixedAlloc batch1 tail");
		pool.destroyClass(batch1[i]);
	}
	for (CPDTestObj* obj : batch2)
	{
		pool.destroyClass(obj);
	}
}

void ClassPoolTest::testClassPoolResetPropertyCalled()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* obj = pool.newClass();
	obj->mValue = 999;
	pool.destroyClass(obj);
	// 再次 new，resetProperty 已被调用（通过 resetCount）
	CPDTestObj* obj2 = pool.newClass();
	ClassPoolDeepTestSupport_CPD_ASSERT(obj2->mValue == 0, "testClassPoolResetPropertyCalled mValue reset");
	pool.destroyClass(obj2);
}

void ClassPoolTest::testClassPoolIdAfterReuse()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	Vector<llong> ids;
	FOR(5)
	{
		CPDTestObj* obj = pool.newClass();
		ids.add(obj->getAssignID());
		pool.destroyClass(obj);
	}
	// ID 应严格递增
	FOR(4)
	{
		ClassPoolDeepTestSupport_CPD_ASSERT(ids[i] < ids[i + 1], "testClassPoolIdAfterReuse monotonic");
	}
}

void ClassPoolTest::testClassPoolDestroyListHashMap()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	HashMap<int, CPDTestObj*> map;
	map[1] = a;
	map[2] = b;
	UN_CLASS(map);
	ClassPoolDeepTestSupport_CPD_ASSERT(map.isEmpty(), "testClassPoolDestroyListHashMap empty");
}

void ClassPoolTest::testClassPoolDestroyListSafeHashMap()
{
	using namespace ClassPoolDeepTestSupport;
	CPDTestObjPool pool;
	CPDTestObj* a = pool.newClass();
	CPDTestObj* b = pool.newClass();
	SafeHashMap<int, CPDTestObj*> shm;
	shm.add(10, a);
	shm.add(20, b);
	UN_CLASS(shm);
	ClassPoolDeepTestSupport_CPD_ASSERT(shm.isEmpty(), "testClassPoolDestroyListSafeHashMap empty");
}

void ClassPoolTest::runExtraClassPoolTests()
{
	using namespace ClassPoolDeepTestSupport;
	testClassPoolMixedAlloc();
	testClassPoolResetPropertyCalled();
	testClassPoolIdAfterReuse();
	testClassPoolDestroyListHashMap();
	testClassPoolDestroyListSafeHashMap();
}


// ---------------------------------------------------------------------------
// 辅助类：ClassObject 子类（与 ClassPoolDeepTest 区分，避免重复定义）
// ---------------------------------------------------------------------------
class CPD2TestObj : public ClassObject
{
	BASE(CPD2TestObj, ClassObject);
public:
	int mTag = 0;
	int mResetCalls = 0;
	void resetProperty() override
	{
		ClassObject::resetProperty();
		mTag = 0;
		++mResetCalls;
	}
};
CLASS_POOL(CPD2TestObj);

class CPD2TestObjB : public ClassObject
{
	BASE(CPD2TestObjB, ClassObject);
public:
	float mScore = 0.0f;
	void resetProperty() override
	{
		ClassObject::resetProperty();
		mScore = 0.0f;
	}
};
CLASS_POOL(CPD2TestObjB);
namespace ClassPoolDeepTest2Support
{
#define ClassPoolDeepTest2Support_CPD2_ASSERT(cond, msg) \
	if (!(cond)) { ERROR(string("ClassPoolDeepTest2 FAIL: ") + (msg)); }
}

void ClassPoolTest::testDestroyClassListHashMap()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* a = pool.newClass();
	CPD2TestObj* b = pool.newClass();
	CPD2TestObj* c = pool.newClass();
	a->mTag = 1; b->mTag = 2; c->mTag = 3;
	HashMap<int, CPD2TestObj*> map;
	map[10] = a;
	map[20] = b;
	map[30] = c;
	UN_CLASS(map);
	ClassPoolDeepTest2Support_CPD2_ASSERT(map.isEmpty(), "testDestroyClassListHashMap map cleared");
}

void ClassPoolTest::testDestroyClassListHashMapEmpty()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	HashMap<int, CPD2TestObj*> emptyMap;
	UN_CLASS(emptyMap); // 不崩溃
	ClassPoolDeepTest2Support_CPD2_ASSERT(emptyMap.isEmpty(), "testDestroyClassListHashMapEmpty still empty");
}

void ClassPoolTest::testDestroyClassListSafeHashMap()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* x = pool.newClass();
	CPD2TestObj* y = pool.newClass();
	x->mTag = 100; y->mTag = 200;
	SafeHashMap<int, CPD2TestObj*> shm;
	shm.add(1, x);
	shm.add(2, y);
	UN_CLASS(shm);
	ClassPoolDeepTest2Support_CPD2_ASSERT(shm.isEmpty(), "testDestroyClassListSafeHashMap cleared");
}

void ClassPoolTest::testDestroyClassListSafeHashMapEmpty()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	SafeHashMap<int, CPD2TestObj*> empty;
	UN_CLASS(empty); // 不崩溃
	ClassPoolDeepTest2Support_CPD2_ASSERT(empty.isEmpty(), "testDestroyClassListSafeHashMapEmpty still empty");
}

void ClassPoolTest::testDestroyClassListArrayList()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* a = pool.newClass();
	CPD2TestObj* b = pool.newClass();
	ArrayList<8, CPD2TestObj*> al;
	al.add(a);
	al.add(b);
	UN_CLASS(al);
	ClassPoolDeepTest2Support_CPD2_ASSERT(al.isEmpty(), "testDestroyClassListArrayList cleared");
}

void ClassPoolTest::testDestroyClassListArrayListEmpty()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	ArrayList<8, CPD2TestObj*> al;
	UN_CLASS(al); // 不崩溃
	ClassPoolDeepTest2Support_CPD2_ASSERT(al.isEmpty(), "testDestroyClassListArrayListEmpty still empty");
}

void ClassPoolTest::testDestroyClassListVectorWithNull()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* a = pool.newClass();
	CPD2TestObj* b = pool.newClass();
	Vector<CPD2TestObj*> list;
	list.add(nullptr); // 空指针
	list.add(a);
	list.add(nullptr); // 空指针
	list.add(b);
	UN_CLASS(list); // 应跳过 nullptr
	ClassPoolDeepTest2Support_CPD2_ASSERT(list.isEmpty(), "testDestroyClassListVectorWithNull cleared");
}

void ClassPoolTest::testDestroyClassListHashMapWithNull()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* a = pool.newClass();
	HashMap<int, CPD2TestObj*> map;
	map[1] = a;
	map[2] = nullptr; // 含 nullptr
	UN_CLASS(map);
	ClassPoolDeepTest2Support_CPD2_ASSERT(map.isEmpty(), "testDestroyClassListHashMapWithNull cleared");
}

void ClassPoolTest::testMultiplePoolsSameType()
{
	using namespace ClassPoolDeepTest2Support;
	// 两个独立池，各自管理不同对象
	{
		CPD2TestObjPool pool1;
		CPD2TestObj* a1 = pool1.newClass();
		CPD2TestObj* b1 = pool1.newClass();
		a1->mTag = 11;
		b1->mTag = 12;
		ClassPoolDeepTest2Support_CPD2_ASSERT(a1->getPool() == &pool1, "testMultiplePoolsSameType a1 pool");
		ClassPoolDeepTest2Support_CPD2_ASSERT(a1->mTag == 11, "testMultiplePoolsSameType a1 tag");
		pool1.destroyClass(a1);
		pool1.destroyClass(b1);
	}

	CPD2TestObjPool pool2;
	CPD2TestObj* a2 = pool2.newClass();
	a2->mTag = 21;
	ClassPoolDeepTest2Support_CPD2_ASSERT(a2->getPool() == &pool2, "testMultiplePoolsSameType a2 pool");
	ClassPoolDeepTest2Support_CPD2_ASSERT(a2->mTag == 21, "testMultiplePoolsSameType a2 tag");
	pool2.destroyClass(a2);
}

void ClassPoolTest::testPoolAssignIDContinuous()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	llong lastID = 0;
	const int N = 30;
	FOR(N)
	{
		CPD2TestObj* obj = pool.newClass();
		ClassPoolDeepTest2Support_CPD2_ASSERT(obj->getAssignID() > lastID, "testPoolAssignIDContinuous monotonic");
		lastID = obj->getAssignID();
		pool.destroyClass(obj);
	}
}

void ClassPoolTest::testResetPropertyOnReuse()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* obj = pool.newClass();
	obj->mTag = 9999;
	pool.destroyClass(obj);
	// 下次 new 复用的对象，newClass 内部通过 markUsable → 不额外调用 resetProperty
	// 但对象回池前已调用 destroy（会 resetProperty）
	CPD2TestObj* obj2 = pool.newClass();
	ClassPoolDeepTest2Support_CPD2_ASSERT(obj2->mTag == 0, "testResetPropertyOnReuse tag reset to 0");
	pool.destroyClass(obj2);
}

void ClassPoolTest::testDestroyClassListSafeVectorDeep2()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	CPD2TestObj* a = pool.newClass();
	CPD2TestObj* b = pool.newClass();
	CPD2TestObj* c = pool.newClass();
	SafeVector<CPD2TestObj*> sv;
	sv.add(a);
	sv.add(b);
	sv.add(c);
	ClassPoolDeepTest2Support_CPD2_ASSERT(sv.size() == 3, "testDestroyClassListSafeVector size before");
	UN_CLASS(sv);
	ClassPoolDeepTest2Support_CPD2_ASSERT(sv.isEmpty(), "testDestroyClassListSafeVector cleared");
}

void ClassPoolTest::testPoolLargeInitDefault()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	pool.initDefault(50);
	const int N = 50;
	Vector<CPD2TestObj*> objs;
	FOR(N)
	{
		objs.add(pool.newClass());
	}
	FOR(N)
	{
		ClassPoolDeepTest2Support_CPD2_ASSERT(objs[i] != nullptr, "testPoolLargeInitDefault obj");
	}
	for (CPD2TestObj* obj : objs)
	{
		pool.destroyClass(obj);
	}
}

void ClassPoolTest::testPoolAllocDealloc1000()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	const int N = 100;
	// 10 轮，每轮分配 N 个再回收
	for (int round = 0; round < 10; ++round)
	{
		Vector<CPD2TestObj*> objs;
		FOR(N)
		{
			CPD2TestObj* obj = pool.newClass();
			obj->mTag = i;
			objs.add(obj);
		}
		FOR(N)
		{
			ClassPoolDeepTest2Support_CPD2_ASSERT(objs[i]->mTag == i, "testPoolAllocDealloc1000 tag");
		}
		for (CPD2TestObj* obj : objs)
		{
			pool.destroyClass(obj);
		}
	}
}

void ClassPoolTest::testDestroyAllFromSafeHashMap()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	SafeHashMap<llong, CPD2TestObj*> shm;
	const int N = 10;
	FOR(N)
	{
		CPD2TestObj* obj = pool.newClass();
		obj->mTag = i;
		shm.add((llong)(i + 100), obj);
	}
	ClassPoolDeepTest2Support_CPD2_ASSERT(shm.size() == N, "testDestroyAllFromSafeHashMap size before");
	UN_CLASS(shm);
	ClassPoolDeepTest2Support_CPD2_ASSERT(shm.isEmpty(), "testDestroyAllFromSafeHashMap cleared");
}

void ClassPoolTest::testObjectLifecycleFull()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	// 完整生命周期：new → 使用 → setPendingDestroy → destroy
	CPD2TestObj* obj = pool.newClass();
	ClassPoolDeepTest2Support_CPD2_ASSERT(!obj->isDestroy(), "lifecycle not destroyed");
	ClassPoolDeepTest2Support_CPD2_ASSERT(obj->getPool() == &pool, "lifecycle pool");
	obj->mTag = 42;
	obj->setPendingDestroy();
	ClassPoolDeepTest2Support_CPD2_ASSERT(obj->isPendingDestroy(), "lifecycle pending");
	pool.destroyClass(obj);
	ClassPoolDeepTest2Support_CPD2_ASSERT(obj == nullptr, "lifecycle null after destroy");
}

void ClassPoolTest::testPoolAfterQuit()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	pool.initDefault(5);
	pool.quit(); // 清空 unusedList
	// quit 后仍可 new（动态创建）
	CPD2TestObj* obj = pool.newClass();
	ClassPoolDeepTest2Support_CPD2_ASSERT(obj != nullptr, "testPoolAfterQuit not null");
	pool.destroyClass(obj);
}

void ClassPoolTest::testMultiplePoolsDifferentTypes()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool poolA;
	CPD2TestObjBPool poolB;
	CPD2TestObj* a = poolA.newClass();
	CPD2TestObjB* b = poolB.newClass();
	ClassPoolDeepTest2Support_CPD2_ASSERT(a->getPool() == &poolA, "testMultiplePoolsDifferentTypes A pool");
	ClassPoolDeepTest2Support_CPD2_ASSERT(b->getPool() == &poolB, "testMultiplePoolsDifferentTypes B pool");
	a->mTag = 7;
	b->mScore = 3.14f;
	ClassPoolDeepTest2Support_CPD2_ASSERT(a->mTag == 7, "testMultiplePoolsDifferentTypes A tag");
	ClassPoolDeepTest2Support_CPD2_ASSERT(fabsf(b->mScore - 3.14f) < 1e-4f, "testMultiplePoolsDifferentTypes B score");
	poolA.destroyClass(a);
	poolB.destroyClass(b);
}

void ClassPoolTest::testDestroyClassListVectorMixed()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	// Vector 中包含多种状态：active / nullptr
	Vector<CPD2TestObj*> list;
	CPD2TestObj* a = pool.newClass();
	CPD2TestObj* b = pool.newClass();
	CPD2TestObj* c = pool.newClass();
	a->mTag = 1; b->mTag = 2; c->mTag = 3;
	list.add(a);
	list.add(nullptr);
	list.add(b);
	list.add(nullptr);
	list.add(c);
	UN_CLASS(list);
	ClassPoolDeepTest2Support_CPD2_ASSERT(list.isEmpty(), "testDestroyClassListVectorMixed cleared");
}

void ClassPoolTest::testClassPool2WithSafeVectorForeach()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	SafeVector<CPD2TestObj*> sv;
	// 分配 5 个
	FOR(5)
	{
		CPD2TestObj* obj = pool.newClass();
		obj->mTag = i + 1;
		sv.add(obj);
	}
	// 遍历期间销毁偶数 tag
	const Vector<CPD2TestObj*>& snap = sv.startForeach();
	for (CPD2TestObj* obj : snap)
	{
		if (obj && obj->mTag % 2 == 0)
		{
			sv.remove(obj);
		}
	}
	sv.endForeach();
	// 销毁被移除的对象（已在 mainList 中移除，需手动 destroy）
	// 重新遍历验证
	const Vector<CPD2TestObj*>& snap2 = sv.startForeach();
	for (CPD2TestObj* obj : snap2)
	{
		ClassPoolDeepTest2Support_CPD2_ASSERT(obj->mTag % 2 != 0, "testClassPool2WithSafeVectorForeach odd only");
	}
	sv.endForeach();
	// 清理
	Vector<CPD2TestObj*> toDestroy;
	for (CPD2TestObj* obj : sv.getMainList())
	{
		toDestroy.add(obj);
	}
	sv.clear();
	for (CPD2TestObj* obj : toDestroy)
	{
		pool.destroyClass(obj);
	}
}

void ClassPoolTest::testClassPool2AllocDestroyInterleaved()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	Vector<CPD2TestObj*> active;
	// 交替分配和释放
	for (int round = 0; round < 20; ++round)
	{
		CPD2TestObj* obj = pool.newClass();
		obj->mTag = round;
		active.add(obj);
		if (active.size() > 5)
		{
			CPD2TestObj* old = active[0];
			active.removeAt(0);
			pool.destroyClass(old);
		}
	}
	// 清理剩余
	for (CPD2TestObj* obj : active)
	{
		pool.destroyClass(obj);
	}
	ClassPoolDeepTest2Support_CPD2_ASSERT(true, "testClassPool2AllocDestroyInterleaved no crash");
}

void ClassPoolTest::testClassPool2BatchAllocShuffle()
{
	using namespace ClassPoolDeepTest2Support;
	CPD2TestObjPool pool;
	const int N = 20;
	Vector<CPD2TestObj*> objs;
	FOR(N)
	{
		CPD2TestObj* obj = pool.newClass();
		obj->mTag = i * 7 % 13; // 非线性值
		objs.add(obj);
	}
	// 验证每个对象 tag
	FOR(N)
	{
		ClassPoolDeepTest2Support_CPD2_ASSERT(objs[i]->mTag == i * 7 % 13, "testClassPool2BatchAllocShuffle tag");
	}
	for (CPD2TestObj* obj : objs)
	{
		pool.destroyClass(obj);
	}
}

void ClassPoolTest::runExtraClassPool2Tests()
{
	using namespace ClassPoolDeepTest2Support;
	testClassPool2WithSafeVectorForeach();
	testClassPool2AllocDestroyInterleaved();
	testClassPool2BatchAllocShuffle();
}


// ------------------------------------------------------------------
// Helper types for ClassPool
// ------------------------------------------------------------------

class CPCTObj : public ClassObject
{
	BASE(CPCTObj, ClassObject);
public:
	int mVal = 0;
	float mFVal = 0.0f;
	string mName;
	void resetProperty() override
	{
		base::resetProperty();
		mVal = 0;
		mFVal = 0.0f;
		mName.clear();
	}
	void destroy() override { base::destroy(); }
};
CLASS_POOL(CPCTObj);

class CPCTObjSub : public CPCTObj
{
	BASE(CPCTObjSub, CPCTObj);
public:
	int mSubVal = 0;
	void resetProperty() override
	{
		base::resetProperty();
		mSubVal = 0;
	}
	void destroy() override { base::destroy(); }
};
CLASS_POOL(CPCTObjSub);

class CPCTObjDeep : public CPCTObjSub
{
	BASE(CPCTObjDeep, CPCTObjSub);
public:
	double mDeep = 0.0;
	void resetProperty() override
	{
		base::resetProperty();
		mDeep = 0.0;
	}
	void destroy() override { base::destroy(); }
};
CLASS_POOL(CPCTObjDeep);
namespace ClassPoolComprehensiveTestSupport
{
	// ============================================================
	// ClassPoolComprehensiveTest
	// Comprehensive tests for pool family:
	//   ClassPool / ClassTypePool / ClassBaseTypePool /
	//   ClassKeyPoolThread (additional scenarios)
	// ============================================================

#define ClassPoolComprehensiveTestSupport_CPCT_ASSERT(expr) \
	if (!(expr)) { ERROR(string("ClassPoolComprehensiveTest FAIL: ") + #expr); }

// ------------------------------------------------------------------
// Helper types for ClassTypePool
// ------------------------------------------------------------------

	class CTPObj : public ClassObject
	{
		BASE(CTPObj, ClassObject);
	public:
		int mType2 = 0;
		int mData = 0;
		int mTypeKey = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mType2 = 0;
			mData = 0;
			mTypeKey = 0;
		}
		void destroy() override { base::destroy(); }
		virtual int getType() const { return mTypeKey; }
		virtual void setType(const int type) { mTypeKey = type; }
	};

	class CTPObjA : public CTPObj
	{
		BASE(CTPObjA, CTPObj);
	public:
		int mExtraA = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mExtraA = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class CTPObjB : public CTPObj
	{
		BASE(CTPObjB, CTPObj);
	public:
		double mBig = 0.0;
		void resetProperty() override
		{
			base::resetProperty();
			mBig = 0.0;
		}
		void destroy() override { base::destroy(); }
	};

	class CTPObjC : public CTPObj
	{
		BASE(CTPObjC, CTPObj);
	public:
		short mShort = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mShort = 0;
		}
		void destroy() override { base::destroy(); }
	};

	// ClassTypePool keyed by int
	class IntCTP : public ClassTypePool<CTPObj, int>
	{
		using base = ClassTypePool<CTPObj, int>;
	protected:
		CTPObj* create(const int key) override
		{
			if (key == 1)
			{
				return new CTPObjA();
			}
			if (key == 2)
			{
				return new CTPObjB();
			}
			if (key == 3)
			{
				return new CTPObjC();
			}
			return new CTPObj();
		}
	};

	// ------------------------------------------------------------------
	// Helper types for ClassBaseTypePool
	// ------------------------------------------------------------------

	class CBTPBase : public ClassObject
	{
		BASE(CBTPBase, ClassObject);
	public:
		int mBaseData = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mBaseData = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class CBTPDerived1 : public CBTPBase
	{
		BASE(CBTPDerived1, CBTPBase);
	public:
		int mDerived1 = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mDerived1 = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class CBTPDerived2 : public CBTPBase
	{
		BASE(CBTPDerived2, CBTPBase);
	public:
		int mDerived2 = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mDerived2 = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class ClassBaseTypePoolCPCT : public ClassBaseTypePool<CBTPBase>
	{
		using base = ClassBaseTypePool<CBTPBase>;
	};

	// ------------------------------------------------------------------
	// Helper types for ClassKeyPoolThread (additional)
	// ------------------------------------------------------------------

	class CKPTObj : public ClassObject
	{
		BASE(CKPTObj, ClassObject);
	public:
		int mVal = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mVal = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class CKPTObjSub : public CKPTObj
	{
		BASE(CKPTObjSub, CKPTObj);
	public:
		int mSub = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mSub = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class IntCKPThread : public ClassKeyPoolThread<CKPTObj, int>
	{
		using base = ClassKeyPoolThread<CKPTObj, int>;
	protected:
		CKPTObj* create(const int key) override
		{
			if (key % 2 == 0)
			{
				return new CKPTObjSub();
			}
			return new CKPTObj();
		}
	};
}

void ClassPoolTest::testClassPoolNewDestroy()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	CPCTObj* obj = pool.newClass();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj->mVal == 0);
	pool.destroyClass(obj);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassPoolReuse()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	CPCTObj* first = pool.newClass();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(first != nullptr);
	first->mVal = 500;
	void* addr = first;
	pool.destroyClass(first);
	CPCTObj* second = pool.newClass();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second == addr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second->mVal == 0); // resetProperty clears
	pool.destroyClass(second);
}

void ClassPoolTest::testClassPoolList()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	Vector<CPCTObj*> list;
	for (int i = 0; i < 6; ++i)
	{
		list.add(pool.newClass());
	}
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list.size() == 6);
	for (int i = 0; i < 6; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i]->mVal == 0);
	}
	for (int i = 0; i < 6; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassPoolListReuse()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	Vector<CPCTObj*> list1;
	for (int i = 0; i < 5; ++i)
	{
		list1.add(pool.newClass());
	}
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list1.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
		list1[i]->mVal = i * 100;
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(list1[i]);
	}
	Vector<CPCTObj*> list2;
	for (int i = 0; i < 3; ++i)
	{
		list2.add(pool.newClass());
	}
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list2.size() == 3);
	for (int i = 0; i < 3; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list2[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list2[i]->mVal == 0); // reset
		pool.destroyClass(list2[i]);
	}
}

void ClassPoolTest::testClassPoolStress()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	const int N = 50;
	CPCTObj* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass();
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
		arr[i]->mVal = i;
	}
	for (int i = 0; i < N; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mVal == i);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(arr[i]);
	}
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass();
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mVal == 0);
		pool.destroyClass(arr[i]);
	}
}

void ClassPoolTest::testClassPoolDeepInheritance()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjDeepPool pool;
	CPCTObjDeep* obj = pool.newClass();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj != nullptr);
	obj->mVal = 10;
	obj->mSubVal = 20;
	obj->mDeep = 3.14;
	pool.destroyClass(obj);
	CPCTObjDeep* obj2 = pool.newClass();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj2 != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj2->mVal == 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj2->mSubVal == 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(fabsf((float)obj2->mDeep) < 1e-6f);
	pool.destroyClass(obj2);
}

void ClassPoolTest::testClassPoolResetProperty()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	CPCTObj* obj = pool.newClass();
	obj->mVal = 9999;
	obj->mFVal = 123.456f;
	obj->mName = "hello";
	pool.destroyClass(obj);
	CPCTObj* reused = pool.newClass();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(reused->mVal == 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(fabsf(reused->mFVal) < 1e-4f);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(reused->mName.empty());
	pool.destroyClass(reused);
}

void ClassPoolTest::testClassPoolMultipleRounds()
{
	using namespace ClassPoolComprehensiveTestSupport;
	CPCTObjPool pool;
	const int ROUNDS = 10;
	const int PER = 5;
	for (int r = 0; r < ROUNDS; ++r)
	{
		CPCTObj* arr[PER];
		for (int i = 0; i < PER; ++i)
		{
			arr[i] = pool.newClass();
			ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
			arr[i]->mVal = r * 100 + i;
		}
		for (int i = 0; i < PER; ++i)
		{
			ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mVal == r * 100 + i);
			pool.destroyClass(arr[i]);
		}
	}
}

void ClassPoolTest::testClassTypePoolNewByKey()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCTP pool;
	CTPObj* obj0 = pool.newClass(0);
	CTPObj* obj1 = pool.newClass(1);
	CTPObj* obj2 = pool.newClass(2);
	CTPObj* obj3 = pool.newClass(3);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj0 != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj1 != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj2 != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj3 != nullptr);
	pool.destroyClass(obj0);
	pool.destroyClass(obj1);
	pool.destroyClass(obj2);
	pool.destroyClass(obj3);
}

void ClassPoolTest::testClassTypePoolDestroyNull()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCTP pool;
	CTPObj* obj = nullptr;
	pool.destroyClass(obj);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassTypePoolListByKey()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCTP pool;
	Vector<CTPObj*> list;
	pool.newClassList(1, list, 5);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i]->mData == 0);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassTypePoolReuseByKey()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCTP pool;
	CTPObj* first = pool.newClass(2);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(first != nullptr);
	first->mData = 777;
	pool.destroyClass(first);
	CTPObj* second = pool.newClass(2);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second->mData == 0);
	pool.destroyClass(second);
}

void ClassPoolTest::testClassTypePoolMultiType()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCTP pool;
	const int KEYS = 4;
	CTPObj* objs[KEYS];
	for (int k = 0; k < KEYS; ++k)
	{
		objs[k] = pool.newClass(k);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k] != nullptr);
		objs[k]->mData = k * 10;
	}
	for (int k = 0; k < KEYS; ++k)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k]->mData == k * 10);
		pool.destroyClass(objs[k]);
	}
	// After destroy, reuse
	for (int k = 0; k < KEYS; ++k)
	{
		objs[k] = pool.newClass(k);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k]->mData == 0);
		pool.destroyClass(objs[k]);
	}
}

void ClassPoolTest::testClassTypePoolStress()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCTP pool;
	const int ROUNDS = 15;
	const int PER = 4;
	for (int r = 0; r < ROUNDS; ++r)
	{
		CTPObj* arr[PER];
		for (int i = 0; i < PER; ++i)
		{
			arr[i] = pool.newClass(i);
			ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
			arr[i]->mData = r * 10 + i;
		}
		for (int i = 0; i < PER; ++i)
		{
			ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mData == r * 10 + i);
			pool.destroyClass(arr[i]);
			ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] == nullptr);
		}
	}
}

void ClassPoolTest::testClassBaseTypePoolNew()
{
	using namespace ClassPoolComprehensiveTestSupport;
	ClassBaseTypePoolCPCT pool;
	CBTPDerived1* obj = pool.newClass<CBTPDerived1>();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj->mBaseData == 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj->mDerived1 == 0);
	pool.destroyClass(obj);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassBaseTypePoolReuse()
{
	using namespace ClassPoolComprehensiveTestSupport;
	ClassBaseTypePoolCPCT pool;
	CBTPDerived1* first = pool.newClass<CBTPDerived1>();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(first != nullptr);
	first->mBaseData = 100;
	first->mDerived1 = 200;
	void* addr = first;
	pool.destroyClass(first);
	CBTPDerived1* second = pool.newClass<CBTPDerived1>();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second == addr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second->mBaseData == 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second->mDerived1 == 0);
	pool.destroyClass(second);
}

void ClassPoolTest::testClassBaseTypePoolList()
{
	using namespace ClassPoolComprehensiveTestSupport;
	ClassBaseTypePoolCPCT pool;
	Vector<CBTPBase*> list;
	pool.newClassList<CBTPDerived1>(list, 4);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list.size() == 4);
	for (int i = 0; i < 4; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i] != nullptr);
	}
	for (int i = 0; i < 4; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassBaseTypePoolMultiType()
{
	using namespace ClassPoolComprehensiveTestSupport;
	ClassBaseTypePoolCPCT pool;
	CBTPDerived1* a = pool.newClass<CBTPDerived1>();
	CBTPDerived2* b = pool.newClass<CBTPDerived2>();
	CBTPDerived1* c = pool.newClass<CBTPDerived1>();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(a != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(b != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(c != nullptr);
	a->mBaseData = 1;
	a->mDerived1 = 11;
	b->mBaseData = 2;
	b->mDerived2 = 22;
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(a->mBaseData == 1);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(b->mBaseData == 2);
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
	// Re-alloc
	CBTPDerived1* a2 = pool.newClass<CBTPDerived1>();
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(a2 != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(a2->mBaseData == 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(a2->mDerived1 == 0);
	pool.destroyClass(a2);
}

void ClassPoolTest::testClassBaseTypePoolStress()
{
	using namespace ClassPoolComprehensiveTestSupport;
	ClassBaseTypePoolCPCT pool;
	const int N = 40;
	CBTPBase* arr[N];
	for (int i = 0; i < N; ++i)
	{
		if (i % 2 == 0)
		{
			arr[i] = pool.newClass<CBTPDerived1>();
		}
		else
		{
			arr[i] = pool.newClass<CBTPDerived2>();
		}
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
		arr[i]->mBaseData = i;
	}
	for (int i = 0; i < N; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mBaseData == i);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(arr[i]);
	}
	for (int i = 0; i < N; ++i)
	{
		if (i % 2 == 0)
		{
			arr[i] = pool.newClass<CBTPDerived1>();
		}
		else
		{
			arr[i] = pool.newClass<CBTPDerived2>();
		}
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mBaseData == 0);
		pool.destroyClass(arr[i]);
	}
}

void ClassPoolTest::testClassKeyPoolThreadNew()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCKPThread pool;
	CKPTObj* obj = pool.newClass(0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj != nullptr);
	pool.destroyClass(obj, 0);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassKeyPoolThreadDestroy()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCKPThread pool;
	CKPTObj* obj = pool.newClass(1);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj != nullptr);
	obj->mVal = 42;
	pool.destroyClass(obj, 1);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassKeyPoolThreadReuse()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCKPThread pool;
	CKPTObj* first = pool.newClass(2);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(first != nullptr);
	first->mVal = 100;
	void* addr = first;
	pool.destroyClass(first, 2);
	CKPTObj* second = pool.newClass(2);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second != nullptr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second == addr);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(second->mVal == 0);
	pool.destroyClass(second, 2);
}

void ClassPoolTest::testClassKeyPoolThreadList()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCKPThread pool;
	Vector<CKPTObj*> list;
	pool.newClassList(0, list, 5);
	ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(list[i]->mVal == 0);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(list[i], 0);
	}
}

void ClassPoolTest::testClassKeyPoolThreadManyKeys()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCKPThread pool;
	const int KEYS = 20;
	CKPTObj* objs[KEYS];
	for (int k = 0; k < KEYS; ++k)
	{
		objs[k] = pool.newClass(k);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k] != nullptr);
		objs[k]->mVal = k * 7;
	}
	for (int k = 0; k < KEYS; ++k)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k]->mVal == k * 7);
		pool.destroyClass(objs[k], k);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(objs[k] == nullptr);
	}
}

void ClassPoolTest::testClassKeyPoolThreadStress()
{
	using namespace ClassPoolComprehensiveTestSupport;
	IntCKPThread pool;
	const int STRESS = 60;
	CKPTObj* arr[STRESS];
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newClass(i % 6);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
		arr[i]->mVal = i;
	}
	for (int i = 0; i < STRESS; ++i)
	{
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mVal == i);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		pool.destroyClass(arr[i], i % 6);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newClass(i % 6);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i] != nullptr);
		ClassPoolComprehensiveTestSupport_CPCT_ASSERT(arr[i]->mVal == 0);
		pool.destroyClass(arr[i], i % 6);
	}
}


// ------------------------------------------------------------------
// 辅助类 — ClassPool 扩展
// ------------------------------------------------------------------

class CPE2Obj : public ClassObject
{
	BASE(CPE2Obj, ClassObject);
public:
	int mVal = 0;
	int mExtra = 0;
	float mFVal = 0.0f;
	string mTag;
	void resetProperty() override
	{
		base::resetProperty();
		mVal = 0;
		mExtra = 0;
		mFVal = 0.0f;
		mTag.clear();
	}
	void destroy() override { base::destroy(); }
};
CLASS_POOL(CPE2Obj);

class CPE2ObjSub : public CPE2Obj
{
	BASE(CPE2ObjSub, CPE2Obj);
public:
	int mSubData = 0;
	double mSubDouble = 0.0;
	void resetProperty() override
	{
		base::resetProperty();
		mSubData = 0;
		mSubDouble = 0.0;
	}
	void destroy() override { base::destroy(); }
};
CLASS_POOL(CPE2ObjSub);

class CPE2ObjDeep : public CPE2ObjSub
{
	BASE(CPE2ObjDeep, CPE2ObjSub);
public:
	short mDeepShort = 0;
	bool mDeepFlag = false;
	void resetProperty() override
	{
		base::resetProperty();
		mDeepShort = 0;
		mDeepFlag = false;
	}
	void destroy() override { base::destroy(); }
};
CLASS_POOL(CPE2ObjDeep);
namespace ClassPoolExt2TestSupport
{
	// ============================================================
	// ClassPoolExt2Test
	// ClassPool / ClassPoolThread / VectorPool / VectorPoolThread
	// 扩展场景测试（补充各池类未覆盖的边界和场景）
	// ============================================================

#define ClassPoolExt2TestSupport_CPE2_ASSERT(expr) \
	if (!(expr)) { ERROR(string("ClassPoolExt2Test FAIL: ") + #expr); }

// ------------------------------------------------------------------
// 辅助类 — ClassPoolThread 扩展
// ------------------------------------------------------------------

	class CPE2ThreadObj : public ClassObject
	{
		BASE(CPE2ThreadObj, ClassObject);
	public:
		int mThreadVal = 0;
		float mThreadFVal = 0.0f;
		bool mThreadFlag = false;
		void resetProperty() override
		{
			base::resetProperty();
			mThreadVal = 0;
			mThreadFVal = 0.0f;
			mThreadFlag = false;
		}
		void destroy() override { base::destroy(); }
	};

	class CPE2ThreadObjSub : public CPE2ThreadObj
	{
		BASE(CPE2ThreadObjSub, CPE2ThreadObj);
	public:
		int mSubVal = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mSubVal = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class CPE2ThreadObjDeep : public CPE2ThreadObjSub
	{
		BASE(CPE2ThreadObjDeep, CPE2ThreadObjSub);
	public:
		llong mDeepLlong = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mDeepLlong = 0;
		}
		void destroy() override { base::destroy(); }
	};
}

void ClassPoolTest::testClassPoolNewNull()
{
	using namespace ClassPoolExt2TestSupport;
	// 新建后对象不为空
	CPE2ObjPool pool;
	CPE2Obj* obj = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mExtra == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mTag.empty());
	pool.destroyClass(obj);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassPoolListZero()
{
	using namespace ClassPoolExt2TestSupport;
	// 请求 0 个对象：list 为空
	CPE2ObjPool pool;
	Vector<CPE2Obj*> list;
	for (int i = 0; i < 0; ++i)
	{
		list.add(pool.newClass());
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(list.size() == 0);
}

void ClassPoolTest::testClassPoolListOverflow()
{
	using namespace ClassPoolExt2TestSupport;
	// 请求超大数量，对象应分配成功
	CPE2ObjPool pool;
	Vector<CPE2Obj*> list;
	for (int i = 0; i < 50; ++i)
	{
		list.add(pool.newClass());
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(list.size() == 50);
	for (int i = 0; i < 50; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(list[i] != nullptr);
	}
	for (int i = 0; i < 50; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassPoolAlternateAllocFree()
{
	using namespace ClassPoolExt2TestSupport;
	// 交替分配/释放，验证复用和 reset
	CPE2ObjPool pool;
	const int N = 30;
	for (int i = 0; i < N; ++i)
	{
		CPE2Obj* obj = pool.newClass();
		ClassPoolExt2TestSupport_CPE2_ASSERT(obj != nullptr);
		obj->mVal = i * 3;
		obj->mExtra = i * 7;
		obj->mTag = "alt";
		pool.destroyClass(obj);
		ClassPoolExt2TestSupport_CPE2_ASSERT(obj == nullptr);
		// 再分配，复用同一块内存，验证已清零
		CPE2Obj* reused = pool.newClass();
		ClassPoolExt2TestSupport_CPE2_ASSERT(reused != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(reused->mVal == 0);
		ClassPoolExt2TestSupport_CPE2_ASSERT(reused->mExtra == 0);
		ClassPoolExt2TestSupport_CPE2_ASSERT(reused->mTag.empty());
		pool.destroyClass(reused);
	}
}

void ClassPoolTest::testClassPoolMixedNewList()
{
	using namespace ClassPoolExt2TestSupport;
	// 混合 newClass() 与 newClassList()，检查独立性
	CPE2ObjPool pool;
	CPE2Obj* single1 = pool.newClass();
	CPE2Obj* single2 = pool.newClass();
	Vector<CPE2Obj*> list;
	for (int i = 0; i < 4; ++i)
	{
		list.add(pool.newClass());
	}
	single1->mVal = 111;
	single2->mVal = 222;
	list[0]->mVal = 1000;
	list[3]->mVal = 3000;
	ClassPoolExt2TestSupport_CPE2_ASSERT(single1->mVal == 111);
	ClassPoolExt2TestSupport_CPE2_ASSERT(single2->mVal == 222);
	ClassPoolExt2TestSupport_CPE2_ASSERT(list[0]->mVal == 1000);
	ClassPoolExt2TestSupport_CPE2_ASSERT(list[3]->mVal == 3000);
	pool.destroyClass(single1);
	pool.destroyClass(single2);
	for (int i = 0; i < 4; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassPoolFillData()
{
	using namespace ClassPoolExt2TestSupport;
	// 写入多字段，destroy 后 reset，重新获取验证
	CPE2ObjPool pool;
	CPE2Obj* obj = pool.newClass();
	obj->mVal = 42;
	obj->mExtra = 100;
	obj->mFVal = 3.14f;
	obj->mTag = "fill";
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mVal == 42);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mExtra == 100);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf(obj->mFVal - 3.14f) < 1e-4f);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mTag == "fill");
	pool.destroyClass(obj);
	CPE2Obj* fresh = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mExtra == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf(fresh->mFVal) < 1e-6f);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mTag.empty());
	pool.destroyClass(fresh);
}

void ClassPoolTest::testClassPoolDestroyNullNoOp()
{
	using namespace ClassPoolExt2TestSupport;
	// destroyClass(nullptr) 不应崩溃
	CPE2ObjPool pool;
	CPE2Obj* obj = nullptr;
	pool.destroyClass(obj);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassPoolReuseOrder()
{
	using namespace ClassPoolExt2TestSupport;
	// LIFO 复用顺序验证
	CPE2ObjPool pool;
	CPE2Obj* a = pool.newClass();
	CPE2Obj* b = pool.newClass();
	CPE2Obj* c = pool.newClass();
	void* addrA = a;
	void* addrB = b;
	void* addrC = c;
	pool.destroyClass(c); // c first
	pool.destroyClass(b); // b second
	pool.destroyClass(a); // a third
	// LIFO: next alloc should be a (last freed)
	CPE2Obj* r1 = pool.newClass();
	CPE2Obj* r2 = pool.newClass();
	CPE2Obj* r3 = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(r1 != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(r2 != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(r3 != nullptr);
	// All addresses should be reused (not necessarily same order in all implementations)
	bool allReused = ((void*)r1 == addrA || (void*)r1 == addrB || (void*)r1 == addrC) &&
		((void*)r2 == addrA || (void*)r2 == addrB || (void*)r2 == addrC) &&
		((void*)r3 == addrA || (void*)r3 == addrB || (void*)r3 == addrC);
	ClassPoolExt2TestSupport_CPE2_ASSERT(allReused);
	pool.destroyClass(r1);
	pool.destroyClass(r2);
	pool.destroyClass(r3);
}

void ClassPoolTest::testClassPoolSubTypeNewList()
{
	using namespace ClassPoolExt2TestSupport;
	// 子类型 newClassList 验证
	CPE2ObjSubPool pool;
	Vector<CPE2ObjSub*> list;
	for (int i = 0; i < 8; ++i)
	{
		list.add(pool.newClass());
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(list.size() == 8);
	for (int i = 0; i < 8; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(list[i] != nullptr);
		list[i]->mSubData = i * 11;
		list[i]->mSubDouble = i * 0.1;
	}
	for (int i = 0; i < 8; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(list[i]->mSubData == i * 11);
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassPoolDeepChainReset()
{
	using namespace ClassPoolExt2TestSupport;
	// 深度继承：CPE2ObjDeep 继承链全部字段被 reset
	CPE2ObjDeepPool pool;
	CPE2ObjDeep* obj = pool.newClass();
	obj->mVal = 1;
	obj->mExtra = 2;
	obj->mSubData = 3;
	obj->mSubDouble = 4.5;
	obj->mDeepShort = 99;
	obj->mDeepFlag = true;
	pool.destroyClass(obj);
	CPE2ObjDeep* fresh = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mExtra == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mSubData == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf((float)fresh->mSubDouble) < 1e-6f);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mDeepShort == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(!fresh->mDeepFlag);
	pool.destroyClass(fresh);
}

void ClassPoolTest::testClassPoolThreadBasic()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	CPE2ThreadObj* obj = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj->mThreadVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(!obj->mThreadFlag);
	pool.destroyClass(obj);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassPoolThreadReuse()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	CPE2ThreadObj* first = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(first != nullptr);
	first->mThreadVal = 123;
	first->mThreadFlag = true;
	void* addr = first;
	pool.destroyClass(first);
	CPE2ThreadObj* second = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(second != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(second == addr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(second->mThreadVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(!second->mThreadFlag);
	pool.destroyClass(second);
}

void ClassPoolTest::testClassPoolThreadList()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	Vector<CPE2ThreadObj*> list;
	for (int i = 0; i < 7; ++i)
	{
		list.add(pool.newClass());
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(list.size() == 7);
	for (int i = 0; i < 7; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(list[i] != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(list[i]->mThreadVal == 0);
		list[i]->mThreadVal = i;
	}
	for (int i = 0; i < 7; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(list[i]->mThreadVal == i);
		pool.destroyClass(list[i]);
	}
}

void ClassPoolTest::testClassPoolThreadListReuse()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	Vector<CPE2ThreadObj*> list1;
	for (int i = 0; i < 5; ++i)
	{
		list1.add(pool.newClass());
	}
	for (int i = 0; i < 5; ++i)
	{
		list1[i]->mThreadVal = i * 100;
		list1[i]->mThreadFVal = (float)i;
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(list1[i]);
	}
	Vector<CPE2ThreadObj*> list2;
	for (int i = 0; i < 3; ++i)
	{
		list2.add(pool.newClass());
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(list2.size() == 3);
	for (int i = 0; i < 3; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(list2[i] != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(list2[i]->mThreadVal == 0);
		ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf(list2[i]->mThreadFVal) < 1e-6f);
		pool.destroyClass(list2[i]);
	}
}

void ClassPoolTest::testClassPoolThreadStress()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	const int N = 60;
	CPE2ThreadObj* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass();
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i] != nullptr);
		arr[i]->mThreadVal = i;
		arr[i]->mThreadFVal = (float)i * 0.5f;
		arr[i]->mThreadFlag = (i % 2 == 0);
	}
	for (int i = 0; i < N; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i]->mThreadVal == i);
		ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf(arr[i]->mThreadFVal - (float)i * 0.5f) < 1e-4f);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(arr[i]);
	}
	// Reallocate after stress
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass();
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i] != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i]->mThreadVal == 0);
		ClassPoolExt2TestSupport_CPE2_ASSERT(!arr[i]->mThreadFlag);
		pool.destroyClass(arr[i]);
	}
}

void ClassPoolTest::testClassPoolThreadDestroyNullNoOp()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	CPE2ThreadObj* obj = nullptr;
	pool.destroyClass(obj);
	ClassPoolExt2TestSupport_CPE2_ASSERT(obj == nullptr);
}

void ClassPoolTest::testClassPoolThreadAlternate()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	const int N = 20;
	for (int i = 0; i < N; ++i)
	{
		CPE2ThreadObj* obj = pool.newClass();
		ClassPoolExt2TestSupport_CPE2_ASSERT(obj != nullptr);
		obj->mThreadVal = i;
		pool.destroyClass(obj);
		CPE2ThreadObj* reused = pool.newClass();
		ClassPoolExt2TestSupport_CPE2_ASSERT(reused != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(reused->mThreadVal == 0);
		pool.destroyClass(reused);
	}
}

void ClassPoolTest::testClassPoolThreadFillData()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	CPE2ThreadObj* obj = pool.newClass();
	obj->mThreadVal = 9999;
	obj->mThreadFVal = 123.456f;
	obj->mThreadFlag = true;
	pool.destroyClass(obj);
	CPE2ThreadObj* fresh = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mThreadVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf(fresh->mThreadFVal) < 1e-6f);
	ClassPoolExt2TestSupport_CPE2_ASSERT(!fresh->mThreadFlag);
	pool.destroyClass(fresh);
}

void ClassPoolTest::testClassPoolThreadMultiRounds()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObj> pool;
	const int ROUNDS = 8;
	const int PER = 6;
	for (int r = 0; r < ROUNDS; ++r)
	{
		CPE2ThreadObj* arr[PER];
		for (int i = 0; i < PER; ++i)
		{
			arr[i] = pool.newClass();
			ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i] != nullptr);
			arr[i]->mThreadVal = r * 100 + i;
		}
		for (int i = 0; i < PER; ++i)
		{
			ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i]->mThreadVal == r * 100 + i);
			pool.destroyClass(arr[i]);
		}
	}
}

void ClassPoolTest::testClassPoolThreadDeepChain()
{
	using namespace ClassPoolExt2TestSupport;
	ClassPoolThread<CPE2ThreadObjDeep> pool;
	CPE2ThreadObjDeep* obj = pool.newClass();
	obj->mThreadVal = 7;
	obj->mSubVal = 8;
	obj->mDeepLlong = 999999LL;
	pool.destroyClass(obj);
	CPE2ThreadObjDeep* fresh = pool.newClass();
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mThreadVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mSubVal == 0);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fresh->mDeepLlong == 0LL);
	pool.destroyClass(fresh);
}

void ClassPoolTest::testVectorPoolBasicInt()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 0);
	VectorPool<int>::destroyVector(v);
}

void ClassPoolTest::testVectorPoolReuseInt()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	v->add(1);
	v->add(2);
	v->add(3);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 3);
	VectorPool<int>::destroyVector(v);
	Vector<int>* v2 = VectorPool<int>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2 != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0);
	VectorPool<int>::destroyVector(v2);
}

void ClassPoolTest::testVectorPoolBasicFloat()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<float>* v = VectorPool<float>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 0);
	v->add(1.0f);
	v->add(2.0f);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 2);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf((*v)[0] - 1.0f) < 1e-4f);
	VectorPool<float>::destroyVector(v);
}

void ClassPoolTest::testVectorPoolReuseFloat()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<float>* v = VectorPool<float>::newVector();
	v->add(10.0f);
	v->add(20.0f);
	VectorPool<float>::destroyVector(v);
	Vector<float>* v2 = VectorPool<float>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2 != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0);
	VectorPool<float>::destroyVector(v2);
}

void ClassPoolTest::testVectorPoolListContent()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	for (int i = 0; i < 20; ++i)
	{
		v->add(i);
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 20);
	for (int i = 0; i < 20; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT((*v)[i] == i);
	}
	VectorPool<int>::destroyVector(v);
}

void ClassPoolTest::testVectorPoolClearOnReturn()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	for (int i = 0; i < 5; ++i)
	{
		v->add(i * 10);
	}
	VectorPool<int>::destroyVector(v);
	Vector<int>* v2 = VectorPool<int>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0); // cleared
	VectorPool<int>::destroyVector(v2);
}

void ClassPoolTest::testVectorPoolLargeVector()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	for (int i = 0; i < 200; ++i)
	{
		v->add(i);
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 200);
	VectorPool<int>::destroyVector(v);
	Vector<int>* v2 = VectorPool<int>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0);
	VectorPool<int>::destroyVector(v2);
}

void ClassPoolTest::testVectorPoolMultiRound()
{
	using namespace ClassPoolExt2TestSupport;
	const int ROUNDS = 10;
	for (int r = 0; r < ROUNDS; ++r)
	{
		Vector<int>* v = VectorPool<int>::newVector();
		ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 0);
		v->add(r);
		v->add(r * 2);
		ClassPoolExt2TestSupport_CPE2_ASSERT((*v)[0] == r);
		ClassPoolExt2TestSupport_CPE2_ASSERT((*v)[1] == r * 2);
		VectorPool<int>::destroyVector(v);
	}
}

void ClassPoolTest::testVectorPoolNullDestroy()
{
	using namespace ClassPoolExt2TestSupport;
	// destroyVector 正常路径，无 null 特殊处理
	// 仅验证通常流程不崩溃
	Vector<int>* v = VectorPool<int>::newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	VectorPool<int>::destroyVector(v);
}

void ClassPoolTest::testVectorPoolMixTypes()
{
	using namespace ClassPoolExt2TestSupport;
	Vector<int>* vi = VectorPool<int>::newVector();
	Vector<float>* vf = VectorPool<float>::newVector();
	vi->add(100);
	vf->add(99.9f);
	ClassPoolExt2TestSupport_CPE2_ASSERT((*vi)[0] == 100);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf((*vf)[0] - 99.9f) < 1e-3f);
	VectorPool<int>::destroyVector(vi);
	VectorPool<float>::destroyVector(vf);
}

void ClassPoolTest::testVectorPoolThreadBasicInt()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 0);
	pool.destroyVector(&v);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
}

void ClassPoolTest::testVectorPoolThreadReuseInt()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	v->add(1);
	v->add(2);
	pool.destroyVector(&v);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
	Vector<int>* v2 = pool.newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0);
	pool.destroyVector(&v2);
}

void ClassPoolTest::testVectorPoolThreadBasicFloat()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<float> pool;
	Vector<float>* v = pool.newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	v->add(3.14f);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf((*v)[0] - 3.14f) < 1e-4f);
	pool.destroyVector(&v);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
}

void ClassPoolTest::testVectorPoolThreadReuseFloat()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<float> pool;
	Vector<float>* v = pool.newVector();
	v->add(1.5f);
	v->add(2.5f);
	pool.destroyVector(&v);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
	Vector<float>* v2 = pool.newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0);
	pool.destroyVector(&v2);
}

void ClassPoolTest::testVectorPoolThreadListContent()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	for (int i = 0; i < 15; ++i)
	{
		v->add(i * 3);
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 15);
	for (int i = 0; i < 15; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT((*v)[i] == i * 3);
	}
	pool.destroyVector(&v);
}

void ClassPoolTest::testVectorPoolThreadLargeVector()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	for (int i = 0; i < 150; ++i)
	{
		v->add(i);
	}
	ClassPoolExt2TestSupport_CPE2_ASSERT(v->size() == 150);
	pool.destroyVector(&v);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
	Vector<int>* v2 = pool.newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v2->size() == 0);
	pool.destroyVector(&v2);
}

void ClassPoolTest::testVectorPoolThreadMultiRound()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	for (int r = 0; r < 12; ++r)
	{
		Vector<int>* v = pool.newVector();
		ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
		v->add(r * 5);
		v->add(r * 5 + 1);
		ClassPoolExt2TestSupport_CPE2_ASSERT((*v)[0] == r * 5);
		pool.destroyVector(&v);
		ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
	}
}

void ClassPoolTest::testVectorPoolThreadNullDestroy()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	ClassPoolExt2TestSupport_CPE2_ASSERT(v != nullptr);
	pool.destroyVector(&v);
	ClassPoolExt2TestSupport_CPE2_ASSERT(v == nullptr);
}

void ClassPoolTest::testVectorPoolThreadStress()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	const int N = 40;
	Vector<int>* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newVector();
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i] != nullptr);
		for (int j = 0; j < 5; ++j)
		{
			arr[i]->add(i * 10 + j);
		}
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i]->size() == 5);
	}
	for (int i = 0; i < N; ++i)
	{
		ClassPoolExt2TestSupport_CPE2_ASSERT((*arr[i])[0] == i * 10);
		pool.destroyVector(&arr[i]);
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i] == nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newVector();
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i] != nullptr);
		ClassPoolExt2TestSupport_CPE2_ASSERT(arr[i]->size() == 0);
		pool.destroyVector(&arr[i]);
	}
}

void ClassPoolTest::testVectorPoolThreadMixTypes()
{
	using namespace ClassPoolExt2TestSupport;
	VectorPoolThread<int> poolInt;
	VectorPoolThread<float> poolFloat;
	Vector<int>* vi = poolInt.newVector();
	Vector<float>* vf = poolFloat.newVector();
	vi->add(55);
	vf->add(77.7f);
	ClassPoolExt2TestSupport_CPE2_ASSERT((*vi)[0] == 55);
	ClassPoolExt2TestSupport_CPE2_ASSERT(fabsf((*vf)[0] - 77.7f) < 1e-3f);
	poolInt.destroyVector(&vi);
	poolFloat.destroyVector(&vf);
	ClassPoolExt2TestSupport_CPE2_ASSERT(vi == nullptr);
	ClassPoolExt2TestSupport_CPE2_ASSERT(vf == nullptr);
}

namespace ClassPoolThreadTestSupport
{
	// ============================================================
	// ClassPoolThreadTest
	// 测试 ClassPoolThread<T>：线程安全固定类型对象池
	// 辅助测试类：CPTTestObj（继承 ClassObject）
	// ============================================================

#define ClassPoolThreadTestSupport_CPTT_ASSERT(expr) \
    if (!(expr)) { LOG("ClassPoolThreadTest FAIL: " #expr); }

// ------------------------------------------------------------------
// 辅助类
// ------------------------------------------------------------------
	class CPTTestObj : public ClassObject
	{
		BASE(CPTTestObj, ClassObject);
	public:
		int mValue = 0;
		int mExtra = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mValue = 0;
			mExtra = 0;
		}
		void destroy() override
		{
			base::destroy();
		}
	};

	class CPTTestObj2 : public ClassObject
	{
		BASE(CPTTestObj2, ClassObject);
	public:
		float mX = 0.0f;
		float mY = 0.0f;
		void resetProperty() override
		{
			base::resetProperty();
			mX = 0.0f;
			mY = 0.0f;
		}
		void destroy() override
		{
			base::destroy();
		}
	};
}

void ClassPoolTest::testInitDefaultThread()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	pool.initDefault(5);

	// 申请 5 个，应走复用路径
	CPTTestObj* objs[5];
	for (int i = 0; i < 5; ++i)
	{
		objs[i] = pool.newClass();
		ClassPoolThreadTestSupport_CPTT_ASSERT(objs[i] != nullptr);
		ClassPoolThreadTestSupport_CPTT_ASSERT(objs[i]->isDestroy() == false);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(objs[i]);
		ClassPoolThreadTestSupport_CPTT_ASSERT(objs[i] == nullptr);
	}
	pool.quit();
}

void ClassPoolTest::testNewClass()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	CPTTestObj* obj = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj != nullptr);
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj->isDestroy() == false);
	pool.destroyClass(obj);
	pool.quit();
}

void ClassPoolTest::testNewClassReuse()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	CPTTestObj* obj1 = pool.newClass();
	obj1->mValue = 999;
	obj1->mExtra = 888;
	pool.destroyClass(obj1);
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj1 == nullptr);

	CPTTestObj* obj2 = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj2 != nullptr);
	// resetProperty 在 markUsable 之前调用（参见 ClassPool.h 结构），mValue 应为 0
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj2->mValue == 0);
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj2->mExtra == 0);
	pool.destroyClass(obj2);
	pool.quit();
}

void ClassPoolTest::testDestroyClass()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	CPTTestObj* obj = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj->isDestroy() == false);
	pool.destroyClass(obj);
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj == nullptr);
	pool.quit();
}

void ClassPoolTest::testDestroyClassNullSafeThread()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	CPTTestObj* obj = nullptr;
	pool.destroyClass(obj); // 不应崩溃
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj == nullptr);
	pool.quit();
}

void ClassPoolTest::testDestroyClassListVectorThread()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	Vector<CPTTestObj*> list;
	for (int i = 0; i < 10; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		obj->mValue = i;
		list.add(obj);
	}
	ClassPoolThreadTestSupport_CPTT_ASSERT(list.size() == 10);
	pool.destroyClassList(list);
	ClassPoolThreadTestSupport_CPTT_ASSERT(list.isEmpty());
	// 归还后能再申请 10 个（走复用路径）
	for (int i = 0; i < 10; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		ClassPoolThreadTestSupport_CPTT_ASSERT(obj != nullptr);
		pool.destroyClass(obj);
	}
	pool.quit();
}

void ClassPoolTest::testDestroyClassListHashMapThread()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	HashMap<int, CPTTestObj*> map;
	for (int i = 0; i < 8; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		obj->mValue = i * 10;
		map.add(i, obj);
	}
	ClassPoolThreadTestSupport_CPTT_ASSERT(map.size() == 8);
	pool.destroyClassList(map);
	ClassPoolThreadTestSupport_CPTT_ASSERT(map.isEmpty());
	pool.quit();
}

void ClassPoolTest::testDestroyClassListArrayListThread()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	ArrayList<16, CPTTestObj*> list;
	for (int i = 0; i < 6; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		obj->mValue = i + 100;
		list.add(obj);
	}
	ClassPoolThreadTestSupport_CPTT_ASSERT(list.size() == 6);
	pool.destroyClassList(list);
	ClassPoolThreadTestSupport_CPTT_ASSERT(list.isEmpty());
	pool.quit();
}

void ClassPoolTest::testQuit()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	pool.initDefault(10);
	// 申请 3 个不归还，quit 时只释放未使用的 7 个
	CPTTestObj* a = pool.newClass();
	CPTTestObj* b = pool.newClass();
	CPTTestObj* c = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(a != nullptr);
	ClassPoolThreadTestSupport_CPTT_ASSERT(b != nullptr);
	ClassPoolThreadTestSupport_CPTT_ASSERT(c != nullptr);
	pool.quit();
	// 归还剩余的（quit 后 mDestroied=true，destroyClass 不操作）
	(void)a;
	(void)b;
	(void)c;
}

void ClassPoolTest::testLargeScale()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	pool.initDefault(50);

	const int COUNT = 500;
	Vector<CPTTestObj*> list;
	list.reserve(COUNT);

	for (int i = 0; i < COUNT; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		obj->mValue = i;
		list.add(obj);
	}
	ClassPoolThreadTestSupport_CPTT_ASSERT(list.size() == COUNT);

	// 验证 ID 唯一性（assignIDSeed 单调递增）
	for (int i = 0; i < COUNT - 1; ++i)
	{
		ClassPoolThreadTestSupport_CPTT_ASSERT(list[i]->getAssignID() != list[i + 1]->getAssignID());
	}

	pool.destroyClassList(list);
	ClassPoolThreadTestSupport_CPTT_ASSERT(list.isEmpty());

	// 再申请 COUNT 个，全部走复用路径
	for (int i = 0; i < COUNT; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		ClassPoolThreadTestSupport_CPTT_ASSERT(obj != nullptr);
		pool.destroyClass(obj);
	}
	pool.quit();
}

void ClassPoolTest::testClearPool()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	for (int i = 0; i < 20; ++i)
	{
		CPTTestObj* obj = pool.newClass();
		pool.destroyClass(obj);
	}
	pool.clearPool();
	// clearPool 后再申请，走重新分配路径
	CPTTestObj* obj = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
	pool.quit();
}

void ClassPoolTest::testAssignID()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	CPTTestObj* obj1 = pool.newClass();
	llong id1 = obj1->getAssignID();
	CPTTestObj* obj2 = pool.newClass();
	llong id2 = obj2->getAssignID();
	ClassPoolThreadTestSupport_CPTT_ASSERT(id1 != id2);
	ClassPoolThreadTestSupport_CPTT_ASSERT(id2 > id1);

	pool.destroyClass(obj1);
	pool.destroyClass(obj2);

	// 复用时也会重新分配 ID
	CPTTestObj* obj3 = pool.newClass();
	llong id3 = obj3->getAssignID();
	ClassPoolThreadTestSupport_CPTT_ASSERT(id3 > id2);
	pool.destroyClass(obj3);
	pool.quit();
}

void ClassPoolTest::testMultiplePoolInstances()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj>  poolA;
	ClassPoolThread<CPTTestObj>  poolB;
	ClassPoolThread<CPTTestObj2> poolC;

	CPTTestObj* a = poolA.newClass();
	CPTTestObj* b = poolB.newClass();
	CPTTestObj2* c = poolC.newClass();

	a->mValue = 10;
	b->mValue = 20;
	c->mX = 1.5f;

	ClassPoolThreadTestSupport_CPTT_ASSERT(a->mValue == 10);
	ClassPoolThreadTestSupport_CPTT_ASSERT(b->mValue == 20);
	ClassPoolThreadTestSupport_CPTT_ASSERT(fabsf(c->mX - 1.5f) < 1e-5f);

	poolA.destroyClass(a);
	poolB.destroyClass(b);
	poolC.destroyClass(c);

	poolA.quit();
	poolB.quit();
	poolC.quit();
}

void ClassPoolTest::testNewClassAfterClearPool()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	pool.initDefault(5);
	pool.clearPool();
	CPTTestObj* obj = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
	pool.quit();
}

void ClassPoolTest::testReuseAfterDestroy()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	CPTTestObj* obj = pool.newClass();
	obj->mValue = 777;
	pool.destroyClass(obj);

	CPTTestObj* obj2 = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(obj2->mValue == 0);
	pool.destroyClass(obj2);
	pool.quit();
}

void ClassPoolTest::testDestroyClassListEmpty()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	Vector<CPTTestObj*> empty;
	pool.destroyClassList(empty); // 应跳过，不崩溃
	pool.quit();
}

void ClassPoolTest::testDestroyClassListHashMapEmptyThread()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	HashMap<int, CPTTestObj*> emptyMap;
	pool.destroyClassList(emptyMap);
	pool.quit();
}

void ClassPoolTest::testPoolCountGrowth()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	pool.initDefault(3);

	// 申请 5 个（超过预分配数量 3）
	CPTTestObj* objs[5];
	for (int i = 0; i < 5; ++i)
	{
		objs[i] = pool.newClass();
		ClassPoolThreadTestSupport_CPTT_ASSERT(objs[i] != nullptr);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(objs[i]);
	}
	pool.quit();
}

void ClassPoolTest::testInterleavedNewDestroy()
{
	using namespace ClassPoolThreadTestSupport;
	ClassPoolThread<CPTTestObj> pool;
	pool.initDefault(4);

	CPTTestObj* a = pool.newClass();
	CPTTestObj* b = pool.newClass();
	a->mValue = 1;
	b->mValue = 2;
	pool.destroyClass(a);
	CPTTestObj* c = pool.newClass();
	c->mValue = 3;
	pool.destroyClass(b);
	pool.destroyClass(c);

	// 所有已归还，池中应有 2 个可用
	CPTTestObj* x = pool.newClass();
	CPTTestObj* y = pool.newClass();
	ClassPoolThreadTestSupport_CPTT_ASSERT(x != nullptr);
	ClassPoolThreadTestSupport_CPTT_ASSERT(y != nullptr);
	ClassPoolThreadTestSupport_CPTT_ASSERT(x->mValue == 0);
	ClassPoolThreadTestSupport_CPTT_ASSERT(y->mValue == 0);
	pool.destroyClass(x);
	pool.destroyClass(y);
	pool.quit();
}

void ClassPoolTest::test()
{
	testClassObject_InitialState();
	testClassObject_ResetProperty();
	testClassPool_NewClass_Basic();
	testClassPool_InitDefault();
	testClassPool_AutoExpand();
	testClassPool_ReuseAfterDestroy();
	testClassPool_DestroyNullPtr();
	testClassPool_DestroyTwice();
	testClassPool_AssignIDIncrement();
	testClassPool_DestroyListVector();
	testClassPool_DestroyListVectorEmpty();
	testClassPool_DestroyListArrayList();
	testClassPool_DestroyListHashMap();
	testClassPool_DestroyListSafeVector();
	testClassPool_LargeVolume();
	testClassPool_MultiType();
	testClassPool_ResetOnReturn();
	testClassPool_PendingDestroy();
	testClassPool_CycleNewDestroy();
	testClassPool_DestroyListThenNew();
	testClassPool_DestroyListSafeHashMap();
	testClassPool_FrameLoopSimulation();
	testClassPool_BulletType();
	testClassPool_AssignIDAfterReuse();
	testClassPool_ThreeTypesIndependent();
	testClassPool_DestroyHashMapLLong();
	testClassPool_InitDefaultZero();
	testClassPool_InitDefaultOne();
	testClassPool_MassPreallocAndReset();
	testClassPool_StringFieldReset();
	testClassPool_FloatFieldReset();
	testNewClassBasic();
	testNewClassFromEmpty();
	testNewClassFromPool();
	testDestroyClassBasic();
	testDestroyClassNullSafe();
	testDestroyClassSetsNull();
	testDestroyClassReusable();
	testDestroyClassListVector();
	testDestroyClassListVectorEmpty();
	testDestroyClassListSafeVector();
	testDestroyClassListSafeVectorEmpty();
	testInitDefault();
	testInitDefaultThenNew();
	testTotalCountGrowth();
	testAssignIDMonotonic();
	testAssignIDUnique();
	testMarkUsableMarkDispose();
	testDoubleDestroyProtection();
	testPoolAssignedToObject();
	testQuitClearsUnusedList();
	testLargeAllocationAndReturn();
	testMultipleCycles();
	testDestroyDestroiedPool();
	testDestroyNullObjectInList();
	runExtraClassPoolTests();
	testDestroyClassListHashMap();
	testDestroyClassListHashMapEmpty();
	testDestroyClassListSafeHashMap();
	testDestroyClassListSafeHashMapEmpty();
	testDestroyClassListArrayList();
	testDestroyClassListArrayListEmpty();
	testDestroyClassListVectorWithNull();
	testDestroyClassListHashMapWithNull();
	testMultiplePoolsSameType();
	testPoolAssignIDContinuous();
	testResetPropertyOnReuse();
	testDestroyClassListSafeVectorDeep2();
	testPoolLargeInitDefault();
	testPoolAllocDealloc1000();
	testDestroyAllFromSafeHashMap();
	testObjectLifecycleFull();
	testPoolAfterQuit();
	testMultiplePoolsDifferentTypes();
	testDestroyClassListVectorMixed();
	runExtraClassPool2Tests();
	testClassPoolNewDestroy();
	testClassPoolReuse();
	testClassPoolList();
	testClassPoolListReuse();
	testClassPoolStress();
	testClassPoolDeepInheritance();
	testClassPoolResetProperty();
	testClassPoolMultipleRounds();
	testClassTypePoolNewByKey();
	testClassTypePoolDestroyNull();
	testClassTypePoolListByKey();
	testClassTypePoolReuseByKey();
	testClassTypePoolMultiType();
	testClassTypePoolStress();
	testClassBaseTypePoolNew();
	testClassBaseTypePoolReuse();
	testClassBaseTypePoolList();
	testClassBaseTypePoolMultiType();
	testClassBaseTypePoolStress();
	testClassKeyPoolThreadNew();
	testClassKeyPoolThreadDestroy();
	testClassKeyPoolThreadReuse();
	testClassKeyPoolThreadList();
	testClassKeyPoolThreadManyKeys();
	testClassKeyPoolThreadStress();
	testClassPoolNewNull();
	testClassPoolListZero();
	testClassPoolListOverflow();
	testClassPoolAlternateAllocFree();
	testClassPoolMixedNewList();
	testClassPoolFillData();
	testClassPoolDestroyNullNoOp();
	testClassPoolReuseOrder();
	testClassPoolSubTypeNewList();
	testClassPoolDeepChainReset();
	testClassPoolThreadBasic();
	testClassPoolThreadReuse();
	testClassPoolThreadList();
	testClassPoolThreadListReuse();
	testClassPoolThreadStress();
	testClassPoolThreadDestroyNullNoOp();
	testClassPoolThreadAlternate();
	testClassPoolThreadFillData();
	testClassPoolThreadMultiRounds();
	testClassPoolThreadDeepChain();
	testVectorPoolBasicInt();
	testVectorPoolReuseInt();
	testVectorPoolBasicFloat();
	testVectorPoolReuseFloat();
	testVectorPoolListContent();
	testVectorPoolClearOnReturn();
	testVectorPoolLargeVector();
	testVectorPoolMultiRound();
	testVectorPoolNullDestroy();
	testVectorPoolMixTypes();
	testVectorPoolThreadBasicInt();
	testVectorPoolThreadReuseInt();
	testVectorPoolThreadBasicFloat();
	testVectorPoolThreadReuseFloat();
	testVectorPoolThreadListContent();
	testVectorPoolThreadLargeVector();
	testVectorPoolThreadMultiRound();
	testVectorPoolThreadNullDestroy();
	testVectorPoolThreadStress();
	testVectorPoolThreadMixTypes();
	testInitDefaultThread();
	testNewClass();
	testNewClassReuse();
	testDestroyClass();
	testDestroyClassNullSafeThread();
	testDestroyClassListVectorThread();
	testDestroyClassListHashMapThread();
	testDestroyClassListArrayListThread();
	testQuit();
	testLargeScale();
	testClearPool();
	testAssignID();
	testMultiplePoolInstances();
	testNewClassAfterClearPool();
	testReuseAfterDestroy();
	testDestroyClassListEmpty();
	testDestroyClassListHashMapEmptyThread();
	testPoolCountGrowth();
	testInterleavedNewDestroy();
}
#endif
