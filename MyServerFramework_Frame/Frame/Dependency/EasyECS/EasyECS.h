#pragma once
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#ifdef _WIN32
#include <Windows.h>
#undef max
#undef min
#endif

#define ECS()
#define NOT_ECS()

#if defined(_MSC_VER)
#define EASY_ECS_FORCE_INLINE __forceinline
#define EASY_ECS_NO_INLINE __declspec(noinline)
#define EASY_ECS_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
#define EASY_ECS_FORCE_INLINE inline __attribute__((always_inline))
#define EASY_ECS_NO_INLINE __attribute__((noinline))
#define EASY_ECS_RESTRICT __restrict__
#else
#define EASY_ECS_FORCE_INLINE inline
#define EASY_ECS_NO_INLINE
#define EASY_ECS_RESTRICT
#endif

constexpr size_t EASY_ECS_MEMORY_ALIGNMENT = 64;
namespace EasyECSRuntime
{
inline constexpr const char* getVersion() noexcept
{
	return "0.5.0";
}
EASY_ECS_FORCE_INLINE size_t alignUp(size_t value, size_t alignment)
{
	return (value + alignment - 1) & ~(alignment - 1);
}
EASY_ECS_FORCE_INLINE uint8_t* alignPointer(void* memory, size_t alignment)
{
	uintptr_t value = reinterpret_cast<uintptr_t>(memory);
	value = (value + alignment - 1) & ~(alignment - 1);
	return reinterpret_cast<uint8_t*>(value);
}
}
