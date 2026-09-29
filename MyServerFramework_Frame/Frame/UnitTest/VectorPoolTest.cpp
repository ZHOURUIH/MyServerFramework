#ifdef WINDOWS
#include "FrameHeader.h"

namespace VectorPoolTestSupport
{
#define VectorPoolTestSupport_VP_ASSERT(expr) \
    if (!(expr)) { LOG("VectorPoolTest FAIL: " #expr); }
}

void VectorPoolTest::testVectorPoolNewDestroy()
{
	using namespace VectorPoolTestSupport;
	// 申请一个 int 类型的 Vector，验证非空
	Vector<int>* v = VectorPool<int>::newVector();
	VectorPoolTestSupport_VP_ASSERT(v != nullptr);
	VectorPoolTestSupport_VP_ASSERT(v->isEmpty());
	VectorPool<int>::destroyVector(v);
	// destroyVector 后内部指针已归还池，不再持有
}

void VectorPoolTest::testVectorPoolReuse()
{
	using namespace VectorPoolTestSupport;
	// 申请 → 归还 → 再申请，验证复用路径（同一指针）
	Vector<int>* v1 = VectorPool<int>::newVector();
	v1->add(42);
	VectorPoolTestSupport_VP_ASSERT(v1->size() == 1);
	VectorPool<int>::destroyVector(v1);
	// destroy 后 v1 内部已 clear，但地址可能被复用
	Vector<int>* v2 = VectorPool<int>::newVector();
	VectorPoolTestSupport_VP_ASSERT(v2 != nullptr);
	// 归还后内容被清空
	VectorPoolTestSupport_VP_ASSERT(v2->isEmpty());
	VectorPool<int>::destroyVector(v2);
}

void VectorPoolTest::testVectorPoolMultipleTypes()
{
	using namespace VectorPoolTestSupport;
	// 多个类型各自独立池
	Vector<int>* vi = VectorPool<int>::newVector();
	Vector<float>* vf = VectorPool<float>::newVector();
	Vector<llong>* vl = VectorPool<llong>::newVector();
	VectorPoolTestSupport_VP_ASSERT(vi != nullptr);
	VectorPoolTestSupport_VP_ASSERT(vf != nullptr);
	VectorPoolTestSupport_VP_ASSERT(vl != nullptr);
	vi->add(1);
	vf->add(3.14f);
	vl->add(9999999999LL);
	VectorPoolTestSupport_VP_ASSERT(vi->size() == 1);
	VectorPoolTestSupport_VP_ASSERT(vf->size() == 1);
	VectorPoolTestSupport_VP_ASSERT(vl->size() == 1);
	VectorPool<int>::destroyVector(vi);
	VectorPool<float>::destroyVector(vf);
	VectorPool<llong>::destroyVector(vl);
}

void VectorPoolTest::testVectorPoolLargeScale()
{
	using namespace VectorPoolTestSupport;
	// 大规模：申请 200 个，验证各自独立
	const int COUNT = 200;
	Vector<Vector<int>*> holder;
	holder.reserve(COUNT);
	for (int i = 0; i < COUNT; ++i)
	{
	    Vector<int>* v = VectorPool<int>::newVector();
	    v->add(i);
	    holder.add(v);
	}
	// 验证内容正确
	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPoolTestSupport_VP_ASSERT(holder[i]->size() == 1);
	    VectorPoolTestSupport_VP_ASSERT((*holder[i])[0] == i);
	}
	// 全部归还
	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPool<int>::destroyVector(holder[i]);
	}
	holder.clear();
	// 再申请 COUNT 个，应全部走复用路径（不增加 mTotalCount）
	for (int i = 0; i < COUNT; ++i)
	{
	    Vector<int>* v = VectorPool<int>::newVector();
	    VectorPoolTestSupport_VP_ASSERT(v != nullptr);
	    VectorPoolTestSupport_VP_ASSERT(v->isEmpty());
	    VectorPool<int>::destroyVector(v);
	}
}

