#ifdef WINDOWS
#include "FrameHeader.h"
#include "SerializerRead.h"
#include "SerializerWrite.h"
#include "SerializerBitRead.h"
#include "SerializerBitWrite.h"
namespace SerializerExtTestSupport
{
#define SXT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SerializerExtTest FAIL: ") + (msg)); } } while(0)
#define SXT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializerExtTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SXT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializerExtTest FAIL: ") + (msg) + \
		" | expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)
#define SXT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-4f) { ERROR(string("SerializerExtTest FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

	// ─────────────────────────────────────────────────────────────────────────────
	// Helper: write T to buffer via SerializerWrite, read back via SerializerRead
	// ─────────────────────────────────────────────────────────────────────────────
	template<typename T>
	static bool sxtRoundTrip(const T& val, T& out)
	{
		SerializerWrite w;
		w.initCapacity(64);
		if (!w.write(val))
		{ 
			return false;
		}
		SerializerRead r(w.getBuffer(), w.getDataSize());
		return r.read(out);
	}
}

void SerializerTest::testSxtPodTypes()
{
	using namespace SerializerExtTestSupport;
	// bool
	{
		bool v1 = true, v2 = false, r1 = false, r2 = true;
		SXT_ASSERT(sxtRoundTrip(v1, r1), "sxt bool true write ok");
		SXT_ASSERT(r1 == true, "sxt bool true read");
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt bool false write ok");
		SXT_ASSERT(r2 == false, "sxt bool false read");
	}

	// byte
	{
		byte v = 255, r = 0;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt byte 255 ok");
		SXT_ASSERT_EQ(r, 255, "sxt byte 255");
		byte z = 0, rz = 99;
		SXT_ASSERT(sxtRoundTrip(z, rz), "sxt byte 0 ok");
		SXT_ASSERT_EQ(rz, 0, "sxt byte 0");
	}

	// short
	{
		short v = -32767, r = 0;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt short -32767 ok");
		SXT_ASSERT_EQ(r, -32767, "sxt short -32767");
		short v2 = 32767, r2 = 0;
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt short max ok");
		SXT_ASSERT_EQ(r2, 32767, "sxt short max=32767");
		short z = 0, rz = 99;
		SXT_ASSERT(sxtRoundTrip(z, rz), "sxt short 0 ok");
		SXT_ASSERT_EQ(rz, 0, "sxt short 0");
	}

	// int
	{
		int v = 2147483647, r = 0;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt int max ok");
		SXT_ASSERT_EQ(r, 2147483647, "sxt int max");
		int v2 = -2147483647, r2 = 0;
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt int min+1 ok");
		SXT_ASSERT_EQ(r2, -2147483647, "sxt int min+1");
		int z = 0, rz = 99;
		SXT_ASSERT(sxtRoundTrip(z, rz), "sxt int 0 ok");
		SXT_ASSERT_EQ(rz, 0, "sxt int 0");
	}

	// uint
	{
		uint v = 4294967295u, r = 0u;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt uint max ok");
		SXT_ASSERT_EQ_LL((llong)r, (llong)4294967295u, "sxt uint max=4294967295");
		uint v2 = 0u, r2 = 99u;
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt uint 0 ok");
		SXT_ASSERT_EQ(r2, 0u, "sxt uint 0");
	}

	// llong
	{
		llong v = 9223372036854775807LL, r = 0LL;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt llong max ok");
		SXT_ASSERT_EQ_LL(r, 9223372036854775807LL, "sxt llong max");
		llong v2 = -9223372036854775807LL, r2 = 0LL;
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt llong min+1 ok");
		SXT_ASSERT_EQ_LL(r2, -9223372036854775807LL, "sxt llong min+1");
	}

	// float
	{
		float v = 3.14159f, r = 0.0f;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt float pi ok");
		SXT_ASSERT_FLOAT(r, 3.14159f, "sxt float pi");
		float v2 = -1234.5f, r2 = 0.0f;
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt float neg ok");
		SXT_ASSERT_FLOAT(r2, -1234.5f, "sxt float -1234.5");
		float z = 0.0f, rz = 99.0f;
		SXT_ASSERT(sxtRoundTrip(z, rz), "sxt float 0 ok");
		SXT_ASSERT_FLOAT(rz, 0.0f, "sxt float 0");
	}

	// double
	{
		double v = 3.141592653589793, r = 0.0;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt double pi ok");
		SXT_ASSERT((fabsf((float)r - 3.14159f) < 1e-4f), "sxt double pi approx");
		double z = 0.0, rz = 99.0;
		SXT_ASSERT(sxtRoundTrip(z, rz), "sxt double 0 ok");
		SXT_ASSERT((fabs(rz) < 1e-10), "sxt double 0");
	}

	// ushort
	{
		ushort v = 65535, r = 0;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt ushort max ok");
		SXT_ASSERT_EQ(r, 65535, "sxt ushort max=65535");
	}

	// char
	{
		char v = 'A', r = 0;
		SXT_ASSERT(sxtRoundTrip(v, r), "sxt char A ok");
		SXT_ASSERT_EQ(r, (char)'A', "sxt char A");
		char v2 = -127, r2 = 0;
		SXT_ASSERT(sxtRoundTrip(v2, r2), "sxt char -127 ok");
		SXT_ASSERT_EQ(r2, (char)-127, "sxt char -127");
	}
}

void SerializerTest::testSxtString()
{
	using namespace SerializerExtTestSupport;
	auto roundTripStr = [](const char* src, string& dst) -> bool
	{
		SerializerWrite w;
		w.initCapacity(128);
		w.writeString(src);
		SerializerRead r(w.getBuffer(), w.getDataSize());
		return r.readString(dst);
	};

	string s;
	SXT_ASSERT(roundTripStr("hello", s), "sxt str hello ok");
	SXT_ASSERT(s == "hello", "sxt str hello value");

	string s2;
	SXT_ASSERT(roundTripStr("", s2), "sxt empty str ok");
	SXT_ASSERT(s2 == "", "sxt empty str value");

	string s3;
	SXT_ASSERT(roundTripStr("test_123_!@#", s3), "sxt special str ok");
	SXT_ASSERT(s3 == "test_123_!@#", "sxt special str value");

	// Long string
	string longStr(200, 'x');
	SerializerWrite w;
	w.initCapacity(256);
	w.writeString(longStr);
	SerializerRead r(w.getBuffer(), w.getDataSize());
	string s4;
	SXT_ASSERT(r.readString(s4), "sxt long str ok");
	SXT_ASSERT(s4 == longStr, "sxt long str value");

	// string overload
	SerializerWrite w2;
	w2.initCapacity(128);
	const string src2 = "overload_test";
	w2.writeString(src2);
	SerializerRead r2(w2.getBuffer(), w2.getDataSize());
	string s5;
	SXT_ASSERT(r2.readString(s5), "sxt string overload ok");
	SXT_ASSERT(s5 == "overload_test", "sxt string overload value");
}

void SerializerTest::testSxtVector2()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(64);
	SXT_ASSERT(w.writeVector2(Vector2(3.14f, 2.71f)), "sxt V2 write ok");

	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector2 v;
	SXT_ASSERT(r.readVector2(v), "sxt V2 read ok");
	SXT_ASSERT_FLOAT(v.x, 3.14f, "sxt V2 x=3.14");
	SXT_ASSERT_FLOAT(v.y, 2.71f, "sxt V2 y=2.71");

	// Multiple V2
	SerializerWrite w2;
	w2.initCapacity(128);
	w2.writeVector2(Vector2(1.0f, 2.0f));
	w2.writeVector2(Vector2(3.0f, 4.0f));
	w2.writeVector2(Vector2(-1.0f, -2.0f));

	SerializerRead r2(w2.getBuffer(), w2.getDataSize());
	Vector2 va, vb, vc;
	r2.readVector2(va);
	r2.readVector2(vb);
	r2.readVector2(vc);
	SXT_ASSERT_FLOAT(va.x, 1.0f, "sxt V2 multi a.x=1");
	SXT_ASSERT_FLOAT(vb.x, 3.0f, "sxt V2 multi b.x=3");
	SXT_ASSERT_FLOAT(vc.x, -1.0f, "sxt V2 multi c.x=-1");
	SXT_ASSERT_FLOAT(vc.y, -2.0f, "sxt V2 multi c.y=-2");
}

void SerializerTest::testSxtVector2Int()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(64);
	SXT_ASSERT(w.writeVector2Int(Vector2Int(100, -200)), "sxt V2I write ok");

	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector2Int v;
	SXT_ASSERT(r.readVector2Int(v), "sxt V2I read ok");
	SXT_ASSERT_EQ(v.x, 100, "sxt V2I x=100");
	SXT_ASSERT_EQ(v.y, -200, "sxt V2I y=-200");

	// Zero
	SerializerWrite w2;
	w2.initCapacity(32);
	w2.writeVector2Int(Vector2Int(0, 0));
	SerializerRead r2(w2.getBuffer(), w2.getDataSize());
	Vector2Int z;
	r2.readVector2Int(z);
	SXT_ASSERT_EQ(z.x, 0, "sxt V2I zero x=0");
	SXT_ASSERT_EQ(z.y, 0, "sxt V2I zero y=0");
}

void SerializerTest::testSxtVector3()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(64);
	SXT_ASSERT(w.writeVector3(Vector3(1.1f, 2.2f, 3.3f)), "sxt V3 write ok");

	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector3 v;
	SXT_ASSERT(r.readVector3(v), "sxt V3 read ok");
	SXT_ASSERT_FLOAT(v.x, 1.1f, "sxt V3 x=1.1");
	SXT_ASSERT_FLOAT(v.y, 2.2f, "sxt V3 y=2.2");
	SXT_ASSERT_FLOAT(v.z, 3.3f, "sxt V3 z=3.3");

	// Negative values
	SerializerWrite w2;
	w2.initCapacity(64);
	w2.writeVector3(Vector3(-5.0f, 0.0f, 5.0f));
	SerializerRead r2(w2.getBuffer(), w2.getDataSize());
	Vector3 v2;
	r2.readVector3(v2);
	SXT_ASSERT_FLOAT(v2.x, -5.0f, "sxt V3 neg x=-5");
	SXT_ASSERT_FLOAT(v2.z, 5.0f, "sxt V3 pos z=5");
}

void SerializerTest::testSxtVector4()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(64);
	SXT_ASSERT(w.writeVector4(Vector4(1.0f, 2.0f, 3.0f, 4.0f)), "sxt V4 write ok");

	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector4 v;
	SXT_ASSERT(r.readVector4(v), "sxt V4 read ok");
	SXT_ASSERT_FLOAT(v.x, 1.0f, "sxt V4 x=1");
	SXT_ASSERT_FLOAT(v.y, 2.0f, "sxt V4 y=2");
	SXT_ASSERT_FLOAT(v.z, 3.0f, "sxt V4 z=3");
	SXT_ASSERT_FLOAT(v.w, 4.0f, "sxt V4 w=4");
}

void SerializerTest::testSxtList()
{
	using namespace SerializerExtTestSupport;
	// int list
	{
		Vector<int> src;
		src.add(10);
		src.add(-20);
		src.add(300);
		src.add(0);

		SerializerWrite w;
		w.initCapacity(128);
		SXT_ASSERT(w.writeList(src), "sxt int list write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<int> dst;
		SXT_ASSERT(r.readList(dst), "sxt int list read ok");
		SXT_ASSERT_EQ(dst.size(), 4, "sxt int list size=4");
		SXT_ASSERT_EQ(dst[0], 10, "sxt int list[0]=10");
		SXT_ASSERT_EQ(dst[1], -20, "sxt int list[1]=-20");
		SXT_ASSERT_EQ(dst[2], 300, "sxt int list[2]=300");
		SXT_ASSERT_EQ(dst[3], 0, "sxt int list[3]=0");
	}

	// float list
	{
		Vector<float> src;
		src.add(1.5f);
		src.add(-2.5f);
		src.add(3.14f);

		SerializerWrite w;
		w.initCapacity(128);
		SXT_ASSERT(w.writeList(src), "sxt float list write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<float> dst;
		SXT_ASSERT(r.readList(dst), "sxt float list read ok");
		SXT_ASSERT_EQ(dst.size(), 3, "sxt float list size=3");
		SXT_ASSERT_FLOAT(dst[0], 1.5f, "sxt float list[0]=1.5");
		SXT_ASSERT_FLOAT(dst[1], -2.5f, "sxt float list[1]=-2.5");
		SXT_ASSERT_FLOAT(dst[2], 3.14f, "sxt float list[2]=3.14");
	}

	// byte list
	{
		Vector<byte> src;
		for (byte i = 0; i < 10; ++i) 
		{ 
			src.add(i);
		}

		SerializerWrite w;
		w.initCapacity(128);
		SXT_ASSERT(w.writeList(src), "sxt byte list write ok");

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<byte> dst;
		SXT_ASSERT(r.readList(dst), "sxt byte list read ok");
		SXT_ASSERT_EQ(dst.size(), 10, "sxt byte list size=10");
		FOR(10)
		{
			SXT_ASSERT_EQ(dst[i], (byte)i, "sxt byte list[i]=i");
		}
	}

	// empty list
	{
		Vector<int> empty;
		SerializerWrite w;
		w.initCapacity(32);
		SXT_ASSERT(w.writeList(empty), "sxt empty list write ok");
		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<int> dst;
		SXT_ASSERT(r.readList(dst), "sxt empty list read ok");
		SXT_ASSERT(dst.isEmpty(), "sxt empty list isEmpty");
	}
}

void SerializerTest::testSxtMixed()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(256);
	w.write((int)42);
	w.write((float)3.14f);
	w.write((bool)true);
	w.write((byte)255);
	w.writeString("mixed_test");
	w.write((short)-100);
	w.write((llong)1234567890123LL);

	SerializerRead r(w.getBuffer(), w.getDataSize());
	int i = 0;  
	r.read(i);
	float f = 0.0f;
	r.read(f);
	bool b = false; 
	r.read(b);
	byte by = 0;
	r.read(by);
	string s;
	r.readString(s);
	short sh = 0;
	r.read(sh);
	llong ll = 0;
	r.read(ll);

	SXT_ASSERT_EQ(i, 42, "mixed int=42");
	SXT_ASSERT_FLOAT(f, 3.14f, "mixed float=3.14");
	SXT_ASSERT(b == true, "mixed bool=true");
	SXT_ASSERT_EQ(by, (byte)255, "mixed byte=255");
	SXT_ASSERT(s == "mixed_test", "mixed str=mixed_test");
	SXT_ASSERT_EQ(sh, -100, "mixed short=-100");
	SXT_ASSERT_EQ_LL(ll, 1234567890123LL, "mixed llong=1234567890123");
}

void SerializerTest::testSxtIndex()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(64);
	w.write((int)1);
	w.write((int)2);
	SXT_ASSERT_EQ(w.getDataSize(), (int)(sizeof(int) * 2), "sxt write size=8");

	// reset and rewrite
	w.clear();
	SXT_ASSERT_EQ(w.getDataSize(), 0, "sxt clear size=0");
	w.write((int)99);

	SerializerRead r(w.getBuffer(), w.getDataSize());
	SXT_ASSERT_EQ(r.getIndex(), 0, "sxt read index=0 initially");
	int v = 0;
	r.read(v);
	SXT_ASSERT_EQ(v, 99, "sxt read after clear=99");
	SXT_ASSERT_EQ(r.getIndex(), (int)sizeof(int), "sxt read index=4 after int");

	// setIndex: rewind and re-read
	r.setIndex(0);
	SXT_ASSERT_EQ(r.getIndex(), 0, "sxt setIndex(0) ok");
	int v2 = 0;
	r.read(v2);
	SXT_ASSERT_EQ(v2, 99, "sxt rewind read=99");
}

