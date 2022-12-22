#pragma once

class FixedSizeAllocator
{
private:
    struct FSAMemoryPage
    {
        FSAMemoryPage* NextPage = nullptr;
        int FreeListHead = 0;
        int InitizalizedBlocks = 0;
        void* Buffer = nullptr;
    };
public:

    FixedSizeAllocator() = default;

    ~FixedSizeAllocator();

    //We cannot copy allocators
    FixedSizeAllocator(const FixedSizeAllocator&) = delete;
    FixedSizeAllocator& operator=(const FixedSizeAllocator&) = delete;
    FixedSizeAllocator(FixedSizeAllocator&&) = delete;
    FixedSizeAllocator& operator=(const FixedSizeAllocator&&) = delete;
    
    void Init(int InBlockSize, int InBlocksPerPage);

    void Destroy();

    void* Alloc();

    void Free(void* Block);

#ifdef _DEBUG
    
    void DumpStat() const;

    void DumpBlocks() const;

    void CheckValid() const;   

#endif

private:

    FSAMemoryPage* PageListHead = nullptr;
    
    [[nodiscard]] FSAMemoryPage* AllocPage() const;

    int BlockSize = 16;
    int BlocksPerPage = 10;

#ifdef _DEBUG
    
    size_t AllocCalls = 0;
    size_t FreeCalls = 0;

#endif
};
