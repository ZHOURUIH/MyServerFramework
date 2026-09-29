#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API SingletonTest 
{
#ifdef WINDOWS
 
public: 
	static void test(); 
private:
	static void testSingleton_GetAfterConstruct_A();
	static void testSingleton_GetAfterConstruct_B();
	static void testSingleton_GetAfterConstruct_C();
	static void testSingleton_NullAfterDestruct_A();
	static void testSingleton_NullAfterDestruct_B();
	static void testSingleton_ModifyViaPtr();
	static void testSingleton_ModifyViaPtr_B();
	static void testSingleton_TypeIndependence();
	static void testSingleton_NullBeforeConstruct();
	static void testSingleton_FirstConstructorWins();
	static void testSingleton_ConstructDestructCycle();
	static void testSingleton_CallMemberFunctions();
	static void testSingleton_PtrEquality();
#endif
};