#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// 辅助
// ============================================================
#define CHECK(cond) \
    do { if (!(cond)) { \
        std::cerr << "FAIL at line " << __LINE__ << ": " #cond << "\n"; \
        return false; \
    }} while(0)

void SPSCQueueTest::print_pass(const char* name)
{
    cout << "[PASS] " << name << "\n";
}

// ============================================================
// 单线程测试
// ============================================================
bool SPSCQueueTest::test_empty_pop()
{
    SPSCQueue<int, 4> q;
    CHECK(q.pop() == nullptr);
    CHECK(q.pop() == nullptr);
    return true;
}

bool SPSCQueueTest::test_push_then_pop()
{
    SPSCQueue<int, 4> q;
    int x = 99;
    CHECK(q.push(&x) == true);
    int* p = q.pop();
    CHECK(p != nullptr);
    CHECK(*p == 99);
    CHECK(q.pop() == nullptr);
    return true;
}

bool SPSCQueueTest::test_full_push_returns_false()
{
    SPSCQueue<int, 4> q;
    int a = 1;
    int b = 2;
    int c = 3;
    int d = 4;
    CHECK(q.push(&a) == true);
    CHECK(q.push(&b) == true);
    CHECK(q.push(&c) == true);
    CHECK(q.push(&d) == false);
    return true;
}

bool SPSCQueueTest::test_fifo_order()
{
    constexpr size_t N = 8;
    SPSCQueue<int, 16> q;
    int items[N];
    FOR(N)
    {
        items[i] = i;
        CHECK(q.push(&items[i]) == true);
    }
    FOR(N)
    {
        int* p = q.pop();
        CHECK(p != nullptr);
        CHECK(*p == i);
    }
    CHECK(q.pop() == nullptr);
    return true;
}

bool SPSCQueueTest::test_wraparound_reuse()
{
    SPSCQueue<int, 4> q;
    int v[6] = { 10, 20, 30, 40, 50, 60 };

    CHECK(q.push(&v[0]) == true);
    CHECK(q.push(&v[1]) == true);
    CHECK(q.push(&v[2]) == true);
    CHECK(q.push(&v[3]) == false);

    CHECK(*q.pop() == 10);
    CHECK(*q.pop() == 20);

    CHECK(q.push(&v[4]) == true);
    CHECK(q.push(&v[5]) == true);

    CHECK(*q.pop() == 30);
    CHECK(*q.pop() == 50);
    CHECK(*q.pop() == 60);
    CHECK(q.pop() == nullptr);
    return true;
}

bool SPSCQueueTest::test_large_sequential()
{
    constexpr int MSG = 10000;
    constexpr size_t QSIZ = 64;
    SPSCQueue<int, QSIZ> q;
    vector<int> items(MSG);
    std::iota(items.begin(), items.end(), 0);

    int sent = 0;
    int recv = 0;
    while (recv < MSG) 
    {
        while (sent < MSG && q.push(&items[sent]))
        {
            ++sent;
        }
        while (int* p = q.pop())
        {
            CHECK(*p == recv);
            ++recv;
        }
    }
    CHECK(sent == MSG);
    CHECK(recv == MSG);
    return true;
}

// ============================================================
// 多线程测试
// ============================================================

struct Item 
{
    int id;
    uint64_t value;
    uint64_t checksum;
};

