#pragma once

#include "FrameDefine.h"

// 是否为大于等于0的数
template<int N>
struct MICRO_LEGEND_FRAME_API IsPositive
{
	static constexpr bool mValue = N > 0;
	using mType = enable_if_t<N >= 0>;
};

// 判断是否为基础数据类型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsPod { static constexpr bool mValue = false; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<bool> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<char> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<byte> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<short> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<ushort> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<int> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<uint> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<long> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<ulong> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<llong> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<ullong> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<float> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPod<double> { static constexpr bool mValue = true; };

template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodType { using mType = enable_if_t<IsPod<typename remove_cv<T>::type>::mValue || is_enum<T>::value>; };

// 是否为指针类型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsPointer
{
	static constexpr bool mValue = false;
};

template<typename T>
struct MICRO_LEGEND_FRAME_API IsPointer<T*>
{
	static constexpr bool mValue = true;
};

// 是否为指针或者基础数据类型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodOrPointerType 
{
	static constexpr bool mValue = IsPod<T>::mValue || IsPointer<T>::mValue;
	using mType = enable_if_t<mValue>; 
};

// 是否不是指针和基础数据类型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsNotPodAndPointerType
{
	static constexpr bool mValue = !IsPod<T>::mValue && !IsPointer<T>::mValue;
	using mType = enable_if_t<mValue>;
};

// 是否为整型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodInteger { static constexpr bool mValue = false; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<bool> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<char> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<byte> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<short> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<ushort> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<int> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<uint> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<long> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<ulong> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<llong> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodInteger<ullong> { static constexpr bool mValue = true; };

template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodIntegerType { using mType = enable_if_t<IsPodInteger<T>::mValue>; };

// 是否为带符号整型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodSignedInteger { static constexpr bool mValue = false; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodSignedInteger<char> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodSignedInteger<short> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodSignedInteger<int> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodSignedInteger<long> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodSignedInteger<llong> { static constexpr bool mValue = true; };

template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodSignedIntegerType { using mType = enable_if_t<IsPodSignedInteger<T>::mValue>; };

// 是否为无符号整型
template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedInteger { static constexpr bool mValue = false; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedInteger<byte> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedInteger<ushort> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedInteger<uint> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedInteger<ulong> { static constexpr bool mValue = true; };

template<>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedInteger<ullong> { static constexpr bool mValue = true; };

template<typename T>
struct MICRO_LEGEND_FRAME_API IsPodUnsignedIntegerType { using mType = enable_if_t<IsPodUnsignedInteger<T>::mValue>; };