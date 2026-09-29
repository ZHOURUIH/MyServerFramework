#ifdef WINDOWS
#include "FrameHeader.h"
namespace ClassBaseTypePoolTestSupport
{
#define ClassBaseTypePoolTestSupport_CBTP_ASSERT(expression) if (!(expression)) { ERROR(string("ClassBaseTypePoolTest FAILED: ") + #expression); }

	// ============================================================
	// ClassBaseTypePool 测试辅助类
	// ClassBaseTypePool<BaseClassType> 基于 typeid 哈希区分子类型
	// 无需实现 create，直接 new T()
	// 注意：已有测试 ClassPoolTest.cpp 测试了 ClassPool<T>
	// 本文件专门测试 ClassBaseTypePool<BaseType>
	// ============================================================

	// 基类
	class BaseUnit : public ClassObject
	{
	public:
		int hp = 0;
		string name;
		bool alive = false;

		virtual void resetProperty() override
		{
			ClassObject::resetProperty();
			hp = 0;
			name.clear();
			alive = false;
		}
		virtual void destroy() override
		{
			hp = 0;
			name.clear();
			alive = false;
		}
		virtual string getUnitType() const { return "BaseUnit"; }
	};

	// 子类 A
	class WarriorUnit : public BaseUnit
	{
	public:
		int attackPower = 0;
		virtual void resetProperty() override
		{
			BaseUnit::resetProperty();
			attackPower = 0;
			alive = true;
		}
		virtual void destroy() override
		{
			BaseUnit::destroy();
			attackPower = 0;
		}
		virtual string getUnitType() const override { return "WarriorUnit"; }
	};

	// 子类 B
	class MageUnit : public BaseUnit
	{
	public:
		float spellPower = 0.0f;
		virtual void resetProperty() override
		{
			BaseUnit::resetProperty();
			spellPower = 0.0f;
			alive = true;
		}
		virtual void destroy() override
		{
			BaseUnit::destroy();
			spellPower = 0.0f;
		}
		virtual string getUnitType() const override { return "MageUnit"; }
	};

	// 子类 C
	class ArcherUnit : public BaseUnit
	{
	public:
		int arrowCount = 0;
		virtual void resetProperty() override
		{
			BaseUnit::resetProperty();
			arrowCount = 30;
			alive = true;
		}
		virtual void destroy() override
		{
			BaseUnit::destroy();
			arrowCount = 0;
		}
		virtual string getUnitType() const override { return "ArcherUnit"; }
	};
}

void ClassBaseTypePoolTest::testInitDefault()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;
	// 预先初始化 5 个 WarriorUnit
	pool.initDefault<WarriorUnit>(5);

	// 取出 5 个，都应该来自池中
	Vector<BaseUnit*> list;
	for (int i = 0; i < 5; ++i)
	{
		BaseUnit* u = pool.newClass<WarriorUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(u != nullptr);
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(u->getUnitType() == "WarriorUnit");
		list.add(u);
	}

	// 归还
	for (int i = 0; i < (int)list.size(); ++i)
	{
		pool.destroyClass(list[i]);
	}
	list.clear();

	pool.quit();
}