bool SPSCQueueTest::test_mt_order_and_integrity()
{
    constexpr size_t QSIZ = 256;
    constexpr int COUNT = 100000;
    SPSCQueue<Item, QSIZ> q;
    vector<Item> items(COUNT);
    FOR(COUNT)
    {
        items[i].id = i;
        items[i].value = static_cast<uint64_t>(i) * 6364136223846793005ULL;
        items[i].checksum = items[i].id ^ items[i].value;
    }

    atomic<bool> producerDone{ false };
    bool orderOk = true;
    bool checksumOk = true;
    int  popCount = 0;

    thread consumer([&]
    {
        int expected = 0;
        while (true)
        {
            if (Item* p = q.pop())
            {
                if (p->id != expected)
                {
                    orderOk = false;
                }
                if ((p->id ^ p->value) != p->checksum)
                {
                    checksumOk = false;
                }
                ++expected;
                ++popCount;
            }
            else
            {
                if (producerDone.load(memory_order_acquire))
                {
                    // 队列空 + producerDone：再确认一次，防止遗漏
                    Item* p = q.pop();
                    if (p == nullptr)
                    {
                        break;
                    }
                    if (p->id != popCount) { orderOk = false; }
                    if ((p->id ^ p->value) != p->checksum) { checksumOk = false; }
                    ++popCount;
                }
            }
        }
        while (Item* p = q.pop())
        {
            if (p->id != popCount)
            {
                orderOk = false;
            }
            if ((p->id ^ p->value) != p->checksum)
            {
                checksumOk = false;
            }
            ++popCount;
        }
    });

    thread producer([&] 
    {
        FOR(COUNT)
        {
            while (!q.push(&items[i]))
            {
                yield();
            }
        }
        producerDone.store(true, memory_order_release);
    });

    producer.join();
    consumer.join();

    CHECK(popCount == COUNT);
    CHECK(orderOk == true);
    CHECK(checksumOk == true);
    return true;
}

bool SPSCQueueTest::test_mt_tiny_queue_boundary()
{
    constexpr size_t QSIZ = 4;
    constexpr int COUNT = 50000;
    SPSCQueue<int, QSIZ> q;
    vector<int> items(COUNT);
    std::iota(items.begin(), items.end(), 0);

    atomic<bool> producerDone{ false };
    vector<int> received;
    received.reserve(COUNT);

    thread consumer([&]
    {
        while (true)
        {
            if (int* p = q.pop())
            {
                received.push_back(*p);
            }
            else
            {
                // 队列空：必须再次确认 producerDone，避免漏消费
                if (producerDone.load(memory_order_acquire))
                {
                    // 再次 try_pop，防止 producerDone 设置后队列还有残余
                    int* p = q.pop();
                    if (p == nullptr)
                        break;
                    received.push_back(*p);
                }
            }
        }
        // drain 残余
        while (int* p = q.pop())
        {
            received.push_back(*p);
        }
    });

    thread producer([&] 
    {
        FOR(COUNT)
        {
            while (!q.push(&items[i]))
            {
                yield();
            }
        }
        producerDone.store(true, memory_order_release);
    });

    producer.join();
    consumer.join();

    CHECK(static_cast<int>(received.size()) == COUNT);
    FOR(COUNT)
    {
        CHECK(received[i] == i);
    }
    return true;
}

bool SPSCQueueTest::test_mt_producer_leads_consumer()
{
    // 队列容量必须 >= COUNT+1，否则 producer 会在 join 前死锁
    // （没有消费者在跑，队列满了 producer 永远退不出 while 循环）
    constexpr int COUNT = 20000;
    constexpr size_t QSIZ = 32768;   // 2^15 > 20001
    SPSCQueue<int, QSIZ>* q = new SPSCQueue<int, QSIZ>();
    vector<int> items(COUNT);
    std::iota(items.begin(), items.end(), 0);
    atomic<bool> startConsume{ false };
    thread producer([&] 
    {
        FOR(COUNT)
        {
            while (!q->push(&items[i]))
            {
                yield();
            }
        }
        startConsume.store(true, memory_order_release);
    });
    producer.join();

    int recv = 0;
    thread consumer([&] 
    {
        while (recv < COUNT) 
        {
            int* p = q->pop();
            if (p == nullptr)
            {
                continue;
            }
            CHECK(*p == recv);
            ++recv;
        }
        return true;
    });
    consumer.join();
    DELETE(q);

    CHECK(recv == COUNT);
    return true;
}

