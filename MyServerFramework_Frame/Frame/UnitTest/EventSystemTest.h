#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API EventSystemTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testEventSystemUnlistenInCallbackSkipNext();
	static void testEventInfoResetProperty();
	static void testPushAndListenEvent();
	static void testUnlistenEvent();
	static void testRemoveCharacterEvent();
	static void testGlobalEventListen();
	static void testEventProcessCallback();
	static void testMultipleListeners();
	static void testEventTypeIsolation();
	static void testLocalUnlistenSelfInCallback();
	static void testGlobalUnlistenSelfInCallback();
	static void testUnlistenPreviousInCallback();
	static void testUnlistenNextInCallback();
	static void testAddListenerInCallback();
	static void testNestedSameEventPush();
	static void testRemoveCharacterEventInCallback();
	static void testDuplicateListenUnlistenSelf();
	static void testGlobalAndLocalDispatch();
	static void testLocalUnlistenGlobalBeforeGlobalDispatch();
	static void testLocalUnlistenSelfAlsoRemovesGlobal();
	static void testGlobalAddListenerInCallback();
	static void testRemoveCharacterEventKeepsOtherScopes();
	static void testRemoveCharacterEventAfterNullSlotInCallback();
	static void testEventCharacterGUID();
	static void testNoDestroyedEventInfoProfileOnNormalMutations();
	static void testExtended();
	static void testNestedEventPush();
	static void testMultipleSameEvents();
	static void testEventTypeIsolation_Strict();
	static void testDoubleListen_SameEvent();
	static void testUnlistenThenPushAgain();
	static void testGlobalVsCharacterScope();
	static void testHighFrequencyMixedEvents();
	static void testPushWithNoListeners();
	static void testDataIntegrityAcrossTypes();
#endif
};