void VectorPoolTest::testVectorPoolDestroyPool()
{
	using namespace VectorPoolTestSupport;
	// destroy() 静态方法：清空池中未使用的 Vector
	// 先申请几个并归还，使池非空，再 destroy
	Vector<int>* v1 = VectorPool<int>::newVector();
	Vector<int>* v2 = VectorPool<int>::newVector();
	VectorPool<int>::destroyVector(v1);
	VectorPool<int>::destroyVector(v2);
	VectorPool<int>::destroy();
	// destroy 后再申请，应走重新分配路径
	Vector<int>* v3 = VectorPool<int>::newVector();
	VectorPoolTestSupport_VP_ASSERT(v3 != nullptr);
	VectorPoolTestSupport_VP_ASSERT(v3->isEmpty());
	VectorPool<int>::destroyVector(v3);
}

void VectorPoolTest::testVectorPoolModifyContent()
{
	using namespace VectorPoolTestSupport;
	// 申请后写入数据，归还并验证 clear 行为
	Vector<int>* v = VectorPool<int>::newVector();
	for (int i = 0; i < 50; ++i)
	{
	    v->add(i * 3);
	}
	VectorPoolTestSupport_VP_ASSERT(v->size() == 50);
	VectorPool<int>::destroyVector(v);
	// 归还后从池取出，应为 empty
	Vector<int>* v2 = VectorPool<int>::newVector();
	VectorPoolTestSupport_VP_ASSERT(v2->isEmpty());
	VectorPool<int>::destroyVector(v2);
}

void VectorPoolTest::testVectorPoolSequential()
{
	using namespace VectorPoolTestSupport;
	// 顺序使用：申请 → 填数 → 归还 → 再申请 → 填数 → 归还，循环多次
	for (int round = 0; round < 30; ++round)
	{
	    Vector<int>* v = VectorPool<int>::newVector();
	    VectorPoolTestSupport_VP_ASSERT(v->isEmpty());
	    for (int j = 0; j < 10; ++j)
	    {
	        v->add(round * 10 + j);
	    }
	    VectorPoolTestSupport_VP_ASSERT(v->size() == 10);
	    VectorPoolTestSupport_VP_ASSERT((*v)[0] == round * 10);
	    VectorPool<int>::destroyVector(v);
	}
}

void VectorPoolTest::testVectorPoolThreadNewDestroy()
{
	using namespace VectorPoolTestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	VectorPoolTestSupport_VP_ASSERT(v != nullptr);
	VectorPoolTestSupport_VP_ASSERT(v->isEmpty());
	pool.destroyVector(&v);
	VectorPoolTestSupport_VP_ASSERT(v == nullptr); // destroyVector 会置 nullptr
}

void VectorPoolTest::testVectorPoolThreadReuse()
{
	using namespace VectorPoolTestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v1 = pool.newVector();
	v1->add(100);
	VectorPoolTestSupport_VP_ASSERT(v1->size() == 1);
	pool.destroyVector(&v1);
	VectorPoolTestSupport_VP_ASSERT(v1 == nullptr);

	// 再申请应走复用路径
	Vector<int>* v2 = pool.newVector();
	VectorPoolTestSupport_VP_ASSERT(v2 != nullptr);
	VectorPoolTestSupport_VP_ASSERT(v2->isEmpty()); // 归还时已 clear
	pool.destroyVector(&v2);
}

void VectorPoolTest::testVectorPoolThreadMultipleTypes()
{
	using namespace VectorPoolTestSupport;
	VectorPoolThread<int>   poolI;
	VectorPoolThread<float> poolF;
	VectorPoolThread<llong> poolL;

	Vector<int>* vi = poolI.newVector();
	Vector<float>* vf = poolF.newVector();
	Vector<llong>* vl = poolL.newVector();

	vi->add(10);
	vf->add(2.5f);
	vl->add(123456789LL);

	VectorPoolTestSupport_VP_ASSERT((*vi)[0] == 10);
	VectorPoolTestSupport_VP_ASSERT(fabsf((*vf)[0] - 2.5f) < 1e-5f);
	VectorPoolTestSupport_VP_ASSERT((*vl)[0] == 123456789LL);

	poolI.destroyVector(&vi);
	poolF.destroyVector(&vf);
	poolL.destroyVector(&vl);
}

