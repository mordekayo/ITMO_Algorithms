#pragma once
#include "../ProtoAllocator.h"


class FixedSizeAllocator
{
private:
    struct FSAMemoryPage
    {
        FSAMemoryPage* NextPage = nullptr;

        void* Buffer = nullptr;
        int FreeListHead = 0;
    };
public:

    FixedSizeAllocator();

    ~FixedSizeAllocator();

    //We cannot copy allocators
    FixedSizeAllocator(const FixedSizeAllocator&) = delete;
    FixedSizeAllocator& operator=(const FixedSizeAllocator&) = delete;
    FixedSizeAllocator(FixedSizeAllocator&&) = delete;
    FixedSizeAllocator& operator=(const FixedSizeAllocator&&) = delete;
    
    void Init(size_t InBlockSize, size_t InBlocksPerPage);

    void Destroy();

    void* Alloc(size_t Size);

    void Free(void* p);

#ifdef _DEBUG
    
    void DumpStat() const;

    void DumpBlocks() const;

#endif

private:

    FSAMemoryPage* PageListHead = nullptr;
    
    [[nodiscard]] FSAMemoryPage* AllocPage() const;

    size_t BlockSize = 16;
    size_t BlocksPerPage = 10;
};
