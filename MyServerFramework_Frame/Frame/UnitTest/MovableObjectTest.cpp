#ifdef WINDOWS
#include "FrameHeader.h"

// ─── Assertion macros ───
#define MOE_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("MovableObjectExtTest FAIL: ") + (msg)); }

// =====================================================================
// MovableObject — 位置测试
// =====================================================================

void MovableObjectTest::testMoePositionDefault()
{
	MovableObject mo;
	const Vector3& pos = mo.getPosition();
	MOE_ASSERT(pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f, "position default zero");
}

void MovableObjectTest::testMoeSetPositionPositive()
{
	MovableObject mo;
	mo.setPosition(Vector3(10.0f, 20.0f, 30.0f));
	const Vector3& pos = mo.getPosition();
	MOE_ASSERT(pos.x == 10.0f && pos.y == 20.0f && pos.z == 30.0f, "position positive");
}

void MovableObjectTest::testMoeSetPositionNegative()
{
	MovableObject mo;
	mo.setPosition(Vector3(-5.0f, -10.0f, -15.0f));
	const Vector3& pos = mo.getPosition();
	MOE_ASSERT(pos.x == -5.0f && pos.y == -10.0f && pos.z == -15.0f, "position negative");
}

void MovableObjectTest::testMoeSetPositionZero()
{
	MovableObject mo;
	mo.setPosition(Vector3(1.0f, 2.0f, 3.0f));
	mo.setPosition(Vector3::ZERO);
	const Vector3& pos = mo.getPosition();
	MOE_ASSERT(pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f, "position reset zero");
}

void MovableObjectTest::testMoeSetPositionMultiple()
{
	MovableObject mo;
	for (int i = 0; i < 10; ++i)
	{
		mo.setPosition(Vector3((float)i, (float)(i * 2), (float)(i * 3)));
		const Vector3& pos = mo.getPosition();
		MOE_ASSERT(pos.x == (float)i && pos.y == (float)(i * 2) && pos.z == (float)(i * 3), "position sequential");
	}
}

void MovableObjectTest::testMoeSetPositionLargeValues()
{
	MovableObject mo;
	mo.setPosition(Vector3(1e6f, -1e6f, 1e-6f));
	const Vector3& pos = mo.getPosition();
	MOE_ASSERT(pos.x == 1e6f && pos.y == -1e6f && pos.z == 1e-6f, "position large values");
}

// =====================================================================
// MovableObject — LastPosition 测试
// =====================================================================

void MovableObjectTest::testMoeHasLastPositionDefault()
{
	MovableObject mo;
	MOE_ASSERT(!mo.hasLastPosition(), "hasLastPosition default false");
}

void MovableObjectTest::testMoeLastPositionDefault()
{
	MovableObject mo;
	const Vector3& last = mo.getLastPosition();
	MOE_ASSERT(last.x == 0.0f && last.y == 0.0f && last.z == 0.0f, "lastPosition default zero");
}

void MovableObjectTest::testMoeIsMovedDefault()
{
	MovableObject mo;
	MOE_ASSERT(!mo.isMoved(), "isMoved default false");
}

// =====================================================================
// MovableObject — Destroy 回调测试
// =====================================================================

static int moeCallbackCounter = 0;
void MovableObjectTest::moeTestCallback(MovableObject* obj, void* userData)
{
	++moeCallbackCounter;
	*static_cast<int*>(userData) += 1;
}

void MovableObjectTest::testMoeCallbackAddOnce()
{
	MovableObject mo;
	int flag = 0;
	mo.addDestroyCallback(moeTestCallback, &flag);
	mo.destroy();
	MOE_ASSERT(flag == 1, "callback add once triggered");
}

void MovableObjectTest::testMoeCallbackAddMultiple()
{
	MovableObject mo;
	int flag1 = 0, flag2 = 0, flag3 = 0;
	mo.addDestroyCallback(moeTestCallback, &flag1);
	mo.addDestroyCallback(moeTestCallback, &flag2);
	mo.addDestroyCallback(moeTestCallback, &flag3);
	mo.destroy();
	MOE_ASSERT(flag1 == 1 && flag2 == 1 && flag3 == 1, "callback add multiple all triggered");
}

