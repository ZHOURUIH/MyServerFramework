#ifdef WINDOWS
#include "FrameHeader.h"
namespace MathStructTestSupport
{
#define MathStructTestSupport_MST_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("MathStructTest FAILED: ") + (msg)); }

#define MathStructTestSupport_MST_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)
#define MathStructTestSupport_MST_VEC2_EQ(v, px, py) (MathStructTestSupport_MST_FLOAT_EQ((v).x, (px)) && MathStructTestSupport_MST_FLOAT_EQ((v).y, (py)))
#define MathStructTestSupport_MST_VEC3_EQ(v, px, py, pz) (MathStructTestSupport_MST_FLOAT_EQ((v).x, (px)) && MathStructTestSupport_MST_FLOAT_EQ((v).y, (py)) && MathStructTestSupport_MST_FLOAT_EQ((v).z, (pz)))
}

void MathStructTest::testVector2_DefaultCtor()
{
	using namespace MathStructTestSupport;
	Vector2 v;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.x, 0.0f) && MathStructTestSupport_MST_FLOAT_EQ(v.y, 0.0f), "V2 default ctor zero");
}

void MathStructTest::testVector2_ParamCtor_Float()
{
	using namespace MathStructTestSupport;
	Vector2 v(3.0f, 4.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.x, 3.0f) && MathStructTestSupport_MST_FLOAT_EQ(v.y, 4.0f), "V2 float ctor");
}

void MathStructTest::testVector2_ParamCtor_Int()
{
	using namespace MathStructTestSupport;
	Vector2 v(2, 5);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.x, 2.0f) && MathStructTestSupport_MST_FLOAT_EQ(v.y, 5.0f), "V2 int ctor");
}

void MathStructTest::testVector2_Clear()
{
	using namespace MathStructTestSupport;
	Vector2 v(7.0f, 8.0f);
	v.clear();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.x, 0.0f) && MathStructTestSupport_MST_FLOAT_EQ(v.y, 0.0f), "V2 clear");
}

void MathStructTest::testVector2_Add()
{
	using namespace MathStructTestSupport;
	Vector2 a(1.0f, 2.0f), b(3.0f, 4.0f);
	Vector2 c = a + b;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(c, 4.0f, 6.0f), "V2 add");
}

void MathStructTest::testVector2_Sub()
{
	using namespace MathStructTestSupport;
	Vector2 a(5.0f, 3.0f), b(2.0f, 1.0f);
	Vector2 c = a - b;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(c, 3.0f, 2.0f), "V2 sub");
}

void MathStructTest::testVector2_MulScalar()
{
	using namespace MathStructTestSupport;
	Vector2 a(2.0f, 3.0f);
	Vector2 c = a * 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(c, 4.0f, 6.0f), "V2 mul scalar");
}

void MathStructTest::testVector2_DivScalar()
{
	using namespace MathStructTestSupport;
	Vector2 a(4.0f, 6.0f);
	Vector2 c = a / 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(c, 2.0f, 3.0f), "V2 div scalar");
}

void MathStructTest::testVector2_AddAssign()
{
	using namespace MathStructTestSupport;
	Vector2 a(1.0f, 2.0f);
	a += Vector2(3.0f, 4.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(a, 4.0f, 6.0f), "V2 +=");
}

void MathStructTest::testVector2_SubAssign()
{
	using namespace MathStructTestSupport;
	Vector2 a(5.0f, 7.0f);
	a -= Vector2(2.0f, 3.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(a, 3.0f, 4.0f), "V2 -=");
}

void MathStructTest::testVector2_Negate()
{
	using namespace MathStructTestSupport;
	Vector2 a(3.0f, -2.0f);
	Vector2 n = -a;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(n, -3.0f, 2.0f), "V2 negate");
}

void MathStructTest::testVector2_StaticMembers()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(Vector2::ZERO, 0.0f, 0.0f), "V2 ZERO");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(Vector2::ONE, 1.0f, 1.0f), "V2 ONE");
}

void MathStructTest::testVector2_Chaining()
{
	using namespace MathStructTestSupport;
	Vector2 a(1.0f, 0.0f);
	a += Vector2(2.0f, 3.0f);
	a -= Vector2(1.0f, 1.0f);
	Vector2 r = a * 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(r, 4.0f, 4.0f), "V2 chaining");
}

void MathStructTest::testVector2_ZeroAdd()
{
	using namespace MathStructTestSupport;
	Vector2 v(3.0f, 4.0f);
	Vector2 r = v + Vector2::ZERO;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(r, 3.0f, 4.0f), "V2 add zero unchanged");
}

void MathStructTest::testVector2_NegativeValues_MST()
{
	using namespace MathStructTestSupport;
	Vector2 a(-5.0f, -3.0f);
	Vector2 b(2.0f, 1.0f);
	Vector2 c = a + b;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(c, -3.0f, -2.0f), "V2 neg add");
}

void MathStructTest::testVector2_LargeValues_MST()
{
	using namespace MathStructTestSupport;
	Vector2 a(1e6f, 2e6f);
	Vector2 b = a * 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(b.x / 1e6f, 2.0f), "V2 large mul x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(b.y / 1e6f, 4.0f), "V2 large mul y");
}

void MathStructTest::testVector3_DefaultCtor()
{
	using namespace MathStructTestSupport;
	Vector3 v;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 0.0f, 0.0f, 0.0f), "V3 default ctor zero");
}

void MathStructTest::testVector3_ParamCtor_Float()
{
	using namespace MathStructTestSupport;
	Vector3 v(1.0f, 2.0f, 3.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 1.0f, 2.0f, 3.0f), "V3 float ctor");
}

void MathStructTest::testVector3_ParamCtor_Int()
{
	using namespace MathStructTestSupport;
	Vector3 v(10, 20, 30);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 10.0f, 20.0f, 30.0f), "V3 int ctor");
}

void MathStructTest::testVector3_Clear()
{
	using namespace MathStructTestSupport;
	Vector3 v(1.0f, 2.0f, 3.0f);
	v.clear();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 0.0f, 0.0f, 0.0f), "V3 clear");
}

void MathStructTest::testVector3_Add()
{
	using namespace MathStructTestSupport;
	Vector3 a(1.0f, 2.0f, 3.0f), b(4.0f, 5.0f, 6.0f);
	Vector3 c = a + b;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(c, 5.0f, 7.0f, 9.0f), "V3 add");
}

void MathStructTest::testVector3_Sub()
{
	using namespace MathStructTestSupport;
	Vector3 a(10.0f, 10.0f, 10.0f), b(1.0f, 2.0f, 3.0f);
	Vector3 c = a - b;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(c, 9.0f, 8.0f, 7.0f), "V3 sub");
}

void MathStructTest::testVector3_MulScalar()
{
	using namespace MathStructTestSupport;
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 r = v * 3.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(r, 3.0f, 6.0f, 9.0f), "V3 mul");
}

void MathStructTest::testVector3_DivScalar()
{
	using namespace MathStructTestSupport;
	Vector3 v(6.0f, 9.0f, 12.0f);
	Vector3 r = v / 3.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(r, 2.0f, 3.0f, 4.0f), "V3 div");
}

void MathStructTest::testVector3_AddAssign()
{
	using namespace MathStructTestSupport;
	Vector3 v(1.0f, 2.0f, 3.0f);
	v += Vector3(10.0f, 20.0f, 30.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 11.0f, 22.0f, 33.0f), "V3 +=");
}

void MathStructTest::testVector3_SubAssign()
{
	using namespace MathStructTestSupport;
	Vector3 v(10.0f, 20.0f, 30.0f);
	v -= Vector3(1.0f, 2.0f, 3.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 9.0f, 18.0f, 27.0f), "V3 -=");
}

void MathStructTest::testVector3_Negate()
{
	using namespace MathStructTestSupport;
	Vector3 v(1.0f, -2.0f, 3.0f);
	Vector3 n = -v;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(n, -1.0f, 2.0f, -3.0f), "V3 negate");
}

void MathStructTest::testVector3_StaticMembers()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(Vector3::ZERO, 0.0f, 0.0f, 0.0f), "V3 ZERO");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(Vector3::ONE, 1.0f, 1.0f, 1.0f), "V3 ONE");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector3::UP.y, 1.0f), "V3 UP.y==1");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector3::DOWN.y, -1.0f), "V3 DOWN.y==-1");
}

void MathStructTest::testVector3_Chaining()
{
	using namespace MathStructTestSupport;
	Vector3 v = Vector3(1.0f, 2.0f, 3.0f) + Vector3(1.0f, 1.0f, 1.0f);
	v -= Vector3(0.5f, 0.5f, 0.5f);
	v = v * 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(v, 3.0f, 5.0f, 7.0f), "V3 chain");
}

void MathStructTest::testVector3_AddZero()
{
	using namespace MathStructTestSupport;
	Vector3 v(1.0f, 2.0f, 3.0f);
	Vector3 r = v + Vector3::ZERO;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(r, 1.0f, 2.0f, 3.0f), "V3 add zero");
}

void MathStructTest::testVector3_MulOne()
{
	using namespace MathStructTestSupport;
	Vector3 v(5.0f, 6.0f, 7.0f);
	Vector3 r = v * 1.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(r, 5.0f, 6.0f, 7.0f), "V3 mul 1 unchanged");
}

void MathStructTest::testVector3_NegativeComponents()
{
	using namespace MathStructTestSupport;
	Vector3 v(-1.0f, -2.0f, -3.0f);
	Vector3 r = v * (-1.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(r, 1.0f, 2.0f, 3.0f), "V3 mul neg neg");
}

void MathStructTest::testColor_DefaultCtor()
{
	using namespace MathStructTestSupport;
	Color c;
	MathStructTestSupport_MST_ASSERT(c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0, "Color default ctor");
}

void MathStructTest::testColor_ParamCtor()
{
	using namespace MathStructTestSupport;
	Color c(255, 128, 64, 32);
	MathStructTestSupport_MST_ASSERT(c.r == 255 && c.g == 128 && c.b == 64 && c.a == 32, "Color param ctor");
}

void MathStructTest::testColor_Add()
{
	using namespace MathStructTestSupport;
	Color a(100, 50, 30, 20);
	Color b(10, 20, 30, 40);
	Color c = a + b;
	MathStructTestSupport_MST_ASSERT(c.r == 110 && c.g == 70 && c.b == 60 && c.a == 60, "Color add");
}

void MathStructTest::testColor_Sub()
{
	using namespace MathStructTestSupport;
	Color a(200, 100, 50, 60);
	Color b(50, 30, 20, 10);
	Color c = a - b;
	MathStructTestSupport_MST_ASSERT(c.r == 150 && c.g == 70 && c.b == 30 && c.a == 50, "Color sub");
}

void MathStructTest::testColor_AddAssign()
{
	using namespace MathStructTestSupport;
	Color c(10, 20, 30, 40);
	c += Color(1, 2, 3, 4);
	MathStructTestSupport_MST_ASSERT(c.r == 11 && c.g == 22 && c.b == 33 && c.a == 44, "Color +=");
}

void MathStructTest::testColor_SubAssign()
{
	using namespace MathStructTestSupport;
	Color c(50, 60, 70, 80);
	c -= Color(10, 20, 30, 40);
	MathStructTestSupport_MST_ASSERT(c.r == 40 && c.g == 40 && c.b == 40 && c.a == 40, "Color -=");
}

void MathStructTest::testColor_StaticWhite()
{
	using namespace MathStructTestSupport;
	Color w = Color::WHITE;
	MathStructTestSupport_MST_ASSERT(w.r == 255 && w.g == 255 && w.b == 255 && w.a == 255, "Color WHITE");
}

void MathStructTest::testColor_StaticBlack()
{
	using namespace MathStructTestSupport;
	Color bl = Color::BLACK;
	MathStructTestSupport_MST_ASSERT(bl.r == 0 && bl.g == 0 && bl.b == 0 && bl.a == 255, "Color BLACK");
}

void MathStructTest::testColor_StaticRed()
{
	using namespace MathStructTestSupport;
	Color red = Color::RED;
	MathStructTestSupport_MST_ASSERT(red.r == 255 && red.g == 0 && red.b == 0, "Color RED");
}

void MathStructTest::testColor_AddOverflow()
{
	using namespace MathStructTestSupport;
	// byte overflow wraps (unsigned arithmetic)
	Color c(250, 0, 0, 0);
	Color d = c + Color(10, 0, 0, 0); // 250+10=260 wraps to 4 as byte
	MathStructTestSupport_MST_ASSERT(d.r == (byte)(250 + 10), "Color overflow wrap");
}

void MathStructTest::testColor_AddAssignChain()
{
	using namespace MathStructTestSupport;
	Color c(10, 10, 10, 10);
	c += Color(5, 5, 5, 5);
	c += Color(5, 5, 5, 5);
	MathStructTestSupport_MST_ASSERT(c.r == 20 && c.g == 20 && c.b == 20 && c.a == 20, "Color += chain");
}

void MathStructTest::testColor_SubAssignToZero()
{
	using namespace MathStructTestSupport;
	Color c(100, 100, 100, 100);
	c -= Color(100, 100, 100, 100);
	MathStructTestSupport_MST_ASSERT(c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0, "Color -= to zero");
}

void MathStructTest::testQuaternion_DefaultCtor()
{
	using namespace MathStructTestSupport;
	Quaternion q;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.x, 0.0f), "Q default x==0");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.y, 0.0f), "Q default y==0");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.z, 0.0f), "Q default z==0");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.w, 1.0f), "Q default w==1");
}

void MathStructTest::testQuaternion_ParamCtor()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.1f, 0.2f, 0.3f, 0.9f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.x, 0.1f), "Q ctor x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.y, 0.2f), "Q ctor y");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.z, 0.3f), "Q ctor z");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.w, 0.9f), "Q ctor w");
}

void MathStructTest::testQuaternion_Clear()
{
	using namespace MathStructTestSupport;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q.clear();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.x, 0.0f) && MathStructTestSupport_MST_FLOAT_EQ(q.y, 0.0f) && MathStructTestSupport_MST_FLOAT_EQ(q.z, 0.0f) && MathStructTestSupport_MST_FLOAT_EQ(q.w, 1.0f), "Q clear");
}

void MathStructTest::testQuaternion_Conjugate_MST()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion c = q.conjugate();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(c.x, -0.5f), "Q conjugate x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(c.y, -0.5f), "Q conjugate y");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(c.z, -0.5f), "Q conjugate z");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(c.w, 0.5f), "Q conjugate w");
}

void MathStructTest::testQuaternion_Normalize_MST()
{
	using namespace MathStructTestSupport;
	Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion n = q.normalize();
	float len = n.length();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(len, 1.0f), "Q normalize length==1");
}

void MathStructTest::testQuaternion_NormalizeIdentity()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f); // identity
	Quaternion n = q.normalize();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(n.w, 1.0f), "Q normalize identity w==1");
}

void MathStructTest::testQuaternion_Length()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.length(), 1.0f), "Q identity length==1");

	Quaternion q2(1.0f, 0.0f, 0.0f, 0.0f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.length(), 1.0f), "Q pure length==1");
}

void MathStructTest::testQuaternion_Dot()
{
	using namespace MathStructTestSupport;
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	float d = Quaternion::dot(q1, q2);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(d, 1.0f), "Q dot identity==1");
}

void MathStructTest::testQuaternion_Add()
{
	using namespace MathStructTestSupport;
	Quaternion q1(0.1f, 0.2f, 0.3f, 0.4f);
	Quaternion q2(0.1f, 0.2f, 0.3f, 0.4f);
	Quaternion r = q1 + q2;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.x, 0.2f) && MathStructTestSupport_MST_FLOAT_EQ(r.y, 0.4f), "Q add xy");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.z, 0.6f) && MathStructTestSupport_MST_FLOAT_EQ(r.w, 0.8f), "Q add zw");
}

void MathStructTest::testQuaternion_AddAssign()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.1f, 0.2f, 0.3f, 0.4f);
	q += Quaternion(0.1f, 0.1f, 0.1f, 0.1f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.x, 0.2f), "Q += x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.w, 0.5f), "Q += w");
}

void MathStructTest::testQuaternion_MulScalar()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion r = q * 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.x, 1.0f) && MathStructTestSupport_MST_FLOAT_EQ(r.w, 1.0f), "Q mul scalar");
}

void MathStructTest::testQuaternion_MulAssignScalar()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.25f, 0.25f, 0.25f, 0.5f);
	q *= 4.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.x, 1.0f), "Q *= x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.w, 2.0f), "Q *= w");
}

void MathStructTest::testQuaternion_DivScalar()
{
	using namespace MathStructTestSupport;
	Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
	Quaternion r = q / 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.x, 1.0f) && MathStructTestSupport_MST_FLOAT_EQ(r.y, 2.0f), "Q div scalar xy");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.z, 3.0f) && MathStructTestSupport_MST_FLOAT_EQ(r.w, 4.0f), "Q div scalar zw");
}

void MathStructTest::testQuaternion_Negate_MST()
{
	using namespace MathStructTestSupport;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion n = -q;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(n.x, -1.0f), "Q neg x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(n.w, -4.0f), "Q neg w");
}

void MathStructTest::testQuaternion_Lerp_MST()
{
	using namespace MathStructTestSupport;
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion r = Quaternion::lerp(q1, q2, 0.5f);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.w, 1.0f), "Q lerp same quaternion w==1");
}

void MathStructTest::testQuaternion_CopyCtor()
{
	using namespace MathStructTestSupport;
	Quaternion q1(0.1f, 0.2f, 0.3f, 0.9f);
	Quaternion q2(q1);
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.x, 0.1f) && MathStructTestSupport_MST_FLOAT_EQ(q2.y, 0.2f), "Q copy ctor xy");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.z, 0.3f) && MathStructTestSupport_MST_FLOAT_EQ(q2.w, 0.9f), "Q copy ctor zw");
}

void MathStructTest::testQuaternion_AxisAngleCtor()
{
	using namespace MathStructTestSupport;
	// 绕 Y 轴旋转 90 度
	Vector3 axis(0.0f, 1.0f, 0.0f);
	float angle = 1.5707963f; // pi/2
	Quaternion q(axis, angle);
	// 旋转后的 y 分量应接近 sin(pi/4) ≈ 0.7071
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.w, cosf(angle * 0.5f)), "Q axis ctor w==cos(a/2)");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.y, sinf(angle * 0.5f)), "Q axis ctor y==sin(a/2)");
}

void MathStructTest::testQuaternion_EulerAngles_MST()
{
	using namespace MathStructTestSupport;
	// 单位四元数欧拉角应为零
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	Vector3 euler = q.eulerAngles();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(euler.x, 0.0f), "Q euler pitch==0");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(euler.y, 0.0f), "Q euler yaw==0");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(euler.z, 0.0f), "Q euler roll==0");
}

void MathStructTest::testQuaternion_Mul_Identity()
{
	using namespace MathStructTestSupport;
	Quaternion identity(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q(0.1f, 0.2f, 0.3f, 0.9f);
	Quaternion n = q.normalize();
	Quaternion r = n * identity;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(r.x, n.x) && MathStructTestSupport_MST_FLOAT_EQ(r.w, n.w), "Q mul identity unchanged");
}

void MathStructTest::testQuaternion_Mul_Vector3()
{
	using namespace MathStructTestSupport;
	// 旋转 z 轴 180 度后 x=1 应变成 x=-1
	Quaternion q(0.0f, 0.0f, 1.0f, 0.0f); // 180 around Z
	Vector3 v(1.0f, 0.0f, 0.0f);
	Vector3 r = q * v;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(fabsf(r.x), 1.0f), "Q mul vec3 abs x==1");
}

void MathStructTest::testQuaternion_NormalizeThenLength()
{
	using namespace MathStructTestSupport;
	Quaternion q(3.0f, 1.0f, 4.0f, 1.0f);
	Quaternion n = q.normalize();
	float len = n.length();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(len, 1.0f), "Q normalize then length==1");
}

void MathStructTest::testQuaternion_InverseOfNormalized()
{
	using namespace MathStructTestSupport;
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f); // identity
	Quaternion inv = q.inverse();
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(inv.w, 1.0f), "Q inverse identity w==1");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(inv.x, 0.0f), "Q inverse identity x==0");
}

void MathStructTest::testVector2_ChainAssign()
{
	using namespace MathStructTestSupport;
	Vector2 v(0.0f, 0.0f);
	for (int i = 1; i <= 5; ++i)
	{
	    v += Vector2((float)i, (float)i);
	}
	// sum = 1+2+3+4+5 = 15
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.x, 15.0f), "V2 chain += x=15");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.y, 15.0f), "V2 chain += y=15");

	for (int i = 1; i <= 5; ++i)
	{
	    v -= Vector2((float)i, (float)i);
	}
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.x, 0.0f), "V2 chain -= x=0");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(v.y, 0.0f), "V2 chain -= y=0");
}

void MathStructTest::testVector3_ScaleAndNeg()
{
	using namespace MathStructTestSupport;
	Vector3 v(2.0f, 3.0f, 4.0f);
	Vector3 scaled = v * 2.5f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(scaled.x, 5.0f), "V3 scale x=5");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(scaled.y, 7.5f), "V3 scale y=7.5");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(scaled.z, 10.0f), "V3 scale z=10");

	Vector3 neg = -v;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(neg.x, -2.0f), "V3 neg x=-2");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(neg.y, -3.0f), "V3 neg y=-3");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(neg.z, -4.0f), "V3 neg z=-4");

	Vector3 div = v / 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(div.x, 1.0f), "V3 div x=1");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(div.y, 1.5f), "V3 div y=1.5");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(div.z, 2.0f), "V3 div z=2");
}

void MathStructTest::testQuaternion_DivScalarExtended()
{
	using namespace MathStructTestSupport;
	Quaternion q(4.0f, 8.0f, 12.0f, 16.0f);
	q /= 4.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.x, 1.0f), "Q /= x=1");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.y, 2.0f), "Q /= y=2");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.z, 3.0f), "Q /= z=3");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q.w, 4.0f), "Q /= w=4");

	// scalar multiply then divide: round-trip
	Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
	q2 *= 3.0f;
	q2 /= 3.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.x, 1.0f), "Q *3/3 x");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.y, 2.0f), "Q *3/3 y");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.z, 3.0f), "Q *3/3 z");
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(q2.w, 4.0f), "Q *3/3 w");
}

void MathStructTest::testVector2_ParamCtor()
{
	using namespace MathStructTestSupport;
	testVector2_ParamCtor_Float();
	testVector2_ParamCtor_Int();
}

void MathStructTest::testVector2_MulAssign()
{
	using namespace MathStructTestSupport;
	Vector2 a(3.0f, 4.0f);
	a = a * 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(a, 6.0f, 8.0f), "V2 *=2");
}

void MathStructTest::testVector2_DivAssign()
{
	using namespace MathStructTestSupport;
	Vector2 a(6.0f, 8.0f);
	a = a / 2.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(a, 3.0f, 4.0f), "V2 /=2");
}

void MathStructTest::testVector2_StaticZero()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(Vector2::ZERO, 0.0f, 0.0f), "V2 ZERO");
}

void MathStructTest::testVector2_StaticOne()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC2_EQ(Vector2::ONE, 1.0f, 1.0f), "V2 ONE");
}

void MathStructTest::testVector2_StaticUp()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector2::UP.y, 1.0f), "V2 UP.y==1");
}

void MathStructTest::testVector2_StaticDown()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector2::DOWN.y, -1.0f), "V2 DOWN.y==-1");
}

void MathStructTest::testVector2_StaticLeft()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector2::LEFT.x, -1.0f), "V2 LEFT.x==-1");
}

void MathStructTest::testVector2_StaticRight()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector2::RIGHT.x, 1.0f), "V2 RIGHT.x==1");
}

void MathStructTest::testVector3_ParamCtor()
{
	using namespace MathStructTestSupport;
	testVector3_ParamCtor_Float();
	testVector3_ParamCtor_Int();
}

void MathStructTest::testVector3_MulAssign()
{
	using namespace MathStructTestSupport;
	Vector3 a(1.0f, 2.0f, 3.0f);
	a = a * 3.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(a, 3.0f, 6.0f, 9.0f), "V3 *=3");
}

void MathStructTest::testVector3_DivAssign()
{
	using namespace MathStructTestSupport;
	Vector3 a(3.0f, 6.0f, 9.0f);
	a = a / 3.0f;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(a, 1.0f, 2.0f, 3.0f), "V3 /=3");
}

void MathStructTest::testVector3_StaticZero()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(Vector3::ZERO, 0.0f, 0.0f, 0.0f), "V3 ZERO");
}

void MathStructTest::testVector3_StaticOne()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_VEC3_EQ(Vector3::ONE, 1.0f, 1.0f, 1.0f), "V3 ONE");
}

void MathStructTest::testVector3_StaticUp()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector3::UP.y, 1.0f), "V3 UP.y==1");
}

void MathStructTest::testVector3_StaticDown()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector3::DOWN.y, -1.0f), "V3 DOWN.y==-1");
}

void MathStructTest::testVector3_StaticLeft()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector3::LEFT.x, -1.0f), "V3 LEFT.x==-1");
}

void MathStructTest::testVector3_StaticRight()
{
	using namespace MathStructTestSupport;
	MathStructTestSupport_MST_ASSERT(MathStructTestSupport_MST_FLOAT_EQ(Vector3::RIGHT.x, 1.0f), "V3 RIGHT.x==1");
}

void MathStructTest::testColor_Clear()
{
	using namespace MathStructTestSupport;
	Color c(100, 200, 150, 255);
	c = Color();  // 重置为默认 (0,0,0,0)
	MathStructTestSupport_MST_ASSERT(c.r == 0 && c.g == 0 && c.b == 0 && c.a == 0, "Color clear via default ctor");
}

void MathStructTest::testColor_Mul()
{
	using namespace MathStructTestSupport;
	// Color 无 * 运算符，用手动构造等效结果验证
	Color c(100, 50, 25, 255);
	Color d(c.r / 2, c.g / 2, c.b / 2, c.a / 2);
	MathStructTestSupport_MST_ASSERT(d.r == 50 && d.g == 25 && d.b == 12 && d.a == 127, "Color manual mul half");
}

void MathStructTest::testColor_MulAssign()
{
	using namespace MathStructTestSupport;
	Color c(100, 50, 25, 255);
	c = Color(c.r / 2, c.g / 2, c.b / 2, c.a / 2);
	MathStructTestSupport_MST_ASSERT(c.r == 50 && c.g == 25 && c.b == 12 && c.a == 127, "Color manual mulAssign half");
}

void MathStructTest::testColor_EqualityExtended()
{
	using namespace MathStructTestSupport;
	Color a((byte)255, (byte)128, (byte)0, (byte)255);
	Color b((byte)255, (byte)128, (byte)0, (byte)255);
	Color c((byte)255, (byte)128, (byte)0, (byte)229);
	MathStructTestSupport_MST_ASSERT(a == b, "Color eq same");
	MathStructTestSupport_MST_ASSERT(!(a == c), "Color not eq diff alpha");
	MathStructTestSupport_MST_ASSERT(a != c, "Color neq diff alpha");

	// White
	Color white = Color::WHITE;
	Color white2 = Color::WHITE;
	MathStructTestSupport_MST_ASSERT(white == white2, "White == White");
	MathStructTestSupport_MST_ASSERT(!(white != white2), "White not != White");

	// Black
	Color black = Color::BLACK;
	MathStructTestSupport_MST_ASSERT(black != white, "Black != White");
}

void MathStructTest::testVector2_EqualityExtended()
{
	using namespace MathStructTestSupport;
	Vector2 a(1.0f, 2.0f);
	Vector2 b(1.0f, 2.0f);
	Vector2 c(1.0f, 3.0f);
	MathStructTestSupport_MST_ASSERT(a == b, "V2 eq same");
	MathStructTestSupport_MST_ASSERT(a != c, "V2 neq diff");
	MathStructTestSupport_MST_ASSERT(!(a != b), "V2 not neq same");
	MathStructTestSupport_MST_ASSERT(!(a == c), "V2 not eq diff");

	// ZERO
	Vector2 z1 = Vector2::ZERO;
	Vector2 z2 = Vector2::ZERO;
	MathStructTestSupport_MST_ASSERT(z1 == z2, "V2 ZERO == ZERO");
	MathStructTestSupport_MST_ASSERT(z1 != a, "V2 ZERO != (1,2)");
}

void MathStructTest::testVector3_EqualityExtended()
{
	using namespace MathStructTestSupport;
	Vector3 a(1.0f, 2.0f, 3.0f);
	Vector3 b(1.0f, 2.0f, 3.0f);
	Vector3 c(1.0f, 2.0f, 4.0f);
	MathStructTestSupport_MST_ASSERT(a == b, "V3 eq same");
	MathStructTestSupport_MST_ASSERT(a != c, "V3 neq diff z");
	MathStructTestSupport_MST_ASSERT(!(a != b), "V3 not neq same");
	MathStructTestSupport_MST_ASSERT(!(a == c), "V3 not eq diff z");

	// ZERO
	Vector3 z1 = Vector3::ZERO;
	Vector3 z2 = Vector3::ZERO;
	MathStructTestSupport_MST_ASSERT(z1 == z2, "V3 ZERO == ZERO");
	MathStructTestSupport_MST_ASSERT(z1 != a, "V3 ZERO != (1,2,3)");
}

namespace MathStructTest2Support
{
    // ============================================================
    // MathStructTest2
    // 覆盖几何结构体：
    //   Line2 (成员访问)
    //   Line2D (getPointYOnLine / getPointXOnLine / 垂直线 / 斜线 / 水平线)
    //   Line3 (toLine2IgnoreY / toLine2IgnoreX)
    //   Rect (getSize / getMin / getMax / getCenter / getDiagonal)
    //   Rect3 (getSize / getCenter / getDiagonalIgnoreY)
    //   Triangle2 (构造 / 成员访问)
    //   Triangle3 (构造 / 成员访问)
    // ============================================================

#define MathStructTest2Support_MST2_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("MathStructTest2 FAILED: ") + (msg)); } } while(0)
#define MathStructTest2Support_MST2_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("MathStructTest2 FAILED: ") + (msg) \
        + " expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define MathStructTest2Support_MST2_ASSERT_F(a, b, msg) \
    do { if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("MathStructTest2 FAILED: ") + (msg) \
        + " expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)

// ============================================================
// Line2 —— 构造、成员访问
// ============================================================

    // ============================================================
    // Line2D —— 斜线
    // ============================================================

    // ============================================================
    // Line2D —— 垂直线（无斜率）
    // ============================================================

    // ============================================================
    // Line2D —— 水平线
    // ============================================================

    // ============================================================
    // Line2D —— 负斜率
    // ============================================================

    // ============================================================
    // Line3 —— toLine2IgnoreY / toLine2IgnoreX
    // ============================================================

    // ============================================================
    // Rect —— 构造 + getSize/getMin/getMax/getCenter/getDiagonal
    // ============================================================

    // ============================================================
    // Rect3 —— 构造 + getSize/getCenter/getDiagonalIgnoreY
    // ============================================================

    // ============================================================
    // Triangle2 —— 构造 + 成员访问
    // ============================================================

    // ============================================================
    // Triangle3 —— 构造 + 成员访问
    // ============================================================

    // ============================================================
    // 综合场景：用几何类做简单碰撞/范围检测逻辑
    // ============================================================

    // ============================================================
    // 数值精度测试
    // ============================================================

    // ============================================================
    // 主入口
    // ============================================================
}

void MathStructTest::testLine2B()
{
	using namespace MathStructTest2Support;
	Line2 l1({ 1.0f, 2.0f }, { 5.0f, 6.0f });

	MathStructTest2Support_MST2_ASSERT_F(l1.mStart.x, 1.0f, "Line2 start.x");
	MathStructTest2Support_MST2_ASSERT_F(l1.mStart.y, 2.0f, "Line2 start.y");
	MathStructTest2Support_MST2_ASSERT_F(l1.mEnd.x, 5.0f, "Line2 end.x");
	MathStructTest2Support_MST2_ASSERT_F(l1.mEnd.y, 6.0f, "Line2 end.y");

	// 负坐标
	Line2 l2({ -3.0f, -4.0f }, { 7.0f, 8.0f });
	MathStructTest2Support_MST2_ASSERT_F(l2.mStart.x, -3.0f, "Line2 neg start.x");
	MathStructTest2Support_MST2_ASSERT_F(l2.mEnd.y, 8.0f, "Line2 pos end.y");

	// 起点等于终点（退化线段）
	Line2 l3({ 0.0f, 0.0f }, { 0.0f, 0.0f });
	MathStructTest2Support_MST2_ASSERT_F(l3.mStart.x, l3.mEnd.x, "Line2 degenerate x");
	MathStructTest2Support_MST2_ASSERT_F(l3.mStart.y, l3.mEnd.y, "Line2 degenerate y");

	// 水平线
	Line2 l4({ -10.0f, 5.0f }, { 10.0f, 5.0f });
	MathStructTest2Support_MST2_ASSERT_F(l4.mStart.y, l4.mEnd.y, "Line2 horizontal same y");

	// 垂直线
	Line2 l5({ 3.0f, -10.0f }, { 3.0f, 10.0f });
	MathStructTest2Support_MST2_ASSERT_F(l5.mStart.x, l5.mEnd.x, "Line2 vertical same x");
}

void MathStructTest::testLine2DSloped()
{
	using namespace MathStructTest2Support;
	// 斜率 k=1，截距 b=0，从 (0,0) 到 (10,10)
	Line2D ld({ 0.0f, 0.0f }, { 10.0f, 10.0f });
	MathStructTest2Support_MST2_ASSERT(ld.mHasK, "Line2D slope: mHasK==true");
	MathStructTest2Support_MST2_ASSERT_F(ld.mK, 1.0f, "Line2D slope k==1");
	MathStructTest2Support_MST2_ASSERT_F(ld.mB, 0.0f, "Line2D slope b==0");

	// getPointYOnLine
	float y = 0.0f;
	bool r = ld.getPointYOnLine(5.0f, y);
	MathStructTest2Support_MST2_ASSERT(r, "Line2D getPointY has slope");
	MathStructTest2Support_MST2_ASSERT_F(y, 5.0f, "Line2D getPointY x=5 -> y=5");

	// getPointXOnLine
	float x = 0.0f;
	bool r2 = ld.getPointXOnLine(7.0f, x);
	MathStructTest2Support_MST2_ASSERT(r2, "Line2D getPointX has slope");
	MathStructTest2Support_MST2_ASSERT_F(x, 7.0f, "Line2D getPointX y=7 -> x=7");

	// 斜率 k=2，截距 b=1，从 (0,1) 到 (5,11)
	Line2D ld2({ 0.0f, 1.0f }, { 5.0f, 11.0f });
	MathStructTest2Support_MST2_ASSERT(ld2.mHasK, "Line2D slope2: mHasK==true");
	MathStructTest2Support_MST2_ASSERT_F(ld2.mK, 2.0f, "Line2D slope2 k==2");
	MathStructTest2Support_MST2_ASSERT_F(ld2.mB, 1.0f, "Line2D slope2 b==1");

	float y2 = 0.0f;
	ld2.getPointYOnLine(3.0f, y2);
	MathStructTest2Support_MST2_ASSERT_F(y2, 7.0f, "Line2D slope2 x=3 -> y=7");

	float x2 = 0.0f;
	ld2.getPointXOnLine(11.0f, x2);
	MathStructTest2Support_MST2_ASSERT_F(x2, 5.0f, "Line2D slope2 y=11 -> x=5");
}

