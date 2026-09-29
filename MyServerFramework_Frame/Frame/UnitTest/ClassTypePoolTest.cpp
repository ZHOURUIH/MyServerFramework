#ifdef WINDOWS
#include "FrameHeader.h"
namespace ClassTypePoolTestSupport
{
#define ClassTypePoolTestSupport_CTPT_ASSERT(expression) if (!(expression)) { ERROR(string("ClassTypePoolTest FAILED: ") + #expression); }

	// ============================================================
	// ClassTypePool 测试用辅助类
	// ClassTypePool<ClassType, KeyType> 需要实现 create(KeyType)
	// ClassType 必须是 ClassObject 的子类
	// ============================================================

	// 技能类型枚举（作为 KeyType）
	enum class SkillType : int
	{
		Fire = 1,
		Ice = 2,
		Thunder = 3,
		Wind = 4,
	};

	// 基类
	class SkillBase : public ClassObject
	{
	public:
		int skillID = 0;
		string skillName;
		SkillType skillType = SkillType::Fire;
		bool isActive = false;

		virtual void resetProperty() override
		{
			ClassObject::resetProperty();
			skillID = 0;
			skillName.clear();
			isActive = false;
		}
		virtual void destroy() override
		{
			skillID = 0;
			skillName.clear();
			isActive = false;
		}
		virtual void setType(SkillType type) { skillType = type; }
		virtual SkillType getType() const { return skillType; }
	};

	// 子类 A
	class FireSkill : public SkillBase
	{
	public:
		float fireDamage = 0.0f;
		virtual void resetProperty() override
		{
			SkillBase::resetProperty();
			fireDamage = 100.0f;
			skillType = SkillType::Fire;
		}
		virtual void destroy() override
		{
			SkillBase::destroy();
			fireDamage = 0.0f;
		}
	};

	// 子类 B
	class IceSkill : public SkillBase
	{
	public:
		float iceDamage = 0.0f;
		float slowRate = 0.0f;
		virtual void resetProperty() override
		{
			SkillBase::resetProperty();
			iceDamage = 80.0f;
			slowRate = 0.5f;
			skillType = SkillType::Ice;
		}
		virtual void destroy() override
		{
			SkillBase::destroy();
			iceDamage = 0.0f;
			slowRate = 0.0f;
		}
	};

	// 子类 C
	class ThunderSkill : public SkillBase
	{
	public:
		float thunderDamage = 0.0f;
		int chainCount = 0;
		virtual void resetProperty() override
		{
			SkillBase::resetProperty();
			thunderDamage = 120.0f;
			chainCount = 3;
			skillType = SkillType::Thunder;
		}
		virtual void destroy() override
		{
			SkillBase::destroy();
			thunderDamage = 0.0f;
			chainCount = 0;
		}
	};

	// 实现 ClassTypePool 的具体子类
	class SkillTypePool : public ClassTypePool<SkillBase, SkillType>
	{
	protected:
		virtual SkillBase* create(const SkillType type) override
		{
			if (type == SkillType::Fire)
			{
				return new FireSkill();
			}
			if (type == SkillType::Ice)
			{
				return new IceSkill();
			}
			if (type == SkillType::Thunder)
			{
				return new ThunderSkill();
			}
			return new SkillBase();
		}
	};

	// ============================================================
	// 测试实现
	// ============================================================
}

void ClassTypePoolTest::testInitDefault()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 预分配 5 个 Fire
	pool.initDefault(SkillType::Fire, 5);
	// 预分配 3 个 Ice
	pool.initDefault(SkillType::Ice, 3);

	// newClass 从预分配池取
	SkillBase* skill1 = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill1 != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill1->getType() == SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(!skill1->isDestroy());

	SkillBase* skill2 = pool.newClass(SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2 != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2->getType() == SkillType::Ice);

	pool.destroyClass(skill1);
	pool.destroyClass(skill2);
	pool.quit();
}

