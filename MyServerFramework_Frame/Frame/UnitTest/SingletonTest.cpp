#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// SingletonTest — Singleton<T> 模板测试
// 覆盖：构造后 getSingleton() 不为 null / 析构后 null /
//       多次构造只生效一次 / 类型独立性 / 继承链中的单例
// ============================================================

#define SGT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("SingletonTest FAILED: ") + (msg)); }

// ============================================================
// 测试用单例类
// ============================================================
class TestSingletonA : public Singleton<TestSingletonA>
{
public:
    int value = 0;
    string name;
    TestSingletonA() : value(0), name("A") {}
};

class TestSingletonB : public Singleton<TestSingletonB>
{
public:
    int counter = 0;
    TestSingletonB() : counter(100) {}
    void increment() { ++counter; }
};

class TestSingletonC : public Singleton<TestSingletonC>
{
public:
    float data = 0.0f;
    TestSingletonC() : data(3.14f) {}
};

// ============================================================
// 测试：构造后可通过 getSingleton() 获取
// ============================================================
void SingletonTest::testSingleton_GetAfterConstruct_A()
{
    TestSingletonA inst;
    SGT_ASSERT(TestSingletonA::getSingleton() != nullptr, "getSingleton A non-null");
    SGT_ASSERT(TestSingletonA::getSingleton() == &inst, "getSingleton A == &inst");
}

void SingletonTest::testSingleton_GetAfterConstruct_B()
{
    TestSingletonB inst;
    SGT_ASSERT(TestSingletonB::getSingleton() != nullptr, "getSingleton B non-null");
    SGT_ASSERT(TestSingletonB::getSingleton() == &inst, "getSingleton B == &inst");
}

void SingletonTest::testSingleton_GetAfterConstruct_C()
{
    TestSingletonC inst;
    SGT_ASSERT(TestSingletonC::getSingleton() != nullptr, "getSingleton C non-null");
    SGT_ASSERT(fabsf(TestSingletonC::getSingleton()->data - 3.14f) < 1e-4f, "getSingleton C data==3.14");
}

// ============================================================
// 测试：析构后 getSingleton() 返回 null
// ============================================================
void SingletonTest::testSingleton_NullAfterDestruct_A()
{
    {
        TestSingletonA inst;
        SGT_ASSERT(TestSingletonA::getSingleton() != nullptr, "A non-null in scope");
    }
    SGT_ASSERT(TestSingletonA::getSingleton() == nullptr, "A null after scope");
}

void SingletonTest::testSingleton_NullAfterDestruct_B()
{
    {
        TestSingletonB inst;
        SGT_ASSERT(TestSingletonB::getSingleton() != nullptr, "B non-null in scope");
    }
    SGT_ASSERT(TestSingletonB::getSingleton() == nullptr, "B null after scope");
}

// ============================================================
// 测试：通过指针修改成员
// ============================================================
void SingletonTest::testSingleton_ModifyViaPtr()
{
    TestSingletonA inst;
    inst.value = 42;
    inst.name = "modified";
    TestSingletonA* ptr = TestSingletonA::getSingleton();
    SGT_ASSERT(ptr != nullptr, "ptr non-null");
    SGT_ASSERT(ptr->value == 42, "ptr->value==42");
    SGT_ASSERT(ptr->name == "modified", "ptr->name==modified");
}

void SingletonTest::testSingleton_ModifyViaPtr_B()
{
    TestSingletonB inst;
    inst.increment();
    inst.increment();
    inst.increment();
    SGT_ASSERT(TestSingletonB::getSingleton()->counter == 103, "B counter==103");
}

// ============================================================
// 测试：不同类型互不干扰
// ============================================================
void SingletonTest::testSingleton_TypeIndependence()
{
    TestSingletonA a;
    TestSingletonB b;
    TestSingletonC c;
    a.value = 1;
    b.counter = 200;
    c.data = 9.9f;

    SGT_ASSERT(TestSingletonA::getSingleton() != nullptr, "A non-null");
    SGT_ASSERT(TestSingletonB::getSingleton() != nullptr, "B non-null");
    SGT_ASSERT(TestSingletonC::getSingleton() != nullptr, "C non-null");

    SGT_ASSERT(TestSingletonA::getSingleton()->value == 1, "A value==1");
    SGT_ASSERT(TestSingletonB::getSingleton()->counter == 200, "B counter==200");
    SGT_ASSERT(fabsf(TestSingletonC::getSingleton()->data - 9.9f) < 1e-4f, "C data==9.9");

    // 各自的指针不同
    SGT_ASSERT((void*)TestSingletonA::getSingleton() != (void*)TestSingletonB::getSingleton(),
        "A != B ptr");
    SGT_ASSERT((void*)TestSingletonB::getSingleton() != (void*)TestSingletonC::getSingleton(),
        "B != C ptr");
}

