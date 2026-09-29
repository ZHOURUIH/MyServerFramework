#ifdef WINDOWS
#include "FrameHeader.h"

#define CR_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("CommandReceiverExtTest FAIL: ") + (msg)); }

// =====================================================================
// CommandReceiver — setName / getUTF8Name
// =====================================================================

void CommandReceiverTest::testCrNameDefault()
{
	CommandReceiver cr;
	CR_ASSERT(cr.getUTF8Name().empty(), "name default empty");
}

void CommandReceiverTest::testCrSetNameBasic()
{
	CommandReceiver cr;
	cr.setName("receiver_1");
	CR_ASSERT(cr.getUTF8Name() == "receiver_1", "name basic");
}

void CommandReceiverTest::testCrSetNameWithUnderscore()
{
	CommandReceiver cr;
	cr.setName("my_command_receiver_test");
	CR_ASSERT(cr.getUTF8Name() == "my_command_receiver_test", "name with underscores");
}

void CommandReceiverTest::testCrSetNameWithNumbers()
{
	CommandReceiver cr;
	cr.setName("cmd_123_456");
	CR_ASSERT(cr.getUTF8Name() == "cmd_123_456", "name with numbers");
}

void CommandReceiverTest::testCrSetNameEmpty()
{
	CommandReceiver cr;
	cr.setName("something");
	cr.setName("");
	CR_ASSERT(cr.getUTF8Name().empty(), "name set empty");
}

void CommandReceiverTest::testCrSetNameOverride()
{
	CommandReceiver cr;
	cr.setName("first");
	cr.setName("second");
	cr.setName("third");
	CR_ASSERT(cr.getUTF8Name() == "third", "name override");
}

void CommandReceiverTest::testCrSetNameLong()
{
	CommandReceiver cr;
	string longName;
	for (int i = 0; i < 100; ++i) longName += "x";
	cr.setName(longName);
	CR_ASSERT(cr.getUTF8Name() == longName, "name long (100 chars)");
}

void CommandReceiverTest::testCrSetNameVeryLong()
{
	CommandReceiver cr;
	string longName;
	for (int i = 0; i < 1000; ++i) longName += "abc";
	cr.setName(longName);
	CR_ASSERT(cr.getUTF8Name() == longName, "name very long (3000 chars)");
}

void CommandReceiverTest::testCrSetNameSingleChar()
{
	CommandReceiver cr;
	cr.setName("a");
	CR_ASSERT(cr.getUTF8Name() == "a", "name single char");
}

void CommandReceiverTest::testCrSetNameSpecial()
{
	CommandReceiver cr;
	cr.setName("test-name_with.special~chars!");
	CR_ASSERT(cr.getUTF8Name() == "test-name_with.special~chars!", "name special chars");
}

void CommandReceiverTest::testCrSetNameMultipleCR()
{
	CommandReceiver cr1, cr2, cr3;
	cr1.setName("alpha");
	cr2.setName("beta");
	cr3.setName("gamma");
	CR_ASSERT(cr1.getUTF8Name() == "alpha", "name multiple[0]");
	CR_ASSERT(cr2.getUTF8Name() == "beta", "name multiple[1]");
	CR_ASSERT(cr3.getUTF8Name() == "gamma", "name multiple[2]");
}

// =====================================================================
// CommandReceiver — getPrintName
// =====================================================================

void CommandReceiverTest::testCrPrintNameDefault()
{
	CommandReceiver cr;
	// 默认空名字，getPrintName 需要先 setName
	cr.setName("print_test");
	const string& print = cr.getPrintName();
	CR_ASSERT(!print.empty(), "printName non-empty after setName");
}

void CommandReceiverTest::testCrPrintNameAfterReset()
{
	CommandReceiver cr;
	cr.setName("print_before_reset");
	cr.resetProperty();
	// reset 后名字应该为空
	CR_ASSERT(cr.getUTF8Name().empty(), "printName after reset empty");
}

// =====================================================================
// CommandReceiver — resetProperty
// =====================================================================

void CommandReceiverTest::testCrResetPropertyName()
{
	CommandReceiver cr;
	cr.setName("will_be_reset");
	cr.resetProperty();
	CR_ASSERT(cr.getUTF8Name().empty(), "resetProperty name empty");
}

void CommandReceiverTest::testCrResetPropertyThenSet()
{
	CommandReceiver cr;
	cr.setName("old");
	cr.resetProperty();
	cr.setName("new");
	CR_ASSERT(cr.getUTF8Name() == "new", "resetProperty then setName");
}

void CommandReceiverTest::testCrResetPropertyMultipleTimes()
{
	CommandReceiver cr;
	for (int i = 0; i < 10; ++i)
	{
		cr.setName("cycle_" + IToS(i));
		cr.resetProperty();
		CR_ASSERT(cr.getUTF8Name().empty(), "resetProperty cycle");
	}
}

void CommandReceiverTest::testCrResetPropertyEmptyName()
{
	CommandReceiver cr;
	cr.resetProperty();
	CR_ASSERT(cr.getUTF8Name().empty(), "resetProperty empty name still empty");
}

// =====================================================================
// CommandReceiver — 继承自 ClassObject 的 resetProperty
// =====================================================================

void CommandReceiverTest::testCrClassObjectResetChain()
{
	CommandReceiver cr;
	cr.setName("test");
	cr.resetProperty();
	// ClassObject::resetProperty 也应被调用
	CR_ASSERT(cr.getUTF8Name().empty(), "reset chain via ClassObject");
}

// =====================================================================
// 主入口
// =====================================================================
void CommandReceiverTest::test()
{
	testCrNameDefault();
	testCrSetNameBasic();
	testCrSetNameWithUnderscore();
	testCrSetNameWithNumbers();
	testCrSetNameEmpty();
	testCrSetNameOverride();
	testCrSetNameLong();
	testCrSetNameVeryLong();
	testCrSetNameSingleChar();
	testCrSetNameSpecial();
	testCrSetNameMultipleCR();

	testCrPrintNameDefault();
	testCrPrintNameAfterReset();

	testCrResetPropertyName();
	testCrResetPropertyThenSet();
	testCrResetPropertyMultipleTimes();
	testCrResetPropertyEmptyName();

	testCrClassObjectResetChain();
}
#endif
