#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MovableObject;

class MICRO_LEGEND_FRAME_API MovableObjectTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testMoePositionDefault();
	static void testMoeSetPositionPositive();
	static void testMoeSetPositionNegative();
	static void testMoeSetPositionZero();
	static void testMoeSetPositionMultiple();
	static void testMoeSetPositionLargeValues();
	static void testMoeHasLastPositionDefault();
	static void testMoeLastPositionDefault();
	static void testMoeIsMovedDefault();
	static void moeTestCallback(MovableObject* obj, void* userData);
	static void testMoeCallbackAddOnce();
	static void testMoeCallbackAddMultiple();
	static void testMoeCallbackAddAndRemove();
	static void testMoeCallbackAddRemoveDifferentOrder();
	static void testMoeCallbackRemoveAll();
	static void testMoeCallbackAddSameTwice();
	static void testMoeCallbackDestroyTwice();
	static void testMoeCallbackResetProperty();
	static void testMoeCallbackLargeCount();
	static void testMoeResetPropertyPosition();
	static void testMoeResetPropertyStateFlags();
	static void testMoeResetPropertyMultiple();
	static void testMoePositionUpdateSequence();
	static void testMoeCallbackAddRemoveInterleaved();
	static void testMoeMixedOperations();
#endif
};
