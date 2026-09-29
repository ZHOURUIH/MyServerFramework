#ifdef WINDOWS
#include "FrameHeader.h"

// ============================================================
// TypeUtilityTest — TypeUtility namespace 全方位测试
// 覆盖：isType<T0,T1>/isSubClass<B,C>/isPODType<T>/
//        isSignedInteger<T>/isUnsignedInteger<T>/
//        IsPod<T>::mValue/IsPodSignedInteger<T>::mValue/
//        IsPodUnsignedInteger<T>::mValue/IsPointer<T>::mValue/
//        IsSubClassOf<B,C>::mValue/IsNotPodAndPointerType/
//        IsPodOrPointerType/编译期验证
// ============================================================

#define TUT_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("TypeUtilityTest FAILED: ") + (msg)); }

// -----------------------------------------------------------
// 辅助类层次
// -----------------------------------------------------------
struct Animal {};
struct Dog : public Animal {};
struct Cat : public Animal {};
struct Poodle : public Dog {};

// -----------------------------------------------------------
// isType<T0, T1>
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsType_Same()
{
    TUT_ASSERT((isType<int, int>()), "isType int==int");
    TUT_ASSERT((isType<float, float>()), "isType float==float");
    TUT_ASSERT((isType<bool, bool>()), "isType bool==bool");
    TUT_ASSERT((isType<llong, llong>()), "isType llong==llong");
    TUT_ASSERT((isType<string, string>()), "isType string==string");
    TUT_ASSERT((isType<double, double>()), "isType double==double");
    TUT_ASSERT((isType<byte, byte>()), "isType byte==byte");
    TUT_ASSERT((isType<short, short>()), "isType short==short");
    TUT_ASSERT((isType<ushort, ushort>()), "isType ushort==ushort");
    TUT_ASSERT((isType<uint, uint>()), "isType uint==uint");
    TUT_ASSERT((isType<char, char>()), "isType char==char");
}

void TypeUtilityTest::testTypeUtil_IsType_Different()
{
    TUT_ASSERT((!isType<int, float>()), "isType int!=float");
    TUT_ASSERT((!isType<int, bool>()), "isType int!=bool");
    TUT_ASSERT((!isType<float, double>()), "isType float!=double");
    TUT_ASSERT((!isType<short, int>()), "isType short!=int");
    TUT_ASSERT((!isType<llong, int>()), "isType llong!=int");
    TUT_ASSERT((!isType<string, int>()), "isType string!=int");
    TUT_ASSERT((!isType<byte, ushort>()), "isType byte!=ushort");
    TUT_ASSERT((!isType<uint, int>()), "isType uint!=int");
    TUT_ASSERT((!isType<bool, byte>()), "isType bool!=byte");
}

void TypeUtilityTest::testTypeUtil_IsType_WithConst()
{
    // isType 会 decay const/ref/volatile，所以 const int == int
    TUT_ASSERT((isType<const int, int>()), "isType const int==int");
    TUT_ASSERT((isType<int&, int>()), "isType int& == int");
    TUT_ASSERT((isType<const float&, float>()), "isType const float& == float");
    TUT_ASSERT((isType<volatile int, int>()), "isType volatile int == int");
}

void TypeUtilityTest::testTypeUtil_IsType_UserDefined()
{
    TUT_ASSERT((isType<Animal, Animal>()), "isType Animal==Animal");
    TUT_ASSERT((!isType<Dog, Animal>()), "isType Dog!=Animal");
    TUT_ASSERT((!isType<Animal, Dog>()), "isType Animal!=Dog");
    TUT_ASSERT((isType<Dog, Dog>()), "isType Dog==Dog");
}