void SerializerTest::testSxtBuffer()
{
	using namespace SerializerExtTestSupport;
	const char src[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
	SerializerWrite w;
	w.initCapacity(32);
	SXT_ASSERT(w.writeBuffer(src, 5), "sxt writeBuffer ok");
	SXT_ASSERT_EQ(w.getDataSize(), 5, "sxt writeBuffer size=5");

	char dst[5] = {};
	SerializerRead r(w.getBuffer(), w.getDataSize());
	SXT_ASSERT(r.readBuffer(dst, 5, 5), "sxt readBuffer ok");
	FOR(5)
	{
		SXT_ASSERT_EQ(dst[i], src[i], "sxt buffer[i] match");
	}
}

void SerializerTest::testSxtStringList()
{
	using namespace SerializerExtTestSupport;
	Vector<string> src;
	src.add("alpha");
	src.add("beta");
	src.add("gamma");
	src.add("");

	SerializerWrite w;
	w.initCapacity(256);
	SXT_ASSERT(w.writeStringList(src), "sxt string list write ok");

	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector<string> dst;
	SXT_ASSERT(r.readStringList(dst), "sxt string list read ok");
	SXT_ASSERT_EQ(dst.size(), 4, "sxt string list size=4");
	SXT_ASSERT(dst[0] == "alpha", "sxt strlist[0]=alpha");
	SXT_ASSERT(dst[1] == "beta", "sxt strlist[1]=beta");
	SXT_ASSERT(dst[2] == "gamma", "sxt strlist[2]=gamma");
	SXT_ASSERT(dst[3] == "", "sxt strlist[3]=empty");
}

void SerializerTest::testSxtVectorLists()
{
	using namespace SerializerExtTestSupport;
	// Vector2 list
	{
		Vector<Vector2> src;
		src.add(Vector2(1.0f, 2.0f));
		src.add(Vector2(-3.0f, 4.0f));
		src.add(Vector2(0.0f, 0.0f));

		SerializerWrite w;
		w.initCapacity(128);
		// Use manual approach since writeList takes POD only
		// Write count then each vector
		w.write((int)src.size());
		for (const auto& v : src) 
		{
			w.writeVector2(v); 
		}

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<Vector2> dst;
		SXT_ASSERT(r.readVector2List(dst), "sxt V2 list read ok");
		SXT_ASSERT_EQ(dst.size(), 3, "sxt V2 list size=3");
		SXT_ASSERT_FLOAT(dst[0].x, 1.0f, "sxt V2 list[0].x=1");
		SXT_ASSERT_FLOAT(dst[1].x, -3.0f, "sxt V2 list[1].x=-3");
		SXT_ASSERT_FLOAT(dst[2].x, 0.0f, "sxt V2 list[2].x=0");
	}

	// Vector3 list
	{
		Vector<Vector3> src;
		src.add(Vector3(1.0f, 2.0f, 3.0f));
		src.add(Vector3(-1.0f, -2.0f, -3.0f));

		SerializerWrite w;
		w.initCapacity(128);
		w.write((int)src.size());
		for (const auto& v : src)
		{
			w.writeVector3(v); 
		}

		SerializerRead r(w.getBuffer(), w.getDataSize());
		Vector<Vector3> dst;
		SXT_ASSERT(r.readVector3List(dst), "sxt V3 list read ok");
		SXT_ASSERT_EQ(dst.size(), 2, "sxt V3 list size=2");
		SXT_ASSERT_FLOAT(dst[0].z, 3.0f, "sxt V3 list[0].z=3");
		SXT_ASSERT_FLOAT(dst[1].z, -3.0f, "sxt V3 list[1].z=-3");
	}
}

void SerializerTest::testSxtStress()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(4096);
	FOR(1000)
	{
		SXT_ASSERT(w.write(i), "sxt stress write ok");
	}
	SXT_ASSERT_EQ(w.getDataSize(), (int)(1000 * sizeof(int)), "sxt stress size=4000");

	SerializerRead r(w.getBuffer(), w.getDataSize());
	FOR(1000)
	{
		int v = -1;
		SXT_ASSERT(r.read(v), "sxt stress read ok");
		SXT_ASSERT_EQ(v, i, "sxt stress val=i");
	}

	// Alternating int/float
	SerializerWrite w2;
	w2.initCapacity(2048);
	FOR(100)
	{
		w2.write(i);
		w2.write((float)i * 0.5f);
	}
	SerializerRead r2(w2.getBuffer(), w2.getDataSize());
	FOR(100)
	{
		int iv = -1;
		float fv = -1.0f;
		r2.read(iv);
		r2.read(fv);
		SXT_ASSERT_EQ(iv, i, "sxt alt int");
		SXT_ASSERT_FLOAT(fv, i * 0.5f, "sxt alt float");
	}
}

void SerializerTest::testSxtBoundaryValues()
{
	using namespace SerializerExtTestSupport;
	// bool both values
	{ bool a = true, b = false; SXT_ASSERT(sxtRoundTrip(a, b) && b, "boundary bool true"); }
	{ bool a = false, b = true; SXT_ASSERT(sxtRoundTrip(a, b) && !b, "boundary bool false"); }

	// byte 0 and 255
	{ byte a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary byte 0"); }
	{ byte a = 255, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 255, "boundary byte 255"); }
	{ byte a = 128, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 128, "boundary byte 128"); }
	{ byte a = 127, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 127, "boundary byte 127"); }

	// short: 0, max, min+1, -1
	{ short a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary short 0"); }
	{ short a = 32767, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 32767, "boundary short max"); }
	{ short a = -32767, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -32767, "boundary short min+1"); }
	{ short a = -1, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -1, "boundary short -1"); }

	// int: 0, max, min+1, -1, 1
	{ int a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary int 0"); }
	{ int a = 2147483647, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 2147483647, "boundary int max"); }
	{ int a = -2147483647, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -2147483647, "boundary int min+1"); }
	{ int a = -1, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == -1, "boundary int -1"); }
	{ int a = 1, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 1, "boundary int 1"); }

	// llong
	{ llong a = 0LL, b = 1LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0LL, "boundary llong 0"); }
	{ llong a = 9223372036854775807LL, b = 0LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == 9223372036854775807LL, "boundary llong max"); }
	{ llong a = -9223372036854775807LL, b = 0LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == -9223372036854775807LL, "boundary llong min+1"); }
	{ llong a = -1LL, b = 0LL; SXT_ASSERT(sxtRoundTrip(a, b) && b == -1LL, "boundary llong -1"); }

	// uint
	{ uint a = 0u, b = 1u; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0u, "boundary uint 0"); }
	{ uint a = 4294967295u, b = 0u; SXT_ASSERT(sxtRoundTrip(a, b) && b == 4294967295u, "boundary uint max"); }

	// ushort
	{ ushort a = 0, b = 1; SXT_ASSERT(sxtRoundTrip(a, b) && b == 0, "boundary ushort 0"); }
	{ ushort a = 65535, b = 0; SXT_ASSERT(sxtRoundTrip(a, b) && b == 65535, "boundary ushort max"); }

	// float special values
	{ float a = 0.0f, b = 1.0f; SXT_ASSERT(sxtRoundTrip(a, b), "float 0 roundtrip"); SXT_ASSERT_FLOAT(b, 0.0f, "boundary float 0"); }
	{ float a = -0.0f, b = 1.0f; sxtRoundTrip(a, b); }  // just no crash
	{ float a = 1e38f, b = 0.0f; SXT_ASSERT(sxtRoundTrip(a, b), "float large roundtrip"); SXT_ASSERT_FLOAT(b, 1e38f, "boundary float large"); }
	{ float a = -1e38f, b = 0.0f; SXT_ASSERT(sxtRoundTrip(a, b), "float large neg roundtrip"); SXT_ASSERT_FLOAT(b, -1e38f, "boundary float large neg"); }

	// double
	{ double a = 0.0, b = 1.0; SXT_ASSERT(sxtRoundTrip(a, b), "double 0 roundtrip"); SXT_ASSERT((b >= -1e-12 && b <= 1e-12), "boundary double 0"); }
	{ double a = 3.141592653589793, b = 0.0; SXT_ASSERT(sxtRoundTrip(a, b), "double pi roundtrip"); SXT_ASSERT(fabsf((float)(b - 3.141592653589793)) < 1e-6f, "boundary double pi"); }
}

void SerializerTest::testSxtSequentialWrites()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(256);

	// Write a sequence of different types
	w.write((int)1001);
	w.write((float)3.14f);
	w.write((bool)true);
	w.write((short)(-500));
	w.write((llong)999999999LL);
	w.write((byte)200);
	w.write((ushort)60000);
	w.write((uint)123456789u);
	w.write((double)2.718281828);

	SerializerRead r(w.getBuffer(), w.getDataSize());
	int ri = 0;
	r.read(ri);
	SXT_ASSERT_EQ(ri, 1001, "seq int=1001");
	float rf = 0;
	r.read(rf);
	SXT_ASSERT_FLOAT(rf, 3.14f, "seq float=3.14");
	bool rb = false;
	r.read(rb);
	SXT_ASSERT(rb, "seq bool=true");
	short rs = 0;
	r.read(rs);
	SXT_ASSERT_EQ((int)rs, -500, "seq short=-500");
	llong rll = 0; 
	r.read(rll);  
	SXT_ASSERT_EQ_LL(rll, 999999999LL, "seq llong");
	byte rby = 0;  
	r.read(rby);  
	SXT_ASSERT_EQ((int)rby, 200, "seq byte=200");
	ushort rus = 0; 
	r.read(rus);  
	SXT_ASSERT_EQ((int)rus, 60000, "seq ushort=60000");
	uint rui = 0;  
	r.read(rui);  
	SXT_ASSERT(rui == 123456789u, "seq uint");
	double rd = 0; 
	r.read(rd);   
	SXT_ASSERT(fabsf((float)(rd - 2.718281828)) < 1e-6f, "seq double");
}

void SerializerTest::testSxtLargeString()
{
	using namespace SerializerExtTestSupport;
	// Build a 500-char string
	string big;
	big.reserve(500);
	FOR(500)
	{
		big += (char)('a' + (i % 26));
	}

	// string 不是 POD，用 writeString/readString 专用接口
	{
		SerializerWrite w;
		w.initCapacity(512);
		SXT_ASSERT(w.writeString(big), "large string write ok");
		SerializerRead r(w.getBuffer(), w.getDataSize());
		string out;
		SXT_ASSERT(r.readString(out), "large string read ok");
		SXT_ASSERT_EQ((int)out.size(), 500, "large string size=500");
		SXT_ASSERT(out == big, "large string content match");
	}

	// Empty string
	{
		SerializerWrite w;
		w.initCapacity(16);
		string empty;
		SXT_ASSERT(w.writeString(empty), "empty string write ok");
		SerializerRead r(w.getBuffer(), w.getDataSize());
		string emptyOut;
		SXT_ASSERT(r.readString(emptyOut), "empty string read ok");
		SXT_ASSERT_EQ((int)emptyOut.size(), 0, "empty string size=0");
	}

	// Single char
	{
		SerializerWrite w;
		w.initCapacity(16);
		string sc = "X";
		SXT_ASSERT(w.writeString(sc), "single char string write ok");
		SerializerRead r(w.getBuffer(), w.getDataSize());
		string scOut;
		SXT_ASSERT(r.readString(scOut), "single char string read ok");
		SXT_ASSERT(scOut == "X", "single char string match");
	}
}

void SerializerTest::testSxtManySmallInts()
{
	using namespace SerializerExtTestSupport;
	const int COUNT = 50;
	SerializerWrite w;
	w.initCapacity(COUNT * 4 + 16);
	FOR(COUNT)
	{
		w.write(i * 3 - COUNT);  // mix of neg and pos
	}

	SerializerRead r(w.getBuffer(), w.getDataSize());
	FOR(COUNT)
	{
		int v = 0;
		r.read(v);
		SXT_ASSERT_EQ(v, i * 3 - COUNT, "many small ints");
	}
}

void SerializerTest::testSxtBoolArray()
{
	using namespace SerializerExtTestSupport;
	const bool src[] = { true, false, true, true, false, false, true, false };
	const int N = 8;
	SerializerWrite w;
	w.initCapacity(64);
	FOR(N)
	{
		w.write(src[i]);
	}

	SerializerRead r(w.getBuffer(), w.getDataSize());
	FOR(N)
	{
		bool v = !src[i];  // start different
		r.read(v);
		SXT_ASSERT(v == src[i], "bool array element");
	}
}

void SerializerTest::testSxtStringIntInterleaved()
{
	using namespace SerializerExtTestSupport;
	SerializerWrite w;
	w.initCapacity(256);
	const int PAIRS = 10;
	FOR(PAIRS)
	{
		string s = "item" + IToS(i);
		w.write(i * 100);
		w.writeString(s);
	}

	SerializerRead r(w.getBuffer(), w.getDataSize());
	FOR(PAIRS)
	{
		int iv = -1;
		string sv;
		r.read(iv);
		r.readString(sv);
		SXT_ASSERT_EQ(iv, i * 100, "interleaved int");
		string expected = "item" + IToS(i);
		SXT_ASSERT(sv == expected, "interleaved string");
	}
}

namespace SerializerByteTestSupport
{
	// ============================================================
	// SerializerByteTest — SerializerWrite/SerializerRead 全方位测试
	// 覆盖：write<T>/read<T>/writeVector2/3/4/readVector2/3/4/
	//        writeString/readString/writeList/readList/
	//        writeCustom/readCustom/writeCustomList/readCustomList/
	//        writeStringList/readStringList/writeBuffer/readBuffer/
	//        readBufferNoCopy/setIndex/getIndex/getBuffer/getBufferSize/
	//        initCapacity/clear/getDataSize
	// ============================================================

#define SBT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("SerializerByteTest FAILED: ") + (msg)); }

// ------------------------------------------------------------------
// 辅助：自定义可序列化结构体（继承 SerializableData）
// ------------------------------------------------------------------
	struct TestPoint : public SerializableData
	{
		int x = 0;
		int y = 0;
		int z = 0;

		bool readFromBuffer(SerializerRead* reader) override
		{
			return reader->read(x) && reader->read(y) && reader->read(z);
		}
		bool writeToBuffer(SerializerWrite* serializer) const override
		{
			return serializer->write(x) && serializer->write(y) && serializer->write(z);
		}
		void resetProperty() override { x = 0; y = 0; z = 0; }
		bool operator==(const TestPoint& o) const { return x == o.x && y == o.y && z == o.z; }
	};

	struct TestPlayerInfo : public SerializableData
	{
		int id = 0;
		float hp = 0.0f;
		string name;

		bool readFromBuffer(SerializerRead* reader) override
		{
			return reader->read(id) && reader->read(hp) && reader->readString(name);
		}
		bool writeToBuffer(SerializerWrite* serializer) const override
		{
			return serializer->write(id) && serializer->write(hp) && serializer->writeString(name);
		}
		void resetProperty() override { id = 0; hp = 0.0f; name.clear(); }
	};
}

void SerializerTest::testSB_BoolRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<bool>(true);
	sw.write<bool>(false);
	sw.write<bool>(true);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	bool v1;
	bool v2;
	bool v3;
	SBT_ASSERT(sr.read(v1) && v1 == true, "bool true");
	SBT_ASSERT(sr.read(v2) && v2 == false, "bool false");
	SBT_ASSERT(sr.read(v3) && v3 == true, "bool true again");
}