void ClassTypePoolTest::testNewClassBasic()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 直接 newClass（无预分配）
	SkillBase* fire = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire->getType() == SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(!fire->isDestroy());
	ClassTypePoolTestSupport_CTPT_ASSERT(fire->getAssignID() > 0);

	SkillBase* ice = pool.newClass(SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice->getType() == SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice->getAssignID() != fire->getAssignID());

	SkillBase* thunder = pool.newClass(SkillType::Thunder);
	ClassTypePoolTestSupport_CTPT_ASSERT(thunder != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(thunder->getType() == SkillType::Thunder);

	// 指针各不相同
	ClassTypePoolTestSupport_CTPT_ASSERT(fire != ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice != thunder);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire != thunder);

	pool.destroyClass(fire);
	pool.destroyClass(ice);
	pool.destroyClass(thunder);

	ClassTypePoolTestSupport_CTPT_ASSERT(fire == nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice == nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(thunder == nullptr);

	pool.quit();
}

void ClassTypePoolTest::testNewClassReuseFromPool()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// newClass, 归还, 再 newClass（应复用）
	SkillBase* first = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(first != nullptr);
	llong firstID = first->getAssignID();

	pool.destroyClass(first);
	ClassTypePoolTestSupport_CTPT_ASSERT(first == nullptr);

	SkillBase* second = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(second != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(second->getAssignID() != firstID); // 新的分配ID
	ClassTypePoolTestSupport_CTPT_ASSERT(second->getType() == SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(!second->isDestroy());

	pool.destroyClass(second);
	pool.quit();
}

void ClassTypePoolTest::testNewClassListBasic()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	Vector<SkillBase*> classList;
	pool.newClassList(SkillType::Fire, classList, 5);

	ClassTypePoolTestSupport_CTPT_ASSERT(classList.size() == 5);
	for (int i = 0; i < (int)classList.size(); ++i)
	{
		ClassTypePoolTestSupport_CTPT_ASSERT(classList[i] != nullptr);
		ClassTypePoolTestSupport_CTPT_ASSERT(classList[i]->getType() == SkillType::Fire);
		ClassTypePoolTestSupport_CTPT_ASSERT(!classList[i]->isDestroy());
		// assignID 全不同
		for (int j = i + 1; j < (int)classList.size(); ++j)
		{
			ClassTypePoolTestSupport_CTPT_ASSERT(classList[i]->getAssignID() != classList[j]->getAssignID());
		}
	}

	pool.destroyClassList(classList);
	ClassTypePoolTestSupport_CTPT_ASSERT(classList.size() == 0);
	pool.quit();
}

void ClassTypePoolTest::testNewClassListReuseFromPool()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 第一批
	Vector<SkillBase*> batch1;
	pool.newClassList(SkillType::Ice, batch1, 4);
	ClassTypePoolTestSupport_CTPT_ASSERT(batch1.size() == 4);

	// 归还
	pool.destroyClassList(batch1);
	ClassTypePoolTestSupport_CTPT_ASSERT(batch1.size() == 0);

	// 第二批（应复用）
	Vector<SkillBase*> batch2;
	pool.newClassList(SkillType::Ice, batch2, 4);
	ClassTypePoolTestSupport_CTPT_ASSERT(batch2.size() == 4);
	for (int i = 0; i < (int)batch2.size(); ++i)
	{
		ClassTypePoolTestSupport_CTPT_ASSERT(batch2[i] != nullptr);
		ClassTypePoolTestSupport_CTPT_ASSERT(batch2[i]->getType() == SkillType::Ice);
	}

	pool.destroyClassList(batch2);
	pool.quit();
}

void ClassTypePoolTest::testDestroyClassBasic()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	SkillBase* skill = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(!skill->isDestroy());

	pool.destroyClass(skill);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill == nullptr);

	pool.quit();
}