// -----------------------------------------------------------
// isSubClass<B, C>
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSubClass_DirectChild()
{
    TUT_ASSERT((isSubClass<Animal, Dog>()), "isSubClass Dog is Animal");
    TUT_ASSERT((isSubClass<Animal, Cat>()), "isSubClass Cat is Animal");
    TUT_ASSERT((isSubClass<Dog, Poodle>()), "isSubClass Poodle is Dog");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_Transitive()
{
    TUT_ASSERT((isSubClass<Animal, Poodle>()), "isSubClass Poodle is Animal (transitive)");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_SameClass()
{
    // IsSubClassOf<T, T>::mValue is true (same type passes test)
    TUT_ASSERT((isSubClass<Animal, Animal>()), "isSubClass same class true");
    TUT_ASSERT((isSubClass<Dog, Dog>()), "isSubClass Dog same true");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_NotRelated()
{
    TUT_ASSERT((!isSubClass<Dog, Cat>()), "isSubClass Cat is NOT Dog");
    TUT_ASSERT((!isSubClass<Cat, Dog>()), "isSubClass Dog is NOT Cat");
    TUT_ASSERT((!isSubClass<Dog, Animal>()), "isSubClass Animal is NOT Dog");
    TUT_ASSERT((!isSubClass<Cat, Animal>()), "isSubClass Animal is NOT Cat");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_Poodle()
{
    TUT_ASSERT((isSubClass<Dog, Poodle>()), "isSubClass Poodle is Dog");
    TUT_ASSERT((isSubClass<Animal, Poodle>()), "isSubClass Poodle is Animal");
    TUT_ASSERT((!isSubClass<Cat, Poodle>()), "isSubClass Poodle is NOT Cat");
}

// -----------------------------------------------------------
// isPODType<T>
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPODType_True()
{
    TUT_ASSERT((isPODType<bool>()), "isPOD bool true");
    TUT_ASSERT((isPODType<char>()), "isPOD char true");
    TUT_ASSERT((isPODType<byte>()), "isPOD byte true");
    TUT_ASSERT((isPODType<short>()), "isPOD short true");
    TUT_ASSERT((isPODType<ushort>()), "isPOD ushort true");
    TUT_ASSERT((isPODType<int>()), "isPOD int true");
    TUT_ASSERT((isPODType<uint>()), "isPOD uint true");
    TUT_ASSERT((isPODType<long>()), "isPOD long true");
    TUT_ASSERT((isPODType<ulong>()), "isPOD ulong true");
    TUT_ASSERT((isPODType<llong>()), "isPOD llong true");
    TUT_ASSERT((isPODType<ullong>()), "isPOD ullong true");
    TUT_ASSERT((isPODType<float>()), "isPOD float true");
    TUT_ASSERT((isPODType<double>()), "isPOD double true");
}

void TypeUtilityTest::testTypeUtil_IsPODType_False()
{
    TUT_ASSERT((!isPODType<string>()), "isPOD string false");
    TUT_ASSERT((!isPODType<Animal>()), "isPOD Animal false");
    TUT_ASSERT((!isPODType<Dog>()), "isPOD Dog false");
    TUT_ASSERT((!isPODType<Vector2>()), "isPOD Vector2 false");
    TUT_ASSERT((!isPODType<Vector3>()), "isPOD Vector3 false");
}

// -----------------------------------------------------------
// isSignedInteger<T>
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSignedInteger_True()
{
    TUT_ASSERT((isSignedInteger<char>()), "signed char true");
    TUT_ASSERT((isSignedInteger<short>()), "signed short true");
    TUT_ASSERT((isSignedInteger<int>()), "signed int true");
    TUT_ASSERT((isSignedInteger<long>()), "signed long true");
    TUT_ASSERT((isSignedInteger<llong>()), "signed llong true");
}

void TypeUtilityTest::testTypeUtil_IsSignedInteger_False()
{
    TUT_ASSERT((!isSignedInteger<bool>()), "signed bool false");
    TUT_ASSERT((!isSignedInteger<byte>()), "signed byte false");
    TUT_ASSERT((!isSignedInteger<ushort>()), "signed ushort false");
    TUT_ASSERT((!isSignedInteger<uint>()), "signed uint false");
    TUT_ASSERT((!isSignedInteger<ulong>()), "signed ulong false");
    TUT_ASSERT((!isSignedInteger<ullong>()), "signed ullong false");
    TUT_ASSERT((!isSignedInteger<float>()), "signed float false");
    TUT_ASSERT((!isSignedInteger<double>()), "signed double false");
    TUT_ASSERT((!isSignedInteger<string>()), "signed string false");
}

// -----------------------------------------------------------
// isUnsignedInteger<T>
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsUnsignedInteger_True()
{
    TUT_ASSERT((isUnsignedInteger<byte>()), "unsigned byte true");
    TUT_ASSERT((isUnsignedInteger<ushort>()), "unsigned ushort true");
    TUT_ASSERT((isUnsignedInteger<uint>()), "unsigned uint true");
    TUT_ASSERT((isUnsignedInteger<ulong>()), "unsigned ulong true");
    TUT_ASSERT((isUnsignedInteger<ullong>()), "unsigned ullong true");
}

void TypeUtilityTest::testTypeUtil_IsUnsignedInteger_False()
{
    TUT_ASSERT((!isUnsignedInteger<bool>()), "unsigned bool false");
    TUT_ASSERT((!isUnsignedInteger<char>()), "unsigned char false");
    TUT_ASSERT((!isUnsignedInteger<short>()), "unsigned short false");
    TUT_ASSERT((!isUnsignedInteger<int>()), "unsigned int false");
    TUT_ASSERT((!isUnsignedInteger<long>()), "unsigned long false");
    TUT_ASSERT((!isUnsignedInteger<llong>()), "unsigned llong false");
    TUT_ASSERT((!isUnsignedInteger<float>()), "unsigned float false");
    TUT_ASSERT((!isUnsignedInteger<double>()), "unsigned double false");
}

// -----------------------------------------------------------
// IsPod<T>::mValue 直接访问
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPodDirect()
{
    TUT_ASSERT((IsPod<int>::mValue), "IsPod<int>::mValue true");
    TUT_ASSERT((IsPod<float>::mValue), "IsPod<float>::mValue true");
    TUT_ASSERT((!IsPod<string>::mValue), "IsPod<string>::mValue false");
    TUT_ASSERT((!IsPod<Animal>::mValue), "IsPod<Animal>::mValue false");
    TUT_ASSERT((IsPod<llong>::mValue), "IsPod<llong>::mValue true");
    TUT_ASSERT((IsPod<bool>::mValue), "IsPod<bool>::mValue true");
}

// -----------------------------------------------------------
// IsPointer<T>::mValue
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPointer()
{
    TUT_ASSERT((IsPointer<int*>::mValue), "IsPointer<int*> true");
    TUT_ASSERT((IsPointer<Animal*>::mValue), "IsPointer<Animal*> true");
    TUT_ASSERT((IsPointer<void*>::mValue), "IsPointer<void*> true");
    TUT_ASSERT((!IsPointer<int>::mValue), "IsPointer<int> false");
    TUT_ASSERT((!IsPointer<string>::mValue), "IsPointer<string> false");
    TUT_ASSERT((!IsPointer<Animal>::mValue), "IsPointer<Animal> false");
    TUT_ASSERT((!IsPointer<float>::mValue), "IsPointer<float> false");
}

// -----------------------------------------------------------
// IsPodOrPointerType<T>::mValue
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPodOrPointer()
{
    TUT_ASSERT((IsPodOrPointerType<int>::mValue), "IsPodOrPointer int true");
    TUT_ASSERT((IsPodOrPointerType<float>::mValue), "IsPodOrPointer float true");
    TUT_ASSERT((IsPodOrPointerType<int*>::mValue), "IsPodOrPointer int* true");
    TUT_ASSERT((IsPodOrPointerType<Animal*>::mValue), "IsPodOrPointer Animal* true");
    TUT_ASSERT((!IsPodOrPointerType<string>::mValue), "IsPodOrPointer string false");
    TUT_ASSERT((!IsPodOrPointerType<Animal>::mValue), "IsPodOrPointer Animal false");
    TUT_ASSERT((IsPodOrPointerType<llong>::mValue), "IsPodOrPointer llong true");
}

// -----------------------------------------------------------
// IsNotPodAndPointerType<T>::mValue
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsNotPodAndPointer()
{
    TUT_ASSERT((IsNotPodAndPointerType<string>::mValue), "NotPodPointer string true");
    TUT_ASSERT((IsNotPodAndPointerType<Animal>::mValue), "NotPodPointer Animal true");
    TUT_ASSERT((!IsNotPodAndPointerType<int>::mValue), "NotPodPointer int false");
    TUT_ASSERT((!IsNotPodAndPointerType<float>::mValue), "NotPodPointer float false");
    TUT_ASSERT((!IsNotPodAndPointerType<int*>::mValue), "NotPodPointer int* false");
    TUT_ASSERT((!IsNotPodAndPointerType<Animal*>::mValue), "NotPodPointer Animal* false");
}

// -----------------------------------------------------------
// IsSubClassOf<B,C>::mValue 直接访问
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSubClassOfDirect()
{
    TUT_ASSERT((IsSubClassOfV<Animal, Dog>::mValue), "IsSubClassOf Dog:Animal true");
    TUT_ASSERT((IsSubClassOfV<Animal, Poodle>::mValue), "IsSubClassOf Poodle:Animal true");
    TUT_ASSERT((!IsSubClassOfV<Dog, Animal>::mValue), "IsSubClassOf Animal:Dog false");
    TUT_ASSERT((!IsSubClassOfV<Cat, Dog>::mValue), "IsSubClassOf Dog:Cat false");
    TUT_ASSERT((IsSubClassOfV<ClassObject, ClassObject>::mValue), "IsSubClassOf ClassObject same true");
}

// -----------------------------------------------------------
// 编译期约束与运行期综合测试
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_RuntimeComposite_Pod()
{
    const int podCount = 13; // bool/char/byte/short/ushort/int/uint/long/ulong/llong/ullong/float/double
    int detected = 0;
    if (isPODType<bool>())   ++detected;
    if (isPODType<char>())   ++detected;
    if (isPODType<byte>())   ++detected;
    if (isPODType<short>())  ++detected;
    if (isPODType<ushort>()) ++detected;
    if (isPODType<int>())    ++detected;
    if (isPODType<uint>())   ++detected;
    if (isPODType<long>())   ++detected;
    if (isPODType<ulong>())  ++detected;
    if (isPODType<llong>())  ++detected;
    if (isPODType<ullong>()) ++detected;
    if (isPODType<float>())  ++detected;
    if (isPODType<double>()) ++detected;
    TUT_ASSERT(detected == podCount, "runtime pod count==13");
}

void TypeUtilityTest::testTypeUtil_RuntimeComposite_Signed()
{
    int signedCount = 0;
    if (isSignedInteger<char>())  ++signedCount;
    if (isSignedInteger<short>()) ++signedCount;
    if (isSignedInteger<int>())   ++signedCount;
    if (isSignedInteger<long>())  ++signedCount;
    if (isSignedInteger<llong>()) ++signedCount;
    TUT_ASSERT(signedCount == 5, "runtime signed count==5");
}

void TypeUtilityTest::testTypeUtil_RuntimeComposite_Unsigned()
{
    int unsignedCount = 0;
    if (isUnsignedInteger<byte>())   ++unsignedCount;
    if (isUnsignedInteger<ushort>()) ++unsignedCount;
    if (isUnsignedInteger<uint>())   ++unsignedCount;
    if (isUnsignedInteger<ulong>())  ++unsignedCount;
    if (isUnsignedInteger<ullong>()) ++unsignedCount;
    TUT_ASSERT(unsignedCount == 5, "runtime unsigned count==5");
}

void TypeUtilityTest::testTypeUtil_RuntimeComposite_Hierarchy()
{
    // 运行时动态模拟类型判断
    bool dogIsAnimal   = isSubClass<Animal, Dog>();
    bool catIsAnimal   = isSubClass<Animal, Cat>();
    bool poodleIsDog   = isSubClass<Dog, Poodle>();
    bool dogIsNotCat   = !isSubClass<Cat, Dog>();
    bool animalIsNotDog = !isSubClass<Dog, Animal>();

    TUT_ASSERT(dogIsAnimal,    "dynamic dogIsAnimal");
    TUT_ASSERT(catIsAnimal,    "dynamic catIsAnimal");
    TUT_ASSERT(poodleIsDog,    "dynamic poodleIsDog");
    TUT_ASSERT(dogIsNotCat,    "dynamic dogIsNotCat");
    TUT_ASSERT(animalIsNotDog, "dynamic animalIsNotDog");
}

void TypeUtilityTest::testTypeUtil_IsType_EnumType()
{
    // enum 也可以匹配 isType
    enum Color { RED, GREEN, BLUE };
    TUT_ASSERT((isType<Color, Color>()), "isType enum same");
    TUT_ASSERT((!isType<Color, int>()), "isType enum!=int");
}

void TypeUtilityTest::testTypeUtil_IsPODType_Enum()
{
    // IsPodType includes enums via is_enum<T>
    enum ETest { EA, EB };
    TUT_ASSERT((isPODType<ETest>()), "isPOD enum true (is_enum)");
}

void TypeUtilityTest::testTypeUtil_ComplexSignedChain()
{
    // 验证有符号/无符号互不交叉
    TUT_ASSERT((isSignedInteger<int>() && !isUnsignedInteger<int>()), "int: signed not unsigned");
    TUT_ASSERT((!isSignedInteger<uint>() && isUnsignedInteger<uint>()), "uint: unsigned not signed");
    TUT_ASSERT((isSignedInteger<llong>() && !isUnsignedInteger<llong>()), "llong: signed not unsigned");
    TUT_ASSERT((!isSignedInteger<ullong>() && isUnsignedInteger<ullong>()), "ullong: unsigned not signed");
}

void TypeUtilityTest::testTypeUtil_PointerVsNot()
{
    TUT_ASSERT(IsPointer<int*>::mValue && !IsPointer<int>::mValue, "int* is pointer, int is not");
    TUT_ASSERT(IsPointer<Dog*>::mValue && !IsPointer<Dog>::mValue, "Dog* is pointer, Dog is not");
    TUT_ASSERT(IsPointer<void*>::mValue, "void* is pointer");
}

void TypeUtilityTest::testTypeUtil_IsPodNotFloat()
{
    TUT_ASSERT(!isSignedInteger<float>(), "float not signed integer");
    TUT_ASSERT(!isUnsignedInteger<float>(), "float not unsigned integer");
    TUT_ASSERT(isPODType<float>(), "float is POD");
    TUT_ASSERT(!isSignedInteger<double>(), "double not signed integer");
    TUT_ASSERT(isPODType<double>(), "double is POD");
}

void TypeUtilityTest::testTypeUtil_BoolNotSignedOrUnsigned()
{
    // bool: IsPodSignedInteger false, IsPodUnsignedInteger false
    TUT_ASSERT(!isSignedInteger<bool>(), "bool not signed");
    TUT_ASSERT(!isUnsignedInteger<bool>(), "bool not unsigned");
    TUT_ASSERT(isPODType<bool>(), "bool is POD");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_ClassObjectHierarchy()
{
    // ClassObject 层次
    TUT_ASSERT((isSubClass<ClassObject, ClassObject>()), "ClassObject same");
    // IEventListener 应该是 ClassObject 的父类（ClassObject 继承自 IEventListener）
    // 所以 ClassObject 是 IEventListener 的子类
}

void TypeUtilityTest::testTypeUtil_IsPodOfAllFrameBasicTypes()
{
    // 确认框架使用的所有基础类型
    int count = 0;
    if (IsPod<bool>::mValue)   ++count;
    if (IsPod<char>::mValue)   ++count;
    if (IsPod<byte>::mValue)   ++count;
    if (IsPod<short>::mValue)  ++count;
    if (IsPod<ushort>::mValue) ++count;
    if (IsPod<int>::mValue)    ++count;
    if (IsPod<uint>::mValue)   ++count;
    if (IsPod<llong>::mValue)  ++count;
    if (IsPod<ullong>::mValue) ++count;
    if (IsPod<float>::mValue)  ++count;
    if (IsPod<double>::mValue) ++count;
    TUT_ASSERT(count == 11, "IsPod all 11 basic types");
}

void TypeUtilityTest::testTypeUtil_IsType_PodAndNonPodMix()
{
    TUT_ASSERT((!isType<int, string>()), "int vs string diff");
    TUT_ASSERT((!isType<float, Vector2>()), "float vs Vector2 diff");
    TUT_ASSERT((!isType<double, float>()), "double vs float diff");
    TUT_ASSERT((isType<Vector2, Vector2>()), "Vector2 same");
    TUT_ASSERT((isType<Color, Color>()), "Color same");
}

// ============================================================
// TypeUtilityTest — 扩展测试（Part 2）
// 覆盖更多边界情况和组合验证
// ============================================================

// -----------------------------------------------------------
// 辅助枚举（测试用）
// -----------------------------------------------------------
enum class EColor { RED, GREEN, BLUE };
enum EOldStyle { OLD_A, OLD_B, OLD_C };

struct EmptyStruct {};
struct NestedParent {};
struct NestedChild : public NestedParent {};
struct NestedGrandchild : public NestedChild {};
struct UnrelatedStruct {};

// -----------------------------------------------------------
// isType<T0, T1> — 更多组合
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsType_AllPodPairs()
{
    // 每对不同基础类型都应返回 false
    TUT_ASSERT((!isType<int, llong>()), "int!=llong");
    TUT_ASSERT((!isType<int, uint>()), "int!=uint");
    TUT_ASSERT((!isType<float, int>()), "float!=int");
    TUT_ASSERT((!isType<double, llong>()), "double!=llong");
    TUT_ASSERT((!isType<char, byte>()), "char!=byte");
    TUT_ASSERT((!isType<short, ushort>()), "short!=ushort");
    TUT_ASSERT((!isType<byte, bool>()), "byte!=bool");
    TUT_ASSERT((!isType<bool, char>()), "bool!=char");
    TUT_ASSERT((!isType<uint, ulong>()), "uint!=ulong");
    TUT_ASSERT((!isType<ulong, ullong>()), "ulong!=ullong");
    TUT_ASSERT((!isType<llong, ullong>()), "llong!=ullong");
}

void TypeUtilityTest::testTypeUtil_IsType_StructTypes()
{
    TUT_ASSERT((isType<EmptyStruct, EmptyStruct>()), "EmptyStruct same");
    TUT_ASSERT((!isType<EmptyStruct, UnrelatedStruct>()), "EmptyStruct!=Unrelated");
    TUT_ASSERT((isType<Vector2, Vector2>()), "Vector2 same");
    TUT_ASSERT((isType<Vector3, Vector3>()), "Vector3 same");
    TUT_ASSERT((!isType<Vector2, Vector3>()), "Vector2!=Vector3");
    TUT_ASSERT((!isType<Vector2Int, Vector2>()), "Vector2Int!=Vector2");
}

void TypeUtilityTest::testTypeUtil_IsType_ScopedEnum()
{
    TUT_ASSERT((isType<EColor, EColor>()), "scoped enum same");
    TUT_ASSERT((!isType<EColor, EOldStyle>()), "scoped!=old style");
    TUT_ASSERT((!isType<EColor, int>()), "scoped enum!=int");
}

// -----------------------------------------------------------
// isSubClass<B, C> — 更多层次
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSubClass_3Level()
{
    // NestedGrandchild → NestedChild → NestedParent
    TUT_ASSERT((isSubClass<NestedParent, NestedChild>()), "Child is Parent");
    TUT_ASSERT((isSubClass<NestedChild, NestedGrandchild>()), "GC is Child");
    TUT_ASSERT((isSubClass<NestedParent, NestedGrandchild>()), "GC is Parent (transitive)");
    TUT_ASSERT((!isSubClass<NestedChild, NestedParent>()), "Parent not Child");
    TUT_ASSERT((!isSubClass<NestedGrandchild, NestedParent>()), "Parent not GC");
    TUT_ASSERT((!isSubClass<UnrelatedStruct, NestedParent>()), "Unrelated not Parent");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_Self()
{
    // Same class as both args: isSubClass<T, T> should return true (treated as same)
    TUT_ASSERT((isSubClass<Animal, Animal>()), "Animal isSubClass Animal");
    TUT_ASSERT((isSubClass<Dog, Dog>()), "Dog isSubClass Dog");
    TUT_ASSERT((isSubClass<NestedParent, NestedParent>()), "NestedParent self");
}

void TypeUtilityTest::testTypeUtil_IsSubClass_CrossBranch()
{
    // Different branches under same parent
    TUT_ASSERT((!isSubClass<Dog, Cat>()), "Cat not subclass of Dog");
    TUT_ASSERT((!isSubClass<Cat, Dog>()), "Dog not subclass of Cat");
    TUT_ASSERT((!isSubClass<Poodle, Cat>()), "Cat not subclass of Poodle");
}

// -----------------------------------------------------------
// isPODType<T> — 覆盖所有框架类型
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPODType_AllBasic()
{
    TUT_ASSERT(isPODType<bool>(),   "bool isPOD");
    TUT_ASSERT(isPODType<char>(),   "char isPOD");
    TUT_ASSERT(isPODType<byte>(),   "byte isPOD");
    TUT_ASSERT(isPODType<short>(),  "short isPOD");
    TUT_ASSERT(isPODType<ushort>(), "ushort isPOD");
    TUT_ASSERT(isPODType<int>(),    "int isPOD");
    TUT_ASSERT(isPODType<uint>(),   "uint isPOD");
    TUT_ASSERT(isPODType<long>(),   "long isPOD");
    TUT_ASSERT(isPODType<ulong>(),  "ulong isPOD");
    TUT_ASSERT(isPODType<llong>(),  "llong isPOD");
    TUT_ASSERT(isPODType<ullong>(), "ullong isPOD");
    TUT_ASSERT(isPODType<float>(),  "float isPOD");
    TUT_ASSERT(isPODType<double>(), "double isPOD");
    TUT_ASSERT(!isPODType<string>(), "string not POD");
    TUT_ASSERT(!isPODType<Animal>(), "Animal not POD");
}

// -----------------------------------------------------------
// isSignedInteger<T> — 完整覆盖
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSignedInteger_Exhaustive()
{
    TUT_ASSERT(isSignedInteger<char>(),  "char signed");
    TUT_ASSERT(isSignedInteger<short>(), "short signed");
    TUT_ASSERT(isSignedInteger<int>(),   "int signed");
    TUT_ASSERT(isSignedInteger<long>(),  "long signed");
    TUT_ASSERT(isSignedInteger<llong>(), "llong signed");

    TUT_ASSERT(!isSignedInteger<byte>(),   "byte not signed");
    TUT_ASSERT(!isSignedInteger<ushort>(), "ushort not signed");
    TUT_ASSERT(!isSignedInteger<uint>(),   "uint not signed");
    TUT_ASSERT(!isSignedInteger<ulong>(),  "ulong not signed");
    TUT_ASSERT(!isSignedInteger<ullong>(), "ullong not signed");
    TUT_ASSERT(!isSignedInteger<bool>(),   "bool not signed");
    TUT_ASSERT(!isSignedInteger<float>(),  "float not signed");
    TUT_ASSERT(!isSignedInteger<double>(), "double not signed");
}

// -----------------------------------------------------------
// isUnsignedInteger<T> — 完整覆盖
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsUnsignedInteger_Exhaustive()
{
    TUT_ASSERT(isUnsignedInteger<byte>(),   "byte unsigned");
    TUT_ASSERT(isUnsignedInteger<ushort>(), "ushort unsigned");
    TUT_ASSERT(isUnsignedInteger<uint>(),   "uint unsigned");
    TUT_ASSERT(isUnsignedInteger<ulong>(),  "ulong unsigned");
    TUT_ASSERT(isUnsignedInteger<ullong>(), "ullong unsigned");

    TUT_ASSERT(!isUnsignedInteger<char>(),  "char not unsigned");
    TUT_ASSERT(!isUnsignedInteger<short>(), "short not unsigned");
    TUT_ASSERT(!isUnsignedInteger<int>(),   "int not unsigned");
    TUT_ASSERT(!isUnsignedInteger<long>(),  "long not unsigned");
    TUT_ASSERT(!isUnsignedInteger<llong>(), "llong not unsigned");
    TUT_ASSERT(!isUnsignedInteger<bool>(),  "bool not unsigned");
    TUT_ASSERT(!isUnsignedInteger<float>(), "float not unsigned");
    TUT_ASSERT(!isUnsignedInteger<double>(),"double not unsigned");
}

// -----------------------------------------------------------
// IsPod<T>::mValue — 完整覆盖
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPodMValue_AllTypes()
{
    TUT_ASSERT(IsPod<bool>::mValue,   "IsPod bool");
    TUT_ASSERT(IsPod<char>::mValue,   "IsPod char");
    TUT_ASSERT(IsPod<byte>::mValue,   "IsPod byte");
    TUT_ASSERT(IsPod<short>::mValue,  "IsPod short");
    TUT_ASSERT(IsPod<ushort>::mValue, "IsPod ushort");
    TUT_ASSERT(IsPod<int>::mValue,    "IsPod int");
    TUT_ASSERT(IsPod<uint>::mValue,   "IsPod uint");
    TUT_ASSERT(IsPod<long>::mValue,   "IsPod long");
    TUT_ASSERT(IsPod<ulong>::mValue,  "IsPod ulong");
    TUT_ASSERT(IsPod<llong>::mValue,  "IsPod llong");
    TUT_ASSERT(IsPod<ullong>::mValue, "IsPod ullong");
    TUT_ASSERT(IsPod<float>::mValue,  "IsPod float");
    TUT_ASSERT(IsPod<double>::mValue, "IsPod double");
    TUT_ASSERT(!IsPod<string>::mValue, "IsPod string false");
}

// -----------------------------------------------------------
// IsPointer<T>::mValue — 各种指针类型
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsPointerMValue_Various()
{
    TUT_ASSERT(IsPointer<int*>::mValue,    "int* pointer");
    TUT_ASSERT(IsPointer<float*>::mValue,  "float* pointer");
    TUT_ASSERT(IsPointer<double*>::mValue, "double* pointer");
    TUT_ASSERT(IsPointer<char*>::mValue,   "char* pointer");
    TUT_ASSERT(IsPointer<void*>::mValue,   "void* pointer");
    TUT_ASSERT(IsPointer<Dog*>::mValue,    "Dog* pointer");
    TUT_ASSERT(IsPointer<Animal*>::mValue, "Animal* pointer");

    TUT_ASSERT(!IsPointer<int>::mValue,    "int not pointer");
    TUT_ASSERT(!IsPointer<float>::mValue,  "float not pointer");
    TUT_ASSERT(!IsPointer<Dog>::mValue,    "Dog not pointer");
    TUT_ASSERT(!IsPointer<string>::mValue, "string not pointer");
}

// -----------------------------------------------------------
// IsSubClassOf<B, C>::mValue — 完整验证
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSubClassOfMValue_Full()
{
    TUT_ASSERT((IsSubClassOfV<Animal, Dog>::mValue),   "SOF Dog:Animal");
    TUT_ASSERT((IsSubClassOfV<Animal, Cat>::mValue),   "SOF Cat:Animal");
    TUT_ASSERT((IsSubClassOfV<Dog, Poodle>::mValue),   "SOF Poodle:Dog");
    TUT_ASSERT((IsSubClassOfV<Animal, Poodle>::mValue),"SOF Poodle:Animal");
    TUT_ASSERT((IsSubClassOfV<NestedParent, NestedChild>::mValue), "SOF NC:NP");
    TUT_ASSERT((!IsSubClassOfV<Dog, Animal>::mValue),  "SOF Animal:Dog false");
    TUT_ASSERT((!IsSubClassOfV<Cat, Dog>::mValue),     "SOF Dog:Cat false");
    TUT_ASSERT((!IsSubClassOfV<NestedChild, NestedParent>::mValue),"SOF NP:NC false");
}

// -----------------------------------------------------------
// 混合测试：指针 + 继承组合
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_PointerAndInheritance()
{
    // isType<Dog*, Dog*> should be true, isType<Dog*, Animal*> false
    TUT_ASSERT((isType<Dog*, Dog*>()), "Dog* same");
    TUT_ASSERT((!isType<Dog*, Animal*>()), "Dog*!=Animal*");
    TUT_ASSERT((IsPointer<Dog*>::mValue), "Dog* is pointer");
    // Dog* is not a POD in the sense of signed/unsigned integer
    TUT_ASSERT(!isSignedInteger<Dog*>(), "Dog* not signed int");
    TUT_ASSERT(!isUnsignedInteger<Dog*>(), "Dog* not unsigned int");
}

// -----------------------------------------------------------
// isType 对 nullptr_t
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_NullptrType()
{
    TUT_ASSERT((isType<nullptr_t, nullptr_t>()), "nullptr_t same");
    TUT_ASSERT((!isType<nullptr_t, int*>()), "nullptr_t != int*");
}

// -----------------------------------------------------------
// IsSubClassOf — 同类测试（边界）
// -----------------------------------------------------------
void TypeUtilityTest::testTypeUtil_IsSubClassOf_BoundaryRules()
{
    // Self-to-self should be true
    TUT_ASSERT((IsSubClassOfV<Dog, Dog>::mValue), "SOF Dog:Dog true");
    TUT_ASSERT((IsSubClassOfV<Animal, Animal>::mValue), "SOF Animal:Animal true");
    // Different unrelated types
    TUT_ASSERT((!IsSubClassOfV<NestedParent, UnrelatedStruct>::mValue), "SOF Unrelated:NP false");
    TUT_ASSERT((!IsSubClassOfV<UnrelatedStruct, NestedParent>::mValue), "SOF NP:Unrelated false");
}

// ============================================================
// 扩展入口（由 TypeUtilityTest::test 调用内部注册）
// 实际上是附加到 TypeUtilityTest.cpp 末尾的补充测试
// ============================================================
void TypeUtilityTest::testTypeUtil_ExtendedAllPodPairs()
{
    testTypeUtil_IsType_AllPodPairs();
    testTypeUtil_IsType_StructTypes();
    testTypeUtil_IsType_ScopedEnum();
    testTypeUtil_IsSubClass_3Level();
    testTypeUtil_IsSubClass_Self();
    testTypeUtil_IsSubClass_CrossBranch();
    testTypeUtil_IsPODType_AllBasic();
    testTypeUtil_IsSignedInteger_Exhaustive();
    testTypeUtil_IsUnsignedInteger_Exhaustive();
    testTypeUtil_IsPodMValue_AllTypes();
    testTypeUtil_IsPointerMValue_Various();
    testTypeUtil_IsSubClassOfMValue_Full();
    testTypeUtil_PointerAndInheritance();
    testTypeUtil_NullptrType();
    testTypeUtil_IsSubClassOf_BoundaryRules();
}

// ============================================================
// 主入口（覆盖前面重复定义的入口）
// ============================================================
void TypeUtilityTest::test()
{
    testTypeUtil_IsType_Same();
    testTypeUtil_IsType_Different();
    testTypeUtil_IsType_WithConst();
    testTypeUtil_IsType_UserDefined();
    testTypeUtil_IsSubClass_DirectChild();
    testTypeUtil_IsSubClass_Transitive();
    testTypeUtil_IsSubClass_SameClass();
    testTypeUtil_IsSubClass_NotRelated();
    testTypeUtil_IsSubClass_Poodle();
    testTypeUtil_IsPODType_True();
    testTypeUtil_IsPODType_False();
    testTypeUtil_IsSignedInteger_True();
    testTypeUtil_IsSignedInteger_False();
    testTypeUtil_IsUnsignedInteger_True();
    testTypeUtil_IsUnsignedInteger_False();
    testTypeUtil_IsPodDirect();
    testTypeUtil_IsPointer();
    testTypeUtil_IsPodOrPointer();
    testTypeUtil_IsNotPodAndPointer();
    testTypeUtil_IsSubClassOfDirect();
    testTypeUtil_RuntimeComposite_Pod();
    testTypeUtil_RuntimeComposite_Signed();
    testTypeUtil_RuntimeComposite_Unsigned();
    testTypeUtil_RuntimeComposite_Hierarchy();
    testTypeUtil_IsType_EnumType();
    testTypeUtil_IsPODType_Enum();
    testTypeUtil_ComplexSignedChain();
    testTypeUtil_PointerVsNot();
    testTypeUtil_IsPodNotFloat();
    testTypeUtil_BoolNotSignedOrUnsigned();
    testTypeUtil_IsSubClass_ClassObjectHierarchy();
    testTypeUtil_IsPodOfAllFrameBasicTypes();
    testTypeUtil_IsType_PodAndNonPodMix();
    testTypeUtil_ExtendedAllPodPairs();
	testExtended();
}


#define TUTE_ASSERT(expr) \
	if (!(expr)) { ERROR(string("TypeUtilityTest FAILED: ") + (#expr)); }

// Helper base and derived class for isSubClass tests
struct TuteBase {};
struct TuteDerived : public TuteBase {};
struct TuteUnrelated {};

// ========== isType ==========

void TypeUtilityTest::testTUTE_IsTypeBasic()
{
	TUTE_ASSERT((isType<int, int>()));
	TUTE_ASSERT((isType<float, float>()));
	TUTE_ASSERT((isType<bool, bool>()));
	TUTE_ASSERT((isType<double, double>()));
	TUTE_ASSERT((isType<llong, llong>()));
}

void TypeUtilityTest::testTUTE_IsTypeWithConst()
{
	TUTE_ASSERT((isType<const int, int>()));
	TUTE_ASSERT((isType<const float, float>()));
	TUTE_ASSERT((isType<const bool, bool>()));
}

void TypeUtilityTest::testTUTE_IsTypeWithRef()
{
	TUTE_ASSERT((isType<int&, int>()));
	TUTE_ASSERT((isType<const int&, int>()));
	TUTE_ASSERT((isType<float&, float>()));
}

void TypeUtilityTest::testTUTE_IsTypeDifferent()
{
	TUTE_ASSERT(!(isType<int, float>()));
	TUTE_ASSERT(!(isType<int, llong>()));
	TUTE_ASSERT(!(isType<float, double>()));
	TUTE_ASSERT(!(isType<bool, int>()));
}

// ========== isSubClass ==========

void TypeUtilityTest::testTUTE_IsSubClassTrue()
{
	TUTE_ASSERT((isSubClass<TuteBase, TuteDerived>()));
}

void TypeUtilityTest::testTUTE_IsSubClassFalse()
{
	TUTE_ASSERT(!(isSubClass<TuteBase, TuteUnrelated>()));
	TUTE_ASSERT(!(isSubClass<TuteDerived, TuteBase>()));
	TUTE_ASSERT(!(isSubClass<TuteUnrelated, TuteDerived>()));
}

// ========== isPODType ==========

void TypeUtilityTest::testTUTE_IsPODTypeTrue()
{
	TUTE_ASSERT((isPODType<int>()));
	TUTE_ASSERT((isPODType<float>()));
	TUTE_ASSERT((isPODType<double>()));
	TUTE_ASSERT((isPODType<bool>()));
	TUTE_ASSERT((isPODType<char>()));
	TUTE_ASSERT((isPODType<byte>()));
	TUTE_ASSERT((isPODType<short>()));
	TUTE_ASSERT((isPODType<ushort>()));
	TUTE_ASSERT((isPODType<uint>()));
	TUTE_ASSERT((isPODType<llong>()));
}

void TypeUtilityTest::testTUTE_IsPODTypeFalse()
{
	TUTE_ASSERT(!(isPODType<string>()));
	TUTE_ASSERT(!(isPODType<TuteBase>()));
	TUTE_ASSERT(!(isPODType<TuteDerived>()));
}

// ========== isSignedInteger ==========

void TypeUtilityTest::testTUTE_IsSignedIntegerTrue()
{
	TUTE_ASSERT((isSignedInteger<int>()));
	TUTE_ASSERT((isSignedInteger<llong>()));
	TUTE_ASSERT((isSignedInteger<short>()));
	TUTE_ASSERT((isSignedInteger<char>()));
}

void TypeUtilityTest::testTUTE_IsSignedIntegerFalse()
{
	TUTE_ASSERT(!(isSignedInteger<uint>()));
	TUTE_ASSERT(!(isSignedInteger<ushort>()));
	TUTE_ASSERT(!(isSignedInteger<byte>()));
	TUTE_ASSERT(!(isSignedInteger<float>()));
	TUTE_ASSERT(!(isSignedInteger<double>()));
}

// ========== isUnsignedInteger ==========

void TypeUtilityTest::testTUTE_IsUnsignedIntegerTrue()
{
	TUTE_ASSERT((isUnsignedInteger<uint>()));
	TUTE_ASSERT((isUnsignedInteger<ushort>()));
	TUTE_ASSERT((isUnsignedInteger<byte>()));
}

void TypeUtilityTest::testTUTE_IsUnsignedIntegerFalse()
{
	TUTE_ASSERT(!(isUnsignedInteger<int>()));
	TUTE_ASSERT(!(isUnsignedInteger<llong>()));
	TUTE_ASSERT(!(isUnsignedInteger<short>()));
	TUTE_ASSERT(!(isUnsignedInteger<float>()));
	TUTE_ASSERT(!(isUnsignedInteger<double>()));
}

// ========== mixed primitive types ==========

void TypeUtilityTest::testTUTE_IsTypeMixedPrimitive()
{
	TUTE_ASSERT((isType<uint, uint>()));
	TUTE_ASSERT((isType<ushort, ushort>()));
	TUTE_ASSERT((isType<byte, byte>()));
	TUTE_ASSERT(!(isType<uint, int>()));
	TUTE_ASSERT(!(isType<ushort, short>()));
	TUTE_ASSERT(!(isType<byte, char>()));
}

void TypeUtilityTest::testExtended()
{
	testTUTE_IsTypeBasic();
	testTUTE_IsTypeWithConst();
	testTUTE_IsTypeWithRef();
	testTUTE_IsTypeDifferent();
	testTUTE_IsSubClassTrue();
	testTUTE_IsSubClassFalse();
	testTUTE_IsPODTypeTrue();
	testTUTE_IsPODTypeFalse();
	testTUTE_IsSignedIntegerTrue();
	testTUTE_IsSignedIntegerFalse();
	testTUTE_IsUnsignedIntegerTrue();
	testTUTE_IsUnsignedIntegerFalse();
	testTUTE_IsTypeMixedPrimitive();
}
#endif
