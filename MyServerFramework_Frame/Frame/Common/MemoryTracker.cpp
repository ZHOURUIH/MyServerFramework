#include "FrameHeader.h"

MemoryTracker* MemoryTracker::mInstance = nullptr;

void MemoryTracker::RegisterAlloc(void* ptr, size_t size, const char* file, int line)
{
    if (!IsMainThread() || ptr == nullptr || mIsSelfAlloc)
    {
        return;
    }

    mIsSelfAlloc = true;

    size_t h = Hash(ptr);
    AllocNode* node = (AllocNode*)malloc(sizeof(AllocNode));
    node->ptr = ptr;
    node->size = size;
    node->file = file;
    node->line = line;
    node->next = mBuckets[h];
    mBuckets[h] = node;

    mTotalAllocated += size;
    mIsSelfAlloc = false;
}

void MemoryTracker::RegisterFree(void* ptr)
{
    if (!IsMainThread() || ptr == nullptr || mIsSelfAlloc)
    {
        return;
    }

    mIsSelfAlloc = true;

    size_t h = Hash(ptr);
    AllocNode** pp = &mBuckets[h];
    AllocNode* node = mBuckets[h];
    while (node)
    {
        if (node->ptr == ptr)
        {
            *pp = node->next;
            mTotalAllocated -= node->size;
            free(node);
            break;
        }
        pp = &node->next;
        node = node->next;
    }

    mIsSelfAlloc = false;
}

void MemoryTracker::DumpReport(const char* filename)
{
#ifdef _MEMORY_TRACK
    createDir(getFilePath(filename));
    mIsSelfAlloc = true;
    FILE* fp = nullptr;
    fopen_s(&fp, filename, "w");
    if (fp == nullptr)
    {
        mIsSelfAlloc = false;
        return;
    }

    fprintf(fp, "Total Allocated: %llu MB\n", mTotalAllocated / 1024 / 1024);
    std::unordered_map<const char*, std::unordered_map<int, unsigned long long>> fileList;
    for (size_t i = 0; i < HASH_SIZE; ++i)
    {
        AllocNode* node = mBuckets[i];
        while (node != nullptr)
        {
            fileList[node->file][node->line] += node->size;
            node = node->next;
        }
    }
    for (const auto& iter0 : fileList)
    {
        for (const auto& iter1 : iter0.second)
        {
            if (iter1.second > 1024 * 1024)
            {
                fprintf(fp, "file:%s:%d, size:%dMB\n", iter0.first, iter1.first, (int)(iter1.second / 1024 / 1024));
            }
        }
    }

    fclose(fp);
    mIsSelfAlloc = false;
#endif
}