void ClassBaseTypePoolTest::testNewClass()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	// 直接新建（无预填充）
	WarriorUnit* w = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w->getUnitType() == "WarriorUnit");
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w->alive == true);

	MageUnit* m = pool.newClass<MageUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m->getUnitType() == "MageUnit");
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m->alive == true);

	ArcherUnit* a = pool.newClass<ArcherUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a->getUnitType() == "ArcherUnit");
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a->arrowCount == 30);

	// 归还
	pool.destroyClass(w);
	pool.destroyClass(m);
	pool.destroyClass(a);

	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w == nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m == nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a == nullptr);

	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassReuseFromPool()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	// 新建并归还
	WarriorUnit* w1 = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w1 != nullptr);
	void* addr1 = (void*)w1;
	pool.destroyClass(w1);

	// 再次获取，应该复用同一个地址
	WarriorUnit* w2 = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2 != nullptr);
	void* addr2 = (void*)w2;
	// 地址应相同（复用了）
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(addr1 == addr2);
	// resetProperty 应该已被调用
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2->alive == true);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2->hp == 0);

	pool.destroyClass(w2);
	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassList()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	Vector<BaseUnit*> classList;
	pool.newClassList<WarriorUnit>(classList, 10);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT((int)classList.size() == 10);
	for (int i = 0; i < (int)classList.size(); ++i)
	{
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(classList[i] != nullptr);
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(classList[i]->getUnitType() == "WarriorUnit");
	}

	// 归还所有
	pool.destroyClassList(classList);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(classList.isEmpty());

	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassListReuseFromPool()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;
	pool.initDefault<MageUnit>(5);

	Vector<BaseUnit*> classList;
	pool.newClassList<MageUnit>(classList, 8);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT((int)classList.size() == 8);
	for (int i = 0; i < (int)classList.size(); ++i)
	{
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(classList[i] != nullptr);
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(classList[i]->getUnitType() == "MageUnit");
	}

	pool.destroyClassList(classList);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(classList.isEmpty());

	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClass()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	WarriorUnit* w = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
	w->hp = 500;
	w->name = "Knight";

	pool.destroyClass(w);
	// w 应被置为 nullptr
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w == nullptr);

	// 下次取出时应该已经 resetProperty
	WarriorUnit* w2 = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2 != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2->hp == 0);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2->name.empty());

	pool.destroyClass(w2);
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassNullSafe()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	// 销毁 nullptr 不应崩溃
	WarriorUnit* w = nullptr;
	pool.destroyClass(w);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w == nullptr);

	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassListVector()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	Vector<WarriorUnit*> warriors;
	for (int i = 0; i < 8; ++i)
	{
		WarriorUnit* w = pool.newClass<WarriorUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
		w->hp = i * 10;
		warriors.add(w);
	}
	ClassBaseTypePoolTestSupport_CBTP_ASSERT((int)warriors.size() == 8);

	pool.destroyClassList(warriors);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(warriors.isEmpty());

	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassListHashMap()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	HashMap<int, MageUnit*> mageMap;
	for (int i = 0; i < 5; ++i)
	{
		MageUnit* m = pool.newClass<MageUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(m != nullptr);
		m->hp = i * 20;
		mageMap.add(i, m);
	}
	ClassBaseTypePoolTestSupport_CBTP_ASSERT((int)mageMap.size() == 5);

	pool.destroyClassList(mageMap);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(mageMap.isEmpty());

	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassListArrayList()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	ArrayList<32, ArcherUnit*> archers;
	for (int i = 0; i < 10; ++i)
	{
		ArcherUnit* a = pool.newClass<ArcherUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(a != nullptr);
		a->arrowCount = 10 + i;
		archers.add(a);
	}
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(archers.size() == 10);

	pool.destroyClassList(archers);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(archers.isEmpty());

	pool.quit();
}

void ClassBaseTypePoolTest::testMultipleTypes()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	// 同时管理三种子类
	WarriorUnit* w = pool.newClass<WarriorUnit>();
	MageUnit* m = pool.newClass<MageUnit>();
	ArcherUnit* a = pool.newClass<ArcherUnit>();

	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a != nullptr);

	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w->getUnitType() == "WarriorUnit");
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m->getUnitType() == "MageUnit");
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a->getUnitType() == "ArcherUnit");

	pool.destroyClass(w);
	pool.destroyClass(m);
	pool.destroyClass(a);

	// 再各取一个，顺序应该对
	WarriorUnit* w2 = pool.newClass<WarriorUnit>();
	MageUnit* m2 = pool.newClass<MageUnit>();
	ArcherUnit* a2 = pool.newClass<ArcherUnit>();

	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2 != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m2 != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(a2 != nullptr);

	pool.destroyClass(w2);
	pool.destroyClass(m2);
	pool.destroyClass(a2);

	pool.quit();
}