void MathStructTest::testLine2DVertical()
{
	using namespace MathStructTest2Support;
	// 垂直线：x=3，从 (3,0) 到 (3,10)
	Line2D ld({ 3.0f, 0.0f }, { 3.0f, 10.0f });
	MathStructTest2Support_MST2_ASSERT(!ld.mHasK, "Line2D vertical: mHasK==false");

	// getPointYOnLine 对垂直线返回 false
	float y = 99.0f;
	bool r = ld.getPointYOnLine(3.0f, y);
	MathStructTest2Support_MST2_ASSERT(!r, "Line2D vertical getPointY returns false");

	// getPointXOnLine 对垂直线：应返回 x 为直线所在的 x 值
	float x = 0.0f;
	bool r2 = ld.getPointXOnLine(5.0f, x);
	// 垂直线上每个 y 对应 x=3
	MathStructTest2Support_MST2_ASSERT(r2, "Line2D vertical getPointX returns true");
	MathStructTest2Support_MST2_ASSERT_F(x, 3.0f, "Line2D vertical getPointX==3");
}

void MathStructTest::testLine2DHorizontal()
{
	using namespace MathStructTest2Support;
	// 水平线：y=5，从 (-10,5) 到 (10,5)
	Line2D ld({ -10.0f, 5.0f }, { 10.0f, 5.0f });
	MathStructTest2Support_MST2_ASSERT(ld.mHasK, "Line2D horizontal: mHasK==true (k=0)");
	MathStructTest2Support_MST2_ASSERT_F(ld.mK, 0.0f, "Line2D horizontal k==0");
	MathStructTest2Support_MST2_ASSERT_F(ld.mB, 5.0f, "Line2D horizontal b==5");

	// getPointYOnLine：对任意 x，y=5
	float y = 0.0f;
	ld.getPointYOnLine(0.0f, y);
	MathStructTest2Support_MST2_ASSERT_F(y, 5.0f, "Line2D horizontal getPointY x=0 -> y=5");

	float y2 = 0.0f;
	ld.getPointYOnLine(-100.0f, y2);
	MathStructTest2Support_MST2_ASSERT_F(y2, 5.0f, "Line2D horizontal getPointY x=-100 -> y=5");
}

void MathStructTest::testLine2DNegativeSlope()
{
	using namespace MathStructTest2Support;
	// k=-1，b=10，从 (0,10) 到 (10,0)
	Line2D ld({ 0.0f, 10.0f }, { 10.0f, 0.0f });
	MathStructTest2Support_MST2_ASSERT(ld.mHasK, "Line2D neg slope: mHasK==true");
	MathStructTest2Support_MST2_ASSERT_F(ld.mK, -1.0f, "Line2D neg slope k==-1");
	MathStructTest2Support_MST2_ASSERT_F(ld.mB, 10.0f, "Line2D neg slope b==10");

	float y = 0.0f;
	ld.getPointYOnLine(4.0f, y);
	MathStructTest2Support_MST2_ASSERT_F(y, 6.0f, "Line2D neg slope x=4 -> y=6");

	float x = 0.0f;
	ld.getPointXOnLine(6.0f, x);
	MathStructTest2Support_MST2_ASSERT_F(x, 4.0f, "Line2D neg slope y=6 -> x=4");
}

void MathStructTest::testLine3Conversions()
{
	using namespace MathStructTest2Support;
	// Line3 从 (1,10,2) 到 (4,20,6)
	Line3 l3({ 1.0f, 10.0f, 2.0f }, { 4.0f, 20.0f, 6.0f });

	// toLine2IgnoreY：投影到 XZ 平面 → Line2D({1,2},{4,6})
	Line2D l2_xz = l3.toLine2IgnoreY();
	MathStructTest2Support_MST2_ASSERT_F(l2_xz.mStart.x, 1.0f, "Line3 ignoreY start.x");
	MathStructTest2Support_MST2_ASSERT_F(l2_xz.mStart.y, 2.0f, "Line3 ignoreY start.y (=z)");
	MathStructTest2Support_MST2_ASSERT_F(l2_xz.mEnd.x, 4.0f, "Line3 ignoreY end.x");
	MathStructTest2Support_MST2_ASSERT_F(l2_xz.mEnd.y, 6.0f, "Line3 ignoreY end.y (=z)");
	MathStructTest2Support_MST2_ASSERT(l2_xz.mHasK, "Line3 ignoreY line2d has slope");

	// toLine2IgnoreX：投影到 ZY 平面 → Line2D({2,10},{6,20})
	Line2D l2_zy = l3.toLine2IgnoreX();
	MathStructTest2Support_MST2_ASSERT_F(l2_zy.mStart.x, 2.0f, "Line3 ignoreX start.x (=z)");
	MathStructTest2Support_MST2_ASSERT_F(l2_zy.mStart.y, 10.0f, "Line3 ignoreX start.y");
	MathStructTest2Support_MST2_ASSERT_F(l2_zy.mEnd.x, 6.0f, "Line3 ignoreX end.x (=z)");
	MathStructTest2Support_MST2_ASSERT_F(l2_zy.mEnd.y, 20.0f, "Line3 ignoreX end.y");

	// 用 getPointYOnLine 验证 ignoreY 投影
	float y = 0.0f;
	l2_xz.getPointYOnLine(2.5f, y);
	// slope = (6-2)/(4-1) = 4/3, b = 2 - 4/3*1 = 2 - 1.333 = 0.667
	// y at x=2.5: 4/3*2.5 + 0.667 = 3.333+0.667 = 4
	MathStructTest2Support_MST2_ASSERT_F(y, 4.0f, "Line3 ignoreY getPointY correct");

	// 垂直线 Line3
	Line3 vert({ 5.0f, 0.0f, 5.0f }, { 5.0f, 100.0f, 5.0f });
	Line2D l2_vert = vert.toLine2IgnoreY();
	// XZ 平面：(5,5)->(5,5)，退化点
	MathStructTest2Support_MST2_ASSERT(!l2_vert.mHasK, "Line3 vert ignoreY no slope (degenerate)");

	// 水平线 Line3
	Line3 horiz({ 0.0f, 3.0f, 0.0f }, { 10.0f, 3.0f, 0.0f });
	Line2D l2_horiz = horiz.toLine2IgnoreY();
	// XZ 平面：(0,0)->(10,0)，水平线 k=0
	MathStructTest2Support_MST2_ASSERT(l2_horiz.mHasK, "Line3 horiz ignoreY has slope");
	MathStructTest2Support_MST2_ASSERT_F(l2_horiz.mK, 0.0f, "Line3 horiz ignoreY k==0");
}

void MathStructTest::testRectB()
{
	using namespace MathStructTest2Support;
	// 基本矩形：(x=1, y=2, w=4, h=3) → min=(1,2), max=(5,5), center=(3,3.5)
	Rect r(1.0f, 2.0f, 4.0f, 3.0f);

	// getSize
	Vector2 sz = r.getSize();
	MathStructTest2Support_MST2_ASSERT_F(sz.x, 4.0f, "Rect getSize.x==4");
	MathStructTest2Support_MST2_ASSERT_F(sz.y, 3.0f, "Rect getSize.y==3");

	// getMin
	Vector2 mn = r.getMin();
	MathStructTest2Support_MST2_ASSERT_F(mn.x, 1.0f, "Rect getMin.x==1");
	MathStructTest2Support_MST2_ASSERT_F(mn.y, 2.0f, "Rect getMin.y==2");

	// getMax
	Vector2 mx = r.getMax();
	MathStructTest2Support_MST2_ASSERT_F(mx.x, 5.0f, "Rect getMax.x==5");
	MathStructTest2Support_MST2_ASSERT_F(mx.y, 5.0f, "Rect getMax.y==5");

	// getCenter
	Vector2 ct = r.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(ct.x, 3.0f, "Rect getCenter.x==3");
	MathStructTest2Support_MST2_ASSERT_F(ct.y, 3.5f, "Rect getCenter.y==3.5");

	// getDiagonal = sqrt(4^2 + 3^2) = sqrt(16+9) = sqrt(25) = 5
	float diag = r.getDiagonal();
	MathStructTest2Support_MST2_ASSERT_F(diag, 5.0f, "Rect getDiagonal==5");

	// 正方形
	Rect sq(0.0f, 0.0f, 10.0f, 10.0f);
	MathStructTest2Support_MST2_ASSERT_F(sq.getDiagonal(), sqrtf(200.0f), "Rect square diagonal==sqrt200");
	Vector2 sqCenter = sq.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(sqCenter.x, 5.0f, "Rect square center.x==5");
	MathStructTest2Support_MST2_ASSERT_F(sqCenter.y, 5.0f, "Rect square center.y==5");

	// 负坐标矩形
	Rect neg(-5.0f, -3.0f, 10.0f, 6.0f);
	Vector2 negMin = neg.getMin();
	Vector2 negMax = neg.getMax();
	MathStructTest2Support_MST2_ASSERT_F(negMin.x, -5.0f, "Rect neg getMin.x==-5");
	MathStructTest2Support_MST2_ASSERT_F(negMin.y, -3.0f, "Rect neg getMin.y==-3");
	MathStructTest2Support_MST2_ASSERT_F(negMax.x, 5.0f, "Rect neg getMax.x==5");
	MathStructTest2Support_MST2_ASSERT_F(negMax.y, 3.0f, "Rect neg getMax.y==3");
	Vector2 negCenter = neg.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(negCenter.x, 0.0f, "Rect neg center.x==0");
	MathStructTest2Support_MST2_ASSERT_F(negCenter.y, 0.0f, "Rect neg center.y==0");

	// Vector2 构造版本
	Rect rv({ 2.0f, 3.0f }, { 6.0f, 4.0f });
	MathStructTest2Support_MST2_ASSERT_F(rv.x, 2.0f, "Rect V2 ctor x");
	MathStructTest2Support_MST2_ASSERT_F(rv.y, 3.0f, "Rect V2 ctor y");
	MathStructTest2Support_MST2_ASSERT_F(rv.width, 6.0f, "Rect V2 ctor width");
	MathStructTest2Support_MST2_ASSERT_F(rv.height, 4.0f, "Rect V2 ctor height");

	// 极小矩形（1x1）
	Rect tiny(0.0f, 0.0f, 1.0f, 1.0f);
	MathStructTest2Support_MST2_ASSERT_F(tiny.getDiagonal(), sqrtf(2.0f), "Rect 1x1 diagonal==sqrt2");

	// 极大矩形
	Rect huge(0.0f, 0.0f, 1000.0f, 1000.0f);
	MathStructTest2Support_MST2_ASSERT_F(huge.getCenter().x, 500.0f, "Rect huge center.x==500");
	MathStructTest2Support_MST2_ASSERT_F(huge.getDiagonal(), sqrtf(2000000.0f), "Rect huge diagonal");
}

void MathStructTest::testRect3B()
{
	using namespace MathStructTest2Support;
	// min=(1,0,2), max=(5,10,6) → size=(4,10,4), center=(3,5,4)
	Rect3 r3({ 1.0f, 0.0f, 2.0f }, { 5.0f, 10.0f, 6.0f });

	// getSize
	Vector3 sz = r3.getSize();
	MathStructTest2Support_MST2_ASSERT_F(sz.x, 4.0f, "Rect3 getSize.x==4");
	MathStructTest2Support_MST2_ASSERT_F(sz.y, 10.0f, "Rect3 getSize.y==10");
	MathStructTest2Support_MST2_ASSERT_F(sz.z, 4.0f, "Rect3 getSize.z==4");

	// getCenter
	Vector3 ct = r3.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(ct.x, 3.0f, "Rect3 getCenter.x==3");
	MathStructTest2Support_MST2_ASSERT_F(ct.y, 5.0f, "Rect3 getCenter.y==5");
	MathStructTest2Support_MST2_ASSERT_F(ct.z, 4.0f, "Rect3 getCenter.z==4");

	// getDiagonalIgnoreY：size.y 被忽略，size=(4,0,4)，diagonal=sqrt(4^2+4^2)=sqrt(32)
	float diagIgnoreY = r3.getDiagonalIgnoreY();
	MathStructTest2Support_MST2_ASSERT_F(diagIgnoreY, sqrtf(32.0f), "Rect3 getDiagonalIgnoreY==sqrt32");

	// 立方体（正方体）
	Rect3 cube({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f });
	Vector3 cubeCenter = cube.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(cubeCenter.x, 5.0f, "Rect3 cube center.x==5");
	MathStructTest2Support_MST2_ASSERT_F(cubeCenter.y, 5.0f, "Rect3 cube center.y==5");
	MathStructTest2Support_MST2_ASSERT_F(cubeCenter.z, 5.0f, "Rect3 cube center.z==5");
	// getDiagonalIgnoreY：(10,10) → sqrt(200)
	MathStructTest2Support_MST2_ASSERT_F(cube.getDiagonalIgnoreY(), sqrtf(200.0f), "Rect3 cube diag ignoreY==sqrt200");

	// 扁平矩形（y=0）
	Rect3 flat({ 0.0f, 0.0f, 0.0f }, { 4.0f, 0.0f, 3.0f });
	MathStructTest2Support_MST2_ASSERT_F(flat.getDiagonalIgnoreY(), 5.0f, "Rect3 flat diag==5 (3-4-5)");

	// 零尺寸矩形
	Rect3 zero({ 5.0f, 5.0f, 5.0f }, { 5.0f, 5.0f, 5.0f });
	MathStructTest2Support_MST2_ASSERT_F(zero.getSize().x, 0.0f, "Rect3 zero size.x==0");
	MathStructTest2Support_MST2_ASSERT_F(zero.getCenter().x, 5.0f, "Rect3 zero center.x==5");
	MathStructTest2Support_MST2_ASSERT_F(zero.getDiagonalIgnoreY(), 0.0f, "Rect3 zero diag==0");

	// 负坐标
	Rect3 neg({ -5.0f, -5.0f, -5.0f }, { 5.0f, 5.0f, 5.0f });
	Vector3 negCenter = neg.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(negCenter.x, 0.0f, "Rect3 neg center.x==0");
	MathStructTest2Support_MST2_ASSERT_F(negCenter.y, 0.0f, "Rect3 neg center.y==0");
	MathStructTest2Support_MST2_ASSERT_F(negCenter.z, 0.0f, "Rect3 neg center.z==0");
}

void MathStructTest::testTriangle2B()
{
	using namespace MathStructTest2Support;
	// 基本三角形
	Triangle2 t1({ 0.0f, 0.0f }, { 4.0f, 0.0f }, { 2.0f, 3.0f });
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint0.x, 0.0f, "Triangle2 p0.x");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint0.y, 0.0f, "Triangle2 p0.y");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint1.x, 4.0f, "Triangle2 p1.x");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint1.y, 0.0f, "Triangle2 p1.y");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint2.x, 2.0f, "Triangle2 p2.x");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint2.y, 3.0f, "Triangle2 p2.y");

	// 退化三角形（三点共线）
	Triangle2 t2({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 2.0f, 2.0f });
	MathStructTest2Support_MST2_ASSERT_F(t2.mPoint2.x, 2.0f, "Triangle2 degenerate p2.x");

	// 负坐标
	Triangle2 t3({ -1.0f, -2.0f }, { -3.0f, -4.0f }, { -5.0f, -6.0f });
	MathStructTest2Support_MST2_ASSERT_F(t3.mPoint0.x, -1.0f, "Triangle2 neg p0.x");
	MathStructTest2Support_MST2_ASSERT_F(t3.mPoint1.y, -4.0f, "Triangle2 neg p1.y");
	MathStructTest2Support_MST2_ASSERT_F(t3.mPoint2.x, -5.0f, "Triangle2 neg p2.x");

	// 等边三角形
	const float h = sqrtf(3.0f) * 0.5f;
	Triangle2 equilateral({ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, h });
	MathStructTest2Support_MST2_ASSERT_F(equilateral.mPoint2.x, 0.5f, "Triangle2 equilateral p2.x");
	MathStructTest2Support_MST2_ASSERT_F(equilateral.mPoint2.y, h, "Triangle2 equilateral p2.y");

	// 默认构造（不崩溃）
	Triangle2 t_default;
	MathStructTest2Support_MST2_ASSERT(true, "Triangle2 default ctor no crash");

	// 大坐标
	Triangle2 big({ 1000.0f, 1000.0f }, { 2000.0f, 1000.0f }, { 1500.0f, 2000.0f });
	MathStructTest2Support_MST2_ASSERT_F(big.mPoint0.x, 1000.0f, "Triangle2 big p0.x");
	MathStructTest2Support_MST2_ASSERT_F(big.mPoint2.y, 2000.0f, "Triangle2 big p2.y");
}

void MathStructTest::testTriangle3B()
{
	using namespace MathStructTest2Support;
	// 基本三角形
	Triangle3 t1({ 0.0f, 0.0f, 0.0f }, { 4.0f, 0.0f, 0.0f }, { 2.0f, 0.0f, 3.0f });
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint0.x, 0.0f, "Triangle3 p0.x");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint0.y, 0.0f, "Triangle3 p0.y");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint0.z, 0.0f, "Triangle3 p0.z");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint1.x, 4.0f, "Triangle3 p1.x");
	MathStructTest2Support_MST2_ASSERT_F(t1.mPoint2.z, 3.0f, "Triangle3 p2.z");

	// 3D 空间中的三角形
	Triangle3 t2({ 1.0f, 2.0f, 3.0f }, { 4.0f, 5.0f, 6.0f }, { 7.0f, 8.0f, 9.0f });
	MathStructTest2Support_MST2_ASSERT_F(t2.mPoint0.x, 1.0f, "Triangle3 3D p0.x");
	MathStructTest2Support_MST2_ASSERT_F(t2.mPoint1.y, 5.0f, "Triangle3 3D p1.y");
	MathStructTest2Support_MST2_ASSERT_F(t2.mPoint2.z, 9.0f, "Triangle3 3D p2.z");

	// 负坐标
	Triangle3 t3({ -1.0f, -2.0f, -3.0f }, { -4.0f, -5.0f, -6.0f }, { -7.0f, -8.0f, -9.0f });
	MathStructTest2Support_MST2_ASSERT_F(t3.mPoint0.x, -1.0f, "Triangle3 neg p0.x");
	MathStructTest2Support_MST2_ASSERT_F(t3.mPoint2.z, -9.0f, "Triangle3 neg p2.z");

	// 默认构造（不崩溃）
	Triangle3 t_default;
	MathStructTest2Support_MST2_ASSERT(true, "Triangle3 default ctor no crash");
}

void MathStructTest::testGeometryScenario()
{
	using namespace MathStructTest2Support;
	// 场景1：判断点是否在 Rect 内
	Rect zone(0.0f, 0.0f, 100.0f, 100.0f);
	Vector2 pt1 = { 50.0f, 50.0f };
	Vector2 pt2 = { 150.0f, 50.0f };

	Vector2 mn = zone.getMin();
	Vector2 mx = zone.getMax();
	bool inZone1 = (pt1.x >= mn.x && pt1.x <= mx.x && pt1.y >= mn.y && pt1.y <= mx.y);
	bool inZone2 = (pt2.x >= mn.x && pt2.x <= mx.x && pt2.y >= mn.y && pt2.y <= mx.y);
	MathStructTest2Support_MST2_ASSERT(inZone1, "scenario: pt1 in zone");
	MathStructTest2Support_MST2_ASSERT(!inZone2, "scenario: pt2 not in zone");

	// 场景2：两个 Rect 是否重叠
	Rect r1(0.0f, 0.0f, 10.0f, 10.0f);
	Rect r2(5.0f, 5.0f, 10.0f, 10.0f);  // 重叠
	Rect r3(20.0f, 20.0f, 5.0f, 5.0f);  // 不重叠

	auto overlap = [](const Rect& a, const Rect& b) -> bool {
	    return a.getMin().x < b.getMax().x && a.getMax().x > b.getMin().x &&
	        a.getMin().y < b.getMax().y && a.getMax().y > b.getMin().y;
	    };

	MathStructTest2Support_MST2_ASSERT(overlap(r1, r2), "scenario: r1 and r2 overlap");
	MathStructTest2Support_MST2_ASSERT(!overlap(r1, r3), "scenario: r1 and r3 no overlap");

	// 场景3：用 Line2D 计算轨迹上的点
	// 子弹从 (0,0) 射向 (100,50)，在 x=20 时的 y 坐标
	Line2D trajectory({ 0.0f, 0.0f }, { 100.0f, 50.0f });
	float bulletY = 0.0f;
	trajectory.getPointYOnLine(20.0f, bulletY);
	// y = (50/100) * 20 = 10
	MathStructTest2Support_MST2_ASSERT_F(bulletY, 10.0f, "scenario: bullet y at x=20");

	// 场景4：Rect3 范围检测
	Rect3 battleZone({ -50.0f, 0.0f, -50.0f }, { 50.0f, 100.0f, 50.0f });
	Vector3 center = battleZone.getCenter();
	MathStructTest2Support_MST2_ASSERT_F(center.x, 0.0f, "scenario: battleZone center.x==0");
	MathStructTest2Support_MST2_ASSERT_F(center.y, 50.0f, "scenario: battleZone center.y==50");
	MathStructTest2Support_MST2_ASSERT_F(center.z, 0.0f, "scenario: battleZone center.z==0");
}

void MathStructTest::testGeometryPrecision()
{
	using namespace MathStructTest2Support;
	// 非整数坐标的精度
	Rect r(0.1f, 0.2f, 3.3f, 4.4f);
	MathStructTest2Support_MST2_ASSERT_F(r.getMax().x, 3.4f, "precision Rect getMax.x");
	MathStructTest2Support_MST2_ASSERT_F(r.getMax().y, 4.6f, "precision Rect getMax.y");
	MathStructTest2Support_MST2_ASSERT_F(r.getCenter().x, 1.75f, "precision Rect center.x");

	// 3-4-5 直角三角形对角线
	Rect rect345(0.0f, 0.0f, 3.0f, 4.0f);
	MathStructTest2Support_MST2_ASSERT_F(rect345.getDiagonal(), 5.0f, "precision 3-4-5 diagonal");

	// Line2D 精度
	Line2D ld({ 0.0f, 0.0f }, { 3.0f, 1.0f });
	// k = 1/3, b = 0
	float y = 0.0f;
	ld.getPointYOnLine(1.5f, y);
	// y = (1/3)*1.5 = 0.5
	MathStructTest2Support_MST2_ASSERT_F(y, 0.5f, "precision Line2D y at x=1.5");
}

namespace MathStructTest3Support
{
#define MathStructTest3Support_MS3_ASSERT(expr, msg) if (!(expr)) { ERROR(string("MathStructTest3 FAILED: ") + (msg)); }
#define MathStructTest3Support_MS3_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)
}

void MathStructTest::testVector4_DefaultCtor()
{
	using namespace MathStructTest3Support;
	Vector4 v;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 0.0f), "V4 default x==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.y, 0.0f), "V4 default y==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.z, 0.0f), "V4 default z==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 0.0f), "V4 default w==0");
}

void MathStructTest::testVector4_ParamCtor()
{
	using namespace MathStructTest3Support;
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 1.0f), "V4 ctor x==1");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.y, 2.0f), "V4 ctor y==2");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.z, 3.0f), "V4 ctor z==3");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 4.0f), "V4 ctor w==4");
}

void MathStructTest::testVector4_Clear()
{
	using namespace MathStructTest3Support;
	Vector4 v(10.0f, 20.0f, 30.0f, 40.0f);
	v.clear();
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 0.0f), "V4 clear x==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.y, 0.0f), "V4 clear y==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.z, 0.0f), "V4 clear z==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 0.0f), "V4 clear w==0");
}

void MathStructTest::testVector4_Add()
{
	using namespace MathStructTest3Support;
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(10.0f, 20.0f, 30.0f, 40.0f);
	Vector4 c = a + b;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.x, 11.0f), "V4 add x==11");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.y, 22.0f), "V4 add y==22");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.z, 33.0f), "V4 add z==33");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.w, 44.0f), "V4 add w==44");
}

void MathStructTest::testVector4_Sub()
{
	using namespace MathStructTest3Support;
	Vector4 a(10.0f, 20.0f, 30.0f, 40.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 c = a - b;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.x, 9.0f), "V4 sub x==9");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.y, 18.0f), "V4 sub y==18");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.z, 27.0f), "V4 sub z==27");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.w, 36.0f), "V4 sub w==36");
}

void MathStructTest::testVector4_AddAssign()
{
	using namespace MathStructTest3Support;
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	a += Vector4(10.0f, 10.0f, 10.0f, 10.0f);
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.x, 11.0f), "V4 += x==11");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.y, 12.0f), "V4 += y==12");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.z, 13.0f), "V4 += z==13");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.w, 14.0f), "V4 += w==14");
}

void MathStructTest::testVector4_SubAssign()
{
	using namespace MathStructTest3Support;
	Vector4 a(10.0f, 20.0f, 30.0f, 40.0f);
	a -= Vector4(1.0f, 2.0f, 3.0f, 4.0f);
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.x, 9.0f), "V4 -= x==9");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.y, 18.0f), "V4 -= y==18");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.z, 27.0f), "V4 -= z==27");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(a.w, 36.0f), "V4 -= w==36");
}

void MathStructTest::testVector4_Negate()
{
	using namespace MathStructTest3Support;
	Vector4 a(1.0f, -2.0f, 3.0f, -4.0f);
	Vector4 n = -a;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(n.x, -1.0f), "V4 negate x==-1");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(n.y, 2.0f), "V4 negate y==2");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(n.z, -3.0f), "V4 negate z==-3");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(n.w, 4.0f), "V4 negate w==4");
}

void MathStructTest::testVector4_AddSubChain()
{
	using namespace MathStructTest3Support;
	Vector4 v(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 0; i < 10; ++i)
	{
	    v += Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 11.0f), "V4 chain add x==11");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 11.0f), "V4 chain add w==11");
	for (int i = 0; i < 10; ++i)
	{
	    v -= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 1.0f), "V4 chain sub x==1");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 1.0f), "V4 chain sub w==1");
}

void MathStructTest::testVector4_NegativeValues()
{
	using namespace MathStructTest3Support;
	Vector4 a(-1.0f, -2.0f, -3.0f, -4.0f);
	Vector4 b(-5.0f, -6.0f, -7.0f, -8.0f);
	Vector4 c = a + b;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.x, -6.0f), "V4 neg add x==-6");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.w, -12.0f), "V4 neg add w==-12");
	Vector4 d = a - b;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(d.x, 4.0f), "V4 neg sub x==4");
}

void MathStructTest::testVector4_ZeroAdd()
{
	using namespace MathStructTest3Support;
	Vector4 v(5.0f, 6.0f, 7.0f, 8.0f);
	Vector4 z;
	Vector4 r = v + z;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(r.x, 5.0f), "V4 add zero x unchanged");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(r.w, 8.0f), "V4 add zero w unchanged");
}

void MathStructTest::testVector4_SelfAddSub()
{
	using namespace MathStructTest3Support;
	Vector4 a(3.0f, 4.0f, 5.0f, 6.0f);
	Vector4 b = a + a;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(b.x, 6.0f), "V4 self add x==6");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(b.w, 12.0f), "V4 self add w==12");
	Vector4 c = b - a;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(c.x, 3.0f), "V4 self sub x==3");
}

void MathStructTest::testVector4_LargeValues()
{
	using namespace MathStructTest3Support;
	Vector4 a(1e6f, 2e6f, 3e6f, 4e6f);
	Vector4 b = a + a;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(b.x / 1e6f, 2.0f), "V4 large x==2M");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(b.w / 1e6f, 8.0f), "V4 large w==8M");
}

void MathStructTest::testVector4_AddAssignChain()
{
	using namespace MathStructTest3Support;
	Vector4 v;
	Vector4 step(1.0f, 2.0f, 3.0f, 4.0f);
	for (int i = 0; i < 5; ++i)
	{
	    v += step;
	}
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 5.0f), "V4 chain+= x==5");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.y, 10.0f), "V4 chain+= y==10");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.z, 15.0f), "V4 chain+= z==15");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 20.0f), "V4 chain+= w==20");
}

void MathStructTest::testVector4_SubAssignChain()
{
	using namespace MathStructTest3Support;
	Vector4 v(100.0f, 100.0f, 100.0f, 100.0f);
	Vector4 step(10.0f, 10.0f, 10.0f, 10.0f);
	for (int i = 0; i < 10; ++i)
	{
	    v -= step;
	}
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.x, 0.0f), "V4 chain-= x==0");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(v.w, 0.0f), "V4 chain-= w==0");
}

void MathStructTest::testVector4_CopyCtor()
{
	using namespace MathStructTest3Support;
	Vector4 src(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 dst(src);
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(dst.x, 1.0f), "V4 copy ctor x");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(dst.w, 4.0f), "V4 copy ctor w");
	src.x = 99.0f;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(dst.x, 1.0f), "V4 copy ctor independent");
}

void MathStructTest::testVector4_AssignOp()
{
	using namespace MathStructTest3Support;
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b;
	b = a;
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(b.x, 1.0f), "V4 assign x");
	MathStructTest3Support_MS3_ASSERT(MathStructTest3Support_MS3_FLOAT_EQ(b.w, 4.0f), "V4 assign w");
}

void MathStructTest::testVector4Int_DefaultCtor()
{
	using namespace MathStructTest3Support;
	Vector4Int v;
	MathStructTest3Support_MS3_ASSERT(v.x == 0, "V4I default x==0");
	MathStructTest3Support_MS3_ASSERT(v.y == 0, "V4I default y==0");
	MathStructTest3Support_MS3_ASSERT(v.z == 0, "V4I default z==0");
	MathStructTest3Support_MS3_ASSERT(v.w == 0, "V4I default w==0");
}

void MathStructTest::testVector4Int_ParamCtor()
{
	using namespace MathStructTest3Support;
	Vector4Int v(1, 2, 3, 4);
	MathStructTest3Support_MS3_ASSERT(v.x == 1, "V4I ctor x==1");
	MathStructTest3Support_MS3_ASSERT(v.y == 2, "V4I ctor y==2");
	MathStructTest3Support_MS3_ASSERT(v.z == 3, "V4I ctor z==3");
	MathStructTest3Support_MS3_ASSERT(v.w == 4, "V4I ctor w==4");
}

void MathStructTest::testVector4Int_Clear()
{
	using namespace MathStructTest3Support;
	Vector4Int v(10, 20, 30, 40);
	v.clear();
	MathStructTest3Support_MS3_ASSERT(v.x == 0, "V4I clear x==0");
	MathStructTest3Support_MS3_ASSERT(v.y == 0, "V4I clear y==0");
	MathStructTest3Support_MS3_ASSERT(v.z == 0, "V4I clear z==0");
	MathStructTest3Support_MS3_ASSERT(v.w == 0, "V4I clear w==0");
}

void MathStructTest::testVector4Int_Add()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(10, 20, 30, 40);
	Vector4Int c = a + b;
	MathStructTest3Support_MS3_ASSERT(c.x == 11, "V4I add x==11");
	MathStructTest3Support_MS3_ASSERT(c.y == 22, "V4I add y==22");
	MathStructTest3Support_MS3_ASSERT(c.z == 33, "V4I add z==33");
	MathStructTest3Support_MS3_ASSERT(c.w == 44, "V4I add w==44");
}

void MathStructTest::testVector4Int_Sub()
{
	using namespace MathStructTest3Support;
	Vector4Int a(100, 200, 300, 400);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c = a - b;
	MathStructTest3Support_MS3_ASSERT(c.x == 99, "V4I sub x==99");
	MathStructTest3Support_MS3_ASSERT(c.y == 198, "V4I sub y==198");
	MathStructTest3Support_MS3_ASSERT(c.z == 297, "V4I sub z==297");
	MathStructTest3Support_MS3_ASSERT(c.w == 396, "V4I sub w==396");
}

void MathStructTest::testVector4Int_AddAssign()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, 2, 3, 4);
	a += Vector4Int(10, 10, 10, 10);
	MathStructTest3Support_MS3_ASSERT(a.x == 11, "V4I += x==11");
	MathStructTest3Support_MS3_ASSERT(a.y == 12, "V4I += y==12");
	MathStructTest3Support_MS3_ASSERT(a.z == 13, "V4I += z==13");
	MathStructTest3Support_MS3_ASSERT(a.w == 14, "V4I += w==14");
}

void MathStructTest::testVector4Int_SubAssign()
{
	using namespace MathStructTest3Support;
	Vector4Int a(20, 30, 40, 50);
	a -= Vector4Int(5, 5, 5, 5);
	MathStructTest3Support_MS3_ASSERT(a.x == 15, "V4I -= x==15");
	MathStructTest3Support_MS3_ASSERT(a.y == 25, "V4I -= y==25");
	MathStructTest3Support_MS3_ASSERT(a.z == 35, "V4I -= z==35");
	MathStructTest3Support_MS3_ASSERT(a.w == 45, "V4I -= w==45");
}

void MathStructTest::testVector4Int_Negate()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, -2, 3, -4);
	Vector4Int n = -a;
	MathStructTest3Support_MS3_ASSERT(n.x == -1, "V4I negate x==-1");
	MathStructTest3Support_MS3_ASSERT(n.y == 2, "V4I negate y==2");
	MathStructTest3Support_MS3_ASSERT(n.z == -3, "V4I negate z==-3");
	MathStructTest3Support_MS3_ASSERT(n.w == 4, "V4I negate w==4");
}

void MathStructTest::testVector4Int_MulScalar()
{
	using namespace MathStructTest3Support;
	Vector4Int a(2, 3, 4, 5);
	Vector4Int r = a * 10;
	MathStructTest3Support_MS3_ASSERT(r.x == 20, "V4I mul x==20");
	MathStructTest3Support_MS3_ASSERT(r.y == 30, "V4I mul y==30");
	MathStructTest3Support_MS3_ASSERT(r.z == 40, "V4I mul z==40");
	MathStructTest3Support_MS3_ASSERT(r.w == 50, "V4I mul w==50");
}

void MathStructTest::testVector4Int_MulByZero()
{
	using namespace MathStructTest3Support;
	Vector4Int a(7, 8, 9, 10);
	Vector4Int r = a * 0;
	MathStructTest3Support_MS3_ASSERT(r.x == 0, "V4I mul0 x==0");
	MathStructTest3Support_MS3_ASSERT(r.y == 0, "V4I mul0 y==0");
	MathStructTest3Support_MS3_ASSERT(r.z == 0, "V4I mul0 z==0");
	MathStructTest3Support_MS3_ASSERT(r.w == 0, "V4I mul0 w==0");
}

void MathStructTest::testVector4Int_MulByOne()
{
	using namespace MathStructTest3Support;
	Vector4Int a(3, 4, 5, 6);
	Vector4Int r = a * 1;
	MathStructTest3Support_MS3_ASSERT(r.x == 3, "V4I mul1 x unchanged");
	MathStructTest3Support_MS3_ASSERT(r.w == 6, "V4I mul1 w unchanged");
}

