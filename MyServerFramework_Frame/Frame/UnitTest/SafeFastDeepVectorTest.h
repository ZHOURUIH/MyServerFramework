#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API SafeFastDeepVectorTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testSv0IntDefaultState();
	static void testSv0IntAdd();
	static void testSv0IntAddMultiple();
	static void testSv0IntAddAfterClear();
	static void testSv0IntRemoveExisting();
	static void testSv0IntRemoveFirst();
	static void testSv0IntRemoveLast();
	static void testSv0IntRemoveNonexistent();
	static void testSv0IntRemoveDuplicates();
	static void testSv0IntRemoveFromEmpty();
	static void testSv0IntRemoveAll();
	static void testSv0IntForeachBasic();
	static void testSv0IntForeachNested();
	static void testSv0IntForeachRemoveDuringIteration();
	static void testSv0IntClear();
	static void testSv0IntClearEmpty();
	static void testSv0IntClearAndReadd();
	static void testSv0IntAddRemoveInterleaved();
	static void testSv0IntLargeVolume();
	static void testSv0IntStress();
	static void testSv0StringAdd();
	static void testSv0StringRemove();
	static void testSv0StringClear();
	static void testSv0FloatAddRemove();
#endif
};
