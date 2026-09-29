#ifdef WINDOWS
#include "FrameHeader.h"
namespace ClassKeyPoolTestSupport
{
    // ============================================================
    // ClassKeyPoolTest
    // Testing ClassKeyPool<ClassType, KeyType>
    // ClassKeyPool is keyed by KeyType (not ClassType::mType),
    // create() is pure virtual and must be implemented by subclass.
    // API: newClass(key), newClass<T>(key), newClassList(key, list, count),
    //      newClassList<T>(key, list, count), destroyClass(obj, key), dump(), quit()
    // ============================================================

#define ClassKeyPoolTestSupport_CKP_ASSERT(expr) \
    if (!(expr)) { ERROR(string("ClassKeyPoolTest FAIL: ") + #expr); }

// ------------------------------------------------------------------
// Helper types
// ------------------------------------------------------------------

// Base class for pooled objects
    class SKBase : public ClassObject
    {
        BASE(SKBase, ClassObject);
    public:
        int mValue = 0;
        float mWeight = 0.0f;
        void resetProperty() override
        {
            base::resetProperty();
            mValue = 0;
            mWeight = 0.0f;
        }
        void destroy() override
        {
            base::destroy();
        }
    };

    class SKSmall : public SKBase
    {
        BASE(SKSmall, SKBase);
    public:
        int mSmallExtra = 0;
        void resetProperty() override
        {
            base::resetProperty();
            mSmallExtra = 0;
        }
        void destroy() override { base::destroy(); }
    };

    class SKLarge : public SKBase
    {
        BASE(SKLarge, SKBase);
    public:
        double mBigData = 0.0;
        int mIndex = -1;
        void resetProperty() override
        {
            base::resetProperty();
            mBigData = 0.0;
            mIndex = -1;
        }
        void destroy() override { base::destroy(); }
    };

    // Pool keyed by int
    class IntKeyedPool : public ClassKeyPool<SKBase, int>
    {
        using base = ClassKeyPool<SKBase, int>;
    protected:
        SKBase* create(const int key) override
        {
            // key 1 -> SKSmall, key 2 -> SKLarge, others -> SKBase default
            if (key == 1)
            {
                return new SKSmall();
            }
            if (key == 2)
            {
                return new SKLarge();
            }
            return new SKBase();
        }
    };

    // Pool keyed by string
    class StringKeyedPool : public ClassKeyPool<SKBase, string>
    {
        using base = ClassKeyPool<SKBase, string>;
    protected:
        SKBase* create(const string key) override
        {
            if (key == "small")
            {
                return new SKSmall();
            }
            if (key == "large")
            {
                return new SKLarge();
            }
            return new SKBase();
        }
    };

    // Pool using a struct as key
    struct TeamKey
    {
        int mTeamID = 0;
        int mSlot = 0;
        bool operator==(const TeamKey& other) const
        {
            return mTeamID == other.mTeamID && mSlot == other.mSlot;
        }
        bool operator<(const TeamKey& other) const
        {
            if (mTeamID != other.mTeamID)
            {
                return mTeamID < other.mTeamID;
            }
            return mSlot < other.mSlot;
        }
    };


    // std::hash for TeamKey (required for HashMap key)
}


namespace std
{
    template<>
    struct hash<ClassKeyPoolTestSupport::TeamKey>
    {
        size_t operator()(const ClassKeyPoolTestSupport::TeamKey& k) const
        {
            return hash<int>()(k.mTeamID) ^ (hash<int>()(k.mSlot) << 4);
        }
    };
}
namespace ClassKeyPoolTestExtraSupport
{
    using SKBase = ClassKeyPoolTestSupport::SKBase;
    using IntKeyedPool = ClassKeyPoolTestSupport::IntKeyedPool;
    using StringKeyedPool = ClassKeyPoolTestSupport::StringKeyedPool;
    using SKSmall = ClassKeyPoolTestSupport::SKSmall;
    using SKLarge = ClassKeyPoolTestSupport::SKLarge;
    using TeamKey = ClassKeyPoolTestSupport::TeamKey;

    class TeamKeyedPool : public ClassKeyPool<SKBase, TeamKey>
    {
        using base = ClassKeyPool<SKBase, TeamKey>;
    protected:
        SKBase* create(const TeamKey key) override
        {
            (void)key;
            return new SKBase();
        }
    };

    // ------------------------------------------------------------------
    // Test implementation
    // ------------------------------------------------------------------
}

void ClassKeyPoolTest::testNewClassSingleKey()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* obj = pool.newClass(0);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj != nullptr);
	pool.destroyClass(obj, 0);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj == nullptr);
}