void SerializerTest::testSB_ByteRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<byte>((byte)0);
	sw.write<byte>((byte)127);
	sw.write<byte>((byte)255);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	byte v1;
	byte v2;
	byte v3;
	SBT_ASSERT(sr.read(v1) && v1 == 0, "byte 0");
	SBT_ASSERT(sr.read(v2) && v2 == 127, "byte 127");
	SBT_ASSERT(sr.read(v3) && v3 == 255, "byte 255");
}

void SerializerTest::testSB_ShortRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<short>((short)-32767);
	sw.write<short>((short)0);
	sw.write<short>((short)32767);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	short v1;
	short v2;
	short v3;
	SBT_ASSERT(sr.read(v1) && v1 == -32767, "short min");
	SBT_ASSERT(sr.read(v2) && v2 == 0, "short 0");
	SBT_ASSERT(sr.read(v3) && v3 == 32767, "short max");
}

void SerializerTest::testSB_UShortRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<ushort>((ushort)0);
	sw.write<ushort>((ushort)65535);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	ushort v1;
	ushort v2;
	SBT_ASSERT(sr.read(v1) && v1 == 0, "ushort 0");
	SBT_ASSERT(sr.read(v2) && v2 == 65535, "ushort 65535");
}

void SerializerTest::testSB_IntRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(-2147483647);
	sw.write<int>(0);
	sw.write<int>(2147483647);
	sw.write<int>(12345678);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	int v1;
	int v2;
	int v3;
	int v4;
	SBT_ASSERT(sr.read(v1) && v1 == -2147483647, "int min");
	SBT_ASSERT(sr.read(v2) && v2 == 0, "int 0");
	SBT_ASSERT(sr.read(v3) && v3 == 2147483647, "int max");
	SBT_ASSERT(sr.read(v4) && v4 == 12345678, "int mid");
}

void SerializerTest::testSB_UIntRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<uint>(0u);
	sw.write<uint>(4294967295u);
	sw.write<uint>(99999u);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	uint v1;
	uint v2;
	uint v3;
	SBT_ASSERT(sr.read(v1) && v1 == 0u, "uint 0");
	SBT_ASSERT(sr.read(v2) && v2 == 4294967295u, "uint max");
	SBT_ASSERT(sr.read(v3) && v3 == 99999u, "uint mid");
}

void SerializerTest::testSB_LLongRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(128);
	sw.write<llong>(-9223372036854775807LL);
	sw.write<llong>(0LL);
	sw.write<llong>(9223372036854775807LL);
	sw.write<llong>(1234567890123456LL);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	llong v1;
	llong v2;
	llong v3;
	llong v4;
	SBT_ASSERT(sr.read(v1) && v1 == -9223372036854775807LL, "llong min");
	SBT_ASSERT(sr.read(v2) && v2 == 0LL, "llong 0");
	SBT_ASSERT(sr.read(v3) && v3 == 9223372036854775807LL, "llong max");
	SBT_ASSERT(sr.read(v4) && v4 == 1234567890123456LL, "llong mid");
}

void SerializerTest::testSB_FloatRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(128);
	sw.write<float>(0.0f);
	sw.write<float>(3.14159f);
	sw.write<float>(-99.9f);
	sw.write<float>(1234567.0f);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	float v1;
	float v2;
	float v3;
	float v4;
	SBT_ASSERT(sr.read(v1) && fabsf(v1) < 1e-6f, "float 0");
	SBT_ASSERT(sr.read(v2) && fabsf(v2 - 3.14159f) < 1e-4f, "float pi");
	SBT_ASSERT(sr.read(v3) && fabsf(v3 + 99.9f) < 1e-3f, "float neg");
	SBT_ASSERT(sr.read(v4) && fabsf(v4 - 1234567.0f) < 1.0f, "float large");
}

void SerializerTest::testSB_DoubleRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(128);
	sw.write<double>(0.0);
	sw.write<double>(3.14159265358979);
	sw.write<double>(-1.23456789e10);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	double v1;
	double v2;
	double v3;
	SBT_ASSERT(sr.read(v1) && fabs(v1) < 1e-15, "double 0");
	SBT_ASSERT(sr.read(v2) && fabs(v2 - 3.14159265358979) < 1e-10, "double pi");
	SBT_ASSERT(sr.read(v3) && fabs(v3 + 1.23456789e10) < 1.0, "double large neg");
}

void SerializerTest::testSB_ReadReturnValue()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(42);
	sw.write<float>(1.5f);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	int iv = sr.read<int>();
	float fv = sr.read<float>();
	SBT_ASSERT(iv == 42, "readReturn int==42");
	SBT_ASSERT(fabsf(fv - 1.5f) < 1e-5f, "readReturn float==1.5");
}

void SerializerTest::testSB_Vector2RoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector2 in = { 1.0f, 2.5f };
	sw.writeVector2(in);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector2 out;
	SBT_ASSERT(sr.readVector2(out), "readVector2 true");
	SBT_ASSERT(fabsf(out.x - 1.0f) < 1e-5f && fabsf(out.y - 2.5f) < 1e-5f, "vector2 values");
}

void SerializerTest::testSB_Vector2IntRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector2Int in = { -100, 200 };
	sw.writeVector2Int(in);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector2Int out;
	SBT_ASSERT(sr.readVector2Int(out), "readVector2Int true");
	SBT_ASSERT(out.x == -100 && out.y == 200, "vector2int values");
}

void SerializerTest::testSB_Vector3RoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector3 in = { 1.0f, 2.0f, 3.0f };
	sw.writeVector3(in);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector3 out;
	SBT_ASSERT(sr.readVector3(out), "readVector3 true");
	SBT_ASSERT(fabsf(out.x - 1.0f) < 1e-5f, "vector3 x");
	SBT_ASSERT(fabsf(out.y - 2.0f) < 1e-5f, "vector3 y");
	SBT_ASSERT(fabsf(out.z - 3.0f) < 1e-5f, "vector3 z");
}

void SerializerTest::testSB_Vector4RoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector4 in = { 1.0f, 2.0f, 3.0f, 4.0f };
	sw.writeVector4(in);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector4 out;
	SBT_ASSERT(sr.readVector4(out), "readVector4 true");
	SBT_ASSERT(fabsf(out.x - 1.0f) < 1e-5f, "vector4 x");
	SBT_ASSERT(fabsf(out.y - 2.0f) < 1e-5f, "vector4 y");
	SBT_ASSERT(fabsf(out.z - 3.0f) < 1e-5f, "vector4 z");
	SBT_ASSERT(fabsf(out.w - 4.0f) < 1e-5f, "vector4 w");
}

void SerializerTest::testSB_MultipleVector3()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(128);
	FOR(5)
	{
		Vector3 v = { (float)i, (float)(i * 2), (float)(i * 3) };
		sw.writeVector3(v);
	}
	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	FOR(5)
	{
		Vector3 out;
		SBT_ASSERT(sr.readVector3(out), "multiVec3 read true");
		SBT_ASSERT(fabsf(out.x - i) < 1e-5f, "multiVec3 x");
		SBT_ASSERT(fabsf(out.y - i * 2) < 1e-5f, "multiVec3 y");
		SBT_ASSERT(fabsf(out.z - i * 3) < 1e-5f, "multiVec3 z");
	}
}

void SerializerTest::testSB_StringRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	sw.writeString("hello world");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	string s;
	SBT_ASSERT(sr.readString(s), "readString true");
	SBT_ASSERT(s == "hello world", "string value");
}

void SerializerTest::testSB_EmptyStringRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.writeString("");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	string s;
	SBT_ASSERT(sr.readString(s), "readString empty true");
	SBT_ASSERT(s.empty(), "empty string");
}

void SerializerTest::testSB_MultipleStrings()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(512);
	const char* strs[] = { "alpha", "beta", "gamma", "delta", "epsilon" };
	FOR(5)
	{ 
		sw.writeString(strs[i]);
	}

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	FOR(5)
	{
		string out;
		SBT_ASSERT(sr.readString(out), "multiStr read true");
		SBT_ASSERT(out == strs[i], "multiStr value");
	}
}

void SerializerTest::testSB_LongString()
{
	using namespace SerializerByteTestSupport;
	string longStr(1000, 'X');
	SerializerWrite sw;
	sw.initCapacity(1200);
	sw.writeString(longStr);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	string out;
	SBT_ASSERT(sr.readString(out), "longStr read true");
	SBT_ASSERT(out.size() == 1000, "longStr size");
	SBT_ASSERT(out == longStr, "longStr equal");
}

void SerializerTest::testSB_StringWithStdString()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(128);
	string in = "test_std_string";
	sw.writeString(in);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	string out;
	SBT_ASSERT(sr.readString(out) && out == in, "stdString roundtrip");
}

void SerializerTest::testSB_IntListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	Vector<int> src;
	FOR(10)
	{
		src.add(i * 10); 
	}
	SBT_ASSERT(sw.writeList(src), "writeIntList true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<int> dst;
	SBT_ASSERT(sr.readList(dst), "readIntList true");
	SBT_ASSERT(dst.size() == 10, "intList size==10");
	FOR(10)
	{
		SBT_ASSERT(dst[i] == i * 10, "intList[i]==i*10");
	}
}

void SerializerTest::testSB_EmptyListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector<int> empty;
	SBT_ASSERT(sw.writeList(empty), "writeEmptyList true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<int> dst;
	SBT_ASSERT(sr.readList(dst), "readEmptyList true");
	SBT_ASSERT(dst.isEmpty(), "emptyList empty");
}

void SerializerTest::testSB_FloatListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	Vector<float> src;
	FOR(5) { src.add(i * 1.5f); }
	sw.writeList(src);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<float> dst;
	SBT_ASSERT(sr.readList(dst), "floatList read true");
	SBT_ASSERT(dst.size() == 5, "floatList size==5");
	FOR(5)
	{
		SBT_ASSERT(fabsf(dst[i] - i * 1.5f) < 1e-4f, "floatList[i] correct");
	}
}

void SerializerTest::testSB_LLongListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	Vector<llong> src;
	src.add(1LL); 
	src.add(-9999999999LL); 
	src.add(0LL);
	sw.writeList(src);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<llong> dst;
	SBT_ASSERT(sr.readList(dst), "llongList read true");
	SBT_ASSERT(dst.size() == 3, "llongList size==3");
	SBT_ASSERT(dst[0] == 1LL && dst[1] == -9999999999LL && dst[2] == 0LL, "llongList values");
}

void SerializerTest::testSB_StringListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(512);
	Vector<string> src;
	src.add("one"); src.add("two"); src.add("three");
	SBT_ASSERT(sw.writeStringList(src), "writeStringList true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<string> dst;
	SBT_ASSERT(sr.readStringList(dst), "readStringList true");
	SBT_ASSERT(dst.size() == 3, "stringList size==3");
	SBT_ASSERT(dst[0] == "one" && dst[1] == "two" && dst[2] == "three", "stringList values");
}

void SerializerTest::testSB_EmptyStringList()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector<string> empty;
	sw.writeStringList(empty);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<string> dst;
	SBT_ASSERT(sr.readStringList(dst), "emptyStringList read true");
	SBT_ASSERT(dst.isEmpty(), "emptyStringList empty");
}

void SerializerTest::testSB_CustomRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	TestPoint p1;
	p1.x = 10; 
	p1.y = 20; 
	p1.z = 30;
	SBT_ASSERT(sw.writeCustom(p1), "writeCustom true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	TestPoint p2;
	SBT_ASSERT(sr.readCustom(p2), "readCustom true");
	SBT_ASSERT(p2.x == 10 && p2.y == 20 && p2.z == 30, "custom values");
}

void SerializerTest::testSB_PlayerInfoRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	TestPlayerInfo pi;
	pi.id = 12345;
	pi.hp = 98.5f; 
	pi.name = "Alice";
	SBT_ASSERT(sw.writeCustom(pi), "writePlayerInfo true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	TestPlayerInfo out;
	SBT_ASSERT(sr.readCustom(out), "readPlayerInfo true");
	SBT_ASSERT(out.id == 12345, "playerInfo id");
	SBT_ASSERT(fabsf(out.hp - 98.5f) < 1e-4f, "playerInfo hp");
	SBT_ASSERT(out.name == "Alice", "playerInfo name");
}

void SerializerTest::testSB_CustomListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(512);
	Vector<TestPoint> src;
	FOR(5)
	{
		TestPoint p; p.x = i; p.y = i * 2; p.z = i * 3;
		src.add(p);
	}
	SBT_ASSERT(sw.writeCustomList(src), "writeCustomList true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<TestPoint> dst;
	SBT_ASSERT(sr.readCustomList(dst), "readCustomList true");
	SBT_ASSERT(dst.size() == 5, "customList size==5");
	FOR(5)
	{
		SBT_ASSERT(dst[i].x == i && dst[i].y == i * 2 && dst[i].z == i * 3, "customList[i]");
	}
}

void SerializerTest::testSB_EmptyCustomList()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	Vector<TestPoint> empty;
	sw.writeCustomList(empty);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<TestPoint> dst;
	SBT_ASSERT(sr.readCustomList(dst), "emptyCustomList read true");
	SBT_ASSERT(dst.isEmpty(), "emptyCustomList empty");
}

void SerializerTest::testSB_BufferRoundTrip()
{
	using namespace SerializerByteTestSupport;
	const char raw[] = "Hello, Buffer!";
	const int rawLen = (int)strlen(raw);

	SerializerWrite sw;
	sw.initCapacity(256);
	SBT_ASSERT(sw.writeBuffer(raw, rawLen), "writeBuffer true");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	char out[64] = {};
	SBT_ASSERT(sr.readBuffer(out, 64, rawLen), "readBuffer true");
	SBT_ASSERT(memcmp(out, raw, rawLen) == 0, "buffer content match");
}

void SerializerTest::testSB_BufferNoCopy()
{
	using namespace SerializerByteTestSupport;
	const char raw[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.writeBuffer(raw, 5);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	const char* ptr = nullptr;
	SBT_ASSERT(sr.readBufferNoCopy(ptr, 5), "readBufferNoCopy true");
	SBT_ASSERT(ptr != nullptr, "readBufferNoCopy non-null");
	SBT_ASSERT(memcmp(ptr, raw, 5) == 0, "bufferNoCopy content");
}

void SerializerTest::testSB_LargeBuffer()
{
	using namespace SerializerByteTestSupport;
	const int N = 4096;
	Vector<byte> src;
	FOR(N) { src.add((byte)(i & 0xFF)); }

	SerializerWrite sw;
	sw.initCapacity(N + 16);
	sw.writeBuffer((const char*)src.data(), N);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<byte> dst;
	dst.resize(N);
	SBT_ASSERT(sr.readBuffer((char*)dst.data(), N, N), "largeBuffer read true");
	FOR(N)
	{
		SBT_ASSERT(dst[i] == (byte)(i & 0xFF), "largeBuffer byte[i]");
	}
}

void SerializerTest::testSB_SetIndex()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(10);
	sw.write<int>(20);
	sw.write<int>(30);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	// 跳过第一个 int
	sr.setIndex(sizeof(int));
	int v;
	SBT_ASSERT(sr.read(v) && v == 20, "setIndex read at 1==20");
	SBT_ASSERT(sr.getIndex() == 2 * (int)sizeof(int), "setIndex after read");
}