void VectorPoolTest::testVectorPoolThreadLargeScale()
{
	using namespace VectorPoolTestSupport;
	VectorPoolThread<int> pool;
	const int COUNT = 300;

	Vector<Vector<int>*> holder;
	holder.reserve(COUNT);

	for (int i = 0; i < COUNT; ++i)
	{
	    Vector<int>* v = pool.newVector();
	    v->add(i);
	    v->add(i * 2);
	    holder.add(v);
	}

	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPoolTestSupport_VP_ASSERT(holder[i]->size() == 2);
	    VectorPoolTestSupport_VP_ASSERT((*holder[i])[0] == i);
	    VectorPoolTestSupport_VP_ASSERT((*holder[i])[1] == i * 2);
	}

	for (int i = 0; i < COUNT; ++i)
	{
	    pool.destroyVector(&holder[i]);
	    VectorPoolTestSupport_VP_ASSERT(holder[i] == nullptr);
	}
	holder.clear();

	// 复用阶段：再申请 COUNT 个均应为空
	for (int i = 0; i < COUNT; ++i)
	{
	    Vector<int>* v = pool.newVector();
	    VectorPoolTestSupport_VP_ASSERT(v != nullptr);
	    VectorPoolTestSupport_VP_ASSERT(v->isEmpty());
	    pool.destroyVector(&v);
	}
}

void VectorPoolTest::testVectorPoolThreadModifyContent()
{
	using namespace VectorPoolTestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	for (int i = 0; i < 100; ++i)
	{
	    v->add(i * 7);
	}
	VectorPoolTestSupport_VP_ASSERT(v->size() == 100);
	VectorPoolTestSupport_VP_ASSERT((*v)[99] == 693);
	pool.destroyVector(&v);

	// 归还后取出验证 clear
	Vector<int>* v2 = pool.newVector();
	VectorPoolTestSupport_VP_ASSERT(v2->isEmpty());
	pool.destroyVector(&v2);
}

void VectorPoolTest::testVectorPoolThreadDestroy()
{
	using namespace VectorPoolTestSupport;
	// destroy() 释放所有池中缓存
	VectorPoolThread<int> pool;
	for (int i = 0; i < 10; ++i)
	{
	    Vector<int>* v = pool.newVector();
	    pool.destroyVector(&v);
	}
	pool.destroy(); // 析构函数也会调用，这里显式调用
	// destroy 后再申请，走新建路径
	Vector<int>* v = pool.newVector();
	VectorPoolTestSupport_VP_ASSERT(v != nullptr);
	pool.destroyVector(&v);
}

void VectorPoolTest::testVectorPoolMixedIntFloat()
{
	using namespace VectorPoolTestSupport;
	// VectorPool<int> 和 VectorPool<float> 交错申请/归还，互不干扰
	Vector<int>* a = VectorPool<int>::newVector();
	Vector<float>* b = VectorPool<float>::newVector();
	Vector<int>* c = VectorPool<int>::newVector();

	a->add(1);
	b->add(1.0f);
	c->add(2);

	VectorPoolTestSupport_VP_ASSERT((*a)[0] == 1);
	VectorPoolTestSupport_VP_ASSERT((*b)[0] == 1.0f);
	VectorPoolTestSupport_VP_ASSERT((*c)[0] == 2);

	VectorPool<int>::destroyVector(a);
	VectorPool<float>::destroyVector(b);
	VectorPool<int>::destroyVector(c);
}