void ClassKeyPoolTest::testNewClassMultipleKeys()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* a = pool.newClass(1);
	SKBase* b = pool.newClass(2);
	SKBase* c = pool.newClass(3);
	ClassKeyPoolTestSupport_CKP_ASSERT(a != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(b != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(c != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(a != b);
	ClassKeyPoolTestSupport_CKP_ASSERT(b != c);
	pool.destroyClass(a, 1);
	pool.destroyClass(b, 2);
	pool.destroyClass(c, 3);
}

void ClassKeyPoolTest::testDestroyClassNullAfter()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* obj = pool.newClass(10);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj != nullptr);
	pool.destroyClass(obj, 10);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj == nullptr);
}

void ClassKeyPoolTest::testDestroyClassReuse()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* first = pool.newClass(5);
	ClassKeyPoolTestSupport_CKP_ASSERT(first != nullptr);
	first->mValue = 99;
	pool.destroyClass(first, 5);
	SKBase* second = pool.newClass(5);
	ClassKeyPoolTestSupport_CKP_ASSERT(second != nullptr);
	// After resetProperty(), mValue should be 0
	ClassKeyPoolTestSupport_CKP_ASSERT(second->mValue == 0);
	pool.destroyClass(second, 5);
}

void ClassKeyPoolTest::testDestroyNullSafe()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* obj = nullptr;
	pool.destroyClass(obj, 0);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj == nullptr);
}

void ClassKeyPoolTest::testNewClassListSingleKey()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	Vector<SKBase*> list;
	pool.newClassList(0, list, 5);
	ClassKeyPoolTestSupport_CKP_ASSERT(list.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(list[i] != nullptr);
	}
	for (int i = 0; i < 5; ++i)
	{
	    pool.destroyClass(list[i], 0);
	}
}

void ClassKeyPoolTest::testNewClassListMultipleKeys()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	Vector<SKBase*> listA;
	Vector<SKBase*> listB;
	pool.newClassList(1, listA, 3);
	pool.newClassList(2, listB, 4);
	ClassKeyPoolTestSupport_CKP_ASSERT(listA.size() == 3);
	ClassKeyPoolTestSupport_CKP_ASSERT(listB.size() == 4);
	for (int i = 0; i < 3; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(listA[i] != nullptr);
	    pool.destroyClass(listA[i], 1);
	}
	for (int i = 0; i < 4; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(listB[i] != nullptr);
	    pool.destroyClass(listB[i], 2);
	}
}

void ClassKeyPoolTest::testNewClassListPartialReuse()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	Vector<SKBase*> first;
	pool.newClassList(7, first, 4);
	ClassKeyPoolTestSupport_CKP_ASSERT(first.size() == 4);
	for (int i = 0; i < 4; ++i)
	{
	    pool.destroyClass(first[i], 7);
	}

	Vector<SKBase*> second;
	pool.newClassList(7, second, 2);
	ClassKeyPoolTestSupport_CKP_ASSERT(second.size() == 2);
	for (int i = 0; i < 2; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(second[i] != nullptr);
	    pool.destroyClass(second[i], 7);
	}
}