// ============================================================
// 测试：getSingleton 在构造前返回 null
// ============================================================
void SingletonTest::testSingleton_NullBeforeConstruct()
{
    // 确保析构后是 null（依赖前面测试没有留下实例）
    SGT_ASSERT(TestSingletonA::getSingleton() == nullptr, "A null before construct");
    SGT_ASSERT(TestSingletonB::getSingleton() == nullptr, "B null before construct");
    SGT_ASSERT(TestSingletonC::getSingleton() == nullptr, "C null before construct");
}

// ============================================================
// 测试：多次构造只有第一个有效（第二个不覆盖）
// ============================================================
void SingletonTest::testSingleton_FirstConstructorWins()
{
    TestSingletonA first;
    first.value = 100;
    TestSingletonA* firstPtr = TestSingletonA::getSingleton();
    SGT_ASSERT(firstPtr == &first, "first ptr is first inst");
    SGT_ASSERT(firstPtr->value == 100, "first value==100");

    // 第二个实例：Singleton 内部检测到已有实例，会 return 不更新 ms_Singleton
    {
        TestSingletonA second;
        second.value = 999;
        // ms_Singleton 仍应指向 first（因为 first 还在作用域）
        SGT_ASSERT(TestSingletonA::getSingleton() == &first, "still first ptr");
        SGT_ASSERT(TestSingletonA::getSingleton()->value == 100, "still first value==100");
    }
    // second 析构后 ms_Singleton 变为 null（second 的析构会置 null）
    // 注意：Singleton 析构总是将 ms_Singleton = nullptr，所以此时是 null
    SGT_ASSERT(TestSingletonA::getSingleton() == nullptr, "after second destruct null");
}

// ============================================================
// 测试：析构与重建循环
// ============================================================
void SingletonTest::testSingleton_ConstructDestructCycle()
{
    FOR(200)
    {
        SGT_ASSERT(TestSingletonA::getSingleton() == nullptr, "cycle start null");
        {
            TestSingletonA inst;
            inst.value = i;
            SGT_ASSERT(TestSingletonA::getSingleton() != nullptr, "cycle in scope non-null");
            SGT_ASSERT(TestSingletonA::getSingleton()->value == i, "cycle value==i");
        }
        SGT_ASSERT(TestSingletonA::getSingleton() == nullptr, "cycle end null");
    }
}

// ============================================================
// 测试：通过 Singleton ptr 调用成员函数
// ============================================================
void SingletonTest::testSingleton_CallMemberFunctions()
{
    TestSingletonB inst;
    TestSingletonB* ptr = TestSingletonB::getSingleton();
    SGT_ASSERT(ptr != nullptr, "callMember ptr non-null");
    SGT_ASSERT(ptr->counter == 100, "initial counter==100");
    ptr->increment();
    SGT_ASSERT(ptr->counter == 101, "after increment counter==101");
    ptr->increment();
    ptr->increment();
    SGT_ASSERT(ptr->counter == 103, "after 3 increments counter==103");
    SGT_ASSERT(inst.counter == 103, "inst counter also 103");
}

// ============================================================
// 测试：getSingleton 引用等价于实例
// ============================================================
void SingletonTest::testSingleton_PtrEquality()
{
    TestSingletonA a;
    SGT_ASSERT(TestSingletonA::getSingleton() == &a, "ptr == &a");
    TestSingletonA* p = TestSingletonA::getSingleton();
    p->value = 777;
    SGT_ASSERT(a.value == 777, "modify via ptr reflects in inst");
}

// ============================================================
// 主入口
// ============================================================
void SingletonTest::test()
{
    // 确保所有单例都已销毁（从之前的测试）
    testSingleton_NullBeforeConstruct();
    testSingleton_GetAfterConstruct_A();
    testSingleton_NullAfterDestruct_A();
    testSingleton_GetAfterConstruct_B();
    testSingleton_NullAfterDestruct_B();
    testSingleton_GetAfterConstruct_C();
    testSingleton_ModifyViaPtr();
    testSingleton_NullAfterDestruct_A();
    testSingleton_ModifyViaPtr_B();
    testSingleton_NullAfterDestruct_B();
    testSingleton_TypeIndependence();
    testSingleton_NullBeforeConstruct();
    testSingleton_FirstConstructorWins();
    testSingleton_ConstructDestructCycle();
    testSingleton_CallMemberFunctions();
    testSingleton_NullAfterDestruct_B();
    testSingleton_PtrEquality();
    testSingleton_NullAfterDestruct_A();
}
#endif