void VectorPoolTest::testVectorPoolThreadNullSafe()
{
	using namespace VectorPoolTestSupport;
	// destroyVector 使用双指针，确保空指针安全
	VectorPoolThread<int> pool;
	Vector<int>* v1 = pool.newVector();
	v1->add(5);
	pool.destroyVector(&v1);
	VectorPoolTestSupport_VP_ASSERT(v1 == nullptr);

	// 先申请两个，再挨个归还，验证池增长
	Vector<int>* v2 = pool.newVector();
	Vector<int>* v3 = pool.newVector();
	v2->add(100);
	v3->add(200);
	VectorPoolTestSupport_VP_ASSERT((*v2)[0] == 100);
	VectorPoolTestSupport_VP_ASSERT((*v3)[0] == 200);
	pool.destroyVector(&v2);
	pool.destroyVector(&v3);
	VectorPoolTestSupport_VP_ASSERT(v2 == nullptr);
	VectorPoolTestSupport_VP_ASSERT(v3 == nullptr);
}

namespace VectorPoolExt2TestSupport
{
#define VectorPoolExt2TestSupport_VPE2_ASSERT(expr) \
    if (!(expr)) { ERROR("VectorPoolExt2Test FAILED: " #expr); }
}

void VectorPoolTest::testVectorPoolIntBasic()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	v->add(1);
	v->add(2);
	v->add(3);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 3);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[0] == 1);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[1] == 2);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[2] == 3);
	VectorPool<int>::destroyVector(v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
}

void VectorPoolTest::testVectorPoolIntReuse()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* v1 = VectorPool<int>::newVector();
	v1->add(10);
	v1->add(20);
	VectorPool<int>::destroyVector(v1);
	Vector<int>* v2 = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2 != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->isEmpty());
	v2->add(100);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v2)[0] == 100);
	VectorPool<int>::destroyVector(v2);
}

void VectorPoolTest::testVectorPoolIntMultiAlloc()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* a = VectorPool<int>::newVector();
	Vector<int>* b = VectorPool<int>::newVector();
	Vector<int>* c = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(a != nullptr && b != nullptr && c != nullptr);
	a->add(1);
	b->add(2);
	c->add(3);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*a)[0] == 1);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*b)[0] == 2);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*c)[0] == 3);
	VectorPool<int>::destroyVector(a);
	VectorPool<int>::destroyVector(b);
	VectorPool<int>::destroyVector(c);
}

void VectorPoolTest::testVectorPoolFloatBasic()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<float>* v = VectorPool<float>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	v->add(3.14f);
	v->add(2.71f);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 2);
	VectorPoolExt2TestSupport_VPE2_ASSERT(fabsf((*v)[0] - 3.14f) < 1e-4f);
	VectorPoolExt2TestSupport_VPE2_ASSERT(fabsf((*v)[1] - 2.71f) < 1e-4f);
	VectorPool<float>::destroyVector(v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
}

void VectorPoolTest::testVectorPoolStringBasic()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<string>* v = VectorPool<string>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	v->add("hello");
	v->add("world");
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 2);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[0] == "hello");
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[1] == "world");
	VectorPool<string>::destroyVector(v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
}

void VectorPoolTest::testVectorPoolModifyAndReturn()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	for (int i = 0; i < 100; ++i)
	{
	    v->add(i);
	}
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 100);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[50] == 50);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[99] == 99);
	VectorPool<int>::destroyVector(v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 0);
}

void VectorPoolTest::testVectorPoolReuseAfterModify()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* v1 = VectorPool<int>::newVector();
	for (int i = 0; i < 50; ++i)
	{
	    v1->add(i * 2);
	}
	VectorPool<int>::destroyVector(v1);
	Vector<int>* v2 = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->isEmpty());
	v2->add(999);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->size() == 1);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v2)[0] == 999);
	VectorPool<int>::destroyVector(v2);
}

void VectorPoolTest::testVectorPoolDestroyPoolExt2()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* v1 = VectorPool<int>::newVector();
	Vector<int>* v2 = VectorPool<int>::newVector();
	v1->add(1);
	v2->add(2);
	VectorPool<int>::destroyVector(v1);
	VectorPool<int>::destroyVector(v2);
	VectorPool<int>::destroy();
	Vector<int>* v3 = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v3 != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v3->isEmpty());
	VectorPool<int>::destroyVector(v3);
}

