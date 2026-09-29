#ifdef WINDOWS
#include "FrameHeader.h"

#define CH_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("CharacterExtTest FAIL: ") + (msg)); }

// =====================================================================
// Character GUID
// =====================================================================

void CharacterTest::testChGuidDefault()
{
	Character ch;
	CH_ASSERT(ch.getGUID() == 0, "GUID default 0");
}

void CharacterTest::testChGuidPositive()
{
	Character ch;
	ch.setGUID(12345);
	CH_ASSERT(ch.getGUID() == 12345, "GUID positive");
}

void CharacterTest::testChGuidNegative()
{
	Character ch;
	ch.setGUID(-1);
	CH_ASSERT(ch.getGUID() == -1, "GUID negative");
}

void CharacterTest::testChGuidLarge()
{
	Character ch;
	ch.setGUID(9223372036854775807LL); // LLONG_MAX safe
	CH_ASSERT(ch.getGUID() == 9223372036854775807LL, "GUID large positive");
}

void CharacterTest::testChGuidZero()
{
	Character ch;
	ch.setGUID(999);
	ch.setGUID(0);
	CH_ASSERT(ch.getGUID() == 0, "GUID reset zero");
}

void CharacterTest::testChGuidSequential()
{
	Character ch;
	for (int i = 0; i < 20; ++i)
	{
		ch.setGUID(i * 10000LL);
		CH_ASSERT(ch.getGUID() == i * 10000LL, "GUID sequential");
	}
}

void CharacterTest::testChGuidMultipleCharacters()
{
	Character ch1, ch2, ch3;
	ch1.setGUID(1);
	ch2.setGUID(2);
	ch3.setGUID(3);
	CH_ASSERT(ch1.getGUID() == 1, "GUID multiple[0]");
	CH_ASSERT(ch2.getGUID() == 2, "GUID multiple[1]");
	CH_ASSERT(ch3.getGUID() == 3, "GUID multiple[2]");
	// modify
	ch2.setGUID(100);
	CH_ASSERT(ch2.getGUID() == 100, "GUID multiple modify");
	CH_ASSERT(ch1.getGUID() == 1, "GUID multiple unchanged[0]");
	CH_ASSERT(ch3.getGUID() == 3, "GUID multiple unchanged[2]");
}

// =====================================================================
// Character Type
// =====================================================================

void CharacterTest::testChTypeDefault()
{
	Character ch;
	CH_ASSERT(ch.getType() == 0, "type default 0");
}

void CharacterTest::testChTypePositive()
{
	Character ch;
	ch.setType(5);
	CH_ASSERT(ch.getType() == 5, "type positive");
}

void CharacterTest::testChTypeMax()
{
	Character ch;
	ch.setType(255);
	CH_ASSERT(ch.getType() == 255, "type max byte");
}

void CharacterTest::testChTypeZero()
{
	Character ch;
	ch.setType(127);
	ch.setType(0);
	CH_ASSERT(ch.getType() == 0, "type reset zero");
}

void CharacterTest::testChTypeSequential()
{
	Character ch;
	for (int i = 0; i <= 10; ++i)
	{
		ch.setType((byte)i);
		CH_ASSERT(ch.getType() == (byte)i, "type sequential");
	}
}

// =====================================================================
// Character GUID + Type 组合
// =====================================================================

void CharacterTest::testChGUIDAndType()
{
	Character ch;
	ch.setGUID(1000);
	ch.setType(3);
	CH_ASSERT(ch.getGUID() == 1000 && ch.getType() == 3, "GUID+type combined");
}

void CharacterTest::testChGUIDAndTypeSwapOrder()
{
	Character ch;
	ch.setType(7);
	ch.setGUID(5000);
	CH_ASSERT(ch.getType() == 7 && ch.getGUID() == 5000, "GUID+type swap order");
}

// =====================================================================
// Character ResetProperty
// =====================================================================

void CharacterTest::testChResetPropertyGuid()
{
	Character ch;
	ch.setGUID(8888);
	ch.resetProperty();
	CH_ASSERT(ch.getGUID() == 0, "resetProperty GUID zero");
}

void CharacterTest::testChResetPropertyType()
{
	Character ch;
	ch.setType(10);
	ch.resetProperty();
	CH_ASSERT(ch.getType() == 0, "resetProperty type zero");
}

void CharacterTest::testChResetPropertyBoth()
{
	Character ch;
	ch.setGUID(7777);
	ch.setType(20);
	ch.resetProperty();
	CH_ASSERT(ch.getGUID() == 0 && ch.getType() == 0, "resetProperty both zero");
}

void CharacterTest::testChResetPropertyMultiple()
{
	Character ch;
	for (int i = 0; i < 5; ++i)
	{
		ch.setGUID(i * 100);
		ch.setType((byte)(i * 10));
		ch.resetProperty();
		CH_ASSERT(ch.getGUID() == 0 && ch.getType() == 0, "resetProperty cycle");
	}
}

void CharacterTest::testChResetThenSet()
{
	Character ch;
	ch.setGUID(100);
	ch.setType(5);
	ch.resetProperty();
	ch.setGUID(200);
	ch.setType(6);
	CH_ASSERT(ch.getGUID() == 200, "reset then set GUID");
	CH_ASSERT(ch.getType() == 6, "reset then set type");
}

// =====================================================================
// Character 继承链
// =====================================================================

void CharacterTest::testChInheritsMovableObjectPosition()
{
	Character ch;
	ch.setPosition(Vector3(1.0f, 2.0f, 3.0f));
	const Vector3& pos = ch.getPosition();
	CH_ASSERT(pos.x == 1.0f && pos.y == 2.0f && pos.z == 3.0f, "inherits position");
}

void CharacterTest::testChInheritsCommandReceiverName()
{
	Character ch;
	ch.setName("test_char");
	CH_ASSERT(ch.getUTF8Name() == "test_char", "inherits name");
}

// =====================================================================
// 主入口
// =====================================================================
void CharacterTest::test()
{
	testChGuidDefault();
	testChGuidPositive();
	testChGuidNegative();
	testChGuidLarge();
	testChGuidZero();
	testChGuidSequential();
	testChGuidMultipleCharacters();

	testChTypeDefault();
	testChTypePositive();
	testChTypeMax();
	testChTypeZero();
	testChTypeSequential();

	testChGUIDAndType();
	testChGUIDAndTypeSwapOrder();

	testChResetPropertyGuid();
	testChResetPropertyType();
	testChResetPropertyBoth();
	testChResetPropertyMultiple();
	testChResetThenSet();

	testChInheritsMovableObjectPosition();
	testChInheritsCommandReceiverName();
}
#endif