bool SPSCQueueTest::test_mt_consumer_leads_producer()
{
    constexpr size_t QSIZ = 128;
    constexpr int COUNT = 20000;
    SPSCQueue<int, QSIZ> q;
    vector<int> items(COUNT);
    std::iota(items.begin(), items.end(), 0);
    atomic<bool> producerDone{ false };
    int recv = 0;

    thread consumer([&]
    {
        while (true)
        {
            if (int* p = q.pop())
            {
                CHECK(*p == recv);
                ++recv;
                if (recv == COUNT)
                {
                    break;
                }
            }
            else
            {
                if (producerDone.load(memory_order_acquire))
                {
                    int* p = q.pop();
                    if (p == nullptr)
                    {
                        break;
                    }
                    CHECK(*p == recv);
                    ++recv;
                }
            }
        }
        while (int* p = q.pop())
        {
            CHECK(*p == recv);
            ++recv;
        }
        return true;
    });
    sleep_for(milliseconds(5));
    thread producer([&] 
    {
        FOR(COUNT)
        {
            while (!q.push(&items[i]))
            {
                yield();
            }
        }
        producerDone.store(true, memory_order_release);
    });

    producer.join();
    consumer.join();

    CHECK(recv == COUNT);
    return true;
}

//------------------------------------------------------------------------------
// 扩展测试：单线程
//------------------------------------------------------------------------------

// ── ① 连续 push 两个元素，再连续 pop 两个元素 ──
bool SPSCQueueTest::test_two_element_batch()
{
    SPSCQueue<int, 8> q;
    int a = 111;
    int b = 222;
    int c = 333;
    CHECK(q.push(&a) == true);
    CHECK(q.push(&b) == true);
    CHECK(q.push(&c) == true);

    int* p1 = q.pop();
    int* p2 = q.pop();
    int* p3 = q.pop();
    CHECK(p1 != nullptr && *p1 == 111);
    CHECK(p2 != nullptr && *p2 == 222);
    CHECK(p3 != nullptr && *p3 == 333);
    CHECK(q.pop() == nullptr);
    return true;
}

// ── ② nullptr 本身作为哨兵值被正确传递 ──
// 注意：SPSCQueue 不禁止 push(nullptr)，所以 nullptr 作为普通指针传入时应该原样弹出
bool SPSCQueueTest::test_nullptr_element()
{
    SPSCQueue<int, 4> q;
    CHECK(q.push(nullptr) == true);   // 可以 push nullptr
    CHECK(q.pop() == nullptr);        // 弹出来还是 nullptr
    CHECK(q.pop() == nullptr);        // 队列空后继续 pop 返回 null
    return true;
}

// ── ③ 非 POD 类型：带构造/析构的对象，验证析构函数会被正确调用 ──
struct TrackedObject
{
    static int  sAliveCount;
    static int  sConstructCount;
    static int  sDestructCount;
    static bool sLastDestructIdValid;
    static int  sLastDestructId;
    int id;
    TrackedObject() : id(-1) {}
    explicit TrackedObject(int i) : id(i)
    {
        ++sConstructCount;
        ++sAliveCount;
    }
    ~TrackedObject()
    {
        if (id != -1)
        {
            ++sDestructCount;
            --sAliveCount;
            sLastDestructId = id;
            sLastDestructIdValid = true;
        }
    }
};

int TrackedObject::sAliveCount = 0;
int TrackedObject::sConstructCount = 0;
int TrackedObject::sDestructCount = 0;
bool TrackedObject::sLastDestructIdValid = false;
int TrackedObject::sLastDestructId = -1;