void SerializerTest::testSB_GetIndex()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(1);
	sw.write<int>(2); 
	sw.write<int>(3);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	SBT_ASSERT(sr.getIndex() == 0, "getIndex init==0");
	int v;
	sr.read(v);
	SBT_ASSERT(sr.getIndex() == (int)sizeof(int), "getIndex after 1 read");
	sr.read(v); 
	sr.read(v);
	SBT_ASSERT(sr.getIndex() == 3 * (int)sizeof(int), "getIndex after 3 reads");
}

void SerializerTest::testSW_SetIndex()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(100);
	sw.write<int>(200);
	// 倒回重写第一个
	sw.setIndex(0);
	sw.write<int>(999);
	sw.setIndex(2 * (int)sizeof(int));

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	int v1;
	int v2;
	SBT_ASSERT(sr.read(v1) && v1 == 999, "sw.setIndex overwrite v1==999");
	SBT_ASSERT(sr.read(v2) && v2 == 200, "sw.setIndex v2==200");
}

void SerializerTest::testSB_InitCapacity()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(512);
	SBT_ASSERT(sw.getBufferSize() >= 512, "initCapacity bufferSize>=512");
	SBT_ASSERT(sw.getDataSize() == 0, "initCapacity dataSize==0");
}

void SerializerTest::testSB_Clear()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(1); sw.write<int>(2); sw.write<int>(3);
	SBT_ASSERT(sw.getDataSize() == 3 * (int)sizeof(int), "beforeClear dataSize");
	sw.clear();
	SBT_ASSERT(sw.getDataSize() == 0, "afterClear dataSize==0");
}

void SerializerTest::testSB_GetBuffer()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(64);
	sw.write<int>(42);
	const char* buf = sw.getBuffer();
	SBT_ASSERT(buf != nullptr, "getBuffer non-null");

	SerializerRead sr(buf, sw.getDataSize());
	SBT_ASSERT(sr.getBuffer() == buf, "sr.getBuffer == sw.getBuffer");
	SBT_ASSERT(sr.getBufferSize() == sw.getDataSize(), "sr.getBufferSize == sw.getDataSize");
}

void SerializerTest::testSB_MixedTypes()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(512);
	sw.write<int>(42);
	sw.write<float>(3.14f);
	sw.writeString("mixed");
	sw.write<bool>(true);
	sw.write<llong>(9876543210LL);
	Vector2 v2 = { 1.0f, 2.0f };
	sw.writeVector2(v2);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	int i; 
	float f; 
	string s; 
	bool b; 
	llong ll; 
	Vector2 rv2;
	SBT_ASSERT(sr.read(i) && i == 42, "mixed int");
	SBT_ASSERT(sr.read(f) && fabsf(f - 3.14f) < 1e-4f, "mixed float");
	SBT_ASSERT(sr.readString(s) && s == "mixed", "mixed string");
	SBT_ASSERT(sr.read(b) && b == true, "mixed bool");
	SBT_ASSERT(sr.read(ll) && ll == 9876543210LL, "mixed llong");
	SBT_ASSERT(sr.readVector2(rv2), "mixed vec2 read");
	SBT_ASSERT(fabsf(rv2.x - 1.0f) < 1e-5f && fabsf(rv2.y - 2.0f) < 1e-5f, "mixed vec2 vals");
}

void SerializerTest::testSB_NestedCustomObjects()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(512);
	TestPoint p1; 
	p1.x = 100; 
	p1.y = 200; 
	p1.z = 300;
	TestPoint p2; 
	p2.x = -1;  
	p2.y = -2;  
	p2.z = -3;
	TestPlayerInfo pi;
	pi.id = 1;
	pi.hp = 50.0f;
	pi.name = "Bob";

	sw.writeCustom(p1);
	sw.writeCustom(p2);
	sw.writeCustom(pi);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	TestPoint op1, op2;
	TestPlayerInfo opi;
	SBT_ASSERT(sr.readCustom(op1), "nested p1 read");
	SBT_ASSERT(op1.x == 100 && op1.y == 200 && op1.z == 300, "nested p1 vals");
	SBT_ASSERT(sr.readCustom(op2), "nested p2 read");
	SBT_ASSERT(op2.x == -1 && op2.y == -2 && op2.z == -3, "nested p2 vals");
	SBT_ASSERT(sr.readCustom(opi), "nested pi read");
	SBT_ASSERT(opi.id == 1 && fabsf(opi.hp - 50.0f) < 1e-4f && opi.name == "Bob", "nested pi vals");
}

void SerializerTest::testSB_MultipleListsInterleaved()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(1024);
	Vector<int> ints;
	ints.add(1, 2, 3); 
	Vector<float> floats;
	floats.add(1.1f, 2.2f);
	Vector<string> strs;
	strs.add("a", "b", "c", "d");

	sw.writeList(ints);
	sw.writeList(floats);
	sw.writeStringList(strs);

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<int> ri;
	Vector<float> rf;
	Vector<string> rs;
	SBT_ASSERT(sr.readList(ri) && ri.size() == 3, "interleaved intList");
	SBT_ASSERT(sr.readList(rf) && rf.size() == 2, "interleaved floatList");
	SBT_ASSERT(sr.readStringList(rs) && rs.size() == 4, "interleaved strList");
	SBT_ASSERT(ri[2] == 3, "interleaved int val");
	SBT_ASSERT(fabsf(rf[1] - 2.2f) < 1e-4f, "interleaved float val");
	SBT_ASSERT(rs[3] == "d", "interleaved str val");
}

void SerializerTest::testSB_ClearAndReuse()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	sw.write<int>(111);
	sw.write<int>(222);
	sw.clear();

	sw.write<int>(999);
	SBT_ASSERT(sw.getDataSize() == (int)sizeof(int), "clearReuse dataSize==4");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	int v;
	SBT_ASSERT(sr.read(v) && v == 999, "clearReuse value==999");
}

void SerializerTest::testSB_LargeVolume()
{
	using namespace SerializerByteTestSupport;
	const int N = 2000;
	SerializerWrite sw;
	sw.initCapacity(N * 4 + 64);
	FOR(N) { sw.write<int>(i); }
	SBT_ASSERT(sw.getDataSize() == N * (int)sizeof(int), "largeVolume dataSize");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	FOR(N)
	{
		int v;
		SBT_ASSERT(sr.read(v) && v == i, "largeVolume val[i]");
	}
}

void SerializerTest::testSB_StringCharBuffer()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	sw.writeString("char_buf_test");

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	char buf[64] = {};
	SBT_ASSERT(sr.readString(buf, 64), "readStringCharBuf true");
	SBT_ASSERT(strcmp(buf, "char_buf_test") == 0, "charBuf value match");
}

void SerializerTest::testSB_Vector2ListRoundTrip()
{
	using namespace SerializerByteTestSupport;
	SerializerWrite sw;
	sw.initCapacity(256);
	Vector<Vector2> src;
	FOR(5)
	{
		src.add({ (float)i, (float)(i * 2) });
	}
	// 手动写 count + 数据
	sw.write<int>(src.size());
	for (const Vector2& v : src) 
	{
		sw.writeVector2(v); 
	}

	SerializerRead sr(sw.getBuffer(), sw.getDataSize());
	Vector<Vector2> dst;
	SBT_ASSERT(sr.readVector2List(dst), "readVec2List true");
	SBT_ASSERT(dst.size() == 5, "vec2List size==5");
	FOR(5)
	{
		SBT_ASSERT(fabsf(dst[i].x - i) < 1e-5f, "vec2List x");
		SBT_ASSERT(fabsf(dst[i].y - i * 2) < 1e-5f, "vec2List y");
	}
}

namespace SerializerByteExt2TestSupport
{
#define SBE2_ASSERT(expr) if (!(expr)) { ERROR(string("SerializerByteExt2Test FAILED: ") + (#expr)); }
#define SBE2_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)

	static const int SBE2_BUF = 2048;

	// ========== Vector3Int round-trip ==========





	// ========== readBuffer ==========



	// ========== setIndex / getIndex ==========


	// ========== getBufferSize ==========


	// ========== read<T>() return value form ==========


	// ========== multiple types sequence ==========


	// ========== ushort round-trip ==========


	// ========== uint round-trip ==========


	// ========== Vector2UInt, Vector2Short, Vector2UShort ==========




	// ========== int list with boundary values ==========


	// ========== float list boundary ==========


	// ========== double round-trip ==========



	// ========== bool list ==========
}

void SerializerTest::testVector3IntRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector3Int src(10, -20, 30);
	SBE2_ASSERT(w.write(src.x));
	SBE2_ASSERT(w.write(src.y));
	SBE2_ASSERT(w.write(src.z));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector3Int dst;
	SBE2_ASSERT(r.readVector3Int(dst));
	SBE2_ASSERT(dst.x == 10);
	SBE2_ASSERT(dst.y == -20);
	SBE2_ASSERT(dst.z == 30);
}

void SerializerTest::testVector3IntZero()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector3Int src(0, 0, 0);
	SBE2_ASSERT(w.write(src.x));
	SBE2_ASSERT(w.write(src.y));
	SBE2_ASSERT(w.write(src.z));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector3Int dst;
	SBE2_ASSERT(r.readVector3Int(dst));
	SBE2_ASSERT(dst.x == 0);
	SBE2_ASSERT(dst.y == 0);
	SBE2_ASSERT(dst.z == 0);
}

void SerializerTest::testVector3IntListRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector<Vector3Int> src;
	src.add(Vector3Int(1, 2, 3));
	src.add(Vector3Int(-1, -2, -3));
	src.add(Vector3Int(100, 200, 300));
	int srcCount = (int)src.size();
	SBE2_ASSERT(w.write(srcCount));
	FOR(srcCount)
	{
		SBE2_ASSERT(w.writeVector3(src[i]));
	}
	SerializerRead r(w.getBuffer(), w.getDataSize());
	int dstCount = 0;
	SBE2_ASSERT(r.read(dstCount));
	SBE2_ASSERT(dstCount == 3);
	Vector<Vector3Int> dst;
	FOR(dstCount)
	{
		Vector3Int v;
		SBE2_ASSERT(r.readVector3Int(v));
		dst.add(v);
	}
	SBE2_ASSERT(dst[0].x == 1 && dst[0].y == 2 && dst[0].z == 3);
	SBE2_ASSERT(dst[1].x == -1 && dst[1].y == -2 && dst[1].z == -3);
	SBE2_ASSERT(dst[2].x == 100 && dst[2].y == 200 && dst[2].z == 300);
}

void SerializerTest::testVector3IntListEmpty()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	int srcCount = 0;
	SBE2_ASSERT(w.write(srcCount));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	int dstCount = 1;
	SBE2_ASSERT(r.read(dstCount));
	SBE2_ASSERT(dstCount == 0);
}

void SerializerTest::testReadBufferFixed()
{
	using namespace SerializerByteExt2TestSupport;
	const char data[] = "HELLO";
	int dataLen = 5;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.writeBuffer(data, dataLen));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	char outBuf[16] = {};
	SBE2_ASSERT(r.readBuffer(outBuf, (int)sizeof(outBuf), dataLen));
	SBE2_ASSERT(outBuf[0] == 'H');
	SBE2_ASSERT(outBuf[1] == 'E');
	SBE2_ASSERT(outBuf[4] == 'O');
}

void SerializerTest::testReadBufferNoCopy()
{
	using namespace SerializerByteExt2TestSupport;
	const char data[] = { 0x01, 0x02, 0x03, 0x04 };
	int dataLen = 4;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.writeBuffer(data, dataLen));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	const char* ptr = nullptr;
	SBE2_ASSERT(r.readBufferNoCopy(ptr, dataLen));
	SBE2_ASSERT(ptr != nullptr);
	SBE2_ASSERT(ptr[0] == 0x01);
	SBE2_ASSERT(ptr[1] == 0x02);
	SBE2_ASSERT(ptr[3] == 0x04);
}

void SerializerTest::testSetIndexGetIndex()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.write((int)42));
	SBE2_ASSERT(w.write((int)99));
	// getDataSize should be 8
	int dataSize = w.getDataSize();
	SBE2_ASSERT(dataSize == 8);
	// reset index and re-read first int
	SerializerRead r(w.getBuffer(), dataSize);
	int v0 = 0;
	SBE2_ASSERT(r.read(v0));
	SBE2_ASSERT(v0 == 42);
	SBE2_ASSERT(r.getIndex() == 4);
	// setIndex back to 0 and re-read
	r.setIndex(0);
	SBE2_ASSERT(r.getIndex() == 0);
	int v1 = 0;
	SBE2_ASSERT(r.read(v1));
	SBE2_ASSERT(v1 == 42);
}

void SerializerTest::testGetBufferSize()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(512);
	SBE2_ASSERT(w.getBufferSize() == 512);
	SBE2_ASSERT(w.write((int)10));
	SBE2_ASSERT(w.getBufferSize() == 512);
	SerializerRead r(w.getBuffer(), 512);
	SBE2_ASSERT(r.getBufferSize() == 512);
}

void SerializerTest::testReadReturnValueTemplate()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.write((float)3.14f));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	float v = r.read<float>();
	SBE2_ASSERT(SBE2_NEAR(v, 3.14f));
}

void SerializerTest::testMultipleTypes()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.write((int)100));
	SBE2_ASSERT(w.write((float)1.5f));
	SBE2_ASSERT(w.write((bool)true));
	SBE2_ASSERT(w.write((llong)1234567890LL));
	SBE2_ASSERT(w.writeString("abc"));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	int vi = 0;
	float vf = 0.0f;
	bool vb = false;
	llong vl = 0;
	string vs;
	SBE2_ASSERT(r.read(vi));
	SBE2_ASSERT(r.read(vf));
	SBE2_ASSERT(r.read(vb));
	SBE2_ASSERT(r.read(vl));
	SBE2_ASSERT(r.readString(vs));
	SBE2_ASSERT(vi == 100);
	SBE2_ASSERT(SBE2_NEAR(vf, 1.5f));
	SBE2_ASSERT(vb == true);
	SBE2_ASSERT(vl == 1234567890LL);
	SBE2_ASSERT(vs == "abc");
}

void SerializerTest::testUShortRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.write((ushort)65535));
	SBE2_ASSERT(w.write((ushort)0));
	SBE2_ASSERT(w.write((ushort)1000));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	ushort v0 = 0;
	ushort v1 = 1;
	ushort v2 = 2;
	SBE2_ASSERT(r.read(v0));
	SBE2_ASSERT(r.read(v1));
	SBE2_ASSERT(r.read(v2));
	SBE2_ASSERT(v0 == 65535);
	SBE2_ASSERT(v1 == 0);
	SBE2_ASSERT(v2 == 1000);
}

void SerializerTest::testUIntRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.write((uint)4294967295u));
	SBE2_ASSERT(w.write((uint)0));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	uint v0 = 0;
	uint v1 = 1;
	SBE2_ASSERT(r.read(v0));
	SBE2_ASSERT(r.read(v1));
	SBE2_ASSERT(v0 == 4294967295u);
	SBE2_ASSERT(v1 == 0);
}

void SerializerTest::testVector2UIntRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	// use generic write since no dedicated writeVector2UInt
	Vector2UInt src;
	src.x = 100;
	src.y = 200;
	SBE2_ASSERT(w.write(src.x));
	SBE2_ASSERT(w.write(src.y));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector2UInt dst;
	SBE2_ASSERT(r.read(dst.x));
	SBE2_ASSERT(r.read(dst.y));
	SBE2_ASSERT(dst.x == 100);
	SBE2_ASSERT(dst.y == 200);
}

