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

    bool Free(void* Block);

    void CheckValid() const;

#ifdef _DEBUG
    
    void DumpStat() const;

    void DumpBlocks() const;

#endif

private:

    FSAMemoryPage* PageListHead = nullptr;
    
    [[nodiscard]] FSAMemoryPage* AllocPage() const;

    int BlockSize = 16;
    int BlocksPerPage = 10;

#ifdef _DEBUG
    
    size_t AllocCalls = 0;
    size_t FreeCalls = 0;

    static constexpr char LeftDebugFlag = 0x007E;
    static constexpr char RightDebugFlag = 0x007C;
#endif
};