void ClassKeyPoolTest::testNewClassListSizeExact()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	const int N = 10;
	Vector<SKBase*> list;
	pool.newClassList(0, list, N);
	ClassKeyPoolTestSupport_CKP_ASSERT((int)list.size() == N);
	for (int i = 0; i < N; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(list[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
	    pool.destroyClass(list[i], 0);
	}
}

void ClassKeyPoolTest::testNewClassListEmpty()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	Vector<SKBase*> list;
	pool.newClassList(0, list, 0);
	ClassKeyPoolTestSupport_CKP_ASSERT(list.size() == 0);
}

void ClassKeyPoolTest::testMultiRoundAllocFree()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	const int ROUNDS = 5;
	const int COUNT = 8;
	for (int r = 0; r < ROUNDS; ++r)
	{
	    SKBase* objs[COUNT];
	    for (int i = 0; i < COUNT; ++i)
	    {
	        objs[i] = pool.newClass(r % 3);
	        ClassKeyPoolTestSupport_CKP_ASSERT(objs[i] != nullptr);
	    }
	    for (int i = 0; i < COUNT; ++i)
	    {
	        pool.destroyClass(objs[i], r % 3);
	        ClassKeyPoolTestSupport_CKP_ASSERT(objs[i] == nullptr);
	    }
	}
}

void ClassKeyPoolTest::testNewClassSubType()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	// newClass<SKSmall>(key) - allocate as subtype
	SKSmall* s = pool.newClass<SKSmall>(1);
	ClassKeyPoolTestSupport_CKP_ASSERT(s != nullptr);
	s->mSmallExtra = 777;
	pool.destroyClass(s, 1);
	ClassKeyPoolTestSupport_CKP_ASSERT(s == nullptr);
}

void ClassKeyPoolTest::testNewClassListSubType()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	Vector<SKBase*> list;
	pool.newClassList<SKSmall>(1, list, 3);
	ClassKeyPoolTestSupport_CKP_ASSERT(list.size() == 3);
	for (int i = 0; i < 3; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(list[i] != nullptr);
	    pool.destroyClass(list[i], 1);
	}
}

void ClassKeyPoolTest::testAllocManyKeys()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	const int KEY_COUNT = 20;
	SKBase* objs[KEY_COUNT];
	for (int k = 0; k < KEY_COUNT; ++k)
	{
	    objs[k] = pool.newClass(k);
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[k] != nullptr);
	    objs[k]->mValue = k * 10;
	}
	for (int k = 0; k < KEY_COUNT; ++k)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[k]->mValue == k * 10);
	    pool.destroyClass(objs[k], k);
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[k] == nullptr);
	}
}

void ClassKeyPoolTest::testDestroyAllThenReallocate()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	const int N = 6;
	SKBase* objs[N];
	for (int i = 0; i < N; ++i)
	{
	    objs[i] = pool.newClass(0);
	}
	for (int i = 0; i < N; ++i)
	{
	    pool.destroyClass(objs[i], 0);
	}
	for (int i = 0; i < N; ++i)
	{
	    objs[i] = pool.newClass(0);
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
	    pool.destroyClass(objs[i], 0);
	}
}

void ClassKeyPoolTest::testLargeAllocFreeStress()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	const int STRESS = 100;
	SKBase* objs[STRESS];
	for (int i = 0; i < STRESS; ++i)
	{
	    objs[i] = pool.newClass(i % 5);
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[i] != nullptr);
	}
	for (int i = 0; i < STRESS; ++i)
	{
	    pool.destroyClass(objs[i], i % 5);
	}
	for (int i = 0; i < STRESS; ++i)
	{
	    objs[i] = pool.newClass(i % 5);
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[i] != nullptr);
	    pool.destroyClass(objs[i], i % 5);
	}
}

void ClassKeyPoolTest::testKeyZero()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* a = pool.newClass(0);
	ClassKeyPoolTestSupport_CKP_ASSERT(a != nullptr);
	pool.destroyClass(a, 0);
}

