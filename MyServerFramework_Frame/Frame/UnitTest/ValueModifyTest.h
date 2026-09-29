#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API ValueModifyTest
{
#ifdef WINDOWS
public:
    static void test();
private:
    static void testValueModifyBasicAdd();
    static void testValueModifyBasicRemove();
    static void testValueModifyIntType();
    static void testValueModifyFloatType();
    static void testValueModifyStringType();
    static void testValueModifyVectorUsage();
    static void testValueModifySimulateApply();
    static void testMapModifyBasicAdd();
    static void testMapModifyBasicRemove();
    static void testMapModifyIntInt();
    static void testMapModifyStringInt();
    static void testMapModifyVectorUsage();
    static void testMapModifySimulateApply();
    static void testMapModifyEraseOnly();
    static void testMapModifyMixedAddRemove();
    static void testValueModifyMixedIntBool();
    static void testValueModifyLargeScale();
    static void testMapModifyLargeScale();
	static void testMapModify();
	static void testValueModify();
#endif
};
