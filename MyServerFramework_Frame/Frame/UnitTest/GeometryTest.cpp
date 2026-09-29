#ifdef WINDOWS
#include "FrameHeader.h"
namespace GeometryTestSupport
{
#define GeometryTestSupport_GEO_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("GeometryTest FAIL: ") + (msg)); } } while(0)
#define GeometryTestSupport_GEO_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("GeometryTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define GeometryTestSupport_GEO_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-4f) { ERROR(string("GeometryTest FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)
}

void GeometryTest::testRect()
{
	using namespace GeometryTestSupport;
	// constructor with 4 floats
	const Rect r1(1.0f, 2.0f, 10.0f, 5.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(r1.x, 1.0f, "Rect x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r1.y, 2.0f, "Rect y=2");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r1.width, 10.0f, "Rect width=10");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r1.height, 5.0f, "Rect height=5");

	// getMin
	const Vector2 minPt = r1.getMin();
	GeometryTestSupport_GEO_ASSERT_FLOAT(minPt.x, 1.0f, "Rect getMin x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(minPt.y, 2.0f, "Rect getMin y=2");

	// getMax
	const Vector2 maxPt = r1.getMax();
	GeometryTestSupport_GEO_ASSERT_FLOAT(maxPt.x, 11.0f, "Rect getMax x=11");
	GeometryTestSupport_GEO_ASSERT_FLOAT(maxPt.y, 7.0f, "Rect getMax y=7");

	// getCenter
	const Vector2 center = r1.getCenter();
	GeometryTestSupport_GEO_ASSERT_FLOAT(center.x, 6.0f, "Rect center x=6");
	GeometryTestSupport_GEO_ASSERT_FLOAT(center.y, 4.5f, "Rect center y=4.5");

	// getSize
	const Vector2 size = r1.getSize();
	GeometryTestSupport_GEO_ASSERT_FLOAT(size.x, 10.0f, "Rect getSize x=10");
	GeometryTestSupport_GEO_ASSERT_FLOAT(size.y, 5.0f, "Rect getSize y=5");

	// getDiagonal: sqrt(10^2 + 5^2) = sqrt(125) ~ 11.1803
	const float diag = r1.getDiagonal();
	GeometryTestSupport_GEO_ASSERT_FLOAT(diag, sqrtf(125.0f), "Rect diagonal");

	// constructor with Vector2 min/size
	const Rect r2(Vector2(3.0f, 4.0f), Vector2(6.0f, 8.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(r2.x, 3.0f, "Rect V2 ctor x=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r2.y, 4.0f, "Rect V2 ctor y=4");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r2.width, 6.0f, "Rect V2 ctor width=6");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r2.height, 8.0f, "Rect V2 ctor height=8");

	// zero-size rect
	const Rect r3(5.0f, 5.0f, 0.0f, 0.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(r3.getDiagonal(), 0.0f, "Rect zero diagonal");
	const Vector2 minMax = r3.getMax();
	GeometryTestSupport_GEO_ASSERT_FLOAT(minMax.x, 5.0f, "Rect zero max.x=min.x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(minMax.y, 5.0f, "Rect zero max.y=min.y");

	// unit square
	const Rect unit(0.0f, 0.0f, 1.0f, 1.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(unit.getDiagonal(), sqrtf(2.0f), "Unit rect diagonal=sqrt(2)");
	GeometryTestSupport_GEO_ASSERT_FLOAT(unit.getCenter().x, 0.5f, "Unit rect center x=0.5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(unit.getCenter().y, 0.5f, "Unit rect center y=0.5");

	// negative coordinates
	const Rect neg(-10.0f, -5.0f, 20.0f, 10.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(neg.getMax().x, 10.0f, "Neg Rect max.x=10");
	GeometryTestSupport_GEO_ASSERT_FLOAT(neg.getMax().y, 5.0f, "Neg Rect max.y=5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(neg.getCenter().x, 0.0f, "Neg Rect center.x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(neg.getCenter().y, 0.0f, "Neg Rect center.y=0");
}

void GeometryTest::testRect3()
{
	using namespace GeometryTestSupport;
	const Rect3 r(Vector3(1.0f, 2.0f, 3.0f), Vector3(4.0f, 6.0f, 8.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(r.mMin.x, 1.0f, "Rect3 mMin x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r.mMin.y, 2.0f, "Rect3 mMin y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r.mMin.z, 3.0f, "Rect3 mMin z");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r.mMax.x, 4.0f, "Rect3 mMax x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r.mMax.y, 6.0f, "Rect3 mMax y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(r.mMax.z, 8.0f, "Rect3 mMax z");

	// getSize
	const Vector3 sz = r.getSize();
	GeometryTestSupport_GEO_ASSERT_FLOAT(sz.x, 3.0f, "Rect3 size x=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(sz.y, 4.0f, "Rect3 size y=4");
	GeometryTestSupport_GEO_ASSERT_FLOAT(sz.z, 5.0f, "Rect3 size z=5");

	// getCenter
	const Vector3 ctr = r.getCenter();
	GeometryTestSupport_GEO_ASSERT_FLOAT(ctr.x, 2.5f, "Rect3 center x=2.5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ctr.y, 4.0f, "Rect3 center y=4");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ctr.z, 5.5f, "Rect3 center z=5.5");

	// getDiagonalIgnoreY: sqrt(3^2 + 5^2) = sqrt(34) ~ 5.831
	// Note: resetY sets y to 0 before computing length
	const float diagXZ = r.getDiagonalIgnoreY();
	GeometryTestSupport_GEO_ASSERT_FLOAT(diagXZ, sqrtf(34.0f), "Rect3 diag ignore Y");

	// unit cube
	const Rect3 unit(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(unit.getSize().x, 1.0f, "Unit Rect3 size x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(unit.getCenter().x, 0.5f, "Unit Rect3 center x=0.5");

	// zero volume
	const Rect3 point(Vector3(5.0f, 5.0f, 5.0f), Vector3(5.0f, 5.0f, 5.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(point.getSize().x, 0.0f, "Point Rect3 size=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(point.getDiagonalIgnoreY(), 0.0f, "Point Rect3 diag=0");
}

void GeometryTest::testLine2()
{
	using namespace GeometryTestSupport;
	const Line2 l(Vector2(1.0f, 2.0f), Vector2(4.0f, 6.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mStart.x, 1.0f, "Line2 start x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mStart.y, 2.0f, "Line2 start y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mEnd.x, 4.0f, "Line2 end x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mEnd.y, 6.0f, "Line2 end y");

	// Compute direction manually
	const float dx = l.mEnd.x - l.mStart.x; // 3
	const float dy = l.mEnd.y - l.mStart.y; // 4
	GeometryTestSupport_GEO_ASSERT_FLOAT(dx, 3.0f, "Line2 dx=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(dy, 4.0f, "Line2 dy=4");
	const float len = sqrtf(dx * dx + dy * dy);
	GeometryTestSupport_GEO_ASSERT_FLOAT(len, 5.0f, "Line2 length=5 (3-4-5 triangle)");

	// Midpoint
	const float midX = (l.mStart.x + l.mEnd.x) * 0.5f;
	const float midY = (l.mStart.y + l.mEnd.y) * 0.5f;
	GeometryTestSupport_GEO_ASSERT_FLOAT(midX, 2.5f, "Line2 midX=2.5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(midY, 4.0f, "Line2 midY=4");

	// Horizontal line
	const Line2 h(Vector2(0.0f, 0.0f), Vector2(5.0f, 0.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(h.mEnd.y - h.mStart.y, 0.0f, "Line2 horizontal dy=0");

	// Vertical line
	const Line2 v(Vector2(3.0f, 1.0f), Vector2(3.0f, 7.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(v.mEnd.x - v.mStart.x, 0.0f, "Line2 vertical dx=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(v.mEnd.y - v.mStart.y, 6.0f, "Line2 vertical dy=6");
}

void GeometryTest::testLine2D()
{
	using namespace GeometryTestSupport;
	// Slope 1 line: (0,0) -> (5,5)  -> k=1, b=0
	Line2D diag(Vector2(0.0f, 0.0f), Vector2(5.0f, 5.0f));
	GeometryTestSupport_GEO_ASSERT(diag.mHasK, "Line2D diag hasK");
	GeometryTestSupport_GEO_ASSERT_FLOAT(diag.mK, 1.0f, "Line2D diag k=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(diag.mB, 0.0f, "Line2D diag b=0");

	// getPointYOnLine at x=3 -> y=3
	float y = 0.0f;
	GeometryTestSupport_GEO_ASSERT(diag.getPointYOnLine(3.0f, y), "Line2D diag getPointY returns true");
	GeometryTestSupport_GEO_ASSERT_FLOAT(y, 3.0f, "Line2D diag y@x=3 is 3");

	// Horizontal line: (0,2) -> (4,2) -> k=0, b=2
	Line2D horiz(Vector2(0.0f, 2.0f), Vector2(4.0f, 2.0f));
	GeometryTestSupport_GEO_ASSERT(horiz.mHasK, "Line2D horiz hasK");
	GeometryTestSupport_GEO_ASSERT_FLOAT(horiz.mK, 0.0f, "Line2D horiz k=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(horiz.mB, 2.0f, "Line2D horiz b=2");

	float yH = 0.0f;
	GeometryTestSupport_GEO_ASSERT(horiz.getPointYOnLine(10.0f, yH), "Line2D horiz getPointY returns true");
	GeometryTestSupport_GEO_ASSERT_FLOAT(yH, 2.0f, "Line2D horiz y@any x=2");

	// Vertical line: (3,0) -> (3,5) -> no slope
	Line2D vert(Vector2(3.0f, 0.0f), Vector2(3.0f, 5.0f));
	GeometryTestSupport_GEO_ASSERT(!vert.mHasK, "Line2D vert no hasK");

	float yV = 999.0f;
	GeometryTestSupport_GEO_ASSERT(!vert.getPointYOnLine(3.0f, yV), "Line2D vert getPointY returns false");
	GeometryTestSupport_GEO_ASSERT_FLOAT(yV, 0.0f, "Line2D vert y=0 when no slope");

	// Negative slope: (0,4) -> (4,0) -> k=-1, b=4
	Line2D negSlope(Vector2(0.0f, 4.0f), Vector2(4.0f, 0.0f));
	GeometryTestSupport_GEO_ASSERT(negSlope.mHasK, "Line2D neg hasK");
	GeometryTestSupport_GEO_ASSERT_FLOAT(negSlope.mK, -1.0f, "Line2D neg k=-1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(negSlope.mB, 4.0f, "Line2D neg b=4");
	float yNeg = 0.0f;
	GeometryTestSupport_GEO_ASSERT(negSlope.getPointYOnLine(2.0f, yNeg), "Line2D neg getPointY true");
	GeometryTestSupport_GEO_ASSERT_FLOAT(yNeg, 2.0f, "Line2D neg y@x=2 is 2");

	// Fractional slope: (0,0) -> (2,1) -> k=0.5
	Line2D half(Vector2(0.0f, 0.0f), Vector2(2.0f, 1.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(half.mK, 0.5f, "Line2D half k=0.5");
	float yHalf = 0.0f;
	half.getPointYOnLine(4.0f, yHalf);
	GeometryTestSupport_GEO_ASSERT_FLOAT(yHalf, 2.0f, "Line2D half y@x=4 is 2");
}

void GeometryTest::testLine3()
{
	using namespace GeometryTestSupport;
	const Line3 l(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 2.0f, 3.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mStart.x, 0.0f, "Line3 start x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mStart.y, 0.0f, "Line3 start y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mStart.z, 0.0f, "Line3 start z");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mEnd.x, 1.0f, "Line3 end x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mEnd.y, 2.0f, "Line3 end y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(l.mEnd.z, 3.0f, "Line3 end z");

	// toLine2IgnoreY: project onto XZ plane -> (0,0) -> (1,3)
	const Line2D xz = l.toLine2IgnoreY();
	GeometryTestSupport_GEO_ASSERT_FLOAT(xz.mStart.x, 0.0f, "L3 toLine2IgnoreY start x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(xz.mStart.y, 0.0f, "L3 toLine2IgnoreY start y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(xz.mEnd.x, 1.0f, "L3 toLine2IgnoreY end x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(xz.mEnd.y, 3.0f, "L3 toLine2IgnoreY end y=3");

	// toLine2IgnoreX: project onto ZY plane -> (0,0) -> (3,2)
	const Line2D zy = l.toLine2IgnoreX();
	GeometryTestSupport_GEO_ASSERT_FLOAT(zy.mStart.x, 0.0f, "L3 toLine2IgnoreX start x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(zy.mEnd.x, 3.0f, "L3 toLine2IgnoreX end x=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(zy.mEnd.y, 2.0f, "L3 toLine2IgnoreX end y=2");

	// 3D direction vector
	const float dx = l.mEnd.x - l.mStart.x;
	const float dy = l.mEnd.y - l.mStart.y;
	const float dz = l.mEnd.z - l.mStart.z;
	const float len = sqrtf(dx*dx + dy*dy + dz*dz);
	GeometryTestSupport_GEO_ASSERT_FLOAT(len, sqrtf(14.0f), "Line3 length=sqrt(14)");
}

void GeometryTest::testTriangle2()
{
	using namespace GeometryTestSupport;
	const Triangle2 t(
		Vector2(0.0f, 0.0f),
		Vector2(3.0f, 0.0f),
		Vector2(0.0f, 4.0f));

	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint0.x, 0.0f, "Tri2 p0 x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint0.y, 0.0f, "Tri2 p0 y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint1.x, 3.0f, "Tri2 p1 x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint1.y, 0.0f, "Tri2 p1 y");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint2.x, 0.0f, "Tri2 p2 x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint2.y, 4.0f, "Tri2 p2 y");

	// Compute area using cross product: area = 0.5 * |e01 x e02|
	// e01 = (3,0), e02 = (0,4), cross = 3*4 - 0*0 = 12, area = 6
	const float cx = (t.mPoint1.x - t.mPoint0.x) * (t.mPoint2.y - t.mPoint0.y)
					- (t.mPoint1.y - t.mPoint0.y) * (t.mPoint2.x - t.mPoint0.x);
	GeometryTestSupport_GEO_ASSERT_FLOAT(fabsf(cx) * 0.5f, 6.0f, "Tri2 area=6 (3-4-5 right triangle)");

	// Equilateral triangle
	const Triangle2 eq(
		Vector2(0.0f, 0.0f),
		Vector2(2.0f, 0.0f),
		Vector2(1.0f, sqrtf(3.0f)));
	// area = sqrt(3) ~ 1.732
	const float cx2 = (eq.mPoint1.x - eq.mPoint0.x) * (eq.mPoint2.y - eq.mPoint0.y)
					 - (eq.mPoint1.y - eq.mPoint0.y) * (eq.mPoint2.x - eq.mPoint0.x);
	GeometryTestSupport_GEO_ASSERT_FLOAT(fabsf(cx2) * 0.5f, sqrtf(3.0f), "Tri2 equilateral area=sqrt(3)");

	// default ctor
	const Triangle2 def;
	GeometryTestSupport_GEO_ASSERT_FLOAT(def.mPoint0.x, 0.0f, "Tri2 default p0 x=0");
}

void GeometryTest::testTriangle3()
{
	using namespace GeometryTestSupport;
	const Triangle3 t(
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 1.0f, 0.0f));

	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint0.x, 0.0f, "Tri3 p0 x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint1.x, 1.0f, "Tri3 p1 x");
	GeometryTestSupport_GEO_ASSERT_FLOAT(t.mPoint2.y, 1.0f, "Tri3 p2 y");

	// Normal via cross product: (1,0,0) x (0,1,0) = (0,0,1)
	const float e01x = t.mPoint1.x - t.mPoint0.x;
	const float e01y = t.mPoint1.y - t.mPoint0.y;
	const float e01z = t.mPoint1.z - t.mPoint0.z;
	const float e02x = t.mPoint2.x - t.mPoint0.x;
	const float e02y = t.mPoint2.y - t.mPoint0.y;
	const float e02z = t.mPoint2.z - t.mPoint0.z;
	const float normX = e01y * e02z - e01z * e02y;
	const float normY = e01z * e02x - e01x * e02z;
	const float normZ = e01x * e02y - e01y * e02x;
	GeometryTestSupport_GEO_ASSERT_FLOAT(normX, 0.0f, "Tri3 normal x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(normY, 0.0f, "Tri3 normal y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(normZ, 1.0f, "Tri3 normal z=1");

	// Area = 0.5 * |normal| = 0.5
	const float normLen = sqrtf(normX*normX + normY*normY + normZ*normZ);
	GeometryTestSupport_GEO_ASSERT_FLOAT(normLen * 0.5f, 0.5f, "Tri3 area=0.5");

	// 3D right triangle: (0,0,0)-(3,0,0)-(0,0,4): area = 0.5*3*4=6
	const Triangle3 t2(
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(3.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f, 4.0f));
	const float e1x = t2.mPoint1.x - t2.mPoint0.x;
	const float e1z = t2.mPoint1.z - t2.mPoint0.z;
	const float e2x = t2.mPoint2.x - t2.mPoint0.x;
	const float e2z = t2.mPoint2.z - t2.mPoint0.z;
	// cross = (0, e1x*e2z - e1z*e2x, 0) wait: XZ plane cross
	const float ny = e1x * e2z - e1z * e2x; // = 3*4 - 0*0 = 12
	GeometryTestSupport_GEO_ASSERT_FLOAT(fabsf(ny) * 0.5f, 6.0f, "Tri3 3D right triangle area=6");

	// default ctor
	const Triangle3 def;
	GeometryTestSupport_GEO_ASSERT_FLOAT(def.mPoint0.x, 0.0f, "Tri3 default p0 x=0");
}

void GeometryTest::testTriangleIntersect()
{
	using namespace GeometryTestSupport;
	TriangleIntersect ti;
	ti.mIntersectPoint = Vector2(2.0f, 3.0f);
	ti.mLinePoint0 = Vector2(1.0f, 1.0f);
	ti.mLinePoint1 = Vector2(3.0f, 5.0f);

	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mIntersectPoint.x, 2.0f, "TI intersect x=2");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mIntersectPoint.y, 3.0f, "TI intersect y=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mLinePoint0.x, 1.0f, "TI line p0 x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mLinePoint1.y, 5.0f, "TI line p1 y=5");

	// Verify intersect lies on the line (parametric check)
	const float t_param = (ti.mIntersectPoint.x - ti.mLinePoint0.x)
		/ (ti.mLinePoint1.x - ti.mLinePoint0.x);
	const float expectedY = ti.mLinePoint0.y + t_param * (ti.mLinePoint1.y - ti.mLinePoint0.y);
	GeometryTestSupport_GEO_ASSERT_FLOAT(expectedY, ti.mIntersectPoint.y, "TI point on line");
}

void GeometryTest::testTriangleIntersect3()
{
	using namespace GeometryTestSupport;
	TriangleIntersect3 ti;
	ti.mIntersectPoint = Vector3(1.0f, 2.0f, 3.0f);
	ti.mLinePoint0 = Vector3(0.0f, 0.0f, 0.0f);
	ti.mLinePoint1 = Vector3(2.0f, 4.0f, 6.0f);

	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mIntersectPoint.x, 1.0f, "TI3 intersect x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mIntersectPoint.y, 2.0f, "TI3 intersect y=2");
	GeometryTestSupport_GEO_ASSERT_FLOAT(ti.mIntersectPoint.z, 3.0f, "TI3 intersect z=3");

	// Verify parametric: t=0.5, point = p0 + t*(p1-p0) = (1,2,3)
	const float tx = (ti.mIntersectPoint.x - ti.mLinePoint0.x)
		/ (ti.mLinePoint1.x - ti.mLinePoint0.x);
	const float ty = ti.mLinePoint0.y + tx * (ti.mLinePoint1.y - ti.mLinePoint0.y);
	GeometryTestSupport_GEO_ASSERT_FLOAT(ty, 2.0f, "TI3 point on line y=2");
	const float tz = ti.mLinePoint0.z + tx * (ti.mLinePoint1.z - ti.mLinePoint0.z);
	GeometryTestSupport_GEO_ASSERT_FLOAT(tz, 3.0f, "TI3 point on line z=3");
}

void GeometryTest::testMathHelpers()
{
	using namespace GeometryTestSupport;
	// getLength(float a, float b) = sqrt(a^2 + b^2)
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(3.0f, 4.0f), 5.0f, "getLength(3,4)=5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(0.0f, 0.0f), 0.0f, "getLength(0,0)=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(1.0f, 0.0f), 1.0f, "getLength(1,0)=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(5.0f, 12.0f), 13.0f, "getLength(5,12)=13");

	// Vector3 length
	const Vector3 v(1.0f, 2.0f, 2.0f);
	// length = sqrt(1+4+4) = 3
	const float len = getLength(v);
	GeometryTestSupport_GEO_ASSERT_FLOAT(len, 3.0f, "V3 length=3 (1,2,2)");

	// Vector2 length
	const Vector2 v2(3.0f, 4.0f);
	const float len2 = getLength(v2);
	GeometryTestSupport_GEO_ASSERT_FLOAT(len2, 5.0f, "V2 length=5 (3,4)");

	// dot product (Vector2)
	const Vector2 a(1.0f, 0.0f);
	const Vector2 b(0.0f, 1.0f);
	const float dotAB = MathUtility::dot(a, b);
	GeometryTestSupport_GEO_ASSERT_FLOAT(dotAB, 0.0f, "V2 dot perpendicular=0");

	const Vector2 c(1.0f, 0.0f);
	const Vector2 d(1.0f, 0.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(c, d), 1.0f, "V2 dot parallel=1");

	const Vector2 e(3.0f, 4.0f);
	const Vector2 f(3.0f, 4.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(e, f), 25.0f, "V2 dot self = length^2 = 25");

	// normalize Vector2
	const Vector2 norm2 = normalize(Vector2(3.0f, 4.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm2.x, 0.6f, "norm2 x=0.6");
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm2.y, 0.8f, "norm2 y=0.8");
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(norm2), 1.0f, "norm2 length=1");

	// normalize Vector3
	const Vector3 norm3 = normalize(Vector3(0.0f, 3.0f, 4.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm3.y, 0.6f, "norm3 y=0.6");
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm3.z, 0.8f, "norm3 z=0.8");
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(norm3), 1.0f, "norm3 length=1");

	// cross product Vector3: 框架 cross(v0,v1) 与 Unity 一致，结果与标准右手系反向
	const Vector3 crossVec = MathUtility::cross(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossVec.x, 0.0f, "cross x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossVec.y, 0.0f, "cross y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossVec.z, 1.0f, "cross z=1");

	// resetY: set y to 0
	const Vector3 noY = resetY(Vector3(1.0f, 99.0f, 2.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(noY.x, 1.0f, "resetY x=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(noY.y, 0.0f, "resetY y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(noY.z, 2.0f, "resetY z=2");
}

void GeometryTest::testVectorAdvanced()
{
	using namespace GeometryTestSupport;
	// Distance between two Vector2 points
	const Vector2 p1(1.0f, 1.0f);
	const Vector2 p2(4.0f, 5.0f);
	const Vector2 diff2 = p2 - p1;
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(diff2), 5.0f, "V2 dist(1,1)-(4,5)=5");

	// Distance between two Vector3 points
	const Vector3 q1(0.0f, 0.0f, 0.0f);
	const Vector3 q2(2.0f, 2.0f, 1.0f);
	const Vector3 diff3 = q2 - q1;
	GeometryTestSupport_GEO_ASSERT_FLOAT(getLength(diff3), 3.0f, "V3 dist (0-2,0-2,0-1) = 3");

	// dot product Vector3
	const Vector3 axis(0.0f, 1.0f, 0.0f);
	const Vector3 vec(3.0f, 5.0f, 7.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(axis, vec), 5.0f, "V3 dot Y axis with (3,5,7)=5");

	// cross product
	const Vector3 c2 = MathUtility::cross(Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(c2.x, 0.0f, "cross2 x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(c2.y, 0.0f, "cross2 y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(c2.z, -1.0f, "cross2 z=-1");

	// Perpendicular vectors: dot = 0
	const Vector3 ex(1.0f, 0.0f, 0.0f);
	const Vector3 ey(0.0f, 1.0f, 0.0f);
	const Vector3 ez(0.0f, 0.0f, 1.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(ex, ey), 0.0f, "ex . ey = 0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(ey, ez), 0.0f, "ey . ez = 0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(ex, ez), 0.0f, "ex . ez = 0");

	// Parallel vectors: dot = product of magnitudes
	const Vector3 v1(2.0f, 0.0f, 0.0f);
	const Vector3 v2_b(3.0f, 0.0f, 0.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(v1, v2_b), 6.0f, "parallel V3 dot=6");
}

void GeometryTest::testRect_Extended()
{
	using namespace GeometryTestSupport;
	// Zero-size rect
	const Rect zero(0.0f, 0.0f, 0.0f, 0.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(zero.getDiagonal(), 0.0f, "zero rect diagonal=0");
	const Vector2 zCenter = zero.getCenter();
	GeometryTestSupport_GEO_ASSERT_FLOAT(zCenter.x, 0.0f, "zero rect center x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(zCenter.y, 0.0f, "zero rect center y=0");

	// Square
	const Rect sq(0.0f, 0.0f, 4.0f, 4.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(sq.getDiagonal(), sqrtf(32.0f), "square diagonal=sqrt(32)");
	const Vector2 sqCenter = sq.getCenter();
	GeometryTestSupport_GEO_ASSERT_FLOAT(sqCenter.x, 2.0f, "square center x=2");
	GeometryTestSupport_GEO_ASSERT_FLOAT(sqCenter.y, 2.0f, "square center y=2");

	// Negative origin
	const Rect neg(-5.0f, -3.0f, 10.0f, 6.0f);
	const Vector2 negMin = neg.getMin();
	GeometryTestSupport_GEO_ASSERT_FLOAT(negMin.x, -5.0f, "neg rect min x=-5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(negMin.y, -3.0f, "neg rect min y=-3");
	const Vector2 negMax = neg.getMax();
	GeometryTestSupport_GEO_ASSERT_FLOAT(negMax.x, 5.0f, "neg rect max x=5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(negMax.y, 3.0f, "neg rect max y=3");
	const Vector2 negCenter = neg.getCenter();
	GeometryTestSupport_GEO_ASSERT_FLOAT(negCenter.x, 0.0f, "neg rect center x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(negCenter.y, 0.0f, "neg rect center y=0");

	// Very thin horizontal rect
	const Rect thin(1.0f, 2.0f, 100.0f, 0.01f);
	const float thinDiag = thin.getDiagonal();
	GeometryTestSupport_GEO_ASSERT(thinDiag > 99.9f && thinDiag < 100.1f, "thin rect diagonal ~100");

	// Vector2 constructor
	const Rect v2r(Vector2(2.0f, 3.0f), Vector2(8.0f, 6.0f));
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2r.x, 2.0f, "V2 ctor rect x=2");
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2r.y, 3.0f, "V2 ctor rect y=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2r.width, 8.0f, "V2 ctor rect w=8");
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2r.height, 6.0f, "V2 ctor rect h=6");
	const Vector2 v2rMax = v2r.getMax();
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2rMax.x, 10.0f, "V2 ctor rect max x=10");
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2rMax.y, 9.0f, "V2 ctor rect max y=9");

	// Size
	const Vector2 v2rSize = v2r.getSize();
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2rSize.x, 8.0f, "V2 ctor rect size x=8");
	GeometryTestSupport_GEO_ASSERT_FLOAT(v2rSize.y, 6.0f, "V2 ctor rect size y=6");
}

void GeometryTest::testVectorMathExtended()
{
	using namespace GeometryTestSupport;
	// dot product of zero vector
	const Vector3 zeroV(0.0f, 0.0f, 0.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::dot(zeroV, zeroV), 0.0f, "dot zero=0");

	// dot with self = squared length
	const Vector3 v(3.0f, 4.0f, 0.0f);
	const float dotSelf = MathUtility::dot(v, v);
	GeometryTestSupport_GEO_ASSERT_FLOAT(dotSelf, 25.0f, "dot self = squared length 25");

	// cross of parallel vectors = zero vector
	const Vector3 a(1.0f, 0.0f, 0.0f);
	const Vector3 b(2.0f, 0.0f, 0.0f);
	const Vector3 crossParallel = MathUtility::cross(a, b);
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossParallel.x, 0.0f, "cross parallel x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossParallel.y, 0.0f, "cross parallel y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossParallel.z, 0.0f, "cross parallel z=0");

	// cross of zero vector = zero
	const Vector3 crossZero = MathUtility::cross(zeroV, a);
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossZero.x, 0.0f, "cross zero x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossZero.y, 0.0f, "cross zero y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(crossZero.z, 0.0f, "cross zero z=0");

	// getLength 3D
	const Vector3 v2(1.0f, 2.0f, 2.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getLength(v2), 3.0f, "V3 length (1,2,2)=3");

	// getSquaredLength
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getSquaredLength(v2), 9.0f, "V3 sqLen=9");

	// normalize 3D
	const Vector3 big(0.0f, 0.0f, 5.0f);
	const Vector3 norm = MathUtility::normalize(big);
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm.x, 0.0f, "normalize x=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm.y, 0.0f, "normalize y=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(norm.z, 1.0f, "normalize z=1");

	// getLength 2D float
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getLength(3.0f, 4.0f), 5.0f, "2D length (3,4)=5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getLength(0.0f, 0.0f), 0.0f, "2D length (0,0)=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getLength(5.0f, 0.0f), 5.0f, "2D length (5,0)=5");

	// toVec2
	const Vector3 tv3(7.0f, 8.0f, 9.0f);
	const Vector2 tv2 = MathUtility::toVec2(tv3);
	GeometryTestSupport_GEO_ASSERT_FLOAT(tv2.x, 7.0f, "toVec2 x=7");
	GeometryTestSupport_GEO_ASSERT_FLOAT(tv2.y, 8.0f, "toVec2 y=8");

	// toVec3
	const Vector2 tv2b(11.0f, 22.0f);
	const Vector3 tv3b = MathUtility::toVec3(tv2b);
	GeometryTestSupport_GEO_ASSERT_FLOAT(tv3b.x, 11.0f, "toVec3 x=11");
	GeometryTestSupport_GEO_ASSERT_FLOAT(tv3b.y, 22.0f, "toVec3 y=22");
	GeometryTestSupport_GEO_ASSERT_FLOAT(tv3b.z, 0.0f, "toVec3 z=0");
}

void GeometryTest::testDegreeRadianExtended()
{
	using namespace GeometryTestSupport;
	// 0 deg = 0 rad
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toRadian(0.0f), 0.0f, "0 deg=0 rad");

	// 180 deg = PI rad
	const float pi = 3.14159265358979f;
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toRadian(180.0f), pi, "180 deg=PI");

	// 360 deg = 2*PI rad
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toRadian(360.0f), 2.0f * pi, "360 deg=2PI");

	// 90 deg = PI/2
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toRadian(90.0f), pi * 0.5f, "90 deg=PI/2");

	// 45 deg = PI/4
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toRadian(45.0f), pi * 0.25f, "45 deg=PI/4");

	// -90 deg
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toRadian(-90.0f), -pi * 0.5f, "-90 deg=-PI/2");

	// toDegree round-trip
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toDegree(MathUtility::toRadian(45.0f)), 45.0f, "rtt 45 deg");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toDegree(MathUtility::toRadian(90.0f)), 90.0f, "rtt 90 deg");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toDegree(MathUtility::toRadian(180.0f)), 180.0f, "rtt 180 deg");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::toDegree(MathUtility::toRadian(-45.0f)), -45.0f, "rtt -45 deg");
}

void GeometryTest::testLerpEdgeCases()
{
	using namespace GeometryTestSupport;
	// lerp t=0 -> a, t=1 -> b
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::lerp(0.0f, 10.0f, 0.0f), 0.0f, "lerp t=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::lerp(0.0f, 10.0f, 1.0f), 10.0f, "lerp t=1");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::lerp(0.0f, 10.0f, 0.5f), 5.0f, "lerp t=0.5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::lerp(-10.0f, 10.0f, 0.5f), 0.0f, "lerp neg to pos t=0.5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::lerp(5.0f, 5.0f, 0.5f), 5.0f, "lerp same a=b");

	// inverseLerp
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::inverseLerp(0.0f, 10.0f, 0.0f), 0.0f, "invLerp v=a");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::inverseLerp(0.0f, 10.0f, 10.0f), 1.0f, "invLerp v=b");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::inverseLerp(0.0f, 10.0f, 5.0f), 0.5f, "invLerp v=mid");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::inverseLerp(-5.0f, 5.0f, 0.0f), 0.5f, "invLerp neg range");
}

void GeometryTest::testClampEdgeCases()
{
	using namespace GeometryTestSupport;
	// value below min
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(-5.0f, 0.0f, 10.0f), 0.0f, "clamp below min");
	// value above max
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(15.0f, 0.0f, 10.0f), 10.0f, "clamp above max");
	// value in range
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(5.0f, 0.0f, 10.0f), 5.0f, "clamp in range");
	// value equals min
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(0.0f, 0.0f, 10.0f), 0.0f, "clamp equals min");
	// value equals max
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(10.0f, 0.0f, 10.0f), 10.0f, "clamp equals max");
	// int clamp
	GeometryTestSupport_GEO_ASSERT_EQ(MathUtility::clamp(3, 1, 5), 3, "int clamp in range");
	GeometryTestSupport_GEO_ASSERT_EQ(MathUtility::clamp(0, 1, 5), 1, "int clamp below");
	GeometryTestSupport_GEO_ASSERT_EQ(MathUtility::clamp(9, 1, 5), 5, "int clamp above");
	// negative range
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(-3.0f, -5.0f, -1.0f), -3.0f, "clamp neg range in");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(-8.0f, -5.0f, -1.0f), -5.0f, "clamp neg range below");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::clamp(0.0f, -5.0f, -1.0f), -1.0f, "clamp neg range above");
}

void GeometryTest::testIsZeroAndEqual()
{
	using namespace GeometryTestSupport;
	// isZero
	GeometryTestSupport_GEO_ASSERT(MathUtility::isZero(0.0f), "isZero 0.0f");
	GeometryTestSupport_GEO_ASSERT(MathUtility::isZero(0.00001f), "isZero tiny positive");
	GeometryTestSupport_GEO_ASSERT(MathUtility::isZero(-0.00001f), "isZero tiny negative");
	GeometryTestSupport_GEO_ASSERT(!MathUtility::isZero(0.01f), "isZero 0.01 false");
	GeometryTestSupport_GEO_ASSERT(!MathUtility::isZero(-0.1f), "isZero -0.1 false");

	// isEqual
	GeometryTestSupport_GEO_ASSERT(MathUtility::isEqual(1.0f, 1.0f), "isEqual same");
	GeometryTestSupport_GEO_ASSERT(MathUtility::isEqual(1.0f, 1.000005f), "isEqual tiny diff");
	GeometryTestSupport_GEO_ASSERT(!MathUtility::isEqual(1.0f, 1.1f), "isEqual 0.1 diff");
	GeometryTestSupport_GEO_ASSERT(!MathUtility::isEqual(0.0f, 1.0f), "isEqual 0 vs 1");

	// int isZero
	GeometryTestSupport_GEO_ASSERT(MathUtility::isZero(0.0f), "isZero int 0");
	GeometryTestSupport_GEO_ASSERT(!MathUtility::isZero(1.0f), "isZero int 1 false");
}

void GeometryTest::testMinMaxExtended()
{
	using namespace GeometryTestSupport;
	// Float
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getMin(3.0f, 5.0f), 3.0f, "getMin 3,5=3");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getMin(-2.0f, -5.0f), -5.0f, "getMin -2,-5=-5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getMin(0.0f, 0.0f), 0.0f, "getMin same=0");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getMax(3.0f, 5.0f), 5.0f, "getMax 3,5=5");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getMax(-2.0f, -5.0f), -2.0f, "getMax -2,-5=-2");
	GeometryTestSupport_GEO_ASSERT_FLOAT(MathUtility::getMax(0.0f, 0.0f), 0.0f, "getMax same=0");

	// Int
	GeometryTestSupport_GEO_ASSERT_EQ(MathUtility::getMin(7, 3), 3, "int getMin=3");
	GeometryTestSupport_GEO_ASSERT_EQ(MathUtility::getMax(7, 3), 7, "int getMax=7");

	// Three-argument clamp via min/max
	const float r = MathUtility::getMin(MathUtility::getMax(15.0f, 0.0f), 10.0f);
	GeometryTestSupport_GEO_ASSERT_FLOAT(r, 10.0f, "manual clamp(15,0,10)=10");
}

namespace GeometryExt2TestSupport
{
#define GeometryExt2TestSupport_GEO2_ASSERT(expr) \
    do { if (!(expr)) { ERROR("GeometryExt2Test FAIL: " #expr); } } while(0)
#define GeometryExt2TestSupport_GEO2_NEAR(a, b) (fabsf((a) - (b)) < 1e-3f)
}

void GeometryTest::testIntersectLine2Basic()
{
	using namespace GeometryExt2TestSupport;
	Line2D line0 = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	Line2D line1 = {{5.0f, -5.0f}, {5.0f, 5.0f}};
	Vector2 intersect = {0.0f, 0.0f};
	bool hit = MathUtility::intersectLine2(line0, line1, intersect);
	GeometryExt2TestSupport_GEO2_ASSERT(hit == true);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(intersect.x, 5.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(intersect.y, 0.0f));
}

void GeometryTest::testIntersectLine2Parallel()
{
	using namespace GeometryExt2TestSupport;
	Line2D line0 = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	Line2D line1 = {{0.0f, 1.0f}, {10.0f, 1.0f}};
	Vector2 intersect = {0.0f, 0.0f};
	bool hit = MathUtility::intersectLine2(line0, line1, intersect);
	GeometryExt2TestSupport_GEO2_ASSERT(hit == false);
}

void GeometryTest::testIntersectLine2SamePoint()
{
	using namespace GeometryExt2TestSupport;
	Line2D line0 = {{0.0f, 0.0f}, {4.0f, 4.0f}};
	Line2D line1 = {{0.0f, 4.0f}, {4.0f, 0.0f}};
	Vector2 intersect = {0.0f, 0.0f};
	bool hit = MathUtility::intersectLine2(line0, line1, intersect);
	GeometryExt2TestSupport_GEO2_ASSERT(hit == true);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(intersect.x, 2.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(intersect.y, 2.0f));
}

void GeometryTest::testIntersectLineSectionBasic()
{
	using namespace GeometryExt2TestSupport;
	Line2D line0 = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	Line2D line1 = {{5.0f, -5.0f}, {5.0f, 5.0f}};
	Vector2 intersect = {0.0f, 0.0f};
	bool hit = MathUtility::intersectLineSection(line0, line1, intersect);
	GeometryExt2TestSupport_GEO2_ASSERT(hit == true);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(intersect.x, 5.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(intersect.y, 0.0f));
}

void GeometryTest::testIntersectLineSectionNoIntersect()
{
	using namespace GeometryExt2TestSupport;
	Line2D line0 = {{0.0f, 0.0f}, {4.0f, 0.0f}};
	Line2D line1 = {{6.0f, -1.0f}, {6.0f, 1.0f}};
	Vector2 intersect = {0.0f, 0.0f};
	bool hit = MathUtility::intersectLineSection(line0, line1, intersect);
	GeometryExt2TestSupport_GEO2_ASSERT(hit == false);
}

void GeometryTest::testIntersectLineSectionEndpoint()
{
	using namespace GeometryExt2TestSupport;
	Line2D line0 = {{0.0f, 0.0f}, {5.0f, 0.0f}};
	Line2D line1 = {{5.0f, -3.0f}, {5.0f, 3.0f}};
	Vector2 intersect = {0.0f, 0.0f};
	bool hit = MathUtility::intersectLineSection(line0, line1, intersect, true);
	GeometryExt2TestSupport_GEO2_ASSERT(hit == true);
}

void GeometryTest::testGetDistanceToLineOrigin()
{
	using namespace GeometryExt2TestSupport;
	Line2D line = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	float dist = MathUtility::getDistanceToLine({5.0f, 3.0f}, line);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(dist, 3.0f));
}

void GeometryTest::testGetDistanceToLineUnit()
{
	using namespace GeometryExt2TestSupport;
	Line2D line = {{0.0f, 0.0f}, {0.0f, 10.0f}};
	float dist = MathUtility::getDistanceToLine({4.0f, 5.0f}, line);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(dist, 4.0f));
}

void GeometryTest::testGetDistanceToLineOnLine()
{
	using namespace GeometryExt2TestSupport;
	Line2D line = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	float dist = MathUtility::getDistanceToLine({3.0f, 0.0f}, line);
	GeometryExt2TestSupport_GEO2_ASSERT(dist < 1e-3f);
}

void GeometryTest::testGetProjectPointBasic()
{
	using namespace GeometryExt2TestSupport;
	Line2D line = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	Vector2 proj = MathUtility::getProjectPoint({5.0f, 3.0f}, line);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.x, 5.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.y, 0.0f));
}

void GeometryTest::testGetProjectPointOnLine()
{
	using namespace GeometryExt2TestSupport;
	Line2D line = {{0.0f, 0.0f}, {10.0f, 0.0f}};
	Vector2 proj = MathUtility::getProjectPoint({3.0f, 0.0f}, line);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.x, 3.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.y, 0.0f));
}

void GeometryTest::testGetProjectPointPerp()
{
	using namespace GeometryExt2TestSupport;
	Line2D line = {{0.0f, 0.0f}, {0.0f, 10.0f}};
	Vector2 proj = MathUtility::getProjectPoint({5.0f, 5.0f}, line);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.x, 0.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.y, 5.0f));
}

void GeometryTest::testGetProjectionBasic()
{
	using namespace GeometryExt2TestSupport;
	Vector2 v1 = {3.0f, 4.0f};
	Vector2 v2 = {5.0f, 0.0f};
	Vector2 proj = MathUtility::getProjection(v1, v2);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.x, 3.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.y, 0.0f));
}

void GeometryTest::testGetProjectionZeroVector()
{
	using namespace GeometryExt2TestSupport;
	Vector2 v1 = {3.0f, 4.0f};
	Vector2 v2 = {0.0f, 0.0f};
	Vector2 proj = MathUtility::getProjection(v1, v2);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.x, 0.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.y, 0.0f));
}

void GeometryTest::testGetProjectionParallel()
{
	using namespace GeometryExt2TestSupport;
	Vector2 v1 = {4.0f, 0.0f};
	Vector2 v2 = {2.0f, 0.0f};
	Vector2 proj = MathUtility::getProjection(v1, v2);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.x, 4.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(proj.y, 0.0f));
}

void GeometryTest::testGetAngleBetweenVec3Same()
{
	using namespace GeometryExt2TestSupport;
	float angle = MathUtility::getAngleBetweenVector(
	    Vector3{1.0f, 0.0f, 0.0f},
	    Vector3{1.0f, 0.0f, 0.0f});
	GeometryExt2TestSupport_GEO2_ASSERT(fabsf(angle) < 1e-3f);
}

void GeometryTest::testGetAngleBetweenVec3Perp()
{
	using namespace GeometryExt2TestSupport;
	float angle = MathUtility::getAngleBetweenVector(
	    Vector3{1.0f, 0.0f, 0.0f},
	    Vector3{0.0f, 1.0f, 0.0f});
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(angle, MathUtility::HALF_PI_RADIAN));
}

void GeometryTest::testGetAngleBetweenVec2Same()
{
	using namespace GeometryExt2TestSupport;
	float angle = MathUtility::getAngleBetweenVector(
	    Vector2{1.0f, 0.0f},
	    Vector2{1.0f, 0.0f});
	GeometryExt2TestSupport_GEO2_ASSERT(fabsf(angle) < 1e-3f);
}

void GeometryTest::testGetAngleBetweenVec2Perp()
{
	using namespace GeometryExt2TestSupport;
	float angle = MathUtility::getAngleBetweenVector(
	    Vector2{1.0f, 0.0f},
	    Vector2{0.0f, 1.0f});
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(angle, MathUtility::HALF_PI_RADIAN));
}

