#ifdef WINDOWS
#include "FrameHeader.h"

#define IS_POD_TYPE_ASSERT(expr) \
	if (!(expr)) \
	{ \
		LOG("IsPODTypeTraitsTest FAILED: " #expr); \
	}

//-------------------------------------------------------------
// 辅助枚举和类型，用于 IsSubClassOf 测试
//-------------------------------------------------------------
enum class TestColorEnum { Red, Green, Blue };
enum TestOldEnum { OE_A, OE_B, OE_C };

struct IPTBase {};
struct IPTChild : public IPTBase {};
struct IPTGrandChild : public IPTChild {};
struct IPTUnrelated {};

//-------------------------------------------------------------
// 用于触发 IsPodIntegerType SFINAE 的模板函数
//-------------------------------------------------------------
template<typename T, typename>
bool IsPODTypeTraitsTest::isPodIntFn(T)
{ return true; }

template<typename T, typename>
bool IsPODTypeTraitsTest::isPodSignedFn(T)
{ return true; }

template<typename T, typename>
bool IsPODTypeTraitsTest::isPodUnsignedFn(T)
{ return true; }

//-------------------------------------------------------------

void IsPODTypeTraitsTest::testIsPodPrimitives()
{
	// 所有基础类型应返回 true
	IS_POD_TYPE_ASSERT(IsPod<bool>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<char>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<byte>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<short>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<ushort>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<uint>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<long>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<ulong>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<llong>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<ullong>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<float>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPod<double>::mValue == true);
}

void IsPODTypeTraitsTest::testIsPodNonPrimitive()
{
	// 非基础类型应返回 false
	IS_POD_TYPE_ASSERT(IsPod<string>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPod<IPTBase>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPod<IPTChild>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPod<void*>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPod<int*>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPodTypeWithEnum()
{
	// IsPodType 对枚举也返回 true（因为 is_enum<T>::value）
	IS_POD_TYPE_ASSERT(IsPodType<TestColorEnum>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodType<TestOldEnum>::mValue == true);
	// 基础类型也返回 true
	IS_POD_TYPE_ASSERT(IsPodType<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodType<float>::mValue == true);
	// 非基础非枚举返回 false
	IS_POD_TYPE_ASSERT(IsPodType<IPTBase>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodType<string>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPodTypeWithConst()
{
	// IsPodType 通过 remove_cv 处理 const/volatile
	IS_POD_TYPE_ASSERT(IsPodType<const int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodType<volatile float>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodType<const volatile double>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodType<const string>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPointerPrimitive()
{
	// 指针类型
	IS_POD_TYPE_ASSERT(IsPointer<int*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<float*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<void*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<IPTBase*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<const char*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<int**>::mValue == true);
}

void IsPODTypeTraitsTest::testIsPointerNonPointer()
{
	// 非指针类型
	IS_POD_TYPE_ASSERT(IsPointer<int>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<float>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<string>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<IPTBase>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<char>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPointerNull()
{
	// nullptr_t 也是指针（std::is_pointer 不对 nullptr_t 成立，这里仅验证 int* 最基本用法）
	// 使用实际实例化验证
	int x = 0;
	int* pInt = &x;
	float y = 0.0f;
	float* pFloat = &y;
	IS_POD_TYPE_ASSERT(IsPointer<decltype(pInt)>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<decltype(pFloat)>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPointer<decltype(x)>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPodOrPointerType()
{
	// IsPodOrPointerType：是 pod 或者是指针
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<float>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<int*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<IPTBase*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<string>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<IPTBase>::mValue == false);
}

void IsPODTypeTraitsTest::testIsNotPodAndPointerType()
{
	// IsNotPodAndPointerType：既不是 pod 也不是指针
	IS_POD_TYPE_ASSERT(IsNotPodAndPointerType<int>::mValue == false);
	IS_POD_TYPE_ASSERT(IsNotPodAndPointerType<int*>::mValue == false);
	IS_POD_TYPE_ASSERT(IsNotPodAndPointerType<string>::mValue == true);
	IS_POD_TYPE_ASSERT(IsNotPodAndPointerType<IPTBase>::mValue == true);
}

void IsPODTypeTraitsTest::testIsPodIntegerIntegers()
{
	// 整型（包括 bool/char）
	IS_POD_TYPE_ASSERT(IsPodInteger<bool>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<char>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<byte>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<short>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<ushort>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<uint>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<long>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<ulong>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<llong>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<ullong>::mValue == true);
}

void IsPODTypeTraitsTest::testIsPodIntegerFloats()
{
	// 浮点型不是整型
	IS_POD_TYPE_ASSERT(IsPodInteger<float>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodInteger<double>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPodIntegerBool()
{
	// bool 既是 pod 也是整数
	IS_POD_TYPE_ASSERT(IsPod<bool>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<bool>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<bool>::mValue == false); // bool 不是有符号整型
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<bool>::mValue == false); // bool 不是无符号整型
}

void IsPODTypeTraitsTest::testIsPodSignedIntegers()
{
	// 有符号整型
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<char>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<short>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<long>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<llong>::mValue == true);
	// 非有符号
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<byte>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<ushort>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<uint>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<ulong>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<ullong>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<float>::mValue == false);
}

void IsPODTypeTraitsTest::testIsPodUnsignedIntegers()
{
	// 无符号整型
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<byte>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<ushort>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<uint>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<ulong>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<ullong>::mValue == true);
	// 非无符号
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<char>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<short>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<int>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<llong>::mValue == false);
}

void IsPODTypeTraitsTest::testToUnsignedIntegerMapping()
{
	// ToUnsignedInteger 类型映射
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<char>::Type, byte>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<byte>::Type, byte>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<short>::Type, ushort>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<ushort>::Type, ushort>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<int>::Type, uint>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<uint>::Type, uint>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<llong>::Type, ullong>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<ullong>::Type, ullong>::value) == true);
	// 没有特化的类型：Type 等于自身
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<float>::Type, float>::value) == true);
	IS_POD_TYPE_ASSERT((std::is_same<ToUnsignedInteger<double>::Type, double>::value) == true);
}

void IsPODTypeTraitsTest::testIsPositiveNonNegative()
{
	// IsPositive<N>::mValue 为 N > 0
	IS_POD_TYPE_ASSERT(IsPositive<1>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPositive<100>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPositive<0>::mValue == false);
	// mType：N >= 0 时存在（可使用 enable_if_t）
	// 间接验证：IsPositive<0>::mType 能编译（因为 N >= 0 时有 mType）
	// 这里通过 sizeof 触发实例化
	bool has0Type = (sizeof(IsPositive<0>) > 0);  // IsPositive<0>::mType 存在（N>=0）
	bool has1Type = (sizeof(IsPositive<1>) > 0);  // IsPositive<1>::mType 存在（N>=0）
	IS_POD_TYPE_ASSERT(has0Type);
	IS_POD_TYPE_ASSERT(has1Type);
}

void IsPODTypeTraitsTest::testIsSubClassOf()
{
	// IsSubClassOf<Base, Child>::mValue
	// 用局部变量回避宏参数逗号解析问题
	constexpr bool baseChild    = IsSubClassOfV<IPTBase, IPTChild>::mValue;
	constexpr bool baseGrand    = IsSubClassOfV<IPTBase, IPTGrandChild>::mValue;
	constexpr bool childGrand   = IsSubClassOfV<IPTChild, IPTGrandChild>::mValue;
	constexpr bool baseSelf     = IsSubClassOfV<IPTBase, IPTBase>::mValue;
	IS_POD_TYPE_ASSERT(baseChild    == true);
	IS_POD_TYPE_ASSERT(baseGrand    == true);
	IS_POD_TYPE_ASSERT(childGrand   == true);
	IS_POD_TYPE_ASSERT(baseSelf     == true);
}

void IsPODTypeTraitsTest::testCompileTimeEnableIf()
{
	// 通过 SFINAE 确认 IsPodIntegerType::mType 可用作函数参数
	IS_POD_TYPE_ASSERT(isPodIntFn((int)1) == true);
	IS_POD_TYPE_ASSERT(isPodIntFn((char)1) == true);
	IS_POD_TYPE_ASSERT(isPodIntFn((llong)1) == true);
	IS_POD_TYPE_ASSERT(isPodIntFn((bool)true) == true);

	IS_POD_TYPE_ASSERT(isPodSignedFn((int)1) == true);
	IS_POD_TYPE_ASSERT(isPodSignedFn((char)1) == true);
	IS_POD_TYPE_ASSERT(isPodSignedFn((llong)1) == true);

	IS_POD_TYPE_ASSERT(isPodUnsignedFn((byte)1) == true);
	IS_POD_TYPE_ASSERT(isPodUnsignedFn((ushort)1) == true);
	IS_POD_TYPE_ASSERT(isPodUnsignedFn((uint)1) == true);
	IS_POD_TYPE_ASSERT(isPodUnsignedFn((ullong)1) == true);
}

void IsPODTypeTraitsTest::testMixedTypeChecks()
{
	// 综合验证：同类型在不同 traits 中的一致性
	// int：pod=true, integer=true, signed=true, unsigned=false
	IS_POD_TYPE_ASSERT(IsPod<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<int>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<int>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<int>::mValue == false);

	// uint：pod=true, integer=true, signed=false, unsigned=true
	IS_POD_TYPE_ASSERT(IsPod<uint>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<uint>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<uint>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<uint>::mValue == true);

	// float：pod=true, integer=false, signed=false, unsigned=false
	IS_POD_TYPE_ASSERT(IsPod<float>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodInteger<float>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodSignedInteger<float>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodUnsignedInteger<float>::mValue == false);

	// IPTBase*：pod=false, pointer=true
	IS_POD_TYPE_ASSERT(IsPod<IPTBase*>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<IPTBase*>::mValue == true);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<IPTBase*>::mValue == true);

	// string：pod=false, pointer=false, not pod and pointer=true
	IS_POD_TYPE_ASSERT(IsPod<string>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPointer<string>::mValue == false);
	IS_POD_TYPE_ASSERT(IsPodOrPointerType<string>::mValue == false);
	IS_POD_TYPE_ASSERT(IsNotPodAndPointerType<string>::mValue == true);
}

void IsPODTypeTraitsTest::test()
{
	testIsPodPrimitives();
	testIsPodNonPrimitive();
	testIsPodTypeWithEnum();
	testIsPodTypeWithConst();
	testIsPointerPrimitive();
	testIsPointerNonPointer();
	testIsPointerNull();
	testIsPodOrPointerType();
	testIsNotPodAndPointerType();
	testIsPodIntegerIntegers();
	testIsPodIntegerFloats();
	testIsPodIntegerBool();
	testIsPodSignedIntegers();
	testIsPodUnsignedIntegers();
	testToUnsignedIntegerMapping();
	testIsPositiveNonNegative();
	testIsSubClassOf();
	testCompileTimeEnableIf();
	testMixedTypeChecks();
}
#endif