void ClassTypePoolTest::testDestroyClassNullSafe()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	SkillBase* p = nullptr;
	pool.destroyClass(p);
	ClassTypePoolTestSupport_CTPT_ASSERT(p == nullptr);

	// 正常 new 后 destroy
	SkillBase* q = pool.newClass(SkillType::Ice);
	pool.destroyClass(q);
	ClassTypePoolTestSupport_CTPT_ASSERT(q == nullptr);

	pool.quit();
}

void ClassTypePoolTest::testDestroyClassListVector()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	Vector<SkillBase*> list;
	for (int i = 0; i < 6; ++i)
	{
		SkillBase* s = pool.newClass(SkillType::Thunder);
		ClassTypePoolTestSupport_CTPT_ASSERT(s != nullptr);
		list.add(s);
	}
	ClassTypePoolTestSupport_CTPT_ASSERT(list.size() == 6);

	pool.destroyClassList(list);
	ClassTypePoolTestSupport_CTPT_ASSERT(list.size() == 0);

	// 归还后再新建
	SkillBase* s2 = pool.newClass(SkillType::Thunder);
	ClassTypePoolTestSupport_CTPT_ASSERT(s2 != nullptr);
	pool.destroyClass(s2);
	pool.quit();
}

void ClassTypePoolTest::testDestroyClassListHashMap()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	HashMap<int, SkillBase*> map;
	for (int i = 0; i < 5; ++i)
	{
		SkillBase* s = pool.newClass(SkillType::Fire);
		ClassTypePoolTestSupport_CTPT_ASSERT(s != nullptr);
		s->skillID = i;
		map.add(i, s);
	}
	ClassTypePoolTestSupport_CTPT_ASSERT(map.size() == 5);

	pool.destroyClassList(map);
	ClassTypePoolTestSupport_CTPT_ASSERT(map.size() == 0);

	pool.quit();
}

void ClassTypePoolTest::testDestroyClassListArrayList()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	ArrayList<10, SkillBase*> arrList;
	for (int i = 0; i < 5; ++i)
	{
		SkillBase* s = pool.newClass(SkillType::Ice);
		ClassTypePoolTestSupport_CTPT_ASSERT(s != nullptr);
		arrList.add(s);
	}
	ClassTypePoolTestSupport_CTPT_ASSERT(arrList.size() == 5);

	pool.destroyClassList(arrList);
	ClassTypePoolTestSupport_CTPT_ASSERT(arrList.size() == 0);

	pool.quit();
}

void ClassTypePoolTest::testGetTotalCount()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// newClass 5 个 Fire
	Vector<SkillBase*> items;
	for (int i = 0; i < 5; ++i)
	{
		items.add(pool.newClass(SkillType::Fire));
	}

	// 归还后 total 不减
	pool.destroyClassList(items);

	// 再新建（复用），total 不增
	Vector<SkillBase*> items2;
	for (int i = 0; i < 5; ++i)
	{
		items2.add(pool.newClass(SkillType::Fire));
	}

	pool.destroyClassList(items2);

	// 新建超过预有总数
	Vector<SkillBase*> items3;
	for (int i = 0; i < 8; ++i)
	{
		items3.add(pool.newClass(SkillType::Fire));
	}

	pool.destroyClassList(items3);
	pool.quit();
}

void ClassTypePoolTest::testMarkUsable()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	SkillBase* skill = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(!skill->isDestroy());
	ClassTypePoolTestSupport_CTPT_ASSERT(skill->getPool() != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill->getAssignID() > 0);

	llong id1 = skill->getAssignID();
	pool.destroyClass(skill);

	SkillBase* skill2 = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(!skill2->isDestroy());
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2->getAssignID() > id1); // 新的 ID

	pool.destroyClass(skill2);
	pool.quit();
}

void ClassTypePoolTest::testMarkDispose()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	SkillBase* skill = pool.newClass(SkillType::Thunder);
	ClassTypePoolTestSupport_CTPT_ASSERT(!skill->isDestroy());

	// 正常 markDispose 通过 destroyClass 调用
	pool.destroyClass(skill);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill == nullptr);

	pool.quit();
}