void ClassKeyPoolTest::testKeyNegative()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* a = pool.newClass(-1);
	ClassKeyPoolTestSupport_CKP_ASSERT(a != nullptr);
	SKBase* b = pool.newClass(-999);
	ClassKeyPoolTestSupport_CKP_ASSERT(b != nullptr);
	pool.destroyClass(a, -1);
	pool.destroyClass(b, -999);
}

void ClassKeyPoolTest::testMixedKeyAccess()
{
	using namespace ClassKeyPoolTestExtraSupport;
	StringKeyedPool pool;
	SKBase* objSmall = pool.newClass(string("small"));
	SKBase* objLarge = pool.newClass(string("large"));
	SKBase* objDef = pool.newClass(string("default"));
	ClassKeyPoolTestSupport_CKP_ASSERT(objSmall != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(objLarge != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(objDef != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(objSmall != objLarge);
	ClassKeyPoolTestSupport_CKP_ASSERT(objLarge != objDef);
	pool.destroyClass(objSmall, string("small"));
	pool.destroyClass(objLarge, string("large"));
	pool.destroyClass(objDef, string("default"));
}

void ClassKeyPoolTest::testDestroyFromDifferentKeys()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* a = pool.newClass(1);
	SKBase* b = pool.newClass(2);
	SKBase* c = pool.newClass(1);
	ClassKeyPoolTestSupport_CKP_ASSERT(a != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(b != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(c != nullptr);
	pool.destroyClass(a, 1);
	pool.destroyClass(b, 2);
	pool.destroyClass(c, 1);
	// After destroying, allocate again and check reuse works
	SKBase* a2 = pool.newClass(1);
	ClassKeyPoolTestSupport_CKP_ASSERT(a2 != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(a2->mValue == 0);
	pool.destroyClass(a2, 1);
}

void ClassKeyPoolTest::testCountTracking()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	const int N = 15;
	SKBase* objs[N];
	for (int i = 0; i < N; ++i)
	{
	    objs[i] = pool.newClass(i % 3);
	    ClassKeyPoolTestSupport_CKP_ASSERT(objs[i] != nullptr);
	}
	for (int i = 0; i < N; ++i)
	{
	    pool.destroyClass(objs[i], i % 3);
	}
}

void ClassKeyPoolTest::testIsEmpty()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	SKBase* obj = pool.newClass(0);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj != nullptr);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj->mValue == 0);
	pool.destroyClass(obj, 0);
	ClassKeyPoolTestSupport_CKP_ASSERT(obj == nullptr);
}

void ClassKeyPoolTest::testNewClassListReuseExhaustFallback()
{
	using namespace ClassKeyPoolTestExtraSupport;
	IntKeyedPool pool;
	// Pre-populate pool with 3 objects for key=0
	Vector<SKBase*> init;
	pool.newClassList(0, init, 3);
	ClassKeyPoolTestSupport_CKP_ASSERT(init.size() == 3);
	for (int i = 0; i < 3; ++i)
	{
	    pool.destroyClass(init[i], 0);
	}
	// Now request 5 objects: 3 from cache + 2 new
	Vector<SKBase*> result;
	pool.newClassList(0, result, 5);
	ClassKeyPoolTestSupport_CKP_ASSERT(result.size() == 5);
	for (int i = 0; i < 5; ++i)
	{
	    ClassKeyPoolTestSupport_CKP_ASSERT(result[i] != nullptr);
	}
	for (int i = 0; i < 5; ++i)
	{
	    pool.destroyClass(result[i], 0);
	}
}

void ClassKeyPoolTest::testBatchAllocByKey()
{
	using namespace ClassKeyPoolTestExtraSupport;
	TeamKeyedPool pool;
	const int TEAMS = 4;
	const int SLOTS = 5;
	SKBase* matrix[TEAMS][SLOTS];
	for (int t = 0; t < TEAMS; ++t)
	{
	    for (int s = 0; s < SLOTS; ++s)
	    {
	        TeamKey k;
	        k.mTeamID = t;
	        k.mSlot = s;
	        matrix[t][s] = pool.newClass(k);
	        ClassKeyPoolTestSupport_CKP_ASSERT(matrix[t][s] != nullptr);
	    }
	}
	for (int t = 0; t < TEAMS; ++t)
	{
	    for (int s = 0; s < SLOTS; ++s)
	    {
	        TeamKey k;
	        k.mTeamID = t;
	        k.mSlot = s;
	        pool.destroyClass(matrix[t][s], k);
	        ClassKeyPoolTestSupport_CKP_ASSERT(matrix[t][s] == nullptr);
	    }
	}
}

void ClassKeyPoolTest::testNewAfterQuit()
{
	using namespace ClassKeyPoolTestExtraSupport;
	// quit() clears pool's internal lists; after quit, new calls shouldn't crash
	// (They'll just create fresh objects)
	IntKeyedPool pool;
	SKBase* a = pool.newClass(0);
	ClassKeyPoolTestSupport_CKP_ASSERT(a != nullptr);
	pool.destroyClass(a, 0);
	pool.quit();
}

namespace ClassKeyPoolThreadTestSupport
{
    // ============================================================
    // ClassKeyPoolThreadTest
    // 测试 ClassKeyPoolThread<ClassType,KeyType>（线程安全，key 仅为索引，不调用 setType）
    // 注意：与 ClassTypePool/ClassTypePoolThread 的区别是
    //       destroyClass 需要手动传入 key，不从对象内部读 type。
    // 辅助类：TaskBase → TaskSmall, TaskLarge
    // TaskKeyThreadPool 继承 ClassKeyPoolThread 实现 create()
    // ============================================================

#define ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(expr) \
    if (!(expr)) { LOG("ClassKeyPoolThreadTest FAIL: " #expr); }

// ------------------------------------------------------------------
// 辅助类（key 只是优先级标识，不代表对象种类）
// ------------------------------------------------------------------
    enum class TaskPriority : int
    {
        Low = 1,
        Normal = 2,
        High = 3,
    };

    class TaskBase : public ClassObject
    {
        BASE(TaskBase, ClassObject);
    public:
        int   mID = 0;
        float mProgress = 0.0f;
        bool  mCompleted = false;
        void resetProperty() override
        {
            base::resetProperty();
            mID = 0;
            mProgress = 0.0f;
            mCompleted = false;
        }
        void destroy() override
        {
            base::destroy();
        }
    };

    class TaskSmall : public TaskBase
    {
        BASE(TaskSmall, TaskBase);
    public:
        int mSmallData = 0;
        void resetProperty() override
        {
            base::resetProperty();
            mSmallData = 0;
        }
        void destroy() override { base::destroy(); }
    };

    class TaskLarge : public TaskBase
    {
        BASE(TaskLarge, TaskBase);
    public:
        int mLargeData[8] = {};
        void resetProperty() override
        {
            base::resetProperty();
            memset(mLargeData, 0, sizeof(mLargeData));
        }
        void destroy() override { base::destroy(); }
    };

    // ------------------------------------------------------------------
    // Pool 实现：create 根据 key（优先级）决定创建 TaskSmall 或 TaskLarge
    // ------------------------------------------------------------------
    class TaskKeyThreadPool : public ClassKeyPoolThread<TaskBase, TaskPriority>
    {
        typedef TaskKeyThreadPool This;
        typedef ClassKeyPoolThread<TaskBase, TaskPriority> base;
    protected:
        TaskBase* create(const TaskPriority key) override
        {
            if (key == TaskPriority::High)
            {
                return new TaskLarge();
            }
            return new TaskSmall();
        }
    };

    // ------------------------------------------------------------------
    // 测试入口
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testNewClass：基本创建
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testNewClassTemplate：模板版 newClass<T>
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testNewClassReuse：归还后取出，resetProperty 已调用
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testNewClassList：批量申请
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testDestroyClass：销毁置 nullptr
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testDestroyClassNullSafe
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testDestroyClassListVector
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testDestroyClassListHashMap：通过 HashMap 批量销毁
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testDestroyClassListArrayList
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testQuit
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testLargeScale：大规模申请 + 归还
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testAssignID：每次 newClass 得到递增 ID
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testReuseResetProperty：复用时数据归零
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testMultipleKeys：不同 key 的池桶独立
    // ------------------------------------------------------------------

    // ------------------------------------------------------------------
    // testInterleavedNewDestroy：交错申请销毁
    // ------------------------------------------------------------------
}

void ClassKeyPoolTest::testNewClass()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* low = pool.newClass(TaskPriority::Low);
	TaskBase* normal = pool.newClass(TaskPriority::Normal);
	TaskBase* high = pool.newClass(TaskPriority::High);

	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(low != nullptr && low->isDestroy() == false);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(normal != nullptr && normal->isDestroy() == false);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(high != nullptr && high->isDestroy() == false);

	// ClassKeyPoolThread 不调用 setType，所以 getType() 返回默认值（0）
	// 这正是与 ClassTypePoolThread 的关键区别

	pool.destroyClass(low, TaskPriority::Low);
	pool.destroyClass(normal, TaskPriority::Normal);
	pool.destroyClass(high, TaskPriority::High);
	pool.quit();
}

void ClassKeyPoolTest::testNewClassTemplate()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskSmall* s = pool.newClass<TaskSmall>(TaskPriority::Low);
	TaskLarge* l = pool.newClass<TaskLarge>(TaskPriority::High);

	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(s != nullptr && s->isDestroy() == false);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(l != nullptr && l->isDestroy() == false);

	pool.destroyClass(s, TaskPriority::Low);
	pool.destroyClass(l, TaskPriority::High);
	pool.quit();
}

void ClassKeyPoolTest::testNewClassReuse()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* t = pool.newClass(TaskPriority::Normal);
	t->mID = 999;
	t->mProgress = 0.75f;
	pool.destroyClass(t, TaskPriority::Normal);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t == nullptr);

	TaskBase* t2 = pool.newClass(TaskPriority::Normal);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t2 != nullptr);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t2->mID == 0);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(fabsf(t2->mProgress) < 1e-5f);
	pool.destroyClass(t2, TaskPriority::Normal);
	pool.quit();
}