void MathStructTest::testVector4Int_MulByNeg()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, 2, 3, 4);
	Vector4Int r = a * (-3);
	MathStructTest3Support_MS3_ASSERT(r.x == -3, "V4I mulNeg x==-3");
	MathStructTest3Support_MS3_ASSERT(r.y == -6, "V4I mulNeg y==-6");
	MathStructTest3Support_MS3_ASSERT(r.z == -9, "V4I mulNeg z==-9");
	MathStructTest3Support_MS3_ASSERT(r.w == -12, "V4I mulNeg w==-12");
}

void MathStructTest::testVector4Int_Equality()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c(1, 2, 3, 5);
	MathStructTest3Support_MS3_ASSERT(a == b, "V4I eq same");
	MathStructTest3Support_MS3_ASSERT(!(a == c), "V4I eq diff");
	MathStructTest3Support_MS3_ASSERT(a != c, "V4I neq diff");
	MathStructTest3Support_MS3_ASSERT(!(a != b), "V4I not neq same");
}

void MathStructTest::testVector4Int_LessThan()
{
	using namespace MathStructTest3Support;
	// < 按 x+y+z+w 比较
	Vector4Int a(1, 1, 1, 1); // sum=4
	Vector4Int b(2, 2, 2, 2); // sum=8
	MathStructTest3Support_MS3_ASSERT(a < b, "V4I < a<b");
	MathStructTest3Support_MS3_ASSERT(!(b < a), "V4I < !(b<a)");
}

void MathStructTest::testVector4Int_LessThan_Equal()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, 2, 3, 4); // sum=10
	Vector4Int b(4, 3, 2, 1); // sum=10
	MathStructTest3Support_MS3_ASSERT(!(a < b), "V4I < equal not less");
	MathStructTest3Support_MS3_ASSERT(!(b < a), "V4I < equal not less reverse");
}

void MathStructTest::testVector4Int_NegativeValues()
{
	using namespace MathStructTest3Support;
	Vector4Int a(-10, -20, -30, -40);
	Vector4Int b(10, 20, 30, 40);
	Vector4Int c = a + b;
	MathStructTest3Support_MS3_ASSERT(c.x == 0, "V4I neg+pos x==0");
	MathStructTest3Support_MS3_ASSERT(c.y == 0, "V4I neg+pos y==0");
	MathStructTest3Support_MS3_ASSERT(c.z == 0, "V4I neg+pos z==0");
	MathStructTest3Support_MS3_ASSERT(c.w == 0, "V4I neg+pos w==0");
}

void MathStructTest::testVector4Int_Chain()
{
	using namespace MathStructTest3Support;
	Vector4Int v;
	for (int i = 0; i < 10; ++i)
	{
	    v += Vector4Int(1, 2, 3, 4);
	}
	MathStructTest3Support_MS3_ASSERT(v.x == 10, "V4I chain x==10");
	MathStructTest3Support_MS3_ASSERT(v.y == 20, "V4I chain y==20");
	MathStructTest3Support_MS3_ASSERT(v.z == 30, "V4I chain z==30");
	MathStructTest3Support_MS3_ASSERT(v.w == 40, "V4I chain w==40");
}

void MathStructTest::testVector4Int_UseInHashMap()
{
	using namespace MathStructTest3Support;
	// Vector4Int 本身没有 hash，但可以用 Map（有序）
	Map<int, Vector4Int> m;
	m.add(1, Vector4Int(1, 2, 3, 4));
	m.add(2, Vector4Int(5, 6, 7, 8));
	MathStructTest3Support_MS3_ASSERT(m.contains(1), "V4I in Map contains 1");
	const Vector4Int& v = m.get(1);
	MathStructTest3Support_MS3_ASSERT(v == Vector4Int(1, 2, 3, 4), "V4I in Map value");
}

void MathStructTest::testVector4Int_SumOfComponents()
{
	using namespace MathStructTest3Support;
	Vector4Int a(1, 2, 3, 4);
	int sum = a.x + a.y + a.z + a.w;
	MathStructTest3Support_MS3_ASSERT(sum == 10, "V4I sum of components==10");
}

void MathStructTest::testVector2Int_DefaultCtor()
{
	using namespace MathStructTest3Support;
	Vector2Int v;
	MathStructTest3Support_MS3_ASSERT(v.x == 0, "V2I default x==0");
	MathStructTest3Support_MS3_ASSERT(v.y == 0, "V2I default y==0");
}

void MathStructTest::testVector2Int_ParamCtor()
{
	using namespace MathStructTest3Support;
	Vector2Int v(5, 7);
	MathStructTest3Support_MS3_ASSERT(v.x == 5, "V2I ctor x==5");
	MathStructTest3Support_MS3_ASSERT(v.y == 7, "V2I ctor y==7");
}

void MathStructTest::testVector2Int_Clear()
{
	using namespace MathStructTest3Support;
	Vector2Int v(100, 200);
	v.clear();
	MathStructTest3Support_MS3_ASSERT(v.x == 0, "V2I clear x==0");
	MathStructTest3Support_MS3_ASSERT(v.y == 0, "V2I clear y==0");
}

void MathStructTest::testVector2Int_Add()
{
	using namespace MathStructTest3Support;
	Vector2Int a(3, 4);
	Vector2Int b(7, 6);
	Vector2Int c = a + b;
	MathStructTest3Support_MS3_ASSERT(c.x == 10, "V2I add x==10");
	MathStructTest3Support_MS3_ASSERT(c.y == 10, "V2I add y==10");
}

void MathStructTest::testVector2Int_Sub()
{
	using namespace MathStructTest3Support;
	Vector2Int a(10, 15);
	Vector2Int b(3, 5);
	Vector2Int c = a - b;
	MathStructTest3Support_MS3_ASSERT(c.x == 7, "V2I sub x==7");
	MathStructTest3Support_MS3_ASSERT(c.y == 10, "V2I sub y==10");
}

void MathStructTest::testVector2Int_AddAssign()
{
	using namespace MathStructTest3Support;
	Vector2Int a(1, 2);
	a += Vector2Int(3, 4);
	MathStructTest3Support_MS3_ASSERT(a.x == 4, "V2I += x==4");
	MathStructTest3Support_MS3_ASSERT(a.y == 6, "V2I += y==6");
}

void MathStructTest::testVector2Int_SubAssign()
{
	using namespace MathStructTest3Support;
	Vector2Int a(10, 20);
	a -= Vector2Int(3, 5);
	MathStructTest3Support_MS3_ASSERT(a.x == 7, "V2I -= x==7");
	MathStructTest3Support_MS3_ASSERT(a.y == 15, "V2I -= y==15");
}

void MathStructTest::testVector2Int_Negate()
{
	using namespace MathStructTest3Support;
	Vector2Int a(3, -5);
	Vector2Int n = -a;
	MathStructTest3Support_MS3_ASSERT(n.x == -3, "V2I negate x==-3");
	MathStructTest3Support_MS3_ASSERT(n.y == 5, "V2I negate y==5");
}

void MathStructTest::testVector2Int_MulScalar()
{
	using namespace MathStructTest3Support;
	Vector2Int a(4, 5);
	Vector2Int r = a * 3;
	MathStructTest3Support_MS3_ASSERT(r.x == 12, "V2I mul x==12");
	MathStructTest3Support_MS3_ASSERT(r.y == 15, "V2I mul y==15");
}

void MathStructTest::testVector2Int_MulByZero()
{
	using namespace MathStructTest3Support;
	Vector2Int a(9, 11);
	Vector2Int r = a * 0;
	MathStructTest3Support_MS3_ASSERT(r.x == 0, "V2I mul0 x==0");
	MathStructTest3Support_MS3_ASSERT(r.y == 0, "V2I mul0 y==0");
}

void MathStructTest::testVector2Int_MulByNeg()
{
	using namespace MathStructTest3Support;
	Vector2Int a(3, 4);
	Vector2Int r = a * (-2);
	MathStructTest3Support_MS3_ASSERT(r.x == -6, "V2I mulNeg x==-6");
	MathStructTest3Support_MS3_ASSERT(r.y == -8, "V2I mulNeg y==-8");
}

void MathStructTest::testVector2Int_Equality()
{
	using namespace MathStructTest3Support;
	Vector2Int a(3, 4);
	Vector2Int b(3, 4);
	Vector2Int c(3, 5);
	MathStructTest3Support_MS3_ASSERT(a == b, "V2I eq same");
	MathStructTest3Support_MS3_ASSERT(!(a == c), "V2I eq diff");
	MathStructTest3Support_MS3_ASSERT(a != c, "V2I neq diff");
	MathStructTest3Support_MS3_ASSERT(!(a != b), "V2I not neq same");
}

void MathStructTest::testVector2Int_LessThan()
{
	using namespace MathStructTest3Support;
	// < 按 x+y 比较
	Vector2Int a(1, 1); // sum=2
	Vector2Int b(2, 3); // sum=5
	MathStructTest3Support_MS3_ASSERT(a < b, "V2I < a<b");
	MathStructTest3Support_MS3_ASSERT(!(b < a), "V2I < !(b<a)");
}

void MathStructTest::testVector2Int_LessThan_Equal()
{
	using namespace MathStructTest3Support;
	Vector2Int a(3, 2); // sum=5
	Vector2Int b(1, 4); // sum=5
	MathStructTest3Support_MS3_ASSERT(!(a < b), "V2I < eq not less");
	MathStructTest3Support_MS3_ASSERT(!(b < a), "V2I < eq not less rev");
}

void MathStructTest::testVector2Int_StaticZero()
{
	using namespace MathStructTest3Support;
	Vector2Int z = Vector2Int::ZERO;
	MathStructTest3Support_MS3_ASSERT(z.x == 0 && z.y == 0, "V2I ZERO");
}

void MathStructTest::testVector2Int_StdHash()
{
	using namespace MathStructTest3Support;
	// 测试 std::hash<Vector2Int> 可以在 unordered_map 中使用
	unordered_map<Vector2Int, int> hm;
	Vector2Int k1(1, 2);
	Vector2Int k2(3, 4);
	Vector2Int k3(1, 2); // same as k1
	hm[k1] = 100;
	hm[k2] = 200;
	hm[k3] = 300; // 覆盖 k1
	MathStructTest3Support_MS3_ASSERT(hm.count(k1) == 1, "V2I hash contains k1");
	MathStructTest3Support_MS3_ASSERT(hm[k1] == 300, "V2I hash k1 updated to 300");
	MathStructTest3Support_MS3_ASSERT(hm[k2] == 200, "V2I hash k2==200");
}

void MathStructTest::testVector2Int_StdHash_Distinct()
{
	using namespace MathStructTest3Support;
	// 不同坐标应有不同哈希（实际上无法保证100%，但常见值不同）
	std::hash<Vector2Int> h;
	size_t h3 = h(Vector2Int(1, 1));
	// h1/h2 可能不同（期望），h3 肯定不等于 h(ZERO)
	size_t h0 = h(Vector2Int(0, 0));
	MathStructTest3Support_MS3_ASSERT(h3 != h0, "V2I hash (1,1) != (0,0)");
}

void MathStructTest::testVector2Int_UseAsMapKey()
{
	using namespace MathStructTest3Support;
	// Vector2Int::operator< 用 x+y 排序，(1,0) 与 (0,1) 的 x+y 均为 1 → 视为相同 key
	// 使用 x+y 互不相同的 key 保证 3 个不同 key
	Map<Vector2Int, int> m;
	m.add(Vector2Int(0, 0), 1);  // x+y=0
	m.add(Vector2Int(1, 0), 2);  // x+y=1
	m.add(Vector2Int(2, 0), 3);  // x+y=2
	MathStructTest3Support_MS3_ASSERT(m.size() == 3, "V2I map key size==3");
	MathStructTest3Support_MS3_ASSERT(m.contains(Vector2Int(1, 0)), "V2I map key (1,0) found");
	MathStructTest3Support_MS3_ASSERT(m.get(Vector2Int(2, 0), -1) == 3, "V2I map key (2,0) val==3");
}

void MathStructTest::testVector2Int_Chain()
{
	using namespace MathStructTest3Support;
	Vector2Int v(0, 0);
	for (int i = 1; i <= 5; ++i)
	{
	    v += Vector2Int(i, i);
	}
	// sum: 1+2+3+4+5 = 15
	MathStructTest3Support_MS3_ASSERT(v.x == 15, "V2I chain x==15");
	MathStructTest3Support_MS3_ASSERT(v.y == 15, "V2I chain y==15");
	for (int i = 1; i <= 5; ++i)
	{
	    v -= Vector2Int(i, i);
	}
	MathStructTest3Support_MS3_ASSERT(v.x == 0, "V2I chain sub x==0");
	MathStructTest3Support_MS3_ASSERT(v.y == 0, "V2I chain sub y==0");
}

void MathStructTest::testVector2Int_NegativeCoords()
{
	using namespace MathStructTest3Support;
	Vector2Int a(-5, -3);
	Vector2Int b(2, 1);
	Vector2Int c = a + b;
	MathStructTest3Support_MS3_ASSERT(c.x == -3, "V2I negCoords x==-3");
	MathStructTest3Support_MS3_ASSERT(c.y == -2, "V2I negCoords y==-2");
	Vector2Int n = -a;
	MathStructTest3Support_MS3_ASSERT(n.x == 5, "V2I negCoords neg x==5");
	MathStructTest3Support_MS3_ASSERT(n.y == 3, "V2I negCoords neg y==3");
}

namespace MathStructTest4Support
{
#define MathStructTest4Support_MST4_ASSERT(expr) \
	if (!(expr)) { ERROR(string("MathStructTest4 FAIL: ") + #expr); }
}

void MathStructTest::testVector2IntDefault()
{
	using namespace MathStructTest4Support;
	Vector2Int v;
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
}

void MathStructTest::testVector2IntConstruct4()
{
	using namespace MathStructTest4Support;
	Vector2Int v(3, -7);
	MathStructTest4Support_MST4_ASSERT(v.x == 3);
	MathStructTest4Support_MST4_ASSERT(v.y == -7);
	Vector2Int v2(-100, 200);
	MathStructTest4Support_MST4_ASSERT(v2.x == -100);
	MathStructTest4Support_MST4_ASSERT(v2.y == 200);
	Vector2Int v3(0, 0);
	MathStructTest4Support_MST4_ASSERT(v3.x == 0);
	MathStructTest4Support_MST4_ASSERT(v3.y == 0);
}

void MathStructTest::testVector2IntAdd()
{
	using namespace MathStructTest4Support;
	Vector2Int a(1, 2);
	Vector2Int b(3, 4);
	Vector2Int c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 4);
	MathStructTest4Support_MST4_ASSERT(c.y == 6);
	Vector2Int d(-5, 10);
	Vector2Int e(5, -10);
	Vector2Int f = d + e;
	MathStructTest4Support_MST4_ASSERT(f.x == 0);
	MathStructTest4Support_MST4_ASSERT(f.y == 0);
	Vector2Int g(0, 0);
	Vector2Int h(7, -3);
	Vector2Int i2 = g + h;
	MathStructTest4Support_MST4_ASSERT(i2.x == 7);
	MathStructTest4Support_MST4_ASSERT(i2.y == -3);
}

void MathStructTest::testVector2IntSub()
{
	using namespace MathStructTest4Support;
	Vector2Int a(10, 5);
	Vector2Int b(3, 2);
	Vector2Int c = a - b;
	MathStructTest4Support_MST4_ASSERT(c.x == 7);
	MathStructTest4Support_MST4_ASSERT(c.y == 3);
	Vector2Int d(0, 0);
	Vector2Int e(4, -4);
	Vector2Int f = d - e;
	MathStructTest4Support_MST4_ASSERT(f.x == -4);
	MathStructTest4Support_MST4_ASSERT(f.y == 4);
}

void MathStructTest::testVector2IntMul()
{
	using namespace MathStructTest4Support;
	Vector2Int a(3, -4);
	Vector2Int b = a * 5;
	MathStructTest4Support_MST4_ASSERT(b.x == 15);
	MathStructTest4Support_MST4_ASSERT(b.y == -20);
	Vector2Int c(7, 2);
	Vector2Int d = c * 0;
	MathStructTest4Support_MST4_ASSERT(d.x == 0);
	MathStructTest4Support_MST4_ASSERT(d.y == 0);
	Vector2Int e(-3, -3);
	Vector2Int f = e * (-2);
	MathStructTest4Support_MST4_ASSERT(f.x == 6);
	MathStructTest4Support_MST4_ASSERT(f.y == 6);
}

void MathStructTest::testVector2IntNeg()
{
	using namespace MathStructTest4Support;
	Vector2Int a(5, -8);
	Vector2Int b = -a;
	MathStructTest4Support_MST4_ASSERT(b.x == -5);
	MathStructTest4Support_MST4_ASSERT(b.y == 8);
	Vector2Int c(0, 0);
	Vector2Int d = -c;
	MathStructTest4Support_MST4_ASSERT(d.x == 0);
	MathStructTest4Support_MST4_ASSERT(d.y == 0);
}

void MathStructTest::testVector2IntAddAssign()
{
	using namespace MathStructTest4Support;
	Vector2Int a(1, 2);
	Vector2Int b(3, 4);
	a += b;
	MathStructTest4Support_MST4_ASSERT(a.x == 4);
	MathStructTest4Support_MST4_ASSERT(a.y == 6);
	a += Vector2Int(-4, -6);
	MathStructTest4Support_MST4_ASSERT(a.x == 0);
	MathStructTest4Support_MST4_ASSERT(a.y == 0);
}

void MathStructTest::testVector2IntSubAssign()
{
	using namespace MathStructTest4Support;
	Vector2Int a(10, 20);
	Vector2Int b(3, 5);
	a -= b;
	MathStructTest4Support_MST4_ASSERT(a.x == 7);
	MathStructTest4Support_MST4_ASSERT(a.y == 15);
	a -= Vector2Int(7, 15);
	MathStructTest4Support_MST4_ASSERT(a.x == 0);
	MathStructTest4Support_MST4_ASSERT(a.y == 0);
}

void MathStructTest::testVector2IntEqual()
{
	using namespace MathStructTest4Support;
	Vector2Int a(5, 7);
	Vector2Int b(5, 7);
	Vector2Int c(5, 8);
	MathStructTest4Support_MST4_ASSERT(a == b);
	MathStructTest4Support_MST4_ASSERT(!(a == c));
	Vector2Int d(0, 0);
	Vector2Int e(0, 0);
	MathStructTest4Support_MST4_ASSERT(d == e);
}

void MathStructTest::testVector2IntNotEqual()
{
	using namespace MathStructTest4Support;
	Vector2Int a(1, 2);
	Vector2Int b(1, 3);
	Vector2Int c(1, 2);
	MathStructTest4Support_MST4_ASSERT(a != b);
	MathStructTest4Support_MST4_ASSERT(!(a != c));
}

void MathStructTest::testVector2IntLess4()
{
	using namespace MathStructTest4Support;
	Vector2Int a(1, 1);
	Vector2Int b(2, 2);
	MathStructTest4Support_MST4_ASSERT(a < b);
	MathStructTest4Support_MST4_ASSERT(!(b < a));
	Vector2Int c(3, 0);
	Vector2Int d(1, 2);
	// c.x+c.y == 3, d.x+d.y == 3 => not less
	MathStructTest4Support_MST4_ASSERT(!(c < d));
	MathStructTest4Support_MST4_ASSERT(!(d < c));
}

void MathStructTest::testVector2IntClear4()
{
	using namespace MathStructTest4Support;
	Vector2Int v(10, -5);
	v.clear();
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
}

void MathStructTest::testVector2IntZero()
{
	using namespace MathStructTest4Support;
	Vector2Int z = Vector2Int::ZERO;
	MathStructTest4Support_MST4_ASSERT(z.x == 0);
	MathStructTest4Support_MST4_ASSERT(z.y == 0);
}

void MathStructTest::testVector2IntHash()
{
	using namespace MathStructTest4Support;
	Vector2Int a(10, 20);
	Vector2Int b(10, 20);
	Vector2Int c(20, 10);
	std::hash<Vector2Int> h;
	MathStructTest4Support_MST4_ASSERT(h(a) == h(b));
	// Different coordinates may produce different hash (not guaranteed but typical)
	(void)h(c);
}

void MathStructTest::testVector2IntChainOps()
{
	using namespace MathStructTest4Support;
	Vector2Int a(1, 1);
	Vector2Int b = (a + Vector2Int(2, 3)) * 2 - Vector2Int(1, 1);
	// (1+2, 1+3) = (3,4); *2 = (6,8); -(1,1) = (5,7)
	MathStructTest4Support_MST4_ASSERT(b.x == 5);
	MathStructTest4Support_MST4_ASSERT(b.y == 7);
}

void MathStructTest::testVector2IntBoundary()
{
	using namespace MathStructTest4Support;
	Vector2Int a(2147483647, 2147483647);
	Vector2Int b(-2147483647, -2147483647);
	MathStructTest4Support_MST4_ASSERT(a.x == 2147483647);
	MathStructTest4Support_MST4_ASSERT(a.y == 2147483647);
	MathStructTest4Support_MST4_ASSERT(b.x == -2147483647);
	MathStructTest4Support_MST4_ASSERT(b.y == -2147483647);
	Vector2Int c = a + Vector2Int(-2147483647, -2147483647);
	MathStructTest4Support_MST4_ASSERT(c.x == 0);
	MathStructTest4Support_MST4_ASSERT(c.y == 0);
}

void MathStructTest::testVector2UIntDefault()
{
	using namespace MathStructTest4Support;
	Vector2UInt v;
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
}

void MathStructTest::testVector2UIntConstruct()
{
	using namespace MathStructTest4Support;
	Vector2UInt v(100u, 200u);
	MathStructTest4Support_MST4_ASSERT(v.x == 100u);
	MathStructTest4Support_MST4_ASSERT(v.y == 200u);
	Vector2UInt v2(0u, 0u);
	MathStructTest4Support_MST4_ASSERT(v2.x == 0u);
	MathStructTest4Support_MST4_ASSERT(v2.y == 0u);
}

void MathStructTest::testVector2UIntAdd()
{
	using namespace MathStructTest4Support;
	Vector2UInt a(3u, 5u);
	Vector2UInt b(7u, 9u);
	Vector2UInt c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 10u);
	MathStructTest4Support_MST4_ASSERT(c.y == 14u);
	Vector2UInt d(0u, 0u);
	Vector2UInt e(100u, 200u);
	Vector2UInt f = d + e;
	MathStructTest4Support_MST4_ASSERT(f.x == 100u);
	MathStructTest4Support_MST4_ASSERT(f.y == 200u);
}

void MathStructTest::testVector2UIntSub()
{
	using namespace MathStructTest4Support;
	Vector2UInt a(10u, 20u);
	Vector2UInt b(3u, 7u);
	Vector2UInt c = a - b;
	MathStructTest4Support_MST4_ASSERT(c.x == 7u);
	MathStructTest4Support_MST4_ASSERT(c.y == 13u);
	Vector2UInt d(5u, 5u);
	Vector2UInt e(5u, 5u);
	Vector2UInt f = d - e;
	MathStructTest4Support_MST4_ASSERT(f.x == 0u);
	MathStructTest4Support_MST4_ASSERT(f.y == 0u);
}

void MathStructTest::testVector2UIntEqual()
{
	using namespace MathStructTest4Support;
	Vector2UInt a(7u, 8u);
	Vector2UInt b(7u, 8u);
	Vector2UInt c(7u, 9u);
	MathStructTest4Support_MST4_ASSERT(a == b);
	MathStructTest4Support_MST4_ASSERT(!(a == c));
}

void MathStructTest::testVector2UIntNotEqual()
{
	using namespace MathStructTest4Support;
	Vector2UInt a(1u, 2u);
	Vector2UInt b(1u, 3u);
	Vector2UInt c(1u, 2u);
	MathStructTest4Support_MST4_ASSERT(a != b);
	MathStructTest4Support_MST4_ASSERT(!(a != c));
}

void MathStructTest::testVector2UIntClear()
{
	using namespace MathStructTest4Support;
	Vector2UInt v(500u, 600u);
	v.clear();
	MathStructTest4Support_MST4_ASSERT(v.x == 0u);
	MathStructTest4Support_MST4_ASSERT(v.y == 0u);
}

void MathStructTest::testVector2UIntLarge()
{
	using namespace MathStructTest4Support;
	Vector2UInt a(4294967295u, 0u);
	MathStructTest4Support_MST4_ASSERT(a.x == 4294967295u);
	MathStructTest4Support_MST4_ASSERT(a.y == 0u);
	Vector2UInt b(0u, 4294967295u);
	MathStructTest4Support_MST4_ASSERT(b.x == 0u);
	MathStructTest4Support_MST4_ASSERT(b.y == 4294967295u);
}

void MathStructTest::testVector2ShortDefault()
{
	using namespace MathStructTest4Support;
	Vector2Short v;
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
}

void MathStructTest::testVector2ShortConstruct()
{
	using namespace MathStructTest4Support;
	Vector2Short v((short)100, (short)-200);
	MathStructTest4Support_MST4_ASSERT(v.x == 100);
	MathStructTest4Support_MST4_ASSERT(v.y == -200);
	Vector2Short v2((short)-1, (short)1);
	MathStructTest4Support_MST4_ASSERT(v2.x == -1);
	MathStructTest4Support_MST4_ASSERT(v2.y == 1);
}

void MathStructTest::testVector2ShortAddSub()
{
	using namespace MathStructTest4Support;
	Vector2Short a((short)10, (short)20);
	Vector2Short b((short)5, (short)-5);
	Vector2Short c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 15);
	MathStructTest4Support_MST4_ASSERT(c.y == 15);
	Vector2Short d = a - b;
	MathStructTest4Support_MST4_ASSERT(d.x == 5);
	MathStructTest4Support_MST4_ASSERT(d.y == 25);
	Vector2Short e((short)3, (short)4);
	Vector2Short neg = -e;
	MathStructTest4Support_MST4_ASSERT(neg.x == -3);
	MathStructTest4Support_MST4_ASSERT(neg.y == -4);
	e += Vector2Short((short)1, (short)1);
	MathStructTest4Support_MST4_ASSERT(e.x == 4);
	MathStructTest4Support_MST4_ASSERT(e.y == 5);
	e -= Vector2Short((short)4, (short)5);
	MathStructTest4Support_MST4_ASSERT(e.x == 0);
	MathStructTest4Support_MST4_ASSERT(e.y == 0);
}

void MathStructTest::testVector2ShortEqual()
{
	using namespace MathStructTest4Support;
	Vector2Short a((short)3, (short)4);
	Vector2Short b((short)3, (short)4);
	Vector2Short c((short)3, (short)5);
	MathStructTest4Support_MST4_ASSERT(a == b);
	MathStructTest4Support_MST4_ASSERT(!(a == c));
	MathStructTest4Support_MST4_ASSERT(a != c);
	MathStructTest4Support_MST4_ASSERT(!(a != b));
}

void MathStructTest::testVector2ShortBoundary()
{
	using namespace MathStructTest4Support;
	Vector2Short a((short)32767, (short)-32767);
	MathStructTest4Support_MST4_ASSERT(a.x == 32767);
	MathStructTest4Support_MST4_ASSERT(a.y == -32767);
	Vector2Short b((short)0, (short)0);
	MathStructTest4Support_MST4_ASSERT(b.x == 0);
	MathStructTest4Support_MST4_ASSERT(b.y == 0);
	a.clear();
	MathStructTest4Support_MST4_ASSERT(a.x == 0);
	MathStructTest4Support_MST4_ASSERT(a.y == 0);
}

void MathStructTest::testVector2UShortDefault()
{
	using namespace MathStructTest4Support;
	Vector2UShort v;
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
}

void MathStructTest::testVector2UShortConstruct()
{
	using namespace MathStructTest4Support;
	Vector2UShort v((ushort)1000, (ushort)2000);
	MathStructTest4Support_MST4_ASSERT(v.x == 1000);
	MathStructTest4Support_MST4_ASSERT(v.y == 2000);
	Vector2UShort v2((ushort)0, (ushort)65535);
	MathStructTest4Support_MST4_ASSERT(v2.x == 0);
	MathStructTest4Support_MST4_ASSERT(v2.y == 65535);
}

void MathStructTest::testVector2UShortAddSub()
{
	using namespace MathStructTest4Support;
	Vector2UShort a((ushort)100, (ushort)200);
	Vector2UShort b((ushort)50, (ushort)100);
	Vector2UShort c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 150);
	MathStructTest4Support_MST4_ASSERT(c.y == 300);
	Vector2UShort d = a - b;
	MathStructTest4Support_MST4_ASSERT(d.x == 50);
	MathStructTest4Support_MST4_ASSERT(d.y == 100);
	a += Vector2UShort((ushort)10, (ushort)20);
	MathStructTest4Support_MST4_ASSERT(a.x == 110);
	MathStructTest4Support_MST4_ASSERT(a.y == 220);
	a -= Vector2UShort((ushort)10, (ushort)20);
	MathStructTest4Support_MST4_ASSERT(a.x == 100);
	MathStructTest4Support_MST4_ASSERT(a.y == 200);
}

void MathStructTest::testVector2UShortEqual()
{
	using namespace MathStructTest4Support;
	Vector2UShort a((ushort)5, (ushort)6);
	Vector2UShort b((ushort)5, (ushort)6);
	Vector2UShort c((ushort)5, (ushort)7);
	MathStructTest4Support_MST4_ASSERT(a == b);
	MathStructTest4Support_MST4_ASSERT(!(a == c));
	MathStructTest4Support_MST4_ASSERT(a != c);
	MathStructTest4Support_MST4_ASSERT(!(a != b));
	a.clear();
	MathStructTest4Support_MST4_ASSERT(a.x == 0);
	MathStructTest4Support_MST4_ASSERT(a.y == 0);
}

void MathStructTest::testVector3IntDefault()
{
	using namespace MathStructTest4Support;
	Vector3Int v;
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
	MathStructTest4Support_MST4_ASSERT(v.z == 0);
}

void MathStructTest::testVector3IntConstruct()
{
	using namespace MathStructTest4Support;
	Vector3Int v(1, -2, 3);
	MathStructTest4Support_MST4_ASSERT(v.x == 1);
	MathStructTest4Support_MST4_ASSERT(v.y == -2);
	MathStructTest4Support_MST4_ASSERT(v.z == 3);
	Vector3Int v2(0, 0, 0);
	MathStructTest4Support_MST4_ASSERT(v2.x == 0);
	MathStructTest4Support_MST4_ASSERT(v2.y == 0);
	MathStructTest4Support_MST4_ASSERT(v2.z == 0);
	Vector3Int v3(-100, -200, -300);
	MathStructTest4Support_MST4_ASSERT(v3.x == -100);
	MathStructTest4Support_MST4_ASSERT(v3.y == -200);
	MathStructTest4Support_MST4_ASSERT(v3.z == -300);
}

void MathStructTest::testVector3IntAdd()
{
	using namespace MathStructTest4Support;
	Vector3Int a(1, 2, 3);
	Vector3Int b(4, 5, 6);
	Vector3Int c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 5);
	MathStructTest4Support_MST4_ASSERT(c.y == 7);
	MathStructTest4Support_MST4_ASSERT(c.z == 9);
	Vector3Int d(-1, -2, -3);
	Vector3Int e(1, 2, 3);
	Vector3Int f = d + e;
	MathStructTest4Support_MST4_ASSERT(f.x == 0);
	MathStructTest4Support_MST4_ASSERT(f.y == 0);
	MathStructTest4Support_MST4_ASSERT(f.z == 0);
	Vector3Int g(0, 0, 0);
	Vector3Int h(7, -3, 5);
	Vector3Int i2 = g + h;
	MathStructTest4Support_MST4_ASSERT(i2.x == 7);
	MathStructTest4Support_MST4_ASSERT(i2.y == -3);
	MathStructTest4Support_MST4_ASSERT(i2.z == 5);
}

void MathStructTest::testVector3IntSub()
{
	using namespace MathStructTest4Support;
	Vector3Int a(10, 20, 30);
	Vector3Int b(1, 2, 3);
	Vector3Int c = a - b;
	MathStructTest4Support_MST4_ASSERT(c.x == 9);
	MathStructTest4Support_MST4_ASSERT(c.y == 18);
	MathStructTest4Support_MST4_ASSERT(c.z == 27);
	Vector3Int d(5, 5, 5);
	Vector3Int e(5, 5, 5);
	Vector3Int f = d - e;
	MathStructTest4Support_MST4_ASSERT(f.x == 0);
	MathStructTest4Support_MST4_ASSERT(f.y == 0);
	MathStructTest4Support_MST4_ASSERT(f.z == 0);
	Vector3Int neg = -Vector3Int(3, -4, 5);
	MathStructTest4Support_MST4_ASSERT(neg.x == -3);
	MathStructTest4Support_MST4_ASSERT(neg.y == 4);
	MathStructTest4Support_MST4_ASSERT(neg.z == -5);
}

void MathStructTest::testVector3IntMul()
{
	using namespace MathStructTest4Support;
	Vector3Int a(2, -3, 4);
	Vector3Int b = a * 3;
	MathStructTest4Support_MST4_ASSERT(b.x == 6);
	MathStructTest4Support_MST4_ASSERT(b.y == -9);
	MathStructTest4Support_MST4_ASSERT(b.z == 12);
	Vector3Int c(5, 6, 7);
	Vector3Int d = c * 0;
	MathStructTest4Support_MST4_ASSERT(d.x == 0);
	MathStructTest4Support_MST4_ASSERT(d.y == 0);
	MathStructTest4Support_MST4_ASSERT(d.z == 0);
	Vector3Int e(-1, -2, -3);
	Vector3Int f = e * (-5);
	MathStructTest4Support_MST4_ASSERT(f.x == 5);
	MathStructTest4Support_MST4_ASSERT(f.y == 10);
	MathStructTest4Support_MST4_ASSERT(f.z == 15);
}

void MathStructTest::testVector3IntEqual()
{
	using namespace MathStructTest4Support;
	Vector3Int a(1, 2, 3);
	Vector3Int b(1, 2, 3);
	Vector3Int c(1, 2, 4);
	MathStructTest4Support_MST4_ASSERT(a == b);
	MathStructTest4Support_MST4_ASSERT(!(a == c));
	a += Vector3Int(1, 1, 1);
	MathStructTest4Support_MST4_ASSERT(a.x == 2);
	MathStructTest4Support_MST4_ASSERT(a.y == 3);
	MathStructTest4Support_MST4_ASSERT(a.z == 4);
	a -= Vector3Int(1, 1, 1);
	MathStructTest4Support_MST4_ASSERT(a.x == 1);
	MathStructTest4Support_MST4_ASSERT(a.y == 2);
	MathStructTest4Support_MST4_ASSERT(a.z == 3);
}

void MathStructTest::testVector3IntNotEqual()
{
	using namespace MathStructTest4Support;
	Vector3Int a(1, 1, 1);
	Vector3Int b(1, 1, 2);
	Vector3Int c(1, 1, 1);
	MathStructTest4Support_MST4_ASSERT(a != b);
	MathStructTest4Support_MST4_ASSERT(!(a != c));
}