void MovableObjectTest::testMoeCallbackAddAndRemove()
{
	MovableObject mo;
	int flag = 0;
	mo.addDestroyCallback(moeTestCallback, &flag);
	mo.removeDestroyCallback(moeTestCallback, &flag);
	mo.destroy();
	MOE_ASSERT(flag == 0, "callback add+remove not triggered");
}

void MovableObjectTest::testMoeCallbackAddRemoveDifferentOrder()
{
	MovableObject mo;
	int flags[4] = {};
	for (int i = 0; i < 4; ++i)
	{
		mo.addDestroyCallback(moeTestCallback, &flags[i]);
	}
	// 移除中间两个
	mo.removeDestroyCallback(moeTestCallback, &flags[1]);
	mo.removeDestroyCallback(moeTestCallback, &flags[2]);
	mo.destroy();
	MOE_ASSERT(flags[0] == 1, "callback add remove mid[0] triggered");
	MOE_ASSERT(flags[1] == 0, "callback add remove mid[1] NOT triggered");
	MOE_ASSERT(flags[2] == 0, "callback add remove mid[2] NOT triggered");
	MOE_ASSERT(flags[3] == 1, "callback add remove mid[3] triggered");
}

void MovableObjectTest::testMoeCallbackRemoveAll()
{
	MovableObject mo;
	int flags[3] = {};
	for (int i = 0; i < 3; ++i)
	{
		mo.addDestroyCallback(moeTestCallback, &flags[i]);
		mo.removeDestroyCallback(moeTestCallback, &flags[i]);
	}
	mo.destroy();
	for (int i = 0; i < 3; ++i)
	{
		MOE_ASSERT(flags[i] == 0, "callback remove all not triggered");
	}
}

void MovableObjectTest::testMoeCallbackAddSameTwice()
{
	MovableObject mo;
	int flag = 0;
	mo.addDestroyCallback(moeTestCallback, &flag);
	mo.addDestroyCallback(moeTestCallback, &flag); // same callback+data twice
	mo.destroy();
	MOE_ASSERT(flag == 2, "callback added twice triggered twice");
}

void MovableObjectTest::testMoeCallbackDestroyTwice()
{
	MovableObject mo;
	int flag = 0;
	mo.addDestroyCallback(moeTestCallback, &flag);
	mo.destroy(); // first: trigger
	MOE_ASSERT(flag == 1, "first destroy");
	flag = 0;
	mo.destroy(); // second: list already cleared
	MOE_ASSERT(flag == 0, "second destroy no trigger");
}

void MovableObjectTest::testMoeCallbackResetProperty()
{
	MovableObject mo;
	int flag = 0;
	mo.addDestroyCallback(moeTestCallback, &flag);
	mo.resetProperty();
	// reset 后 destroy 不应触发回调
	mo.destroy();
	MOE_ASSERT(flag == 0, "callback after resetProperty not triggered");
}

void MovableObjectTest::testMoeCallbackLargeCount()
{
	constexpr int COUNT = 50;
	MovableObject mo;
	int flags[COUNT] = {};
	for (int i = 0; i < COUNT; ++i)
	{
		mo.addDestroyCallback(moeTestCallback, &flags[i]);
	}
	mo.destroy();
	for (int i = 0; i < COUNT; ++i)
	{
		MOE_ASSERT(flags[i] == 1, "callback large count triggered");
	}
}

// =====================================================================
// MovableObject — ResetProperty 测试
// =====================================================================

void MovableObjectTest::testMoeResetPropertyPosition()
{
	MovableObject mo;
	mo.setPosition(Vector3(100.0f, 200.0f, 300.0f));
	mo.resetProperty();
	const Vector3& pos = mo.getPosition();
	MOE_ASSERT(pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f, "resetProperty position zero");
}