void ClassKeyPoolTest::testNewClassList()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	Vector<TaskBase*> list;
	pool.newClassList(TaskPriority::Normal, list, 6);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(list.size() == 6);
	for (TaskBase* t : list)
	{
	    ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t != nullptr && t->isDestroy() == false);
	}
	// 批量归还（需要一个一个指定 key）
	for (int i = 0; i < list.size(); ++i)
	{
	    pool.destroyClass(list[i], TaskPriority::Normal);
	}
	list.clear();
	pool.quit();
}

void ClassKeyPoolTest::testDestroyClass()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* t = pool.newClass(TaskPriority::High);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t->isDestroy() == false);
	pool.destroyClass(t, TaskPriority::High);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t == nullptr);
	pool.quit();
}

void ClassKeyPoolTest::testDestroyClassNullSafe()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* t = nullptr;
	pool.destroyClass(t, TaskPriority::Low);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t == nullptr);
	pool.quit();
}

void ClassKeyPoolTest::testDestroyClassListVector()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	Vector<TaskBase*> list;
	for (int i = 0; i < 8; ++i)
	{
	    TaskBase* t = pool.newClass(TaskPriority::Low);
	    t->mID = i;
	    list.add(t);
	}
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(list.size() == 8);
	// ClassKeyPoolThread 没有无 key 的 destroyClassList，需手动
	for (int i = 0; i < list.size(); ++i)
	{
	    pool.destroyClass(list[i], TaskPriority::Low);
	}
	list.clear();
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(list.isEmpty());
	pool.quit();
}