void MathStructTest::testVector3IntClear()
{
	using namespace MathStructTest4Support;
	Vector3Int v(5, -3, 8);
	v.clear();
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
	MathStructTest4Support_MST4_ASSERT(v.z == 0);
}

void MathStructTest::testVector3IntZero()
{
	using namespace MathStructTest4Support;
	Vector3Int v(7, 8, 9);
	v.clear();
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
	MathStructTest4Support_MST4_ASSERT(v.z == 0);
}

void MathStructTest::testVector3IntBoundary()
{
	using namespace MathStructTest4Support;
	Vector3Int a(2147483647, -2147483647, 0);
	MathStructTest4Support_MST4_ASSERT(a.x == 2147483647);
	MathStructTest4Support_MST4_ASSERT(a.y == -2147483647);
	MathStructTest4Support_MST4_ASSERT(a.z == 0);
	Vector3Int b(-2147483647, 0, 2147483647);
	Vector3Int c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 0);
	MathStructTest4Support_MST4_ASSERT(c.y == -2147483647);
	MathStructTest4Support_MST4_ASSERT(c.z == 2147483647);
}

void MathStructTest::testVector3IntChain()
{
	using namespace MathStructTest4Support;
	Vector3Int a(1, 2, 3);
	Vector3Int b = (a + Vector3Int(1, 1, 1)) * 2 - Vector3Int(1, 1, 1);
	// (2,3,4)*2 = (4,6,8); -(1,1,1) = (3,5,7)
	MathStructTest4Support_MST4_ASSERT(b.x == 3);
	MathStructTest4Support_MST4_ASSERT(b.y == 5);
	MathStructTest4Support_MST4_ASSERT(b.z == 7);
	// less operator: x+y+z
	Vector3Int lo(1, 1, 1);
	Vector3Int hi(2, 2, 2);
	MathStructTest4Support_MST4_ASSERT(lo < hi);
	MathStructTest4Support_MST4_ASSERT(!(hi < lo));
}

void MathStructTest::testVector4IntDefault()
{
	using namespace MathStructTest4Support;
	Vector4Int v;
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
	MathStructTest4Support_MST4_ASSERT(v.z == 0);
	MathStructTest4Support_MST4_ASSERT(v.w == 0);
}

void MathStructTest::testVector4IntConstruct()
{
	using namespace MathStructTest4Support;
	Vector4Int v(1, -2, 3, -4);
	MathStructTest4Support_MST4_ASSERT(v.x == 1);
	MathStructTest4Support_MST4_ASSERT(v.y == -2);
	MathStructTest4Support_MST4_ASSERT(v.z == 3);
	MathStructTest4Support_MST4_ASSERT(v.w == -4);
	Vector4Int v2(0, 0, 0, 0);
	MathStructTest4Support_MST4_ASSERT(v2.x == 0);
	MathStructTest4Support_MST4_ASSERT(v2.y == 0);
	MathStructTest4Support_MST4_ASSERT(v2.z == 0);
	MathStructTest4Support_MST4_ASSERT(v2.w == 0);
}

void MathStructTest::testVector4IntAdd()
{
	using namespace MathStructTest4Support;
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(10, 20, 30, 40);
	Vector4Int c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 11);
	MathStructTest4Support_MST4_ASSERT(c.y == 22);
	MathStructTest4Support_MST4_ASSERT(c.z == 33);
	MathStructTest4Support_MST4_ASSERT(c.w == 44);
	Vector4Int d(-1, -2, -3, -4);
	Vector4Int e(1, 2, 3, 4);
	Vector4Int f = d + e;
	MathStructTest4Support_MST4_ASSERT(f.x == 0);
	MathStructTest4Support_MST4_ASSERT(f.y == 0);
	MathStructTest4Support_MST4_ASSERT(f.z == 0);
	MathStructTest4Support_MST4_ASSERT(f.w == 0);
}

void MathStructTest::testVector4IntSub()
{
	using namespace MathStructTest4Support;
	Vector4Int a(10, 20, 30, 40);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c = a - b;
	MathStructTest4Support_MST4_ASSERT(c.x == 9);
	MathStructTest4Support_MST4_ASSERT(c.y == 18);
	MathStructTest4Support_MST4_ASSERT(c.z == 27);
	MathStructTest4Support_MST4_ASSERT(c.w == 36);
	Vector4Int neg = -Vector4Int(1, -2, 3, -4);
	MathStructTest4Support_MST4_ASSERT(neg.x == -1);
	MathStructTest4Support_MST4_ASSERT(neg.y == 2);
	MathStructTest4Support_MST4_ASSERT(neg.z == -3);
	MathStructTest4Support_MST4_ASSERT(neg.w == 4);
}

void MathStructTest::testVector4IntMul()
{
	using namespace MathStructTest4Support;
	Vector4Int a(2, -3, 4, -5);
	Vector4Int b = a * 4;
	MathStructTest4Support_MST4_ASSERT(b.x == 8);
	MathStructTest4Support_MST4_ASSERT(b.y == -12);
	MathStructTest4Support_MST4_ASSERT(b.z == 16);
	MathStructTest4Support_MST4_ASSERT(b.w == -20);
	Vector4Int c(3, 3, 3, 3);
	Vector4Int d = c * 0;
	MathStructTest4Support_MST4_ASSERT(d.x == 0);
	MathStructTest4Support_MST4_ASSERT(d.y == 0);
	MathStructTest4Support_MST4_ASSERT(d.z == 0);
	MathStructTest4Support_MST4_ASSERT(d.w == 0);
}

void MathStructTest::testVector4IntEqual()
{
	using namespace MathStructTest4Support;
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c(1, 2, 3, 5);
	MathStructTest4Support_MST4_ASSERT(a == b);
	MathStructTest4Support_MST4_ASSERT(!(a == c));
}

void MathStructTest::testVector4IntNotEqual()
{
	using namespace MathStructTest4Support;
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 5);
	Vector4Int c(1, 2, 3, 4);
	MathStructTest4Support_MST4_ASSERT(a != b);
	MathStructTest4Support_MST4_ASSERT(!(a != c));
}

void MathStructTest::testVector4IntClear()
{
	using namespace MathStructTest4Support;
	Vector4Int v(7, -8, 9, -10);
	v.clear();
	MathStructTest4Support_MST4_ASSERT(v.x == 0);
	MathStructTest4Support_MST4_ASSERT(v.y == 0);
	MathStructTest4Support_MST4_ASSERT(v.z == 0);
	MathStructTest4Support_MST4_ASSERT(v.w == 0);
}

void MathStructTest::testVector4IntBoundary()
{
	using namespace MathStructTest4Support;
	Vector4Int a(2147483647, -2147483647, 0, 1);
	MathStructTest4Support_MST4_ASSERT(a.x == 2147483647);
	MathStructTest4Support_MST4_ASSERT(a.y == -2147483647);
	MathStructTest4Support_MST4_ASSERT(a.z == 0);
	MathStructTest4Support_MST4_ASSERT(a.w == 1);
	Vector4Int b(-2147483647, 2147483647, 0, -1);
	Vector4Int c = a + b;
	MathStructTest4Support_MST4_ASSERT(c.x == 0);
	MathStructTest4Support_MST4_ASSERT(c.y == 0);
	MathStructTest4Support_MST4_ASSERT(c.z == 0);
	MathStructTest4Support_MST4_ASSERT(c.w == 0);
}

void MathStructTest::testVector4IntChain()
{
	using namespace MathStructTest4Support;
	Vector4Int a(1, 1, 1, 1);
	a += Vector4Int(2, 2, 2, 2);
	MathStructTest4Support_MST4_ASSERT(a.x == 3);
	MathStructTest4Support_MST4_ASSERT(a.y == 3);
	MathStructTest4Support_MST4_ASSERT(a.z == 3);
	MathStructTest4Support_MST4_ASSERT(a.w == 3);
	a -= Vector4Int(1, 1, 1, 1);
	MathStructTest4Support_MST4_ASSERT(a.x == 2);
	MathStructTest4Support_MST4_ASSERT(a.y == 2);
	MathStructTest4Support_MST4_ASSERT(a.z == 2);
	MathStructTest4Support_MST4_ASSERT(a.w == 2);
	Vector4Int b = a * 3;
	MathStructTest4Support_MST4_ASSERT(b.x == 6);
	MathStructTest4Support_MST4_ASSERT(b.y == 6);
	MathStructTest4Support_MST4_ASSERT(b.z == 6);
	MathStructTest4Support_MST4_ASSERT(b.w == 6);
	// less: x+y+z+w
	Vector4Int lo(1, 1, 1, 1);
	Vector4Int hi(2, 2, 2, 2);
	MathStructTest4Support_MST4_ASSERT(lo < hi);
	MathStructTest4Support_MST4_ASSERT(!(hi < lo));
}

void MathStructTest::testMixedIntVectorOps()
{
	using namespace MathStructTest4Support;
	// Store V2I in a Vector container
	Vector<Vector2Int> list;
	list.add(Vector2Int(1, 2));
	list.add(Vector2Int(3, 4));
	list.add(Vector2Int(5, 6));
	MathStructTest4Support_MST4_ASSERT(list.size() == 3);
	MathStructTest4Support_MST4_ASSERT(list[0].x == 1);
	MathStructTest4Support_MST4_ASSERT(list[1].y == 4);
	MathStructTest4Support_MST4_ASSERT(list[2].x == 5);

	// Store V3I in a vector
	Vector<Vector3Int> v3list;
	v3list.add(Vector3Int(1, 2, 3));
	v3list.add(Vector3Int(4, 5, 6));
	MathStructTest4Support_MST4_ASSERT(v3list.size() == 2);
	MathStructTest4Support_MST4_ASSERT(v3list[0].z == 3);
	MathStructTest4Support_MST4_ASSERT(v3list[1].x == 4);

	// HashMap with V2Int as key
	HashMap<Vector2Int, int> hmap;
	hmap.add(Vector2Int(10, 20), 100);
	hmap.add(Vector2Int(30, 40), 200);
	MathStructTest4Support_MST4_ASSERT(hmap.contains(Vector2Int(10, 20)));
	MathStructTest4Support_MST4_ASSERT(hmap.get(Vector2Int(10, 20), 0) == 100);
	MathStructTest4Support_MST4_ASSERT(hmap.get(Vector2Int(30, 40), 0) == 200);
	MathStructTest4Support_MST4_ASSERT(!hmap.contains(Vector2Int(0, 0)));
}

void MathStructTest::testIntVectorStress()
{
	using namespace MathStructTest4Support;
	// Stress test with many Vector2Int operations
	const int N = 100;
	Vector<Vector2Int> arr;
	for (int i = 0; i < N; ++i)
	{
	    arr.add(Vector2Int(i, N - i));
	}
	MathStructTest4Support_MST4_ASSERT(arr.size() == N);
	for (int i = 0; i < N; ++i)
	{
	    MathStructTest4Support_MST4_ASSERT(arr[i].x == i);
	    MathStructTest4Support_MST4_ASSERT(arr[i].y == N - i);
	}
	// Sum all
	Vector2Int sum;
	for (int i = 0; i < N; ++i)
	{
	    sum += arr[i];
	}
	// x sum = 0+1+...+99 = 4950; y sum = 100+99+...+1 = 5050
	MathStructTest4Support_MST4_ASSERT(sum.x == 4950);
	MathStructTest4Support_MST4_ASSERT(sum.y == 5050);

	// Vector3Int stress
	Vector<Vector3Int> v3arr;
	for (int i = 0; i < N; ++i)
	{
	    v3arr.add(Vector3Int(i, i * 2, i * 3));
	}
	for (int i = 0; i < N; ++i)
	{
	    MathStructTest4Support_MST4_ASSERT(v3arr[i].x == i);
	    MathStructTest4Support_MST4_ASSERT(v3arr[i].y == i * 2);
	    MathStructTest4Support_MST4_ASSERT(v3arr[i].z == i * 3);
	}

	// Vector4Int stress
	Vector<Vector4Int> v4arr;
	for (int i = 0; i < N; ++i)
	{
	    v4arr.add(Vector4Int(i, -i, i * 2, -i * 2));
	}
	for (int i = 0; i < N; ++i)
	{
	    MathStructTest4Support_MST4_ASSERT(v4arr[i].x == i);
	    MathStructTest4Support_MST4_ASSERT(v4arr[i].y == -i);
	    MathStructTest4Support_MST4_ASSERT(v4arr[i].z == i * 2);
	    MathStructTest4Support_MST4_ASSERT(v4arr[i].w == -i * 2);
	}
}

namespace MathStructTest5Support
{
#define MathStructTest5Support_MS5_ASSERT(expr) \
	if (!(expr)) \
	{ \
		LOG("MathStructTest5 FAILED: " #expr); \
	}
}

void MathStructTest::testVector2ShortConstruct5()
{
	using namespace MathStructTest5Support;
	Vector2Short v;
	MathStructTest5Support_MS5_ASSERT(v.x == 0);
	MathStructTest5Support_MS5_ASSERT(v.y == 0);

	Vector2Short v2((short)3, (short)-5);
	MathStructTest5Support_MS5_ASSERT(v2.x == 3);
	MathStructTest5Support_MS5_ASSERT(v2.y == -5);
}

void MathStructTest::testVector2ShortArithmetic5()
{
	using namespace MathStructTest5Support;
	Vector2Short a((short)10, (short)20);
	Vector2Short b((short)3, (short)7);

	// operator+
	Vector2Short add = a + b;
	MathStructTest5Support_MS5_ASSERT(add.x == 13);
	MathStructTest5Support_MS5_ASSERT(add.y == 27);

	// operator-
	Vector2Short sub = a - b;
	MathStructTest5Support_MS5_ASSERT(sub.x == 7);
	MathStructTest5Support_MS5_ASSERT(sub.y == 13);

	// operator* scalar
	Vector2Short mul = a * (short)2;
	MathStructTest5Support_MS5_ASSERT(mul.x == 20);
	MathStructTest5Support_MS5_ASSERT(mul.y == 40);

	// operator+=
	Vector2Short c((short)5, (short)5);
	c += b;
	MathStructTest5Support_MS5_ASSERT(c.x == 8);
	MathStructTest5Support_MS5_ASSERT(c.y == 12);

	// operator-=
	Vector2Short d((short)10, (short)10);
	d -= b;
	MathStructTest5Support_MS5_ASSERT(d.x == 7);
	MathStructTest5Support_MS5_ASSERT(d.y == 3);
}

void MathStructTest::testVector2ShortCompare()
{
	using namespace MathStructTest5Support;
	Vector2Short a((short)3, (short)4);
	Vector2Short b((short)3, (short)4);
	Vector2Short c((short)5, (short)6);

	MathStructTest5Support_MS5_ASSERT(a == b);
	MathStructTest5Support_MS5_ASSERT(a != c);
	MathStructTest5Support_MS5_ASSERT(a < c);  // 3+4=7 < 5+6=11
	MathStructTest5Support_MS5_ASSERT(!(c < a));
}

void MathStructTest::testVector2ShortClear()
{
	using namespace MathStructTest5Support;
	Vector2Short v((short)100, (short)-200);
	v.clear();
	MathStructTest5Support_MS5_ASSERT(v.x == 0);
	MathStructTest5Support_MS5_ASSERT(v.y == 0);
}

void MathStructTest::testVector2ShortNegate()
{
	using namespace MathStructTest5Support;
	Vector2Short v((short)5, (short)-3);
	Vector2Short neg = -v;
	MathStructTest5Support_MS5_ASSERT(neg.x == -5);
	MathStructTest5Support_MS5_ASSERT(neg.y == 3);
}

void MathStructTest::testVector2UShortConstruct5()
{
	using namespace MathStructTest5Support;
	Vector2UShort v;
	MathStructTest5Support_MS5_ASSERT(v.x == 0);
	MathStructTest5Support_MS5_ASSERT(v.y == 0);

	Vector2UShort v2((ushort)100, (ushort)200);
	MathStructTest5Support_MS5_ASSERT(v2.x == 100);
	MathStructTest5Support_MS5_ASSERT(v2.y == 200);
}

void MathStructTest::testVector2UShortArithmetic()
{
	using namespace MathStructTest5Support;
	Vector2UShort a((ushort)50, (ushort)100);
	Vector2UShort b((ushort)10, (ushort)20);

	Vector2UShort add = a + b;
	MathStructTest5Support_MS5_ASSERT(add.x == 60);
	MathStructTest5Support_MS5_ASSERT(add.y == 120);

	Vector2UShort sub = a - b;
	MathStructTest5Support_MS5_ASSERT(sub.x == 40);
	MathStructTest5Support_MS5_ASSERT(sub.y == 80);

	Vector2UShort mul = a * (short)2;
	MathStructTest5Support_MS5_ASSERT(mul.x == 100);
	MathStructTest5Support_MS5_ASSERT(mul.y == 200);

	Vector2UShort c((ushort)5, (ushort)5);
	c += b;
	MathStructTest5Support_MS5_ASSERT(c.x == 15);
	MathStructTest5Support_MS5_ASSERT(c.y == 25);

	Vector2UShort d((ushort)30, (ushort)40);
	d -= b;
	MathStructTest5Support_MS5_ASSERT(d.x == 20);
	MathStructTest5Support_MS5_ASSERT(d.y == 20);
}

void MathStructTest::testVector2UShortCompare()
{
	using namespace MathStructTest5Support;
	Vector2UShort a((ushort)3, (ushort)4);
	Vector2UShort b((ushort)3, (ushort)4);
	Vector2UShort c((ushort)10, (ushort)10);

	MathStructTest5Support_MS5_ASSERT(a == b);
	MathStructTest5Support_MS5_ASSERT(a != c);
	MathStructTest5Support_MS5_ASSERT(a < c);
	MathStructTest5Support_MS5_ASSERT(!(c < a));
}

void MathStructTest::testVector2UShortClear()
{
	using namespace MathStructTest5Support;
	Vector2UShort v((ushort)500, (ushort)1000);
	v.clear();
	MathStructTest5Support_MS5_ASSERT(v.x == 0);
	MathStructTest5Support_MS5_ASSERT(v.y == 0);
}

void MathStructTest::testVector2UIntConstruct5()
{
	using namespace MathStructTest5Support;
	Vector2UInt v;
	MathStructTest5Support_MS5_ASSERT(v.x == 0u);
	MathStructTest5Support_MS5_ASSERT(v.y == 0u);

	Vector2UInt v2(1000000u, 2000000u);
	MathStructTest5Support_MS5_ASSERT(v2.x == 1000000u);
	MathStructTest5Support_MS5_ASSERT(v2.y == 2000000u);
}

void MathStructTest::testVector2UIntArithmetic()
{
	using namespace MathStructTest5Support;
	Vector2UInt a(100u, 200u);
	Vector2UInt b(30u, 50u);

	Vector2UInt add = a + b;
	MathStructTest5Support_MS5_ASSERT(add.x == 130u);
	MathStructTest5Support_MS5_ASSERT(add.y == 250u);

	Vector2UInt sub = a - b;
	MathStructTest5Support_MS5_ASSERT(sub.x == 70u);
	MathStructTest5Support_MS5_ASSERT(sub.y == 150u);

	Vector2UInt mul = a * (short)3;
	MathStructTest5Support_MS5_ASSERT(mul.x == 300u);
	MathStructTest5Support_MS5_ASSERT(mul.y == 600u);

	Vector2UInt c(10u, 10u);
	c += b;
	MathStructTest5Support_MS5_ASSERT(c.x == 40u);
	MathStructTest5Support_MS5_ASSERT(c.y == 60u);

	Vector2UInt d(100u, 100u);
	d -= b;
	MathStructTest5Support_MS5_ASSERT(d.x == 70u);
	MathStructTest5Support_MS5_ASSERT(d.y == 50u);
}

void MathStructTest::testVector2UIntCompare()
{
	using namespace MathStructTest5Support;
	Vector2UInt a(5u, 5u);
	Vector2UInt b(5u, 5u);
	Vector2UInt c(20u, 20u);

	MathStructTest5Support_MS5_ASSERT(a == b);
	MathStructTest5Support_MS5_ASSERT(a != c);
	MathStructTest5Support_MS5_ASSERT(a < c);
}

void MathStructTest::testVector2UIntClear5()
{
	using namespace MathStructTest5Support;
	Vector2UInt v(9999u, 8888u);
	v.clear();
	MathStructTest5Support_MS5_ASSERT(v.x == 0u);
	MathStructTest5Support_MS5_ASSERT(v.y == 0u);
}

void MathStructTest::testQuaternionDefault5()
{
	using namespace MathStructTest5Support;
	Quaternion q;
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 1.0f) < 1e-4f);
}

void MathStructTest::testQuaternionConstruct5()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x - 1.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y - 2.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z - 3.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 4.0f) < 1e-4f);

	// 拷贝构造
	Quaternion q2(q);
	MathStructTest5Support_MS5_ASSERT(fabsf(q2.x - 1.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q2.w - 4.0f) < 1e-4f);
}

void MathStructTest::testQuaternionConjugate5()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion conj = q.conjugate();
	MathStructTest5Support_MS5_ASSERT(fabsf(conj.x - (-1.0f)) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(conj.y - (-2.0f)) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(conj.z - (-3.0f)) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(conj.w - 4.0f) < 1e-4f);
}

void MathStructTest::testQuaternionDot5()
{
	using namespace MathStructTest5Support;
	Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
	// dot = 1+4+9+16 = 30
	float d = Quaternion::dot(q1, q2);
	MathStructTest5Support_MS5_ASSERT(fabsf(d - 30.0f) < 1e-4f);

	// dot(q, identity(0,0,0,1)) = w
	Quaternion id;
	float d2 = Quaternion::dot(q1, id);
	MathStructTest5Support_MS5_ASSERT(fabsf(d2 - (1.0f * 0.0f + 2.0f * 0.0f + 3.0f * 0.0f + 4.0f * 1.0f)) < 1e-4f);
}

void MathStructTest::testQuaternionLength5()
{
	using namespace MathStructTest5Support;
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	// 单位四元数长度 = 1
	MathStructTest5Support_MS5_ASSERT(fabsf(q.length() - 1.0f) < 1e-4f);

	Quaternion q2(1.0f, 0.0f, 0.0f, 0.0f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q2.length() - 1.0f) < 1e-4f);
}

void MathStructTest::testQuaternionNormalize5()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion n = q.normalize();
	float len = n.length();
	MathStructTest5Support_MS5_ASSERT(fabsf(len - 1.0f) < 1e-4f);
}

void MathStructTest::testQuaternionMultiplyScalar()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion r = q * 2.0f;
	MathStructTest5Support_MS5_ASSERT(fabsf(r.x - 2.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.y - 4.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.z - 6.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.w - 8.0f) < 1e-4f);
}

void MathStructTest::testQuaternionDivideScalar()
{
	using namespace MathStructTest5Support;
	Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
	Quaternion r = q / 2.0f;
	MathStructTest5Support_MS5_ASSERT(fabsf(r.x - 1.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.y - 2.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.z - 3.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.w - 4.0f) < 1e-4f);
}

void MathStructTest::testQuaternionAdd5()
{
	using namespace MathStructTest5Support;
	Quaternion q1(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion q2(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion r = q1 + q2;
	MathStructTest5Support_MS5_ASSERT(fabsf(r.x - 1.5f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.y - 2.5f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.z - 3.5f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(r.w - 4.5f) < 1e-4f);
}

void MathStructTest::testQuaternionNegate()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, -2.0f, 3.0f, -4.0f);
	Quaternion neg = -q;
	MathStructTest5Support_MS5_ASSERT(fabsf(neg.x - (-1.0f)) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(neg.y - 2.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(neg.z - (-3.0f)) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(neg.w - 4.0f) < 1e-4f);
}

void MathStructTest::testQuaternionMultiplyAssign()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q *= 3.0f;
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x - 3.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y - 6.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z - 9.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 12.0f) < 1e-4f);
}

void MathStructTest::testQuaternionDivideAssign()
{
	using namespace MathStructTest5Support;
	Quaternion q(4.0f, 8.0f, 12.0f, 16.0f);
	q /= 4.0f;
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x - 1.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y - 2.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z - 3.0f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 4.0f) < 1e-4f);
}

void MathStructTest::testQuaternionAddAssign5()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 1.0f, 1.0f, 1.0f);
	Quaternion add(0.5f, 0.5f, 0.5f, 0.5f);
	q += add;
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x - 1.5f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y - 1.5f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z - 1.5f) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 1.5f) < 1e-4f);
}

void MathStructTest::testQuaternionEulerAngles5()
{
	using namespace MathStructTest5Support;
	// 单位四元数：欧拉角应接近(0,0,0)
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	Vector3 euler = q.eulerAngles();
	MathStructTest5Support_MS5_ASSERT(fabsf(euler.x) < 1e-3f);
	MathStructTest5Support_MS5_ASSERT(fabsf(euler.y) < 1e-3f);
	MathStructTest5Support_MS5_ASSERT(fabsf(euler.z) < 1e-3f);
}

void MathStructTest::testQuaternionLerp5()
{
	using namespace MathStructTest5Support;
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	// lerp(q1, q2, 0.5) = q1 * 0.5 + q2 * 0.5
	Quaternion lr = Quaternion::lerp(q1, q2, 0.5f);
	MathStructTest5Support_MS5_ASSERT(fabsf(lr.w - 1.0f) < 1e-4f);

	// lerp at 0 = q1, at 1 = q2
	Quaternion l0 = Quaternion::lerp(q1, q2, 0.0f);
	MathStructTest5Support_MS5_ASSERT(fabsf(l0.w - 1.0f) < 1e-4f);
	Quaternion l1 = Quaternion::lerp(q1, q2, 1.0f);
	MathStructTest5Support_MS5_ASSERT(fabsf(l1.w - 1.0f) < 1e-4f);
}

void MathStructTest::testQuaternionClear5()
{
	using namespace MathStructTest5Support;
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q.clear();
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z) < 1e-4f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 1.0f) < 1e-4f);
}

void MathStructTest::testQuaternionFromAxisAngle5()
{
	using namespace MathStructTest5Support;
	// 绕 z 轴旋转 90 度
	Vector3 axis(0.0f, 0.0f, 1.0f);
	float angle = 3.14159265f * 0.5f;
	Quaternion q(axis, angle);
	// q = (sin(45)*z, cos(45)) ≈ (0, 0, 0.7071, 0.7071)
	MathStructTest5Support_MS5_ASSERT(fabsf(q.x) < 1e-3f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.y) < 1e-3f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.z - 0.7071f) < 1e-3f);
	MathStructTest5Support_MS5_ASSERT(fabsf(q.w - 0.7071f) < 1e-3f);
}

void MathStructTest::testQuaternionFromEulerAngles()
{
	using namespace MathStructTest5Support;
	// 绕 z 轴 90 度的欧拉角 (0, 0, PI/2)
	Vector3 euler(0.0f, 0.0f, 3.14159265f * 0.5f);
	Quaternion q = Quaternion::eulerAnglesToQuaterion(euler);
	float len = q.length();
	// 转换后应为单位四元数
	MathStructTest5Support_MS5_ASSERT(fabsf(len - 1.0f) < 1e-3f);
}

namespace MathStructExt2TestSupport
{
#define MathStructExt2TestSupport_MSE2_ASSERT(expr) \
    if (!(expr)) { ERROR("MathStructExt2Test FAILED: " #expr); }

#define MathStructExt2TestSupport_MSE2_NEAR(a, b) (fabsf((float)(a) - (float)(b)) < 1e-4f)
}

void MathStructTest::testLine2Construction()
{
	using namespace MathStructExt2TestSupport;
	Line2 line({ 0.0f, 0.0f }, { 1.0f, 1.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.y, 1.0f));
	Line2 line2({ -3.0f, 5.0f }, { 7.0f, -2.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line2.mStart.x, -3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line2.mStart.y, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line2.mEnd.x, 7.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line2.mEnd.y, -2.0f));
}

void MathStructTest::testLine2Members()
{
	using namespace MathStructExt2TestSupport;
	Line2 line({ 1.5f, 2.5f }, { 3.5f, 4.5f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.x, 1.5f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.y, 2.5f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.x, 3.5f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.y, 4.5f));
	Vector2 dir = line.mEnd - line.mStart;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(dir.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(dir.y, 2.0f));
}

void MathStructTest::testLine2Multiple()
{
	using namespace MathStructExt2TestSupport;
	Line2 horizontal({ 0.0f, 3.0f }, { 10.0f, 3.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(horizontal.mStart.y, horizontal.mEnd.y));
	Line2 vertical({ 5.0f, 0.0f }, { 5.0f, 10.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(vertical.mStart.x, vertical.mEnd.x));
	Line2 diagonal({ 0.0f, 0.0f }, { 5.0f, 5.0f });
	Vector2 diff = diagonal.mEnd - diagonal.mStart;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diff.x, diff.y));
}

void MathStructTest::testLine2ZeroLength()
{
	using namespace MathStructExt2TestSupport;
	Line2 zero({ 3.0f, 4.0f }, { 3.0f, 4.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(zero.mStart.x, zero.mEnd.x));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(zero.mStart.y, zero.mEnd.y));
	Vector2 diff = zero.mEnd - zero.mStart;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diff.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diff.y, 0.0f));
}

void MathStructTest::testTriangle2Construction()
{
	using namespace MathStructExt2TestSupport;
	Triangle2 tri({ 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.5f, 1.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint1.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint1.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint2.x, 0.5f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint2.y, 1.0f));
	Triangle2 tri2({ -1.0f, -1.0f }, { 1.0f, -1.0f }, { 0.0f, 1.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri2.mPoint0.x, -1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri2.mPoint2.y, 1.0f));
}

void MathStructTest::testTriangle2Members()
{
	using namespace MathStructExt2TestSupport;
	Triangle2 tri({ 0.0f, 0.0f }, { 4.0f, 0.0f }, { 2.0f, 3.0f });
	Vector2 centroid;
	centroid.x = (tri.mPoint0.x + tri.mPoint1.x + tri.mPoint2.x) / 3.0f;
	centroid.y = (tri.mPoint0.y + tri.mPoint1.y + tri.mPoint2.y) / 3.0f;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(centroid.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(centroid.y, 1.0f));
	Vector2 side1 = tri.mPoint1 - tri.mPoint0;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(side1.x, 4.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(side1.y, 0.0f));
}

void MathStructTest::testTriangle2DefaultInit()
{
	using namespace MathStructExt2TestSupport;
	Triangle2 tri;
	tri.mPoint0 = { 1.0f, 2.0f };
	tri.mPoint1 = { 3.0f, 4.0f };
	tri.mPoint2 = { 5.0f, 6.0f };
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint1.x, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint2.x, 5.0f));
}

void MathStructTest::testTriangle3Construction()
{
	using namespace MathStructExt2TestSupport;
	Triangle3 tri({ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 1.0f, 0.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.z, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint1.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint2.y, 1.0f));
	Triangle3 tri2({ -1.0f, 0.0f, 2.0f }, { 3.0f, -1.0f, 2.0f }, { 1.0f, 2.0f, 0.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri2.mPoint0.x, -1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri2.mPoint0.z, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri2.mPoint2.y, 2.0f));
}

void MathStructTest::testTriangle3Members()
{
	using namespace MathStructExt2TestSupport;
	Triangle3 tri({ 0.0f, 0.0f, 0.0f }, { 4.0f, 0.0f, 0.0f }, { 2.0f, 3.0f, 0.0f });
	Vector3 centroid;
	centroid.x = (tri.mPoint0.x + tri.mPoint1.x + tri.mPoint2.x) / 3.0f;
	centroid.y = (tri.mPoint0.y + tri.mPoint1.y + tri.mPoint2.y) / 3.0f;
	centroid.z = (tri.mPoint0.z + tri.mPoint1.z + tri.mPoint2.z) / 3.0f;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(centroid.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(centroid.y, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(centroid.z, 0.0f));
	Vector3 edge1 = tri.mPoint1 - tri.mPoint0;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(edge1.x, 4.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(edge1.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(edge1.z, 0.0f));
}

void MathStructTest::testTriangle3DefaultInit()
{
	using namespace MathStructExt2TestSupport;
	Triangle3 tri;
	tri.mPoint0 = { 1.0f, 2.0f, 3.0f };
	tri.mPoint1 = { 4.0f, 5.0f, 6.0f };
	tri.mPoint2 = { 7.0f, 8.0f, 9.0f };
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint0.z, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint1.z, 6.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(tri.mPoint2.z, 9.0f));
}

void MathStructTest::testRectConstruction()
{
	using namespace MathStructExt2TestSupport;
	Rect r(0.0f, 0.0f, 10.0f, 5.0f);
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.width, 10.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.height, 5.0f));
	Rect r2(-3.0f, -2.0f, 6.0f, 4.0f);
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.x, -3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.y, -2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.width, 6.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.height, 4.0f));
}

void MathStructTest::testRectVector2Constructor()
{
	using namespace MathStructExt2TestSupport;
	Rect r({ 2.0f, 3.0f }, { 8.0f, 6.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.y, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.width, 8.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.height, 6.0f));
	Rect r2({ 0.0f, 0.0f }, { 1.0f, 1.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.width, 1.0f));
}

void MathStructTest::testRectGetSize()
{
	using namespace MathStructExt2TestSupport;
	Rect r(1.0f, 2.0f, 5.0f, 3.0f);
	Vector2 sz = r.getSize();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.x, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.y, 3.0f));
	Rect r2(0.0f, 0.0f, 10.0f, 10.0f);
	Vector2 sz2 = r2.getSize();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz2.x, 10.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz2.y, 10.0f));
	Rect r3(5.0f, 5.0f, 0.0f, 0.0f);
	Vector2 sz3 = r3.getSize();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz3.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz3.y, 0.0f));
}

void MathStructTest::testRectGetMin()
{
	using namespace MathStructExt2TestSupport;
	Rect r(2.0f, 3.0f, 5.0f, 4.0f);
	Vector2 mn = r.getMin();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mn.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mn.y, 3.0f));
	Rect r2(-10.0f, -5.0f, 20.0f, 10.0f);
	Vector2 mn2 = r2.getMin();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mn2.x, -10.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mn2.y, -5.0f));
}

void MathStructTest::testRectGetMax()
{
	using namespace MathStructExt2TestSupport;
	Rect r(2.0f, 3.0f, 5.0f, 4.0f);
	Vector2 mx = r.getMax();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mx.x, 7.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mx.y, 7.0f));
	Rect r2(-10.0f, -5.0f, 20.0f, 10.0f);
	Vector2 mx2 = r2.getMax();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mx2.x, 10.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mx2.y, 5.0f));
	Rect r3(0.0f, 0.0f, 1.0f, 1.0f);
	Vector2 mx3 = r3.getMax();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mx3.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(mx3.y, 1.0f));
}

void MathStructTest::testRectGetCenter()
{
	using namespace MathStructExt2TestSupport;
	Rect r(0.0f, 0.0f, 4.0f, 6.0f);
	Vector2 c = r.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.y, 3.0f));
	Rect r2(2.0f, 4.0f, 6.0f, 8.0f);
	Vector2 c2 = r2.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c2.x, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c2.y, 8.0f));
	Rect r3(-4.0f, -6.0f, 8.0f, 12.0f);
	Vector2 c3 = r3.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c3.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c3.y, 0.0f));
}

