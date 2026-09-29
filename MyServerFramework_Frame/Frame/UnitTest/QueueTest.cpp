#ifdef WINDOWS
#include "FrameHeader.h"

void QueueTest::test()
{
	testQueueBasicInt();
	testQueuePopEmpty();
	testQueuePopOnly();
	testQueueClear();
	testQueueAddRange();
	testQueueFIFOOrder();
	testQueueString();
	testQueueInterleavedAddPop();
	testQueueStress();
	testQueueWithModifyScenario();
}

// ============================================================
// Consolidated queue/modify tests
// ============================================================
#define QUEUETEST_QMT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("QueueTest FAILED: ") + (msg)); } } while(0)
#define QUEUETEST_QMT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("QueueTest FAILED: ") + (msg) + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

void QueueTest::testQueueBasicInt()
{
    Queue<int> q;

    // 初始状态
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue initial isEmpty");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 0, "queue initial size==0");

    // add
    q.add(10);
    QUEUETEST_QMT_ASSERT(!q.isEmpty(), "queue after add not empty");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 1, "queue size==1 after add");

    q.add(20);
    q.add(30);
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 3, "queue size==3");

    // front
    QUEUETEST_QMT_ASSERT_EQ(q.front(), 10, "queue front==10");

    // pop
    int v1 = q.pop(0);
    QUEUETEST_QMT_ASSERT_EQ(v1, 10, "queue pop first==10");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 2, "queue size==2 after pop");

    int v2 = q.pop(0);
    QUEUETEST_QMT_ASSERT_EQ(v2, 20, "queue pop second==20");

    int v3 = q.pop(0);
    QUEUETEST_QMT_ASSERT_EQ(v3, 30, "queue pop third==30");

    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue empty after all pops");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 0, "queue size==0 after all pops");

}

void QueueTest::testQueuePopEmpty()
{
    Queue<int> q;

    // pop 空队列返回默认值
    int v = q.pop(-99);
    QUEUETEST_QMT_ASSERT_EQ(v, -99, "queue pop empty returns defaultValue");
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue still empty after pop-empty");

    // 多次 pop 空队列
    int v2 = q.pop(0);
    int v3 = q.pop(0);
    QUEUETEST_QMT_ASSERT_EQ(v2, 0, "queue pop empty 2");
    QUEUETEST_QMT_ASSERT_EQ(v3, 0, "queue pop empty 3");

}

void QueueTest::testQueuePopOnly()
{
    Queue<int> q;
    q.add(1);
    q.add(2);
    q.add(3);

    // popOnly 只弹出不返回
    q.popOnly();
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 2, "queue size==2 after popOnly");
    QUEUETEST_QMT_ASSERT_EQ(q.front(), 2, "queue front==2 after popOnly");

    q.popOnly();
    QUEUETEST_QMT_ASSERT_EQ(q.front(), 3, "queue front==3 after second popOnly");

    q.popOnly();
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue empty after 3 popOnly");

}

void QueueTest::testQueueClear()
{
    Queue<int> q;
    for (int i = 0; i < 10; ++i)
    {
        q.add(i * 100);
    }
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 10, "queue size==10 before clear");

    q.clear();
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue isEmpty after clear");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 0, "queue size==0 after clear");

    // 清空后可继续使用
    q.add(42);
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 1, "queue add after clear size==1");
    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 42, "queue pop after clear==42");

}

void QueueTest::testQueueAddRange()
{
    Queue<int*> q;

    // 构造测试数据（逐个 add 验证 FIFO）
    int a = 10, b = 20, c = 30;
    q.add(&a);
    q.add(&b);
    q.add(&c);

    QUEUETEST_QMT_ASSERT_EQ(q.size(), 3, "queue add x3 size==3");

    int* p1 = q.pop(nullptr);
    QUEUETEST_QMT_ASSERT(p1 == &a, "queue add pop p1==&a");
    int* p2 = q.pop(nullptr);
    QUEUETEST_QMT_ASSERT(p2 == &b, "queue add pop p2==&b");
    int* p3 = q.pop(nullptr);
    QUEUETEST_QMT_ASSERT(p3 == &c, "queue add pop p3==&c");

    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue empty after pops");

    // 空队列不影响
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue still empty");

}