void ClassKeyPoolTest::testDestroyClassListHashMap()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	HashMap<int, TaskBase*> map;
	for (int i = 0; i < 5; ++i)
	{
	    TaskBase* t = pool.newClass(TaskPriority::Normal);
	    t->mID = i;
	    map.add(i, t);
	}
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(map.size() == 5);
	// 手动遍历销毁（key 需手动传）
	for (auto& pair : map)
	{
	    pool.destroyClass(pair.second, TaskPriority::Normal);
	}
	map.clear();
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(map.isEmpty());
	pool.quit();
}

void ClassKeyPoolTest::testDestroyClassListArrayList()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	ArrayList<8, TaskBase*> arr;
	for (int i = 0; i < 4; ++i)
	{
	    TaskBase* t = pool.newClass(TaskPriority::High);
	    arr.add(t);
	}
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(arr.size() == 4);
	for (int i = 0; i < arr.size(); ++i)
	{
	    pool.destroyClass(arr[i], TaskPriority::High);
	}
	arr.clear();
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(arr.isEmpty());
	pool.quit();
}

void ClassKeyPoolTest::testQuit()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* a = pool.newClass(TaskPriority::Low);
	TaskBase* b = pool.newClass(TaskPriority::Normal);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(a != nullptr);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(b != nullptr);
	pool.quit();
	(void)a;
	(void)b;
}

