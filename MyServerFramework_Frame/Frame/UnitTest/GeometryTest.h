#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API GeometryTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testRect();
	static void testRect3();
	static void testLine2();
	static void testLine2D();
	static void testLine3();
	static void testTriangle2();
	static void testTriangle3();
	static void testTriangleIntersect();
	static void testTriangleIntersect3();
	static void testMathHelpers();
	static void testVectorAdvanced();
	static void testRect_Extended();
	static void testVectorMathExtended();
	static void testDegreeRadianExtended();
	static void testLerpEdgeCases();
	static void testClampEdgeCases();
	static void testIsZeroAndEqual();
	static void testMinMaxExtended();
	static void testIntersectLine2Basic();
	static void testIntersectLine2Parallel();
	static void testIntersectLine2SamePoint();
	static void testIntersectLineSectionBasic();
	static void testIntersectLineSectionNoIntersect();
	static void testIntersectLineSectionEndpoint();
	static void testGetDistanceToLineOrigin();
	static void testGetDistanceToLineUnit();
	static void testGetDistanceToLineOnLine();
	static void testGetProjectPointBasic();
	static void testGetProjectPointOnLine();
	static void testGetProjectPointPerp();
	static void testGetProjectionBasic();
	static void testGetProjectionZeroVector();
	static void testGetProjectionParallel();
	static void testGetAngleBetweenVec3Same();
	static void testGetAngleBetweenVec3Perp();
	static void testGetAngleBetweenVec2Same();
	static void testGetAngleBetweenVec2Perp();
	static void testRotateVector3AroundYBasic();
	static void testRotateVector3AroundXBasic();
	static void testRotateVector3AroundZBasic();
	static void testResetYLine3();
	static void testGetVectorFromAngleZero();
	static void testGetVectorFromAngle90();
#endif
};