void GeometryTest::testRotateVector3AroundYBasic()
{
	using namespace GeometryExt2TestSupport;
	Vector3 v = {1.0f, 0.0f, 0.0f};
	float halfPi = MathUtility::HALF_PI_RADIAN;
	Vector3 r = MathUtility::rotateVector3AroundY(v, halfPi);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(r.y, 0.0f));
}

void GeometryTest::testRotateVector3AroundXBasic()
{
	using namespace GeometryExt2TestSupport;
	Vector3 v = {0.0f, 1.0f, 0.0f};
	float halfPi = MathUtility::HALF_PI_RADIAN;
	Vector3 r = MathUtility::rotateVector3AroundX(v, halfPi);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(r.x, 0.0f));
}

void GeometryTest::testRotateVector3AroundZBasic()
{
	using namespace GeometryExt2TestSupport;
	Vector3 v = {1.0f, 0.0f, 0.0f};
	float halfPi = MathUtility::HALF_PI_RADIAN;
	Vector3 r = MathUtility::rotateVector3AroundZ(v, halfPi);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(r.z, 0.0f));
}

void GeometryTest::testResetYLine3()
{
	using namespace GeometryExt2TestSupport;
	Line3 line = {{1.0f, 5.0f, 2.0f}, {3.0f, 7.0f, 4.0f}};
	Line3 result = MathUtility::resetY(line);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(result.mStart.y, 0.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(result.mEnd.y, 0.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(result.mStart.x, 1.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(result.mEnd.x, 3.0f));
}

void GeometryTest::testGetVectorFromAngleZero()
{
	using namespace GeometryExt2TestSupport;
	Vector3 v = MathUtility::getVectorFromAngle(0.0f);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(v.x, 0.0f));
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(v.z, 1.0f));
}

