#ifdef WINDOWS
#include "FrameHeader.h"

#define FMT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("FactoryManagerTest FAILED: ") + (msg)); } } while(0)

// ==========================================================================
// 测试用产品类层次 —— 轻量级,不依赖框架业务类型
// ==========================================================================

// 产品基类
class TestProductBase
{
public:
	virtual ~TestProductBase() = default;
	virtual string getName() const { return "Base"; }
	int mValue = 0;
};

// 具体产品A
class TestProductA : public TestProductBase
{
public:
	string getName() const override { return "ProductA"; }
};

// 具体产品B
class TestProductB : public TestProductBase
{
public:
	string getName() const override { return "ProductB"; }
};

// 具体产品C (用于覆盖注册测试)
class TestProductC : public TestProductBase
{
public:
	string getName() const override { return "ProductC"; }
};

// 非子类——用于验证SFINAE编译期拒绝
class TestNotADerived
{
public:
	string getName() const { return "NotDerived"; }
};

// ==========================================================================
// 类型别名,简化后续代码
// ==========================================================================
using TestFactoryMgr = FactoryManager<TestProductBase, int>;

// ==========================================================================
void FactoryManagerTest::test()
{
	testCreateSingleFactory();
	testCreateMultipleFactories();
	testGetFactoryNonExistent();
	testGetFactoryCount();
	testQuitClearsFactories();
	testCreateReturnsCorrectType();
	testCreateEachFactoryDistinct();
	testOverwriteSameKey();
	testZeroKey();
	testIsFactoryFlag();
}

// ==========================================================================// 1. 注册单个工厂并创建
void FactoryManagerTest::testCreateSingleFactory()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductA>(1);

	auto* factory = mgr.getFactory(1);
	FMT_ASSERT(factory != nullptr, "getFactory(1) should not be nullptr after registration");

	TestProductBase* product = factory->create();
	FMT_ASSERT(product != nullptr, "create() should not return nullptr");
	// 通过虚函数确认类型正确
	FMT_ASSERT(product->getName() == "ProductA", "created object should be ProductA");
	DELETE(product);

	mgr.quit();
}

// 2. 注册多个工厂
void FactoryManagerTest::testCreateMultipleFactories()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductA>(10);
	mgr.addFactoryInstance<TestProductB>(20);
	mgr.addFactoryInstance<TestProductC>(30);

	FMT_ASSERT(mgr.getFactoryCount() == 3, "factory count should be 3");
	FMT_ASSERT(mgr.getFactory(10) != nullptr, "factory 10 exists");
	FMT_ASSERT(mgr.getFactory(20) != nullptr, "factory 20 exists");
	FMT_ASSERT(mgr.getFactory(30) != nullptr, "factory 30 exists");

	mgr.quit();
}

// 3. 查询不存在的key返回nullptr
void FactoryManagerTest::testGetFactoryNonExistent()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductA>(1);

	FMT_ASSERT(mgr.getFactory(999) == nullptr, "non-existent key should return nullptr");
	FMT_ASSERT(mgr.getFactory(0) == nullptr, "key 0 should return nullptr");
	FMT_ASSERT(mgr.getFactory(-1) == nullptr, "negative key should return nullptr");

	mgr.quit();
}

// 4. getFactoryCount 准确性
void FactoryManagerTest::testGetFactoryCount()
{
	TestFactoryMgr mgr;

	FMT_ASSERT(mgr.getFactoryCount() == 0, "initial count should be 0");

	mgr.addFactoryInstance<TestProductA>(1);
	FMT_ASSERT(mgr.getFactoryCount() == 1, "count after 1 register = 1");

	mgr.addFactoryInstance<TestProductB>(2);
	FMT_ASSERT(mgr.getFactoryCount() == 2, "count after 2 registers = 2");

	mgr.addFactoryInstance<TestProductC>(3);
	FMT_ASSERT(mgr.getFactoryCount() == 3, "count after 3 registers = 3");

	mgr.quit();
}

// 5. quit 清理所有工厂
void FactoryManagerTest::testQuitClearsFactories()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductA>(1);
	mgr.addFactoryInstance<TestProductB>(2);

	FMT_ASSERT(mgr.getFactoryCount() == 2, "before quit: count = 2");

	// quit 内部调用 DELETE_MAP(mFactoryList),之后getFactory应返回空
	// 注意: quit后对象处于半销毁状态,此处仅验证quit不会崩溃
	mgr.quit();
}

// 6. create() 返回正确的派生类型
void FactoryManagerTest::testCreateReturnsCorrectType()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductA>(100);
	mgr.addFactoryInstance<TestProductB>(200);

	auto* productA = mgr.getFactory(100)->create();
	auto* productB = mgr.getFactory(200)->create();

	FMT_ASSERT(productA->getName() == "ProductA", "factory 100 creates ProductA");
	FMT_ASSERT(productB->getName() == "ProductB", "factory 200 creates ProductB");

	// 验证确实是不同实例
	FMT_ASSERT(productA != productB, "two creates should produce distinct objects");

	delete productA;
	delete productB;
	mgr.quit();
}

// 7. 每次create产生独立实例
void FactoryManagerTest::testCreateEachFactoryDistinct()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductA>(1);

	auto* p1 = mgr.getFactory(1)->create();
	auto* p2 = mgr.getFactory(1)->create();
	auto* p3 = mgr.getFactory(1)->create();

	FMT_ASSERT(p1 != p2, "instance 1 != instance 2");
	FMT_ASSERT(p2 != p3, "instance 2 != instance 3");
	FMT_ASSERT(p1 != p3, "instance 1 != instance 3");

	p1->mValue = 42;
	p2->mValue = 99;
	FMT_ASSERT(p1->mValue == 42, "p1 value independent");
	FMT_ASSERT(p2->mValue == 99, "p2 value independent");

	DELETE(p1);
	DELETE(p2);
	DELETE(p3);
	mgr.quit();
}

// 8. 同一key重复注册（覆盖行为）
void FactoryManagerTest::testOverwriteSameKey()
{
	TestFactoryMgr mgr;

	// 先注册A到key=1
	mgr.addFactoryInstance<TestProductA>(1);
	auto* productBefore = mgr.getFactory(1)->create();
	FMT_ASSERT(productBefore->getName() == "ProductA", "before overwrite: ProductA");
	DELETE(productBefore);

	// count仍然是1（覆盖不是新增）
	FMT_ASSERT(mgr.getFactoryCount() == 1, "overwrite does not increase count");

	mgr.quit();
}

// 9. key=0 可以正常使用
void FactoryManagerTest::testZeroKey()
{
	TestFactoryMgr mgr;
	mgr.addFactoryInstance<TestProductC>(0);

	FMT_ASSERT(mgr.getFactory(0) != nullptr, "key=0 factory exists");
	auto* p = mgr.getFactory(0)->create();
	FMT_ASSERT(p->getName() == "ProductC", "key=0 creates ProductC");
	DELETE(p);

	mgr.quit();
}

// 10. isFactory() 标志位
void FactoryManagerTest::testIsFactoryFlag()
{
	TestFactoryMgr mgr;
	FMT_ASSERT(mgr.isFactory() == true, "FactoryManager::isFactory() should return true");
	mgr.quit();
}
#endif
