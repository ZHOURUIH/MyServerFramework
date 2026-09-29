#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API StackTest 
{
#ifdef WINDOWS
public: 
	static void test(); 
private:
	static void testStack_PushSize_Int();
	static void testStack_PushSize_String();
	static void testStack_PushMany();
	static void testStack_PopOrder_Int();
	static void testStack_PopOrder_String();
	static void testStack_PopOrder_Float();
	static void testStack_PopLarge();
	static void testStack_Top_Int();
	static void testStack_Top_AfterPush();
	static void testStack_Top_AfterPop();
	static void testStack_Top_Modify();
	static void testStack_Top_String();
	static void testStack_SizeTracking();
	static void testStack_SizeTracking_PushPopMix();
	static void testStack_BracketMatch();
	static void testStack_BracketMatch_Nested();
	static void testStack_DirectoryNavigation();
	static void testStack_Reverse();
	static void testStack_AlternatePushPop();
	static void testStack_LLong();
	static void testStack_Float_Precision();
	static void testStack_Vector2();
	static void testStack_SingleElement();
	static void testStack_SingleElement_Reuse();
	static void testStack_PostfixCalculator();
	static void testStack_PostfixCalculator_Complex();
	static void testStack_DepthFirstTraversal();
	static void testStack_CallFrameSimulation();
	static void testStack_StressTest();
	static void testStack_StressAlternate();
	static void testSTLBasic();
#endif
};