void ClassKeyPoolTest::testLargeScale()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	const int COUNT = 300;
	Vector<TaskBase*> lowList;
	Vector<TaskBase*> normalList;
	Vector<TaskBase*> highList;
	lowList.reserve(COUNT);
	normalList.reserve(COUNT);
	highList.reserve(COUNT);

	for (int i = 0; i < COUNT; ++i)
	{
	    TaskBase* lo = pool.newClass(TaskPriority::Low);
	    TaskBase* no = pool.newClass(TaskPriority::Normal);
	    TaskBase* hi = pool.newClass(TaskPriority::High);
	    lo->mID = i;
	    no->mID = i + 1;
	    hi->mID = i + 2;
	    lowList.add(lo);
	    normalList.add(no);
	    highList.add(hi);
	}

	for (int i = 0; i < COUNT; ++i)
	{
	    ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(lowList[i]->mID == i);
	    ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(normalList[i]->mID == i + 1);
	    ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(highList[i]->mID == i + 2);
	}

	for (int i = 0; i < COUNT; ++i)
	{
	    pool.destroyClass(lowList[i], TaskPriority::Low);
	    pool.destroyClass(normalList[i], TaskPriority::Normal);
	    pool.destroyClass(highList[i], TaskPriority::High);
	}
	lowList.clear();
	normalList.clear();
	highList.clear();

	// 再申请 COUNT 个，走复用路径
	for (int i = 0; i < COUNT; ++i)
	{
	    TaskBase* t = pool.newClass(TaskPriority::Normal);
	    ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t != nullptr);
	    ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t->mID == 0); // reset
	    pool.destroyClass(t, TaskPriority::Normal);
	}
	pool.quit();
}

void ClassKeyPoolTest::testAssignID()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* a = pool.newClass(TaskPriority::Low);
	TaskBase* b = pool.newClass(TaskPriority::Normal);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(a->getAssignID() != b->getAssignID());
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(b->getAssignID() > a->getAssignID());
	pool.destroyClass(a, TaskPriority::Low);
	TaskBase* c = pool.newClass(TaskPriority::Low);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(c->getAssignID() > b->getAssignID());
	pool.destroyClass(b, TaskPriority::Normal);
	pool.destroyClass(c, TaskPriority::Low);
	pool.quit();
}