void MovableObjectTest::testMoeResetPropertyStateFlags()
{
	MovableObject mo;
	mo.resetProperty();
	MOE_ASSERT(!mo.hasLastPosition(), "resetProperty hasLastPosition false");
	MOE_ASSERT(!mo.isMoved(), "resetProperty isMoved false");
}

void MovableObjectTest::testMoeResetPropertyMultiple()
{
	MovableObject mo;
	for (int cycle = 0; cycle < 5; ++cycle)
	{
		mo.setPosition(Vector3((float)cycle, (float)(cycle * 10), (float)(cycle * 100)));
		mo.resetProperty();
		const Vector3& pos = mo.getPosition();
		MOE_ASSERT(pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f, "resetProperty multiple cycles");
	}
}

// =====================================================================
// MovableObject — 综合场景
// =====================================================================

void MovableObjectTest::testMoePositionUpdateSequence()
{
	MovableObject mo;
	// 模拟位置更新序列
	Vector3 positions[] =
	{
		Vector3(0, 0, 0),
		Vector3(10, 0, 0),
		Vector3(10, 20, 0),
		Vector3(10, 20, 30),
		Vector3(0, 0, 0),
	};
	for (int i = 0; i < 5; ++i)
	{
		mo.setPosition(positions[i]);
		const Vector3& p = mo.getPosition();
		MOE_ASSERT(p.x == positions[i].x && p.y == positions[i].y && p.z == positions[i].z, "position sequence");
	}
}

void MovableObjectTest::testMoeCallbackAddRemoveInterleaved()
{
	MovableObject mo;
	int flags[5] = {};
	auto cb = [](MovableObject* obj, void* userData) { ++(*static_cast<int*>(userData)); };

	for (int cycle = 0; cycle < 3; ++cycle)
	{
		for (int i = 0; i < 5; ++i)
		{
			mo.addDestroyCallback(cb, &flags[i]);
		}
		mo.removeDestroyCallback(cb, &flags[2]);
		mo.destroy();
		MOE_ASSERT(flags[0] == cycle + 1, "interleaved[0]");
		MOE_ASSERT(flags[1] == cycle + 1, "interleaved[1]");
		MOE_ASSERT(flags[2] == 0,     "interleaved[2] not triggered"); // removed each time
		MOE_ASSERT(flags[3] == cycle + 1, "interleaved[3]");
		MOE_ASSERT(flags[4] == cycle + 1, "interleaved[4]");
	}
}

void MovableObjectTest::testMoeMixedOperations()
{
	MovableObject mo;
	// 混合操作不崩溃
	mo.setPosition(Vector3(1, 2, 3));
	mo.resetProperty();
	int flag = 0;
	mo.addDestroyCallback(moeTestCallback, &flag);
	mo.setPosition(Vector3(4, 5, 6));
	mo.removeDestroyCallback(moeTestCallback, &flag);
	mo.destroy();
	MOE_ASSERT(flag == 0, "mixed operations callback not triggered");
	MOE_ASSERT(true, "mixed operations no crash");
}

// =====================================================================
// 主入口
// =====================================================================
void MovableObjectTest::test()
{
	testMoePositionDefault();
	testMoeSetPositionPositive();
	testMoeSetPositionNegative();
	testMoeSetPositionZero();
	testMoeSetPositionMultiple();
	testMoeSetPositionLargeValues();

	testMoeHasLastPositionDefault();
	testMoeLastPositionDefault();
	testMoeIsMovedDefault();

	testMoeCallbackAddOnce();
	testMoeCallbackAddMultiple();
	testMoeCallbackAddAndRemove();
	testMoeCallbackAddRemoveDifferentOrder();
	testMoeCallbackRemoveAll();
	testMoeCallbackAddSameTwice();
	testMoeCallbackDestroyTwice();
	testMoeCallbackResetProperty();
	testMoeCallbackLargeCount();

	testMoeResetPropertyPosition();
	testMoeResetPropertyStateFlags();
	testMoeResetPropertyMultiple();

	testMoePositionUpdateSequence();
	testMoeCallbackAddRemoveInterleaved();
	testMoeMixedOperations();
}
#endif