void MathStructTest::testRectGetDiagonal()
{
	using namespace MathStructExt2TestSupport;
	Rect r(0.0f, 0.0f, 3.0f, 4.0f);
	float diag = r.getDiagonal();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag, 5.0f));
	Rect r2(0.0f, 0.0f, 1.0f, 0.0f);
	float diag2 = r2.getDiagonal();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag2, 1.0f));
	Rect r3(0.0f, 0.0f, 0.0f, 0.0f);
	float diag3 = r3.getDiagonal();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag3, 0.0f));
	Rect r4(0.0f, 0.0f, 5.0f, 5.0f);
	float diag4 = r4.getDiagonal();
	MathStructExt2TestSupport_MSE2_ASSERT(diag4 > 7.0f && diag4 < 7.1f);
}

void MathStructTest::testRectEdgeCases()
{
	using namespace MathStructExt2TestSupport;
	Rect rUnit(0.0f, 0.0f, 1.0f, 1.0f);
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(rUnit.getDiagonal(), sqrtf(2.0f)));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(rUnit.getCenter().x, 0.5f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(rUnit.getCenter().y, 0.5f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(rUnit.getMax().x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(rUnit.getMax().y, 1.0f));
	Rect rNeg(-5.0f, -3.0f, 10.0f, 6.0f);
	Vector2 center = rNeg.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(center.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(center.y, 0.0f));
}

void MathStructTest::testRect3Construction()
{
	using namespace MathStructExt2TestSupport;
	Rect3 r({ 0.0f, 0.0f, 0.0f }, { 10.0f, 5.0f, 8.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.mMin.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.mMin.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.mMin.z, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.mMax.x, 10.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.mMax.y, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r.mMax.z, 8.0f));
	Rect3 r2({ -5.0f, -3.0f, -2.0f }, { 5.0f, 3.0f, 2.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.mMin.x, -5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(r2.mMax.x, 5.0f));
}

void MathStructTest::testRect3GetSize()
{
	using namespace MathStructExt2TestSupport;
	Rect3 r({ 0.0f, 0.0f, 0.0f }, { 4.0f, 3.0f, 2.0f });
	Vector3 sz = r.getSize();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.x, 4.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.y, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.z, 2.0f));
	Rect3 r2({ 1.0f, 1.0f, 1.0f }, { 4.0f, 6.0f, 11.0f });
	Vector3 sz2 = r2.getSize();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz2.x, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz2.y, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz2.z, 10.0f));
}

void MathStructTest::testRect3GetCenter()
{
	using namespace MathStructExt2TestSupport;
	Rect3 r({ 0.0f, 0.0f, 0.0f }, { 4.0f, 6.0f, 8.0f });
	Vector3 c = r.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.y, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.z, 4.0f));
	Rect3 r2({ -2.0f, -3.0f, -4.0f }, { 2.0f, 3.0f, 4.0f });
	Vector3 c2 = r2.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c2.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c2.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c2.z, 0.0f));
}

void MathStructTest::testRect3GetDiagonalIgnoreY()
{
	using namespace MathStructExt2TestSupport;
	Rect3 r({ 0.0f, 0.0f, 0.0f }, { 3.0f, 100.0f, 4.0f });
	float diag = r.getDiagonalIgnoreY();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag, 5.0f));
	Rect3 r2({ 0.0f, 0.0f, 0.0f }, { 0.0f, 50.0f, 5.0f });
	float diag2 = r2.getDiagonalIgnoreY();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag2, 5.0f));
	Rect3 r3({ 0.0f, 0.0f, 0.0f }, { 3.0f, 0.0f, 0.0f });
	float diag3 = r3.getDiagonalIgnoreY();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag3, 3.0f));
}

void MathStructTest::testRect3EdgeCases()
{
	using namespace MathStructExt2TestSupport;
	Rect3 rZero({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	Vector3 sz = rZero.getSize();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(sz.z, 0.0f));
	Vector3 c = rZero.getCenter();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(c.z, 0.0f));
	float diag = rZero.getDiagonalIgnoreY();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diag, 0.0f));
}

void MathStructTest::testLine3Construction()
{
	using namespace MathStructExt2TestSupport;
	Line3 line({ 0.0f, 0.0f, 0.0f }, { 1.0f, 2.0f, 3.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mStart.z, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.y, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line.mEnd.z, 3.0f));
	Line3 line2({ -1.0f, 2.0f, -3.0f }, { 4.0f, -5.0f, 6.0f });
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line2.mStart.x, -1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(line2.mEnd.z, 6.0f));
}

void MathStructTest::testLine3Members()
{
	using namespace MathStructExt2TestSupport;
	Line3 line({ 1.0f, 2.0f, 3.0f }, { 4.0f, 6.0f, 9.0f });
	Vector3 dir = line.mEnd - line.mStart;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(dir.x, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(dir.y, 4.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(dir.z, 6.0f));
}

void MathStructTest::testLine3ZeroLength()
{
	using namespace MathStructExt2TestSupport;
	Line3 zero({ 5.0f, 6.0f, 7.0f }, { 5.0f, 6.0f, 7.0f });
	Vector3 diff = zero.mEnd - zero.mStart;
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diff.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diff.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(diff.z, 0.0f));
}

void MathStructTest::testLine3ToLine2IgnoreY()
{
	using namespace MathStructExt2TestSupport;
	Line3 line({ 1.0f, 100.0f, 2.0f }, { 3.0f, 200.0f, 4.0f });
	Line2D l2 = line.toLine2IgnoreY();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mStart.x, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mStart.y, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mEnd.x, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mEnd.y, 4.0f));
	Line3 line2({ 0.0f, 50.0f, 0.0f }, { 5.0f, 100.0f, 5.0f });
	Line2D l2b = line2.toLine2IgnoreY();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mStart.x, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mStart.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mEnd.x, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mEnd.y, 5.0f));
}

void MathStructTest::testLine3ToLine2IgnoreX()
{
	using namespace MathStructExt2TestSupport;
	Line3 line({ 100.0f, 1.0f, 2.0f }, { 200.0f, 3.0f, 4.0f });
	Line2D l2 = line.toLine2IgnoreX();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mStart.x, 2.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mStart.y, 1.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mEnd.x, 4.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2.mEnd.y, 3.0f));
	Line3 line2({ 99.0f, 0.0f, 5.0f }, { 88.0f, 7.0f, 3.0f });
	Line2D l2b = line2.toLine2IgnoreX();
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mStart.x, 5.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mStart.y, 0.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mEnd.x, 3.0f));
	MathStructExt2TestSupport_MSE2_ASSERT(MathStructExt2TestSupport_MSE2_NEAR(l2b.mEnd.y, 7.0f));
}


void MathStructTest::test()
{
    // Vector2
    testVector2_DefaultCtor();
    testVector2_ParamCtor();
    testVector2_Clear();
    testVector2_Add();
    testVector2_Sub();
    testVector2_MulScalar();
    testVector2_DivScalar();
    testVector2_AddAssign();
    testVector2_SubAssign();
    testVector2_MulAssign();
    testVector2_DivAssign();
    testVector2_Negate();
    testVector2_StaticZero();
    testVector2_StaticOne();
    testVector2_StaticUp();
    testVector2_StaticDown();
    testVector2_StaticLeft();
    testVector2_StaticRight();
    
    // Vector3
    testVector3_DefaultCtor();
    testVector3_ParamCtor();
    testVector3_Clear();
    testVector3_Add();
    testVector3_Sub();
    testVector3_MulScalar();
    testVector3_DivScalar();
    testVector3_AddAssign();
    testVector3_SubAssign();
    testVector3_MulAssign();
    testVector3_DivAssign();
    testVector3_Negate();
    testVector3_StaticZero();
    testVector3_StaticOne();
    testVector3_StaticUp();
    testVector3_StaticDown();
    testVector3_StaticLeft();
    testVector3_StaticRight();
    
    // Color
    testColor_DefaultCtor();
    testColor_ParamCtor();
    testColor_Clear();
    testColor_Mul();
    testColor_MulAssign();
    testColor_Add();
    testColor_Sub();
    testColor_AddAssign();
    testColor_SubAssign();
    testColor_StaticWhite();
    testColor_StaticBlack();
    testColor_StaticRed();
    testColor_AddOverflow();
    testColor_AddAssignChain();
    testColor_SubAssignToZero();
    
    // Quaternion
    testQuaternion_DefaultCtor();
    testQuaternion_ParamCtor();
    testQuaternion_Clear();
    testQuaternion_Conjugate_MST();
    testQuaternion_Normalize_MST();
    testQuaternion_NormalizeIdentity();
    testQuaternion_Length();
    testQuaternion_Dot();
    testQuaternion_Add();
    testQuaternion_AddAssign();
    testQuaternion_MulScalar();
    testQuaternion_MulAssignScalar();
    testQuaternion_DivScalar();
    testQuaternion_Negate_MST();
    testQuaternion_Lerp_MST();
    testQuaternion_CopyCtor();
    testQuaternion_AxisAngleCtor();
    testQuaternion_EulerAngles_MST();
    testQuaternion_Mul_Identity();
    testQuaternion_Mul_Vector3();
    testQuaternion_NormalizeThenLength();
    testQuaternion_InverseOfNormalized();
    
    // Extended
    testVector2_ChainAssign();
    testVector3_ScaleAndNeg();
    testQuaternion_DivScalarExtended();
    testColor_EqualityExtended();
    testVector2_EqualityExtended();
    testVector3_EqualityExtended();
    testLine2B();
    testLine2DSloped();
    testLine2DVertical();
    testLine2DHorizontal();
    testLine2DNegativeSlope();
    testLine3Conversions();
    testRectB();
    testRect3B();
    testTriangle2B();
    testTriangle3B();
    testGeometryScenario();
    testGeometryPrecision();
    // Vector4
    testVector4_DefaultCtor();
    testVector4_ParamCtor();
    testVector4_Clear();
    testVector4_Add();
    testVector4_Sub();
    testVector4_AddAssign();
    testVector4_SubAssign();
    testVector4_Negate();
    testVector4_AddSubChain();
    testVector4_NegativeValues();
    testVector4_ZeroAdd();
    testVector4_SelfAddSub();
    testVector4_LargeValues();
    testVector4_AddAssignChain();
    testVector4_SubAssignChain();
    testVector4_CopyCtor();
    testVector4_AssignOp();
    
    // Vector4Int
    testVector4Int_DefaultCtor();
    testVector4Int_ParamCtor();
    testVector4Int_Clear();
    testVector4Int_Add();
    testVector4Int_Sub();
    testVector4Int_AddAssign();
    testVector4Int_SubAssign();
    testVector4Int_Negate();
    testVector4Int_MulScalar();
    testVector4Int_MulByZero();
    testVector4Int_MulByOne();
    testVector4Int_MulByNeg();
    testVector4Int_Equality();
    testVector4Int_LessThan();
    testVector4Int_LessThan_Equal();
    testVector4Int_NegativeValues();
    testVector4Int_Chain();
    testVector4Int_UseInHashMap();
    testVector4Int_SumOfComponents();
    
    // Vector2Int
    testVector2Int_DefaultCtor();
    testVector2Int_ParamCtor();
    testVector2Int_Clear();
    testVector2Int_Add();
    testVector2Int_Sub();
    testVector2Int_AddAssign();
    testVector2Int_SubAssign();
    testVector2Int_Negate();
    testVector2Int_MulScalar();
    testVector2Int_MulByZero();
    testVector2Int_MulByNeg();
    testVector2Int_Equality();
    testVector2Int_LessThan();
    testVector2Int_LessThan_Equal();
    testVector2Int_StaticZero();
    testVector2Int_StdHash();
    testVector2Int_StdHash_Distinct();
    testVector2Int_UseAsMapKey();
    testVector2Int_Chain();
    testVector2Int_NegativeCoords();
    // Vector2Int
    testVector2IntDefault();
    testVector2IntConstruct4();
    testVector2IntAdd();
    testVector2IntSub();
    testVector2IntMul();
    testVector2IntNeg();
    testVector2IntAddAssign();
    testVector2IntSubAssign();
    testVector2IntEqual();
    testVector2IntNotEqual();
    testVector2IntLess4();
    testVector2IntClear4();
    testVector2IntZero();
    testVector2IntHash();
    testVector2IntChainOps();
    testVector2IntBoundary();
    // Vector2UInt
    testVector2UIntDefault();
    testVector2UIntConstruct();
    testVector2UIntAdd();
    testVector2UIntSub();
    testVector2UIntEqual();
    testVector2UIntNotEqual();
    testVector2UIntClear();
    testVector2UIntLarge();
    // Vector2Short
    testVector2ShortDefault();
    testVector2ShortConstruct();
    testVector2ShortAddSub();
    testVector2ShortEqual();
    testVector2ShortBoundary();
    // Vector2UShort
    testVector2UShortDefault();
    testVector2UShortConstruct();
    testVector2UShortAddSub();
    testVector2UShortEqual();
    // Vector3Int
    testVector3IntDefault();
    testVector3IntConstruct();
    testVector3IntAdd();
    testVector3IntSub();
    testVector3IntMul();
    testVector3IntEqual();
    testVector3IntNotEqual();
    testVector3IntClear();
    testVector3IntZero();
    testVector3IntBoundary();
    testVector3IntChain();
    // Vector4Int
    testVector4IntDefault();
    testVector4IntConstruct();
    testVector4IntAdd();
    testVector4IntSub();
    testVector4IntMul();
    testVector4IntEqual();
    testVector4IntNotEqual();
    testVector4IntClear();
    testVector4IntBoundary();
    testVector4IntChain();
    // Mixed
    testMixedIntVectorOps();
    testIntVectorStress();
    // Vector2Short
    testVector2ShortConstruct5();
    testVector2ShortArithmetic5();
    testVector2ShortCompare();
    testVector2ShortClear();
    testVector2ShortNegate();
    // Vector2UShort
    testVector2UShortConstruct5();
    testVector2UShortArithmetic();
    testVector2UShortCompare();
    testVector2UShortClear();
    // Vector2UInt
    testVector2UIntConstruct5();
    testVector2UIntArithmetic();
    testVector2UIntCompare();
    testVector2UIntClear5();
    // Quaternion
    testQuaternionDefault5();
    testQuaternionConstruct5();
    testQuaternionConjugate5();
    testQuaternionDot5();
    testQuaternionLength5();
    testQuaternionNormalize5();
    testQuaternionMultiplyScalar();
    testQuaternionDivideScalar();
    testQuaternionAdd5();
    testQuaternionNegate();
    testQuaternionMultiplyAssign();
    testQuaternionDivideAssign();
    testQuaternionAddAssign5();
    testQuaternionEulerAngles5();
    testQuaternionLerp5();
    testQuaternionClear5();
    testQuaternionFromAxisAngle5();
    testQuaternionFromEulerAngles();
    testLine2Construction();
    testLine2Members();
    testLine2Multiple();
    testLine2ZeroLength();
    testTriangle2Construction();
    testTriangle2Members();
    testTriangle2DefaultInit();
    testTriangle3Construction();
    testTriangle3Members();
    testTriangle3DefaultInit();
    testRectConstruction();
    testRectVector2Constructor();
    testRectGetSize();
    testRectGetMin();
    testRectGetMax();
    testRectGetCenter();
    testRectGetDiagonal();
    testRectEdgeCases();
    testRect3Construction();
    testRect3GetSize();
    testRect3GetCenter();
    testRect3GetDiagonalIgnoreY();
    testRect3EdgeCases();
    testLine3Construction();
    testLine3Members();
    testLine3ZeroLength();
    testLine3ToLine2IgnoreY();
    testLine3ToLine2IgnoreX();
	testGeneralMathStructs();
	testColorQuaternion();
	testVector2Family();
	testVector3Family();
	testVector4Family();
}

// ============================================================
// Consolidated MathTest
// ============================================================
#define MT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg)); } } while(0)
#define MT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg)); } } while(0)
#define MT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a) - (b)) > 1e-4f) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg) + \
		" | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)

//------------------------------------------------------------------------------
// Vector2
//------------------------------------------------------------------------------
void MathStructTest::testVector2_Operators()
{
	const Vector2 a(1.0f, 2.0f);
	const Vector2 b(3.0f, 4.0f);

	// +
	const Vector2 add = a + b;
	MT_ASSERT_FLOAT(add.x, 4.0f, "V2 add x");
	MT_ASSERT_FLOAT(add.y, 6.0f, "V2 add y");

	// -
	const Vector2 sub = b - a;
	MT_ASSERT_FLOAT(sub.x, 2.0f, "V2 sub x");
	MT_ASSERT_FLOAT(sub.y, 2.0f, "V2 sub y");

	// * scalar
	const Vector2 scaled = a * 3.0f;
	MT_ASSERT_FLOAT(scaled.x, 3.0f, "V2 scale x");
	MT_ASSERT_FLOAT(scaled.y, 6.0f, "V2 scale y");

	// / scalar
	const Vector2 divided = b / 2.0f;
	MT_ASSERT_FLOAT(divided.x, 1.5f, "V2 div x");
	MT_ASSERT_FLOAT(divided.y, 2.0f, "V2 div y");

	// unary -
	const Vector2 neg = -a;
	MT_ASSERT_FLOAT(neg.x, -1.0f, "V2 neg x");
	MT_ASSERT_FLOAT(neg.y, -2.0f, "V2 neg y");

	// +=
	Vector2 acc(1.0f, 1.0f);
	acc += Vector2(2.0f, 3.0f);
	MT_ASSERT_FLOAT(acc.x, 3.0f, "V2 += x");
	MT_ASSERT_FLOAT(acc.y, 4.0f, "V2 += y");

	// -=
	acc -= Vector2(1.0f, 1.0f);
	MT_ASSERT_FLOAT(acc.x, 2.0f, "V2 -= x");
	MT_ASSERT_FLOAT(acc.y, 3.0f, "V2 -= y");

	// clear
	Vector2 v(5.0f, 6.0f);
	v.clear();
	MT_ASSERT_FLOAT(v.x, 0.0f, "V2 clear x");
	MT_ASSERT_FLOAT(v.y, 0.0f, "V2 clear y");

	// ZERO / ONE constants
	MT_ASSERT_FLOAT(Vector2::ZERO.x, 0.0f, "V2::ZERO x");
	MT_ASSERT_FLOAT(Vector2::ZERO.y, 0.0f, "V2::ZERO y");
	MT_ASSERT_FLOAT(Vector2::ONE.x,  1.0f, "V2::ONE x");
	MT_ASSERT_FLOAT(Vector2::ONE.y,  1.0f, "V2::ONE y");
}

//------------------------------------------------------------------------------
// Vector3
//------------------------------------------------------------------------------
void MathStructTest::testVector3_Operators()
{
	const Vector3 a(1.0f, 2.0f, 3.0f);
	const Vector3 b(4.0f, 5.0f, 6.0f);

	// +
	const Vector3 add = a + b;
	MT_ASSERT_FLOAT(add.x, 5.0f, "V3 add x");
	MT_ASSERT_FLOAT(add.y, 7.0f, "V3 add y");
	MT_ASSERT_FLOAT(add.z, 9.0f, "V3 add z");

	// -
	const Vector3 sub = b - a;
	MT_ASSERT_FLOAT(sub.x, 3.0f, "V3 sub x");
	MT_ASSERT_FLOAT(sub.y, 3.0f, "V3 sub y");
	MT_ASSERT_FLOAT(sub.z, 3.0f, "V3 sub z");

	// * scalar
	const Vector3 scaled = a * 2.0f;
	MT_ASSERT_FLOAT(scaled.x, 2.0f, "V3 scale x");
	MT_ASSERT_FLOAT(scaled.y, 4.0f, "V3 scale y");
	MT_ASSERT_FLOAT(scaled.z, 6.0f, "V3 scale z");

	// / scalar
	const Vector3 divided = b / 2.0f;
	MT_ASSERT_FLOAT(divided.x, 2.0f, "V3 div x");
	MT_ASSERT_FLOAT(divided.y, 2.5f, "V3 div y");
	MT_ASSERT_FLOAT(divided.z, 3.0f, "V3 div z");

	// unary -
	const Vector3 neg = -a;
	MT_ASSERT_FLOAT(neg.x, -1.0f, "V3 neg x");
	MT_ASSERT_FLOAT(neg.y, -2.0f, "V3 neg y");
	MT_ASSERT_FLOAT(neg.z, -3.0f, "V3 neg z");

	// +=
	Vector3 acc(0.0f, 0.0f, 0.0f);
	acc += a;
	MT_ASSERT_FLOAT(acc.x, 1.0f, "V3 += x");
	MT_ASSERT_FLOAT(acc.y, 2.0f, "V3 += y");
	MT_ASSERT_FLOAT(acc.z, 3.0f, "V3 += z");

	// -=
	acc -= a;
	MT_ASSERT_FLOAT(acc.x, 0.0f, "V3 -= x");
	MT_ASSERT_FLOAT(acc.y, 0.0f, "V3 -= y");
	MT_ASSERT_FLOAT(acc.z, 0.0f, "V3 -= z");

	// clear
	Vector3 v(1.0f, 2.0f, 3.0f);
	v.clear();
	MT_ASSERT_FLOAT(v.x, 0.0f, "V3 clear x");
	MT_ASSERT_FLOAT(v.y, 0.0f, "V3 clear y");
	MT_ASSERT_FLOAT(v.z, 0.0f, "V3 clear z");

	// constants
	MT_ASSERT_FLOAT(Vector3::ZERO.x, 0.0f, "V3::ZERO x");
	MT_ASSERT_FLOAT(Vector3::UP.y,   1.0f, "V3::UP y");
}

//------------------------------------------------------------------------------
// Vector2Int
//------------------------------------------------------------------------------
void MathStructTest::testVector2Int_Operators()
{
	const Vector2Int a(2, 3);
	const Vector2Int b(5, 7);

	// +
	const Vector2Int add = a + b;
	MT_ASSERT_EQ(add.x, 7, "V2Int add x");
	MT_ASSERT_EQ(add.y, 10, "V2Int add y");

	// -
	const Vector2Int sub = b - a;
	MT_ASSERT_EQ(sub.x, 3, "V2Int sub x");
	MT_ASSERT_EQ(sub.y, 4, "V2Int sub y");

	// * scalar
	const Vector2Int scaled = a * 3;
	MT_ASSERT_EQ(scaled.x, 6,  "V2Int scale x");
	MT_ASSERT_EQ(scaled.y, 9,  "V2Int scale y");

	// unary -
	const Vector2Int neg = -a;
	MT_ASSERT_EQ(neg.x, -2, "V2Int neg x");
	MT_ASSERT_EQ(neg.y, -3, "V2Int neg y");

	// ==, !=
	MT_ASSERT(a == Vector2Int(2, 3), "V2Int ==");
	MT_ASSERT(a != b,                "V2Int !=");

	// +=, -=
	Vector2Int acc(0, 0);
	acc += a;
	MT_ASSERT_EQ(acc.x, 2, "V2Int += x");
	MT_ASSERT_EQ(acc.y, 3, "V2Int += y");
	acc -= a;
	MT_ASSERT_EQ(acc.x, 0, "V2Int -= x");
	MT_ASSERT_EQ(acc.y, 0, "V2Int -= y");

	// clear
	Vector2Int v(4, 5);
	v.clear();
	MT_ASSERT_EQ(v.x, 0, "V2Int clear x");
	MT_ASSERT_EQ(v.y, 0, "V2Int clear y");

	// ZERO
	MT_ASSERT_EQ(Vector2Int::ZERO.x, 0, "V2Int::ZERO x");
	MT_ASSERT_EQ(Vector2Int::ZERO.y, 0, "V2Int::ZERO y");
}

//------------------------------------------------------------------------------
// Color
//------------------------------------------------------------------------------
void MathStructTest::testColor_Operators()
{
	const Color a((byte)10u, (byte)20u, (byte)30u, (byte)255u);
	const Color b((byte)5u,  (byte)10u, (byte)15u, (byte)100u);

	// +
	const Color added = a + b;
	MT_ASSERT_EQ((int)added.r, 15,  "Color add r");
	MT_ASSERT_EQ((int)added.g, 30,  "Color add g");
	MT_ASSERT_EQ((int)added.b, 45,  "Color add b");
	MT_ASSERT_EQ((int)added.a, 355 & 0xFF, "Color add a wrap");

	// -
	const Color subbed = a - b;
	MT_ASSERT_EQ((int)subbed.r, 5,  "Color sub r");
	MT_ASSERT_EQ((int)subbed.g, 10, "Color sub g");
	MT_ASSERT_EQ((int)subbed.b, 15, "Color sub b");

	// +=
	Color acc((byte)1u, (byte)2u, (byte)3u, (byte)4u);
	acc += Color((byte)10u, (byte)20u, (byte)30u, (byte)40u);
	MT_ASSERT_EQ((int)acc.r, 11, "Color += r");
	MT_ASSERT_EQ((int)acc.g, 22, "Color += g");
	MT_ASSERT_EQ((int)acc.b, 33, "Color += b");
	MT_ASSERT_EQ((int)acc.a, 44, "Color += a");

	// static constants
	MT_ASSERT_EQ((int)Color::WHITE.r, 255, "Color::WHITE r");
	MT_ASSERT_EQ((int)Color::WHITE.g, 255, "Color::WHITE g");
	MT_ASSERT_EQ((int)Color::WHITE.b, 255, "Color::WHITE b");
	MT_ASSERT_EQ((int)Color::WHITE.a, 255, "Color::WHITE a");

	MT_ASSERT_EQ((int)Color::BLACK.r, 0,   "Color::BLACK r");
	MT_ASSERT_EQ((int)Color::BLACK.a, 255, "Color::BLACK a");

	MT_ASSERT_EQ((int)Color::RED.r, 255,   "Color::RED r");
	MT_ASSERT_EQ((int)Color::RED.g, 0,     "Color::RED g");
}

//------------------------------------------------------------------------------
// Quaternion
//------------------------------------------------------------------------------
void MathStructTest::testQuaternion_Basic()
{
	// identity
	const Quaternion identity;
	MT_ASSERT_FLOAT(identity.x, 0.0f, "Quat identity x");
	MT_ASSERT_FLOAT(identity.y, 0.0f, "Quat identity y");
	MT_ASSERT_FLOAT(identity.z, 0.0f, "Quat identity z");
	MT_ASSERT_FLOAT(identity.w, 1.0f, "Quat identity w");

	// dot with itself = 1
	MT_ASSERT_FLOAT(Quaternion::dot(identity, identity), 1.0f, "Quat dot(id,id)=1");

	// length of identity = 1
	MT_ASSERT_FLOAT(identity.length(), 1.0f, "Quat length(id)=1");

	// conjugate of identity
	const Quaternion conj = identity.conjugate();
	MT_ASSERT_FLOAT(conj.x,  0.0f, "Quat conj x");
	MT_ASSERT_FLOAT(conj.y,  0.0f, "Quat conj y");
	MT_ASSERT_FLOAT(conj.z,  0.0f, "Quat conj z");
	MT_ASSERT_FLOAT(conj.w,  1.0f, "Quat conj w");

	// normalize of identity = identity
	const Quaternion norm = identity.normalize();
	MT_ASSERT_FLOAT(norm.w, 1.0f, "Quat normalize(id) w");
	MT_ASSERT_FLOAT(norm.length(), 1.0f, "Quat normalize length=1");

	// * scalar
	const Quaternion scaled = identity * 2.0f;
	MT_ASSERT_FLOAT(scaled.w, 2.0f, "Quat scale w");

	// +
	const Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	const Quaternion q2(0.1f, 0.2f, 0.3f, 0.9f);
	const Quaternion sum = q1 + q2;
	MT_ASSERT_FLOAT(sum.x, 0.1f, "Quat + x");
	MT_ASSERT_FLOAT(sum.w, 1.9f, "Quat + w");

	// clear
	Quaternion q = q2;
	q.clear();
	MT_ASSERT_FLOAT(q.x, 0.0f, "Quat clear x");
	MT_ASSERT_FLOAT(q.w, 1.0f, "Quat clear w");

	// axis-angle construction -> angle should round-trip
	const Vector3 axis(0.0f, 1.0f, 0.0f);
	const float angleRad = 1.0f; // ~57 deg
	const Quaternion fromAxis(axis, angleRad);
	MT_ASSERT_FLOAT(fromAxis.length(), 1.0f, "Quat axis-angle length=1");
	MT_ASSERT_FLOAT(fromAxis.angle(), angleRad, "Quat axis-angle round-trip");
}

//------------------------------------------------------------------------------
// Vector2 — extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector2_StaticConstants()
{
	MT_ASSERT_FLOAT(Vector2::UP.x,    0.0f, "V2::UP x=0");
	MT_ASSERT_FLOAT(Vector2::UP.y,    1.0f, "V2::UP y=1");
	MT_ASSERT_FLOAT(Vector2::DOWN.x,  0.0f, "V2::DOWN x=0");
	MT_ASSERT_FLOAT(Vector2::DOWN.y, -1.0f, "V2::DOWN y=-1");
	MT_ASSERT_FLOAT(Vector2::LEFT.x, -1.0f, "V2::LEFT x=-1");
	MT_ASSERT_FLOAT(Vector2::LEFT.y,  0.0f, "V2::LEFT y=0");
	MT_ASSERT_FLOAT(Vector2::RIGHT.x, 1.0f, "V2::RIGHT x=1");
	MT_ASSERT_FLOAT(Vector2::RIGHT.y, 0.0f, "V2::RIGHT y=0");
}

void MathStructTest::testVector2_IntConstructor()
{
	Vector2 v(3, 4);
	MT_ASSERT_FLOAT(v.x, 3.0f, "V2 int ctor x");
	MT_ASSERT_FLOAT(v.y, 4.0f, "V2 int ctor y");
}

void MathStructTest::testVector2_ChainedOps()
{
	Vector2 v(1.0f, 1.0f);
	v += Vector2(1.0f, 0.0f);
	v += Vector2(0.0f, 2.0f);
	v -= Vector2(0.5f, 0.5f);
	// v = (1.5, 2.5)
	MT_ASSERT_FLOAT(v.x, 1.5f, "V2 chained x");
	MT_ASSERT_FLOAT(v.y, 2.5f, "V2 chained y");

	Vector2 scaled = v * 4.0f;
	MT_ASSERT_FLOAT(scaled.x, 6.0f, "V2 chained scale x");
	MT_ASSERT_FLOAT(scaled.y, 10.0f, "V2 chained scale y");

	Vector2 divided = scaled / 2.0f;
	MT_ASSERT_FLOAT(divided.x, 3.0f, "V2 chained div x");
	MT_ASSERT_FLOAT(divided.y, 5.0f, "V2 chained div y");
}

void MathStructTest::testVector2_NegativeValues()
{
	Vector2 a(-3.0f, -4.0f);
	Vector2 b(-1.0f, 2.0f);
	Vector2 sum = a + b;
	MT_ASSERT_FLOAT(sum.x, -4.0f, "V2 neg add x");
	MT_ASSERT_FLOAT(sum.y, -2.0f, "V2 neg add y");

	Vector2 neg = -a;
	MT_ASSERT_FLOAT(neg.x, 3.0f, "V2 double neg x");
	MT_ASSERT_FLOAT(neg.y, 4.0f, "V2 double neg y");
}

void MathStructTest::testVector2_SmallValues()
{
	Vector2 tiny(0.001f, -0.002f);
	MT_ASSERT_FLOAT(tiny.x * 1000.0f, 1.0f, "V2 tiny scale x");
	MT_ASSERT_FLOAT(tiny.y * (-500.0f), 1.0f, "V2 tiny scale y");
}

void MathStructTest::testVector2_LargeValues()
{
	Vector2 big(100000.0f, -200000.0f);
	Vector2 scaled = big * 0.00001f;
	MT_ASSERT_FLOAT(scaled.x, 1.0f, "V2 large scale x");
	MT_ASSERT_FLOAT(scaled.y, -2.0f, "V2 large scale y");
}

//------------------------------------------------------------------------------
// Vector3 — extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector3_StaticConstants()
{
	MT_ASSERT_FLOAT(Vector3::ONE.x,     1.0f,  "V3::ONE x");
	MT_ASSERT_FLOAT(Vector3::ONE.y,     1.0f,  "V3::ONE y");
	MT_ASSERT_FLOAT(Vector3::ONE.z,     1.0f,  "V3::ONE z");
	MT_ASSERT_FLOAT(Vector3::DOWN.y,   -1.0f,  "V3::DOWN y");
	MT_ASSERT_FLOAT(Vector3::LEFT.x,   -1.0f,  "V3::LEFT x");
	MT_ASSERT_FLOAT(Vector3::RIGHT.x,   1.0f,  "V3::RIGHT x");
	MT_ASSERT_FLOAT(Vector3::FORWARD.z, 1.0f,  "V3::FORWARD z");
	MT_ASSERT_FLOAT(Vector3::BACK.z,   -1.0f,  "V3::BACK z");
}

void MathStructTest::testVector3_IntConstructor()
{
	Vector3 v(1, 2, 3);
	MT_ASSERT_FLOAT(v.x, 1.0f, "V3 int ctor x");
	MT_ASSERT_FLOAT(v.y, 2.0f, "V3 int ctor y");
	MT_ASSERT_FLOAT(v.z, 3.0f, "V3 int ctor z");
}

void MathStructTest::testVector3_MultiplyScalarBothSides()
{
	Vector3 a(2.0f, 3.0f, 4.0f);
	Vector3 b = a * 5.0f;
	MT_ASSERT_FLOAT(b.x, 10.0f, "V3 *5 x");
	MT_ASSERT_FLOAT(b.y, 15.0f, "V3 *5 y");
	MT_ASSERT_FLOAT(b.z, 20.0f, "V3 *5 z");

	Vector3 c = b / 5.0f;
	MT_ASSERT_FLOAT(c.x, a.x, "V3 /5 = original x");
	MT_ASSERT_FLOAT(c.y, a.y, "V3 /5 = original y");
	MT_ASSERT_FLOAT(c.z, a.z, "V3 /5 = original z");
}

void MathStructTest::testVector3_NegativeAndClear()
{
	Vector3 v(-1.0f, -2.0f, -3.0f);
	Vector3 pos = -v;
	MT_ASSERT_FLOAT(pos.x, 1.0f, "V3 neg x");
	MT_ASSERT_FLOAT(pos.y, 2.0f, "V3 neg y");
	MT_ASSERT_FLOAT(pos.z, 3.0f, "V3 neg z");

	pos.clear();
	MT_ASSERT_FLOAT(pos.x, 0.0f, "V3 clear x");
	MT_ASSERT_FLOAT(pos.y, 0.0f, "V3 clear y");
	MT_ASSERT_FLOAT(pos.z, 0.0f, "V3 clear z");
}