void ClassBaseTypePoolTest::testReuseByTypeHash()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	// 新建 3 个 WarriorUnit，归还
	Vector<WarriorUnit*> warriors;
	for (int i = 0; i < 3; ++i)
	{
		warriors.add(pool.newClass<WarriorUnit>());
	}
	pool.destroyClassList(warriors);

	// 新建 3 个 MageUnit，归还
	Vector<MageUnit*> mages;
	for (int i = 0; i < 3; ++i)
	{
		mages.add(pool.newClass<MageUnit>());
	}
	pool.destroyClassList(mages);

	// 再次取 WarriorUnit，不应取到 MageUnit
	WarriorUnit* w = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w->getUnitType() == "WarriorUnit");
	pool.destroyClass(w);

	// 再次取 MageUnit，不应取到 WarriorUnit
	MageUnit* m = pool.newClass<MageUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m != nullptr);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m->getUnitType() == "MageUnit");
	pool.destroyClass(m);

	pool.quit();
}

void ClassBaseTypePoolTest::testQuit()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	pool.initDefault<WarriorUnit>(5);
	pool.initDefault<MageUnit>(3);

	// quit 不应崩溃
	pool.quit();
}

void ClassBaseTypePoolTest::testLargeScale()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	const int count = 500;
	Vector<WarriorUnit*> warriors;
	for (int i = 0; i < count; ++i)
	{
		WarriorUnit* w = pool.newClass<WarriorUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
		w->hp = i;
		warriors.add(w);
	}
	ClassBaseTypePoolTestSupport_CBTP_ASSERT((int)warriors.size() == count);

	pool.destroyClassList(warriors);
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(warriors.isEmpty());

	// 再次取，全部来自池
	for (int i = 0; i < count; ++i)
	{
		WarriorUnit* w = pool.newClass<WarriorUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(w->alive == true);
		warriors.add(w);
	}
	ClassBaseTypePoolTestSupport_CBTP_ASSERT((int)warriors.size() == count);

	pool.destroyClassList(warriors);
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyThenNew()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	// 多次 destroy-then-new 循环
	for (int round = 0; round < 10; ++round)
	{
		WarriorUnit* w = pool.newClass<WarriorUnit>();
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(w != nullptr);
		w->hp = round * 100;
		w->attackPower = round * 5;

		pool.destroyClass(w);
		ClassBaseTypePoolTestSupport_CBTP_ASSERT(w == nullptr);
	}

	pool.quit();
}

void ClassBaseTypePoolTest::testMarkUsableAssignID()
{
	using namespace ClassBaseTypePoolTestSupport;
	ClassBaseTypePool<BaseUnit> pool;

	WarriorUnit* w1 = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w1 != nullptr);
	llong id1 = w1->getAssignID();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(id1 > 0);

	WarriorUnit* w2 = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w2 != nullptr);
	llong id2 = w2->getAssignID();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(id2 > id1);

	MageUnit* m = pool.newClass<MageUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(m != nullptr);
	llong idM = m->getAssignID();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(idM > id2);

	pool.destroyClass(w1);
	pool.destroyClass(w2);
	pool.destroyClass(m);

	// 复用后 assignID 应该重新赋值（更大）
	WarriorUnit* w3 = pool.newClass<WarriorUnit>();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(w3 != nullptr);
	llong id3 = w3->getAssignID();
	ClassBaseTypePoolTestSupport_CBTP_ASSERT(id3 > idM);

	pool.destroyClass(w3);
	pool.quit();
}