void SerializerTest::testVector2ShortRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector2Short src;
	src.x = -32000;
	src.y = 32000;
	SBE2_ASSERT(w.write(src.x));
	SBE2_ASSERT(w.write(src.y));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector2Short dst;
	SBE2_ASSERT(r.read(dst.x));
	SBE2_ASSERT(r.read(dst.y));
	SBE2_ASSERT(dst.x == -32000);
	SBE2_ASSERT(dst.y == 32000);
}

void SerializerTest::testVector2UShortRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector2UShort src;
	src.x = 0;
	src.y = 65535;
	SBE2_ASSERT(w.write(src.x));
	SBE2_ASSERT(w.write(src.y));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector2UShort dst;
	SBE2_ASSERT(r.read(dst.x));
	SBE2_ASSERT(r.read(dst.y));
	SBE2_ASSERT(dst.x == 0);
	SBE2_ASSERT(dst.y == 65535);
}

void SerializerTest::testIntListMixed()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector<int> src;
	src.add(0);
	src.add(-1);
	src.add(2147483647);
	src.add(-2147483647);
	src.add(1);
	SBE2_ASSERT(w.writeList(src));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector<int> dst;
	SBE2_ASSERT(r.readList(dst));
	SBE2_ASSERT((int)dst.size() == 5);
	SBE2_ASSERT(dst[0] == 0);
	SBE2_ASSERT(dst[1] == -1);
	SBE2_ASSERT(dst[2] == 2147483647);
	SBE2_ASSERT(dst[3] == -2147483647);
	SBE2_ASSERT(dst[4] == 1);
}

void SerializerTest::testFloatListBoundary()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector<float> src;
	src.add(0.0f);
	src.add(-0.0f);
	src.add(1e10f);
	src.add(-1e10f);
	SBE2_ASSERT(w.writeList(src));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector<float> dst;
	SBE2_ASSERT(r.readList(dst));
	SBE2_ASSERT((int)dst.size() == 4);
	SBE2_ASSERT(SBE2_NEAR(dst[2], 1e10f));
	SBE2_ASSERT(SBE2_NEAR(dst[3], -1e10f));
}

void SerializerTest::testDoubleRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	SBE2_ASSERT(w.write((double)3.14159265358979));
	SBE2_ASSERT(w.write((double)0.0));
	SBE2_ASSERT(w.write((double)-1.0));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	double v0 = 0.0;
	double v1 = 0.0;
	double v2 = 0.0;
	SBE2_ASSERT(r.read(v0));
	SBE2_ASSERT(r.read(v1));
	SBE2_ASSERT(r.read(v2));
	SBE2_ASSERT(fabsf((float)(v0 - 3.14159265358979)) < 1e-6f);
	SBE2_ASSERT(v1 == 0.0);
	SBE2_ASSERT(v2 == -1.0);
}

void SerializerTest::testDoubleListRoundTrip()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector<double> src;
	src.add(1.1);
	src.add(2.2);
	src.add(3.3);
	SBE2_ASSERT(w.writeList(src));
	SerializerRead r(w.getBuffer(), w.getDataSize());
	Vector<double> dst;
	SBE2_ASSERT(r.readList(dst));
	SBE2_ASSERT((int)dst.size() == 3);
	SBE2_ASSERT(fabsf((float)(dst[0] - 1.1)) < 1e-6f);
	SBE2_ASSERT(fabsf((float)(dst[1] - 2.2)) < 1e-6f);
	SBE2_ASSERT(fabsf((float)(dst[2] - 3.3)) < 1e-6f);
}

void SerializerTest::testBoolList()
{
	using namespace SerializerByteExt2TestSupport;
	SerializerWrite w;
	w.initCapacity(SBE2_BUF);
	Vector<bool> src;
	src.add(true);
	src.add(false);
	src.add(true);
	SBE2_ASSERT(w.writeList(src));
	SerializerRead r(w.getBuffer(), w.getDataSize());
}

namespace SerializerBitExt2TestSupport
{
#define SBE2_MSG_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("SerializerBitExt2Test FAILED: ") + (msg)); } } while(0)

#define SBE2_F(a, b, msg) \
    do { if (fabsf((a) - (b)) > 1e-2f) { ERROR(string("SerializerBitExt2Test FLOAT: ") + (msg) \
        + " exp=" + FToS(b) + " got=" + FToS(a)); } } while(0)
}

void SerializerTest::testWriteReadVector2()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector2 v(3.14f, -2.71f);
	SBE2_MSG_ASSERT(w.writeVector2(true, v), "writeVector2 ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2 vOut;
	SBE2_MSG_ASSERT(r.readVector2(true, vOut), "readVector2 ok");
	SBE2_F(vOut.x, v.x, "Vector2 x");
	SBE2_F(vOut.y, v.y, "Vector2 y");
}

void SerializerTest::testWriteReadVector2Int()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector2Int v(100, -200);
	SBE2_MSG_ASSERT(w.writeVector2Int(true, v), "writeVector2Int ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2Int vOut;
	SBE2_MSG_ASSERT(r.readVector2Int(true, vOut), "readVector2Int ok");
	SBE2_MSG_ASSERT(vOut.x == 100, "Vector2Int x");
	SBE2_MSG_ASSERT(vOut.y == -200, "Vector2Int y");
}

void SerializerTest::testWriteReadVector2UInt()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector2UInt v;
	v.x = 12345;
	v.y = 67890;
	SBE2_MSG_ASSERT(w.writeVector2UInt(v), "writeVector2UInt ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2UInt vOut;
	SBE2_MSG_ASSERT(r.readVector2UInt(vOut), "readVector2UInt ok");
	SBE2_MSG_ASSERT(vOut.x == 12345, "Vector2UInt x");
	SBE2_MSG_ASSERT(vOut.y == 67890, "Vector2UInt y");
}

void SerializerTest::testWriteReadVector2Short()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector2Short v((short)500, (short)-300);
	SBE2_MSG_ASSERT(w.writeVector2Short(true, v), "writeVector2Short ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2Short vOut;
	SBE2_MSG_ASSERT(r.readVector2Short(true, vOut), "readVector2Short ok");
	SBE2_MSG_ASSERT(vOut.x == 500, "Vector2Short x");
	SBE2_MSG_ASSERT(vOut.y == -300, "Vector2Short y");
}

void SerializerTest::testWriteReadVector2UShort()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector2UShort v;
	v.x = (ushort)1000;
	v.y = (ushort)2000;
	SBE2_MSG_ASSERT(w.writeVector2UShort(v), "writeVector2UShort ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2UShort vOut;
	SBE2_MSG_ASSERT(r.readVector2UShort(vOut), "readVector2UShort ok");
	SBE2_MSG_ASSERT(vOut.x == 1000, "Vector2UShort x");
	SBE2_MSG_ASSERT(vOut.y == 2000, "Vector2UShort y");
}

void SerializerTest::testWriteReadVector3()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector3 v(1.5f, -2.5f, 3.5f);
	SBE2_MSG_ASSERT(w.writeVector3(true, v), "writeVector3 ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector3 vOut;
	SBE2_MSG_ASSERT(r.readVector3(true, vOut), "readVector3 ok");
	SBE2_F(vOut.x, v.x, "Vector3 x");
	SBE2_F(vOut.y, v.y, "Vector3 y");
	SBE2_F(vOut.z, v.z, "Vector3 z");
}

void SerializerTest::testWriteReadVector4()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector4 v;
	v.x = 1.0f;
	v.y = -1.0f;
	v.z = 2.0f;
	v.w = -2.0f;
	SBE2_MSG_ASSERT(w.writeVector4(true, v), "writeVector4 ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector4 vOut;
	SBE2_MSG_ASSERT(r.readVector4(true, vOut), "readVector4 ok");
	SBE2_F(vOut.x, v.x, "Vector4 x");
	SBE2_F(vOut.y, v.y, "Vector4 y");
	SBE2_F(vOut.z, v.z, "Vector4 z");
	SBE2_F(vOut.w, v.w, "Vector4 w");
}

void SerializerTest::testWriteReadBoolTrue()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(16);
	SBE2_MSG_ASSERT(w.writeBool(true), "writeBool true");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	bool val = false;
	SBE2_MSG_ASSERT(r.readBool(val), "readBool ok");
	SBE2_MSG_ASSERT(val == true, "bool == true");
}

void SerializerTest::testWriteReadBoolFalse()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(16);
	SBE2_MSG_ASSERT(w.writeBool(false), "writeBool false");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	bool val = true;
	SBE2_MSG_ASSERT(r.readBool(val), "readBool ok");
	SBE2_MSG_ASSERT(val == false, "bool == false");
}

void SerializerTest::testWriteReadBoolSequence()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	bool seq[] = { true, false, true, true, false, false, true };
	FOR(7)
	{
		w.writeBool(seq[i]);
	}
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	FOR(7)
	{
		bool v = !seq[i];
		SBE2_MSG_ASSERT(r.readBool(v), "readBool seq ok");
		SBE2_MSG_ASSERT(v == seq[i], "bool seq value");
	}
}

void SerializerTest::testWriteReadString()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(128);
	SBE2_MSG_ASSERT(w.writeString("HelloBitStr"), "writeString ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	string s;
	SBE2_MSG_ASSERT(r.readString(s), "readString ok");
	SBE2_MSG_ASSERT(s == "HelloBitStr", "string value");
}

void SerializerTest::testWriteReadStringEmpty()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	SBE2_MSG_ASSERT(w.writeString(""), "writeString empty ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	string s = "notempty";
	SBE2_MSG_ASSERT(r.readString(s), "readString empty ok");
	SBE2_MSG_ASSERT(s == "", "empty string");
}

void SerializerTest::testWriteReadStringList()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(256);
	Vector<string> wList;
	wList.add("alpha");
	wList.add("beta");
	wList.add("gamma");
	SBE2_MSG_ASSERT(w.writeStringList(wList), "writeStringList ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<string> rList;
	SBE2_MSG_ASSERT(r.readStringList(rList), "readStringList ok");
	SBE2_MSG_ASSERT(rList.size() == 3, "stringList size==3");
	SBE2_MSG_ASSERT(rList[0] == "alpha", "stringList[0]");
	SBE2_MSG_ASSERT(rList[1] == "beta", "stringList[1]");
	SBE2_MSG_ASSERT(rList[2] == "gamma", "stringList[2]");
}

void SerializerTest::testWriteReadStringListEmpty()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector<string> wList;
	SBE2_MSG_ASSERT(w.writeStringList(wList), "writeStringList empty ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<string> rList;
	SBE2_MSG_ASSERT(r.readStringList(rList), "readStringList empty ok");
	SBE2_MSG_ASSERT(rList.size() == 0, "empty stringList size==0");
}

void SerializerTest::testSkipToByteEnd()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	w.writeBool(true);
	w.writeBool(false);
	w.writeBool(true);
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	bool v;
	r.readBool(v);
	r.skipToByteEnd();
	SBE2_MSG_ASSERT((r.getBitIndex() % 8) == 0, "skipToByteEnd aligned");
}

void SerializerTest::testGetBitIndex()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	SBE2_MSG_ASSERT(r.getBitIndex() == 0, "initial bitIndex==0");
	w.writeBool(true);
	SerializerBitRead r2(w.getBuffer(), w.getByteCount());
	bool v;
	r2.readBool(v);
	SBE2_MSG_ASSERT(r2.getBitIndex() == 1, "after readBool bitIndex==1");
}

void SerializerTest::testGetReadByteCount()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	w.writeSigned(false, (int)12345);
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	int v = 0;
	r.readSigned(false, v);
	SBE2_MSG_ASSERT(r.getReadByteCount() >= 1, "getReadByteCount >= 1");
	SBE2_MSG_ASSERT(v == 12345, "readSigned value ok");
}

void SerializerTest::testWriteReadBuffer()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	const char src[] = { 0x41, 0x42, 0x43, 0x44 };
	SBE2_MSG_ASSERT(w.writeBuffer(src, 4), "writeBuffer ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	char dst[4] = {};
	SBE2_MSG_ASSERT(r.readBuffer(dst, 4), "readBuffer ok");
	SBE2_MSG_ASSERT(dst[0] == 0x41, "buf[0]");
	SBE2_MSG_ASSERT(dst[1] == 0x42, "buf[1]");
	SBE2_MSG_ASSERT(dst[2] == 0x43, "buf[2]");
	SBE2_MSG_ASSERT(dst[3] == 0x44, "buf[3]");
}

void SerializerTest::testWriteReadFloatList()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(128);
	Vector<float> wList;
	wList.add(1.5f);
	wList.add(-2.5f);
	wList.add(3.0f);
	SBE2_MSG_ASSERT(w.writeFloatList(true, wList), "writeFloatList ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<float> rList;
	SBE2_MSG_ASSERT(r.readFloatList(true, rList), "readFloatList ok");
	SBE2_MSG_ASSERT(rList.size() == 3, "floatList size==3");
	SBE2_F(rList[0], 1.5f, "floatList[0]");
	SBE2_F(rList[1], -2.5f, "floatList[1]");
	SBE2_F(rList[2], 3.0f, "floatList[2]");
}

void SerializerTest::testWriteReadDoubleList()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(128);
	Vector<double> wList;
	wList.add(1.1234);
	wList.add(-5.6789);
	SBE2_MSG_ASSERT(w.writeDoubleList(true, wList), "writeDoubleList ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<double> rList;
	SBE2_MSG_ASSERT(r.readDoubleList(true, rList), "readDoubleList ok");
	SBE2_MSG_ASSERT(rList.size() == 2, "doubleList size==2");
	SBE2_MSG_ASSERT(fabs(rList[0] - 1.1234) < 1e-3, "doubleList[0]");
	SBE2_MSG_ASSERT(fabs(rList[1] - (-5.6789)) < 1e-3, "doubleList[1]");
}

void SerializerTest::testWriteReadMixedTypes()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(256);
	w.writeBool(true);
	w.writeSigned(true, (int)42);
	w.writeUnsigned((uint)100u);
	w.writeFloat(true, 3.14f);
	w.writeString("test");

	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	bool b = false;
	SBE2_MSG_ASSERT(r.readBool(b), "mixed readBool");
	SBE2_MSG_ASSERT(b, "mixed bool==true");
	int vi = 0;
	SBE2_MSG_ASSERT(r.readSigned(true, vi), "mixed readSigned");
	SBE2_MSG_ASSERT(vi == 42, "mixed int==42");
	uint vu = 0;
	SBE2_MSG_ASSERT(r.readUnsigned(vu), "mixed readUnsigned");
	SBE2_MSG_ASSERT(vu == 100u, "mixed uint==100");
	float vf = 0.0f;
	SBE2_MSG_ASSERT(r.readFloat(true, vf), "mixed readFloat");
	SBE2_F(vf, 3.14f, "mixed float");
	string vs;
	SBE2_MSG_ASSERT(r.readString(vs), "mixed readString");
	SBE2_MSG_ASSERT(vs == "test", "mixed string");
}

void SerializerTest::testWriteReadSignedSingle()
{
	using namespace SerializerBitExt2TestSupport;
	for (int val : { -1000, -1, 0, 1, 999, 32767 })
	{
		SerializerBitWrite w;
		w.initCapacity(32);
		w.writeSigned(true, val);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		int out = 0;
		SBE2_MSG_ASSERT(r.readSigned(true, out), "readSigned single ok");
		SBE2_MSG_ASSERT(out == val, "readSigned single value");
	}
}