void MathStructTest::testVector3_ChainedArithmetic()
{
	Vector3 acc(0.0f, 0.0f, 0.0f);
	for (int i = 1; i <= 10; ++i)
	{
		acc += Vector3((float)i, (float)i, (float)i);
	}
	// sum of 1..10 = 55
	MT_ASSERT_FLOAT(acc.x, 55.0f, "V3 chain sum x");
	MT_ASSERT_FLOAT(acc.y, 55.0f, "V3 chain sum y");
	MT_ASSERT_FLOAT(acc.z, 55.0f, "V3 chain sum z");

	for (int i = 1; i <= 10; ++i)
	{
		acc -= Vector3((float)i, (float)i, (float)i);
	}
	MT_ASSERT_FLOAT(acc.x, 0.0f, "V3 chain sub back to 0 x");
	MT_ASSERT_FLOAT(acc.y, 0.0f, "V3 chain sub back to 0 y");
	MT_ASSERT_FLOAT(acc.z, 0.0f, "V3 chain sub back to 0 z");
}

//------------------------------------------------------------------------------
// Vector2Int — extended
//------------------------------------------------------------------------------
void MathStructTest::testVector2Int_Extended()
{
	// Zero operations
	Vector2Int zero = Vector2Int::ZERO;
	MT_ASSERT_EQ(zero.x, 0, "V2Int::ZERO x=0");
	MT_ASSERT_EQ(zero.y, 0, "V2Int::ZERO y=0");

	// Large values
	Vector2Int big(10000, -10000);
	Vector2Int doubled = big * 2;
	MT_ASSERT_EQ(doubled.x, 20000, "V2Int big *2 x");
	MT_ASSERT_EQ(doubled.y, -20000, "V2Int big *2 y");

	// Chain add/subtract
	Vector2Int a(5, 10);
	Vector2Int b(3, 4);
	Vector2Int c = a + b;
	MT_ASSERT_EQ(c.x, 8, "V2Int + x");
	MT_ASSERT_EQ(c.y, 14, "V2Int + y");

	Vector2Int d = c - a;
	MT_ASSERT_EQ(d.x, b.x, "V2Int - x = b.x");
	MT_ASSERT_EQ(d.y, b.y, "V2Int - y = b.y");

	// Equality / inequality
	MT_ASSERT(c != a, "V2Int != a");
	MT_ASSERT(d == b, "V2Int d == b");

	// Negative
	Vector2Int neg = -a;
	MT_ASSERT_EQ(neg.x, -5, "V2Int neg x");
	MT_ASSERT_EQ(neg.y, -10, "V2Int neg y");

	// += and -=
	Vector2Int acc(0, 0);
	for (int i = 1; i <= 5; ++i)
	{
		acc += Vector2Int(i, i * 2);
	}
	// x: 1+2+3+4+5=15, y: 2+4+6+8+10=30
	MT_ASSERT_EQ(acc.x, 15, "V2Int chain += x");
	MT_ASSERT_EQ(acc.y, 30, "V2Int chain += y");
}

//------------------------------------------------------------------------------
// Color — extended
//------------------------------------------------------------------------------
void MathStructTest::testColor_Extended()
{
	// Default constructor (should be (0,0,0,0) or (0,0,0,255)?)
	Color c;
	(void)c; // just ensure it compiles

	// GREY
	MT_ASSERT_EQ((int)Color::GREY.r, 128, "Color::GREY r");
	MT_ASSERT_EQ((int)Color::GREY.g, 128, "Color::GREY g");
	MT_ASSERT_EQ((int)Color::GREY.b, 128, "Color::GREY b");

	// overflow wrap behavior (byte arithmetic)
	Color cMax((byte)255u, (byte)255u, (byte)255u, (byte)255u);
	Color cOne((byte)1u, (byte)1u, (byte)1u, (byte)1u);
	Color overflow = cMax + cOne;
	MT_ASSERT_EQ((int)overflow.r, 0, "Color overflow r=0");
	MT_ASSERT_EQ((int)overflow.g, 0, "Color overflow g=0");
	MT_ASSERT_EQ((int)overflow.b, 0, "Color overflow b=0");

	// underflow
	Color cZero((byte)0u, (byte)0u, (byte)0u, (byte)0u);
	Color underflow = cZero - cOne;
	MT_ASSERT_EQ((int)underflow.r, 255, "Color underflow r=255");

	// -= operator
	Color a((byte)50u, (byte)60u, (byte)70u, (byte)80u);
	Color b((byte)10u, (byte)20u, (byte)30u, (byte)40u);
	a -= b;
	MT_ASSERT_EQ((int)a.r, 40, "Color -= r");
	MT_ASSERT_EQ((int)a.g, 40, "Color -= g");
	MT_ASSERT_EQ((int)a.b, 40, "Color -= b");
	MT_ASSERT_EQ((int)a.a, 40, "Color -= a");
}

//------------------------------------------------------------------------------
// Quaternion — extended
//------------------------------------------------------------------------------
void MathStructTest::testQuaternion_Conjugate()
{
	Quaternion q(0.1f, 0.2f, 0.3f, 0.9f);
	Quaternion conj = q.conjugate();
	MT_ASSERT_FLOAT(conj.x, -0.1f, "Quat conj x");
	MT_ASSERT_FLOAT(conj.y, -0.2f, "Quat conj y");
	MT_ASSERT_FLOAT(conj.z, -0.3f, "Quat conj z");
	MT_ASSERT_FLOAT(conj.w,  0.9f, "Quat conj w");
}

void MathStructTest::testQuaternion_Normalize()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion n = q.normalize();
	MT_ASSERT_FLOAT(n.length(), 1.0f, "Quat normalize length=1");
}

void MathStructTest::testQuaternion_AxisAngle()
{
	// Y axis rotation by π/2
	const Vector3 axisY(0.0f, 1.0f, 0.0f);
	const float halfPi = 1.5707963f;
	Quaternion q(axisY, halfPi);
	MT_ASSERT_FLOAT(q.length(), 1.0f, "Quat Y-axis pi/2 len=1");
	MT_ASSERT_FLOAT(q.angle(), halfPi, "Quat Y-axis pi/2 angle round-trip");

	// Z axis rotation by π/4
	const Vector3 axisZ(0.0f, 0.0f, 1.0f);
	const float quarterPi = 0.7853982f;
	Quaternion qz(axisZ, quarterPi);
	MT_ASSERT_FLOAT(qz.length(), 1.0f, "Quat Z-axis pi/4 len=1");
	MT_ASSERT_FLOAT(qz.angle(), quarterPi, "Quat Z-axis pi/4 angle round-trip");
}

void MathStructTest::testQuaternion_DotProduct()
{
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	MT_ASSERT_FLOAT(Quaternion::dot(q1, q2), 1.0f, "Quat dot identity=1");

	Quaternion q3(1.0f, 0.0f, 0.0f, 0.0f);
	MT_ASSERT_FLOAT(Quaternion::dot(q1, q3), 0.0f, "Quat dot perp=0");
}

void MathStructTest::testQuaternion_Lerp()
{
	Quaternion q1(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	// lerp between identical quaternions = same quaternion
	Quaternion lerped = Quaternion::lerp(q1, q2, 0.5f);
	MT_ASSERT_FLOAT(lerped.x, 0.0f, "Quat lerp same x");
	MT_ASSERT_FLOAT(lerped.w, 1.0f, "Quat lerp same w");

	// lerp at t=0 -> q1
	Quaternion at0 = Quaternion::lerp(q1, q2, 0.0f);
	MT_ASSERT_FLOAT(at0.w, q1.w, "Quat lerp t=0 w=q1.w");

	// lerp at t=1 -> q2
	Quaternion at1 = Quaternion::lerp(q1, q2, 1.0f);
	MT_ASSERT_FLOAT(at1.w, q2.w, "Quat lerp t=1 w=q2.w");
}

void MathStructTest::testQuaternion_AddAndScale()
{
	Quaternion q1(0.1f, 0.2f, 0.3f, 0.4f);
	Quaternion q2(0.5f, 0.6f, 0.7f, 0.8f);
	Quaternion sum = q1 + q2;
	MT_ASSERT_FLOAT(sum.x, 0.6f, "Quat + x");
	MT_ASSERT_FLOAT(sum.y, 0.8f, "Quat + y");
	MT_ASSERT_FLOAT(sum.z, 1.0f, "Quat + z");
	MT_ASSERT_FLOAT(sum.w, 1.2f, "Quat + w");

	Quaternion scaled = q1 * 2.0f;
	MT_ASSERT_FLOAT(scaled.x, 0.2f, "Quat *2 x");
	MT_ASSERT_FLOAT(scaled.y, 0.4f, "Quat *2 y");
	MT_ASSERT_FLOAT(scaled.z, 0.6f, "Quat *2 z");
	MT_ASSERT_FLOAT(scaled.w, 0.8f, "Quat *2 w");
}

void MathStructTest::testQuaternion_CopyConstructor()
{
	Quaternion q1(0.1f, 0.2f, 0.3f, 0.4f);
	Quaternion q2(q1);
	MT_ASSERT_FLOAT(q2.x, q1.x, "Quat copy x");
	MT_ASSERT_FLOAT(q2.y, q1.y, "Quat copy y");
	MT_ASSERT_FLOAT(q2.z, q1.z, "Quat copy z");
	MT_ASSERT_FLOAT(q2.w, q1.w, "Quat copy w");
}

void MathStructTest::testQuaternion_Negate()
{
	Quaternion q(0.1f, -0.2f, 0.3f, 0.9f);
	Quaternion neg = -q;
	MT_ASSERT_FLOAT(neg.x, -0.1f, "Quat neg x");
	MT_ASSERT_FLOAT(neg.y,  0.2f, "Quat neg y");
	MT_ASSERT_FLOAT(neg.z, -0.3f, "Quat neg z");
	MT_ASSERT_FLOAT(neg.w, -0.9f, "Quat neg w");
}

void MathStructTest::testQuaternion_MultiplyAssignScale()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q *= 0.5f;
	MT_ASSERT_FLOAT(q.x, 0.5f, "Quat *=0.5 x");
	MT_ASSERT_FLOAT(q.y, 1.0f, "Quat *=0.5 y");
	MT_ASSERT_FLOAT(q.z, 1.5f, "Quat *=0.5 z");
	MT_ASSERT_FLOAT(q.w, 2.0f, "Quat *=0.5 w");
}

void MathStructTest::testQuaternion_DivideAssignScale()
{
	Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
	q /= 2.0f;
	MT_ASSERT_FLOAT(q.x, 1.0f, "Quat /=2 x");
	MT_ASSERT_FLOAT(q.y, 2.0f, "Quat /=2 y");
	MT_ASSERT_FLOAT(q.z, 3.0f, "Quat /=2 z");
	MT_ASSERT_FLOAT(q.w, 4.0f, "Quat /=2 w");
}

void MathStructTest::testQuaternion_EulerAngles()
{
	// Identity quaternion should give (0,0,0) euler angles
	Quaternion identity;
	Vector3 euler = identity.eulerAngles();
	MT_ASSERT_FLOAT(euler.x, 0.0f, "Quat identity euler x=0");
	MT_ASSERT_FLOAT(euler.y, 0.0f, "Quat identity euler y=0");
	MT_ASSERT_FLOAT(euler.z, 0.0f, "Quat identity euler z=0");

	MT_ASSERT_FLOAT(identity.pitch(), 0.0f, "Quat identity pitch=0");
	MT_ASSERT_FLOAT(identity.yaw(), 0.0f, "Quat identity yaw=0");
	MT_ASSERT_FLOAT(identity.roll(), 0.0f, "Quat identity roll=0");
}

void MathStructTest::testQuaternion_Inverse()
{
	// For a unit quaternion, inverse = conjugate
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion inv = q.inverse();
	MT_ASSERT_FLOAT(inv.x, 0.0f, "Quat inv identity x");
	MT_ASSERT_FLOAT(inv.y, 0.0f, "Quat inv identity y");
	MT_ASSERT_FLOAT(inv.z, 0.0f, "Quat inv identity z");
	MT_ASSERT_FLOAT(inv.w, 1.0f, "Quat inv identity w");
}

//------------------------------------------------------------------------------
// Cross-type interactions
//------------------------------------------------------------------------------
void MathStructTest::testCrossType_Vector2ToVec3()
{
	// A Vector3 can be constructed from three floats like a Vector2 + z
	Vector2 v2(1.0f, 2.0f);
	Vector3 v3(v2.x, v2.y, 0.0f);
	MT_ASSERT_FLOAT(v3.x, v2.x, "V2->V3 x");
	MT_ASSERT_FLOAT(v3.y, v2.y, "V2->V3 y");
	MT_ASSERT_FLOAT(v3.z, 0.0f, "V2->V3 z=0");
}

void MathStructTest::testCrossType_V3AsQuatAxis()
{
	// Standard rotation axes should produce unit quaternions
	const float angle = 1.0f;
	Quaternion qx(Vector3(1.0f, 0.0f, 0.0f), angle);
	Quaternion qy(Vector3(0.0f, 1.0f, 0.0f), angle);
	Quaternion qz(Vector3(0.0f, 0.0f, 1.0f), angle);
	MT_ASSERT_FLOAT(qx.length(), 1.0f, "qx length=1");
	MT_ASSERT_FLOAT(qy.length(), 1.0f, "qy length=1");
	MT_ASSERT_FLOAT(qz.length(), 1.0f, "qz length=1");
}

void MathStructTest::testCrossType_ColorArithmetic()
{
	// Multiple additions
	Color c((byte)0u, (byte)0u, (byte)0u, (byte)0u);
	FOR(10)
	{
		c += Color((byte)10u, (byte)5u, (byte)3u, (byte)1u);
	}
	MT_ASSERT_EQ((int)c.r, 100, "Color accum r");
	MT_ASSERT_EQ((int)c.g, 50, "Color accum g");
	MT_ASSERT_EQ((int)c.b, 30, "Color accum b");
	MT_ASSERT_EQ((int)c.a, 10, "Color accum a");
}

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
void MathStructTest::testGeneralMathStructs()
{
	testVector2_Operators();
	testVector3_Operators();
	testVector2Int_Operators();
	testColor_Operators();
	testQuaternion_Basic();
	testVector2_StaticConstants();
	testVector2_IntConstructor();
	testVector2_ChainedOps();
	testVector2_NegativeValues();
	testVector2_SmallValues();
	testVector2_LargeValues();
	testVector3_StaticConstants();
	testVector3_IntConstructor();
	testVector3_MultiplyScalarBothSides();
	testVector3_NegativeAndClear();
	testVector3_ChainedArithmetic();
	testVector2Int_Extended();
	testColor_Extended();
	testQuaternion_Conjugate();
	testQuaternion_Normalize();
	testQuaternion_AxisAngle();
	testQuaternion_DotProduct();
	testQuaternion_Lerp();
	testQuaternion_AddAndScale();
	testQuaternion_CopyConstructor();
	testQuaternion_Negate();
	testQuaternion_MultiplyAssignScale();
	testQuaternion_DivideAssignScale();
	testQuaternion_EulerAngles();
	testQuaternion_Inverse();
	testCrossType_Vector2ToVec3();
	testCrossType_V3AsQuatAxis();
	testCrossType_ColorArithmetic();
	testExtended();
}


#define MATH_ET_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg)); } } while(0)
#define MET_ASSERT_EQ_INT(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define MET_ASSERT_EQ_UINT(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg) + \
		" | expected=" + UIToS((uint)(b)) + " actual=" + UIToS((uint)(a))); } } while(0)
#define MET_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-4f) { ERROR(string("MathStructTest testGeneralMathStructs FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

//------------------------------------------------------------------------------
// Vector2Short
//------------------------------------------------------------------------------
void MathStructTest::testVector2Short()
{
	// constructor + member access
	const Vector2Short a((short)3, (short)4);
	MET_ASSERT_EQ_INT(a.x, 3, "V2S ctor x");
	MET_ASSERT_EQ_INT(a.y, 4, "V2S ctor y");

	// default ctor == 0
	const Vector2Short zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V2S default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V2S default y=0");

	// operator+
	const Vector2Short b((short)1, (short)2);
	const Vector2Short add = a + b;
	MET_ASSERT_EQ_INT(add.x, 4, "V2S add x");
	MET_ASSERT_EQ_INT(add.y, 6, "V2S add y");

	// operator-
	const Vector2Short sub = a - b;
	MET_ASSERT_EQ_INT(sub.x, 2, "V2S sub x");
	MET_ASSERT_EQ_INT(sub.y, 2, "V2S sub y");

	// operator* scalar
	const Vector2Short scaled = a * (short)2;
	MET_ASSERT_EQ_INT(scaled.x, 6, "V2S scale x");
	MET_ASSERT_EQ_INT(scaled.y, 8, "V2S scale y");

	// unary -
	const Vector2Short neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -3, "V2S neg x");
	MET_ASSERT_EQ_INT(neg.y, -4, "V2S neg y");

	// operator+=
	Vector2Short acc((short)5, (short)6);
	acc += Vector2Short((short)1, (short)2);
	MET_ASSERT_EQ_INT(acc.x, 6, "V2S += x");
	MET_ASSERT_EQ_INT(acc.y, 8, "V2S += y");

	// operator-=
	acc -= Vector2Short((short)3, (short)3);
	MET_ASSERT_EQ_INT(acc.x, 3, "V2S -= x");
	MET_ASSERT_EQ_INT(acc.y, 5, "V2S -= y");

	// operator==
	const Vector2Short c((short)3, (short)4);
	MATH_ET_ASSERT(a == c, "V2S == true");
	MATH_ET_ASSERT(!(a == b), "V2S == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V2S != true");
	MATH_ET_ASSERT(!(a != c), "V2S != false");

	// operator< (compares sum)
	MATH_ET_ASSERT(b < a, "V2S < : (1+2)<(3+4)");
	MATH_ET_ASSERT(!(a < b), "V2S < false: (3+4)>=(1+2)");

	// clear
	Vector2Short d((short)10, (short)20);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V2S clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V2S clear y=0");
}

//------------------------------------------------------------------------------
// Vector2UInt
//------------------------------------------------------------------------------
void MathStructTest::testVector2UInt()
{
	// constructor
	const Vector2UInt a(10u, 20u);
	MET_ASSERT_EQ_UINT(a.x, 10u, "V2UI ctor x");
	MET_ASSERT_EQ_UINT(a.y, 20u, "V2UI ctor y");

	// default ctor
	const Vector2UInt zero;
	MET_ASSERT_EQ_UINT(zero.x, 0u, "V2UI default x=0");
	MET_ASSERT_EQ_UINT(zero.y, 0u, "V2UI default y=0");

	// operator+
	const Vector2UInt b(5u, 3u);
	const Vector2UInt add = a + b;
	MET_ASSERT_EQ_UINT(add.x, 15u, "V2UI add x");
	MET_ASSERT_EQ_UINT(add.y, 23u, "V2UI add y");

	// operator-
	const Vector2UInt sub = a - b;
	MET_ASSERT_EQ_UINT(sub.x, 5u, "V2UI sub x");
	MET_ASSERT_EQ_UINT(sub.y, 17u, "V2UI sub y");

	// operator+=
	Vector2UInt acc(100u, 200u);
	acc += Vector2UInt(50u, 75u);
	MET_ASSERT_EQ_UINT(acc.x, 150u, "V2UI += x");
	MET_ASSERT_EQ_UINT(acc.y, 275u, "V2UI += y");

	// operator-=
	acc -= Vector2UInt(30u, 25u);
	MET_ASSERT_EQ_UINT(acc.x, 120u, "V2UI -= x");
	MET_ASSERT_EQ_UINT(acc.y, 250u, "V2UI -= y");

	// operator==
	const Vector2UInt c(10u, 20u);
	MATH_ET_ASSERT(a == c, "V2UI == true");
	MATH_ET_ASSERT(!(a == b), "V2UI == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V2UI != true");
	MATH_ET_ASSERT(!(a != c), "V2UI != false");

	// operator< (sum comparison)
	MATH_ET_ASSERT(b < a, "V2UI < : (5+3)<(10+20)");

	// clear
	Vector2UInt d(9u, 8u);
	d.clear();
	MET_ASSERT_EQ_UINT(d.x, 0u, "V2UI clear x=0");
	MET_ASSERT_EQ_UINT(d.y, 0u, "V2UI clear y=0");
}

//------------------------------------------------------------------------------
// Vector2UShort
//------------------------------------------------------------------------------
void MathStructTest::testVector2UShort()
{
	// constructor
	const Vector2UShort a((ushort)7, (ushort)9);
	MET_ASSERT_EQ_INT(a.x, 7, "V2US ctor x");
	MET_ASSERT_EQ_INT(a.y, 9, "V2US ctor y");

	// default ctor
	const Vector2UShort zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V2US default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V2US default y=0");

	// operator+
	const Vector2UShort b((ushort)3, (ushort)1);
	const Vector2UShort add = a + b;
	MET_ASSERT_EQ_INT(add.x, 10, "V2US add x");
	MET_ASSERT_EQ_INT(add.y, 10, "V2US add y");

	// operator-
	const Vector2UShort sub = a - b;
	MET_ASSERT_EQ_INT(sub.x, 4, "V2US sub x");
	MET_ASSERT_EQ_INT(sub.y, 8, "V2US sub y");

	// operator* scalar
	const Vector2UShort scaled = a * (short)3;
	MET_ASSERT_EQ_INT(scaled.x, 21, "V2US scale x");
	MET_ASSERT_EQ_INT(scaled.y, 27, "V2US scale y");

	// operator+=
	Vector2UShort acc((ushort)10, (ushort)20);
	acc += Vector2UShort((ushort)5, (ushort)5);
	MET_ASSERT_EQ_INT(acc.x, 15, "V2US += x");
	MET_ASSERT_EQ_INT(acc.y, 25, "V2US += y");

	// operator-=
	acc -= Vector2UShort((ushort)5, (ushort)5);
	MET_ASSERT_EQ_INT(acc.x, 10, "V2US -= x");
	MET_ASSERT_EQ_INT(acc.y, 20, "V2US -= y");

	// operator==
	const Vector2UShort c((ushort)7, (ushort)9);
	MATH_ET_ASSERT(a == c, "V2US == true");
	MATH_ET_ASSERT(!(a == b), "V2US == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V2US != true");
	MATH_ET_ASSERT(!(a != c), "V2US != false");

	// operator< (sum comparison)
	MATH_ET_ASSERT(b < a, "V2US < : (3+1)<(7+9)");

	// clear
	Vector2UShort d((ushort)50, (ushort)60);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V2US clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V2US clear y=0");
}

//------------------------------------------------------------------------------
// Vector3Int
//------------------------------------------------------------------------------
void MathStructTest::testVector3Int()
{
	// constructor
	const Vector3Int a(1, 2, 3);
	MET_ASSERT_EQ_INT(a.x, 1, "V3I ctor x");
	MET_ASSERT_EQ_INT(a.y, 2, "V3I ctor y");
	MET_ASSERT_EQ_INT(a.z, 3, "V3I ctor z");

	// default ctor
	const Vector3Int zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V3I default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V3I default y=0");
	MET_ASSERT_EQ_INT(zero.z, 0, "V3I default z=0");

	// operator+
	const Vector3Int b(4, 5, 6);
	const Vector3Int add = a + b;
	MET_ASSERT_EQ_INT(add.x, 5, "V3I add x");
	MET_ASSERT_EQ_INT(add.y, 7, "V3I add y");
	MET_ASSERT_EQ_INT(add.z, 9, "V3I add z");

	// operator-
	const Vector3Int sub = b - a;
	MET_ASSERT_EQ_INT(sub.x, 3, "V3I sub x");
	MET_ASSERT_EQ_INT(sub.y, 3, "V3I sub y");
	MET_ASSERT_EQ_INT(sub.z, 3, "V3I sub z");

	// operator* scalar
	const Vector3Int scaled = a * 3;
	MET_ASSERT_EQ_INT(scaled.x, 3, "V3I scale x");
	MET_ASSERT_EQ_INT(scaled.y, 6, "V3I scale y");
	MET_ASSERT_EQ_INT(scaled.z, 9, "V3I scale z");

	// unary -
	const Vector3Int neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -1, "V3I neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V3I neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V3I neg z");

	// operator+=
	Vector3Int acc(10, 20, 30);
	acc += Vector3Int(5, 5, 5);
	MET_ASSERT_EQ_INT(acc.x, 15, "V3I += x");
	MET_ASSERT_EQ_INT(acc.y, 25, "V3I += y");
	MET_ASSERT_EQ_INT(acc.z, 35, "V3I += z");

	// operator-=
	acc -= Vector3Int(5, 5, 5);
	MET_ASSERT_EQ_INT(acc.x, 10, "V3I -= x");
	MET_ASSERT_EQ_INT(acc.y, 20, "V3I -= y");
	MET_ASSERT_EQ_INT(acc.z, 30, "V3I -= z");

	// operator==
	const Vector3Int c(1, 2, 3);
	MATH_ET_ASSERT(a == c, "V3I == true");
	MATH_ET_ASSERT(!(a == b), "V3I == false");

	// operator!=
	MATH_ET_ASSERT(a != b, "V3I != true");
	MATH_ET_ASSERT(!(a != c), "V3I != false");

	// operator< (sum comparison)
	MATH_ET_ASSERT(a < b, "V3I < : 1+2+3 < 4+5+6");
	MATH_ET_ASSERT(!(b < a), "V3I < false");

	// clear
	Vector3Int d(7, 8, 9);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V3I clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V3I clear y=0");
	MET_ASSERT_EQ_INT(d.z, 0, "V3I clear z=0");
}

//------------------------------------------------------------------------------
// Vector4
//------------------------------------------------------------------------------
void MathStructTest::testVector4()
{
	// constructor
	const Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	MET_ASSERT_FLOAT(a.x, 1.0f, "V4 ctor x");
	MET_ASSERT_FLOAT(a.y, 2.0f, "V4 ctor y");
	MET_ASSERT_FLOAT(a.z, 3.0f, "V4 ctor z");
	MET_ASSERT_FLOAT(a.w, 4.0f, "V4 ctor w");

	// default ctor
	const Vector4 zero;
	MET_ASSERT_FLOAT(zero.x, 0.0f, "V4 default x=0");
	MET_ASSERT_FLOAT(zero.y, 0.0f, "V4 default y=0");
	MET_ASSERT_FLOAT(zero.z, 0.0f, "V4 default z=0");
	MET_ASSERT_FLOAT(zero.w, 0.0f, "V4 default w=0");

	// operator+
	const Vector4 b(5.0f, 6.0f, 7.0f, 8.0f);
	const Vector4 add = a + b;
	MET_ASSERT_FLOAT(add.x, 6.0f, "V4 add x");
	MET_ASSERT_FLOAT(add.y, 8.0f, "V4 add y");
	MET_ASSERT_FLOAT(add.z, 10.0f, "V4 add z");
	MET_ASSERT_FLOAT(add.w, 12.0f, "V4 add w");

	// operator-
	const Vector4 sub = b - a;
	MET_ASSERT_FLOAT(sub.x, 4.0f, "V4 sub x");
	MET_ASSERT_FLOAT(sub.y, 4.0f, "V4 sub y");
	MET_ASSERT_FLOAT(sub.z, 4.0f, "V4 sub z");
	MET_ASSERT_FLOAT(sub.w, 4.0f, "V4 sub w");

	// unary -
	const Vector4 neg = -a;
	MET_ASSERT_FLOAT(neg.x, -1.0f, "V4 neg x");
	MET_ASSERT_FLOAT(neg.y, -2.0f, "V4 neg y");
	MET_ASSERT_FLOAT(neg.z, -3.0f, "V4 neg z");
	MET_ASSERT_FLOAT(neg.w, -4.0f, "V4 neg w");

	// operator+= (fixed bug: was using -= before)
	Vector4 acc(1.0f, 1.0f, 1.0f, 1.0f);
	acc += Vector4(2.0f, 3.0f, 4.0f, 5.0f);
	MET_ASSERT_FLOAT(acc.x, 3.0f, "V4 += x");
	MET_ASSERT_FLOAT(acc.y, 4.0f, "V4 += y");
	MET_ASSERT_FLOAT(acc.z, 5.0f, "V4 += z");
	MET_ASSERT_FLOAT(acc.w, 6.0f, "V4 += w");

	// operator-=
	acc -= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	MET_ASSERT_FLOAT(acc.x, 2.0f, "V4 -= x");
	MET_ASSERT_FLOAT(acc.y, 3.0f, "V4 -= y");
	MET_ASSERT_FLOAT(acc.z, 4.0f, "V4 -= z");
	MET_ASSERT_FLOAT(acc.w, 5.0f, "V4 -= w");

	// clear
	Vector4 d(9.0f, 8.0f, 7.0f, 6.0f);
	d.clear();
	MET_ASSERT_FLOAT(d.x, 0.0f, "V4 clear x=0");
	MET_ASSERT_FLOAT(d.y, 0.0f, "V4 clear y=0");
	MET_ASSERT_FLOAT(d.z, 0.0f, "V4 clear z=0");
	MET_ASSERT_FLOAT(d.w, 0.0f, "V4 clear w=0");

	// -= semantics are preserved
	Vector4 e(10.0f, 10.0f, 10.0f, 10.0f);
	e -= Vector4(3.0f, 4.0f, 5.0f, 6.0f);
	MET_ASSERT_FLOAT(e.x, 7.0f, "V4 -= semantics x");
	MET_ASSERT_FLOAT(e.y, 6.0f, "V4 -= semantics y");
	MET_ASSERT_FLOAT(e.z, 5.0f, "V4 -= semantics z");
	MET_ASSERT_FLOAT(e.w, 4.0f, "V4 -= semantics w");
}

