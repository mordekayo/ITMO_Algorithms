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
    
    void Init(int InBlockSize, int InBlocksPerPage);

    void Destroy();

    void* Alloc();

    void Free(void* Block) const;

#ifdef _DEBUG
    
    void DumpStat() const;

    void DumpBlocks() const;

#endif

private:

    FSAMemoryPage* PageListHead = nullptr;
    
    [[nodiscard]] FSAMemoryPage* AllocPage() const;

    int BlockSize = 16;
    int BlocksPerPage = 10;
};