namespace ClassBaseTypePoolThreadTestSupport
{
	// ============================================================
	// ClassBaseTypePoolThreadTest
	// 测试 ClassBaseTypePoolThread<BaseType>：线程安全多态对象池
	// 辅助类层次：AnimalBase → DogObj, CatObj, BirdObj
	// ============================================================

#define ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(expr) \
    if (!(expr)) { LOG("ClassBaseTypePoolThreadTest FAIL: " #expr); }

// ------------------------------------------------------------------
// 辅助类
// ------------------------------------------------------------------
	class AnimalBase : public ClassObject
	{
		BASE(AnimalBase, ClassObject);
	public:
		int mHP = 0;
		int mID = 0;
		virtual int getType() const { return 0; }
		void resetProperty() override
		{
			base::resetProperty();
			mHP = 0;
			mID = 0;
		}
		void destroy() override
		{
			base::destroy();
		}
	};

	class DogObj : public AnimalBase
	{
		BASE(DogObj, AnimalBase);
	public:
		float mSpeed = 0.0f;
		int getType() const override { return 1; }
		void resetProperty() override
		{
			base::resetProperty();
			mSpeed = 0.0f;
		}
		void destroy() override
		{
			base::destroy();
		}
	};

	class CatObj : public AnimalBase
	{
		BASE(CatObj, AnimalBase);
	public:
		bool mStealth = false;
		int getType() const override { return 2; }
		void resetProperty() override
		{
			base::resetProperty();
			mStealth = false;
		}
		void destroy() override
		{
			base::destroy();
		}
	};

	class BirdObj : public AnimalBase
	{
		BASE(BirdObj, AnimalBase);
	public:
		float mFlyHeight = 0.0f;
		int getType() const override { return 3; }
		void resetProperty() override
		{
			base::resetProperty();
			mFlyHeight = 0.0f;
		}
		void destroy() override
		{
			base::destroy();
		}
	};

	// ------------------------------------------------------------------
	// 测试入口
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testInitDefault：预分配多个 DogObj，验证可立即复用
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testNewClass：动态创建各类型
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testNewClassReuse：归还后重取，resetProperty 已调用
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testNewClassList：批量申请
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testDestroyClass：基本销毁语义，置 nullptr
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testDestroyClassNullSafe：nullptr 不崩溃
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testDestroyClassListVector：批量销毁 Vector<T*>
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testDestroyClassListHashMap：批量销毁 HashMap<K, T*>
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testDestroyClassListArrayList：批量销毁 ArrayList<N, T*>
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testMultipleTypes：多个类型共享同一个 pool，各自独立 bucket
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testQuit：quit 清空未使用列表
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testClearPool：clearPool 删除所有缓存
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testAssignID：每次 newClass 得到递增 ID
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testLargeScale：大规模多类型申请 + 归还
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testMixedNewDestroy：交错混合类型申请和归还
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testNewClassListMultipleTypes：同一 pool 批量申请不同类型
	// ------------------------------------------------------------------

	// ------------------------------------------------------------------
	// testDestroyAndReuseByType：销毁 DogObj 后，复用时仍是 DogObj
	// ------------------------------------------------------------------
}

void ClassBaseTypePoolTest::testInitDefaultThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	pool.initDefault<DogObj>(5);
	pool.initDefault<CatObj>(3);

	// 申请 5 个 DogObj，前 5 个走复用
	DogObj* dogs[5];
	for (int i = 0; i < 5; ++i)
	{
		dogs[i] = pool.newClass<DogObj>();
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs[i] != nullptr);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs[i]->isDestroy() == false);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(dogs[i]);
	}
	// 申请 3 个 CatObj
	CatObj* cats[3];
	for (int i = 0; i < 3; ++i)
	{
		cats[i] = pool.newClass<CatObj>();
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats[i] != nullptr);
	}
	for (int i = 0; i < 3; ++i)
	{
		pool.destroyClass(cats[i]);
	}
	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	DogObj* dog = pool.newClass<DogObj>();
	CatObj* cat = pool.newClass<CatObj>();
	BirdObj* bird = pool.newClass<BirdObj>();

	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog != nullptr);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cat != nullptr);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(bird != nullptr);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog->isDestroy() == false);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cat->isDestroy() == false);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(bird->isDestroy() == false);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog->getType() == 1);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cat->getType() == 2);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(bird->getType() == 3);

	pool.destroyClass(dog);
	pool.destroyClass(cat);
	pool.destroyClass(bird);
	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassReuse()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	DogObj* dog = pool.newClass<DogObj>();
	dog->mSpeed = 5.5f;
	dog->mHP = 100;
	pool.destroyClass(dog);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog == nullptr);

	DogObj* dog2 = pool.newClass<DogObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog2 != nullptr);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(fabsf(dog2->mSpeed) < 1e-5f);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog2->mHP == 0);
	pool.destroyClass(dog2);
	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassListThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	Vector<DogObj*> dogList;
	pool.newClassList<DogObj>(dogList, 6);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogList.size() == 6);
	for (DogObj* d : dogList)
	{
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(d != nullptr);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(d->isDestroy() == false);
	}
	pool.destroyClassList(dogList);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogList.isEmpty());
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	CatObj* cat = pool.newClass<CatObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cat->isDestroy() == false);
	pool.destroyClass(cat);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cat == nullptr);
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassNullSafeThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	DogObj* dog = nullptr;
	pool.destroyClass(dog);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog == nullptr);
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassListVectorThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	Vector<CatObj*> cats;
	for (int i = 0; i < 8; ++i)
	{
		CatObj* c = pool.newClass<CatObj>();
		c->mHP = i * 5;
		cats.add(c);
	}
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats.size() == 8);
	pool.destroyClassList(cats);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats.isEmpty());
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassListHashMapThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	HashMap<int, DogObj*> map;
	for (int i = 0; i < 6; ++i)
	{
		DogObj* d = pool.newClass<DogObj>();
		d->mSpeed = (float)i;
		map.add(i, d);
	}
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(map.size() == 6);
	pool.destroyClassList(map);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(map.isEmpty());
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyClassListArrayListThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	ArrayList<10, BirdObj*> birds;
	for (int i = 0; i < 5; ++i)
	{
		BirdObj* b = pool.newClass<BirdObj>();
		b->mFlyHeight = (float)(i * 2);
		birds.add(b);
	}
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(birds.size() == 5);
	pool.destroyClassList(birds);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(birds.isEmpty());
	pool.quit();
}