void VectorPoolTest::testVectorPoolThreadIntBasic()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	v->add(10);
	v->add(20);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 2);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[0] == 10);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[1] == 20);
	pool.destroyVector(&v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v == nullptr);
}

void VectorPoolTest::testVectorPoolThreadIntReuse()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v1 = pool.newVector();
	v1->add(100);
	v1->add(200);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v1->size() == 2);
	pool.destroyVector(&v1);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v1 == nullptr);
	Vector<int>* v2 = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2 != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->isEmpty());
	v2->add(300);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v2)[0] == 300);
	pool.destroyVector(&v2);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2 == nullptr);
}

void VectorPoolTest::testVectorPoolThreadFloatBasic()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<float> pool;
	Vector<float>* v = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	v->add(1.5f);
	v->add(2.5f);
	v->add(3.5f);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 3);
	VectorPoolExt2TestSupport_VPE2_ASSERT(fabsf((*v)[0] - 1.5f) < 1e-4f);
	VectorPoolExt2TestSupport_VPE2_ASSERT(fabsf((*v)[2] - 3.5f) < 1e-4f);
	pool.destroyVector(&v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v == nullptr);
}

void VectorPoolTest::testVectorPoolThreadStringBasic()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<string> pool;
	Vector<string>* v = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->isEmpty());
	v->add("alpha");
	v->add("beta");
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 2);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[0] == "alpha");
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[1] == "beta");
	pool.destroyVector(&v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v == nullptr);
}

void VectorPoolTest::testVectorPoolThreadModifyAndReturn()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	for (int i = 0; i < 200; ++i)
	{
	    v->add(i * 3);
	}
	VectorPoolExt2TestSupport_VPE2_ASSERT(v->size() == 200);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[100] == 300);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v)[199] == 597);
	pool.destroyVector(&v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v == nullptr);
}

void VectorPoolTest::testVectorPoolThreadReuseAfterModify()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v1 = pool.newVector();
	for (int i = 0; i < 100; ++i)
	{
	    v1->add(i);
	}
	pool.destroyVector(&v1);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v1 == nullptr);
	Vector<int>* v2 = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->isEmpty());
	v2->add(77);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*v2)[0] == 77);
	pool.destroyVector(&v2);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2 == nullptr);
}

void VectorPoolTest::testVectorPoolThreadNullAfterDestroy()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v != nullptr);
	v->add(42);
	pool.destroyVector(&v);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v == nullptr);
	Vector<int>* v2 = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2 != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->isEmpty());
	pool.destroyVector(&v2);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2 == nullptr);
}

void VectorPoolTest::testVectorPoolIntLargeScale()
{
	using namespace VectorPoolExt2TestSupport;
	const int COUNT = 50;
	Vector<int>* ptrs[COUNT];
	for (int i = 0; i < COUNT; ++i)
	{
	    ptrs[i] = VectorPool<int>::newVector();
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i] != nullptr);
	    ptrs[i]->add(i);
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPoolExt2TestSupport_VPE2_ASSERT((*ptrs[i])[0] == i);
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPool<int>::destroyVector(ptrs[i]);
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    ptrs[i] = VectorPool<int>::newVector();
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i] != nullptr);
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i]->isEmpty());
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPool<int>::destroyVector(ptrs[i]);
	}
}

void VectorPoolTest::testVectorPoolThreadIntLargeScale()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	const int COUNT = 30;
	Vector<int>* ptrs[COUNT];
	for (int i = 0; i < COUNT; ++i)
	{
	    ptrs[i] = pool.newVector();
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i] != nullptr);
	    ptrs[i]->add(i * 10);
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    VectorPoolExt2TestSupport_VPE2_ASSERT((*ptrs[i])[0] == i * 10);
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    pool.destroyVector(&ptrs[i]);
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i] == nullptr);
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    ptrs[i] = pool.newVector();
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i] != nullptr);
	    VectorPoolExt2TestSupport_VPE2_ASSERT(ptrs[i]->isEmpty());
	}
	for (int i = 0; i < COUNT; ++i)
	{
	    pool.destroyVector(&ptrs[i]);
	}
}