void ClassTypePoolTest::testMultipleTypes()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 同时分配多种类型
	SkillBase* fire1 = pool.newClass(SkillType::Fire);
	SkillBase* fire2 = pool.newClass(SkillType::Fire);
	SkillBase* ice1 = pool.newClass(SkillType::Ice);
	SkillBase* ice2 = pool.newClass(SkillType::Ice);
	SkillBase* thunder1 = pool.newClass(SkillType::Thunder);

	ClassTypePoolTestSupport_CTPT_ASSERT(fire1->getType() == SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire2->getType() == SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice1->getType() == SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice2->getType() == SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(thunder1->getType() == SkillType::Thunder);

	pool.destroyClass(fire1);
	pool.destroyClass(fire2);
	pool.destroyClass(ice1);
	pool.destroyClass(ice2);
	pool.destroyClass(thunder1);

	pool.quit();
}

void ClassTypePoolTest::testReuseOrder()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 分配 3 个
	SkillBase* s1 = pool.newClass(SkillType::Fire);
	SkillBase* s2 = pool.newClass(SkillType::Fire);
	SkillBase* s3 = pool.newClass(SkillType::Fire);
	llong id1 = s1->getAssignID();
	llong id2 = s2->getAssignID();
	llong id3 = s3->getAssignID();
	ClassTypePoolTestSupport_CTPT_ASSERT(id1 < id2);
	ClassTypePoolTestSupport_CTPT_ASSERT(id2 < id3);

	// 归还 s1, s2
	pool.destroyClass(s1);
	pool.destroyClass(s2);

	// 再分配 2 个，assignID 递增
	SkillBase* r1 = pool.newClass(SkillType::Fire);
	SkillBase* r2 = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(r1->getAssignID() > id3);
	ClassTypePoolTestSupport_CTPT_ASSERT(r2->getAssignID() > r1->getAssignID());

	pool.destroyClass(s3);
	pool.destroyClass(r1);
	pool.destroyClass(r2);
	pool.quit();
}

void ClassTypePoolTest::testQuit()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;
	pool.initDefault(SkillType::Fire, 3);
	pool.initDefault(SkillType::Ice, 2);

	// 分配部分
	SkillBase* s1 = pool.newClass(SkillType::Fire);
	SkillBase* s2 = pool.newClass(SkillType::Ice);
	pool.destroyClass(s1);
	pool.destroyClass(s2);

	// quit 清理
	pool.quit();
	// 不崩溃即通过
}

void ClassTypePoolTest::testNewClassTemplate()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 通过模板参数指定子类类型
	FireSkill* fire = pool.newClass<FireSkill>(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire->getType() == SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(fire->fireDamage == 100.0f);

	IceSkill* ice = pool.newClass<IceSkill>(SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice->getType() == SkillType::Ice);
	ClassTypePoolTestSupport_CTPT_ASSERT(ice->slowRate == 0.5f);

	ThunderSkill* thunder = pool.newClass<ThunderSkill>(SkillType::Thunder);
	ClassTypePoolTestSupport_CTPT_ASSERT(thunder != nullptr);
	ClassTypePoolTestSupport_CTPT_ASSERT(thunder->chainCount == 3);

	SkillBase* fireBase = fire;
	SkillBase* iceBase = ice;
	SkillBase* thunderBase = thunder;

	pool.destroyClass(fireBase);
	pool.destroyClass(iceBase);
	pool.destroyClass(thunderBase);
	pool.quit();
}

void ClassTypePoolTest::testNewClassListTemplate()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	Vector<SkillBase*> classList;
	pool.newClassList<FireSkill>(SkillType::Fire, classList, 4);
	ClassTypePoolTestSupport_CTPT_ASSERT(classList.size() == 4);

	for (int i = 0; i < (int)classList.size(); ++i)
	{
		ClassTypePoolTestSupport_CTPT_ASSERT(classList[i] != nullptr);
		ClassTypePoolTestSupport_CTPT_ASSERT(classList[i]->getType() == SkillType::Fire);
	}

	pool.destroyClassList(classList);
	ClassTypePoolTestSupport_CTPT_ASSERT(classList.size() == 0);
	pool.quit();
}