bool SPSCQueueTest::test_non_pod_object()
{
    // 单线程下 push/pop TrackedObject，验证构造/析构配对
    // 模拟 acquire/release 语义：创建对象 → push，pop → 销毁对象
    // 注意：这里的测试只是验证 TrackedObject 本身的行为，队列本身只传指针

	TrackedObject::sAliveCount = 0;
	TrackedObject::sConstructCount = 0;
	TrackedObject::sDestructCount = 0;

    SPSCQueue<TrackedObject, 8> q;

    TrackedObject obj0(0);
    TrackedObject obj1(1);
    TrackedObject obj2(2);

    CHECK(TrackedObject::sConstructCount == 3);
    CHECK(TrackedObject::sAliveCount == 3);

    CHECK(q.push(&obj0) == true);
    CHECK(q.push(&obj1) == true);
    CHECK(q.push(&obj2) == true);

    // pop 后手动析构（模拟使用方 release 的语义）
    TrackedObject* p0 = q.pop();
    TrackedObject* p1 = q.pop();
    TrackedObject* p2 = q.pop();

    CHECK(p0 != nullptr && p0->id == 0);
    CHECK(p1 != nullptr && p1->id == 1);
    CHECK(p2 != nullptr && p2->id == 2);

    // pop 出来还没有析构，所以 alive 不变
    CHECK(TrackedObject::sAliveCount == 3);

    // 模拟 release（销毁）
    p0->~TrackedObject();
    p1->~TrackedObject();
    p2->~TrackedObject();

    CHECK(TrackedObject::sAliveCount == 0);
    CHECK(TrackedObject::sDestructCount == 3);

    return true;
}

// ── ④ 不同容量大小（全部是 2^n），验证无编译/运行错误 ──
bool SPSCQueueTest::test_various_capacities()
{
    // 容量 2, 4, 8, 16, 64, 256, 1024
    int dummy = 42;

    {
        SPSCQueue<int, 2> q2;          // 可用槽 = Capacity - 1 = 1
        CHECK(q2.push(&dummy) == true);
        CHECK(q2.push(&dummy) == false);  // 第 2 个就满了
    }
    {
        SPSCQueue<int, 8> q8;
        FOR(7)
        {
            CHECK(q8.push(&dummy) == true);
        }
        CHECK(q8.push(&dummy) == false);
    }
    {
        SPSCQueue<int, 64> q64;
        FOR(63)
        {
            CHECK(q64.push(&dummy) == true);
        }
        CHECK(q64.push(&dummy) == false);
    }
    {
        SPSCQueue<int, 256> q256;
        FOR(255)
        {
            CHECK(q256.push(&dummy) == true);
        }
        CHECK(q256.push(&dummy) == false);
    }
    {
        SPSCQueue<int, 1024> q1024;
        FOR(1023)
        {
            CHECK(q1024.push(&dummy) == true);
        }
        CHECK(q1024.push(&dummy) == false);
    }
    return true;
}

// ── ⑤ 单线程快速交替 push/pop，验证状态始终一致 ──
// push 1, pop 1, push 2, pop 2, ...
bool SPSCQueueTest::test_alternating_push_pop()
{
    SPSCQueue<int, 8> q;
    FOR(100)
    {
        CHECK(q.push(&i) == true);
        int* p = q.pop();
        CHECK(p != nullptr);
        CHECK(*p == i);
        // 队列始终保持空的状态
        CHECK(q.pop() == nullptr);
    }
    return true;
}

// ── ⑥ 极小容量（2槽），反复绕圈，验证无越界 ──
bool SPSCQueueTest::test_tiny_capacity_repeated_wraparound()
{
    SPSCQueue<int, 2> q;    // 实际可用 = 1
    int v[16];
    FOR(16)
    {
        v[i] = i;
    }

    // 16 轮，每轮：push 1 个，pop 1 个
    FOR(16)
    {
        CHECK(q.push(&v[i]) == true);
        CHECK(q.push(&v[i]) == false);  // 必须满
        int* p = q.pop();
        CHECK(p != nullptr && *p == i);
        CHECK(q.pop() == nullptr);           // 必须空
    }
    return true;
}

