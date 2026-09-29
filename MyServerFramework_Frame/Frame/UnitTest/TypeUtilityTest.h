#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API TypeUtilityTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testTypeUtil_IsType_Same();
	static void testTypeUtil_IsType_Different();
	static void testTypeUtil_IsType_WithConst();
	static void testTypeUtil_IsType_UserDefined();
	static void testTypeUtil_IsSubClass_DirectChild();
	static void testTypeUtil_IsSubClass_Transitive();
	static void testTypeUtil_IsSubClass_SameClass();
	static void testTypeUtil_IsSubClass_NotRelated();
	static void testTypeUtil_IsSubClass_Poodle();
	static void testTypeUtil_IsPODType_True();
	static void testTypeUtil_IsPODType_False();
	static void testTypeUtil_IsSignedInteger_True();
	static void testTypeUtil_IsSignedInteger_False();
	static void testTypeUtil_IsUnsignedInteger_True();
	static void testTypeUtil_IsUnsignedInteger_False();
	static void testTypeUtil_IsPodDirect();
	static void testTypeUtil_IsPointer();
	static void testTypeUtil_IsPodOrPointer();
	static void testTypeUtil_IsNotPodAndPointer();
	static void testTypeUtil_IsSubClassOfDirect();
	static void testTypeUtil_RuntimeComposite_Pod();
	static void testTypeUtil_RuntimeComposite_Signed();
	static void testTypeUtil_RuntimeComposite_Unsigned();
	static void testTypeUtil_RuntimeComposite_Hierarchy();
	static void testTypeUtil_IsType_EnumType();
	static void testTypeUtil_IsPODType_Enum();
	static void testTypeUtil_ComplexSignedChain();
	static void testTypeUtil_PointerVsNot();
	static void testTypeUtil_IsPodNotFloat();
	static void testTypeUtil_BoolNotSignedOrUnsigned();
	static void testTypeUtil_IsSubClass_ClassObjectHierarchy();
	static void testTypeUtil_IsPodOfAllFrameBasicTypes();
	static void testTypeUtil_IsType_PodAndNonPodMix();
	static void testTypeUtil_IsType_AllPodPairs();
	static void testTypeUtil_IsType_StructTypes();
	static void testTypeUtil_IsType_ScopedEnum();
	static void testTypeUtil_IsSubClass_3Level();
	static void testTypeUtil_IsSubClass_Self();
	static void testTypeUtil_IsSubClass_CrossBranch();
	static void testTypeUtil_IsPODType_AllBasic();
	static void testTypeUtil_IsSignedInteger_Exhaustive();
	static void testTypeUtil_IsUnsignedInteger_Exhaustive();
	static void testTypeUtil_IsPodMValue_AllTypes();
	static void testTypeUtil_IsPointerMValue_Various();
	static void testTypeUtil_IsSubClassOfMValue_Full();
	static void testTypeUtil_PointerAndInheritance();
	static void testTypeUtil_NullptrType();
	static void testTypeUtil_IsSubClassOf_BoundaryRules();
	static void testTypeUtil_ExtendedAllPodPairs();
	static void testExtended();
	static void testTUTE_IsTypeBasic();
	static void testTUTE_IsTypeWithConst();
	static void testTUTE_IsTypeWithRef();
	static void testTUTE_IsTypeDifferent();
	static void testTUTE_IsSubClassTrue();
	static void testTUTE_IsSubClassFalse();
	static void testTUTE_IsPODTypeTrue();
	static void testTUTE_IsPODTypeFalse();
	static void testTUTE_IsSignedIntegerTrue();
	static void testTUTE_IsSignedIntegerFalse();
	static void testTUTE_IsUnsignedIntegerTrue();
	static void testTUTE_IsUnsignedIntegerFalse();
	static void testTUTE_IsTypeMixedPrimitive();
#endif
};