void ClassKeyPoolTest::testReuseResetProperty()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* t = pool.newClass(TaskPriority::Normal);
	t->mID = 777;
	t->mProgress = 0.99f;
	t->mCompleted = true;
	pool.destroyClass(t, TaskPriority::Normal);

	TaskBase* t2 = pool.newClass(TaskPriority::Normal);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t2->mID == 0);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(fabsf(t2->mProgress) < 1e-5f);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(t2->mCompleted == false);
	pool.destroyClass(t2, TaskPriority::Normal);
	pool.quit();
}

void ClassKeyPoolTest::testMultipleKeys()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	// 向 Low 桶放入 3 个
	TaskBase* lo1 = pool.newClass(TaskPriority::Low);
	TaskBase* lo2 = pool.newClass(TaskPriority::Low);
	TaskBase* lo3 = pool.newClass(TaskPriority::Low);
	pool.destroyClass(lo1, TaskPriority::Low);
	pool.destroyClass(lo2, TaskPriority::Low);
	pool.destroyClass(lo3, TaskPriority::Low);

	// 从 High 桶取，应走新建路径（不影响 Low 桶）
	TaskBase* hi = pool.newClass(TaskPriority::High);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(hi != nullptr);
	pool.destroyClass(hi, TaskPriority::High);

	// Low 桶能复用
	TaskBase* lo4 = pool.newClass(TaskPriority::Low);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(lo4 != nullptr);
	pool.destroyClass(lo4, TaskPriority::Low);
	pool.quit();
}

void ClassKeyPoolTest::testInterleavedNewDestroy()
{
	using namespace ClassKeyPoolThreadTestSupport;
	TaskKeyThreadPool pool;
	TaskBase* a = pool.newClass(TaskPriority::Low);
	TaskBase* b = pool.newClass(TaskPriority::Normal);
	a->mID = 1;
	b->mID = 2;
	pool.destroyClass(a, TaskPriority::Low);
	TaskBase* c = pool.newClass(TaskPriority::Low);
	c->mID = 3;
	pool.destroyClass(b, TaskPriority::Normal);
	pool.destroyClass(c, TaskPriority::Low);

	TaskBase* x = pool.newClass(TaskPriority::Low);
	TaskBase* y = pool.newClass(TaskPriority::Normal);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(x != nullptr);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(y != nullptr);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(x->mID == 0);
	ClassKeyPoolThreadTestSupport_CKPTT_ASSERT(y->mID == 0);
	pool.destroyClass(x, TaskPriority::Low);
	pool.destroyClass(y, TaskPriority::Normal);
	pool.quit();
}


void ClassKeyPoolTest::test()
{
    testNewClassSingleKey();
    testNewClassMultipleKeys();
    testDestroyClassNullAfter();
    testDestroyClassReuse();
    testDestroyNullSafe();
    testNewClassListSingleKey();
    testNewClassListMultipleKeys();
    testNewClassListPartialReuse();
    testNewClassListSizeExact();
    testNewClassListEmpty();
    testMultiRoundAllocFree();
    testNewClassSubType();
    testNewClassListSubType();
    testAllocManyKeys();
    testDestroyAllThenReallocate();
    testLargeAllocFreeStress();
    testKeyZero();
    testKeyNegative();
    testMixedKeyAccess();
    testDestroyFromDifferentKeys();
    testCountTracking();
    testIsEmpty();
    testNewClassListReuseExhaustFallback();
    testBatchAllocByKey();
    testNewClass();
    testNewClassTemplate();
    testNewClassReuse();
    testNewClassList();
    testDestroyClass();
    testDestroyClassNullSafe();
    testDestroyClassListVector();
    testDestroyClassListHashMap();
    testDestroyClassListArrayList();
    testQuit();
    testLargeScale();
    testAssignID();
    testReuseResetProperty();
    testMultipleKeys();
    testInterleavedNewDestroy();
}
#endif
