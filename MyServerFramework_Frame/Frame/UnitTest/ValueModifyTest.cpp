#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// ValueModifyTest
// 测试 ValueModify<T> 和 MapModify<Key,Value> 两个修改描述结构体
// QueueAndModifyTest.cpp 已覆盖 Queue、MapModify 和 ValueModify 的基础场景，
// 本文件补充更多类型、批量场景和模拟业务场景。
// ============================================================

#define VMT_ASSERT(expr) \
    if (!(expr)) { LOG("ValueModifyTest FAIL: " #expr); }

void ValueModifyTest::test()
{
    testValueModifyBasicAdd();
    testValueModifyBasicRemove();
    testValueModifyIntType();
    testValueModifyFloatType();
    testValueModifyStringType();
    testValueModifyVectorUsage();
    testValueModifySimulateApply();
    testMapModifyBasicAdd();
    testMapModifyBasicRemove();
    testMapModifyIntInt();
    testMapModifyStringInt();
    testMapModifyVectorUsage();
    testMapModifySimulateApply();
    testMapModifyEraseOnly();
    testMapModifyMixedAddRemove();
    testValueModifyMixedIntBool();
    testValueModifyLargeScale();
    testMapModifyLargeScale();
	testMapModify();
	testValueModify();
}

// ------------------------------------------------------------------
// ValueModify<T>
// ------------------------------------------------------------------

void ValueModifyTest::testValueModifyBasicAdd()
{
    ValueModify<int> vm(100, true);
    VMT_ASSERT(vm.mValue == 100);
    VMT_ASSERT(vm.mAdd == true);
}

void ValueModifyTest::testValueModifyBasicRemove()
{
    ValueModify<int> vm(50, false);
    VMT_ASSERT(vm.mValue == 50);
    VMT_ASSERT(vm.mAdd == false);
}

void ValueModifyTest::testValueModifyIntType()
{
    ValueModify<int> add(42,   true);
    ValueModify<int> rem(-1,   false);
    ValueModify<int> zero(0,   true);
    ValueModify<int> neg(-999, false);

    VMT_ASSERT(add.mValue == 42    && add.mAdd == true);
    VMT_ASSERT(rem.mValue == -1    && rem.mAdd == false);
    VMT_ASSERT(zero.mValue == 0    && zero.mAdd == true);
    VMT_ASSERT(neg.mValue == -999  && neg.mAdd == false);
}

void ValueModifyTest::testValueModifyFloatType()
{
    ValueModify<float> va(3.14f, true);
    ValueModify<float> vr(2.72f, false);
    VMT_ASSERT(fabsf(va.mValue - 3.14f) < 1e-5f);
    VMT_ASSERT(va.mAdd == true);
    VMT_ASSERT(fabsf(vr.mValue - 2.72f) < 1e-5f);
    VMT_ASSERT(vr.mAdd == false);
}

void ValueModifyTest::testValueModifyStringType()
{
    ValueModify<string> vadd("hello", true);
    ValueModify<string> vrem("world", false);
    VMT_ASSERT(vadd.mValue == "hello");
    VMT_ASSERT(vadd.mAdd == true);
    VMT_ASSERT(vrem.mValue == "world");
    VMT_ASSERT(vrem.mAdd == false);
}

void ValueModifyTest::testValueModifyVectorUsage()
{
    // 用 Vector 存储一批修改描述，模拟批处理
    Vector<ValueModify<int>> mods;
    mods.add(ValueModify<int>(1, true));
    mods.add(ValueModify<int>(2, true));
    mods.add(ValueModify<int>(3, false));
    mods.add(ValueModify<int>(4, true));
    mods.add(ValueModify<int>(2, false));

    VMT_ASSERT(mods.size() == 5);
    VMT_ASSERT(mods[0].mAdd == true);
    VMT_ASSERT(mods[2].mAdd == false);
    VMT_ASSERT(mods[4].mValue == 2 && mods[4].mAdd == false);
}

void ValueModifyTest::testValueModifySimulateApply()
{
    // 模拟：主线程将修改指令放入 pending 列表，游戏逻辑帧统一应用
    Set<int> activeSet;
    activeSet.add(10);
    activeSet.add(20);
    activeSet.add(30);

    Vector<ValueModify<int>> pending;
    pending.add(ValueModify<int>(40, true));   // 新增 40
    pending.add(ValueModify<int>(50, true));   // 新增 50
    pending.add(ValueModify<int>(10, false));  // 移除 10
    pending.add(ValueModify<int>(99, false));  // 移除 99（不存在，不报错）

    // 应用
    for (const auto& mod : pending)
    {
        if (mod.mAdd)
        {
            activeSet.add(mod.mValue);
        }
        else
        {
            activeSet.remove(mod.mValue);
        }
    }

    VMT_ASSERT(activeSet.contains(20));
    VMT_ASSERT(activeSet.contains(30));
    VMT_ASSERT(activeSet.contains(40));
    VMT_ASSERT(activeSet.contains(50));
    VMT_ASSERT(!activeSet.contains(10));
    VMT_ASSERT(!activeSet.contains(99));
    VMT_ASSERT(activeSet.size() == 4);
}

// ------------------------------------------------------------------
// MapModify<Key,Value>
// ------------------------------------------------------------------

void ValueModifyTest::testMapModifyBasicAdd()
{
    MapModify<int, string> m(1, "hello");
    VMT_ASSERT(m.mKey == 1);
    VMT_ASSERT(m.mValue == "hello");
    VMT_ASSERT(m.mAdd == true);
}

void ValueModifyTest::testMapModifyBasicRemove()
{
    // erase 操作只需 key，使用单参数构造
    MapModify<int, string> m(5);
    VMT_ASSERT(m.mKey == 5);
    VMT_ASSERT(m.mAdd == false);
}

void ValueModifyTest::testMapModifyIntInt()
{
    MapModify<int, int> add(10, 100);
    MapModify<int, int> rem(10);
    VMT_ASSERT(add.mKey == 10 && add.mValue == 100 && add.mAdd == true);
    VMT_ASSERT(rem.mKey == 10 && rem.mAdd == false);
}

void ValueModifyTest::testMapModifyStringInt()
{
    MapModify<string, int> add("player1", 1000);
    MapModify<string, int> rem("player2");
    VMT_ASSERT(add.mKey == "player1");
    VMT_ASSERT(add.mValue == 1000);
    VMT_ASSERT(add.mAdd == true);
    VMT_ASSERT(rem.mKey == "player2");
    VMT_ASSERT(rem.mAdd == false);
}

void ValueModifyTest::testMapModifyVectorUsage()
{
    Vector<MapModify<int, string>> ops;
    ops.add(MapModify<int, string>(1, "Alice"));
    ops.add(MapModify<int, string>(2, "Bob"));
    ops.add(MapModify<int, string>(3));         // erase key=3
    ops.add(MapModify<int, string>(1));         // erase key=1

    VMT_ASSERT(ops.size() == 4);
    VMT_ASSERT(ops[0].mAdd == true  && ops[0].mValue == "Alice");
    VMT_ASSERT(ops[1].mAdd == true  && ops[1].mValue == "Bob");
    VMT_ASSERT(ops[2].mAdd == false && ops[2].mKey == 3);
    VMT_ASSERT(ops[3].mAdd == false && ops[3].mKey == 1);
}

void ValueModifyTest::testMapModifySimulateApply()
{
    // 模拟：玩家上线/下线
    Map<int, string> onlinePlayers;
    onlinePlayers.add(1, "Alice");
    onlinePlayers.add(2, "Bob");
    onlinePlayers.add(3, "Charlie");

    Vector<MapModify<int, string>> pending;
    pending.add(MapModify<int, string>(4, "Dave"));    // 4 上线
    pending.add(MapModify<int, string>(5, "Eve"));     // 5 上线
    pending.add(MapModify<int, string>(2));            // 2 下线
    pending.add(MapModify<int, string>(99));           // 99 不存在，不崩溃

    for (const auto& op : pending)
    {
        if (op.mAdd)
        {
            onlinePlayers.add(op.mKey, op.mValue);
        }
        else
        {
            onlinePlayers.remove(op.mKey);
        }
    }

    VMT_ASSERT(onlinePlayers.contains(1));
    VMT_ASSERT(!onlinePlayers.contains(2));
    VMT_ASSERT(onlinePlayers.contains(3));
    VMT_ASSERT(onlinePlayers.contains(4));
    VMT_ASSERT(onlinePlayers.contains(5));
    VMT_ASSERT(!onlinePlayers.contains(99));
    VMT_ASSERT(onlinePlayers.size() == 4);
    VMT_ASSERT(onlinePlayers.get(4) == "Dave");
    VMT_ASSERT(onlinePlayers.get(5) == "Eve");
}

void ValueModifyTest::testMapModifyEraseOnly()
{
    // 仅执行删除操作
    Map<int, int> scores;
    scores.add(1, 100);
    scores.add(2, 200);
    scores.add(3, 300);

    Vector<MapModify<int, int>> deletes;
    deletes.add(MapModify<int, int>(1));
    deletes.add(MapModify<int, int>(3));

    for (const auto& d : deletes)
    {
        scores.remove(d.mKey);
    }

    VMT_ASSERT(!scores.contains(1));
    VMT_ASSERT(scores.contains(2));
    VMT_ASSERT(!scores.contains(3));
    VMT_ASSERT(scores.size() == 1);
}

void ValueModifyTest::testMapModifyMixedAddRemove()
{
    // 混合增删，操作顺序很重要
    Map<int, int> data;
    data.add(1, 10);
    data.add(2, 20);

    Vector<MapModify<int, int>> ops;
    ops.add(MapModify<int, int>(3, 30));   // 新增
    ops.add(MapModify<int, int>(1));        // 删除 1
    ops.add(MapModify<int, int>(4, 40));   // 新增
    ops.add(MapModify<int, int>(2, 99));   // 更新 2（insert/overwrite）
    ops.add(MapModify<int, int>(3));        // 删除 3

    for (const auto& op : ops)
    {
        if (op.mAdd)
        {
            // insertOrUpdate 语义
            if (!data.add(op.mKey, op.mValue))
            {
                data.addOrSet(op.mKey, op.mValue);
            }
        }
        else
        {
            data.remove(op.mKey);
        }
    }

    VMT_ASSERT(!data.contains(1));
    VMT_ASSERT(data.get(2) == 99);
    VMT_ASSERT(!data.contains(3));
    VMT_ASSERT(data.get(4) == 40);
    VMT_ASSERT(data.size() == 2);
}

void ValueModifyTest::testValueModifyMixedIntBool()
{
    // ValueModify<bool> 场景
    ValueModify<bool> vt(true,  true);
    ValueModify<bool> vf(false, false);
    VMT_ASSERT(vt.mValue == true  && vt.mAdd == true);
    VMT_ASSERT(vf.mValue == false && vf.mAdd == false);

    // 混合 int + bool 场景（用双缓冲队列模拟）
    Vector<ValueModify<int>> intOps;
    for (int i = 0; i < 20; ++i)
    {
        intOps.add(ValueModify<int>(i, i % 2 == 0));
    }
    int addCount = 0;
    int remCount = 0;
    for (const auto& op : intOps)
    {
        if (op.mAdd) { ++addCount; }
        else         { ++remCount; }
    }
    VMT_ASSERT(addCount == 10);
    VMT_ASSERT(remCount == 10);
}

void ValueModifyTest::testValueModifyLargeScale()
{
    // 1000 个 ValueModify<int> 批量创建 + 验证
    Vector<ValueModify<int>> mods;
    mods.reserve(1000);
    for (int i = 0; i < 1000; ++i)
    {
        mods.add(ValueModify<int>(i, i % 3 != 0));
    }
    VMT_ASSERT(mods.size() == 1000);
    int addCnt = 0;
    int remCnt = 0;
    for (int i = 0; i < 1000; ++i)
    {
        VMT_ASSERT(mods[i].mValue == i);
        if (mods[i].mAdd) { ++addCnt; }
        else              { ++remCnt; }
    }
    // i % 3 == 0 则 mAdd=false：0,3,6,...999 → 共 334 个
    VMT_ASSERT(remCnt == 334);
    VMT_ASSERT(addCnt == 666);
}

void ValueModifyTest::testMapModifyLargeScale()
{
    // 1000 个 MapModify<int,int> 批量创建 + 应用
    Map<int, int> target;
    Vector<MapModify<int, int>> ops;
    ops.reserve(1000);
    for (int i = 0; i < 500; ++i)
    {
        ops.add(MapModify<int, int>(i, i * 2)); // 新增 500 个
    }
    for (int i = 0; i < 500; i += 2)
    {
        ops.add(MapModify<int, int>(i)); // 删除偶数 key (共 250 个)
    }
    VMT_ASSERT(ops.size() == 750);

    for (const auto& op : ops)
    {
        if (op.mAdd)
        {
            target.add(op.mKey, op.mValue);
        }
        else
        {
            target.remove(op.mKey);
        }
    }

    // 奇数 key 1,3,5,...499 应存在（共 250 个）
    VMT_ASSERT(target.size() == 250);
    for (int i = 1; i < 500; i += 2)
    {
        VMT_ASSERT(target.contains(i));
        VMT_ASSERT(target.get(i) == i * 2);
    }
    for (int i = 0; i < 500; i += 2)
    {
        VMT_ASSERT(!target.contains(i));
    }
}

// ============================================================
// Consolidated queue/modify tests
// ============================================================
#define VALUEMODIFYTEST_QMT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("ValueModifyTest FAILED: ") + (msg)); } } while(0)
#define VALUEMODIFYTEST_QMT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("ValueModifyTest FAILED: ") + (msg) + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

void ValueModifyTest::testMapModify()
{
    // erase 构造（只有 key，mAdd=false）
    {
        MapModify<int, int> m(42);
        VALUEMODIFYTEST_QMT_ASSERT_EQ(m.mKey, 42, "MapModify erase ctor mKey==42");
        VALUEMODIFYTEST_QMT_ASSERT(!m.mAdd, "MapModify erase ctor mAdd==false");
    }

    // insert 构造（key + value，mAdd=true）
    {
        MapModify<int, int> m(10, 100);
        VALUEMODIFYTEST_QMT_ASSERT_EQ(m.mKey, 10, "MapModify insert ctor mKey==10");
        VALUEMODIFYTEST_QMT_ASSERT_EQ(m.mValue, 100, "MapModify insert ctor mValue==100");
        VALUEMODIFYTEST_QMT_ASSERT(m.mAdd, "MapModify insert ctor mAdd==true");
    }

    // string key
    {
        MapModify<string, int> m(string("name"), 999);
        VALUEMODIFYTEST_QMT_ASSERT(m.mKey == "name", "MapModify string key");
        VALUEMODIFYTEST_QMT_ASSERT_EQ(m.mValue, 999, "MapModify string value");
        VALUEMODIFYTEST_QMT_ASSERT(m.mAdd, "MapModify string mAdd==true");
    }

    // erase 构造 string key
    {
        MapModify<string, float> m(string("erase_key"));
        VALUEMODIFYTEST_QMT_ASSERT(m.mKey == "erase_key", "MapModify string erase key");
        VALUEMODIFYTEST_QMT_ASSERT(!m.mAdd, "MapModify string erase mAdd==false");
    }

    // 批量验证，模拟修改列表
    {
        Vector<MapModify<int, string>> modifyList;
        modifyList.emplace(MapModify<int, string>(1, "one"));
        modifyList.emplace(MapModify<int, string>(2, "two"));
        modifyList.emplace(MapModify<int, string>(3));     // erase 3
        modifyList.emplace(MapModify<int, string>(4, "four"));

        VALUEMODIFYTEST_QMT_ASSERT_EQ(modifyList.size(), 4, "modifyList size==4");
        VALUEMODIFYTEST_QMT_ASSERT(modifyList[0].mAdd, "modifyList[0].mAdd==true");
        VALUEMODIFYTEST_QMT_ASSERT(!modifyList[2].mAdd, "modifyList[2].mAdd==false");
        VALUEMODIFYTEST_QMT_ASSERT(modifyList[1].mKey == 2, "modifyList[1].mKey==2");
        VALUEMODIFYTEST_QMT_ASSERT(modifyList[1].mValue == "two", "modifyList[1].mValue==two");
        VALUEMODIFYTEST_QMT_ASSERT(modifyList[2].mKey == 3, "modifyList[2].mKey==3");
    }

}

void ValueModifyTest::testValueModify()
{
    // add=true
    {
        ValueModify<int> m(42, true);
        VALUEMODIFYTEST_QMT_ASSERT_EQ(m.mValue, 42, "ValueModify value==42");
        VALUEMODIFYTEST_QMT_ASSERT(m.mAdd, "ValueModify add==true");
    }

    // add=false（erase）
    {
        ValueModify<int> m(99, false);
        VALUEMODIFYTEST_QMT_ASSERT_EQ(m.mValue, 99, "ValueModify value==99");
        VALUEMODIFYTEST_QMT_ASSERT(!m.mAdd, "ValueModify add==false");
    }

    // float 类型
    {
        ValueModify<float> m(3.14f, true);
        VALUEMODIFYTEST_QMT_ASSERT(fabsf(m.mValue - 3.14f) < 1e-4f, "ValueModify float value");
        VALUEMODIFYTEST_QMT_ASSERT(m.mAdd, "ValueModify float add==true");
    }

    // string 类型
    {
        ValueModify<string> m(string("hello"), true);
        VALUEMODIFYTEST_QMT_ASSERT(m.mValue == "hello", "ValueModify string value");
        VALUEMODIFYTEST_QMT_ASSERT(m.mAdd, "ValueModify string add==true");
    }

    // 批量验证
    {
        Vector<ValueModify<int>> list;
        list.emplace(ValueModify<int>(10, true));
        list.emplace(ValueModify<int>(20, true));
        list.emplace(ValueModify<int>(10, false)); // erase 10
        list.emplace(ValueModify<int>(30, true));

        VALUEMODIFYTEST_QMT_ASSERT_EQ(list.size(), 4, "ValueModify batch size==4");
        VALUEMODIFYTEST_QMT_ASSERT(list[0].mAdd, "batch[0].mAdd==true");
        VALUEMODIFYTEST_QMT_ASSERT(list[1].mAdd, "batch[1].mAdd==true");
        VALUEMODIFYTEST_QMT_ASSERT(!list[2].mAdd, "batch[2].mAdd==false");
        VALUEMODIFYTEST_QMT_ASSERT_EQ(list[2].mValue, 10, "batch[2].mValue==10");
    }

}
#endif