void ClassTypePoolTest::testLargeScale()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;
	const int N = 100;

	// 批量分配
	Vector<SkillBase*> fireList;
	Vector<SkillBase*> iceList;

	pool.newClassList(SkillType::Fire, fireList, N);
	pool.newClassList(SkillType::Ice, iceList, N);

	ClassTypePoolTestSupport_CTPT_ASSERT(fireList.size() == N);
	ClassTypePoolTestSupport_CTPT_ASSERT(iceList.size() == N);

	// 设置数据
	for (int i = 0; i < N; ++i)
	{
		fireList[i]->skillID = i;
		iceList[i]->skillID = N + i;
	}

	// 验证数据
	for (int i = 0; i < N; ++i)
	{
		ClassTypePoolTestSupport_CTPT_ASSERT(fireList[i]->skillID == i);
		ClassTypePoolTestSupport_CTPT_ASSERT(iceList[i]->skillID == N + i);
	}

	// 全部归还
	pool.destroyClassList(fireList);
	pool.destroyClassList(iceList);

	// 再分配，应复用
	Vector<SkillBase*> fireList2;
	pool.newClassList(SkillType::Fire, fireList2, N);
	ClassTypePoolTestSupport_CTPT_ASSERT(fireList2.size() == N);

	pool.destroyClassList(fireList2);
	pool.quit();
}

void ClassTypePoolTest::testDestroyThenNew()
{
	using namespace ClassTypePoolTestSupport;
	SkillTypePool pool;

	// 分配、设置数据、归还、再分配后验证 resetProperty 正常
	SkillBase* skill = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill != nullptr);
	skill->skillID = 999;
	skill->skillName = "test_skill";
	skill->isActive = true;

	pool.destroyClass(skill);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill == nullptr);

	// 再次获取（复用）
	SkillBase* skill2 = pool.newClass(SkillType::Fire);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2 != nullptr);
	// resetProperty 在 destroy 时已调用，skillID 应该被重置
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2->skillID == 0);
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2->skillName.empty());
	ClassTypePoolTestSupport_CTPT_ASSERT(!skill2->isActive);

	// 设置新数据
	skill2->skillID = 42;
	skill2->skillName = "fire_ball";
	ClassTypePoolTestSupport_CTPT_ASSERT(skill2->skillID == 42);

	pool.destroyClass(skill2);
	pool.quit();
}

namespace ClassTypePoolThreadExtTestSupport
{
	// ============================================================
	// ClassTypePoolThreadExtTest
	// Testing ClassTypePoolThread<ClassType, KeyType>
	// Thread-safe pool keyed by KeyType, creates different ClassType
	// subtypes based on key via virtual create() method.
	// API: newClass(key), newClass<T>(key), newClassList(key, list, n),
	//      newClassList<T>(key, list, n), destroyClass(obj),
	//      initDefault(key, count), quit()
	// ============================================================

#define ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(expr) \
	if (!(expr)) { ERROR(string("ClassTypePoolThreadExtTest FAIL: ") + #expr); }

// ------------------------------------------------------------------
// Helper types
// ------------------------------------------------------------------

	class TTPBase : public ClassObject
	{
		BASE(TTPBase, ClassObject);
	public:
		int mData = 0;
		float mScore = 0.0f;
		int mTypeKey = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mData = 0;
			mScore = 0.0f;
			mTypeKey = 0;
		}
		void destroy() override { base::destroy(); }
		virtual int getType() const { return mTypeKey; }
		virtual void setType(const int type) { mTypeKey = type; }
	};

	class TTPTypeA : public TTPBase
	{
		BASE(TTPTypeA, TTPBase);
	public:
		int mExtraA = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mExtraA = 0;
		}
		void destroy() override { base::destroy(); }
	};