//------------------------------------------------------------------------------
// Vector4Int
//------------------------------------------------------------------------------
void MathStructTest::testVector4Int()
{
	// constructor
	const Vector4Int a(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(a.x, 1, "V4I ctor x");
	MET_ASSERT_EQ_INT(a.y, 2, "V4I ctor y");
	MET_ASSERT_EQ_INT(a.z, 3, "V4I ctor z");
	MET_ASSERT_EQ_INT(a.w, 4, "V4I ctor w");

	// default ctor
	const Vector4Int zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V4I default x=0");
	MET_ASSERT_EQ_INT(zero.y, 0, "V4I default y=0");
	MET_ASSERT_EQ_INT(zero.z, 0, "V4I default z=0");
	MET_ASSERT_EQ_INT(zero.w, 0, "V4I default w=0");

	// operator+
	const Vector4Int b(10, 20, 30, 40);
	const Vector4Int add = a + b;
	MET_ASSERT_EQ_INT(add.x, 11, "V4I add x");
	MET_ASSERT_EQ_INT(add.y, 22, "V4I add y");
	MET_ASSERT_EQ_INT(add.z, 33, "V4I add z");
	MET_ASSERT_EQ_INT(add.w, 44, "V4I add w");

	// operator-
	const Vector4Int sub = b - a;
	MET_ASSERT_EQ_INT(sub.x, 9,  "V4I sub x");
	MET_ASSERT_EQ_INT(sub.y, 18, "V4I sub y");
	MET_ASSERT_EQ_INT(sub.z, 27, "V4I sub z");
	MET_ASSERT_EQ_INT(sub.w, 36, "V4I sub w");

	// unary -
	const Vector4Int neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -1, "V4I neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V4I neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V4I neg z");
	MET_ASSERT_EQ_INT(neg.w, -4, "V4I neg w");

	// operator+=
	Vector4Int acc(5, 5, 5, 5);
	acc += Vector4Int(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(acc.x, 6,  "V4I += x");
	MET_ASSERT_EQ_INT(acc.y, 7,  "V4I += y");
	MET_ASSERT_EQ_INT(acc.z, 8,  "V4I += z");
	MET_ASSERT_EQ_INT(acc.w, 9,  "V4I += w");

	// operator-=
	acc -= Vector4Int(1, 1, 1, 1);
	MET_ASSERT_EQ_INT(acc.x, 5, "V4I -= x");
	MET_ASSERT_EQ_INT(acc.y, 6, "V4I -= y");
	MET_ASSERT_EQ_INT(acc.z, 7, "V4I -= z");
	MET_ASSERT_EQ_INT(acc.w, 8, "V4I -= w");

	// clear
	Vector4Int d(9, 8, 7, 6);
	d.clear();
	MET_ASSERT_EQ_INT(d.x, 0, "V4I clear x=0");
	MET_ASSERT_EQ_INT(d.y, 0, "V4I clear y=0");
	MET_ASSERT_EQ_INT(d.z, 0, "V4I clear z=0");
	MET_ASSERT_EQ_INT(d.w, 0, "V4I clear w=0");

	// large positive values
	const Vector4Int big(1000, 2000, 3000, 4000);
	const Vector4Int bigAdd = big + Vector4Int(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(bigAdd.x, 1001, "V4I big add x");
	MET_ASSERT_EQ_INT(bigAdd.w, 4004, "V4I big add w");

	// negative values
	const Vector4Int neg2(-10, -20, -30, -40);
	const Vector4Int negAdd = neg2 + Vector4Int(10, 20, 30, 40);
	MET_ASSERT_EQ_INT(negAdd.x, 0, "V4I neg add x=0");
	MET_ASSERT_EQ_INT(negAdd.y, 0, "V4I neg add y=0");
	MET_ASSERT_EQ_INT(negAdd.z, 0, "V4I neg add z=0");
	MET_ASSERT_EQ_INT(negAdd.w, 0, "V4I neg add w=0");
}

//------------------------------------------------------------------------------
// Vector2Short extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector2Short_Extended()
{
	// equality/inequality
	Vector2Short a(3, 4);
	Vector2Short b(3, 4);
	Vector2Short c(1, 2);
	MATH_ET_ASSERT(a == b, "V2S eq same");
	MATH_ET_ASSERT(!(a == c), "V2S eq diff");
	MATH_ET_ASSERT(a != c, "V2S neq");
	MATH_ET_ASSERT(!(a != b), "V2S neq same");

	// unary minus
	Vector2Short neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -3, "V2S unary- x");
	MET_ASSERT_EQ_INT(neg.y, -4, "V2S unary- y");

	// less-than (sum comparison)
	Vector2Short v2s_small(1, 1);
	Vector2Short v2s_large(5, 5);
	MATH_ET_ASSERT(v2s_small < v2s_large, "V2S small < large");
	MATH_ET_ASSERT(!(v2s_large < v2s_small), "V2S not large < small");

	// operator* (scale)
	Vector2Short sc(2, 3);
	Vector2Short scaled = sc * (short)4;
	MET_ASSERT_EQ_INT(scaled.x, 8, "V2S scale x");
	MET_ASSERT_EQ_INT(scaled.y, 12, "V2S scale y");

	// operator+=
	Vector2Short acc(1, 2);
	acc += Vector2Short(10, 20);
	MET_ASSERT_EQ_INT(acc.x, 11, "V2S += x");
	MET_ASSERT_EQ_INT(acc.y, 22, "V2S += y");

	// operator-=
	acc -= Vector2Short(1, 2);
	MET_ASSERT_EQ_INT(acc.x, 10, "V2S -= x");
	MET_ASSERT_EQ_INT(acc.y, 20, "V2S -= y");

	// clear then check
	Vector2Short q(99, 88);
	q.clear();
	MET_ASSERT_EQ_INT(q.x, 0, "V2S clear x");
	MET_ASSERT_EQ_INT(q.y, 0, "V2S clear y");

	// zero default
	Vector2Short zero;
	MET_ASSERT_EQ_INT(zero.x, 0, "V2S default x");
	MET_ASSERT_EQ_INT(zero.y, 0, "V2S default y");

	// boundary short max/min
	Vector2Short maxv((short)32767, (short)-32767);
	MET_ASSERT_EQ_INT(maxv.x, 32767, "V2S max x");
	MET_ASSERT_EQ_INT(maxv.y, -32767, "V2S min y");

	// chained add/sub
	Vector2Short ch(5, 5);
	Vector2Short r = (ch + Vector2Short(3, 3)) - Vector2Short(2, 2);
	MET_ASSERT_EQ_INT(r.x, 6, "V2S chain x");
	MET_ASSERT_EQ_INT(r.y, 6, "V2S chain y");
}

//------------------------------------------------------------------------------
// Vector3Int extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector3Int_Extended()
{
	// default constructor
	Vector3Int def;
	MET_ASSERT_EQ_INT(def.x, 0, "V3I def x");
	MET_ASSERT_EQ_INT(def.y, 0, "V3I def y");
	MET_ASSERT_EQ_INT(def.z, 0, "V3I def z");

	// parameterized constructor
	Vector3Int v(1, 2, 3);
	MET_ASSERT_EQ_INT(v.x, 1, "V3I ctor x");
	MET_ASSERT_EQ_INT(v.y, 2, "V3I ctor y");
	MET_ASSERT_EQ_INT(v.z, 3, "V3I ctor z");

	// add
	Vector3Int a(10, 20, 30);
	Vector3Int b(1, 2, 3);
	Vector3Int add = a + b;
	MET_ASSERT_EQ_INT(add.x, 11, "V3I add x");
	MET_ASSERT_EQ_INT(add.y, 22, "V3I add y");
	MET_ASSERT_EQ_INT(add.z, 33, "V3I add z");

	// sub
	Vector3Int sub = a - b;
	MET_ASSERT_EQ_INT(sub.x, 9, "V3I sub x");
	MET_ASSERT_EQ_INT(sub.y, 18, "V3I sub y");
	MET_ASSERT_EQ_INT(sub.z, 27, "V3I sub z");

	// multiply scalar
	Vector3Int mul = b * 5;
	MET_ASSERT_EQ_INT(mul.x, 5, "V3I mul x");
	MET_ASSERT_EQ_INT(mul.y, 10, "V3I mul y");
	MET_ASSERT_EQ_INT(mul.z, 15, "V3I mul z");

	// unary minus
	Vector3Int neg = -v;
	MET_ASSERT_EQ_INT(neg.x, -1, "V3I neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V3I neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V3I neg z");

	// +=
	Vector3Int acc(5, 5, 5);
	acc += Vector3Int(1, 2, 3);
	MET_ASSERT_EQ_INT(acc.x, 6, "V3I += x");
	MET_ASSERT_EQ_INT(acc.y, 7, "V3I += y");
	MET_ASSERT_EQ_INT(acc.z, 8, "V3I += z");

	// -=
	acc -= Vector3Int(1, 1, 1);
	MET_ASSERT_EQ_INT(acc.x, 5, "V3I -= x");
	MET_ASSERT_EQ_INT(acc.y, 6, "V3I -= y");
	MET_ASSERT_EQ_INT(acc.z, 7, "V3I -= z");

	// equality
	Vector3Int e1(1, 2, 3), e2(1, 2, 3), e3(1, 2, 4);
	MATH_ET_ASSERT(e1 == e2, "V3I eq");
	MATH_ET_ASSERT(e1 != e3, "V3I neq");
	MATH_ET_ASSERT(!(e1 == e3), "V3I not eq diff");

	// less-than (sum)
	Vector3Int s1(1, 1, 1), s2(3, 3, 3);
	MATH_ET_ASSERT(s1 < s2, "V3I s1 < s2");
	MATH_ET_ASSERT(!(s2 < s1), "V3I not s2 < s1");

	// clear
	Vector3Int cl(7, 8, 9);
	cl.clear();
	MET_ASSERT_EQ_INT(cl.x, 0, "V3I clear x");
	MET_ASSERT_EQ_INT(cl.y, 0, "V3I clear y");
	MET_ASSERT_EQ_INT(cl.z, 0, "V3I clear z");

	// large values
	Vector3Int big(1000000, 2000000, 3000000);
	Vector3Int bigr = big + Vector3Int(1, 2, 3);
	MET_ASSERT_EQ_INT(bigr.x, 1000001, "V3I big x");
	MET_ASSERT_EQ_INT(bigr.y, 2000002, "V3I big y");
	MET_ASSERT_EQ_INT(bigr.z, 3000003, "V3I big z");

	// negative values
	Vector3Int nv(-5, -10, -15);
	Vector3Int nvr = nv + Vector3Int(5, 10, 15);
	MET_ASSERT_EQ_INT(nvr.x, 0, "V3I neg+pos x");
	MET_ASSERT_EQ_INT(nvr.y, 0, "V3I neg+pos y");
	MET_ASSERT_EQ_INT(nvr.z, 0, "V3I neg+pos z");
}

//------------------------------------------------------------------------------
// Vector4 extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector4_Extended()
{
	// default
	Vector4 def;
	MET_ASSERT_FLOAT(def.x, 0.0f, "V4 def x");
	MET_ASSERT_FLOAT(def.y, 0.0f, "V4 def y");
	MET_ASSERT_FLOAT(def.z, 0.0f, "V4 def z");
	MET_ASSERT_FLOAT(def.w, 0.0f, "V4 def w");

	// parameterized
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	MET_ASSERT_FLOAT(v.x, 1.0f, "V4 ctor x");
	MET_ASSERT_FLOAT(v.y, 2.0f, "V4 ctor y");
	MET_ASSERT_FLOAT(v.z, 3.0f, "V4 ctor z");
	MET_ASSERT_FLOAT(v.w, 4.0f, "V4 ctor w");

	// add
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(0.5f, 0.5f, 0.5f, 0.5f);
	Vector4 add = a + b;
	MET_ASSERT_FLOAT(add.x, 1.5f, "V4 add x");
	MET_ASSERT_FLOAT(add.y, 2.5f, "V4 add y");
	MET_ASSERT_FLOAT(add.z, 3.5f, "V4 add z");
	MET_ASSERT_FLOAT(add.w, 4.5f, "V4 add w");

	// sub
	Vector4 sub = a - b;
	MET_ASSERT_FLOAT(sub.x, 0.5f, "V4 sub x");
	MET_ASSERT_FLOAT(sub.y, 1.5f, "V4 sub y");
	MET_ASSERT_FLOAT(sub.z, 2.5f, "V4 sub z");
	MET_ASSERT_FLOAT(sub.w, 3.5f, "V4 sub w");

	// unary minus
	Vector4 neg = -a;
	MET_ASSERT_FLOAT(neg.x, -1.0f, "V4 neg x");
	MET_ASSERT_FLOAT(neg.y, -2.0f, "V4 neg y");
	MET_ASSERT_FLOAT(neg.z, -3.0f, "V4 neg z");
	MET_ASSERT_FLOAT(neg.w, -4.0f, "V4 neg w");

	// +=
	Vector4 acc(1.0f, 2.0f, 3.0f, 4.0f);
	acc += Vector4(0.1f, 0.2f, 0.3f, 0.4f);
	MET_ASSERT_FLOAT(acc.x, 1.1f, "V4 += x");
	MET_ASSERT_FLOAT(acc.y, 2.2f, "V4 += y");
	MET_ASSERT_FLOAT(acc.z, 3.3f, "V4 += z");
	MET_ASSERT_FLOAT(acc.w, 4.4f, "V4 += w");

	// -=
	acc -= Vector4(0.1f, 0.2f, 0.3f, 0.4f);
	MET_ASSERT_FLOAT(acc.x, 1.0f, "V4 -= x");
	MET_ASSERT_FLOAT(acc.y, 2.0f, "V4 -= y");
	MET_ASSERT_FLOAT(acc.z, 3.0f, "V4 -= z");
	MET_ASSERT_FLOAT(acc.w, 4.0f, "V4 -= w");

	// clear
	Vector4 cl(9.9f, 8.8f, 7.7f, 6.6f);
	cl.clear();
	MET_ASSERT_FLOAT(cl.x, 0.0f, "V4 clear x");
	MET_ASSERT_FLOAT(cl.y, 0.0f, "V4 clear y");
	MET_ASSERT_FLOAT(cl.z, 0.0f, "V4 clear z");
	MET_ASSERT_FLOAT(cl.w, 0.0f, "V4 clear w");

	// chained ops
	Vector4 ch(2.0f, 2.0f, 2.0f, 2.0f);
	Vector4 r = (ch + Vector4(1.0f, 1.0f, 1.0f, 1.0f)) - Vector4(0.5f, 0.5f, 0.5f, 0.5f);
	MET_ASSERT_FLOAT(r.x, 2.5f, "V4 chain x");
	MET_ASSERT_FLOAT(r.y, 2.5f, "V4 chain y");
	MET_ASSERT_FLOAT(r.z, 2.5f, "V4 chain z");
	MET_ASSERT_FLOAT(r.w, 2.5f, "V4 chain w");

	// large values
	Vector4 big(1e5f, 2e5f, 3e5f, 4e5f);
	Vector4 bigr = big + Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	MET_ASSERT_FLOAT(bigr.x, 100001.0f, "V4 big x");
	MET_ASSERT_FLOAT(bigr.w, 400001.0f, "V4 big w");

	// small fractional
	Vector4 frac(0.001f, 0.002f, 0.003f, 0.004f);
	Vector4 fracr = frac + frac;
	MET_ASSERT_FLOAT(fracr.x, 0.002f, "V4 frac x");
	MET_ASSERT_FLOAT(fracr.w, 0.008f, "V4 frac w");
}

//------------------------------------------------------------------------------
// Vector4Int extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector4Int_Extended()
{
	// default
	Vector4Int def;
	MET_ASSERT_EQ_INT(def.x, 0, "V4I-ext def x");
	MET_ASSERT_EQ_INT(def.y, 0, "V4I-ext def y");
	MET_ASSERT_EQ_INT(def.z, 0, "V4I-ext def z");
	MET_ASSERT_EQ_INT(def.w, 0, "V4I-ext def w");

	// equality/inequality
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c(1, 2, 3, 5);
	MATH_ET_ASSERT(a == b, "V4I-ext eq");
	MATH_ET_ASSERT(a != c, "V4I-ext neq");

	// unary minus
	Vector4Int neg = -a;
	MET_ASSERT_EQ_INT(neg.x, -1, "V4I-ext neg x");
	MET_ASSERT_EQ_INT(neg.y, -2, "V4I-ext neg y");
	MET_ASSERT_EQ_INT(neg.z, -3, "V4I-ext neg z");
	MET_ASSERT_EQ_INT(neg.w, -4, "V4I-ext neg w");

	// less-than (sum)
	Vector4Int s1(1, 1, 1, 1);
	Vector4Int s2(2, 2, 2, 2);
	MATH_ET_ASSERT(s1 < s2, "V4I-ext s1 < s2");
	MATH_ET_ASSERT(!(s2 < s1), "V4I-ext not s2 < s1");

	// scale
	Vector4Int sc(2, 3, 4, 5);
	Vector4Int scaled = sc * 3;
	MET_ASSERT_EQ_INT(scaled.x, 6, "V4I-ext scale x");
	MET_ASSERT_EQ_INT(scaled.y, 9, "V4I-ext scale y");
	MET_ASSERT_EQ_INT(scaled.z, 12, "V4I-ext scale z");
	MET_ASSERT_EQ_INT(scaled.w, 15, "V4I-ext scale w");

	// large positive
	Vector4Int big(100000, 200000, 300000, 400000);
	Vector4Int bigr = big + Vector4Int(1, 2, 3, 4);
	MET_ASSERT_EQ_INT(bigr.x, 100001, "V4I-ext big x");
	MET_ASSERT_EQ_INT(bigr.w, 400004, "V4I-ext big w");

	// negative add/sub
	Vector4Int nv(-10, -20, -30, -40);
	Vector4Int pv(10, 20, 30, 40);
	Vector4Int zero = nv + pv;
	MET_ASSERT_EQ_INT(zero.x, 0, "V4I-ext neg+pos x");
	MET_ASSERT_EQ_INT(zero.y, 0, "V4I-ext neg+pos y");
	MET_ASSERT_EQ_INT(zero.z, 0, "V4I-ext neg+pos z");
	MET_ASSERT_EQ_INT(zero.w, 0, "V4I-ext neg+pos w");

	// chained ops
	Vector4Int ch(5, 5, 5, 5);
	Vector4Int r = (ch + Vector4Int(1, 2, 3, 4)) - Vector4Int(2, 3, 4, 5);
	MET_ASSERT_EQ_INT(r.x, 4, "V4I-ext chain x");
	MET_ASSERT_EQ_INT(r.y, 4, "V4I-ext chain y");
	MET_ASSERT_EQ_INT(r.z, 4, "V4I-ext chain z");
	MET_ASSERT_EQ_INT(r.w, 4, "V4I-ext chain w");

	// multiple clear cycles
	Vector4Int mc(1, 2, 3, 4);
	FOR(200)
	{
		mc.clear();
		MET_ASSERT_EQ_INT(mc.x, 0, "V4I-ext multiclear x");
		mc = Vector4Int(i, i+1, i+2, i+3);
	}
}

//------------------------------------------------------------------------------
// Vector2UInt extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector2UInt_Extended()
{
	// default
	Vector2UInt def;
	MET_ASSERT_EQ_UINT(def.x, 0, "V2UI-ext def x");
	MET_ASSERT_EQ_UINT(def.y, 0, "V2UI-ext def y");

	// parameterized
	Vector2UInt v(100u, 200u);
	MET_ASSERT_EQ_UINT(v.x, 100u, "V2UI-ext ctor x");
	MET_ASSERT_EQ_UINT(v.y, 200u, "V2UI-ext ctor y");

	// add
	Vector2UInt a(10u, 20u), b(5u, 8u);
	Vector2UInt add = a + b;
	MET_ASSERT_EQ_UINT(add.x, 15u, "V2UI-ext add x");
	MET_ASSERT_EQ_UINT(add.y, 28u, "V2UI-ext add y");

	// sub
	Vector2UInt sub = a - b;
	MET_ASSERT_EQ_UINT(sub.x, 5u, "V2UI-ext sub x");
	MET_ASSERT_EQ_UINT(sub.y, 12u, "V2UI-ext sub y");

	// scale
	Vector2UInt sc(3u, 4u);
	Vector2UInt scaled = sc * 6u;
	MET_ASSERT_EQ_UINT(scaled.x, 18u, "V2UI-ext scale x");
	MET_ASSERT_EQ_UINT(scaled.y, 24u, "V2UI-ext scale y");

	// +=
	Vector2UInt acc(1u, 2u);
	acc += Vector2UInt(9u, 8u);
	MET_ASSERT_EQ_UINT(acc.x, 10u, "V2UI-ext += x");
	MET_ASSERT_EQ_UINT(acc.y, 10u, "V2UI-ext += y");

	// -=
	acc -= Vector2UInt(5u, 5u);
	MET_ASSERT_EQ_UINT(acc.x, 5u, "V2UI-ext -= x");
	MET_ASSERT_EQ_UINT(acc.y, 5u, "V2UI-ext -= y");

	// equality
	Vector2UInt e1(10u, 20u), e2(10u, 20u), e3(10u, 21u);
	MATH_ET_ASSERT(e1 == e2, "V2UI-ext eq");
	MATH_ET_ASSERT(e1 != e3, "V2UI-ext neq");

	// clear
	Vector2UInt cl(99u, 88u);
	cl.clear();
	MET_ASSERT_EQ_UINT(cl.x, 0u, "V2UI-ext clear x");
	MET_ASSERT_EQ_UINT(cl.y, 0u, "V2UI-ext clear y");

	// large values
	Vector2UInt big(1000000u, 2000000u);
	Vector2UInt bigr = big + Vector2UInt(1u, 2u);
	MET_ASSERT_EQ_UINT(bigr.x, 1000001u, "V2UI-ext big x");
	MET_ASSERT_EQ_UINT(bigr.y, 2000002u, "V2UI-ext big y");
}

//------------------------------------------------------------------------------
// Vector2UShort extended tests
//------------------------------------------------------------------------------
void MathStructTest::testVector2UShort_Extended()
{
	// default
	Vector2UShort def;
	MET_ASSERT_EQ_UINT(def.x, 0u, "V2US-ext def x");
	MET_ASSERT_EQ_UINT(def.y, 0u, "V2US-ext def y");

	// parameterized
	Vector2UShort v((ushort)100, (ushort)200);
	MET_ASSERT_EQ_UINT(v.x, 100u, "V2US-ext ctor x");
	MET_ASSERT_EQ_UINT(v.y, 200u, "V2US-ext ctor y");

	// add
	Vector2UShort a((ushort)50, (ushort)60);
	Vector2UShort b((ushort)10, (ushort)20);
	Vector2UShort add = a + b;
	MET_ASSERT_EQ_UINT(add.x, 60u, "V2US-ext add x");
	MET_ASSERT_EQ_UINT(add.y, 80u, "V2US-ext add y");

	// sub
	Vector2UShort sub = a - b;
	MET_ASSERT_EQ_UINT(sub.x, 40u, "V2US-ext sub x");
	MET_ASSERT_EQ_UINT(sub.y, 40u, "V2US-ext sub y");

	// scale
	Vector2UShort sc((ushort)5, (ushort)7);
	Vector2UShort scaled = sc * (ushort)3;
	MET_ASSERT_EQ_UINT(scaled.x, 15u, "V2US-ext scale x");
	MET_ASSERT_EQ_UINT(scaled.y, 21u, "V2US-ext scale y");

	// +=
	Vector2UShort acc((ushort)1, (ushort)2);
	acc += Vector2UShort((ushort)9, (ushort)8);
	MET_ASSERT_EQ_UINT(acc.x, 10u, "V2US-ext += x");
	MET_ASSERT_EQ_UINT(acc.y, 10u, "V2US-ext += y");

	// -=
	acc -= Vector2UShort((ushort)5, (ushort)5);
	MET_ASSERT_EQ_UINT(acc.x, 5u, "V2US-ext -= x");
	MET_ASSERT_EQ_UINT(acc.y, 5u, "V2US-ext -= y");

	// equality
	Vector2UShort e1((ushort)10, (ushort)20);
	Vector2UShort e2((ushort)10, (ushort)20);
	Vector2UShort e3((ushort)10, (ushort)21);
	MATH_ET_ASSERT(e1 == e2, "V2US-ext eq");
	MATH_ET_ASSERT(e1 != e3, "V2US-ext neq");

	// clear
	Vector2UShort cl((ushort)99, (ushort)88);
	cl.clear();
	MET_ASSERT_EQ_UINT(cl.x, 0u, "V2US-ext clear x");
	MET_ASSERT_EQ_UINT(cl.y, 0u, "V2US-ext clear y");

	// max ushort value
	Vector2UShort maxv((ushort)65535, (ushort)65535);
	MET_ASSERT_EQ_UINT(maxv.x, 65535u, "V2US-ext maxval x");
	MET_ASSERT_EQ_UINT(maxv.y, 65535u, "V2US-ext maxval y");

	// less-than (sum)
	Vector2UShort s1((ushort)1, (ushort)1);
	Vector2UShort s2((ushort)5, (ushort)5);
	MATH_ET_ASSERT(s1 < s2, "V2US-ext s1 < s2");
	MATH_ET_ASSERT(!(s2 < s1), "V2US-ext not s2 < s1");
}

//------------------------------------------------------------------------------
// MathTest entry point
//------------------------------------------------------------------------------
void MathStructTest::testExtended()
{
	testVector2Short();
	testVector2UInt();
	testVector2UShort();
	testVector3Int();
	testVector4();
	testVector4Int();
	testVector2Short_Extended();
	testVector3Int_Extended();
	testVector4_Extended();
	testVector4Int_Extended();
	testVector2UInt_Extended();
	testVector2UShort_Extended();
}

// ============================================================
// Consolidated ColorQuaternionTest
// ============================================================
// ============================================================
// ColorQuaternionTest
// Testing Color struct and Quaternion struct
// Color: RGBA byte struct with arithmetic operators
// Quaternion: float quaternion with rotation math
// ============================================================

#define CQT_ASSERT(expr) \
	if (!(expr)) { ERROR(string("MathStructTest testColorQuaternion FAIL: ") + #expr); }

#define CQT_FLOAT_EQ(a, b) (fabsf((a) - (b)) <= 1e-4f)

void MathStructTest::testColorQuaternion()
{
	// Color
	testColorDefault();
	testColorConstruct();
	testColorAdd();
	testColorSub();
	testColorAddAssign();
	testColorSubAssign();
	testColorEqual();
	testColorNotEqual();
	testColorStaticConstants();
	testColorChain();
	testColorBoundary();
	testColorInVector();
	testColorWrap();
	testColorAlpha();
	testColorGrayscale();
	// Quaternion
	testQuaternionDefault();
	testQuaternionConstruct();
	testQuaternionClear();
	testQuaternionConjugate();
	testQuaternionLength();
	testQuaternionNormalize();
	testQuaternionMulScalar();
	testQuaternionDivScalar();
	testQuaternionAdd();
	testQuaternionNeg();
	testQuaternionMulAssign();
	testQuaternionDivAssign();
	testQuaternionAddAssign();
	testQuaternionDot();
	testQuaternionLerp();
	testQuaternionFromAxisAngle();
	testQuaternionEulerAngles();
	testQuaternionMul();
	testQuaternionCross();
	testQuaternionIdentity();
	testQuaternionStressNormalize();
}

// ============================================================
// Color tests
// ============================================================

void MathStructTest::testColorDefault()
{
	Color c;
	CQT_ASSERT(c.r == 0);
	CQT_ASSERT(c.g == 0);
	CQT_ASSERT(c.b == 0);
	CQT_ASSERT(c.a == 0);
}

void MathStructTest::testColorConstruct()
{
	Color c((byte)255, (byte)128, (byte)64, (byte)32);
	CQT_ASSERT(c.r == 255);
	CQT_ASSERT(c.g == 128);
	CQT_ASSERT(c.b == 64);
	CQT_ASSERT(c.a == 32);
	Color c2((byte)0, (byte)0, (byte)0, (byte)255);
	CQT_ASSERT(c2.r == 0);
	CQT_ASSERT(c2.g == 0);
	CQT_ASSERT(c2.b == 0);
	CQT_ASSERT(c2.a == 255);
}

void MathStructTest::testColorAdd()
{
	Color a((byte)10, (byte)20, (byte)30, (byte)40);
	Color b((byte)5, (byte)10, (byte)15, (byte)20);
	Color c = a + b;
	CQT_ASSERT(c.r == 15);
	CQT_ASSERT(c.g == 30);
	CQT_ASSERT(c.b == 45);
	CQT_ASSERT(c.a == 60);
	Color d((byte)0, (byte)0, (byte)0, (byte)0);
	Color e((byte)100, (byte)200, (byte)50, (byte)150);
	Color f = d + e;
	CQT_ASSERT(f.r == 100);
	CQT_ASSERT(f.g == 200);
	CQT_ASSERT(f.b == 50);
	CQT_ASSERT(f.a == 150);
}

void MathStructTest::testColorSub()
{
	Color a((byte)100, (byte)200, (byte)150, (byte)100);
	Color b((byte)50, (byte)100, (byte)75, (byte)50);
	Color c = a - b;
	CQT_ASSERT(c.r == 50);
	CQT_ASSERT(c.g == 100);
	CQT_ASSERT(c.b == 75);
	CQT_ASSERT(c.a == 50);
	Color d((byte)255, (byte)255, (byte)255, (byte)255);
	Color e((byte)255, (byte)255, (byte)255, (byte)255);
	Color f = d - e;
	CQT_ASSERT(f.r == 0);
	CQT_ASSERT(f.g == 0);
	CQT_ASSERT(f.b == 0);
	CQT_ASSERT(f.a == 0);
}

void MathStructTest::testColorAddAssign()
{
	Color c((byte)10, (byte)20, (byte)30, (byte)40);
	c += Color((byte)5, (byte)10, (byte)15, (byte)20);
	CQT_ASSERT(c.r == 15);
	CQT_ASSERT(c.g == 30);
	CQT_ASSERT(c.b == 45);
	CQT_ASSERT(c.a == 60);
	c += Color((byte)0, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(c.r == 15);
	CQT_ASSERT(c.g == 30);
	CQT_ASSERT(c.b == 45);
	CQT_ASSERT(c.a == 60);
}

void MathStructTest::testColorSubAssign()
{
	Color c((byte)100, (byte)80, (byte)60, (byte)40);
	c -= Color((byte)50, (byte)40, (byte)30, (byte)20);
	CQT_ASSERT(c.r == 50);
	CQT_ASSERT(c.g == 40);
	CQT_ASSERT(c.b == 30);
	CQT_ASSERT(c.a == 20);
	c -= Color((byte)50, (byte)40, (byte)30, (byte)20);
	CQT_ASSERT(c.r == 0);
	CQT_ASSERT(c.g == 0);
	CQT_ASSERT(c.b == 0);
	CQT_ASSERT(c.a == 0);
}

void MathStructTest::testColorEqual()
{
	Color a((byte)255, (byte)128, (byte)64, (byte)32);
	Color b((byte)255, (byte)128, (byte)64, (byte)32);
	Color c((byte)255, (byte)128, (byte)64, (byte)33);
	CQT_ASSERT(a == b);
	CQT_ASSERT(!(a == c));
	Color d((byte)0, (byte)0, (byte)0, (byte)0);
	Color e((byte)0, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(d == e);
}

void MathStructTest::testColorNotEqual()
{
	Color a((byte)100, (byte)200, (byte)150, (byte)255);
	Color b((byte)100, (byte)200, (byte)151, (byte)255);
	Color c((byte)100, (byte)200, (byte)150, (byte)255);
	CQT_ASSERT(a != b);
	CQT_ASSERT(!(a != c));
}

void MathStructTest::testColorStaticConstants()
{
	CQT_ASSERT(Color::WHITE.r == 255 && Color::WHITE.g == 255 && Color::WHITE.b == 255 && Color::WHITE.a == 255);
	CQT_ASSERT(Color::BLACK.r == 0 && Color::BLACK.g == 0 && Color::BLACK.b == 0 && Color::BLACK.a == 255);
	CQT_ASSERT(Color::RED.r == 255 && Color::RED.g == 0 && Color::RED.b == 0 && Color::RED.a == 255);
	CQT_ASSERT(Color::GREEN.r == 0 && Color::GREEN.g == 255 && Color::GREEN.b == 0 && Color::GREEN.a == 255);
	CQT_ASSERT(Color::BLUE.r == 0 && Color::BLUE.g == 0 && Color::BLUE.b == 255 && Color::BLUE.a == 255);
}

void MathStructTest::testColorChain()
{
	Color a((byte)10, (byte)20, (byte)30, (byte)40);
	Color b((byte)5, (byte)10, (byte)5, (byte)10);
	Color c = (a + b) - Color((byte)2, (byte)5, (byte)10, (byte)15);
	// (15, 30, 35, 50) - (2, 5, 10, 15) = (13, 25, 25, 35)
	CQT_ASSERT(c.r == 13);
	CQT_ASSERT(c.g == 25);
	CQT_ASSERT(c.b == 25);
	CQT_ASSERT(c.a == 35);
}

void MathStructTest::testColorBoundary()
{
	Color max((byte)255, (byte)255, (byte)255, (byte)255);
	Color min((byte)0, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(max.r == 255 && max.a == 255);
	CQT_ASSERT(min.r == 0 && min.a == 0);
	// Byte wraps at 255+1=0
	Color c((byte)255, (byte)0, (byte)0, (byte)0);
	Color d((byte)1, (byte)0, (byte)0, (byte)0);
	Color e = c + d;
	CQT_ASSERT(e.r == 0); // byte overflow: 256 & 0xFF == 0
}

void MathStructTest::testColorInVector()
{
	Vector<Color> colors;
	colors.add(Color::RED);
	colors.add(Color::GREEN);
	colors.add(Color::BLUE);
	colors.add(Color::WHITE);
	colors.add(Color::BLACK);
	CQT_ASSERT(colors.size() == 5);
	CQT_ASSERT(colors[0] == Color::RED);
	CQT_ASSERT(colors[1] == Color::GREEN);
	CQT_ASSERT(colors[2] == Color::BLUE);
	CQT_ASSERT(colors[3] == Color::WHITE);
	CQT_ASSERT(colors[4] == Color::BLACK);
}

void MathStructTest::testColorWrap()
{
	// Byte arithmetic: (255 + 2) wraps to 1
	Color c((byte)254, (byte)0, (byte)0, (byte)0);
	c += Color((byte)2, (byte)0, (byte)0, (byte)0);
	CQT_ASSERT(c.r == 0); // 256 mod 256 == 0
}

void MathStructTest::testColorAlpha()
{
	Color transparent((byte)255, (byte)0, (byte)128, (byte)0);
	Color opaque((byte)255, (byte)0, (byte)128, (byte)255);
	CQT_ASSERT(transparent.a == 0);
	CQT_ASSERT(opaque.a == 255);
	Color halfAlpha((byte)200, (byte)100, (byte)50, (byte)128);
	CQT_ASSERT(halfAlpha.a == 128);
}

void MathStructTest::testColorGrayscale()
{
	Color gray((byte)128, (byte)128, (byte)128, (byte)255);
	CQT_ASSERT(gray.r == gray.g && gray.g == gray.b);
	Color white((byte)255, (byte)255, (byte)255, (byte)255);
	Color black((byte)0, (byte)0, (byte)0, (byte)255);
	Color mixed = white + black;
	CQT_ASSERT(mixed.r == 255);
	CQT_ASSERT(mixed.g == 255);
	CQT_ASSERT(mixed.b == 255);
}

// ============================================================
// Quaternion tests
// ============================================================

void MathStructTest::testQuaternionDefault()
{
	Quaternion q;
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 1.0f));
}

void MathStructTest::testQuaternionConstruct()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 4.0f));
	Quaternion q2(q);
	CQT_ASSERT(CQT_FLOAT_EQ(q2.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q2.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q2.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q2.w, 4.0f));
}

void MathStructTest::testQuaternionClear()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 0.5f);
	q.clear();
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 1.0f));
}

void MathStructTest::testQuaternionConjugate()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion c = q.conjugate();
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, -1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.y, -2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.z, -3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 4.0f));
	// Double conjugate == original
	Quaternion cc = c.conjugate();
	CQT_ASSERT(CQT_FLOAT_EQ(cc.x, q.x));
	CQT_ASSERT(CQT_FLOAT_EQ(cc.y, q.y));
	CQT_ASSERT(CQT_FLOAT_EQ(cc.z, q.z));
	CQT_ASSERT(CQT_FLOAT_EQ(cc.w, q.w));
}

void MathStructTest::testQuaternionLength()
{
	Quaternion q(0.0f, 0.0f, 0.0f, 1.0f);
	float len = q.length();
	CQT_ASSERT(CQT_FLOAT_EQ(len, 1.0f));
	Quaternion q2(3.0f, 0.0f, 0.0f, 4.0f);
	float len2 = q2.length();
	CQT_ASSERT(CQT_FLOAT_EQ(len2, 5.0f));
}

void MathStructTest::testQuaternionNormalize()
{
	Quaternion q(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion n = q.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n.length(), 1.0f));
	Quaternion q2(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion n2 = q2.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n2.length(), 1.0f));
	Quaternion q3(1.0f, 1.0f, 1.0f, 1.0f);
	Quaternion n3 = q3.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n3.length(), 1.0f));
}

void MathStructTest::testQuaternionMulScalar()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion r = q * 2.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(r.x, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.y, 4.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.z, 6.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.w, 8.0f));
	Quaternion r2 = q * 0.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(r2.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r2.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r2.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r2.w, 0.0f));
}

void MathStructTest::testQuaternionDivScalar()
{
	Quaternion q(4.0f, 8.0f, 12.0f, 16.0f);
	Quaternion r = q / 4.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(r.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(r.w, 4.0f));
}

void MathStructTest::testQuaternionAdd()
{
	Quaternion a(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion b(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion c = a + b;
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, 1.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.y, 2.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.z, 3.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 4.5f));
}

void MathStructTest::testQuaternionNeg()
{
	Quaternion q(1.0f, -2.0f, 3.0f, -4.0f);
	Quaternion neg = -q;
	CQT_ASSERT(CQT_FLOAT_EQ(neg.x, -1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(neg.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(neg.z, -3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(neg.w, 4.0f));
}

void MathStructTest::testQuaternionMulAssign()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q *= 3.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 6.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 9.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 12.0f));
}

void MathStructTest::testQuaternionDivAssign()
{
	Quaternion q(2.0f, 4.0f, 6.0f, 8.0f);
	q /= 2.0f;
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 2.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 3.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 4.0f));
}

void MathStructTest::testQuaternionAddAssign()
{
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	q += Quaternion(0.5f, 0.5f, 0.5f, 0.5f);
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 1.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 2.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 3.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 4.5f));
}