void QueueTest::testQueueFIFOOrder()
{
    Queue<int> q;
    const int N = 100;
    for (int i = 0; i < N; ++i)
    {
        q.add(i);
    }
    QUEUETEST_QMT_ASSERT_EQ(q.size(), N, "queue FIFO size==N");

    // 按照先进先出顺序弹出
    for (int i = 0; i < N; ++i)
    {
        const int v = q.pop(0);
        QUEUETEST_QMT_ASSERT_EQ(v, i, "queue FIFO pop order");
    }

    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue empty after FIFO drain");

}

void QueueTest::testQueueString()
{
    Queue<string> q;

    q.add("alpha");
    q.add("beta");
    q.add("gamma");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 3, "queue string size==3");
    QUEUETEST_QMT_ASSERT(q.front() == "alpha", "queue string front==alpha");

    const string v1 = q.pop("");
    QUEUETEST_QMT_ASSERT(v1 == "alpha", "queue string pop 1==alpha");

    const string v2 = q.pop("");
    QUEUETEST_QMT_ASSERT(v2 == "beta", "queue string pop 2==beta");

    const string v3 = q.pop("");
    QUEUETEST_QMT_ASSERT(v3 == "gamma", "queue string pop 3==gamma");

    QUEUETEST_QMT_ASSERT(q.isEmpty(), "queue string empty");

}

void QueueTest::testQueueInterleavedAddPop()
{
    Queue<int> q;

    // add 3, pop 2, add 3, pop 4
    q.add(1);
    q.add(2);
    q.add(3);
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 3, "interleaved: size==3");

    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 1, "interleaved: pop 1");
    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 2, "interleaved: pop 2");
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 1, "interleaved: size==1");

    q.add(4);
    q.add(5);
    q.add(6);
    QUEUETEST_QMT_ASSERT_EQ(q.size(), 4, "interleaved: size==4 after add 3 more");

    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 3, "interleaved: pop 3");
    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 4, "interleaved: pop 4");
    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 5, "interleaved: pop 5");
    QUEUETEST_QMT_ASSERT_EQ(q.pop(0), 6, "interleaved: pop 6");
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "interleaved: empty at end");

}

void QueueTest::testQueueStress()
{
    Queue<int> q;
    const int N = 1000;

    // 1000 个元素入队
    for (int i = 0; i < N; ++i)
    {
        q.add(i * 2);
    }
    QUEUETEST_QMT_ASSERT_EQ(q.size(), N, "stress: size==1000");

    // 验证前 10 个
    for (int i = 0; i < 10; ++i)
    {
        const int v = q.pop(0);
        QUEUETEST_QMT_ASSERT_EQ(v, i * 2, "stress: FIFO correct");
    }

    // 清空剩余
    q.clear();
    QUEUETEST_QMT_ASSERT(q.isEmpty(), "stress: clear successful");

}

void QueueTest::testQueueWithModifyScenario()
{
    // 模拟：一边遍历（startForeach），一边将修改操作记录到队列
    Queue<MapModify<int, string>*> pendingQueue;

    // 创建一些操作
    MapModify<int, string> op1(1, "Alice");
    MapModify<int, string> op2(2, "Bob");
    MapModify<int, string> op3(3);            // erase 3
    MapModify<int, string> op4(4, "Dave");

    pendingQueue.add(&op1);
    pendingQueue.add(&op2);
    pendingQueue.add(&op3);
    pendingQueue.add(&op4);

    QUEUETEST_QMT_ASSERT_EQ(pendingQueue.size(), 4, "scenario queue size==4");

    // 消费队列，重建 Map
    Map<int, string> target;
    target.add(3, "Charlie"); // 已有数据

    while (!pendingQueue.isEmpty())
    {
        MapModify<int, string>* op = pendingQueue.pop(nullptr);
        if (op->mAdd)
        {
            target.add(op->mKey, op->mValue);
        }
        else
        {
            target.remove(op->mKey);
        }
    }

    // 验证结果：3 被删除，1/2/4 被插入
    QUEUETEST_QMT_ASSERT(!target.contains(3), "scenario: 3 erased");
    QUEUETEST_QMT_ASSERT(target.contains(1), "scenario: 1 inserted");
    QUEUETEST_QMT_ASSERT(target.contains(2), "scenario: 2 inserted");
    QUEUETEST_QMT_ASSERT(target.contains(4), "scenario: 4 inserted");
    QUEUETEST_QMT_ASSERT(target.get(1, string("")) == "Alice", "scenario: 1==Alice");
    QUEUETEST_QMT_ASSERT(target.get(2, string("")) == "Bob", "scenario: 2==Bob");
    QUEUETEST_QMT_ASSERT(pendingQueue.isEmpty(), "scenario: queue empty after processing");

}
#endif