void ClassBaseTypePoolTest::testMultipleTypesThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	pool.initDefault<DogObj>(3);
	pool.initDefault<CatObj>(3);
	pool.initDefault<BirdObj>(3);

	DogObj* dog = pool.newClass<DogObj>();
	CatObj* cat = pool.newClass<CatObj>();
	BirdObj* bird = pool.newClass<BirdObj>();

	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dog->getType() == 1);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cat->getType() == 2);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(bird->getType() == 3);

	pool.destroyClass(dog);
	pool.destroyClass(cat);
	pool.destroyClass(bird);
	pool.quit();
}

void ClassBaseTypePoolTest::testQuitThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	pool.initDefault<DogObj>(5);
	pool.quit();
	// quit 后再申请，走新建路径
	DogObj* d = pool.newClass<DogObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(d != nullptr);
	pool.destroyClass(d);
	pool.quit();
}

void ClassBaseTypePoolTest::testClearPool()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	for (int i = 0; i < 10; ++i)
	{
		DogObj* d = pool.newClass<DogObj>();
		pool.destroyClass(d);
	}
	pool.clearPool();
	// clearPool 后再申请正常
	DogObj* d = pool.newClass<DogObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(d != nullptr);
	pool.destroyClass(d);
	pool.quit();
}

void ClassBaseTypePoolTest::testAssignID()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	DogObj* a = pool.newClass<DogObj>();
	CatObj* b = pool.newClass<CatObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(a->getAssignID() != b->getAssignID());
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(b->getAssignID() > a->getAssignID());
	pool.destroyClass(a);
	const llong assignIDB = b->getAssignID();
	pool.destroyClass(b);
	DogObj* c = pool.newClass<DogObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(c->getAssignID() > assignIDB);
	pool.destroyClass(c);
	pool.quit();
}