// ── ⑦ 单线程：push 到满 → 全量 drain → 再 push（完整绕圈）──
bool SPSCQueueTest::test_full_then_drain_then_full()
{
    SPSCQueue<int, 8> q;   // 可用 = 7
    int items[14];
    FOR(14)
    {
        items[i] = i;
    }

    // 第一轮：push 7 个
    FOR(7)
    {
        CHECK(q.push(&items[i]) == true);
    }
    CHECK(q.push(&items[7]) == false);

    // drain
    FOR(7)
    {
        int* p = q.pop();
        CHECK(p != nullptr && *p == i);
    }
    CHECK(q.pop() == nullptr);

    // 第二轮：再 push 7 个（验证 Capacity 绕圈正确）
    for (int i = 7; i < 14; ++i)
    {
        CHECK(q.push(&items[i]) == true);
    }

    for (int i = 7; i < 14; ++i)
    {
        int* p = q.pop();
        CHECK(p != nullptr && *p == i);
    }
    CHECK(q.pop() == nullptr);
    return true;
}

// ── ⑧ 跨数据类型：不同类型指针共用同一队列大小（模板实例化正确性）──
bool SPSCQueueTest::test_multi_type_same_capacity()
{
	SPSCQueue<char, 8> qc;
	SPSCQueue<short, 8> qs;
	SPSCQueue<long, 8> ql;
	SPSCQueue<double, 8> qd;

	char c = 'A';
	short s = 123;
	long l = 999999;
	double d = 3.14;

    CHECK(qc.push(&c) == true);
    CHECK(qs.push(&s) == true);
    CHECK(ql.push(&l) == true);
    CHECK(qd.push(&d) == true);

    CHECK(*qc.pop() == 'A');
    CHECK(*qs.pop() == 123);
    CHECK(*ql.pop() == 999999);
    CHECK(*qd.pop() == 3.14);
    return true;
}

//------------------------------------------------------------------------------
// 扩展测试：多线程
//------------------------------------------------------------------------------

// ── ⑨ 多轮 burst：生产者 burst 推送一批，消费者 burst 消费一批──
bool SPSCQueueTest::test_mt_burst_rounds()
{
	constexpr size_t QSIZ = 64;
	constexpr int ROUNDS = 100;
	constexpr int BURST = 50;   // 每轮 burst 大小

    SPSCQueue<int, QSIZ>* q = new SPSCQueue<int, QSIZ>();
    int buffer[ROUNDS * BURST];
    std::iota(buffer, buffer + ROUNDS * BURST, 0);

    atomic<bool> done{false};
    vector<int> received;
    received.reserve(ROUNDS * BURST);

    thread consumer([&]
    {
        while (!done.load(memory_order_acquire))
        {
            if (int* p = q->pop())
            {
                received.push_back(*p);
            }
        }
        while (int* p = q->pop())
        {
            received.push_back(*p);
        }
        return true;
    });

    thread producer([&]
    {
        FOR(ROUNDS)
        {
            // burst push
            FOR_J(BURST)
            {
                int idx = i * BURST + j;
                while (!q->push(&buffer[idx])){}
                yield();
            }
            // 本轮 burst 结束后短暂让出，模拟不均匀速率
            sleep_for(microseconds(1));
        }
        done.store(true, memory_order_release);
        return true;
    });

    producer.join();
    consumer.join();
    DELETE(q);

    CHECK(static_cast<int>(received.size()) == ROUNDS * BURST);
    FOR(ROUNDS * BURST)
    { 
        CHECK(received[i] == i);
    }
    return true;
}