void GeometryTest::testGetVectorFromAngle90()
{
	using namespace GeometryExt2TestSupport;
	Vector3 v = MathUtility::getVectorFromAngle(MathUtility::HALF_PI_RADIAN);
	GeometryExt2TestSupport_GEO2_ASSERT(GeometryExt2TestSupport_GEO2_NEAR(v.z, 0.0f));
}


void GeometryTest::test()
{
	testRect();
	testRect3();
	testLine2();
	testLine2D();
	testLine3();
	testTriangle2();
	testTriangle3();
	testTriangleIntersect();
	testTriangleIntersect3();
	testMathHelpers();
	testVectorAdvanced();
	testRect_Extended();
	testVectorMathExtended();
	testDegreeRadianExtended();
	testLerpEdgeCases();
	testClampEdgeCases();
	testIsZeroAndEqual();
	testMinMaxExtended();
	testIntersectLine2Basic();
	testIntersectLine2Parallel();
	testIntersectLine2SamePoint();
	testIntersectLineSectionBasic();
	testIntersectLineSectionNoIntersect();
	testIntersectLineSectionEndpoint();
	testGetDistanceToLineOrigin();
	testGetDistanceToLineUnit();
	testGetDistanceToLineOnLine();
	testGetProjectPointBasic();
	testGetProjectPointOnLine();
	testGetProjectPointPerp();
	testGetProjectionBasic();
	testGetProjectionZeroVector();
	testGetProjectionParallel();
	testGetAngleBetweenVec3Same();
	testGetAngleBetweenVec3Perp();
	testGetAngleBetweenVec2Same();
	testGetAngleBetweenVec2Perp();
	testRotateVector3AroundYBasic();
	testRotateVector3AroundXBasic();
	testRotateVector3AroundZBasic();
	testResetYLine3();
	testGetVectorFromAngleZero();
	testGetVectorFromAngle90();
}
#endif