void SerializerTest::testWriteReadUnsignedSingle()
{
	using namespace SerializerBitExt2TestSupport;
	for (uint val : { 0u, 1u, 255u, 65535u, 1000000u })
	{
		SerializerBitWrite w;
		w.initCapacity(32);
		w.writeUnsigned(val);
		SerializerBitRead r(w.getBuffer(), w.getByteCount());
		uint out = 0;
		SBE2_MSG_ASSERT(r.readUnsigned(out), "readUnsigned single ok");
		SBE2_MSG_ASSERT(out == val, "readUnsigned single value");
	}
}

void SerializerTest::testWriteReadUnsignedList()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(128);
	Vector<uint> wList;
	wList.add(10u);
	wList.add(200u);
	wList.add(3000u);
	wList.add(0u);
	SBE2_MSG_ASSERT(w.writeUnsignedList(wList), "writeUnsignedList ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<uint> rList;
	SBE2_MSG_ASSERT(r.readUnsignedList(rList), "readUnsignedList ok");
	SBE2_MSG_ASSERT(rList.size() == 4, "unsignedList size==4");
	SBE2_MSG_ASSERT(rList[0] == 10u, "uList[0]");
	SBE2_MSG_ASSERT(rList[1] == 200u, "uList[1]");
	SBE2_MSG_ASSERT(rList[2] == 3000u, "uList[2]");
	SBE2_MSG_ASSERT(rList[3] == 0u, "uList[3]");
}

void SerializerTest::testWriteReadSignedList()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(256);
	Vector<int> wList;
	wList.add(-100);
	wList.add(0);
	wList.add(100);
	wList.add(-32767);
	wList.add(32767);
	SBE2_MSG_ASSERT(w.writeSignedList(true, wList), "writeSignedList ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<int> rList;
	SBE2_MSG_ASSERT(r.readSignedList(true, rList), "readSignedList ok");
	SBE2_MSG_ASSERT(rList.size() == 5, "signedList size==5");
	SBE2_MSG_ASSERT(rList[0] == -100, "sList[0]");
	SBE2_MSG_ASSERT(rList[1] == 0, "sList[1]");
	SBE2_MSG_ASSERT(rList[2] == 100, "sList[2]");
	SBE2_MSG_ASSERT(rList[3] == -32767, "sList[3]");
	SBE2_MSG_ASSERT(rList[4] == 32767, "sList[4]");
}

void SerializerTest::testWriteReadDouble()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	SBE2_MSG_ASSERT(w.writeDouble(true, 3.1415926), "writeDouble ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	double v = 0.0;
	SBE2_MSG_ASSERT(r.readDouble(true, v), "readDouble ok");
	SBE2_MSG_ASSERT(fabs(v - 3.1415926) < 1e-3, "double value");
}

void SerializerTest::testWriteReadDoublePrecision()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	double d = -99.9999;
	SBE2_MSG_ASSERT(w.writeDouble(true, d), "writeDouble neg ok");
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	double v = 0.0;
	SBE2_MSG_ASSERT(r.readDouble(true, v), "readDouble neg ok");
	SBE2_MSG_ASSERT(fabs(v - d) < 1e-2, "double neg value");
}

void SerializerTest::testMultipleStrings()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(512);
	const char* strs[] = { "first", "second", "third", "fourth", "fifth" };
	FOR(5)
	{
		SBE2_MSG_ASSERT(w.writeString(strs[i]), "write multi string");
	}
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	FOR(5)
	{
		string s;
		SBE2_MSG_ASSERT(r.readString(s), "read multi string");
		SBE2_MSG_ASSERT(s == strs[i], "multi string value");
	}
}

void SerializerTest::testVector3CloseToZero()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector3 v(0.001f, 0.0f, -0.001f);
	w.writeVector3(true, v);
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector3 vOut;
	SBE2_MSG_ASSERT(r.readVector3(true, vOut), "readVector3 near zero ok");
	SBE2_F(vOut.y, 0.0f, "Vector3 near-zero y");
}

void SerializerTest::testVector2IntNegative()
{
	using namespace SerializerBitExt2TestSupport;
	SerializerBitWrite w;
	w.initCapacity(64);
	Vector2Int v(-999, -12345);
	w.writeVector2Int(true, v);
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2Int vOut;
	SBE2_MSG_ASSERT(r.readVector2Int(true, vOut), "readVector2Int neg ok");
	SBE2_MSG_ASSERT(vOut.x == -999, "Vector2Int neg x");
	SBE2_MSG_ASSERT(vOut.y == -12345, "Vector2Int neg y");
}

namespace SerializerBitExt3TestSupport
{
#define SBE3_ASSERT(expr) \
	if (!(expr)) { ERROR(string("SerializerBitExt3Test FAILED: ") + (#expr)); }

#define SBE3_NEAR(a, b) (fabsf((float)((a) - (b))) < 1e-3f)

	static const int SBE3_BUF = 4096;

	// ========== writeUnsigned single ==========


	// ========== writeUnsignedList ==========


	// ========== writeSignedList ==========


	// ========== writeDoubleList ==========


	// ========== writeBuffer ==========


	// ========== writeUnsigned multi-arg ==========


	// ========== writeSigned multi-arg ==========


	// ========== writeFloatList ==========


	// ========== writeStringList ==========


	// ========== writeBool sequence ==========


	// ========== getBitCount / getByteCount ==========



	// ========== Vector2UInt / Vector2Short / Vector2UShort ==========




	// ========== boundary values ==========
}

void SerializerTest::testWriteReadUnsignedSingleBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeUnsigned((uint)12345));
	SBE3_ASSERT(w.writeUnsigned((ushort)60000));
	SBE3_ASSERT(w.writeUnsigned((byte)255));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	uint v0 = 0;
	ushort v1 = 0;
	byte v2 = 0;
	SBE3_ASSERT(r.readUnsigned(v0));
	SBE3_ASSERT(r.readUnsigned(v1));
	SBE3_ASSERT(r.readUnsigned(v2));
	SBE3_ASSERT(v0 == 12345);
	SBE3_ASSERT(v1 == 60000);
	SBE3_ASSERT(v2 == 255);
}

void SerializerTest::testWriteReadUnsignedListBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector<uint> src;
	src.add(0);
	src.add(100);
	src.add(4294967295u);
	SBE3_ASSERT(w.writeUnsignedList(src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<uint> dst;
	SBE3_ASSERT(r.readUnsignedList(dst));
	SBE3_ASSERT((int)dst.size() == 3);
	SBE3_ASSERT(dst[0] == 0);
	SBE3_ASSERT(dst[1] == 100);
	SBE3_ASSERT(dst[2] == 4294967295u);
}

void SerializerTest::testWriteReadSignedListBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector<int> src;
	src.add(0);
	src.add(100);
	src.add(-100);
	src.add(2147483647);
	src.add(-2147483647);
	SBE3_ASSERT(w.writeSignedList(true, src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<int> dst;
	SBE3_ASSERT(r.readSignedList(true, dst));
	SBE3_ASSERT((int)dst.size() == 5);
	SBE3_ASSERT(dst[0] == 0);
	SBE3_ASSERT(dst[1] == 100);
	SBE3_ASSERT(dst[2] == -100);
	SBE3_ASSERT(dst[3] == 2147483647);
	SBE3_ASSERT(dst[4] == -2147483647);
}

void SerializerTest::testWriteReadDoubleListBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector<double> src;
	src.add(0.0);
	src.add(1.2345);
	src.add(-9.876);
	SBE3_ASSERT(w.writeDoubleList(true, src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<double> dst;
	SBE3_ASSERT(r.readDoubleList(true, dst));
	SBE3_ASSERT((int)dst.size() == 3);
	SBE3_ASSERT(SBE3_NEAR(dst[0], 0.0));
	SBE3_ASSERT(SBE3_NEAR(dst[1], 1.2345));
	SBE3_ASSERT(SBE3_NEAR(dst[2], -9.876));
}

void SerializerTest::testWriteReadBufferBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	const char data[] = { 0x10, 0x20, 0x30, 0x40 };
	SBE3_ASSERT(w.writeBuffer(data, 4));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	char dst[8] = {};
	SBE3_ASSERT(r.readBuffer(dst, 4));
	SBE3_ASSERT(dst[0] == 0x10);
	SBE3_ASSERT(dst[1] == 0x20);
	SBE3_ASSERT(dst[2] == 0x30);
	SBE3_ASSERT(dst[3] == 0x40);
}

void SerializerTest::testWriteReadUnsignedMultiArg()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeUnsigned((uint)1, (uint)2));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	uint v0 = 0;
	uint v1 = 0;
	SBE3_ASSERT(r.readUnsigned(v0, v1));
	SBE3_ASSERT(v0 == 1);
	SBE3_ASSERT(v1 == 2);
}

void SerializerTest::testWriteReadSignedMultiArg()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeSigned(true, (int)10, (int)-20));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	int v0 = 0;
	int v1 = 0;
	SBE3_ASSERT(r.readSigned(true, v0, v1));
	SBE3_ASSERT(v0 == 10);
	SBE3_ASSERT(v1 == -20);
}

void SerializerTest::testWriteReadFloatListBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector<float> src;
	src.add(0.0f);
	src.add(1.5f);
	src.add(-2.25f);
	src.add(100.0f);
	SBE3_ASSERT(w.writeFloatList(true, src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<float> dst;
	SBE3_ASSERT(r.readFloatList(true, dst));
	SBE3_ASSERT((int)dst.size() == 4);
	SBE3_ASSERT(SBE3_NEAR(dst[0], 0.0f));
	SBE3_ASSERT(SBE3_NEAR(dst[1], 1.5f));
	SBE3_ASSERT(SBE3_NEAR(dst[2], -2.25f));
	SBE3_ASSERT(SBE3_NEAR(dst[3], 100.0f));
}

void SerializerTest::testWriteReadStringListBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector<string> src;
	src.add("alpha");
	src.add("beta");
	src.add("");
	src.add("gamma");
	SBE3_ASSERT(w.writeStringList(src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector<string> dst;
	SBE3_ASSERT(r.readStringList(dst));
	SBE3_ASSERT((int)dst.size() == 4);
	SBE3_ASSERT(dst[0] == "alpha");
	SBE3_ASSERT(dst[1] == "beta");
	SBE3_ASSERT(dst[2] == "");
	SBE3_ASSERT(dst[3] == "gamma");
}

void SerializerTest::testWriteReadBoolSequenceBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeBool(true));
	SBE3_ASSERT(w.writeBool(false));
	SBE3_ASSERT(w.writeBool(true));
	SBE3_ASSERT(w.writeBool(true));
	SBE3_ASSERT(w.writeBool(false));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	bool b0 = false;
	bool b1 = true;
	bool b2 = false;
	bool b3 = false;
	bool b4 = true;
	SBE3_ASSERT(r.readBool(b0));
	SBE3_ASSERT(r.readBool(b1));
	SBE3_ASSERT(r.readBool(b2));
	SBE3_ASSERT(r.readBool(b3));
	SBE3_ASSERT(r.readBool(b4));
	SBE3_ASSERT(b0 == true);
	SBE3_ASSERT(b1 == false);
	SBE3_ASSERT(b2 == true);
	SBE3_ASSERT(b3 == true);
	SBE3_ASSERT(b4 == false);
}

void SerializerTest::testGetBitCount()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.getBitCount() == 0);
	SBE3_ASSERT(w.writeBool(true));
	SBE3_ASSERT(w.getBitCount() == 1);
	SBE3_ASSERT(w.writeBool(false));
	SBE3_ASSERT(w.getBitCount() == 2);
}

void SerializerTest::testGetByteCount()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.getByteCount() == 0);
	SBE3_ASSERT(w.writeBool(true));
	// 1 bit => 1 byte
	SBE3_ASSERT(w.getByteCount() == 1);
	SBE3_ASSERT(w.writeSigned(true, (int)100));
	int bc = w.getByteCount();
	SBE3_ASSERT(bc > 0);
}

void SerializerTest::testWriteReadVector2UIntBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector2UInt src;
	src.x = 999;
	src.y = 12345;
	SBE3_ASSERT(w.writeVector2UInt(src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2UInt dst;
	SBE3_ASSERT(r.readVector2UInt(dst));
	SBE3_ASSERT(dst.x == 999);
	SBE3_ASSERT(dst.y == 12345);
}

void SerializerTest::testWriteReadVector2ShortBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector2Short src;
	src.x = -1000;
	src.y = 1000;
	SBE3_ASSERT(w.writeVector2Short(true, src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2Short dst;
	SBE3_ASSERT(r.readVector2Short(true, dst));
	SBE3_ASSERT(dst.x == -1000);
	SBE3_ASSERT(dst.y == 1000);
}

void SerializerTest::testWriteReadVector2UShortBitExt3()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	Vector2UShort src;
	src.x = 0;
	src.y = 65535;
	SBE3_ASSERT(w.writeVector2UShort(src));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	Vector2UShort dst;
	SBE3_ASSERT(r.readVector2UShort(dst));
	SBE3_ASSERT(dst.x == 0);
	SBE3_ASSERT(dst.y == 65535);
}

void SerializerTest::testWriteReadUIntBoundary()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeUnsigned((uint)0));
	SBE3_ASSERT(w.writeUnsigned((uint)4294967295u));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	uint v0 = 1;
	uint v1 = 0;
	SBE3_ASSERT(r.readUnsigned(v0));
	SBE3_ASSERT(r.readUnsigned(v1));
	SBE3_ASSERT(v0 == 0);
	SBE3_ASSERT(v1 == 4294967295u);
}

void SerializerTest::testWriteReadUShortBoundary()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeUnsigned((ushort)0));
	SBE3_ASSERT(w.writeUnsigned((ushort)65535));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	ushort v0 = 1;
	ushort v1 = 0;
	SBE3_ASSERT(r.readUnsigned(v0));
	SBE3_ASSERT(r.readUnsigned(v1));
	SBE3_ASSERT(v0 == 0);
	SBE3_ASSERT(v1 == 65535);
}

void SerializerTest::testWriteReadShortSigned()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeSigned(true, (short)0));
	SBE3_ASSERT(w.writeSigned(true, (short)32767));
	SBE3_ASSERT(w.writeSigned(true, (short)-32767));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	short v0 = 1;
	short v1 = 0;
	short v2 = 0;
	SBE3_ASSERT(r.readSigned(true, v0));
	SBE3_ASSERT(r.readSigned(true, v1));
	SBE3_ASSERT(r.readSigned(true, v2));
	SBE3_ASSERT(v0 == 0);
	SBE3_ASSERT(v1 == 32767);
	SBE3_ASSERT(v2 == -32767);
}

void SerializerTest::testWriteReadNegativeLlong()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeSigned(true, (llong)-999999999LL));
	SBE3_ASSERT(w.writeSigned(true, (llong)999999999LL));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	llong v0 = 0;
	llong v1 = 0;
	SBE3_ASSERT(r.readSigned(true, v0));
	SBE3_ASSERT(r.readSigned(true, v1));
	SBE3_ASSERT(v0 == -999999999LL);
	SBE3_ASSERT(v1 == 999999999LL);
}