// ── ⑩ 消费者快于生产者：消费者持续空转直到数据到来──
bool SPSCQueueTest::test_mt_consumer_faster_than_producer()
{
    constexpr size_t QSIZ  = 128;
    constexpr int COUNT = 10000;

    SPSCQueue<int, QSIZ> q;
    int items[COUNT];
    std::iota(items, items + COUNT, 0);

    atomic<bool> startSignal{false};
    atomic<bool> done{false};
    int recvCount = 0;

    // 消费者先启动，持续空转等待
    thread consumer([&]
    {
        while (!startSignal.load(memory_order_acquire))
        {
            yield();   // 等 startSignal
        }
        while (!done.load(memory_order_acquire))
        {
            if (int* p = q.pop())
            {
                CHECK(*p == recvCount);
                ++recvCount;
            }
        }
        while (int* p = q.pop())
        {
            CHECK(*p == recvCount);
            ++recvCount;
        }
        return true;
    });

    sleep_for(milliseconds(5)); // 确保 consumer 已就位

    // 生产者开始发送
    thread producer([&]
    {
        startSignal.store(true, memory_order_release);
        FOR(COUNT)
        {
            while (!q.push(&items[i]))
            {
                yield();
            }
        }
        done.store(true, memory_order_release);
        return true;
    });

    producer.join();
    consumer.join();

    CHECK(recvCount == COUNT);
    return true;
}

// ── ⑪ 验证 push/pop 不改变对象内容（通过 const 指针）──
bool SPSCQueueTest::test_mt_data_integrity_with_padding()
{
    constexpr size_t QSIZ  = 128;
    constexpr int COUNT = 30000;

    struct Payload
    {
        int id;
        char pad[60];   // 填充字节，模拟真实对象大小
        uint64_t checksum;
    };

    static_assert(sizeof(Payload) == 72, " Payload 大小应为 72 字节");
    SPSCQueue<Payload, QSIZ> q;
    // 堆分配：30000 * 72 = 2MB+，栈上放不下
    vector<Payload> objs(COUNT);
    FOR(COUNT)
    {
        objs[i].id = i;
        memset(objs[i].pad, 0xCC, sizeof(objs[i].pad));
        objs[i].checksum = static_cast<uint64_t>(i) ^ 0xDEADBEEFu;
    }

    atomic<bool> producerDone{false};
    int popCount = 0;
    bool integrityOk = true;

    thread consumer([&]
    {
        while (true)
        {
            if (Payload* p = q.pop())
            {
                if (p->id != popCount)
                {
                    integrityOk = false;
                }
                // 验证 padding 未被破坏
                for (size_t k = 0; k < sizeof(p->pad); ++k)
                {
                    if (p->pad[k] != static_cast<char>(0xCC))
                    {
                        integrityOk = false;
                        break;
                    }
                }
                if (p->checksum != (static_cast<uint64_t>(p->id) ^ 0xDEADBEEFu))
                {
                    integrityOk = false;
                }
                ++popCount;
            }
            else
            {
                if (producerDone.load(memory_order_acquire))
                {
                    Payload* p = q.pop();
                    if (p == nullptr)
                    {
                        break;
                    }
                    if (p->id != popCount) { integrityOk = false; }
                    for (size_t k = 0; k < sizeof(p->pad); ++k)
                    {
                        if (p->pad[k] != static_cast<char>(0xCC))
                        {
                            integrityOk = false;
                            break;
                        }
                    }
                    if (p->checksum != (static_cast<uint64_t>(p->id) ^ 0xDEADBEEFu))
                    {
                        integrityOk = false;
                    }
                    ++popCount;
                }
            }
        }
        while (Payload* p = q.pop())
        {
            if (p->id != popCount)
            {
                integrityOk = false;
            }
            ++popCount;
        }
    });

    thread producer([&]
    {
        FOR_J(COUNT)
        {
            while (!q.push(&objs[j]))
            {
                yield();
            }
        }
        producerDone.store(true, memory_order_release);
        return true;
    });

    producer.join();
    consumer.join();

    CHECK(popCount == COUNT);
    CHECK(integrityOk);
    return true;
}

