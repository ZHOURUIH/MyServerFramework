#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API CharacterTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testChGuidDefault();
	static void testChGuidPositive();
	static void testChGuidNegative();
	static void testChGuidLarge();
	static void testChGuidZero();
	static void testChGuidSequential();
	static void testChGuidMultipleCharacters();
	static void testChTypeDefault();
	static void testChTypePositive();
	static void testChTypeMax();
	static void testChTypeZero();
	static void testChTypeSequential();
	static void testChGUIDAndType();
	static void testChGUIDAndTypeSwapOrder();
	static void testChResetPropertyGuid();
	static void testChResetPropertyType();
	static void testChResetPropertyBoth();
	static void testChResetPropertyMultiple();
	static void testChResetThenSet();
	static void testChInheritsMovableObjectPosition();
	static void testChInheritsCommandReceiverName();
#endif
};