void SerializerTest::testWriteReadMixedUnsignedSigned()
{
	using namespace SerializerBitExt3TestSupport;
	SerializerBitWrite w;
	w.initCapacity(SBE3_BUF);
	SBE3_ASSERT(w.writeUnsigned((uint)42));
	SBE3_ASSERT(w.writeSigned(true, (int)-7));
	SBE3_ASSERT(w.writeBool(true));
	SBE3_ASSERT(w.writeUnsigned((ushort)500));
	SerializerBitRead r(w.getBuffer(), w.getByteCount());
	uint vu = 0;
	int vi = 0;
	bool vb = false;
	ushort vs = 0;
	SBE3_ASSERT(r.readUnsigned(vu));
	SBE3_ASSERT(r.readSigned(true, vi));
	SBE3_ASSERT(r.readBool(vb));
	SBE3_ASSERT(r.readUnsigned(vs));
	SBE3_ASSERT(vu == 42);
	SBE3_ASSERT(vi == -7);
	SBE3_ASSERT(vb == true);
	SBE3_ASSERT(vs == 500);
}

void SerializerTest::test()
{
	testSxtPodTypes();
	testSxtString();
	testSxtVector2();
	testSxtVector2Int();
	testSxtVector3();
	testSxtVector4();
	testSxtList();
	testSxtMixed();
	testSxtIndex();
	testSxtBuffer();
	testSxtStringList();
	testSxtVectorLists();
	testSxtStress();
	testSxtBoundaryValues();
	testSxtSequentialWrites();
	testSxtLargeString();
	testSxtManySmallInts();
	testSxtBoolArray();
	testSxtStringIntInterleaved();
	testSB_BoolRoundTrip();
	testSB_ByteRoundTrip();
	testSB_ShortRoundTrip();
	testSB_UShortRoundTrip();
	testSB_IntRoundTrip();
	testSB_UIntRoundTrip();
	testSB_LLongRoundTrip();
	testSB_FloatRoundTrip();
	testSB_DoubleRoundTrip();
	testSB_ReadReturnValue();
	testSB_Vector2RoundTrip();
	testSB_Vector2IntRoundTrip();
	testSB_Vector3RoundTrip();
	testSB_Vector4RoundTrip();
	testSB_MultipleVector3();
	testSB_StringRoundTrip();
	testSB_EmptyStringRoundTrip();
	testSB_MultipleStrings();
	testSB_LongString();
	testSB_StringWithStdString();
	testSB_IntListRoundTrip();
	testSB_EmptyListRoundTrip();
	testSB_FloatListRoundTrip();
	testSB_LLongListRoundTrip();
	testSB_StringListRoundTrip();
	testSB_EmptyStringList();
	testSB_CustomRoundTrip();
	testSB_PlayerInfoRoundTrip();
	testSB_CustomListRoundTrip();
	testSB_EmptyCustomList();
	testSB_BufferRoundTrip();
	testSB_BufferNoCopy();
	testSB_LargeBuffer();
	testSB_SetIndex();
	testSB_GetIndex();
	testSW_SetIndex();
	testSB_InitCapacity();
	testSB_Clear();
	testSB_GetBuffer();
	testSB_MixedTypes();
	testSB_NestedCustomObjects();
	testSB_MultipleListsInterleaved();
	testSB_ClearAndReuse();
	testSB_LargeVolume();
	testSB_StringCharBuffer();
	testSB_Vector2ListRoundTrip();
	testVector3IntRoundTrip();
	testVector3IntZero();
	testVector3IntListRoundTrip();
	testVector3IntListEmpty();
	testReadBufferFixed();
	testReadBufferNoCopy();
	testSetIndexGetIndex();
	testGetBufferSize();
	testReadReturnValueTemplate();
	testMultipleTypes();
	testUShortRoundTrip();
	testUIntRoundTrip();
	testVector2UIntRoundTrip();
	testVector2ShortRoundTrip();
	testVector2UShortRoundTrip();
	testIntListMixed();
	testFloatListBoundary();
	testDoubleRoundTrip();
	testDoubleListRoundTrip();
	testBoolList();
	testWriteReadVector2();
	testWriteReadVector2Int();
	testWriteReadVector2UInt();
	testWriteReadVector2Short();
	testWriteReadVector2UShort();
	testWriteReadVector3();
	testWriteReadVector4();
	testWriteReadBoolTrue();
	testWriteReadBoolFalse();
	testWriteReadBoolSequence();
	testWriteReadString();
	testWriteReadStringEmpty();
	testWriteReadStringList();
	testWriteReadStringListEmpty();
	testSkipToByteEnd();
	testGetBitIndex();
	testGetReadByteCount();
	testWriteReadBuffer();
	testWriteReadFloatList();
	testWriteReadDoubleList();
	testWriteReadMixedTypes();
	testWriteReadSignedSingle();
	testWriteReadUnsignedSingle();
	testWriteReadUnsignedList();
	testWriteReadSignedList();
	testWriteReadDouble();
	testWriteReadDoublePrecision();
	testMultipleStrings();
	testVector3CloseToZero();
	testVector2IntNegative();
	testWriteReadUnsignedSingleBitExt3();
	testWriteReadUnsignedListBitExt3();
	testWriteReadSignedListBitExt3();
	testWriteReadDoubleListBitExt3();
	testWriteReadBufferBitExt3();
	testWriteReadUnsignedMultiArg();
	testWriteReadSignedMultiArg();
	testWriteReadFloatListBitExt3();
	testWriteReadStringListBitExt3();
	testWriteReadBoolSequenceBitExt3();
	testGetBitCount();
	testGetByteCount();
	testWriteReadVector2UIntBitExt3();
	testWriteReadVector2ShortBitExt3();
	testWriteReadVector2UShortBitExt3();
	testWriteReadUIntBoundary();
	testWriteReadUShortBoundary();
	testWriteReadShortSigned();
	testWriteReadNegativeLlong();
	testWriteReadMixedUnsignedSigned();
	testBitRoundTrip();
}


namespace SerializerTestBitRoundTripLocal
{
#define SBR_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SerializerTest FAILED: ") + (msg)); } } while(0)
#define SBR_ASSERT_EQ(a, bool_val, msg) \
	do { if ((a) != (bool_val)) { ERROR(string("SerializerTest FAILED: ") + (msg)); } } while(0)
#define SBR_ASSERT_INT(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializerTest FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SBR_ASSERT_FLOAT(a, b, eps, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > (eps)) { ERROR(string("SerializerTest FAILED: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

	// ========== round-trip helper: SerializerBitWrite → SerializerBitRead ==========


	// ========== round-trip helper for signed (needWriteSign parameter) ==========


	// ========== round-trip helper for unsigned ==========


	// ---------- bool ----------


	// ---------- signed int: 典型值 ----------


	// ---------- signed int: 边界值 ----------
	// NOTE: 变长有符号位编码无法表示 INT_MIN/SHRT_MIN/LLONG_MIN,
	//       因为取绝对值后需要的比特数超出长度位容量(sizeof(T)*8 vs 长度位最大值31/15/63),
	//       且 -INT_MIN 本身对有符号整数是未定义行为(溢出)


	// ---------- unsigned int: 典型值 ----------


	// ---------- unsigned int: 最大值 ----------


	// ---------- short / ushort ----------


	// ---------- llong / ullong ----------


	// ---------- byte ----------


	// ---------- float ----------


	// ---------- double ----------
	// NOTE: writeDouble 内部是 roundDouble(value * pow10LLong(precision)) 转成整数存储,
	//       精度固定为 10^{-precision}(默认4位小数)。大数值×10^4可能溢出llong。


	// ---------- float/double: needWriteSign=false ----------


	// ---------- signed int: 多参数列表 ----------


	// ---------- unsigned int: 多参数列表 ----------


	// ---------- string ----------


	// ---------- Vector<int> 列表 round-trip ----------


	// ---------- Vector<uint> 列表 round-trip ----------


	// ---------- Vector<float> 列表 round-trip ----------


	// ---------- Array 固定大小列表 ----------

	// ---------- Vector2 ----------


	// ---------- Vector2Int ----------


	// ---------- Vector2UInt ----------


	// ---------- Vector2Short ----------


	// ---------- Vector2UShort ----------


	// ---------- Vector3 ----------


	// ---------- Vector4 ----------


	// ---------- writeBuffer / readBuffer ----------


	// ---------- readBufferNoCopy ----------


	// ---------- writeCustom / readCustom ----------
	struct TestCustomBit : public SerializableBitData
	{
		int mA = 0;
		int mB = 0;
		bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
		{
			return reader->readSigned(needReadSign, mA) && reader->readSigned(needReadSign, mB);
		}
		bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
		{
			return writer->writeSigned(needWriteSign, mA) && writer->writeSigned(needWriteSign, mB);
		}
	};



	// ---------- writeCustomList / readCustomList ----------


	// ---------- Empty signed/unsigned/float lists ----------


	// ---------- fillZeroToByteEnd / skipToByteEnd ----------


	// ---------- Utility: getByteCount, getBufferSize, getBitIndex, setBitIndex, getReadByteCount, clear ----------


	// ---------- Mixed types in single bit buffer ----------


	// ---------- Double list ----------

}

template<typename T>
bool SerializerTest::bitWriteRead(const T& value, T& outValue,
	                          bool(SerializerBitWrite::*writeFunc)(T),
	                          bool(SerializerBitRead::*readFunc)(T&))
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		if (!(w.*writeFunc)(value))
		{
			return false;
		}

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		return (r.*readFunc)(outValue);
	}

template<typename T>
bool SerializerTest::bitSignedWriteRead(const T& value, T& outValue, bool needSign)
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		if constexpr (is_same<float, T>::value)
		{
			if (!w.writeFloat(needSign, value))
			{
				return false;
			}
		}
		else if constexpr (is_same<double, T>::value)
		{
			if (!w.writeDouble(needSign, value))
			{
				return false;
			}
		}
		else
		{
			if (!w.writeSigned(needSign, value))
			{
				return false;
			}
		}

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		if constexpr (is_same<float, T>::value)
		{
			return r.readFloat(needSign, outValue);
		}
		else if constexpr (is_same<double, T>::value)
		{
			return r.readDouble(needSign, outValue);
		}
		else
		{
			return r.readSigned(needSign, outValue);
		}
	}

template<typename T>
bool SerializerTest::bitUnsignedWriteRead(const T& value, T& outValue)
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		if (!w.writeUnsigned(value))
		{
			return false;
		}

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		return r.readUnsigned(outValue);
	}

void SerializerTest::testRT_Bool()
{
	using namespace SerializerTestBitRoundTripLocal;
		bool val, out;
		val = true;  SBR_ASSERT(bitWriteRead(val, out, &SerializerBitWrite::writeBool, &SerializerBitRead::readBool), "bool true ok");
		SBR_ASSERT(out == true, "bool true value");

		val = false; SBR_ASSERT(bitWriteRead(val, out, &SerializerBitWrite::writeBool, &SerializerBitRead::readBool), "bool false ok");
		SBR_ASSERT(out == false, "bool false value");
	}

void SerializerTest::testRT_SignedInt_Typical()
{
	using namespace SerializerTestBitRoundTripLocal;
		int values[] = {0, 1, -1, 42, -42, 1000, -1000, 99999, -99999};
		for (int v : values)
		{
			int out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "signed int typical write ok");
			SBR_ASSERT_INT(out, v, "signed int typical read match");
		}
	}

void SerializerTest::testRT_SignedInt_Boundary()
{
	using namespace SerializerTestBitRoundTripLocal;
		int boundary[] = {INT_MAX, INT_MAX - 1, INT_MIN + 1};
		for (int v : boundary)
		{
			int out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "signed int boundary write ok");
			SBR_ASSERT_INT(out, v, "signed int boundary read match");
		}
	}

void SerializerTest::testRT_UnsignedInt_Typical()
{
	using namespace SerializerTestBitRoundTripLocal;
		uint values[] = {0u, 1u, 42u, 1000u, 99999u, 4000000000u};
		for (uint v : values)
		{
			uint out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "unsigned int typical write ok");
			SBR_ASSERT((llong)out == (llong)v, "unsigned int typical read match");
		}
	}

void SerializerTest::testRT_UnsignedInt_Max()
{
	using namespace SerializerTestBitRoundTripLocal;
		uint v = UINT_MAX;
		uint out = 0;
		SBR_ASSERT(bitUnsignedWriteRead(v, out), "uint MAX write ok");
		SBR_ASSERT((llong)out == (llong)UINT_MAX, "uint MAX read match");
	}

void SerializerTest::testRT_Short()
{
	using namespace SerializerTestBitRoundTripLocal;
		short svals[] = {0, 1, -1, SHRT_MAX, SHRT_MIN + 1};
		for (short v : svals)
		{
			short out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "short write ok");
			SBR_ASSERT_INT(out, v, "short read match");
		}

		ushort uvals[] = {0, 1, USHRT_MAX};
		for (ushort v : uvals)
		{
			ushort out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "ushort write ok");
			SBR_ASSERT(out == v, "ushort read match");
		}
	}

void SerializerTest::testRT_LLong()
{
	using namespace SerializerTestBitRoundTripLocal;
		llong vals[] = {0LL, 1LL, -1LL, 10000000000LL, -10000000000LL, LLONG_MAX, LLONG_MIN + 1};
		for (llong v : vals)
		{
			llong out = 0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "llong write ok");
			SBR_ASSERT(out == v, "llong read match");
		}

		ullong uvals[] = {0ULL, 1ULL, ULLONG_MAX, 10000000000ULL};
		for (ullong v : uvals)
		{
			ullong out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "ullong write ok");
			SBR_ASSERT(out == v, "ullong read match");
		}
	}

void SerializerTest::testRT_Byte()
{
	using namespace SerializerTestBitRoundTripLocal;
		byte vals[] = {0, 1, 127, 128, 255};
		for (byte v : vals)
		{
			byte out = 0;
			SBR_ASSERT(bitUnsignedWriteRead(v, out), "byte write ok");
			SBR_ASSERT(out == v, "byte read match");
		}
	}

void SerializerTest::testRT_Float()
{
	using namespace SerializerTestBitRoundTripLocal;
		float vals[] = {0.0f, 1.0f, -1.0f, 3.14f, -3.14f, 0.001f, 1000000.0f};
		for (float v : vals)
		{
			float out = 0.0f;
			SBR_ASSERT(bitSignedWriteRead(v, out), "float write ok");
			SBR_ASSERT_FLOAT(out, v, 0.01f * fabsf(v) + 1e-4f, "float read match");
		}
	}

void SerializerTest::testRT_Double()
{
	using namespace SerializerTestBitRoundTripLocal;
		// 小数值：精度损失 ≤ 0.5 × 10^{-4}
		double smallVals[] = {0.0, 1.0, -1.0, 3.14159265358979, -3.14159265358979};
		for (double v : smallVals)
		{
			double out = 0.0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "double write ok");
			double diff = fabs(out - v);
			// 4位小数精度：误差不超过 0.5 × 10^{-4} + 浮点epsilon
			double tol = 0.5e-4 + fabs(v) * 1e-12;
			SBR_ASSERT(diff < tol, "double read match");
		}
		// 大数值（但×10^4后不溢出llong）：验证 round-trip 一致性
		double bigVals[] = {1e8, -1e8, 1e12, -1e12};
		for (double v : bigVals)
		{
			double out = 0.0;
			SBR_ASSERT(bitSignedWriteRead(v, out), "double big write ok");
			// 大数值的小数部分会被完全截断，只验证数量级一致
			double relDiff = (fabs(v) > 0.0) ? fabs(out - v) / fabs(v) : fabs(out);
			SBR_ASSERT(relDiff < 1e-3, "double big read approximate");
		}
	}

void SerializerTest::testRT_Float_NoSign()
{
	using namespace SerializerTestBitRoundTripLocal;
		float v = -3.14f;
		float out = 0.0f;
		SerializerBitWrite w;
		w.initCapacity(256);
		w.writeFloat(false, v); // 无符号写入

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		r.readFloat(false, out);
		// 没符号位时负数会变正数（因为取了 abs），这是预期行为
		SBR_ASSERT(fabsf(out) > 3.0f, "float nosign magnitude preserved");
	}