void ClassBaseTypePoolTest::testLargeScaleThread()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	pool.initDefault<DogObj>(20);
	pool.initDefault<CatObj>(20);
	pool.initDefault<BirdObj>(20);

	Vector<DogObj*>  dogs;
	Vector<CatObj*>  cats;
	Vector<BirdObj*> birds;
	dogs.reserve(100);
	cats.reserve(100);
	birds.reserve(100);

	for (int i = 0; i < 100; ++i)
	{
		DogObj* d = pool.newClass<DogObj>();
		CatObj* c = pool.newClass<CatObj>();
		BirdObj* b = pool.newClass<BirdObj>();
		d->mHP = i;
		c->mHP = i + 1;
		b->mHP = i + 2;
		dogs.add(d);
		cats.add(c);
		birds.add(b);
	}

	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs.size() == 100);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats.size() == 100);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(birds.size() == 100);

	for (int i = 0; i < 100; ++i)
	{
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs[i]->mHP == i);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats[i]->mHP == i + 1);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(birds[i]->mHP == i + 2);
	}

	pool.destroyClassList(dogs);
	pool.destroyClassList(cats);
	pool.destroyClassList(birds);

	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs.isEmpty());
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats.isEmpty());
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(birds.isEmpty());

	pool.quit();
}

void ClassBaseTypePoolTest::testMixedNewDestroy()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	DogObj* d1 = pool.newClass<DogObj>();
	CatObj* c1 = pool.newClass<CatObj>();
	BirdObj* b1 = pool.newClass<BirdObj>();
	d1->mHP = 10;
	c1->mHP = 20;
	b1->mHP = 30;

	pool.destroyClass(c1);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(c1 == nullptr);

	CatObj* c2 = pool.newClass<CatObj>();
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(c2 != nullptr);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(c2->mHP == 0); // reset

	pool.destroyClass(d1);
	pool.destroyClass(c2);
	pool.destroyClass(b1);
	pool.quit();
}

void ClassBaseTypePoolTest::testNewClassListMultipleTypes()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	Vector<DogObj*>  dogs;
	Vector<CatObj*>  cats;
	pool.newClassList<DogObj>(dogs, 10);
	pool.newClassList<CatObj>(cats, 10);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs.size() == 10);
	ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats.size() == 10);
	for (int i = 0; i < 10; ++i)
	{
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(dogs[i]->getType() == 1);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(cats[i]->getType() == 2);
	}
	pool.destroyClassList(dogs);
	pool.destroyClassList(cats);
	pool.quit();
}

void ClassBaseTypePoolTest::testDestroyAndReuseByType()
{
	using namespace ClassBaseTypePoolThreadTestSupport;
	ClassBaseTypePoolThread<AnimalBase> pool;
	// 先放 3 个 DogObj 进池
	DogObj* dogs[3];
	for (int i = 0; i < 3; ++i)
	{
		dogs[i] = pool.newClass<DogObj>();
		dogs[i]->mSpeed = (float)(i + 1) * 2.0f;
	}
	for (int i = 0; i < 3; ++i)
	{
		pool.destroyClass(dogs[i]);
	}
	// 再取出，应仍是 DogObj（通过 getType 验证）
	for (int i = 0; i < 3; ++i)
	{
		DogObj* d = pool.newClass<DogObj>();
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(d != nullptr);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(d->getType() == 1);
		ClassBaseTypePoolThreadTestSupport_CBTPT_ASSERT(fabsf(d->mSpeed) < 1e-5f); // reset
		pool.destroyClass(d);
	}
	pool.quit();
}


void ClassBaseTypePoolTest::test()
{
	testInitDefault();
	testNewClass();
	testNewClassReuseFromPool();
	testNewClassList();
	testNewClassListReuseFromPool();
	testDestroyClass();
	testDestroyClassNullSafe();
	testDestroyClassListVector();
	testDestroyClassListHashMap();
	testDestroyClassListArrayList();
	testMultipleTypes();
	testReuseByTypeHash();
	testQuit();
	testLargeScale();
	testDestroyThenNew();
	testMarkUsableAssignID();
	testInitDefaultThread();
	testNewClassThread();
	testNewClassReuse();
	testNewClassListThread();
	testDestroyClassThread();
	testDestroyClassNullSafeThread();
	testDestroyClassListVectorThread();
	testDestroyClassListHashMapThread();
	testDestroyClassListArrayListThread();
	testMultipleTypesThread();
	testQuitThread();
	testClearPool();
	testAssignID();
	testLargeScaleThread();
	testMixedNewDestroy();
	testNewClassListMultipleTypes();
	testDestroyAndReuseByType();
}
#endif