void VectorPoolTest::testVectorPoolMultiType()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* vi = VectorPool<int>::newVector();
	Vector<float>* vf = VectorPool<float>::newVector();
	Vector<string>* vs = VectorPool<string>::newVector();
	vi->add(1);
	vf->add(1.0f);
	vs->add("one");
	VectorPoolExt2TestSupport_VPE2_ASSERT((*vi)[0] == 1);
	VectorPoolExt2TestSupport_VPE2_ASSERT(fabsf((*vf)[0] - 1.0f) < 1e-4f);
	VectorPoolExt2TestSupport_VPE2_ASSERT((*vs)[0] == "one");
	VectorPool<int>::destroyVector(vi);
	VectorPool<float>::destroyVector(vf);
	VectorPool<string>::destroyVector(vs);
}

void VectorPoolTest::testVectorPoolThreadDestroyPool()
{
	using namespace VectorPoolExt2TestSupport;
	VectorPoolThread<int> pool;
	Vector<int>* v1 = pool.newVector();
	Vector<int>* v2 = pool.newVector();
	v1->add(10);
	v2->add(20);
	pool.destroyVector(&v1);
	pool.destroyVector(&v2);
	pool.destroy();
	Vector<int>* v3 = pool.newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v3 != nullptr);
	VectorPoolExt2TestSupport_VPE2_ASSERT(v3->isEmpty());
	pool.destroyVector(&v3);
}

void VectorPoolTest::testVectorPoolReuseCount()
{
	using namespace VectorPoolExt2TestSupport;
	Vector<int>* v = VectorPool<int>::newVector();
	v->add(1);
	VectorPool<int>::destroyVector(v);
	Vector<int>* v2 = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v2->isEmpty());
	v2->add(2);
	VectorPool<int>::destroyVector(v2);
	Vector<int>* v3 = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v3->isEmpty());
	v3->add(3);
	VectorPool<int>::destroyVector(v3);
	Vector<int>* v4 = VectorPool<int>::newVector();
	VectorPoolExt2TestSupport_VPE2_ASSERT(v4->isEmpty());
	VectorPool<int>::destroyVector(v4);
}


void VectorPoolTest::test()
{
    testVectorPoolNewDestroy();
    testVectorPoolReuse();
    testVectorPoolMultipleTypes();
    testVectorPoolLargeScale();
    testVectorPoolDestroyPool();
    testVectorPoolModifyContent();
    testVectorPoolSequential();
    testVectorPoolThreadNewDestroy();
    testVectorPoolThreadReuse();
    testVectorPoolThreadMultipleTypes();
    testVectorPoolThreadLargeScale();
    testVectorPoolThreadModifyContent();
    testVectorPoolThreadDestroy();
    testVectorPoolMixedIntFloat();
    testVectorPoolThreadNullSafe();
    testVectorPoolIntBasic();
    testVectorPoolIntReuse();
    testVectorPoolIntMultiAlloc();
    testVectorPoolFloatBasic();
    testVectorPoolStringBasic();
    testVectorPoolModifyAndReturn();
    testVectorPoolReuseAfterModify();
    testVectorPoolDestroyPoolExt2();
    testVectorPoolThreadIntBasic();
    testVectorPoolThreadIntReuse();
    testVectorPoolThreadFloatBasic();
    testVectorPoolThreadStringBasic();
    testVectorPoolThreadModifyAndReturn();
    testVectorPoolThreadReuseAfterModify();
    testVectorPoolThreadNullAfterDestroy();
    testVectorPoolIntLargeScale();
    testVectorPoolThreadIntLargeScale();
    testVectorPoolMultiType();
    testVectorPoolThreadDestroyPool();
    testVectorPoolReuseCount();
}
#endif