void SerializerTest::testRT_SignedList()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		int v1 = 100;
		int v2 = -200;
		int v3 = 300;
		SBR_ASSERT(w.writeSigned(true, v1, v2, v3), "signed list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		int o1;
		int o2;
		int o3;
		SBR_ASSERT(r.readSigned(true, o1, o2, o3), "signed list read ok");
		SBR_ASSERT_INT(o1, v1, "list [0]");
		SBR_ASSERT_INT(o2, v2, "list [1]");
		SBR_ASSERT_INT(o3, v3, "list [2]");
	}

void SerializerTest::testRT_UnsignedList()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		uint a = 10u;
		uint b = 20u;
		uint c = 30u;
		SBR_ASSERT(w.writeUnsigned(a, b, c), "uint list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		uint x;
		uint y;
		uint z;
		SBR_ASSERT(r.readUnsigned(x, y, z), "uint list read ok");
		SBR_ASSERT(x == a && y == b && z == c, "uint list values match");
	}

void SerializerTest::testRT_String()
{
	using namespace SerializerTestBitRoundTripLocal;
		string tests[] = {"", "hello", "你好世界", "a\0b"};
		for (const string& str : tests)
		{
			SerializerBitWrite w;
			w.initCapacity(512);
			SBR_ASSERT(w.writeString(str), "string write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			string out;
			SBR_ASSERT(r.readString(out), "string read ok");
			SBR_ASSERT(out == str, "string roundtrip match");
		}
	}

void SerializerTest::testRT_IntVectorList()
{
	using namespace SerializerTestBitRoundTripLocal;
		Vector<int> original{10, -20, 30, -40, 50};

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeSignedList(true, original), "int vector list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<int> result;
		SBR_ASSERT(r.readSignedList(true, result), "int vector list read ok");
		SBR_ASSERT(result.size() == original.size(), "int vector list size match");
		FOR_VECTOR(original)
		{
			SBR_ASSERT_INT(result[i], original[i], "int vector list element match");
		}
	}

void SerializerTest::testRT_UintVectorList()
{
	using namespace SerializerTestBitRoundTripLocal;
		Vector<uint> original {100u, 200u, 300u, UINT_MAX};

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeUnsignedList(original), "uint vector list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<uint> result;
		SBR_ASSERT(r.readUnsignedList(result), "uint vector list read ok");
		SBR_ASSERT(result.size() == original.size(), "uint vector list size match");
		FOR_VECTOR(original)
		{
			SBR_ASSERT(result[i] == original[i], "uint vector list element match");
		}
	}

void SerializerTest::testRT_FloatVectorList()
{
	using namespace SerializerTestBitRoundTripLocal;
		Vector<float> original {1.1f, 2.2f, -3.3f, 0.0f, 100.5f};

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeFloatList(true, original), "float vector list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<float> result;
		SBR_ASSERT(r.readFloatList(true, result), "float vector list read ok");
		SBR_ASSERT(result.size() == original.size(), "float vector list size match");
		for (int i = 0; i < original.size(); ++i)
		{
			SBR_ASSERT_FLOAT(result[i], original[i], 0.01f * fabsf(original[i]) + 1e-5f, "float vector element match");
		}
	}

void SerializerTest::testRT_FixedArray()
{
	using namespace SerializerTestBitRoundTripLocal;
		Array<4, int> arr = {10, -20, 30, -40};

		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeSigned(true, arr), "fixed array write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Array<4, int> outArr;
		// 注意: Array 读需要用指针数组形式
		int* ptrs[4] = {&outArr[0], &outArr[1], &outArr[2], &outArr[3]};
		SBR_ASSERT(r.readSigned(true, *ptrs[0], *ptrs[1], *ptrs[2], *ptrs[3]), "fixed array read ok");
		FOR(4)
		{
			SBR_ASSERT_INT(outArr[i], arr[i], "fixed array element match");
		}
	}

void SerializerTest::testRT_Vector2()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2(true, Vector2(1.5f, -2.5f)), "V2 write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2 out;
		SBR_ASSERT(r.readVector2(true, out), "V2 read ok");
		SBR_ASSERT_FLOAT(out.x, 1.5f, 1e-4f, "V2 x");
		SBR_ASSERT_FLOAT(out.y, -2.5f, 1e-4f, "V2 y");
	}

void SerializerTest::testRT_Vector2Int()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2Int(true, Vector2Int(100, -200)), "V2I write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2Int out;
		SBR_ASSERT(r.readVector2Int(true, out), "V2I read ok");
		SBR_ASSERT_INT(out.x, 100, "V2I x");
		SBR_ASSERT_INT(out.y, -200, "V2I y");
	}

void SerializerTest::testRT_Vector2UInt()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2UInt(Vector2UInt{100u, 200u}), "V2UInt write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2UInt out;
		SBR_ASSERT(r.readVector2UInt(out), "V2UInt read ok");
		SBR_ASSERT(out.x == 100u && out.y == 200u, "V2UInt values");
	}

void SerializerTest::testRT_Vector2Short()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2Short(true, Vector2Short{-32000, 32000}), "V2Short write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2Short out;
		SBR_ASSERT(r.readVector2Short(true, out), "V2Short read ok");
		SBR_ASSERT(out.x == -32000 && out.y == 32000, "V2Short values");
	}

void SerializerTest::testRT_Vector2UShort()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector2UShort(Vector2UShort{0, 65535}), "V2UShort write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector2UShort out;
		SBR_ASSERT(r.readVector2UShort(out), "V2UShort read ok");
		SBR_ASSERT(out.x == 0 && out.y == 65535, "V2UShort values");
	}

void SerializerTest::testRT_Vector3()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector3(true, Vector3(1.0f, 2.0f, 3.0f)), "V3 write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector3 out;
		SBR_ASSERT(r.readVector3(true, out), "V3 read ok");
		SBR_ASSERT_FLOAT(out.x, 1.0f, 1e-4f, "V3 x");
		SBR_ASSERT_FLOAT(out.y, 2.0f, 1e-4f, "V3 y");
		SBR_ASSERT_FLOAT(out.z, 3.0f, 1e-4f, "V3 z");
	}

void SerializerTest::testRT_Vector4()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeVector4(true, Vector4(1.0f, 2.0f, 3.0f, 4.0f)), "V4 write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector4 out;
		SBR_ASSERT(r.readVector4(true, out), "V4 read ok");
		SBR_ASSERT_FLOAT(out.x, 1.0f, 1e-4f, "V4 x");
		SBR_ASSERT_FLOAT(out.y, 2.0f, 1e-4f, "V4 y");
		SBR_ASSERT_FLOAT(out.z, 3.0f, 1e-4f, "V4 z");
		SBR_ASSERT_FLOAT(out.w, 4.0f, 1e-4f, "V4 w");
	}

void SerializerTest::testRT_BufferRoundTrip()
{
	using namespace SerializerTestBitRoundTripLocal;
		const char src[] = { 0x10, 0x20, 0x30, 0x40, (char)0xFF };
		SerializerBitWrite w;
		w.initCapacity(256);
		SBR_ASSERT(w.writeBuffer(src, 5), "bit buf write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		char dst[5] = {};
		SBR_ASSERT(r.readBuffer(dst, 5), "bit buf read ok");
		FOR(5)
		{
			SBR_ASSERT(dst[i] == src[i], "bit buf byte match");
		}
	}

void SerializerTest::testRT_BufferNoCopy()
{
	using namespace SerializerTestBitRoundTripLocal;
		const char src[] = { 0x01, 0x02, 0x03, 0x04 };
		SerializerBitWrite w;
		w.initCapacity(256);
		w.writeBuffer(src, 4);

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		const char* ptr = nullptr;
		SBR_ASSERT(r.readBufferNoCopy(ptr, 4), "bit bufNoCopy read ok");
		SBR_ASSERT(ptr != nullptr, "bit bufNoCopy non-null");
		SBR_ASSERT(ptr[0] == 0x01 && ptr[3] == 0x04, "bit bufNoCopy content");
	}

void SerializerTest::testRT_CustomRoundTrip()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(256);
		TestCustomBit src;
		src.mA = 42;
		src.mB = -99;
		SBR_ASSERT(w.writeCustom(true, src), "bit custom write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		TestCustomBit dst;
		SBR_ASSERT(r.readCustom(true, dst), "bit custom read ok");
		SBR_ASSERT_INT(dst.mA, 42, "bit custom mA");
		SBR_ASSERT_INT(dst.mB, -99, "bit custom mB");
	}

void SerializerTest::testRT_CustomList()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(512);
		Vector<TestCustomBit> src;
		FOR(3)
		{
			TestCustomBit p;
			p.mA = i * 100;
			p.mB = i * -100;
			src.add(p);
		}
		SBR_ASSERT(w.writeCustomList(true, src), "bit customList write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<TestCustomBit> dst;
		SBR_ASSERT(r.readCustomList(true, dst), "bit customList read ok");
		SBR_ASSERT((int)dst.size() == 3, "bit customList size");
		FOR(3)
		{
			SBR_ASSERT_INT(dst[i].mA, i * 100, "bit customList[i].mA");
			SBR_ASSERT_INT(dst[i].mB, i * -100, "bit customList[i].mB");
		}
	}

void SerializerTest::testRT_EmptySignatureList()
{
	using namespace SerializerTestBitRoundTripLocal;
		// signed empty
		{
			Vector<int> empty;
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.writeSignedList(true, empty), "empty signedList write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			Vector<int> dst;
			SBR_ASSERT(r.readSignedList(true, dst), "empty signedList read ok");
			SBR_ASSERT(dst.isEmpty(), "empty signedList isEmpty");
		}
		// unsigned empty
		{
			Vector<uint> empty;
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.writeUnsignedList(empty), "empty unsignedList write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			Vector<uint> dst;
			SBR_ASSERT(r.readUnsignedList(dst), "empty unsignedList read ok");
			SBR_ASSERT(dst.isEmpty(), "empty unsignedList isEmpty");
		}
		// float empty
		{
			Vector<float> empty;
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.writeFloatList(true, empty), "empty floatList write ok");

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			Vector<float> dst;
			SBR_ASSERT(r.readFloatList(true, dst), "empty floatList read ok");
			SBR_ASSERT(dst.isEmpty(), "empty floatList isEmpty");
		}
	}

void SerializerTest::testRT_ByteBoundary()
{
	using namespace SerializerTestBitRoundTripLocal;
		// fillZeroToByteEnd
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeSigned(true, 1);
			w.fillZeroToByteEnd();
			SBR_ASSERT((w.getBitCount() & 7) == 0, "fillZeroToByteEnd aligned");
		}
		// skipToByteEnd
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeSigned(true, 1);
			w.writeUnsigned(2u);

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			int v;
			r.readSigned(true, v);
			r.skipToByteEnd();
			SBR_ASSERT((r.getBitIndex() & 7) == 0, "skipToByteEnd aligned");
		}
	}

void SerializerTest::testRT_BitUtility()
{
	using namespace SerializerTestBitRoundTripLocal;
		// getByteCount, getBufferSize
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			SBR_ASSERT(w.getBufferSize() >= 256, "bit initCapacity ok");
			SBR_ASSERT(w.getByteCount() == 0, "bit byteCount initially 0");
			w.writeSigned(true, 42);
			SBR_ASSERT(w.getByteCount() > 0, "bit byteCount after write > 0");
		}
		// clear
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeSigned(true, 100);
			SBR_ASSERT(w.getBitCount() > 0, "bit bitCount > 0 before clear");
			w.clear();
			SBR_ASSERT(w.getBitCount() == 0, "bit bitCount == 0 after clear");
			// clear后重新写入
			w.writeSigned(true, 200);
			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			int outVal = 0;
			SBR_ASSERT(r.readSigned(true, outVal) && outVal == 200, "bit clear reuse ok");
		}
		// getBitIndex, setBitIndex, getReadByteCount
		{
			SerializerBitWrite w;
			w.initCapacity(256);
			w.writeBool(true);

			SerializerBitRead r(w.getBuffer(), w.getBufferSize());
			SBR_ASSERT(r.getBitIndex() == 0, "bit getBitIndex init 0");
			SBR_ASSERT(r.getReadByteCount() == 0, "bit getReadByteCount init 0");

			bool b = false;
			r.readBool(b);
			SBR_ASSERT(b == true, "bit bool value");
			SBR_ASSERT(r.getBitIndex() == 1, "bit getBitIndex after 1 bool = 1");
			SBR_ASSERT(r.getReadByteCount() == 1, "bit getReadByteCount after 1 bool = 1");

			r.setBitIndex(0);
			SBR_ASSERT(r.getBitIndex() == 0, "bit setBitIndex(0) ok");
		}
	}

void SerializerTest::testRT_MixedTypes()
{
	using namespace SerializerTestBitRoundTripLocal;
		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeBool(true), "mixed bool");
		SBR_ASSERT(w.writeSigned(true, 42), "mixed signed int");
		SBR_ASSERT(w.writeUnsigned(100u), "mixed unsigned");
		SBR_ASSERT(w.writeFloat(true, 3.14f), "mixed float");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		bool b;
		int vi;
		uint vu;
		float vf;
		SBR_ASSERT(r.readBool(b) && b == true, "mixed read bool");
		SBR_ASSERT(r.readSigned(true, vi) && vi == 42, "mixed read signed");
		SBR_ASSERT(r.readUnsigned(vu) && vu == 100u, "mixed read unsigned");
		SBR_ASSERT(r.readFloat(true, vf), "mixed read float");
		SBR_ASSERT_FLOAT(vf, 3.14f, 1e-4f, "mixed float value");
	}

void SerializerTest::testRT_DoubleList()
{
	using namespace SerializerTestBitRoundTripLocal;
		Vector<double> src;
		src.add(1.2345);
		src.add(-6.789);
		src.add(0.0);

		SerializerBitWrite w;
		w.initCapacity(512);
		SBR_ASSERT(w.writeDoubleList(true, src), "double list write ok");

		SerializerBitRead r(w.getBuffer(), w.getBufferSize());
		Vector<double> dst;
		SBR_ASSERT(r.readDoubleList(true, dst), "double list read ok");
		SBR_ASSERT((int)dst.size() == 3, "double list size");
	}

void SerializerTest::testBitRoundTrip()
{
	testRT_Bool();
	testRT_SignedInt_Typical();
	testRT_SignedInt_Boundary();
	testRT_UnsignedInt_Typical();
	testRT_UnsignedInt_Max();
	testRT_Short();
	testRT_LLong();
	testRT_Byte();
	testRT_Float();
	testRT_Double();
	testRT_Float_NoSign();
	testRT_SignedList();
	testRT_UnsignedList();
	testRT_String();
	testRT_IntVectorList();
	testRT_UintVectorList();
	testRT_FloatVectorList();
	testRT_FixedArray();
	testRT_Vector2();
	testRT_Vector2Int();
	testRT_Vector2UInt();
	testRT_Vector2Short();
	testRT_Vector2UShort();
	testRT_Vector3();
	testRT_Vector4();
	testRT_BufferRoundTrip();
	testRT_BufferNoCopy();
	testRT_CustomRoundTrip();
	testRT_CustomList();
	testRT_EmptySignatureList();
	testRT_ByteBoundary();
	testRT_BitUtility();
	testRT_MixedTypes();
	testRT_DoubleList();
}
#endif