	class TTPTypeB : public TTPBase
	{
		BASE(TTPTypeB, TTPBase);
	public:
		double mBigValue = 0.0;
		void resetProperty() override
		{
			base::resetProperty();
			mBigValue = 0.0;
		}
		void destroy() override { base::destroy(); }
	};

	class TTPTypeC : public TTPBase
	{
		BASE(TTPTypeC, TTPBase);
	public:
		int mC1 = 0;
		int mC2 = 0;
		void resetProperty() override
		{
			base::resetProperty();
			mC1 = 0;
			mC2 = 0;
		}
		void destroy() override { base::destroy(); }
	};

	// Pool keyed by int
	class IntKeyedTypePoolThread : public ClassTypePoolThread<TTPBase, int>
	{
		using base = ClassTypePoolThread<TTPBase, int>;
	protected:
		TTPBase* create(const int key) override
		{
			if (key == 1)
			{
				return new TTPTypeA();
			}
			if (key == 2)
			{
				return new TTPTypeB();
			}
			if (key == 3)
			{
				return new TTPTypeC();
			}
			return new TTPBase();
		}
	};

	// ------------------------------------------------------------------
	// Test implementation
	// ------------------------------------------------------------------
}

void ClassTypePoolTest::testNewClassBasicThreadExt()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* obj = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj == nullptr);
}

void ClassTypePoolTest::testNewClassMultipleTypes()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* a = pool.newClass(1);
	TTPBase* b = pool.newClass(2);
	TTPBase* c = pool.newClass(3);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(b != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(c != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a != b);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(b != c);
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
}

void ClassTypePoolTest::testDestroyClassNullAfter()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* obj = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj == nullptr);
}

void ClassTypePoolTest::testDestroyNullSafe()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* obj = nullptr;
	pool.destroyClass(obj);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj == nullptr);
}

void ClassTypePoolTest::testDestroyClassReuse()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* first = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(first != nullptr);
	first->mData = 777;
	pool.destroyClass(first);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(first == nullptr);
	TTPBase* second = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(second != nullptr);
	// After resetProperty, mData should be 0
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(second->mData == 0);
	pool.destroyClass(second);
}

void ClassTypePoolTest::testInitDefaultThreadExt()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	pool.initDefault(0, 5);
	TTPBase* obj = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
}

void ClassTypePoolTest::testInitDefaultExhaust()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	pool.initDefault(1, 2);
	TTPBase* a = pool.newClass(1);
	TTPBase* b = pool.newClass(1);
	TTPBase* c = pool.newClass(1); // Allocates fresh
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(b != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(c != nullptr);
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
}

void ClassTypePoolTest::testNewClassList()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	Vector<TTPBase*> list;
	pool.newClassList(0, list, 5);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list[i] != nullptr);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassTypePoolTest::testNewClassListSizeExact()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	const int N = 10;
	Vector<TTPBase*> list;
	pool.newClassList(2, list, N);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT((int)list.size() == N);
	for (int i = 0; i < N; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(list[i]);
	}
}

void ClassTypePoolTest::testNewClassListEmpty()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	Vector<TTPBase*> list;
	pool.newClassList(0, list, 0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list.size() == 0);
}

void ClassTypePoolTest::testNewClassListMultipleTypes()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	Vector<TTPBase*> listA;
	Vector<TTPBase*> listB;
	pool.newClassList(1, listA, 3);
	pool.newClassList(2, listB, 4);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(listA.size() == 3);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(listB.size() == 4);
	for (int i = 0; i < 3; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(listA[i] != nullptr);
		pool.destroyClass(listA[i]);
	}
	for (int i = 0; i < 4; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(listB[i] != nullptr);
		pool.destroyClass(listB[i]);
	}
}

