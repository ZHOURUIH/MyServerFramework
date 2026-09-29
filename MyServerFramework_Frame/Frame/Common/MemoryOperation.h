//#pragma once
//
//#include "MemoryTracker.h"
//#include <cstdlib>
//
//#ifdef _MEMORY_TRACK
// // 仅在需要检测内存分配或者内存泄漏时才会打开注释启用代码,因为打开后会导致程序无法正常退出,以及运行效率大幅降低等问题
//void* operator new(size_t size, const char* file, int line)
//{
//    void* p = malloc(size);
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterAlloc(p, size, file, line);
//    }
//    return p;
//}
//
//void* operator new[](size_t size, const char* file, int line)
//{
//    void* p = malloc(size);
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterAlloc(p, size, file, line);
//    }
//    return p;
//}
//
//void operator delete(void* p, const char*, int) noexcept
//{
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterFree(p);
//    }
//    free(p);
//}
//
//void operator delete[](void* p, const char*, int) noexcept
//{
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterFree(p);
//    }
//    free(p);
//}
//
//// normal new/delete
//void* operator new(size_t size)
//{
//    void* p = malloc(size);
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterAlloc(p, size, "unknown", 0);
//    }
//    return p;
//}
//
//void* operator new[](size_t size)
//{
//    void* p = malloc(size);
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterAlloc(p, size, "unknown", 0);
//    }
//    return p;
//}
//
//void operator delete(void* p) noexcept
//{
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterFree(p);
//    }
//    free(p);
//}
//
//void operator delete[](void* p) noexcept
//{
//    if (MemoryTracker::Get() != nullptr)
//    {
//        MemoryTracker::Get()->RegisterFree(p);
//    }
//    free(p);
//}
//
//#define new new(__FILE__, __LINE__)
//#endif