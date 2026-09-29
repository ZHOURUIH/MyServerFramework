#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API DoubleBufferTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testDoubleBuffer_BasicAddAndRead();
	static void testDoubleBuffer_AddVector();
	static void testDoubleBuffer_AddEmptyVector();
	static void testDoubleBuffer_AddCopyString();
	static void testDoubleBuffer_AddCopyEmpty();
	static void testDoubleBuffer_ClearAfterAdd();
	static void testDoubleBuffer_ClearEmpty();
	static void testDoubleBuffer_WriteLimit();
	static void testDoubleBuffer_WriteLimitVector();
	static void testDoubleBuffer_WriteLimitZeroMeansNoLimit();
	static void testDoubleBuffer_MultipleSwaps();
	static void testDoubleBuffer_ReadAfterClear();
	static void testDoubleBuffer_TryCheckAfterRead();
	static void testDoubleBuffer_GetAllDataCountBothBuffers();
	static void testDoubleBuffer_LargeVolume();
	static void testDoubleBuffer_LargeVolumeVector();
	static void testDoubleBuffer_StringLargeVolume();
	static void testDoubleBuffer_AddSingleAfterLimit();
	static void testDoubleBuffer_EnableStackTrace();
	static void testDoubleBuffer_AddBatchThenReadMultipleRounds();
	static void testDoubleBuffer_MixAddModes();
	static void testDoubleBuffer_WriteAfterStartReadBeforeEndRead();
	static void testDoubleBuffer_ZeroLimitWithAddVector();
	static void testDoubleBuffer_RepeatedClear();
	static void testDoubleBuffer_AddAfterClear();
	static void testDoubleBuffer_FloatType();
	static void testDoubleBuffer_NegativeValues();
	static void testDoubleBuffer_LlongType();
	static void testDoubleBuffer_SetWriteLimitChangesMidway();
	static void testDoubleBuffer_AllDataCountConsistency();
	static void testDoubleBuffer_AddVectorLimitExact();
	static void testDoubleBuffer_IntegrationFlow();
	static void testDoubleBuffer_AddVectorMultipleTimes();
	static void testDoubleBuffer_UShortType();
	static void testDoubleBuffer_ByteType();
	static void testDoubleBuffer_VectorStringAddCopyMultiRounds();
	static void testDoubleBuffer_AddVectorAfterLimitReset();
	static void testDoubleBuffer_ClearResetsReadThread();
	static void testDoubleBuffer_DoubleBufferIntVector();
	static void testIntWriteRead();
	static void testIntMultiRound();
	static void testIntSwapEmpty();
	static void testFloatWriteRead();
	static void testFloatMultipleValues();
	static void testStringWriteRead();
	static void testStringMultipleValues();
	static void testStructWriteRead();
	static void testStructMultipleValues();
	static void testWriteSideIsEmpty();
	static void testReadSideClearedAfterEndRead();
	static void testScopeBasicInt();
	static void testScopeBasicStruct();
	static void testScopeEndReadClearsAll();
	static void testScopeMultiRound();
	static void testScopeNoDataReturnsNull();
	static void testScopeLargeCount();
	static void testScopeWriteAfterRead();
	static void testStartReadReturnsNullWhenEmpty();
	static void testWriteMultipleThenSwap();
	static void testNestedWritePreservesOrder();
	static void testScopeEmptyBuffer();
	static void testScopeBasicRead();
	static void testScopeEndReadClears();
	static void testScopeNoDataNoAction();
	static void testScopeIntType();
	static void testScopeFloatType();
	static void testScopeStringType();
	static void testScopeMultipleRounds();
	static void testScopeReadListAccess();
	static void testScopeWriteAfterReadReadScope();
	static void testScopeNullptrWhenEmpty();
	static void testScopeLargeData();
	static void testScopeWriteLimit();
	static void testScopeAllDataCount();
	static void testScopeClear();
#endif
};