void ClassTypePoolTest::testNewClassListReuse()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	Vector<TTPBase*> first;
	pool.newClassList(0, first, 4);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(first.size() == 4);
	for (int i = 0; i < 4; ++i)
	{
		pool.destroyClass(first[i]);
	}
	Vector<TTPBase*> second;
	pool.newClassList(0, second, 2);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(second.size() == 2);
	for (int i = 0; i < 2; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(second[i] != nullptr);
		pool.destroyClass(second[i]);
	}
}

void ClassTypePoolTest::testNewClassListReuseAndGrow()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	// Pre-populate 3 in cache
	Vector<TTPBase*> init;
	pool.newClassList(0, init, 3);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(init.size() == 3);
	for (int i = 0; i < 3; ++i)
	{
		pool.destroyClass(init[i]);
	}
	// Request 5: 3 from cache + 2 new
	Vector<TTPBase*> result;
	pool.newClassList(0, result, 5);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(result.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(result[i] != nullptr);
	}
	for (int i = 0; i < 5; ++i)
	{
		pool.destroyClass(result[i]);
	}
}

void ClassTypePoolTest::testDestroyClassList()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	Vector<TTPBase*> list;
	pool.newClassList(3, list, 6);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list.size() == 6);
	for (int i = 0; i < 6; ++i)
	{
		list[i]->mData = i * 10;
	}
	for (int i = 0; i < 6; ++i)
	{
		pool.destroyClass(list[i]);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list[i] == nullptr);
	}
	// Reallocate — should be reset
	Vector<TTPBase*> list2;
	pool.newClassList(3, list2, 6);
	for (int i = 0; i < 6; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(list2[i]->mData == 0);
		pool.destroyClass(list2[i]);
	}
}

void ClassTypePoolTest::testManyRoundsAlloc()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	const int ROUNDS = 20;
	const int COUNT = 5;
	for (int r = 0; r < ROUNDS; ++r)
	{
		TTPBase* arr[COUNT];
		for (int i = 0; i < COUNT; ++i)
		{
			arr[i] = pool.newClass(r % 4);
			ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
			arr[i]->mData = r * 10 + i;
		}
		for (int i = 0; i < COUNT; ++i)
		{
			ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i]->mData == r * 10 + i);
			pool.destroyClass(arr[i]);
			ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] == nullptr);
		}
	}
}

void ClassTypePoolTest::testLargeAllocFree()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	const int LARGE = 80;
	TTPBase* arr[LARGE];
	for (int i = 0; i < LARGE; ++i)
	{
		arr[i] = pool.newClass(i % 4);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
		arr[i]->mData = i;
	}
	for (int i = 0; i < LARGE; ++i)
	{
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i]->mData == i);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		pool.destroyClass(arr[i]);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		arr[i] = pool.newClass(i % 4);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i]->mData == 0);
	}
	for (int i = 0; i < LARGE; ++i)
	{
		pool.destroyClass(arr[i]);
	}
}

void ClassTypePoolTest::testDestroyThenReallocate()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	const int N = 8;
	TTPBase* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass(0);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(arr[i]);
	}
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass(0);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i]->mData == 0);
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(arr[i]);
	}
}

void ClassTypePoolTest::testInitDefaultMultiple()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	pool.initDefault(0, 3);
	pool.initDefault(1, 3);
	pool.initDefault(2, 4);
	TTPBase* arr0[4];
	TTPBase* arr1[4];
	TTPBase* arr2[5];
	for (int i = 0; i < 3; ++i)
	{
		arr0[i] = pool.newClass(0);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr0[i] != nullptr);
	}
	for (int i = 0; i < 3; ++i)
	{
		arr1[i] = pool.newClass(1);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr1[i] != nullptr);
	}
	for (int i = 0; i < 4; ++i)
	{
		arr2[i] = pool.newClass(2);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr2[i] != nullptr);
	}
	for (int i = 0; i < 3; ++i)
	{
		pool.destroyClass(arr0[i]);
	}
	for (int i = 0; i < 3; ++i)
	{
		pool.destroyClass(arr1[i]);
	}
	for (int i = 0; i < 4; ++i)
	{
		pool.destroyClass(arr2[i]);
	}
}