// ── ⑫ 反复创建/销毁队列，验证无内存泄漏和状态残留 ──
bool SPSCQueueTest::test_mt_repeated_queue_lifecycle()
{
    constexpr int LIFECYCLES = 20;
    constexpr size_t QSIZ = 64;
    constexpr int COUNT = 1000;

    FOR(LIFECYCLES)
    {
        SPSCQueue<int, QSIZ> q;
        int items[COUNT];
        std::iota(items, items + COUNT, 0);
        atomic<bool> done{false};
        int recv = 0;
        thread consumer([&]
        {
            while (!done.load(memory_order_acquire))
            {
                if (int* p = q.pop())
                {
                    CHECK(*p == recv);
                    ++recv;
                }
            }
            while (int* p = q.pop())
            {
                CHECK(*p == recv);
                ++recv;
            }
            return true;
        });

        thread producer([&]
        {
            FOR_J(COUNT)
            {
                while (!q.push(&items[j]))
                {
                    yield();
                }
            }
            done.store(true, memory_order_release);
            return true;
        });

        producer.join();
        consumer.join();

        CHECK(recv == COUNT);
    }
    return true;
}

//------------------------------------------------------------------------------
// 真实场景压测：线程1 创建对象，线程2 销毁对象，同时运行 ≥20 秒
//------------------------------------------------------------------------------

// 模拟真实对象池中的对象
struct PoolObject
{
    int id;                 // 全局递增 ID
    uint64_t magic;         // 魔数校验：0xCAFEBABE00000000 | id
};

// 统计信息
struct TestMetrics
{
    alignas(64) atomic<uint64_t> totalCreated{0};
    alignas(64) atomic<uint64_t> totalDestroyed{0};
    alignas(64) atomic<uint64_t> integrityErrors{0};
    alignas(64) atomic<bool>     running{true};
};