void MathStructTest::testQuaternionDot()
{
	Quaternion a(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion b(1.0f, 0.0f, 0.0f, 0.0f);
	float d = Quaternion::dot(a, b);
	CQT_ASSERT(CQT_FLOAT_EQ(d, 1.0f));
	Quaternion c(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion e(4.0f, 3.0f, 2.0f, 1.0f);
	float d2 = Quaternion::dot(c, e);
	// 1*4 + 2*3 + 3*2 + 4*1 = 4+6+6+4 = 20
	CQT_ASSERT(CQT_FLOAT_EQ(d2, 20.0f));
}

void MathStructTest::testQuaternionLerp()
{
	Quaternion q0(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q1(1.0f, 0.0f, 0.0f, 0.0f);
	Quaternion lerp0 = Quaternion::lerp(q0, q1, 0.0f);
	CQT_ASSERT(CQT_FLOAT_EQ(lerp0.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(lerp0.w, 1.0f));
	Quaternion lerp1 = Quaternion::lerp(q0, q1, 1.0f);
	CQT_ASSERT(CQT_FLOAT_EQ(lerp1.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(lerp1.w, 0.0f));
	Quaternion lerp05 = Quaternion::lerp(q0, q1, 0.5f);
	CQT_ASSERT(CQT_FLOAT_EQ(lerp05.x, 0.5f));
	CQT_ASSERT(CQT_FLOAT_EQ(lerp05.w, 0.5f));
}

void MathStructTest::testQuaternionFromAxisAngle()
{
	// Rotation of 0 around any axis = identity-like
	Vector3 axis(0.0f, 1.0f, 0.0f);
	Quaternion q(axis, 0.0f);
	// For angle=0: sin(0)=0, cos(0)=1; x=y=z=0, w=1
	CQT_ASSERT(CQT_FLOAT_EQ(q.w, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(q.z, 0.0f));
	// Rotation PI/2 around Y axis
	float halfAngle = 3.14159265f / 4.0f; // half of PI/2
	Quaternion qY(axis, 3.14159265f / 2.0f);
	float expectedW = cosf(halfAngle);
	float expectedY = sinf(halfAngle);
	CQT_ASSERT(CQT_FLOAT_EQ(qY.w, expectedW));
	CQT_ASSERT(CQT_FLOAT_EQ(qY.y, expectedY));
	CQT_ASSERT(CQT_FLOAT_EQ(qY.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(qY.z, 0.0f));
}

void MathStructTest::testQuaternionEulerAngles()
{
	// Identity quaternion should give zero euler angles
	Quaternion identity(0.0f, 0.0f, 0.0f, 1.0f);
	Vector3 euler = identity.eulerAngles();
	CQT_ASSERT(CQT_FLOAT_EQ(euler.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(euler.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(euler.z, 0.0f));
}

void MathStructTest::testQuaternionMul()
{
	// Identity * anything = anything
	Quaternion identity(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion q(0.5f, 0.5f, 0.5f, 0.5f);
	Quaternion result = identity * q;
	CQT_ASSERT(CQT_FLOAT_EQ(result.x, q.x));
	CQT_ASSERT(CQT_FLOAT_EQ(result.y, q.y));
	CQT_ASSERT(CQT_FLOAT_EQ(result.z, q.z));
	CQT_ASSERT(CQT_FLOAT_EQ(result.w, q.w));
	// q * identity = q
	Quaternion result2 = q * identity;
	CQT_ASSERT(CQT_FLOAT_EQ(result2.x, q.x));
	CQT_ASSERT(CQT_FLOAT_EQ(result2.y, q.y));
	CQT_ASSERT(CQT_FLOAT_EQ(result2.z, q.z));
	CQT_ASSERT(CQT_FLOAT_EQ(result2.w, q.w));
}

void MathStructTest::testQuaternionCross()
{
	Quaternion a(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion b(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion c = Quaternion::cross(a, b);
	// cross(q1,q2) = {q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z,
	//                 q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
	//                 q1.w*q2.y + q1.y*q2.w + q1.z*q2.x - q1.x*q2.z,
	//                 q1.w*q2.z + q1.z*q2.w + q1.x*q2.y - q1.y*q2.x }
	// 初始化列表顺序对应 x,y,z,w
	// 对于 a=b=(x=0,y=0,z=0,w=1)：
	//   x = 1*1-0-0-0 = 1
	//   y = 0, z = 0, w = 0
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 0.0f));
}

void MathStructTest::testQuaternionIdentity()
{
	Quaternion identity;
	CQT_ASSERT(CQT_FLOAT_EQ(identity.length(), 1.0f));
	// Normalizing identity should still be identity
	Quaternion n = identity.normalize();
	CQT_ASSERT(CQT_FLOAT_EQ(n.x, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(n.y, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(n.z, 0.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(n.w, 1.0f));
	// Conjugate of identity = identity
	Quaternion c = identity.conjugate();
	CQT_ASSERT(CQT_FLOAT_EQ(c.w, 1.0f));
	CQT_ASSERT(CQT_FLOAT_EQ(c.x, 0.0f));
}

void MathStructTest::testQuaternionStressNormalize()
{
	// Normalize 20 random-ish quaternions and verify unit length
	for (int i = 1; i <= 20; ++i)
	{
		float fi = (float)i;
		Quaternion q(fi * 0.1f, fi * 0.2f, fi * 0.3f, fi * 0.5f);
		Quaternion n = q.normalize();
		float len = n.length();
		CQT_ASSERT(CQT_FLOAT_EQ(len, 1.0f));
	}
	// Conjugate of normalized = normalized inverse
	Quaternion q(1.0f, 2.0f, 3.0f, 4.0f);
	Quaternion n = q.normalize();
	Quaternion conj = n.conjugate();
	// For unit quaternion, dot(q, conj(q)) = x*(-x)+y*(-y)+z*(-z)+w*w = -(x^2+y^2+z^2)+w^2
	// We just verify length stays 1
	CQT_ASSERT(CQT_FLOAT_EQ(conj.length(), 1.0f));
}

// ============================================================
// Consolidated Vector2Test
// ============================================================
#define V2ET_ASSERT(condition) if (!(condition)) { ERROR("Vector2Test failed: " #condition); }
#define V2ET_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

void MathStructTest::testVector2Family()
{
    testVector2DefaultConstruct();
    testVector2FloatConstruct();
    testVector2IntConstruct();
    testVector2Clear();
    testVector2Arithmetic();
    testVector2AssignOps();
    testVector2Negate();
    testVector2Equality();
    testVector2StaticConstants();
    testVector2IntDefaultConstruct();
    testVector2IntArithmetic();
    testVector2IntAssignOps();
    testVector2IntNegate();
    testVector2IntEquality();
    testVector2IntLess();
    testVector2IntClear();
    testVector2ShortDefaultConstruct();
    testVector2ShortArithmetic();
    testVector2ShortEquality();
    testVector2UShortBasic();
    testVector2UIntBasic();
}

void MathStructTest::testVector2DefaultConstruct()
{
    Vector2 v;
    V2ET_ASSERT(V2ET_NEAR(v.x, 0.0f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 0.0f));
}

void MathStructTest::testVector2FloatConstruct()
{
    Vector2 v(3.5f, 7.5f);
    V2ET_ASSERT(V2ET_NEAR(v.x, 3.5f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 7.5f));
}

void MathStructTest::testVector2IntConstruct()
{
    Vector2 v(3, 7);
    V2ET_ASSERT(V2ET_NEAR(v.x, 3.0f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 7.0f));
}

void MathStructTest::testVector2Clear()
{
    Vector2 v(5.0f, 10.0f);
    v.clear();
    V2ET_ASSERT(V2ET_NEAR(v.x, 0.0f));
    V2ET_ASSERT(V2ET_NEAR(v.y, 0.0f));
}

void MathStructTest::testVector2Arithmetic()
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);

    Vector2 sum = a + b;
    V2ET_ASSERT(V2ET_NEAR(sum.x, 4.0f));
    V2ET_ASSERT(V2ET_NEAR(sum.y, 6.0f));

    Vector2 diff = b - a;
    V2ET_ASSERT(V2ET_NEAR(diff.x, 2.0f));
    V2ET_ASSERT(V2ET_NEAR(diff.y, 2.0f));

    Vector2 scaled = a * 3.0f;
    V2ET_ASSERT(V2ET_NEAR(scaled.x, 3.0f));
    V2ET_ASSERT(V2ET_NEAR(scaled.y, 6.0f));

    Vector2 divided = b / 2.0f;
    V2ET_ASSERT(V2ET_NEAR(divided.x, 1.5f));
    V2ET_ASSERT(V2ET_NEAR(divided.y, 2.0f));
}

void MathStructTest::testVector2AssignOps()
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(3.0f, 4.0f);

    a += b;
    V2ET_ASSERT(V2ET_NEAR(a.x, 4.0f));
    V2ET_ASSERT(V2ET_NEAR(a.y, 6.0f));

    a -= b;
    V2ET_ASSERT(V2ET_NEAR(a.x, 1.0f));
    V2ET_ASSERT(V2ET_NEAR(a.y, 2.0f));
}

void MathStructTest::testVector2Negate()
{
    Vector2 a(1.0f, -2.0f);
    Vector2 b = -a;
    V2ET_ASSERT(V2ET_NEAR(b.x, -1.0f));
    V2ET_ASSERT(V2ET_NEAR(b.y, 2.0f));
}

void MathStructTest::testVector2Equality()
{
    Vector2 a(1.0f, 2.0f);
    Vector2 b(1.0f, 2.0f);
    Vector2 c(1.0f, 3.0f);
    V2ET_ASSERT(a == b);
    V2ET_ASSERT(!(a == c));
    V2ET_ASSERT(a != c);
    V2ET_ASSERT(!(a != b));
}

void MathStructTest::testVector2StaticConstants()
{
    Vector2 zero = Vector2::ZERO;
    V2ET_ASSERT(V2ET_NEAR(zero.x, 0.0f));
    V2ET_ASSERT(V2ET_NEAR(zero.y, 0.0f));

    Vector2 one = Vector2::ONE;
    V2ET_ASSERT(V2ET_NEAR(one.x, 1.0f));
    V2ET_ASSERT(V2ET_NEAR(one.y, 1.0f));

    Vector2 up = Vector2::UP;
    V2ET_ASSERT(V2ET_NEAR(up.y, 1.0f));

    Vector2 down = Vector2::DOWN;
    V2ET_ASSERT(V2ET_NEAR(down.y, -1.0f));

    Vector2 right = Vector2::RIGHT;
    V2ET_ASSERT(V2ET_NEAR(right.x, 1.0f));

    Vector2 left = Vector2::LEFT;
    V2ET_ASSERT(V2ET_NEAR(left.x, -1.0f));
}

void MathStructTest::testVector2IntDefaultConstruct()
{
    Vector2Int v;
    V2ET_ASSERT(v.x == 0);
    V2ET_ASSERT(v.y == 0);
}

void MathStructTest::testVector2IntArithmetic()
{
    Vector2Int a(1, 2);
    Vector2Int b(3, 4);

    Vector2Int sum = a + b;
    V2ET_ASSERT(sum.x == 4);
    V2ET_ASSERT(sum.y == 6);

    Vector2Int diff = b - a;
    V2ET_ASSERT(diff.x == 2);
    V2ET_ASSERT(diff.y == 2);

    Vector2Int scaled = a * 5;
    V2ET_ASSERT(scaled.x == 5);
    V2ET_ASSERT(scaled.y == 10);
}

void MathStructTest::testVector2IntAssignOps()
{
    Vector2Int a(2, 3);
    Vector2Int b(1, 1);
    a += b;
    V2ET_ASSERT(a.x == 3);
    V2ET_ASSERT(a.y == 4);
    a -= b;
    V2ET_ASSERT(a.x == 2);
    V2ET_ASSERT(a.y == 3);
}

void MathStructTest::testVector2IntNegate()
{
    Vector2Int a(3, -5);
    Vector2Int b = -a;
    V2ET_ASSERT(b.x == -3);
    V2ET_ASSERT(b.y == 5);
}

void MathStructTest::testVector2IntEquality()
{
    Vector2Int a(1, 2);
    Vector2Int b(1, 2);
    Vector2Int c(1, 3);
    V2ET_ASSERT(a == b);
    V2ET_ASSERT(!(a == c));
    V2ET_ASSERT(a != c);
    V2ET_ASSERT(!(a != b));
}

void MathStructTest::testVector2IntLess()
{
    Vector2Int a(1, 2);
    Vector2Int b(2, 3);
    V2ET_ASSERT(a < b);
    V2ET_ASSERT(!(b < a));
}

void MathStructTest::testVector2IntClear()
{
    Vector2Int v(5, 10);
    v.clear();
    V2ET_ASSERT(v.x == 0);
    V2ET_ASSERT(v.y == 0);
}

void MathStructTest::testVector2ShortDefaultConstruct()
{
    Vector2Short v;
    V2ET_ASSERT(v.x == 0);
    V2ET_ASSERT(v.y == 0);
}

void MathStructTest::testVector2ShortArithmetic()
{
    Vector2Short a((short)2, (short)3);
    Vector2Short b((short)4, (short)5);

    Vector2Short sum = a + b;
    V2ET_ASSERT(sum.x == 6);
    V2ET_ASSERT(sum.y == 8);

    Vector2Short diff = b - a;
    V2ET_ASSERT(diff.x == 2);
    V2ET_ASSERT(diff.y == 2);

    Vector2Short scaled = a * (short)3;
    V2ET_ASSERT(scaled.x == 6);
    V2ET_ASSERT(scaled.y == 9);

    Vector2Short neg = -a;
    V2ET_ASSERT(neg.x == -2);
    V2ET_ASSERT(neg.y == -3);
}

void MathStructTest::testVector2ShortEquality()
{
    Vector2Short a((short)1, (short)2);
    Vector2Short b((short)1, (short)2);
    Vector2Short c((short)1, (short)3);
    V2ET_ASSERT(a == b);
    V2ET_ASSERT(a != c);
}

void MathStructTest::testVector2UShortBasic()
{
    Vector2UShort a;
    V2ET_ASSERT(a.x == 0);
    V2ET_ASSERT(a.y == 0);

    Vector2UShort b((ushort)5, (ushort)10);
    V2ET_ASSERT(b.x == 5);
    V2ET_ASSERT(b.y == 10);

    b.clear();
    V2ET_ASSERT(b.x == 0);
    V2ET_ASSERT(b.y == 0);
}

void MathStructTest::testVector2UIntBasic()
{
    Vector2UInt a;
    V2ET_ASSERT(a.x == 0u);
    V2ET_ASSERT(a.y == 0u);

    Vector2UInt b(7u, 11u);
    V2ET_ASSERT(b.x == 7u);
    V2ET_ASSERT(b.y == 11u);

    b.clear();
    V2ET_ASSERT(b.x == 0u);
    V2ET_ASSERT(b.y == 0u);
}

// ============================================================
// Consolidated Vector3Test
// ============================================================
#define V3ET_ASSERT(condition) if (!(condition)) { ERROR("Vector3Test failed: " #condition); }
#define V3ET_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

void MathStructTest::testVector3Family()
{
    testDefaultConstruct();
    testFloatConstruct();
    testIntConstruct();
    testClear();
    testOperatorAdd();
    testOperatorSub();
    testOperatorMulScalar();
    testOperatorDivScalar();
    testOperatorAddAssign();
    testOperatorSubAssign();
    testOperatorNegate();
    testOperatorEqual();
    testOperatorNotEqual();
    testStaticZero();
    testStaticOne();
    testStaticForward();
    testStaticBack();
    testStaticLeft();
    testStaticRight();
    testStaticUp();
    testStaticDown();
    testChainOps();
    testFloatPrecision();
}

void MathStructTest::testDefaultConstruct()
{
    Vector3 v;
    V3ET_ASSERT(V3ET_NEAR(v.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 0.0f));
}

void MathStructTest::testFloatConstruct()
{
    Vector3 v(1.5f, 2.5f, 3.5f);
    V3ET_ASSERT(V3ET_NEAR(v.x, 1.5f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 2.5f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 3.5f));
}

void MathStructTest::testIntConstruct()
{
    Vector3 v(1, 2, 3);
    V3ET_ASSERT(V3ET_NEAR(v.x, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 3.0f));
}

void MathStructTest::testClear()
{
    Vector3 v(5.0f, 5.0f, 5.0f);
    v.clear();
    V3ET_ASSERT(V3ET_NEAR(v.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(v.z, 0.0f));
}

void MathStructTest::testOperatorAdd()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(4.0f, 5.0f, 6.0f);
    Vector3 c = a + b;
    V3ET_ASSERT(V3ET_NEAR(c.x, 5.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 7.0f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 9.0f));

    Vector3 d(0.0f, 0.0f, 0.0f);
    Vector3 e = a + d;
    V3ET_ASSERT(V3ET_NEAR(e.x, 1.0f));

    Vector3 neg(-1.0f, -2.0f, -3.0f);
    Vector3 f = a + neg;
    V3ET_ASSERT(V3ET_NEAR(f.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(f.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(f.z, 0.0f));
}

void MathStructTest::testOperatorSub()
{
    Vector3 a(5.0f, 7.0f, 9.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c = a - b;
    V3ET_ASSERT(V3ET_NEAR(c.x, 4.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 5.0f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 6.0f));

    Vector3 d = a - a;
    V3ET_ASSERT(V3ET_NEAR(d.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(d.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(d.z, 0.0f));
}

void MathStructTest::testOperatorMulScalar()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b = a * 2.0f;
    V3ET_ASSERT(V3ET_NEAR(b.x, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(b.y, 4.0f));
    V3ET_ASSERT(V3ET_NEAR(b.z, 6.0f));

    Vector3 c = a * 0.0f;
    V3ET_ASSERT(V3ET_NEAR(c.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 0.0f));

    Vector3 d = a * -1.0f;
    V3ET_ASSERT(V3ET_NEAR(d.x, -1.0f));
    V3ET_ASSERT(V3ET_NEAR(d.y, -2.0f));
    V3ET_ASSERT(V3ET_NEAR(d.z, -3.0f));
}

void MathStructTest::testOperatorDivScalar()
{
    Vector3 a(4.0f, 6.0f, 8.0f);
    Vector3 b = a / 2.0f;
    V3ET_ASSERT(V3ET_NEAR(b.x, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(b.y, 3.0f));
    V3ET_ASSERT(V3ET_NEAR(b.z, 4.0f));

    Vector3 c = a / 4.0f;
    V3ET_ASSERT(V3ET_NEAR(c.x, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 1.5f));
    V3ET_ASSERT(V3ET_NEAR(c.z, 2.0f));
}

void MathStructTest::testOperatorAddAssign()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(10.0f, 20.0f, 30.0f);
    a += b;
    V3ET_ASSERT(V3ET_NEAR(a.x, 11.0f));
    V3ET_ASSERT(V3ET_NEAR(a.y, 22.0f));
    V3ET_ASSERT(V3ET_NEAR(a.z, 33.0f));
}

void MathStructTest::testOperatorSubAssign()
{
    Vector3 a(10.0f, 20.0f, 30.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    a -= b;
    V3ET_ASSERT(V3ET_NEAR(a.x, 9.0f));
    V3ET_ASSERT(V3ET_NEAR(a.y, 18.0f));
    V3ET_ASSERT(V3ET_NEAR(a.z, 27.0f));
}

void MathStructTest::testOperatorNegate()
{
    Vector3 a(1.0f, -2.0f, 3.0f);
    Vector3 b = -a;
    V3ET_ASSERT(V3ET_NEAR(b.x, -1.0f));
    V3ET_ASSERT(V3ET_NEAR(b.y, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(b.z, -3.0f));

    Vector3 zero(0.0f, 0.0f, 0.0f);
    Vector3 negZero = -zero;
    V3ET_ASSERT(V3ET_NEAR(negZero.x, 0.0f));
}

void MathStructTest::testOperatorEqual()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 3.0f);
    Vector3 c(1.0f, 2.0f, 4.0f);
    V3ET_ASSERT(a == b);
    V3ET_ASSERT(!(a == c));
}

void MathStructTest::testOperatorNotEqual()
{
    Vector3 a(1.0f, 2.0f, 3.0f);
    Vector3 b(1.0f, 2.0f, 4.0f);
    Vector3 c(1.0f, 2.0f, 3.0f);
    V3ET_ASSERT(a != b);
    V3ET_ASSERT(!(a != c));
}

void MathStructTest::testStaticZero()
{
    Vector3 z = Vector3::ZERO;
    V3ET_ASSERT(V3ET_NEAR(z.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(z.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(z.z, 0.0f));
}

void MathStructTest::testStaticOne()
{
    Vector3 o = Vector3::ONE;
    V3ET_ASSERT(V3ET_NEAR(o.x, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(o.y, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(o.z, 1.0f));
}

void MathStructTest::testStaticForward()
{
    Vector3 f = Vector3::FORWARD;
    V3ET_ASSERT(V3ET_NEAR(f.z, 1.0f) || V3ET_NEAR(f.z, -1.0f));
}

void MathStructTest::testStaticBack()
{
    Vector3 b = Vector3::BACK;
    Vector3 f = Vector3::FORWARD;
    Vector3 sum = b + f;
    V3ET_ASSERT(V3ET_NEAR(sum.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.z, 0.0f));
}

void MathStructTest::testStaticLeft()
{
    Vector3 l = Vector3::LEFT;
    V3ET_ASSERT(V3ET_NEAR(l.x, -1.0f) || V3ET_NEAR(l.x, 1.0f));
}

void MathStructTest::testStaticRight()
{
    Vector3 r = Vector3::RIGHT;
    Vector3 l = Vector3::LEFT;
    Vector3 sum = r + l;
    V3ET_ASSERT(V3ET_NEAR(sum.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.y, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(sum.z, 0.0f));
}

void MathStructTest::testStaticUp()
{
    Vector3 u = Vector3::UP;
    V3ET_ASSERT(V3ET_NEAR(u.y, 1.0f));
    V3ET_ASSERT(V3ET_NEAR(u.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(u.z, 0.0f));
}

void MathStructTest::testStaticDown()
{
    Vector3 d = Vector3::DOWN;
    V3ET_ASSERT(V3ET_NEAR(d.y, -1.0f));
    V3ET_ASSERT(V3ET_NEAR(d.x, 0.0f));
    V3ET_ASSERT(V3ET_NEAR(d.z, 0.0f));
}

void MathStructTest::testChainOps()
{
    Vector3 a(1.0f, 0.0f, 0.0f);
    Vector3 b(0.0f, 1.0f, 0.0f);
    Vector3 c(0.0f, 0.0f, 1.0f);
    Vector3 result = (a + b + c) * 2.0f;
    V3ET_ASSERT(V3ET_NEAR(result.x, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(result.y, 2.0f));
    V3ET_ASSERT(V3ET_NEAR(result.z, 2.0f));
}

void MathStructTest::testFloatPrecision()
{
    Vector3 a(0.1f, 0.2f, 0.3f);
    Vector3 b(0.4f, 0.5f, 0.6f);
    Vector3 c = a + b;
    V3ET_ASSERT(V3ET_NEAR(c.x, 0.5f));
    V3ET_ASSERT(V3ET_NEAR(c.y, 0.7f));
    V3ET_ASSERT(fabsf(c.z - 0.9f) < 1e-3f);
}

// ============================================================
// Consolidated Vector4Test
// ============================================================
#define V4ET_ASSERT(expr) \
	if (!(expr)) { ERROR(string("MathStructTest testVector4Family FAILED: ") + (#expr)); }

#define V4ET_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

// ========== Vector3Int ==========

void MathStructTest::testV4ET_Vector3IntDefault()
{
	Vector3Int v;
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
}

void MathStructTest::testV4ET_Vector3IntConstruct()
{
	Vector3Int v(3, -5, 7);
	V4ET_ASSERT(v.x == 3);
	V4ET_ASSERT(v.y == -5);
	V4ET_ASSERT(v.z == 7);
}

void MathStructTest::testV4ET_Vector3IntClear()
{
	Vector3Int v(10, 20, 30);
	v.clear();
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
}

void MathStructTest::testV4ET_Vector3IntAdd()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(4, 5, 6);
	Vector3Int c = a + b;
	V4ET_ASSERT(c.x == 5);
	V4ET_ASSERT(c.y == 7);
	V4ET_ASSERT(c.z == 9);
}

void MathStructTest::testV4ET_Vector3IntSub()
{
	Vector3Int a(10, 8, 6);
	Vector3Int b(1, 3, 2);
	Vector3Int c = a - b;
	V4ET_ASSERT(c.x == 9);
	V4ET_ASSERT(c.y == 5);
	V4ET_ASSERT(c.z == 4);
}

void MathStructTest::testV4ET_Vector3IntMulScalar()
{
	Vector3Int v(2, -3, 4);
	Vector3Int r = v * 3;
	V4ET_ASSERT(r.x == 6);
	V4ET_ASSERT(r.y == -9);
	V4ET_ASSERT(r.z == 12);
}

void MathStructTest::testV4ET_Vector3IntAddAssign()
{
	Vector3Int v(1, 2, 3);
	Vector3Int d(10, 20, 30);
	v += d;
	V4ET_ASSERT(v.x == 11);
	V4ET_ASSERT(v.y == 22);
	V4ET_ASSERT(v.z == 33);
}

void MathStructTest::testV4ET_Vector3IntSubAssign()
{
	Vector3Int v(10, 20, 30);
	Vector3Int d(1, 2, 3);
	v -= d;
	V4ET_ASSERT(v.x == 9);
	V4ET_ASSERT(v.y == 18);
	V4ET_ASSERT(v.z == 27);
}

void MathStructTest::testV4ET_Vector3IntNegate()
{
	Vector3Int v(5, -3, 7);
	Vector3Int neg = -v;
	V4ET_ASSERT(neg.x == -5);
	V4ET_ASSERT(neg.y == 3);
	V4ET_ASSERT(neg.z == -7);
}

void MathStructTest::testV4ET_Vector3IntEqual()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(1, 2, 3);
	V4ET_ASSERT(a == b);
	Vector3Int c(1, 2, 4);
	V4ET_ASSERT(!(a == c));
}

void MathStructTest::testV4ET_Vector3IntNotEqual()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(1, 2, 4);
	V4ET_ASSERT(a != b);
	Vector3Int c(1, 2, 3);
	V4ET_ASSERT(!(a != c));
}

void MathStructTest::testV4ET_Vector3IntLess()
{
	// operator< compares sum
	Vector3Int a(1, 1, 1); // sum=3
	Vector3Int b(2, 2, 2); // sum=6
	V4ET_ASSERT(a < b);
	V4ET_ASSERT(!(b < a));
	Vector3Int c(1, 1, 1); // sum=3
	V4ET_ASSERT(!(a < c));
}

// ========== Vector4 ==========

void MathStructTest::testV4ET_Vector4Default()
{
	Vector4 v;
	V4ET_ASSERT(V4ET_NEAR(v.x, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 0.0f));
}

void MathStructTest::testV4ET_Vector4Construct()
{
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	V4ET_ASSERT(V4ET_NEAR(v.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 4.0f));
}

void MathStructTest::testV4ET_Vector4Clear()
{
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	v.clear();
	V4ET_ASSERT(V4ET_NEAR(v.x, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 0.0f));
}

void MathStructTest::testV4ET_Vector4Add()
{
	Vector4 a(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 b(0.5f, 1.5f, 2.5f, 3.5f);
	Vector4 c = a + b;
	V4ET_ASSERT(V4ET_NEAR(c.x, 1.5f));
	V4ET_ASSERT(V4ET_NEAR(c.y, 3.5f));
	V4ET_ASSERT(V4ET_NEAR(c.z, 5.5f));
	V4ET_ASSERT(V4ET_NEAR(c.w, 7.5f));
}

void MathStructTest::testV4ET_Vector4Sub()
{
	Vector4 a(5.0f, 4.0f, 3.0f, 2.0f);
	Vector4 b(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 c = a - b;
	V4ET_ASSERT(V4ET_NEAR(c.x, 4.0f));
	V4ET_ASSERT(V4ET_NEAR(c.y, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(c.z, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(c.w, 1.0f));
}

void MathStructTest::testV4ET_Vector4AddAssign()
{
	Vector4 v(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 d(0.1f, 0.2f, 0.3f, 0.4f);
	v += d;
	V4ET_ASSERT(V4ET_NEAR(v.x, 1.1f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 2.2f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 3.3f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 4.4f));
}

void MathStructTest::testV4ET_Vector4SubAssign()
{
	Vector4 v(2.0f, 4.0f, 6.0f, 8.0f);
	Vector4 d(1.0f, 1.0f, 1.0f, 1.0f);
	v -= d;
	V4ET_ASSERT(V4ET_NEAR(v.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(v.y, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(v.z, 5.0f));
	V4ET_ASSERT(V4ET_NEAR(v.w, 7.0f));
}

void MathStructTest::testV4ET_Vector4Negate()
{
	Vector4 v(1.0f, -2.0f, 3.0f, -4.0f);
	Vector4 neg = -v;
	V4ET_ASSERT(V4ET_NEAR(neg.x, -1.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.y, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.z, -3.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.w, 4.0f));
}

void MathStructTest::testV4ET_Vector4ZeroComponents()
{
	Vector4 a(0.0f, 0.0f, 0.0f, 0.0f);
	Vector4 b(1.0f, 2.0f, 3.0f, 4.0f);
	Vector4 c = a + b;
	V4ET_ASSERT(V4ET_NEAR(c.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(c.w, 4.0f));
}

void MathStructTest::testV4ET_Vector4NegativeComponents()
{
	Vector4 v(-1.0f, -2.0f, -3.0f, -4.0f);
	Vector4 neg = -v;
	V4ET_ASSERT(V4ET_NEAR(neg.x, 1.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.y, 2.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.z, 3.0f));
	V4ET_ASSERT(V4ET_NEAR(neg.w, 4.0f));
}

// ========== Vector4Int ==========

void MathStructTest::testV4ET_Vector4IntDefault()
{
	Vector4Int v;
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
	V4ET_ASSERT(v.w == 0);
}

void MathStructTest::testV4ET_Vector4IntConstruct()
{
	Vector4Int v(1, 2, 3, 4);
	V4ET_ASSERT(v.x == 1);
	V4ET_ASSERT(v.y == 2);
	V4ET_ASSERT(v.z == 3);
	V4ET_ASSERT(v.w == 4);
}

void MathStructTest::testV4ET_Vector4IntClear()
{
	Vector4Int v(10, 20, 30, 40);
	v.clear();
	V4ET_ASSERT(v.x == 0);
	V4ET_ASSERT(v.y == 0);
	V4ET_ASSERT(v.z == 0);
	V4ET_ASSERT(v.w == 0);
}

void MathStructTest::testV4ET_Vector4IntAdd()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(10, 20, 30, 40);
	Vector4Int c = a + b;
	V4ET_ASSERT(c.x == 11);
	V4ET_ASSERT(c.y == 22);
	V4ET_ASSERT(c.z == 33);
	V4ET_ASSERT(c.w == 44);
}

void MathStructTest::testV4ET_Vector4IntSub()
{
	Vector4Int a(10, 20, 30, 40);
	Vector4Int b(1, 2, 3, 4);
	Vector4Int c = a - b;
	V4ET_ASSERT(c.x == 9);
	V4ET_ASSERT(c.y == 18);
	V4ET_ASSERT(c.z == 27);
	V4ET_ASSERT(c.w == 36);
}

void MathStructTest::testV4ET_Vector4IntAddAssign()
{
	Vector4Int v(1, 2, 3, 4);
	Vector4Int d(5, 6, 7, 8);
	v += d;
	V4ET_ASSERT(v.x == 6);
	V4ET_ASSERT(v.y == 8);
	V4ET_ASSERT(v.z == 10);
	V4ET_ASSERT(v.w == 12);
}

void MathStructTest::testV4ET_Vector4IntSubAssign()
{
	Vector4Int v(10, 20, 30, 40);
	Vector4Int d(1, 2, 3, 4);
	v -= d;
	V4ET_ASSERT(v.x == 9);
	V4ET_ASSERT(v.y == 18);
	V4ET_ASSERT(v.z == 27);
	V4ET_ASSERT(v.w == 36);
}

void MathStructTest::testV4ET_Vector4IntNegate()
{
	Vector4Int v(1, -2, 3, -4);
	Vector4Int neg = -v;
	V4ET_ASSERT(neg.x == -1);
	V4ET_ASSERT(neg.y == 2);
	V4ET_ASSERT(neg.z == -3);
	V4ET_ASSERT(neg.w == 4);
}

void MathStructTest::testV4ET_Vector4IntMulScalar()
{
	Vector4Int v(1, 2, 3, 4);
	Vector4Int r = v * 5;
	V4ET_ASSERT(r.x == 5);
	V4ET_ASSERT(r.y == 10);
	V4ET_ASSERT(r.z == 15);
	V4ET_ASSERT(r.w == 20);
}

void MathStructTest::testV4ET_Vector4IntEqual()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 4);
	V4ET_ASSERT(a == b);
	Vector4Int c(1, 2, 3, 5);
	V4ET_ASSERT(!(a == c));
}

void MathStructTest::testV4ET_Vector4IntNotEqual()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(1, 2, 3, 5);
	V4ET_ASSERT(a != b);
	Vector4Int c(1, 2, 3, 4);
	V4ET_ASSERT(!(a != c));
}

void MathStructTest::testV4ET_Vector4IntLess()
{
	// operator< compares x+y+z+w
	Vector4Int a(1, 1, 1, 1); // sum=4
	Vector4Int b(2, 2, 2, 2); // sum=8
	V4ET_ASSERT(a < b);
	V4ET_ASSERT(!(b < a));
	Vector4Int c(1, 1, 1, 1); // sum=4
	V4ET_ASSERT(!(a < c));
}

void MathStructTest::testV4ET_Vector4IntChain()
{
	Vector4Int a(1, 2, 3, 4);
	Vector4Int b(5, 6, 7, 8);
	Vector4Int c(1, 1, 1, 1);
	// (a+b)-c
	Vector4Int r = (a + b) - c;
	V4ET_ASSERT(r.x == 5);
	V4ET_ASSERT(r.y == 7);
	V4ET_ASSERT(r.z == 9);
	V4ET_ASSERT(r.w == 11);
}

void MathStructTest::testV4ET_Vector4IntNegateChain()
{
	Vector4Int v(3, 4, 5, 6);
	Vector4Int neg = -v;
	Vector4Int back = -neg;
	V4ET_ASSERT(back == v);
}

void MathStructTest::testV4ET_Vector3IntChain()
{
	Vector3Int a(1, 2, 3);
	Vector3Int b(4, 5, 6);
	Vector3Int c(1, 1, 1);
	Vector3Int r = (a + b) - c;
	V4ET_ASSERT(r.x == 4);
	V4ET_ASSERT(r.y == 6);
	V4ET_ASSERT(r.z == 8);
}

void MathStructTest::testV4ET_Vector3IntMulZero()
{
	Vector3Int v(5, 10, 15);
	Vector3Int r = v * 0;
	V4ET_ASSERT(r.x == 0);
	V4ET_ASSERT(r.y == 0);
	V4ET_ASSERT(r.z == 0);
}

void MathStructTest::testV4ET_Vector4SubSelf()
{
	Vector4 v(3.0f, 3.0f, 3.0f, 3.0f);
	Vector4 r = v - v;
	V4ET_ASSERT(V4ET_NEAR(r.x, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(r.y, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(r.z, 0.0f));
	V4ET_ASSERT(V4ET_NEAR(r.w, 0.0f));
}

void MathStructTest::testVector4Family()
{
	testV4ET_Vector3IntDefault();
	testV4ET_Vector3IntConstruct();
	testV4ET_Vector3IntClear();
	testV4ET_Vector3IntAdd();
	testV4ET_Vector3IntSub();
	testV4ET_Vector3IntMulScalar();
	testV4ET_Vector3IntAddAssign();
	testV4ET_Vector3IntSubAssign();
	testV4ET_Vector3IntNegate();
	testV4ET_Vector3IntEqual();
	testV4ET_Vector3IntNotEqual();
	testV4ET_Vector3IntLess();
	testV4ET_Vector4Default();
	testV4ET_Vector4Construct();
	testV4ET_Vector4Clear();
	testV4ET_Vector4Add();
	testV4ET_Vector4Sub();
	testV4ET_Vector4AddAssign();
	testV4ET_Vector4SubAssign();
	testV4ET_Vector4Negate();
	testV4ET_Vector4ZeroComponents();
	testV4ET_Vector4NegativeComponents();
	testV4ET_Vector4IntDefault();
	testV4ET_Vector4IntConstruct();
	testV4ET_Vector4IntClear();
	testV4ET_Vector4IntAdd();
	testV4ET_Vector4IntSub();
	testV4ET_Vector4IntAddAssign();
	testV4ET_Vector4IntSubAssign();
	testV4ET_Vector4IntNegate();
	testV4ET_Vector4IntMulScalar();
	testV4ET_Vector4IntEqual();
	testV4ET_Vector4IntNotEqual();
	testV4ET_Vector4IntLess();
	testV4ET_Vector4IntChain();
	testV4ET_Vector4IntNegateChain();
	testV4ET_Vector3IntChain();
	testV4ET_Vector3IntMulZero();
	testV4ET_Vector4SubSelf();
}
#endif