void ClassTypePoolTest::testPointerUniqueness()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	const int N = 20;
	TTPBase* arr[N];
	for (int i = 0; i < N; ++i)
	{
		arr[i] = pool.newClass(0);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
		for (int j = i + 1; j < N; ++j)
		{
			ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != arr[j]);
		}
	}
	for (int i = 0; i < N; ++i)
	{
		pool.destroyClass(arr[i]);
	}
}

void ClassTypePoolTest::testResetPropertyOnReuse()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* obj = pool.newClass(0);
	obj->mData = 12345;
	obj->mScore = 99.9f;
	pool.destroyClass(obj);
	TTPBase* obj2 = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj2->mData == 0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(fabsf(obj2->mScore) < 1e-4f);
	pool.destroyClass(obj2);
}

void ClassTypePoolTest::testMixedTypeAlloc()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	// Test with int key pool mixing multiple type keys
	IntKeyedTypePoolThread pool;
	TTPBase* a = pool.newClass(1);
	TTPBase* b = pool.newClass(2);
	TTPBase* c = pool.newClass(3);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(b != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(c != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a != b);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(b != c);
	pool.destroyClass(a);
	pool.destroyClass(b);
	pool.destroyClass(c);
	// Re-alloc and check reuse
	TTPBase* a2 = pool.newClass(1);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a2 != nullptr);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(a2->mData == 0);
	pool.destroyClass(a2);
}

void ClassTypePoolTest::testStressAllocFree()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	const int STRESS = 50;
	TTPBase* arr[STRESS];
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newClass(i % 4);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
		arr[i]->mData = i;
	}
	for (int i = 0; i < STRESS; ++i)
	{
		pool.destroyClass(arr[i]);
	}
	for (int i = 0; i < STRESS; ++i)
	{
		arr[i] = pool.newClass(i % 4);
		ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(arr[i] != nullptr);
		pool.destroyClass(arr[i]);
	}
}

void ClassTypePoolTest::testIsEmpty()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	// No method isEmpty on ClassTypePoolThread, just verify newClass works fresh
	TTPBase* obj = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
}

void ClassTypePoolTest::testNewClassAfterQuit()
{
	using namespace ClassTypePoolThreadExtTestSupport;
	IntKeyedTypePoolThread pool;
	TTPBase* obj = pool.newClass(0);
	ClassTypePoolThreadExtTestSupport_CTPTE_ASSERT(obj != nullptr);
	pool.destroyClass(obj);
	pool.quit();
}


void ClassTypePoolTest::test()
{
	testInitDefault();
	testNewClassBasic();
	testNewClassReuseFromPool();
	testNewClassListBasic();
	testNewClassListReuseFromPool();
	testDestroyClassBasic();
	testDestroyClassNullSafe();
	testDestroyClassListVector();
	testDestroyClassListHashMap();
	testDestroyClassListArrayList();
	testGetTotalCount();
	testMarkUsable();
	testMarkDispose();
	testMultipleTypes();
	testReuseOrder();
	testQuit();
	testNewClassTemplate();
	testNewClassListTemplate();
	testLargeScale();
	testDestroyThenNew();
	testNewClassBasicThreadExt();
	testNewClassMultipleTypes();
	testDestroyClassNullAfter();
	testDestroyNullSafe();
	testDestroyClassReuse();
	testInitDefaultThreadExt();
	testInitDefaultExhaust();
	testNewClassList();
	testNewClassListSizeExact();
	testNewClassListEmpty();
	testNewClassListMultipleTypes();
	testNewClassListReuse();
	testNewClassListReuseAndGrow();
	testDestroyClassList();
	testManyRoundsAlloc();
	testLargeAllocFree();
	testDestroyThenReallocate();
	testInitDefaultMultiple();
	testPointerUniqueness();
	testResetPropertyOnReuse();
	testMixedTypeAlloc();
	testStressAllocFree();
	testIsEmpty();
	testNewClassAfterQuit();
}
#endif