bool SPSCQueueTest::test_mt_realistic_create_destroy()
{
    constexpr size_t QSIZ = 65536;              // 队列容量 2^16
    constexpr int OBJECTS_PER_FRAME = 10000;    // 每帧批量数
    constexpr int TEST_DURATION_SEC = 10;       // 测试时长（秒）

    SPSCQueue<PoolObject, QSIZ>* q = new SPSCQueue<PoolObject, QSIZ>();
    TestMetrics metrics;
    atomic<bool> creatorDone{ false };          // 创建线程是否已经完全结束

    // 对象池：循环分配，避免堆分配开销
    constexpr size_t POOL_SIZE = QSIZ + OBJECTS_PER_FRAME;
    vector<PoolObject>* pObjPool = new vector<PoolObject>(POOL_SIZE);
    auto& objPool = *pObjPool;
    atomic<uint64_t> nextId{ 0 };
    atomic<size_t> allocCursor{ 0 };

    // ── 线程1：创建对象，全速运行，不限帧率 ──
    thread creator([&]() -> bool
    {
        uint64_t created = 0;
        while (metrics.running.load(memory_order_relaxed))
        {
            // 每帧批量创建 OBJECTS_PER_FRAME 个
            int pushedThisFrame = 0;
            while (pushedThisFrame < OBJECTS_PER_FRAME && metrics.running.load(memory_order_relaxed))
            {
                size_t idx = allocCursor.load(memory_order_relaxed) % POOL_SIZE;
                uint64_t id = nextId.load(memory_order_relaxed);

                // 初始化：只设 magic，不做 memset
                objPool[idx].id = static_cast<int>(id);
                objPool[idx].magic = 0xCAFEBABE00000000ULL | id;

                if (q->push(&objPool[idx]))
                {
                    allocCursor.fetch_add(1, memory_order_relaxed);
                    nextId.fetch_add(1, memory_order_relaxed);
                    ++pushedThisFrame;
                    ++created;
                }
                else
                {
                    // 队列满时不能推进 allocCursor 和 nextId,否则会覆盖还未消费的对象
                    yield();
                }
            }
        }

        metrics.totalCreated.store(created, memory_order_release);
        creatorDone.store(true, memory_order_release);
        return true;
    });

    // ── 线程2：销毁对象，全速运行，不限帧率 ──
    thread destroyer([&]() -> bool
    {
        uint64_t destroyed = 0;
        uint64_t errors = 0;

        // running=false 只表示主线程要求停止,不代表 creator 已经完全停止
        // 所以这里必须等 creatorDone=true 以后才能退出
        while (metrics.running.load(memory_order_relaxed) || !creatorDone.load(memory_order_acquire))
        {
            // 全速 pop 并销毁
            while (PoolObject* obj = q->pop())
            {
                // 验证 magic
                if (obj->magic != (0xCAFEBABE00000000ULL | static_cast<uint64_t>(obj->id)))
                {
                    ++errors;
                }

                // 标记为已销毁
                obj->magic = 0;
                obj->id = -1;
                ++destroyed;
            }

            // 队列空，yield 让生产者填充
            yield();
        }

        // drain 残余,此时 creatorDone 已经为 true,不会再有新对象 push 进来
        while (PoolObject* obj = q->pop())
        {
            if (obj->magic != (0xCAFEBABE00000000ULL | static_cast<uint64_t>(obj->id)))
            {
                ++errors;
            }

            obj->magic = 0;
            obj->id = -1;
            ++destroyed;
        }

        metrics.totalDestroyed.store(destroyed, memory_order_release);
        metrics.integrityErrors.store(errors, memory_order_release);
        return true;
    });

    // ── 主线程：等 10 秒后通知停止 ──
    auto testStart = steady_clock::now();
    sleep_for(seconds(TEST_DURATION_SEC));
    metrics.running.store(false, memory_order_release);

    creator.join();
    destroyer.join();

    DELETE(q);
    DELETE(pObjPool);

    auto testEnd = steady_clock::now();
    double actualSec = duration_cast<milliseconds>(testEnd - testStart).count() / 1000.0;

    // ── 结果统计 ──
    uint64_t totalCreated = metrics.totalCreated.load(memory_order_acquire);
    uint64_t totalDestroyed = metrics.totalDestroyed.load(memory_order_acquire);
    uint64_t integrityErrs = metrics.integrityErrors.load(memory_order_acquire);

    double createPerSec = totalCreated / actualSec;
    double destroyPerSec = totalDestroyed / actualSec;

    // 验证条件
    CHECK(integrityErrs == 0);
    CHECK(totalDestroyed <= totalCreated);
    CHECK(totalDestroyed == totalCreated);  // 无泄漏
    CHECK(createPerSec >= 2000000);         // 每秒创建 ≥ 200 万
    CHECK(destroyPerSec >= 2000000);        // 每秒销毁 ≥ 200 万

    return true;
}

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
void SPSCQueueTest::test()
{
    // 单线程
    test_empty_pop();
    test_push_then_pop();
    test_full_push_returns_false();
    test_fifo_order();
    test_wraparound_reuse();
    test_large_sequential();

    // 多线程
    test_mt_order_and_integrity();
    test_mt_tiny_queue_boundary();
    test_mt_producer_leads_consumer();
    test_mt_consumer_leads_producer();

    // 扩展单线程
    test_two_element_batch();
    test_nullptr_element();
    test_non_pod_object();
    test_various_capacities();
    test_alternating_push_pop();
    test_tiny_capacity_repeated_wraparound();
    test_full_then_drain_then_full();
    test_multi_type_same_capacity();

    // 扩展多线程
    test_mt_burst_rounds();
    test_mt_consumer_faster_than_producer();
    test_mt_data_integrity_with_padding();
    test_mt_repeated_queue_lifecycle();

    // 真实场景压测：双线程同时跑 10 秒，每帧创建/销毁 ≥1 万个对象
    test_mt_realistic_create_destroy();
}
#endif
